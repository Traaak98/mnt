#include <iostream>
#include <stdio.h>
#include <bitset>
#include <fstream>
//#include <Proj.h>

using namespace std;

int main()
{
    //// Initialisation des référentiels de coordonnées :
    //PJ* P = proj_create_crs_to_crs(
    //        PJ_DEFAULT_CTX,
    //        "+proj=longlat +datum=WGS84",
    //        "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
    //        NULL);
    //// ^ peut prendre du temps, à ne faire qu'une seule fois
//
    //// Deux coordonnées à exprimer dans des référentiels différents
    //PJ_COORD geo_coord, cartesian_coord;
//
    //// Position géographique en latitude/longitude de l'ENSTA
    //geo_coord.lpzt.lam = -4.4720707; // longitude
    //geo_coord.lpzt.phi = 48.41908; // latitude
    //geo_coord.lpzt.z = 0.; // le z dans le référentiel ellipsoidale n'est pas nécessaire pour la projection
//
    //// Projection géographique
    //cartesian_coord = proj_trans(P, PJ_FWD, geo_coord);
    //cout << "(" << geo_coord.lpzt.lam << "," << geo_coord.lpzt.phi << ")"
    //     << " -> "
    //     << "(" << cartesian_coord.xy.x << "," << cartesian_coord.xy.y << ")";

    int width = 13, height = 13;

    uint8_t image[13][13] = {
            { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},
            { 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47},
            { 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63},
            { 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79},
            { 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95 },
            { 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111},
            { 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127},
            { 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143},
            { 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159},
            { 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175},
            { 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191},
            { 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207}};

    ofstream f("img_test.pgm", ios::binary);
    f << "P5\n" << width << " " << height << "\n" <<"255\n";
    for(int i = 0; i< height; i++)
    {
        f.write((char *) image[i], width);
    }

    f.close();
    return EXIT_SUCCESS;
}

