//
// Created by apolline on 08/12/22.
//

#include "image.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <proj.h>
#include "My_delaunay.h"
#include <iomanip>

using namespace std;

image::image(int nb_pixels, int nb_zones) : nb_pixel_l(nb_pixels), nb_zone(nb_zones) {
    nb_pixel_h = 0;
    densite = 0;
    max_x = -1e10;
    max_y = -1e10;
    min_x = 1e10;
    min_y = 1e10;
    max_z = -1e10;
    min_z = 1e10;
    nb_donnee = 0;
    max_area = 1;

}

image::image(int nb_pixels, int nb_zones, double max_x, double max_y, double min_x, double min_y) :
            nb_pixel_l(nb_pixels), nb_zone(nb_zones) , min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y){

    max_z = -1e10;
    min_z = 1e10;
    nb_donnee = 0;
    update_densite();
    update_nb_pixel_l();
}




void image::update_min_max(double x, double y, double z){
    if(x<min_x){min_x = floor(x);}
    if(x>max_x){max_x = ceil(x);}
    if(y<min_y){min_y = floor(y);}
    if(y>max_y){max_y = ceil(y);}
    if(z<min_z){min_z = floor(z);}
    if(z>max_z){max_z = ceil(z);}

}

void image::update_densite()
{
    densite = nb_pixel_l/(max_x - min_x);
}

void image::update_nb_pixel_l()
{
    nb_pixel_h = ceil(densite * (max_y - min_y));
}

PJ* image::init_proj(){
    PJ* P = proj_create_crs_to_crs(
            PJ_DEFAULT_CTX,
            "+proj=longlat +datum=WGS84",
            "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
            NULL);

    if (P == 0) {
        cout << "Failed to create transformation object." << endl;
    }
    return P;
}

void image::projection(double lon, double lat, double z, double &x, double &y, PJ* P){

    // Deux coordonnées à exprimer dans des référentiels différents
    PJ_COORD geo_coord, cartesian_coord;

    // Position géographique en latitude/longitude de l'ENSTA
    geo_coord.lpzt.lam = lon; // longitude
    geo_coord.lpzt.phi = lat; // latitude
    geo_coord.lpzt.z = z; // le z dans le référentiel ellipsoidale n'est pas nécessaire pour la projection

    // Projection géographique
    cartesian_coord = proj_trans(P, PJ_FWD, geo_coord);

    x = cartesian_coord.xy.x;
    y = cartesian_coord.xy.y;
}

void image::read_file(My_delaunay &dt, string filename){
    list<double> pts;
    list<double> hts;
    ifstream f(filename);
    string line;
    PJ* P = init_proj();
    if(!f.is_open())
    {
        cout << "Erreur d'ouverture de " << filename << endl;
    }
    else
    {
        while(!f.eof()) {
            nb_donnee = nb_donnee + 1;
            double x, y;
            getline(f, line, ' ');
            double lat = stod(line);
            getline(f, line, ' ');
            double lon = stod(line);
            getline(f, line);
            double h = abs(stod(line));

            projection(lon, lat, h, x, y, P);

            pts.push_back(x);
            pts.push_back(y);

            hts.push_back(h);
            update_min_max(x, y, h);

        }

        vector<double> transforme_pts(pts.begin(), pts.end());
        vector<double> transforme_hts(hts.begin(), hts.end());
        points = transforme_pts;
        dt.hauteurs = transforme_hts;
        update_densite();
        update_nb_pixel_l();
    }

    proj_destroy(P);
    f.close();

}

void image::find_color(double pz, int &val1, int &val2, int &val3){
    int pos = round(255*(pz-min_z)/(max_z-min_z));
    val1 = round(haxby[pos][0]*255);
    val2 = round(haxby[pos][1]*255);
    val3 = round(haxby[pos][2]*255);
}

bool image::find_zone(intervale inter, string &key, double &x, double &y, My_delaunay &dt, int &val1, int &val2, int &val3){
    if(inter.nb_passe == 0){
        string key = "";
    }
    double range_x;
    double range_y;

    //Condition arret:
    if(nb_zone == inter.nb_passe){

        double a;
        double b;
        double c;
        double d;

        if(dt.zones_b.count(key)>0){
            vector<long unsigned int> transforme_map_list(dt.zones_b[key].begin(), dt.zones_b[key].end());
            for (int i = 0; i < transforme_map_list.size(); i += 3) {
                // Coins du triangle version x,y
                double x1 = points[2 * transforme_map_list[i]];
                double y1 = points[2 * transforme_map_list[i]+ 1];
                double x2 = points[2 * transforme_map_list[i+1]];
                double y2 = points[2 * transforme_map_list[i+1] + 1];
                double x3 = points[2 * transforme_map_list[i+2]];
                double y3 = points[2 * transforme_map_list[i+2]+ 1];

                if (dt.in_triangle(x, y, x1, y1, x2, y2, x3, y3)) {

                    // On recupere les hauteurs
                    double z1 = dt.hauteurs[transforme_map_list[i]];
                    double z2 = dt.hauteurs[transforme_map_list[i+1]];
                    double z3 = dt.hauteurs[transforme_map_list[i+2]];

                    dt.find_plane(x1, y1, z1, x2, y2, z2, x3, y3, z3, a, b, c, d);
                    double z = (-d + a * x + b * y) / c;
                    if (z < min_z) { z = min_z; }
                    if (z > max_z) { z = max_z; }
                    find_color(z, val1, val2, val3);
                    break;
                }
            }
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
        if(in_intervale(new_int, cote, range_x, range_y, x, y)){
            if(k==0){key.append("a");}
            if(k==1){key.append("b");}
            if(find_zone(new_int, key, x, y,dt, val1, val2, val3)){}
            key=key.substr(0, key.size()-1);
        }
    }
    return false;
}

bool image::in_intervale(intervale &inter, bool cote, double &range_x, double &range_y, double &x1, double &y1){
     //cote : true -> gauche/haut false -> droite/bas
    if(range_x!=0){
        if(cote){if(x1<=inter.xmax){return true;}}
        else{if(x1>=inter.xmin){return true;}}
    }
    if(range_y!=0){
        if(cote){if(y1<=inter.ymax){return true;}}
        else{if(y1>=inter.ymin){return true;}}
    }
    return false;
}

void image::build_img(My_delaunay &dt, string filename){
    ofstream f(filename, ios::binary);
    f << "P6\n" << nb_pixel_l << " " << nb_pixel_h << "\n" <<"255\n";
    for(int py = 0; py < nb_pixel_h; py++) {
        for (int px = 0; px < nb_pixel_l; px++) {

            int val1 = 1;
            int val2 = 1;
            int val3 = 1;

            cout << " px = " << px << " py = " << py << endl;

            double x = px/densite+min_x;
            double y = max_y-py/densite;

            string key;

            intervale inter_init;
            inter_init.xmin = min_x;
            inter_init.xmax = max_x;
            inter_init.ymin = min_y;
            inter_init.ymax = max_y;
            inter_init.nb_passe = 0;

            find_zone(inter_init, key, x, y, dt, val1, val2, val3);

            f << reinterpret_cast<char*>(&val1) << reinterpret_cast<char*>(&val2) << reinterpret_cast<char*>(&val3);
        }
    }
}

image::~image(){}