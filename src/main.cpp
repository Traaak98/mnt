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
#include <ctime>

using namespace std;

void create_image();



int main()
{
    time_t init_t = time(NULL);
    string name = "rade_brest.txt";
    image img(720, 21);
    My_delaunay dt;

    img.read_file(dt, name);

    dt.make_delaunator(img);
    dt.build_map(img);

    img.build_img(dt, "rade_brest_op.ppm");
    time_t end_t = time(NULL);

    cout << " Temps de calcul " << end_t - init_t << " secondes" << endl;

    return EXIT_SUCCESS;
}


