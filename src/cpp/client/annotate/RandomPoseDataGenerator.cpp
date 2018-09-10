//
// Created by phil on 15/08/2018.
//

#include <opencv2/highgui.hpp>
#include "RandomPoseDataGenerator.h"


void
RandomPoseDataGenerator::
generateSample() {
    float pitch = distr_pitch(generator);
    float roll = distr_roll(generator);
    float yaw = distr_yaw(generator);
    float north = distr_north(generator);
    float east = distr_east(generator);
    float down = distr_down(generator);

    std::cout << "North: " << north << "| East: " << east << "| Down: " << down << std::endl
              << "Yaw: " << yaw << "| Pitch: " << pitch << "| Roll: " << roll << std::endl;

    airsim.setDronePose(north, east, down, roll, pitch, yaw);

    /* Sometimes airsim takes a while to update pose and image so we give the old lady some time*/
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(100));

    auto image = airsim.getImage().toMat();
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(10));
    auto camPose = airsim.getCameraPose();

    auto label = ann.createLabel(camPose);

    ImageLabel label_filtered;
    if (filter) {
        label_filtered = filter->filter(label);
    } else {
        label_filtered = label;
    }

    auto imageAnn = image.clone();
    Annotator::annotate(imageAnn, label_filtered);


    if (label.n_objects == label_filtered.n_objects &&
        label_filtered.n_objects >= min_n_objects) {
        ann.write(image, label_filtered);
        cv::imshow("Sample", imageAnn);
        cv::waitKey(1);
        n_created++;
    } else {
        cv::imshow("Filtered", imageAnn);
        cv::waitKey(1);
        n_filtered++;
    }
    std::cout << "Created: " << n_created << " Filtered: " << n_filtered
              << "-------------------------- " << std::endl;

}

void
RandomPoseDataGenerator::
generate(int n_samples)
    {
        while(n_created < n_samples){
            generateSample();
        }
}


RandomPoseDataGenerator::
RandomPoseDataGenerator(float north_min, float north_max, float east_min, float east_max,
                        float down_mean, float down_variance, float roll_mean,
                        float roll_variance, float pitch_mean, float pitch_variance,
                        float yaw_min, float yaw_max, AirSimInterface &airsim,
                        Annotator &ann, int min_n_objects, ObjectFilter *filter)
        : airsim(airsim), ann(ann), filter(filter), min_n_objects(min_n_objects) {

    init_distrs(pitch_mean,pitch_variance,yaw_min,yaw_max,roll_mean,roll_variance,north_min,north_max,east_min,east_max,down_mean,down_variance);

}

void
RandomPoseDataGenerator::
init_distrs(float pitch_mean, float pitch_variance, float yaw_min, float yaw_max, float roll_mean,
                                          float roll_variance, float north_min, float north_max, float east_min, float east_max,
                                          float down_mean, float down_variance) {
    distr_pitch = std::normal_distribution<float>(pitch_mean,pitch_variance);
    distr_yaw = std::uniform_real_distribution<float>(yaw_min,yaw_max);
    distr_roll = std::normal_distribution<float>(roll_mean,roll_variance);

    distr_north = std::uniform_real_distribution<float>(north_min,north_max);
    distr_east = std::uniform_real_distribution<float>(east_min,east_max);
    distr_down = std::normal_distribution<float>(down_mean,down_variance);
}



