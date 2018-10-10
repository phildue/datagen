//
// Created by phil on 10/09/2018.
//

#ifndef TRAJECTORYPOSEGENERATOR_H
#define TRAJECTORYPOSEGENERATOR_H


#include <sim/Simulation.h>

namespace mav{
class TrajectoryPoseGenerator {
public:
    TrajectoryPoseGenerator(float height, float deltaYaw,int trajectory,Simulation *sim):
    height(height),deltaYaw(deltaYaw),trajectory(trajectory),sim(sim){};
    void generate();
    float height,deltaYaw;
    int trajectory;
protected:
    void trajectory1();
    void trajectory2();
    void trajectory3();
    void trajectoryIros();
    void updateVelocity(float v_x, float v_y, float v_z, float yaw, double t);
    Simulation *sim;

};};
#endif //DATAGEN_TRAJECTORYPOSEGENERATOR_H
