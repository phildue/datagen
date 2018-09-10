//
// Created by phil on 01/04/2018.
//

#include <sim/Simulation.h>
#include "SimRunner.h"
#include "gui/CmdListener.h"
#include "MotionRead.h"
void SimRunner::startSim(std::function<void()> controlLoop) {

    hasControlLoop = true;
	simThread = std::thread(&SimRunner::threadSimulation,this,controlLoop);
    startSim();

}

void SimRunner::startSim() {
    startTime = getCurrentTimeMillis();
    cmdListener.printHelp();
    running = true;
    airsimThread = std::thread(&SimRunner::threadAirsim,this);
    modelThread = std::thread(&SimRunner::threadModel,this);
    cmdThread = std::thread(&SimRunner::threadCmd,this);

}

void SimRunner::stopSim() {
    if(!running){
        return;
    }
    this->running = false;
    std::cout<<"Stopping simulation.."<<std::endl;
    airsimThread.join();
    modelThread.join();
    if(hasControlLoop){
        simThread.join();
        hasControlLoop = false;
    }
    cv::destroyAllWindows();
}

void SimRunner::togglePause() {
    this->paused = !this->paused;

    if(this->paused){
        std::cout<<"Control loop paused." << std::endl;
    }else{
        std::cout<<"Control loop running.." <<std::endl;
    }

}

SimRunner::~SimRunner() {
    if (this->running){
        this->stopSim();
    }
}

SimRunner::SimRunner(const SimRunner &old) {
    this->sim = old.sim;
    this->camStream = old.camStream;
    this->annotator = old.annotator;
}

void SimRunner::threadSimulation(std::function<void()> controlLoop) {
	std::cout << "Starting simulation with control frequency: " << control_frq_hz << ' Hz' << std::endl;
	while(this->running){
        if(!this->paused){
			controlLoop();
			std::this_thread::sleep_for(std::chrono::milliseconds((long long)(1000/control_frq_hz)));

        }
    }
}

void SimRunner::threadModel() {
    std::cout << "Starting model with frequency: " << control_frq_hz << ' Hz' << std::endl;
    while(this->running){
        if(!this->paused){
            sim->modelUpdate();
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)(1000/control_frq_hz)));

        }
    }
}

void SimRunner::threadAirsim() {
    std::cout << "Starting airsim comm with frequency: " << airsim_frq_hz << ' Hz' << std::endl;
    while(this->running){
        if(!this->paused){
            sim->airsimUpdate();
            std::this_thread::sleep_for(std::chrono::milliseconds((long long)(1000/airsim_frq_hz)));

        }
    }
}





SimRunner::SimRunner(Simulation *sim, CamStream *camStream, Annotator *annotator) {
    this->sim = sim;
    sim->setAirsimEventHandler(this);
    this->camStream = camStream;
    this->annotator = annotator;
    cmdListener.addCommand("q","Quit Application",[this](std::string arg0, std::string arg1){ stopSim();});
    cmdListener.addCommand("p","Pause/Continue simulation.",[this](std::string arg0, std::string arg1){togglePause();});
}

void SimRunner::threadCmd() {
    while(this->running){
        cmdListener.listen();
    }
}

void SimRunner::onNewData(cv::Mat &mat, msr::airlib::Pose &pose) {
    if(annotator){
        annotator->onNewData(mat,pose);
    }
    if(camStream){
        camStream->onNewFrame(mat);
    }
}






