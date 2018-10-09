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
    float max_aspect_ratio,
    float min_aspect_ratio,
    int img_width,
    int img_height):max_obj_height(max_obj_height),
                    max_obj_width(max_obj_width),
                    min_obj_height(min_obj_height),
                    min_obj_width(min_obj_width),
                    max_aspect_ratio(max_aspect_ratio),
                    min_aspect_ratio(min_aspect_ratio),
                    img_height(img_height),
                    img_width(img_width){
    }
protected:
    int max_obj_width;
    int min_obj_width;
    int max_obj_height;
    int min_obj_height;
    int img_width;
    int img_height;

    bool out_of_img(int x, int y);
    int out_of_img(ObjectLabel *obj);
    bool out_of_size(ObjectLabel *obj);
    bool bad_angle(ObjectLabel *obj);

    float max_aspect_ratio;
    float min_aspect_ratio;
};


#endif //DATAGEN_FILTER_H
