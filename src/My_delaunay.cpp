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

bool My_delaunay::in_triangle(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3){
    double a11 = x2-x1;
    double a21 = y2-y1;
    double a12 = x3-x1;
    double a22 = y3-y1;
    double det = a11*a22-a21*a12;
    if(det==0){return false;}
    else{
        double px_new = px-x1;
        double py_new = py-y1;
        double sauv = (px_new*a22-py_new*a12)/det;
        py_new = (-px_new*a21+py_new*a11)/det;
        px_new = sauv;
        if(px_new>0 && py_new>0 && px_new+py_new<1){return true;}
        else{return false;}
    }
}