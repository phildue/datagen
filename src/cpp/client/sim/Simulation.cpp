//
// Created by phil on 06/06/2018.
//

#include <opencv2/imgproc.hpp>
#include <Image.h>
#include <opencv2/highgui.hpp>
#include "Simulation.h"
#include "Kinematics.h"


void
Simulation::
airsimUpdate()
{
    /**
     * One iteration for airsim:
     * (1) we get the current position from the model
     *     and update the drone position in airsim accordingly.
     * (2) We get the image from the camera as well as its pose
     * (which is different from the drones position as it is mounted in front of the drone)
     * (3) We call functions of the event handlers.
     */
    guard.lock();
    mav::Kinematics groundTruth = this->groundTruth;
    mav::Kinematics sensorData = this->sensorData;
    guard.unlock();

    airsim->setDronePose(groundTruth.x, groundTruth.y, groundTruth.z, groundTruth.phi, groundTruth.theta, groundTruth.psi);

    //sometimes airsim seems to need some time to react// so we give the old lady 10ms
    std::this_thread::sleep_for(std::chrono::duration<double,std::milli>(10));

    auto pose = airsim->getCameraPose();
    Image image = airsim->getImage();

    cv::Mat mat;
    if(!image.empty()){
        mat = cv::Mat(image.getHeight(), image.getWidth(), CV_8UC4);
        memcpy(mat.data, image.getData().data(), image.getData().size() * sizeof(uint8_t));
    }else{
        mat = cv::Mat(10, 10, CV_8UC4);

    }

    cv::Mat mat_bgr;
    cv::cvtColor(mat, mat_bgr, cv::COLOR_RGB2BGR);

    for(SimEventHandler *h : eventHandlers){
        h->newImage(mat_bgr,pose);
        h->newSensorData(sensorData);

    }
}

void
Simulation::
modelUpdate()
{
    /**
     * One iteration of the model + controller.
     * After updating both we inform the event handler.
     * **/
    guard.lock();
    mav::Kinematics groundTruth = this->groundTruth;
    guard.unlock();


    switch (mode){
        case VELOCITY_MODE:
        {
            guard.lock();
            float v_x = command.vX;
            float v_y = command.vY;
            float v_z = command.vZ;
            float psi = command.psi;
            guard.unlock();
            autopilot.setVelocity(v_x,v_y,v_z,psi);
            break;
        }
        case POSITION_MODE:
        {

            guard.lock();
            float x = command.x ;
            float y = command.y;
            float z = command.z;
            float psi = command.psi;
            guard.unlock();
            autopilot.setReference(x,y,z,psi);
            break;
        }
        case ATTITUDE_MODE:
        {
            guard.lock();
            float phi = command.phi;
            float theta = command.theta;
            float psi = command.psi;
            float altitude = command.z;
            guard.unlock();
            autopilot.setAttitude(phi,theta,psi,altitude);
            break;
        }
        default:
            autopilot.setReference(0,0,0,0);

    }
    mav::Kinematics cmd = autopilot.controllerRun(groundTruth);
    model.setCommand(cmd.phi, cmd.theta, cmd.psi, cmd.accZ);
    model.propagateDroneModel();

    groundTruth = {
            model.getX(),
            model.getY(),
            model.getZ(),
            model.getPhi(),
            model.getTheta(),
            model.getPsi(),
            model.getP(),
            model.getQ(),
            model.getR(),
            model.getAccX(),
            model.getAccY(),
            model.getAccZ(),
            model.getVx(),
            model.getVy(),
            model.getVz()
    };

    sensorModel.update(groundTruth);
    //sensorModel.complementaryFilterAHRS_run();
    sensorModel.kalmanFilterAHRS_run();
    mav::Kinematics sensorData ={
            0,
            0,
            model.getZ(),
            sensorModel.getPhiAHRS(),
            sensorModel.getThetaAHRS(),
            sensorModel.getPSiAHRS(),
            sensorModel.getP(),
            sensorModel.getQ(),
            sensorModel.getR(),
            sensorModel.getAccX(),
            sensorModel.getAccY(),
            sensorModel.getAccZ(),
            0,
            0,
            0,
    };
    std::cout << "done" << std::endl;

    guard.lock();
    this->groundTruth = groundTruth;
    this->sensorData = sensorData;
    guard.unlock();

}


Simulation::
Simulation( AirSimInterface *airsim)
{
    this->airsim = airsim;
    cmdListener.addCommand("q","Quit Application",[this](std::string arg0, std::string arg1){ stopSim();});
    cmdListener.addCommand("p","Pause/Continue simulation.",[this](std::string arg0, std::string arg1){togglePause();});
}

void
Simulation::
setRefPosition(float x, float y, float z, float heading)
{
    guard.lock();
    command.x = x;
    command.y = y;
    command.z = z;
    command.psi = heading;
    mode = POSITION_MODE;
    guard.unlock();

}

void
Simulation::
setRefAttitude(float roll, float pitch, float yaw, float altitude)
{
    guard.lock();
    command.phi = roll;
    command.theta = pitch;
    command.psi = yaw;
    command.z = altitude;
    mode = ATTITUDE_MODE;
    guard.unlock();
}

void Simulation::setRefVelocity(float v_x,float v_y,float v_z,float heading) {
    guard.lock();
    command.vX = v_x;
    command.vY = v_y;
    command.vZ = v_z;
    command.psi = heading;
    mode = VELOCITY_MODE;
    guard.unlock();}

void Simulation::startSim() {
    startTime = getCurrentTimeMillis();
    cmdListener.printHelp();
    running = true;
    airsimThread = std::thread(&Simulation::threadAirsim,this);
    modelThread = std::thread(&Simulation::threadModel,this);
    cmdThread = std::thread(&Simulation::threadCmd,this);

}

void Simulation::stopSim() {
    if(!running){
        return;
    }
    this->running = false;
    std::cout<<"Stopping simulation.."<<std::endl;
    airsimThread.join();
    modelThread.join();
    cv::destroyAllWindows();
}

void Simulation::togglePause() {
    this->paused = !this->paused;

    if(this->paused){
        std::cout<<"Control loop paused." << std::endl;
    }else{
        std::cout<<"Control loop running.." <<std::endl;
    }

}

void Simulation::threadModel() {
    std::cout << "Starting model with frequency: " << control_frq_hz << ' Hz' << std::endl;
    while(this->running){
        if(!this->paused){
            modelUpdate();
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)(1000/control_frq_hz)));

        }
    }
}

void Simulation::threadAirsim() {
    std::cout << "Starting airsim comm with frequency: " << airsim_frq_hz << ' Hz' << std::endl;
    while(this->running){
        if(!this->paused){
            airsimUpdate();
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)(1000/airsim_frq_hz)));

        }
    }
}


void Simulation::threadCmd() {
    while(this->running){
        cmdListener.listen();
    }
}
