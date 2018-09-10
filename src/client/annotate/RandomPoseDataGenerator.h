//
// Created by phil on 15/08/2018.
//

#ifndef RANDOMPOSEDATAGENERATOR_H
#define RANDOMPOSEDATAGENERATOR_H


#include <airsim/AirSimInterface.h>
#include "Annotator.h"
#include "ObjectFilter.h"

class RandomPoseDataGenerator {
public:
    RandomPoseDataGenerator(float north_min, float north_max, float east_min, float east_max,
                            float down_mean, float down_variance, float roll_mean,
                            float roll_variance, float pitch_mean, float pitch_variance,
                            float yaw_min, float yaw_max, AirSimInterface &airsim,
                            Annotator &ann, int min_n_objects = 1, ObjectFilter *filter = nullptr);



    void generate(int n_samples);

    void generateSample();

    int getNSamplesCreated() {
        return n_created;
    };

protected:
    void init_distrs(float pitch_mean, float pitch_variance, float yaw_min, float yaw_max, float roll_mean,
                     float roll_variance, float north_min, float north_max, float east_min, float east_max,
                     float down_mean, float down_variance);

    std::normal_distribution<float> distr_pitch;
    std::uniform_real_distribution<float> distr_yaw;
    std::normal_distribution<float> distr_roll;

    std::uniform_real_distribution<float> distr_north;
    std::uniform_real_distribution<float> distr_east;
    std::normal_distribution<float> distr_down;
    AirSimInterface airsim;
    Annotator ann;
    ObjectFilter *filter;
    std::default_random_engine generator;
    std::atomic<int> n_filtered = 0, n_created = 0;
    int min_n_objects;
};


#endif //DATAGEN_RANDOMPOSEDATAGENERATOR_H
