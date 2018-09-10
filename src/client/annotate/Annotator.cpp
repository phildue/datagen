//
// Created by phil on 09/08/2018.
//

#include <opencv2/highgui.hpp>
#include "Annotator.h"

ImageLabel
Annotator::
createLabel(msr::airlib::Pose &campose) {
    /**
     * For each object we calculate its relative pose to the camera, as well as the view projection.
     * If the object is visible in the image, we add the object label to the image label
     */
    cam.pose = campose;
    ImageLabel imageLabel{};
    for (Object *object : objects) {

        ObjectLabel label{};
        label.name = object->name;
        /* 3D coordinates in camera space */
        auto obj_position_cspace = campose.orientation.toRotationMatrix() * object->pose.position;
        auto cam_position_cspace = campose.orientation.toRotationMatrix() * campose.position;
        label.north = obj_position_cspace[0] - cam_position_cspace[0];
        label.east = obj_position_cspace[1] - cam_position_cspace[1];
        label.down = obj_position_cspace[2] - cam_position_cspace[2];

        /* 2D coordinates image plane with (0,0) being the lower left corner*/
        auto X_p = cam.projectToImagePlane(object);


        int visible_corners = 0;

        for (int i_corners = 0; i_corners < object->n_corners; i_corners++) {

            int x = (int) X_p(0, i_corners);
            int y = (int) X_p(1, i_corners);
            int z = (int) X_p(2, i_corners);
            label.addCorner(x, y);
            if (z > 0
                && 0 < x && x < cam.width
                && 0 < y && y < cam.height) {

                visible_corners ++;
            }
        }
        if(visible_corners >= 2){
            imageLabel.add(label);
        }

    }
    return imageLabel;
}

cv::Mat
Annotator::
annotate(cv::Mat &mat, ImageLabel &label) {
    for (int i = 0; i < label.n_objects; i++) {
        auto object = label.objects[i];
        for (int j = 0; j < object.n_corners; j++) {
            float x_p = object.x[j];
            float y_p = object.y[j];

            cv::circle(mat, cv::Point(static_cast<int>(x_p), mat.rows - static_cast<int>(y_p)), 3,
                       cv::Scalar(0, 255, 0));

        }
    }
    return mat;
}

void
Annotator::
write(cv::Mat &mat, ImageLabel &label,std::string &filename) {
    std::ostringstream labelFileName,imgFileName;
    labelFileName << filename << ".xml";
    imgFileName << filename << ".jpg";
    std::ofstream myfile;
    myfile.open(labelFileName.str());
    auto imgFilenameStr = imgFileName.str();
    myfile << label.toXml(imgFilenameStr);
    myfile.close();

    cv::imwrite(imgFileName.str(),mat);

}

void
Annotator::
write(cv::Mat &mat, ImageLabel &label) {
    std::stringstream name;
    name << path << "/" << std::setfill('0') << std::setw(5) << index++;
    auto nameStr = name.str();
    Annotator::write(mat,label,nameStr);
}

  