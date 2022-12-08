//
// Created by apolline on 08/12/22.
//

#ifndef PROJET_MY_DELAUNAY_H
#define PROJET_MY_DELAUNAY_H

#include<vector>
#include<map>
#include"image.h"

class image;

class My_delaunay{
public:
    My_delaunay();
    void make_delaunator(image img);
    std::vector<double> hauteurs;
    std::vector<long unsigned int> vect_triangles;
    std::map<std::tuple<int, int, int>, int> zones;

};
#endif //PROJET_MY_DELAUNAY_H
