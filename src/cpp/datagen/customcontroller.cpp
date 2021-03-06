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
float flightHeight = -4.0f;
int width = 416, height = 416;
float deltaYaw = M_PIf;
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
    std::string track_str = parser.getCmdOption("-t");
    if (!path_str.empty()) {
        track = std::stoi(track_str);
    }

    std::string delta_yaw_str= parser.getCmdOption("-y");
    if (!path_str.empty()) {
        deltaYaw = std::stof(delta_yaw_str)/180.0f*M_PIf;
    }


    if (parser.cmdOptionExists("-h")) {
        std::cout << "-s --> start_idx\n"
                  << "-n --> n_samples\n"
                  << "-p --> path\n"
                  << "-y --> yaw\n"
                  << "-t --> track\n"
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

    for (auto it = objectPose.begin();
         it != objectPose.end();
         it++) {
        objPoses.push_back(it->second);
        names.push_back(it->first);

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

    TrajectoryPoseGenerator poseGenerator(args.flightHeight,args.deltaYaw,args.track,&sim);

    poseGenerator.generate();

}

                            