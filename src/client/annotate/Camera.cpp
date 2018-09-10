//
// Created by phil on 09/08/2018.
//

#include "Camera.h"
using namespace Eigen;

Eigen::Matrix3Xf
Camera::
projectToImagePlane(Object *obj) {
    auto X_view = transformToView(obj);

    auto X_eun = transformCoordinateSystem(X_view);

    Matrix3Xf X_projected = getIntrinsicMat();
    X_projected *= X_eun;

    auto X_image_plane = normalize2ImagePlane(X_projected);

    return X_image_plane;
}

Eigen::Matrix3Xf
Camera::
normalize2ImagePlane(Eigen::Matrix3Xf X) {
    MatrixXf X_norm(3, X.cols());

    for (int i = 0; i < X.cols(); i++) {

        float z_p = X(2, i);

        X_norm(0, i) = X(0, i) / z_p;
        X_norm(1, i) = X(1, i) / z_p;
        X_norm(2, i) = z_p;

    }

    return X_norm;
}

Eigen::Matrix3Xf
Camera::
transformCoordinateSystem(Eigen::Matrix4Xf X_ned) {
    MatrixXf X_eun(3, X_ned.cols());

    for (int i = 0; i < X_ned.cols(); i++) {

        X_eun(0, i) = X_ned(1, i);
        X_eun(1, i) = -X_ned(2, i);
        X_eun(2, i) = X_ned(0, i);

    }

    return X_eun;
}

Eigen::Matrix<float, 4, 4>
Camera::
getViewMatrix()
{
    Matrix<float, 3,3> R = pose.orientation.toRotationMatrix().inverse();
    auto t = R*(-1*pose.position);

    Matrix<float, 3,4> V;
    V << R,t;

    Matrix<float, 4,4> V_hom;
    V_hom << V,
            0,0,0,1;

    return V_hom;
}

Eigen::Matrix<float, 3, 3>
Camera::
getIntrinsicMat()
{
    float cx = (float) width / 2;
    float cy=(float) height / 2;

    Matrix<float,3,3> C;
    C <<    fx, 0,  cx,
            0, fy, cy,
            0, 0,   1;
    return C;
}


Eigen::Matrix4Xf
Camera::
transformToWorld(Object *obj) {
    auto R = obj->pose.orientation.toRotationMatrix();
    auto t = obj->pose.position;

    Eigen::Matrix<float, 3, 4> T_obj;
    T_obj << R, t;

    Eigen::Matrix<float, 4, 4> T_obj_hom;
    T_obj_hom << T_obj,
            0, 0, 0, 1;

    Matrix4Xf worldMat = T_obj_hom;
    worldMat *= obj->toMat();
    return worldMat;
}

Eigen::Matrix4Xf
Camera::
transformToView(Object *obj) {
    Eigen::Matrix4Xf X = getViewMatrix();
    X *= transformToWorld(obj);
    return X;
};