#include <iostream>
#include <bitset>
#include <fstream>
#include <proj.h>
#include <vector>
#include <string>
#include <map>
#include "image.h"
#include "My_delaunay.h"
#include <list>

using namespace std;

void create_image();



int main()
{
    string name = "rade_brest.txt";
    image img(720, 6);
    My_delaunay dt;

    img.read_file(dt, name);

    dt.make_delaunator(img);
    //dt.build_map_b(img);

    //img.build_img_2(dt, "rade_brest.pgm");
    img.build_img_pas_opti(dt, "rade_t.pgm");

    return EXIT_SUCCESS;
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

    ofstream f("img_test.ppm", ios::binary);
    f << "P5\n" << width << " " << height << "\n" <<"255\n";
    for(int i = 0; i< height; i++)
    {
        f.write((char *) image[i], width);
    }

    f.close();
}


