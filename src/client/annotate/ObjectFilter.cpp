//
// Created by phil on 16/08/2018.
//

#include <iostream>
#include "ObjectFilter.h"

bool ObjectFilter::out_of_size(ObjectLabel &obj) {
    return  obj.width() > max_obj_width || obj.width() < min_obj_width || obj.height() > max_obj_height || obj.height() < min_obj_height;
}

int ObjectFilter::out_of_img(ObjectLabel &obj) {
    int out_of_view = 0;

    for (int i_corners = 0; i_corners < MAX_CORNERS; i_corners++) {
        if(out_of_img(obj.x[i_corners],obj.y[i_corners])){
            out_of_view++;
        }
    }
    return out_of_view;
}

bool ObjectFilter::bad_angle(ObjectLabel &obj) {
    return obj.aspect_ratio() > max_aspect_ratio;
}

bool ObjectFilter::out_of_img(int x, int y) {
    return x > img_width || x < 0 || y > img_height || y < 0;
}

ImageLabel ObjectFilter::filter(ImageLabel &label) {
    ImageLabel filtered{};
    for (int i = 0; i < label.n_objects; i++){
        auto obj = label.objects[i];

        if(out_of_size(obj) || out_of_img(obj) > 1 || bad_angle(obj)){
            continue;
        }else{
            filtered.add(obj);
        }
    }
    return filtered;
}