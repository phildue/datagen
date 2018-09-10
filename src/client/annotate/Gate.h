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

struct Gate : public Object {
    const float width,height,lowBar,highBar,heightCenter;


    explicit Gate(msr::airlib::Pose &pose):
            width(GATE_W),
            highBar(GATE_HH),
            lowBar(GATE_HL),
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
};


#endif //GATE_H
