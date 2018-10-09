//
// Created by phil on 16/08/2018.
//

#ifndef DATAGEN_LABELS_H
#define DATAGEN_LABELS_H

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#define MAX_CORNERS 8

struct ObjectLabel{
    std::string name;
    /*Coordinates in the image plane*/
    int x[MAX_CORNERS]{};
    int y[MAX_CORNERS]{};
    int n_corners = 0;

    void addCorner(int x, int y) {
        this->x[n_corners] = x;
        this->y[n_corners] = y;
        n_corners++;
    }


    /* Coordinates 3D */
    float north{}, east{}, down{};
    float yaw{}, pitch{}, roll{};

    int xMax(){
        return *std::max_element(x, x + n_corners);
    }

    int yMax(){
        return *std::max_element(y, y + n_corners);
    }

    int xMin(){
        return *std::min_element(x, x + n_corners);
    }

    int yMin(){
        return *std::min_element(y, y + n_corners);
    }

    int width(){
        return abs(xMax() - xMin());
    }

    int height(){
        return abs(yMax() - yMin());
    }

    float aspect_ratio(){
        return (float)height()/(float)width();
    }


    virtual std::string toXml() {
        std::ostringstream xml;

        xml << "<object> "
               "<name>" << name << "</name>"
               "<bndbox>";
        xml << "<xmin>" << xMin() << "</xmin>";
        xml << "<ymin>" << yMin() << "</ymin>";
        xml << "<xmax>" << xMax() << "</xmax>";
        xml << "<ymax>" << yMax() << "</ymax>";
        xml << "</bndbox>";
        xml << "<pose>";
        xml << "<north>" << north << "</north>";
        xml << "<east>" << east << "</east>";
        xml << "<down>" << down << "</down>";
        xml << "<yaw>" << yaw << "</yaw>";
        xml << "<pitch>" << pitch << "</pitch>";
        xml << "<roll>" << roll << "</roll>";
        xml << "</pose>";
        xml << "</object>";

        return xml.str();
    }

    virtual std::string toString() {
        std::ostringstream s;

        s << "name: object\n";
        s << "(" << xMin() << "," << yMin() << ")";
        s << "--->";
        s << "(" << xMax() << "," << yMax() << ")";
        s << "\nPose:";
        s << "\nNorth:" << north;
        s << "\nEast:" << east;
        s << "\nDown:" << down;
        s << "\nYaw:" << yaw;
        s << "\nPitch" << pitch;
        s << "\nRoll:" << roll;

        return s.str();
    }

};


struct GateLabel : public ObjectLabel {


    std::string toXml() override {
        std::ostringstream xml;

        xml << "<object> "
               "<name>gate</name>"
               "<bndbox>";
        xml << "<xmin>" << xMin() <<"</xmin>";
        xml << "<ymin>" << yMin() <<"</ymin>";
        xml << "<xmax>" << xMax() <<"</xmax>";
        xml << "<ymax>" << yMax() <<"</ymax>";
        xml << "</bndbox>";
        xml << "<pose>";
        xml << "<north>"<< north << "</north>";
        xml << "<east>" << east << "</east>";
        xml << "<down>" << down << "</down>";
        xml << "<yaw>" << yaw << "</yaw>";
        xml << "<pitch>" << pitch << "</pitch>";
        xml << "<roll>" << roll << "</roll>";
        xml << "</pose>";
        xml << "<gate_corners>";
        xml << "<top_left>" << x[2] << "," << y[2] << "</top_left>";
        xml << "<top_right>" << x[1] << "," << y[1] << "</top_right>";
        xml << "<bottom_right>" << x[3] << "," << y[3] << "</bottom_right>";
        xml << "<bottom_left>" << x[0] << "," << y[0] << "</bottom_left>";
        xml << "<center>" << x[4] << "," << y[4] << "</center>";
        xml << "</gate_corners>";
        xml << "</object>";

        return xml.str();
    }

};

struct ImageLabel{
    std::vector<ObjectLabel*> objects;

    std::string toXml(std::string &filename){
        std::ostringstream xml;
        xml << "<annotation>";
        xml << "<filename>";
        xml << filename;
        xml << "</filename>";

        for (ObjectLabel *obj : objects){
            xml << obj->toXml();
        }
        xml << "</annotation>";

        return xml.str();
    }



};

#endif //DATAGEN_LABELS_H
