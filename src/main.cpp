#include <iostream>
#include <bitset>
#include <fstream>
#include <proj.h>

using namespace std;

void create_image();
float projection(float lon, float lat, float z);

struct point{
    Cell *start;
    Cell *end;
    int larg;
    int lon;
};

int main()
{

    float x=10, y=10;
    [x, y] = projection(-4.4720707, 48.41908, 0.);
    cout << "test : " << x << " et " << y <<endl;

    return EXIT_SUCCESS;
}

float projection(float lon, float lat, float z){
    // Initialisation des référentiels de coordonnées :
    PJ* P = proj_create_crs_to_crs(
            PJ_DEFAULT_CTX,
            "+proj=longlat +datum=WGS84",
            "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
            NULL);

    if (P == 0) {
        fprintf(stderr, "Failed to create transformation object.\n");
        return 1;
    }

    // Deux coordonnées à exprimer dans des référentiels différents
    PJ_COORD geo_coord, cartesian_coord;

    // Position géographique en latitude/longitude de l'ENSTA
    geo_coord.lpzt.lam = lon; // longitude
    geo_coord.lpzt.phi = lat; // latitude
    geo_coord.lpzt.z = z; // le z dans le référentiel ellipsoidale n'est pas nécessaire pour la projection

    // Projection géographique
    cartesian_coord = proj_trans(P, PJ_FWD, geo_coord);
    cout << "(" << geo_coord.lpzt.lam << "," << geo_coord.lpzt.phi << ")"
         << " -> "
         << "(" << cartesian_coord.xy.x << "," << cartesian_coord.xy.y << ")";
    return [cartesian_coord.xy.x, cartesian_coord.xy.y];
}

void create_image()
{
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
}

