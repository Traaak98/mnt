//
// Created by apolline on 08/12/22.
//

#include"My_delaunay.h"
#include"image.h"
#include"delaunator.hpp"
#include<list>
#include<iostream>
#include<fstream>
#include <stdio.h>

using namespace std;

My_delaunay::My_delaunay(){
}

void My_delaunay::make_delaunator(image &img){
    delaunator::Delaunator d(img.points);
    vect_triangles = d.triangles;
}


void My_delaunay::build_map(image &img){
    for(int i = 0; i<vect_triangles.size(); i+=3) {

        int a, b, c;
        // Un triangle est dans une zone si au moins un des ses coins est dans la zone
        img.find_zone(img.points[2 * vect_triangles[i]], img.points[2 * vect_triangles[i] + 1], a);
        img.find_zone(img.points[2 * vect_triangles[i + 1]], img.points[2 * vect_triangles[i + 1] + 1], b);
        img.find_zone(img.points[2 * vect_triangles[i + 2]], img.points[2 * vect_triangles[i + 2] + 1], c);

        if (!zones.count(a)) {
            list<long unsigned int> ind_t = {vect_triangles[i], vect_triangles[i + 1], vect_triangles[i + 2]};
            zones[a] = ind_t;
        } else {
            zones[a].push_back(vect_triangles[i]);
            zones[a].push_back(vect_triangles[i + 1]);
            zones[a].push_back(vect_triangles[i + 2]);
        }

        if (a != b) {
            if (!zones.count(b)) {
                list<long unsigned int> ind_t = {vect_triangles[i], vect_triangles[i + 1], vect_triangles[i + 2]};
                zones[b] = ind_t;
            } else {
                zones[b].push_back(vect_triangles[i]);
                zones[b].push_back(vect_triangles[i + 1]);
                zones[b].push_back(vect_triangles[i + 2]);
            }
        }

        if(a!=c && b!=c){
            if (!zones.count(c)) {
                list<long unsigned int> ind_t = {vect_triangles[i], vect_triangles[i + 1], vect_triangles[i + 2]};
                zones[c] = ind_t;
            } else {
                zones[c].push_back(vect_triangles[i]);
                zones[c].push_back(vect_triangles[i + 1]);
                zones[c].push_back(vect_triangles[i + 2]);
            }
        }
    }
}

void My_delaunay::find_plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double &a, double &b, double &c, double &d){
    double a1 = x2 - x1;
    double b1 = y2 - y1;
    double c1 = z2 - z1;
    double a2 = x3 - x1;
    double b2 = y3 - y1;
    double c2 = z3 - z1;
    a = b1 * c2 - b2 * c1;
    b = a2 * c1 - a1 * c2;
    c = a1 * b2 - b1 * a2;
    d = (- a * x1 - b * y1 - c * z1);
}

bool My_delaunay::in_triangle(int px, int py, int x1, int y1, int x2, int y2, int x3, int y3){
    int det1 = (x2-px)*(y1-py)-(y2-py)*(x1-px);
    int det2 = (x3-px)*(y2-py)-(y3-py)*(x2-px);
    int det3 = (x1-px)*(y3-py)-(y1-py)*(x3-px);
    if(det1<= 0 && det2<=0 && det3<=0){return true;}
    else{return false;}
}