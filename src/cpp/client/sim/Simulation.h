/**
 * MavSim
 * Handles simulation of environment and hardware.
 * AHRS and Drone are simulated from us (DynamicModel)
 * Camera, Obstacles come from airsim.
 */
//
// Created by phil on 06/06/2018.
//

#ifndef MAVSIM_H
#define MAVSIM_H


#include <airsim/AirSimInterface.h>
#include <gui/CmdListener.h>
#include "Controller.h"
#include "Kinematics.h"
#include "SensorModel.h"

/** Derive from this class to register as event handler **/
class SimEventHandler{
public:
    virtual void newImage(cv::Mat &mat,msr::airlib::Pose &pose)=0;
    virtual void newSensorData(mav:: Kinematics &kin)=0;
};

static long long getCurrentTimeMillis(){
    using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
    );
    return ms.count();
}

using namespace mav;
class Simulation{
protected:
    DroneModel model;
    SensorModel sensorModel;
    mav::Kinematics sensorData{},groundTruth{},command{};
    AirSimInterface *airsim;
    Controller autopilot;
    std::mutex guard;
    ControlMode mode=POSITION_MODE;

    CmdListener cmdListener;
    std::thread airsimThread,modelThread,cmdThread;
    long long timestep_ms;
    std::atomic<bool> running=false,paused=false;
    void threadModel();
    void threadAirsim();
    void threadCmd();



public:
    std::vector<SimEventHandler*> eventHandlers;

    explicit Simulation(AirSimInterface *airsim);

    /** Run one loop of model/airsim **/
    void modelUpdate();
    void airsimUpdate();


    /** Set functions for autopilot **/
    void setRefPosition(float x, float y, float z, float heading);
    void setRefAttitude(float roll, float pitch, float yaw, float altitude);
    void setRefVelocity(float v_x,float v_y,float v_z,float heading);


    std::atomic<float> control_frq_hz = 100;
    std::atomic<float> airsim_frq_hz=500;

    /**
     * Start the simulation. Will start several threads that run the individual tasks.
     * @param controlLoop function that contains high level control loop
     */
    void startSim();
    virtual void stopSim();
    virtual void togglePause();
    bool isRunning(){return running;}
    void waitForTearDown(){cmdThread.join();}


};


#endif //OUTERLOOP_SIM_MAVSIM_H
