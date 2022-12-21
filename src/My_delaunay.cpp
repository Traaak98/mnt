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
#include <iomanip>
#include <math.h>

using namespace std;

My_delaunay::My_delaunay(){}

void My_delaunay::make_delaunator(image &img){
    delaunator::Delaunator d(img.points);
    vect_triangles = d.triangles;
}

void My_delaunay::find_plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double &a, double &b, double &c, double &d){
    double a1 = x2 - x1;
    double b1 = y2 - y1;
    double c1 = z2 - z1;
    double a2 = x3 - x1;
    double b2 = y3 - y1;
    double c2 = z3 - z1;
    a = b1 * c2 + b2 * c1;
    b = - a2 * c1 - a1 * c2;
    c = - a1 * b2 + b1 * a2;
    d =  a * x1 + b * y1 - c * z1;
}

bool My_delaunay::in_triangle(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3){
    double det1 = (x2-px)*(y1-py)-(y2-py)*(x1-px);
    double det2 = (x3-px)*(y2-py)-(y3-py)*(x2-px);
    double det3 = (x1-px)*(y3-py)-(y1-py)*(x3-px);
    if(det1>= 0 && det2>=0 && det3>=0){return true;}
    else{return false;}
}

bool My_delaunay::find_zone(intervale inter, string &key, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3, image &img, int &i){
    if(inter.nb_passe == 0){
        string key = "";
    }
    double range_x;
    double range_y;

    //Condition arret:
    if(img.nb_zone == inter.nb_passe){
        if (!zones_b.count(key)){zones_b[key] = list<long unsigned int> {vect_triangles[i], vect_triangles[i+1], vect_triangles[i+2]};}
        else{
            zones_b[key].push_back(vect_triangles[i]);
            zones_b[key].push_back(vect_triangles[i+1]);
            zones_b[key].push_back(vect_triangles[i+2]);
        }
        return true;
    }

    //Quel cote on coupe
    if(inter.nb_passe%2==0){range_x = (inter.xmax-inter.xmin)/2; range_y=0;}
    else{range_y = (inter.ymax-inter.ymin)/2; range_x=0;}

    //On se balade dans l'arbre
    for(int k=0; k<2; k++){
        bool cote;
        intervale new_int;
        new_int.xmin = inter.xmin+k*range_x;
        new_int.xmax = inter.xmax-(1-k)*range_x;
        new_int.ymin = inter.ymin+k*range_y;
        new_int.ymax = inter.ymax-(1-k)*range_y;
        new_int.nb_passe = inter.nb_passe+1;
        if(k==0){cote=true;}
        if(k==1){cote=false;}
        if(in_intervale(new_int, cote, range_x, range_y, x1, y1, x2, y2, x3, y3)){
            if(k==0){key.append("a");}
            if(k==1){key.append("b");}
            if(find_zone(new_int, key, x1, y1, x2, y2, x3, y3, img, i)){}
            key=key.substr(0, key.size()-1);
        }
    }
    return false;
}

bool My_delaunay::in_intervale(intervale &inter, bool cote, double &range_x, double &range_y, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3){
    // cote : true -> gauche/haut false -> droite/bas
    if(range_x!=0){
        if(cote){
            if(x1<=inter.xmax){return true;}
            if(x2<=inter.xmax){return true;}
            if(x3<=inter.xmax){return true;}
        }
        else{
            if(x1>=inter.xmin){return true;}
            if(x2>=inter.xmin){return true;}
            if(x3>=inter.xmin){return true;}
        }
    }
    if(range_y!=0){
        if(cote){
            if(y1<=inter.ymax){return true;}
            if(y2<=inter.ymax){return true;}
            if(y3<=inter.ymax){return true;}
        }
        else{
            if(y1>=inter.ymin){return true;}
            if(y2>=inter.ymin){return true;}
            if(y3>=inter.ymin){return true;}
        }
    }
    return false;
}

void My_delaunay::build_map(image &img){
    double t_area;
    for(int i=0; i<vect_triangles.size(); i+=3) {

        cout <<"\r Creation arbre binaire pour les triangles :  " <<i << " / " << vect_triangles.size()<< " triangles realises" << flush;

        // Coins du triangle version x,y
        double x1 = img.points[2 * vect_triangles[i]];
        double y1 = img.points[2 * vect_triangles[i] + 1];
        double x2 = img.points[2 * vect_triangles[i + 1]];
        double y2 = img.points[2 * vect_triangles[i + 1] + 1];
        double x3 = img.points[2 * vect_triangles[i + 2]];
        double y3 = img.points[2 * vect_triangles[i + 2] + 1];

        area(x1, y1, x2, y2, x3, y3, t_area);

        if(t_area<=img.max_area){
            intervale inter_init;
            inter_init.xmin = img.min_x;
            inter_init.xmax = img.max_x;
            inter_init.ymin = img.min_y;
            inter_init.ymax = img.max_y;
            inter_init.nb_passe = 0;
            string key;

            find_zone(inter_init,key, x1, y1, x2, y2, x3, y3, img, i);}
    }
}

void My_delaunay::area(double x1, double y1, double x2, double y2, double x3, double y3, double &area){
       double ab = pow(pow(x2-x1,2)+pow(y2-y1,2), 0.5);
       double bc = pow(pow(x3-x2,2)+pow(y3-y2,2), 0.5);
       double ca = pow(pow(x1-x3,2)+pow(y1-y3,2), 0.5);
       double p = 0.5*(ab+bc+ca);
       area = pow(p*(p-ab)*(p-bc)*(p-ca), 0.5);
}