//
// Created by phil on 16/08/2018.
//

#ifndef DATAGEN_FILTER_H
#define DATAGEN_FILTER_H


#include "Labels.h"

class ObjectFilter {
public:
    ImageLabel filter(ImageLabel &label);
    ObjectFilter(    int max_obj_width,
    int min_obj_width,
    int max_obj_height,
    int min_obj_height,
    float max_angle,
    int img_width,
    int img_height):max_obj_height(max_obj_height),
                    max_obj_width(max_obj_width),
                    min_obj_height(min_obj_height),
                    min_obj_width(min_obj_width),
                    max_angle(max_angle),
                    img_height(img_height),
                    img_width(img_width){
        max_aspect_ratio = (1.125f / (max_angle / 90.0f));
    }
protected:
    int max_obj_width;
    int min_obj_width;
    int max_obj_height;
    int min_obj_height;
    float max_angle;
    int img_width;
    int img_height;

    bool out_of_img(int x, int y);
    int out_of_img(ObjectLabel &obj);
    bool out_of_size(ObjectLabel &obj);
    bool bad_angle(ObjectLabel &obj);

    float max_aspect_ratio;
};


#endif //DATAGEN_FILTER_H
