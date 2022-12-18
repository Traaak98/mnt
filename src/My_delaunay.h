//
// Created by apolline on 08/12/22.
//

#ifndef PROJET_MY_DELAUNAY_H
#define PROJET_MY_DELAUNAY_H

#include<vector>
#include<map>
#include"image.h"
#include<list>
#include <string>

class image;

class My_delaunay{
public:
    My_delaunay();
    void make_delaunator(image &img);
    std::vector<double> hauteurs;
    std::vector<long unsigned int> vect_triangles;
    std::map<int, std::list<long unsigned int>> zones;
    std::map<std::string, std::list<int>> zones_b;

    struct intervale{
        int nb_passe;
        double xmin;
        double xmax;
        double ymin;
        double ymax;
    };

    void build_map(image &img);
    void find_plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double &a, double &b, double &c, double &d);
    bool in_triangle(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3);
    bool in_intervale(intervale &inter, bool cote, double &range_x, double &range_y, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3);
    bool find_zone(intervale inter, std::string &key, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3, image &img, int &i);
    void build_map_b(image &img);
    void area(double x1, double y1, double x2, double y2, double x3, double y3, double &area);

};
#endif //PROJET_MY_DELAUNAY_H
