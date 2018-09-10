/**
 * AirsimInterface
 * An interface to airsim.
 */
#pragma once
#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#ifndef AIRSIM_INTERFACE_H
#define AIRSIM_INTERFACE_H
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include <iostream>
#include <chrono>
#include <Image.h>
#include <annotate/Gate.h>
#include "Session.h"

#define MAX_OBJS 20
class AirSimInterface
{
protected:
	Session *session;
    msr::airlib::MultirotorRpcLibClient *client;
public:
    AirSimInterface();
	~AirSimInterface();

	void connect();
	/**
	 * Update drone pose
	 * @param north
	 * @param east
	 * @param down
	 * @param roll
	 * @param pitch
	 * @param yaw
	 */
	void setDronePose(float north, float east, float down, float roll, float pitch, float yaw);

	/**
	 * Get camera image of front camera.
	 * @return image
	 */
	Image getImage();

	/**
	 * Get camera pose of front camera.
	 * @return pose in NED centered at the initial position of the drone
	 */
    Pose getCameraPose();

    /**
     * Get the poses of all objects in the environment.
     * @param max_gates maximum number of gates to search for
     * @return list of gate poses in NED centered at initial position of the drone
     */
    std::map<std::string, msr::airlib::Pose> getObjPoses(const std::string &name, int max_gates = MAX_OBJS);

    /**
     * Transform euler angles to quaternion
     * @param roll
     * @param pitch
     * @param yaw
     * @return
     */
    static Eigen::Quaternionf euler2Quat(float roll, float pitch, float yaw) ;
};


#endif