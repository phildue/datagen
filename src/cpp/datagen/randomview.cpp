#include "airsim/AirSimInterface.h"
#include <iostream>
#include <annotate/Camera.h>
#include <annotate/Annotator.h>
#include <annotate/RandomPoseDataGenerator.h>
#include <annotate/BoxObject.h>
#include <gui/InputParser.h>
#include <gui/CmdListener.h>
#include <annotate/Gates.h>
#include <gui/Args.h>
#include "opencv2/highgui/highgui.hpp"

using namespace msr::airlib;

struct ArgsRandomView : public Args{
int start_idx = 0;
int n_samples = 100;
std::string output_path = "./";

int width = 416, height = 416;
float fov = 2*M_PI / 3;
int max_obj_size = 500;
int min_obj_size = 10;
float max_aspect_ratio = 3.0;
float min_aspect_ratio = 0.3;
float north_min = -2.5f, north_max = 2.5f;
float east_min = -0.5f, east_max = 0.5;
float down_mean = -3.5f, down_variance = 0.5;
float roll_mean = 0.0, roll_variance = 0.01f * M_PIf;
float pitch_mean = 0.0, pitch_variance = 0.01f * M_PIf;
float yaw_min = -0.001*M_PIf, yaw_max = 0.001;
std::string className = "gate";
std::string image_format = "jpg";

void parse(int argc, char **argv) override {

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

    ArgsRandomView args;
    args.parse(argc, argv);

    std::cout << "Generating " << args.n_samples << " samples, starting from: " << args.start_idx << std::endl;


    /* Connect to airsim and ask for object positions*/

    AirSimInterface airsim;
    std::map<std::string, msr::airlib::Pose> objectPose = airsim.getObjPoses(args.className);
    std::vector<Object *> objects;

    if (objectPose.empty()){
        std::cerr << "No Objects Found!" << std::endl;
        exit(-1);
    }

    for (auto it = objectPose.begin();
         it != objectPose.end();
         it++) {
        Object *obj;
        if (args.className == "gate") {
            obj = new Gate2017(it->second);
        }else if(args.className == "gate18_"){
            obj = new Gate2018(it->second);
        }else if(args.className == "gateMavlab_"){
            obj = new GateMavLab(it->second);
        }else{
            obj = new BoxObject(it->second, 1, 0.7, 0.7, args.className, Eigen::Vector3f({0, 0, -5.0}));
        }
        objects.push_back(obj);
        std::cout << "Name: " << it->first << std::endl
                  << "Position: " << it->second.position << std::endl
                  << "Orientation: " << it->second.orientation.toRotationMatrix().eulerAngles(0, 1, 2)
                  << std::endl;
    }

    /* Create class instances that generate labels */
    Camera cam(args.fov,
               args.width,
               args.height);
    Annotator ann(objects, cam, args.output_path, args.image_format, args.start_idx);


    /* Create pose generator that create camera views */
    ObjectFilter filter(args.max_obj_size, args.min_obj_size, args.max_obj_size, args.min_obj_size, args.max_aspect_ratio,args.min_aspect_ratio, args.width, args.height);
    RandomPoseDataGenerator generator(args.north_min, args.north_max, args.east_min, args.east_max, args.down_mean, args.down_variance, args.roll_mean,
                                      args.roll_variance, args.pitch_mean, args.pitch_variance, args.yaw_min, args.yaw_max, airsim, ann, 1,
                                      &filter);

    /*User interface*/
    CmdListener cmdListener;
    std::atomic<bool> running = true;
    cmdListener.addCommand("q", "Quit", [&running](std::string arg0, std::string arg1) { running = false; });

    /* Loop in new thread to create samples*/
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