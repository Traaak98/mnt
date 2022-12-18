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


void My_delaunay::build_map(image &img){
    bool first = 0;
    for(int i = 0; i<vect_triangles.size(); i+=3) {

        // Coins du triangle version x,y
        double x1 = img.points[2 * vect_triangles[i]];
        double y1 = img.points[2 * vect_triangles[i] + 1];
        double x2 = img.points[2 * vect_triangles[i + 1]];
        double y2 = img.points[2 * vect_triangles[i + 1] + 1];
        double x3 = img.points[2 * vect_triangles[i + 2]];
        double y3 = img.points[2 * vect_triangles[i + 2] + 1];

        int a, b, c;

        // Un triangle est dans une zone si au moins un des ses coins est dans la zone
        img.find_zone(img.points[2 * vect_triangles[i]],  img.points[2 * vect_triangles[i] + 1], a);
        img.find_zone(img.points[2 * vect_triangles[i + 1]], img.points[2 * vect_triangles[i + 1] + 1], b);
        img.find_zone(img.points[2 * vect_triangles[i + 2]], img.points[2 * vect_triangles[i + 2] + 1], c);

        if(first==0){
            ofstream j("booo.txt");
            j << "[";
            j << setprecision(15)<<img.points[2 * vect_triangles[i]] <<"," <<  img.points[2 * vect_triangles[i] + 1] << "," << img.points[2 * vect_triangles[i + 1]] << ","<< img.points[2 * vect_triangles[i + 1] + 1] << "," <<img.points[2 * vect_triangles[i + 2]]<<","<< img.points[2 * vect_triangles[i + 2] + 1] << "]\n";
            j<<a<<" "<<b<<" "<<c;
            first=1;
            j.close();
        }


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
    //cout << " passe " << endl;
    if(inter.nb_passe == 0){
        string key = "";
    }
    double range_x;
    double range_y;
    //cout << inter.nb_passe << endl;
    //cout<< key << endl;

    //Condition arret:
    if(img.nb_zone == inter.nb_passe){
        if (!zones_b.count(key)){zones_b[key] = list<int> {i};
            //cout << " creation " << key << endl;
        }
        else{
            zones_b[key].push_back(i);
            //cout << " deja " << key << endl;
        }
        //cout << " STOP" << endl;
        return true;
    }

    //Quel cote on coupe
    if(inter.nb_passe%2==0){range_x = (inter.xmax-inter.xmin)/2; range_y=0;
        //cout << " passage X" <<endl
        ;}
    else{range_y = (inter.ymax-inter.ymin)/2; range_x=0;
        //cout << " passage y "<< endl;
         }

    //On se balade dans l'arbre
    for(int k=0; k<2; k++){
        //cout << " GO " << k << endl;
        intervale new_int;
        new_int.xmin = inter.xmin+k*range_x;
        new_int.xmax = inter.xmax-(1-k)*range_x;
        new_int.ymin = inter.ymin+k*range_y;
        new_int.ymax = inter.ymax-(1-k)*range_y;
        new_int.nb_passe = inter.nb_passe+1;
        if(in_intervale(new_int, true, range_x, range_y, x1, y1, x2, y2, x3, y3)){
            //cout << " innnnnn"<<endl;
            if(k==0){key.append("a");}
            if(k==1){key.append("b");}
            if(find_zone(new_int, key, x1, y1, x2, y2, x3, y3, img, i)){}
            key=key.substr(0, key.size()-1);
        }
    }
    //cout << " false " << endl;
    return false;
}

bool My_delaunay::in_intervale(intervale &inter, bool cote, double &range_x, double &range_y, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3){
    //cout << " Xmax = " << inter.xmax << " Xmin = " << inter.xmin << " Ymax = " << inter.ymax << " Ymin = " << inter.ymin << endl;
    //cout << " X1 = " << x1 << " Y1 = " << y1 << " X2 = " << x2 << " Y2 = " << y2 << " X3 = " << x3 << " Y3 = " << y3 << endl;
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

void My_delaunay::build_map_b(image &img){
    int k = 0;
    double t_area;
    time_t init_t = time(NULL);
    for(int i=0; i<vect_triangles.size(); i+=3) {
        time_t to = time(NULL);
        //cout << "#####################################################################triangle = " << k << " / "<<vect_triangles.size()<< "##############################"<<endl;
        k=k+1;

        // Coins du triangle version x,y
        double x1 = img.points[2 * vect_triangles[i]];
        double y1 = img.points[2 * vect_triangles[i] + 1];
        double x2 = img.points[2 * vect_triangles[i + 1]];
        double y2 = img.points[2 * vect_triangles[i + 1] + 1];
        double x3 = img.points[2 * vect_triangles[i + 2]];
        double y3 = img.points[2 * vect_triangles[i + 2] + 1];

        area(x1, y1, x2, y2, x3, y3, t_area);
        cout <<i << " / " << vect_triangles.size()<<endl;
        if(t_area>img.max_area){break;}

        intervale inter_init;
        inter_init.xmin = img.min_x;
        inter_init.xmax = img.max_x;
        inter_init.ymin = img.min_y;
        inter_init.ymax = img.max_y;
        inter_init.nb_passe = 0;
        string key;

        find_zone(inter_init,key, x1, y1, x2, y2, x3, y3, img, i);

        int p = floor(i/(vect_triangles.size()-1)*100);
        //time_t t_now = time(NULL);
        //time_t gap = t_now - to;
        //int estimation = ceil((gap*(vect_triangles.size()-1-i)));
        //cout << "\r[" << p << " %]" << " Taille map = " << zones_b.size() << "      "  << i << " / " << vect_triangles.size() << flush;
    }
    //for(auto item : zones_b)
    // {
    //	cout << " key = " << item.first << " taille liste = " << item.second.size()<< endl;
    //}
    //time_t t_end = time(NULL);
    //cout << " temps total = " << t_end - init_t<< endl;
}

void My_delaunay::area(double x1, double y1, double x2, double y2, double x3, double y3, double &area){
       double ab = pow(pow(x2-x1,2)+pow(y2-y1,2), 0.5);
       double bc = pow(pow(x3-x2,2)+pow(y3-y2,2), 0.5);
       double ca = pow(pow(x1-x3,2)+pow(y1-y3,2), 0.5);
       double p = 0.5*(ab+bc+ca);
       area = pow(p*(p-ab)*(p-bc)*(p-ca), 0.5);
}