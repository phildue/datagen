/**
 * SimRunner.
 * Connects Simulation with HighLevel Control Loop and User Interface elements.
 */
//
// Created by mail- on 01/04/2018.
//

#ifndef AIRSIMCLIENT_H
#define AIRSIMCLIENT_H


#include <gui/CmdListener.h>
#include <annotate/Annotator.h>
#include <Scheduler.h>
#include "airsim/AirSimInterface.h"
#include "gui/CamStream.h"
#include "sim/DynamicModel.h"
#include "HardwareAbstractionLayer.h"
class SimRunner: public AirsimEventHandler {

protected:
    Annotator *annotator;
    CmdListener cmdListener{};
    Simulation *sim;
    CamStream *camStream;
    std::thread simThread,airsimThread,modelThread,cmdThread;
    long long startTime;
    std::atomic<bool> running=false,paused=false,hasControlLoop=false;
    void threadSimulation(std::function<void()> controlLoop);
    void threadModel();
    void threadAirsim();
    void threadCmd();
public:
    ~SimRunner();
    SimRunner() = default;
    SimRunner(const SimRunner &);
    SimRunner(Simulation *sim, CamStream *camStream = nullptr, Annotator *ann = nullptr);

    std::atomic<float> control_frq_hz = 100;
    std::atomic<float> airsim_frq_hz=500;

    /**
     * Start the simulation. Will start several threads that run the individual tasks.
     * @param controlLoop function that contains high level control loop
     */
	void startSim(std::function<void()> controlLoop);
    void startSim();
    Simulation* getSim(){return sim;};
	virtual void stopSim();
	virtual void togglePause();
    bool isRunning(){return running;}
    void waitForTearDown(){cmdThread.join();}
    int getStartupTimeMillis(){
        return static_cast<int>(getCurrentTimeMillis() - startTime);
    }
    void onNewData(cv::Mat &mat, msr::airlib::Pose &pose) override ;
};


#endif //CLIENT_DEMO_AIRSIMCLIENT_H
