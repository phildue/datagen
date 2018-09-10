//
// Created by phil on 09/08/2018.
//

#ifndef ANNOTATER_H
#define ANNOTATER_H


#include <airsim/AirSimInterface.h>
#include <opencv2/imgproc.hpp>
#include "Camera.h"
#include "Labels.h"

class Annotator{
public:

    /**
     * Constructor
     * @param objects list of gate poses obtained from airsim
     * @param cam camera model
     * @param path to save images + labels
     * @param t_log_ms period to store images.
     */
    Annotator(std::vector<Object *> &objects,
              Camera &cam,
              std::string &path,
              int t_log_ms = 500,
              int start_idx = 0)
            : objects(objects),
              cam(cam),
              path(path),
              t_log_ms(t_log_ms),
              index(start_idx)
    {};

    /**
     * Creates label with gate corner annotation depending on the current camera position;
     * @param campose of the camera
     * @return image label with annotated gate corners.
     */
    ImageLabel createLabel(msr::airlib::Pose &campose);


    /**
     * Annotate the visible corners.
     * @param mat image to annotate
     * @param label corresponding label
     * @return annotated image.
     */
    static cv::Mat annotate(cv::Mat &mat, ImageLabel &label);

    /**
     * Write label and image to disk with increasing index.
     * @param mat image
     * @param label
     */
    void write(cv::Mat &mat, ImageLabel &label);

    /**
     * Write label and image to disk.
     * @param mat image
     * @param label
     * @param filename
     */
    static void write(cv::Mat &mat, ImageLabel &label, std::string &filename);

protected:
    std::vector<Object *> objects;
    Camera cam;
    int index;
    int t_log_ms;
    const std::string path;

};


#endif //ANNOTATER_H
