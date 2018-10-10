//
// Created by phil on 10/10/2018.
//

#ifndef DATAGEN_ANNOTATORFLIGHT_H
#define DATAGEN_ANNOTATORFLIGHT_H


#include <sim/Simulation.h>
#include "Annotator.h"

class AnnotatorFlight: public SimEventHandler{
public:
    int t_log_ms;
    Annotator *ann;
    void newImage(cv::Mat &mat,msr::airlib::Pose &pose) override;
    void newSensorData(mav::Kinematics &kin) override;
};


#endif //DATAGEN_ANNOTATORFLIGHT_H
