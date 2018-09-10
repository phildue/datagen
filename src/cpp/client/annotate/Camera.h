//
// Created by phil on 09/08/2018.
//

#ifndef CAMERA_H
#define CAMERA_H


#include <common/Common.hpp>
#include "Gate.h"
class Camera{

public:

    /**
     * Constructor
     * @param fov field of view in radians
     * @param width in pixels
     * @param height in pixels
     */
    explicit Camera(float fov=(float)M_PI/2,
                    int width=416,
                    int height=416):
            fx(width/(2*tan(fov/2))),
            fy(fx),
            width(width),
            height(height),
            pose(msr::airlib::Pose()){}

    Camera(const Camera &that):fx(that.fx),fy(that.fy),width(that.width),height(that.height){
        this->pose = that.pose;
    }

    /**
     * Transforms object to view space.
     * @param obj Object with according pose
     * @return matrix(3,*) containing the 3D corner coordinates in NED.
     */
    Eigen::Matrix4Xf transformToView(Object *obj);

    /**
     * Projects object to image plane.
     * @param gate Gate with according pose.
     * @return matrix(3,5) containing the 2D corner points normalized to image plane in eun, with n unnormalized.
     */
    Eigen::Matrix3Xf projectToImagePlane(Object *obj);
    /**
     * @return intrinsic camera matrix
     */
    Eigen::Matrix<float, 3,3> getIntrinsicMat();

    const float fx,fy;
    const int width,height;
    msr::airlib::Pose pose;
protected:


    /**
     * Returns ViewMatrix of camera that transforms object coordinates to camera space.
     * @return view matrix in homogeneous coordinates ENU.
     */
    Eigen::Matrix<float,4,4> getViewMatrix();

    /**
     * Maps between coordinate systems North-East-Down and East-Up-North.
     * @param X_ned NED homogeneous
     * @return ENU
     */
    Eigen::Matrix3Xf transformCoordinateSystem(Eigen::Matrix4Xf X_ned);

    /**
     * Normalizes X to image plane (by z)
     * @param X corner points after projection
     * @return X normalized
     */
    Eigen::Matrix3Xf normalize2ImagePlane(Eigen::Matrix3Xf X);

    /**
     * Transforms corner coordinates to world space.
     * @return matrix(4,*) with corner coordinates in homogeneous NED, world space.
     */
    virtual Eigen::Matrix4Xf transformToWorld(Object *obj);
};



#endif //CAMERA_H
