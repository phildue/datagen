#include <opencv2/imgproc.hpp>
#include "Image.h"


Image::Image(std::vector<uint8_t> rgba, int width, int height){
    this->data = rgba;
    this->width = width;
    this->height = height;
}

std::vector<uint8_t > Image::getData() {
    return this->data;
}

int Image::getHeight() {
    return this->height;
}

int Image::getWidth() {
    return this->width;
}

bool Image::empty(){
    return data.empty();
}

cv::Mat Image::toMat() {
    cv::Mat mat;
    if(!empty()){
        mat = cv::Mat(getHeight(), getWidth(), CV_8UC4);
        memcpy(mat.data, getData().data(), getData().size() * sizeof(uint8_t));
    }else{
        mat = cv::Mat(10, 10, CV_8UC4);

    }
    cv::Mat mat_bgr;
    cv::cvtColor(mat, mat_bgr, cv::COLOR_RGB2BGR);

    return mat_bgr;
}
