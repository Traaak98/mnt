//
// Created by apolline on 08/12/22.
//

#ifndef PROJET_MY_DELAUNAY_H
#define PROJET_MY_DELAUNAY_H

#include<vector>
#include<map>
#include"image.h"
#include<list>

class image;

class My_delaunay{
public:
    My_delaunay();
    void make_delaunator(image &img);
    std::vector<double> hauteurs;
    std::vector<long unsigned int> vect_triangles;
    std::map<int, std::list<long unsigned int>> zones;

    void build_map(image &img);
    void find_plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double &a, double &b, double &c, double &d);
    bool in_triangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3);

};
#endif //PROJET_MY_DELAUNAY_H
