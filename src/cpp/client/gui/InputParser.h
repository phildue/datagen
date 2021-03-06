//
// Created by phil on 02/09/2018.
//
//https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c

#ifndef INPUTPARSER_H
#define INPUTPARSER_H


#include <string>
#include <vector>

class InputParser {
public:
    InputParser(int &argc, char **argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.emplace_back(argv[i]);
    }

    /// @author iain
    const std::string &getCmdOption(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        static const std::string empty_string;
        return empty_string;
    }

    /// @author iain
    bool cmdOptionExists(const std::string &option) const {
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }

private:
    std::vector<std::string> tokens;
};


#endif //DATAGEN_INPUTPARSER_H
