//
// Created by phil on 09/08/2018.
//

#ifndef OBJECT_H
#define OBJECT_H


#include <common/Common.hpp>
#include "Labels.h"

#ifdef INDUSTRIAL_ROOM
#define GATE_W 3.3f
#define GATE_HH 6.8
#define GATE_HL 3.3f
#else
#define GATE_W 2.6f
#define GATE_HH 3.15f
#define GATE_HL 0.45f
#endif

struct Object {
    msr::airlib::Pose pose;
    const int n_corners;
    std::string name;

    Object(msr::airlib::Pose &pose, const int n_corners, const std::string &name) :
            pose(pose), n_corners(n_corners), name(name) { ; }

    /**
     * Transforms corners to matrix.
     * @return matrix(4,*) with corner coordinates in homogeneous NED, object space.
     */
    virtual Eigen::Matrix4Xf toMat() = 0;

    virtual ObjectLabel* asLabel(){
        return new ObjectLabel();
    }

};



#endif //OBJECT_H
