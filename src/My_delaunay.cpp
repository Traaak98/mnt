//
// Created by apolline on 08/12/22.
//

#include"My_delaunay.h"
#include"image.h"
#include"delaunator.hpp"

using namespace std;

My_delaunay::My_delaunay(){
}

void My_delaunay::make_delaunator(image img){
    delaunator::Delaunator d(img.points);
    vect_triangles = d.triangles;
}
