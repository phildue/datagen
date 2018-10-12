#ifndef IMAGE_H
#define IMAGE_H


#include <cstdint>
#include <vector>
#include <opencv2/core/mat.hpp>

class Image {
    std::vector<uint8_t> data;
    int width=0;
    int height=0;
public:
    Image(std::vector<uint8_t> rgba, int width, int height);
    ~Image() = default;
    Image() = default;
    int getWidth();
    int getHeight();
    bool empty();
    std::vector<uint8_t > getData();
    cv::Mat toMat();
};


#endif //IMAGE_H
