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

using namespace std;

image::image(int nb_pixels, int nb_zones) : nb_pixel_h(nb_pixels), nb_zone(nb_zones) {
    nb_pixel_l = 0;
    densite = 0;
    max_x = -1e10;
    max_y = -1e10;
    min_x = 1e10;
    min_y = 1e10;
    max_z = -1e10;
    min_z = 1e10;
    nb_donnee = 0;
}

void image::update_min_max(double x, double y, double z){
    if(x<min_x){min_x = floor(x);}
    if(x>max_x){max_x = ceil(x);}
    if(y<min_y){min_y = floor(y);}
    if(y>max_y){max_y = ceil(y);}
    if(y<min_z){min_z = floor(z);}
    if(y>max_z){max_z = ceil(z);}

}

void image::update_densite()
{
    densite = nb_pixel_h/(max_x - min_x);
}

void image::update_nb_pixel_l()
{
    nb_pixel_l = ceil(densite * (max_y - min_y));
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
    //cout << "(" << geo_coord.lpzt.lam << "," << geo_coord.lpzt.phi << ")"
    //     << " -> "
    //     << "(" << cartesian_coord.xy.x << "," << cartesian_coord.xy.y << ")";
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
            double h = stod(line);

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
    f.close();

}

void image::find_zone(double x, double y, int &zone){

    //conversion coord to pixel
    int x_p = ceil((x-min_x)*densite);
    int y_p = ceil((y-min_y)*densite);

    // Trouver zone
    int part_x = x_p/(nb_pixel_h/(nb_zone-1));
    int part_y = y_p/(nb_pixel_l/(nb_zone-1));
    zone = (part_y+(part_x)*nb_zone)+1;

}

void image::find_zone(int x_p, int y_p, int &zone){
    int part_x = x_p/(nb_pixel_h/(nb_zone-1));
    int part_y = y_p/(nb_pixel_l/(nb_zone-1));
    zone = (part_y+(part_x)*nb_zone)+1;
}

void image::build_img(My_delaunay &dt, string filename){
    int zone_p;
    for(int py = 0; py < nb_pixel_l+1; py++){
        for(int px = 0; px < nb_pixel_h+1; px++){
            find_zone(px, py, zone_p);

        }
    }
}

image::~image(){}