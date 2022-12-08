//
// Created by apolline on 08/12/22.
//

#ifndef PROJET_IMAGE_H
#define PROJET_IMAGE_H

#include <vector>
#include"My_delaunay.h"
#include<string>
#include <proj.h>

class My_delaunay;

class image{
public:
    image(int nb_pixels, int nb_zones);
    int nb_pixel_h;
    int nb_pixel_l;
    int nb_zone;
    int nb_donnee;
    double densite;
    double max_x;
    double max_y;
    double min_x;
    double min_y;
    std::vector<double> points;
    void update_min_max(double x, double y);
    void update_densite();
    void update_nb_pixel_l();
    void projection(double lon, double lat, double z, double &x, double &y, PJ* P);
    PJ* init_proj();
    void read_file(My_delaunay &dt, std::string filename);
    ~image();

};
#endif //PROJET_IMAGE_H
