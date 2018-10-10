//
// Created by phil on 10/10/2018.
//

#ifndef DATAGEN_ARGS_H
#define DATAGEN_ARGS_H

#include <iostream>
#include "InputParser.h"

struct Args{

    virtual void parse(int argc, char **argv) {

        InputParser parser(argc, argv);


        if (parser.cmdOptionExists("-h")) {
            std::cout << "-h --> display this help";
            exit(0);
        }
    }
};
#endif //DATAGEN_ARGS_H
