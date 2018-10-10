//
// Created by phil on 10/10/2018.
//

#include <opencv2/highgui.hpp>
#include "AnnotatorFlight.h"

void
AnnotatorFlight::
newImage(cv::Mat &mat, msr::airlib::Pose &pose)
{
    static long long lastFrame = getCurrentTimeMillis();
    long long currentTime = getCurrentTimeMillis();

    auto label = ann->createLabel(pose);

    if (currentTime - lastFrame > t_log_ms){
        ann->write(mat,label);
    }
    auto matAnn = Annotator::annotate(mat, label);
    cv::imshow("Image", matAnn);
    cv::waitKey(1);
}

void
AnnotatorFlight::
newSensorData(mav::Kinematics &kin)
{

}

