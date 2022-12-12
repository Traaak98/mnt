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
    int t = 0;
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
            if(t==2*192077){cout <<" line " << line << endl;}
            if(t == 2*192589){cout << " line " << line << endl;}
            double lat = stod(line);
            getline(f, line, ' ');
            if(t==2*192077){cout <<  " line " << line << endl;}
            if(t == 2*192589){cout << " line " << line << endl;}
            double lon = stod(line);
            getline(f, line);
            double h = stod(line);

            projection(lon, lat, h, x, y, P);

            pts.push_back(x);
            pts.push_back(y);

            hts.push_back(h);
            update_min_max(x, y, h);

            if(t==2*192077){cout << lat<< " " << x << " " << lon << " " << y << endl;}
            if(t == 2*192589){cout << lat<< " " << x << " " << lon << " " << y << endl;}
            t = t+1;
        }

        vector<double> transforme_pts(pts.begin(), pts.end());
        vector<double> transforme_hts(hts.begin(), hts.end());
        points = transforme_pts;
        dt.hauteurs = transforme_hts;
        update_densite();
        update_nb_pixel_l();
    }
    int k =0;
    for(auto item : pts){
        if(k == 2*192077){cout << item << endl;}
        if(k == 2*192589){cout << item << endl;}
        k = k+1;
    }

    proj_destroy(P);
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

void image::find_color(double pz, int &val1, int &val2, int &val3){
    cout << "PZ = " << pz << endl;
    cout << "PZ min = " << min_z << endl;
    cout << "PZ max = " << max_z << endl;

    int pos = round(255*(pz-min_z)/(max_z-min_z));
    cout << pos << endl;
    val1 = round(haxby[pos][0]*255);
    val2 = round(haxby[pos][1]*255);
    val3 = round(haxby[pos][2]*255);
}

void image::build_img(My_delaunay &dt, string filename){
    int zone_p;
    int val1;
    int val2;
    int val3;

    double a;
    double b;
    double c;
    double d;

    ofstream f(filename);
    f << "P3\n" << nb_pixel_l << " " << nb_pixel_h << "\n" <<"255\n";
    for(int py = 0; py < nb_pixel_l+1; py++){
        for(int px = 0; px < nb_pixel_h+1; px++){
            cout << " PX = " << px << " PY = " << py << endl;
            find_zone(px, py, zone_p);
            if(!dt.zones.count(zone_p)){
                val1 = 1;
                val2 = 1;
                val3 = 1;
            }
            else{
                for(int i=0; i<dt.zones[zone_p].size(); i+=3){

                    // Coins du triangle version x,y
                    double x1 = points[2 * dt.vect_triangles[i]];
                    double y1 = points[2 * dt.vect_triangles[i]+1];
                    double x2 = points[2 * dt.vect_triangles[i+1]];
                    double y2 = points[2 * dt.vect_triangles[i+1]+1];
                    double x3 = points[2 * dt.vect_triangles[i+2]];
                    double y3 = points[2 * dt.vect_triangles[i+2]+1];

                    // Coins du triangle version pixel
                    int p1x = ceil((points[2 * dt.vect_triangles[i]]-min_x)*densite);
                    int p1y = ceil((points[2 * dt.vect_triangles[i]+1]-min_y)*densite);
                    int p2x = ceil((points[2 * dt.vect_triangles[i+1]]-min_x)*densite);
                    int p2y = ceil((points[2 * dt.vect_triangles[i+1]+1]-min_y)*densite);
                    int p3x = ceil((points[2 * dt.vect_triangles[i+2]]-min_x)*densite);
                    int p3y = ceil((points[2 * dt.vect_triangles[i+2]+1]-min_y)*densite);



                    if(dt.in_triangle(px, py, p1x, p1y, p2x, p2y, p3x, p3y)){

                        // On recupere les hauteurs
                        double p1z = dt.hauteurs[dt.vect_triangles[i]];
                        double p2z = dt.hauteurs[dt.vect_triangles[i+1]];
                        double p3z = dt.hauteurs[dt.vect_triangles[i+2]];
                        double x = px/densite+min_x;
                        double y = py/densite+min_y;
                        dt.find_plane(x1, y1, p1z, x2, y2, p2z, x3, y3, p3z, a, b, c, d);
                        double z = (-d-a*x-b*y)/c;
                        if(z<min_z){z=min_z;}
                        if(z>max_z){z=max_z;}
                        find_color(z, val1, val2, val3);
                        break;
                    }
                    if(i == dt.zones[zone_p].size() - 3){
                        cout << "Bizarre pas de point dans les triangles alors que meme zone"<< endl;
                        val1 = 1;
                        val2 = 1;
                        val3 = 1;
                    }
                }
            }
            f << val1 << " " << val2 << " " << val3 << "\n";
        }
    }
    f.close();
}

void image::build_img_pas_opti(My_delaunay &dt, string filename){
    int val1 = 1;
    int val2 = 1;
    int val3 = 1;

    double a;
    double b;
    double c;
    double d;

    ofstream f(filename);
    f << "P3\n" << nb_pixel_l << " " << nb_pixel_h << "\n" <<"255\n";
    for(int py = 0; py < nb_pixel_l+1; py++){
        for(int px = 0; px < nb_pixel_h+1; px++){
            cout << " px = " << px << " py = " << py << endl;
            bool end = 0;
                for(int i=0; i<dt.vect_triangles.size(); i+=3){

                    // Coins du triangle version x,y
                    double x1 = points[2 * dt.vect_triangles[i]];
                    double y1 = points[2 * dt.vect_triangles[i]+1];
                    double x2 = points[2 * dt.vect_triangles[i+1]];
                    double y2 = points[2 * dt.vect_triangles[i+1]+1];
                    double x3 = points[2 * dt.vect_triangles[i+2]];
                    double y3 = points[2 * dt.vect_triangles[i+2]+1];

                    // Coins du triangle version pixel
                    int p1x = ceil((points[2 * dt.vect_triangles[i]]-min_x)*densite);
                    int p1y = ceil((points[2 * dt.vect_triangles[i]+1]-min_y)*densite);
                    int p2x = ceil((points[2 * dt.vect_triangles[i+1]]-min_x)*densite);
                    int p2y = ceil((points[2 * dt.vect_triangles[i+1]+1]-min_y)*densite);
                    int p3x = ceil((points[2 * dt.vect_triangles[i+2]]-min_x)*densite);
                    int p3y = ceil((points[2 * dt.vect_triangles[i+2]+1]-min_y)*densite);

                    double x = px/densite+min_x;
                    double y = py/densite+min_y;

                    if(dt.in_triangle_b(x, y, x1, y1, x2, y2, x3, y3) && end == 0){
                        cout << " ind tri 1 = " << dt.vect_triangles[i] << " ind tri 2 = " << dt.vect_triangles[i+1] << " ind tri 3 = " << dt.vect_triangles[i+2] << endl;

                        // On recupere les hauteurs
                        double p1z = dt.hauteurs[dt.vect_triangles[i]];
                        double p2z = dt.hauteurs[dt.vect_triangles[i+1]];
                        double p3z = dt.hauteurs[dt.vect_triangles[i+2]];

                        cout << " Z1 = " << p1z << " Z2 = " << p2z << " Z3 = " << p3z << endl;
                        cout << " X1 = " << x1 << " X2 = " << x2 << " X3 = " << x3 << endl;
                        cout << " Y1 = " << y1 << " Y2 = " << y2 << " Y3 = " << y3 << endl;
                        cout << " X = " << x << " Y = " << y << endl;

                        dt.find_plane(x1, y1, p1z, x2, y2, p2z, x3, y3, p3z, a, b, c, d);
                        double z = (-d+a*x+b*y)/c;
                        cout << " VRAI Z = " << z << endl;
                        if(z<min_z){z=min_z;}
                        if(z>max_z){z=max_z;}
                        find_color(z, val1, val2, val3);
                        end = 1;
                    }
                }
            f << val1 << " " << val2 << " " << val3 << "\n";
        }
    }
    f.close();
}

image::~image(){}