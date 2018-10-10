//
// Created by phil on 09/08/2018.
//

#ifndef GATE_H
#define GATE_H


#include <common/Common.hpp>
#include "Object.h"

#ifdef INDUSTRIAL_ROOM
#define GATE_W 3.3f
#define GATE_HH 6.8
#define GATE_HL 3.3f
#else
#define GATE_W 2.6f
#define GATE_HH 3.15f
#define GATE_HL 0.45f
#endif

struct Gate2017 : public Object {
    const float width,height,lowBar,highBar,heightCenter;


    explicit Gate2017(msr::airlib::Pose &pose):
            width(2.6),
            highBar(3.15),
            lowBar(0.45),
            height(highBar-lowBar),
            heightCenter(lowBar+(highBar-lowBar)/2),
            Object(pose, 5, "gate") {
    }

    /**
     * Transforms corners to matrix.
     * @return matrix(4,5) with corner coordinates in homogeneous NED, object space.
     */
    Eigen::Matrix4Xf toMat() {
        Eigen::Matrix<float, 4, 5> mat;

        mat <<
            0, 0, 0, 0, 0,
                -width * 0.5, width * 0.5, -width * 0.5, width * 0.5, 0,
                -lowBar, -highBar, -highBar, -lowBar, -heightCenter,
                1, 1, 1, 1, 1.0;
        return mat;
    }

    ObjectLabel* asLabel(){
        return new GateLabel();
    }
};

struct Gate2018 : public Object {
    const float width,height,lowBar,highBar,heightCenter;


    explicit Gate2018(msr::airlib::Pose &pose):
            width(3.6),
            highBar(7.4),
            lowBar(3.8),
            height(highBar-lowBar),
            heightCenter(lowBar+(highBar-lowBar)/2),
            Object(pose, 5, "gate") {
    }

    /**
     * Transforms corners to matrix.
     * @return matrix(4,5) with corner coordinates in homogeneous NED, object space.
     */
    Eigen::Matrix4Xf toMat() {
        Eigen::Matrix<float, 4, 5> mat;

        mat <<
            0, 0, 0, 0, 0,
                -width * 0.5, width * 0.5, -width * 0.5, width * 0.5, 0,
                -lowBar, -highBar, -highBar, -lowBar, -heightCenter,
                1, 1, 1, 1, 1.0;
        return mat;
    }

    ObjectLabel* asLabel(){
        return new GateLabel();
    }
};

#endif //GATE_H
