//
// Created by phil on 09/08/2018.
//

#include <opencv2/highgui.hpp>
#include "Annotator.h"
#include "Labels.h"

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

        ObjectLabel *label = object->asLabel();

        label->name = object->name;
        /* 3D coordinates in camera space */
        auto obj_position_cspace = campose.orientation.toRotationMatrix() * object->pose.position;
        auto cam_position_cspace = campose.orientation.toRotationMatrix() * campose.position;

        label->north = obj_position_cspace[0] - cam_position_cspace[0];
        label->east = obj_position_cspace[1] - cam_position_cspace[1];
        label->down = obj_position_cspace[2] - cam_position_cspace[2];

        float pitchCam,rollCam,yawCam,pitchObj,rollObj,yawObj;
        toEulerianAngle(campose.orientation,&pitchCam,&rollCam,&yawCam);
        toEulerianAngle(object->pose.orientation,&pitchObj,&rollObj,&yawObj);
        label->yaw = yawObj - yawCam;
        label->pitch = pitchObj - pitchCam;
        label->roll = rollObj - rollCam;

        /* 2D coordinates image plane with (0,0) being the lower left corner*/
        auto X_p = cam.projectToImagePlane(object);


        int visible_corners = 0;
        for (int i_corners = 0; i_corners < object->n_corners; i_corners++) {

            int x = (int) X_p(0, i_corners);
            int y = (int) X_p(1, i_corners);
            int z = (int) X_p(2, i_corners);
            label->addCorner(x, y);
            if (z > 0
                && 0 < x && x < cam.width
                && 0 < y && y < cam.height) {

                visible_corners ++;
            }
        }
        if(visible_corners >= 2){

//            bool overlap = false;
//            float area_i = label->width()*label->height();
//            for (ObjectLabel *obj_k : imageLabel.objects){
//                float overlapArea = label->overlapX(obj_k)*label->overlapY(obj_k);
//                float area_k = obj_k->width()*obj_k->height();
//                if (overlapArea > 0.5 *area_i || overlapArea > 0.5*area_k){
//                    overlap = true;
//                    if (area_i > area_k){
//                        obj_k = label;
//                    }
//                }
//            }
//            if(!overlap){
                imageLabel.objects.push_back(label);
//            }

        }

    }


    return imageLabel;
}

cv::Mat
Annotator::
annotate(cv::Mat &mat, ImageLabel &label) {
    for (ObjectLabel *obj : label.objects) {
        for (int j = 0; j < obj->n_corners; j++) {
            float x_p = obj->x[j];
            float y_p = obj->y[j];

            cv::circle(mat, cv::Point(static_cast<int>(x_p), mat.rows - static_cast<int>(y_p)), 3,
                       cv::Scalar(0, 255, 0));

        }
    }
    return mat;
}

void
Annotator::
write(cv::Mat &mat, ImageLabel &label,std::string &filename,const std::string &image_format) {
    std::ostringstream labelFileName,imgFileName;
    labelFileName << filename << ".xml";
    imgFileName << filename << "." << image_format;
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
    Annotator::write(mat,label,nameStr,image_format);
}

void Annotator::toEulerianAngle(Eigen::Quaternion<float, 2> q, float *pitch, float *roll, float *yaw) {
    //z-y-x rotation convention (Tait-Bryan angles)
    //http://www.sedris.org/wg8home/Documents/WG80485.pdf

    float ysqr = q.y() * q.y();

    // roll (x-axis rotation)
    float t0 = +2.0f * (q.w() * q.x() + q.y() * q.z());
    float t1 = +1.0f - 2.0f * (q.x() * q.x() + ysqr);
    *roll = std::atan2(t0, t1);

    // pitch (y-axis rotation)
    float t2 = +2.0f * (q.w() * q.y() - q.z() * q.x());
    t2 = ((t2 > 1.0f) ? 1.0f : t2);
    t2 = ((t2 < -1.0f) ? -1.0f : t2);
    *pitch = std::asin(t2);

    // yaw (z-axis rotation)
    float t3 = +2.0f * (q.w() * q.z() + q.x() * q.y());
    float t4 = +1.0f - 2.0f * (ysqr + q.z() * q.z());
    *yaw = std::atan2(t3, t4);
}

std::vector<ObjectLabel*> Annotator::filterOverlap( std::vector<ObjectLabel*> objects) {
    std::vector<ObjectLabel*> remaining;
    for (ObjectLabel *obj_i : objects){
        bool overlap = false;
        float area_i = obj_i->width()*obj_i->height();
        for (ObjectLabel *obj_k : remaining){
            float overlapArea = obj_i->overlapX(obj_k)*obj_i->overlapY(obj_k);
            float area_k = obj_k->width()*obj_k->height();
            overlap = true;
            if (overlapArea > 0.5 *area_i || overlapArea > 0.5*area_k){
                if (area_i > area_k){
                    obj_k = obj_i;
                }
            }
        }
        if(!overlap){
            remaining.push_back(obj_i);
        }
    }
    return remaining;
}

  