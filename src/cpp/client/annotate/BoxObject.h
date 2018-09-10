//
// Created by phil on 09/08/2018.
//

#ifndef BOXOBJECT_H
#define BOXOBJECT_H


#include <common/Common.hpp>
#include <utility> #include "Object.h"

struct BoxObject : Object {
    float height, width, depth;
    Eigen::Vector3f offset;

    BoxObject(msr::airlib::Pose &pose,
              float height, float width, float depth,
              const std::string &name = "Object",
              Eigen::Vector3f offset = Eigen::Vector3f({0, 0, 0})) :
            Object(pose, 8, name),
            height(height),
            width(width),
            depth(depth),
            offset(std::move(offset)) { ; }

    /**
     * Transforms corners to matrix.
     * @return matrix(4,8) with corner coordinates in homogeneous NED, object space.
     */
    Eigen::Matrix4Xf toMat() {
        Eigen::Matrix<float, 4, 8> mat;

        Eigen::Vector4f offset_hom;
        offset_hom << offset, 0;

        mat <<
            -depth, depth, -depth, depth, -depth, depth, -depth, depth,
                -width, -width, width, width, -width, -width, width, width,
                -height, -height, -height, -height, height, height, height, height,
                1, 1, 1, 1, 1, 1, 1, 1;

        mat.colwise() += offset_hom;
        return mat;
    }
};


#endif //GATE_H
