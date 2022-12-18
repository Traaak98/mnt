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

void image::find_zone(double x, double y, int &zone){

    //conversion coord to pixel
    //int x_p = ceil((x-min_x)*densite);
    //int y_p = ceil((y-min_y)*densite);

    // Trouver zone
    int part_x = floor((x-min_x)/((max_x-min_x)/nb_zone));
    int part_y = floor((y-min_y)/((max_y-min_y)/nb_zone));
    if(part_y>= nb_zone){part_y=nb_zone-1;}
    if(part_x>= nb_zone){part_x=nb_zone-1;}
    cout << "part x = " << (x-min_x)/((max_x-min_x)/nb_zone) << " part y = " << (y-min_y)/((max_y-min_y)/nb_zone) <<endl;
    cout << "part x = " << part_x << " part y = " << part_y <<endl;
    zone = (part_x+part_y*nb_zone)+1;

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

void image::build_img(My_delaunay &dt, string filename) {
    int zone_p;
    int val1 = 1;
    int val2 = 1;
    int val3 = 1;

    double a;
    double b;
    double c;
    double d;

    double area;

    bool first = 0;

    ofstream f(filename);
    f << "P3\n" << nb_pixel_l << " " << nb_pixel_h << "\n" << "255\n";
    ofstream m("pixel_zone.txt");
    m<<"[";
    for (int py = 0; py < nb_pixel_h; py++) {
        for (int px = 0; px < nb_pixel_l; px++) {
            cout.precision(17);
            double x = px/densite+min_x;
            double y = max_y-py/densite ;

            cout << " PX = " << px << " PY = " << py << endl;
            cout << " Xmax = " << max_x << " Xmin = " << min_x << " Ymax = " << max_y << " Ymin = " << min_y << endl;
            find_zone(x, y, zone_p);
            if (dt.zones.count(zone_p)) {
                if(zone_p == 24){
                    m<<setprecision(17) << px / densite + min_x<<","<<py / densite + min_y<<",";
                }
                for (int i = 0; i < dt.zones[zone_p].size(); i += 3) {

                    // Coins du triangle version x,y
                    double x1 = points[2 * dt.vect_triangles[i]];
                    double y1 = points[2 * dt.vect_triangles[i] + 1];
                    double x2 = points[2 * dt.vect_triangles[i + 1]];
                    double y2 = points[2 * dt.vect_triangles[i + 1] + 1];
                    double x3 = points[2 * dt.vect_triangles[i + 2]];
                    double y3 = points[2 * dt.vect_triangles[i + 2] + 1];

                    // Pour les bords de l'image
                    dt.area(x1, y1, x2, y2, x3, y3, area);
                    if(area>max_area){break;}

                    if (dt.in_triangle(x, y, x1, y1, x2, y2, x3, y3)) {

                        // On recupere les hauteurs
                        double p1z = dt.hauteurs[dt.vect_triangles[i]];
                        double p2z = dt.hauteurs[dt.vect_triangles[i + 1]];
                        double p3z = dt.hauteurs[dt.vect_triangles[i + 2]];

                        dt.find_plane(x1, y1, p1z, x2, y2, p2z, x3, y3, p3z, a, b, c, d);
                        double z = (-d + a * x + b * y) / c;
                        cout << " VRAI Z = " << z << endl;
                        if (z < min_z) { z = min_z; }
                        if (z > max_z) { z = max_z; }

                        break;
                    }
                    if (i == dt.zones[zone_p].size() - 3) {
                        cout << "Bizarre pas de point dans les triangles alors que meme zone" << endl;
                        if(first==0){
                            ofstream g("zone_tri.txt");
                            first = 1;
                            g << zone_p << "\n";
                            for (int l = 0; l < dt.zones[zone_p].size(); l += 3){
                                double x1b = points[2 * dt.vect_triangles[l]];
                                double y1b = points[2 * dt.vect_triangles[l] + 1];
                                double x2b = points[2 * dt.vect_triangles[l + 1]];
                                double y2b = points[2 * dt.vect_triangles[l + 1] + 1];
                                double x3b = points[2 * dt.vect_triangles[l + 2]];
                                double y3b = points[2 * dt.vect_triangles[l + 2] + 1];
                                g<<setprecision(15)<<x1b<<","<<y1b<<","<<x2b<<","<<y2b<<","<<x3b<<","<<y3b<<",";
                            }
                            g.close();
                        }
                    }
                }
                f << val1 << " " << val2 << " " << val3 << "\n";
            }
        }
    }
    m.close();
    f.close();
}

void image::build_img_pas_opti(My_delaunay &dt, string filename){

    double a;
    double b;
    double c;
    double d;

    ofstream f(filename, ios::binary);
    f << "P6\n" << nb_pixel_l << " " << nb_pixel_h << "\n" <<"255\n";
    for(int py = 0; py < nb_pixel_h; py++){
        for(int px = 0; px < nb_pixel_l; px++){
            int val1 = 1;
            int val2 = 1;
            int val3 = 1;

            double area;

            cout.precision(17);
            cout << " px = " << px << " py = " << py << endl;
                for(int i=0; i<dt.vect_triangles.size(); i+=3){

                    // Coins du triangle version x,y
                    double x1 = points[2 * dt.vect_triangles[i]];
                    double y1 = points[2 * dt.vect_triangles[i]+1];
                    double x2 = points[2 * dt.vect_triangles[i+1]];
                    double y2 = points[2 * dt.vect_triangles[i+1]+1];
                    double x3 = points[2 * dt.vect_triangles[i+2]];
                    double y3 = points[2 * dt.vect_triangles[i+2]+1];

                    double x = px/densite+min_x;
                    double y = max_y-py/densite;

                    if(dt.in_triangle(x, y, x1, y1, x2, y2, x3, y3)){

                        // Pour les bords de l'image
                        dt.area(x1, y1, x2, y2, x3, y3, area);
                        cout << "area = " << area << endl;
                        if(area>max_area){cout << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!out!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;break;}

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
                        if(z < min_z){z=min_z;}
                        if(z > max_z){z=max_z;}
                        cout << " VRAI Z = " << z << endl;
                        find_color(z, val1, val2, val3);
                        break;
                    }
                }
            f << reinterpret_cast<char*>(&val1) << reinterpret_cast<char*>(&val2) << reinterpret_cast<char*>(&val3);
            //f << val1 << " " << val2 << " " << val3 << "\n";
        }
    }
    f.close();
}

bool image::find_zone(intervale inter, string &key, double &x, double &y, My_delaunay &dt, ofstream &f){
    //cout << " passe " << endl;
    if(inter.nb_passe == 0){
        string key = "";
    }
    double range_x;
    double range_y;
    //cout << inter.nb_passe << endl;
    //cout<< key << endl;

    //Condition arret:
    if(nb_zone == inter.nb_passe){
        int val1 = 1;
        int val2 = 1;
        int val3 = 1;

        double a;
        double b;
        double c;
        double d;

        if(!dt.zones_b.count(key)){f << val1 << " " << val2 << " " << val3 << "\n";}
        else {
            for (int i = 0; i < dt.zones_b[key].size(); i += 3) {
                // Coins du triangle version x,y
                double x1 = points[2 * dt.vect_triangles[i]];
                double y1 = points[2 * dt.vect_triangles[i] + 1];
                double x2 = points[2 * dt.vect_triangles[i + 1]];
                double y2 = points[2 * dt.vect_triangles[i + 1] + 1];
                double x3 = points[2 * dt.vect_triangles[i + 2]];
                double y3 = points[2 * dt.vect_triangles[i + 2] + 1];

                if (dt.in_triangle(x, y, x1, y1, x2, y2, x3, y3)) {

                    // On recupere les hauteurs
                    double z1 = dt.hauteurs[dt.vect_triangles[i]];
                    double z2 = dt.hauteurs[dt.vect_triangles[i + 1]];
                    double z3 = dt.hauteurs[dt.vect_triangles[i + 2]];

                    dt.find_plane(x1, y1, z1, x2, y2, z2, x3, y3, z3, a, b, c, d);
                    cout << a << " " << b << " " << c << " " << d <<endl;
                    double z = (-d + a * x + b * y) / c;
                    cout << " VRAI Z = " << z << endl;
                    if (z < min_z) { z = min_z; }
                    if (z > max_z) { z = max_z; }
                    find_color(z, val1, val2, val3);
                    break;
                }
            }
            f << val1 << " " << val2 << " " << val3 << "\n";
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
        if(in_intervale(new_int, true, range_x, range_y, x, y)){
            //cout << " innnnnn"<<endl;
            if(k==0){key.append("a");}
            if(k==1){key.append("b");}
            if(find_zone(new_int, key, x, y,dt, f)){}
            key=key.substr(0, key.size()-1);
        }
    }
    //cout << " false " << endl;
    return false;
}

bool image::in_intervale(intervale &inter, bool cote, double &range_x, double &range_y, double &x1, double &y1){
    //cout << " Xmax = " << inter.xmax << " Xmin = " << inter.xmin << " Ymax = " << inter.ymax << " Ymin = " << inter.ymin << endl;
    //cout << " X1 = " << x1 << " Y1 = " << y1 << " X2 = " << x2 << " Y2 = " << y2 << " X3 = " << x3 << " Y3 = " << y3 << endl;
    // cote : true -> gauche/haut false -> droite/bas
    if(range_x!=0){
        if(cote){
            if(x1<=inter.xmax){return true;}
        }
        else{
            if(x1>=inter.xmin){return true;}
        }
    }
    if(range_y!=0){
        if(cote){
            if(y1<=inter.ymax){return true;}
        }
        else{
            if(y1>=inter.ymin){return true;}
        }
    }
    return false;
}

void image::build_img_2(My_delaunay &dt, std::string filename){
    ofstream f(filename);
    f << "P3\n" << nb_pixel_l << " " << nb_pixel_h << "\n" <<"255\n";
    for(int py = 0; py < nb_pixel_h; py++) {
        for (int px = 0; px < nb_pixel_l; px++) {

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

            find_zone(inter_init, key, x, y, dt, f);
        }
    }
}

image::~image(){}