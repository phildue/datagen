//
// Created by phil on 10/09/2018.
//

#include <thread>
#include <iostream>
#include <MotionRead.h>
#include "TrajectoryPoseGenerator.h"

using namespace Eigen;
namespace mav{
void
TrajectoryPoseGenerator::
generate() {
    simClient.startSim();

    //takeOff
    auto sim = simClient.getSim();
    sim->setRefPosition(0.0, 0.0, height, 0);
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));
    sim->setRefPosition(0.0, 0.0, height, 0);
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(2000));

    switch (trajectory){
        case 0:
            trajectory1();
            break;
        case 1:
            trajectory2();
            break;
        case 2:
            trajectory3();
            break;
        default:
            std::cout << "Unknown Trajectory." << std::endl;

    }

    if (simClient.isRunning()) {
        simClient.stopSim();
        std::cout << "Press your favorite key to close.." << std::endl;
    }
    simClient.waitForTearDown();
}

void
TrajectoryPoseGenerator::
updateVelocity(float v_x, float v_y, float v_z, float yaw, double t) {
    auto R = Matrix<float, 3, 3>();
    R << cos(deltaYaw), -sin(deltaYaw), 0,
            sin(deltaYaw), cos(deltaYaw), 0,
            0, 0, 1;

    auto v = Vector3f({v_x, v_y, v_z});
    auto vr = R * v;

    float vr_x = vr[0];
    float vr_y = vr[1];
    float vr_z = vr[2];
    float heading = yaw + deltaYaw;

    std::cout << "v_x: " << vr_x << "v_y: " << vr_y << "heading: " << heading << std::endl;
    simClient.getSim()->setRefVelocity(vr_x, vr_y, vr_z, heading);
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(t));
}

void
TrajectoryPoseGenerator::
trajectory1() {
    float v_x = 0;
    float v_y = -0.92f;
    float v_z = -0.01f;
    float heading = 0;

    updateVelocity(v_x, v_y, v_z, heading, 1000);

    v_x = 1.0;
    v_y = 0;
    updateVelocity(v_x, v_y, v_z, heading, 8000);

    v_x = 0.0;
    v_y = 1.0;
    updateVelocity(v_x, v_y, v_z, heading, 3000);

    v_x = 1.0;
    v_y = 0.0;
    updateVelocity(v_x, v_y, v_z, heading, 14000);

    v_x = 0.0;
    v_y = 0.0;
    heading = 0.5f * M_PIf;
    updateVelocity(v_x, v_y, v_z, heading, 500);

    v_x = 0.0;
    v_y = 1.0;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = -1.0f;
    v_y = 0.0;
    heading = M_PIf;
    updateVelocity(v_x, v_y, v_z, heading, 15000);

    v_x = 0.0;
    v_y = -1.0f;
    heading = M_PIf;
    updateVelocity(v_x, v_y, v_z, heading, 3000);

    v_x = -1.0f;
    v_y = 0;
    heading = M_PIf;
    updateVelocity(v_x, v_y, v_z, heading, 6000);

    v_x = 0;
    v_y = 0;
    heading = 0;
    updateVelocity(v_x, v_y, v_z, heading, 1000);

    v_x = 1.0;
    v_y = 0;
    heading = 0;
    updateVelocity(v_x, v_y, v_z, heading, 6000);


    v_x = 0;
    v_y = 1.0;
    heading = 0;
    updateVelocity(v_x, v_y, v_z, heading, 3000);

    v_x = 1.0;
    v_y = 0;
    heading = 0;
    updateVelocity(v_x, v_y, v_z, heading, 15000);

    v_x = 0.0;
    v_y = -1.0f;
    heading = -0.5 * M_PIf;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = 0.0;
    v_y = 0.0;
    heading = M_PIf;
    updateVelocity(v_x, v_y, v_z, heading, 500);

    v_x = -1.0f;
    v_y = 0.0;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = 0.0;
    v_y = -1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 3000);

    v_x = -1.0f;
    v_y = 0;
    updateVelocity(v_x, v_y, v_z, heading, 11000);

}


void
TrajectoryPoseGenerator::
trajectory2() {
    float v_x = 0;
    float v_y = 1.0f;
    float v_z = -0.01f;
    float heading = M_PIf / 2;

    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = 1.0;
    v_y = 0;
    heading = -M_PIf / 2;
    updateVelocity(v_x, v_y, v_z, heading, 7000);

    v_x = 0.0;
    v_y = -1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = 1.0;
    v_y = 0;
    heading = M_PIf / 2;
    updateVelocity(v_x, v_y, v_z, heading, 4000);

    v_x = 0;
    v_y = 1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 10000);


    v_x = 1.0;
    v_y = 0;
    heading = -M_PIf / 2;
    updateVelocity(v_x, v_y, v_z, heading, 4000);

    v_x = 0.0;
    v_y = -1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = 1.0;
    v_y = 0;
    heading = M_PIf / 2;
    updateVelocity(v_x, v_y, v_z, heading, 4000);

    v_x = 0.0;
    v_y = 1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 12000);

    v_x = 1.0;
    v_y = 0;
    heading = -M_PIf / 2;
    updateVelocity(v_x, v_y, v_z, heading, 4000);

    v_x = 0.0;
    v_y = -1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

}


void
TrajectoryPoseGenerator::
trajectory3() {
    float v_x = 1.0;
    float v_y = 0;
    float v_z = -0.01f;
    float heading = 0;

    updateVelocity(v_x, v_y, v_z, heading, 11000);

    heading = M_PIf / 4;
    updateVelocity(v_x, v_y, v_z, heading, 10000);

    v_x = 0.0;
    v_y = 1.0;
    heading = M_PIf / 2;
    updateVelocity(v_x, v_y, v_z, heading, 7000);


    heading = 3 * M_PIf / 4;
    updateVelocity(v_x, v_y, v_z, heading, 5000);


    heading = M_PIf;
    v_x = -1.0f;
    v_y = 0.0;
    updateVelocity(v_x, v_y, v_z, heading, 16000);


    heading = 5 * M_PIf / 4;
    updateVelocity(v_x, v_y, v_z, heading, 5000);


    heading = -M_PIf / 2;
    v_x = 0.0;
    v_y = -1.0f;
    updateVelocity(v_x, v_y, v_z, heading, 10000);


}
}