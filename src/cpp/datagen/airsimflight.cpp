#include "airsim/AirSimInterface.h"
#include <iostream>
#include <sim/Simulation.h>
#include <sim/Kinematics.h>
#include <annotate/Gates.h>
#include <annotate/Camera.h>
#include <annotate/Annotator.h>
#include <annotate/RandomPoseDataGenerator.h>
#include <annotate/BoxObject.h>
#include <annotate/TrajectoryPoseGenerator.h>
#include <annotate/AnnotatorFlight.h>
#include <gui/Args.h>
#include "opencv2/highgui/highgui.hpp"
#include "sim/DroneModel.h"
#include "sim/Controller.h"

using namespace msr::airlib;



struct ArgsFlight : public Args{
int start_idx = 0, track = 3;
std::string output_path = "./";
float flightHeight = -3.5f;
float speed = 2.0;
float rounds = 3;
int width = 320, height = 240;

std::string name = "gate";
std::string imgFormat = "jpg";

void parse(int argc, char **argv) override {

    InputParser parser(argc, argv);
    std::string start_idx_str = parser.getCmdOption("-s");
    if (!start_idx_str.empty()) {
        start_idx = std::stoi(start_idx_str);
    }

    std::string path_str = parser.getCmdOption("-p");
    if (!path_str.empty()) {
        output_path = path_str;
    }
    std::string vel_str = parser.getCmdOption("-v");
    if (!path_str.empty()) {
        speed = std::stoi(vel_str);
    }

    std::string rounds_str = parser.getCmdOption("-r");
    if (!path_str.empty()) {
        rounds = std::stoi(rounds_str);
    }

    if (parser.cmdOptionExists("-h")) {
        std::cout << "-s --> start_idx\n"
                  << "-p --> path\n"
                  << "-r --> rounds\n"
                  << "-v --> speed\n"
                  << "-h --> display this help";
        exit(0);
    }
}
};

int main(int argc, char *argv[]) {

    ArgsFlight args;
    args.parse(argc, argv);

    AirSimInterface airsim;
    Simulation sim(&airsim);
    sim.control_frq_hz = 500;


    Camera cam(static_cast<float>(2*M_PI / 3),
               args.width,
               args.height);

    std::vector<msr::airlib::Pose> objPoses;
    std::map<std::string, msr::airlib::Pose> objectPose = airsim.getObjPoses(args.name);
    std::vector<Object*> objects;
    std::vector<std::string> names;
    std::vector<Eigen::Vector3f> path;
    for (auto it = objectPose.begin();
         it != objectPose.end();
         it++) {
        objPoses.push_back(it->second);
        names.push_back(it->first);
        float n = it->second.position[0];
        float e = it->second.position[1];

        path.push_back(Vector3f({n,e,args.flightHeight}));
        Object *obj;
        if (args.name == "gate") {
            obj = new Gate2017(it->second);
        }else if(args.name == "gate18_") {
            obj = new Gate2018(it->second);
        }else if(args.name == "gateMavlab_"){
            obj = new GateMavLab(it->second);
        }else{
            obj = new BoxObject(it->second, 1, 0.7, 0.7, args.name, Vector3f({0, 0, -5.0}));
        }

        objects.push_back(obj);
        std::cout << "Name: " << it->first << std::endl
                  << "Position: " << it->second.position << std::endl
                  << "Orientation: " << it->second.orientation.toRotationMatrix().eulerAngles(0, 1, 2)
                  << std::endl;
    }

    Annotator ann(objects, cam, args.output_path, args.imgFormat,args.start_idx);
    AnnotatorFlight flight{};
    flight.t_log_ms = 500;
    flight.ann = &ann;
    sim.eventHandlers.push_back(&flight);

    std::vector<Eigen::Vector3f> rounds;
    for(int i=0; i < args.rounds; i++){
        for (auto &v : path){
            rounds.push_back(v);
        }
    }

    std::atomic<bool> running=true;
    std::thread t = std::thread([&running,&rounds,&airsim,&args](){
        airsim.moveOnPath(rounds,args.speed,10*rounds.size());
        running = false;
    });
    std::this_thread::sleep_for(std::chrono::milliseconds((long long)(3000)));

    while(running){
        auto pose = airsim.getCameraPose();
        Image image = airsim.getImage();

        cv::Mat mat;
        if(!image.empty()){
            mat = cv::Mat(image.getHeight(), image.getWidth(), CV_8UC4);
            memcpy(mat.data, image.getData().data(), image.getData().size() * sizeof(uint8_t));
            cv::Mat mat_bgr;
            cv::cvtColor(mat, mat_bgr, cv::COLOR_RGB2BGR);
            flight.newImage(mat_bgr,pose);
        }

    }
    t.join();
}

                            