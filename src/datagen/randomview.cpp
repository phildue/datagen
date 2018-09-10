#include "airsim/AirSimInterface.h"
#include <iostream>
#include <annotate/Gate.h>
#include <annotate/Camera.h>
#include <annotate/Annotator.h>
#include <annotate/RandomPoseDataGenerator.h>
#include <annotate/BoxObject.h>
#include <gui/InputParser.h>
#include <gui/CmdListener.h>
#include "opencv2/highgui/highgui.hpp"

using namespace msr::airlib;

struct Args{
int start_idx = 0;
int n_samples = 0;
std::string output_path = "./";

int width = 416, height = 416;
int max_obj_size = 416;
int min_obj_size = 10;
float max_angle = 360;
float north_min = -20, north_max = 20;
float east_min = -10, east_max = 10;
float down_mean = -4.5f, down_variance = 0.5;
float roll_mean = 0.0, roll_variance = 0.1f * M_PIf;
float pitch_mean = 0.0, pitch_variance = 0.1f * M_PIf;
float yaw_min = -M_PIf, yaw_max = M_PIf;
std::string className = "gate";

Args(int argc, char *argv[]){
    parseArgs(argc,argv);
}

void parseArgs(int argc, char *argv[]) {

    InputParser parser(argc, argv);
    std::string start_idx_str = parser.getCmdOption("-s");
    if (!start_idx_str.empty()) {
        start_idx = std::stoi(start_idx_str);
    }

    std::string n_samples_str = parser.getCmdOption("-n");
    if (!n_samples_str.empty()) {
        n_samples = std::stoi(n_samples_str);
    }

    std::string path_str = parser.getCmdOption("-p");
    if (!path_str.empty()) {
        output_path = path_str;
    }

    if (parser.cmdOptionExists("-h")) {
        std::cout << "-s --> start_idx\n"
                  << "-n --> n_samples\n"
                  << "-p --> path\n"
                  << "-h --> display this help";
        exit(0);
    }
}
};
int main(int argc, char *argv[]) {

    Args args(argc,argv);

    std::cout << "Generating " << args.n_samples << " samples, starting from: " << args.start_idx << std::endl;


    AirSimInterface airsim;
    Camera cam(static_cast<float>(M_PI / 2),
               args.width,
               args.height);

    std::vector<msr::airlib::Pose> objPoses;
    std::map<std::string, msr::airlib::Pose> objectPose = airsim.getObjPoses(args.className);
    std::vector<Object *> objects;
    std::vector<std::string> names;

    for (auto it = objectPose.begin();
         it != objectPose.end();
         it++) {
        objPoses.push_back(it->second);
        names.push_back(it->first);
        Object *obj = new Gate(it->second);
        objects.push_back(obj);
        std::cout << "Name: " << it->first << std::endl
                  << "Position: " << it->second.position << std::endl
                  << "Orientation: " << it->second.orientation.toRotationMatrix().eulerAngles(0, 1, 2)
                  << std::endl;
    }

    Annotator ann(objects, cam, args.output_path, 500, args.start_idx);


    ObjectFilter filter(args.max_obj_size, args.min_obj_size, args.max_obj_size, args.min_obj_size, args.max_angle, args.width, args.height);
    RandomPoseDataGenerator generator(args.north_min, args.north_max, args.east_min, args.east_max, args.down_mean, args.down_variance, args.roll_mean,
                                      args.roll_variance, args.pitch_mean, args.pitch_variance, args.yaw_min, args.yaw_max, airsim, ann, 1,
                                      nullptr);

    CmdListener cmdListener;
    std::atomic<bool> running = true;
    cmdListener.addCommand("q", "Quit", [&running](std::string arg0, std::string arg1) { running = false; });

    std::thread generatorThread([&running, &generator](int n_samples) {
        while (generator.getNSamplesCreated() < n_samples && running) {
            generator.generateSample();
        }
    }, args.n_samples);
    while (running) {
        cmdListener.listen();
    }
    if (generatorThread.joinable()) {
        generatorThread.join();
    }

}

                            