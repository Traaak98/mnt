#include <iostream>
#include <bitset>
#include <fstream>
#include <proj.h>
#include <vector>
#include <string>
#include <map>
#include "image.h"
#include "My_delaunay.h"

using namespace std;

void create_image();
void equation_plan(double xA, double yA,double zA,double xB,double yB,double zB,double xC,double yC,double zC);


int main()
{
    string name = "rade_brest.txt";
    image img(720, 20);
    My_delaunay dt;

    img.read_file(dt, name);

    dt.make_delaunator(img);

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

    ofstream f("img_test.pgm", ios::binary);
    f << "P5\n" << width << " " << height << "\n" <<"255\n";
    for(int i = 0; i< height; i++)
    {
        f.write((char *) image[i], width);
    }

    f.close();
}

void equation_plan(double xA, double yA,double zA,double xB,double yB,double zB,double xC,double yC,double zC)
{

    double xAb=0,yAb=0,zAb=0,xAc=0,yAc=0,zAc=0,mineurUne=0,mineurDeux=0,mineurTrois=0,termeIndependant=0,verification=0;

    xAb=xB-xA;
    yAb=yB-yA;
    zAb=zB-zA;

    xAc=xC-xA;
    yAc=yC-yA;
    zAc=zC-zA;

    mineurUne=(yAb*zAc)-(zAb*yAc);
    mineurDeux=(-1)*((xAb*zAc)-(zAb*xAc));
    mineurTrois=(xAb*yAc)-(yAb*xAc);
    termeIndependant=((-1*xA*mineurUne)+(-1*yA*mineurDeux)+(-1*zA*mineurTrois));

    printf("L'equation cartesienne est \n\n %lf X + %lf Y + %lf Z + %lf\n\n",mineurUne,mineurDeux,mineurTrois,termeIndependant);
    verification=(xA*mineurUne+yA*mineurDeux+zA*mineurTrois+termeIndependant+xB*mineurUne+yB*mineurDeux+zB*mineurTrois+termeIndependant+xC*mineurUne+yC*mineurDeux+zC*mineurTrois+termeIndependant);
    printf("Verification : On injecte les coordonnees de A,B et C dans l'equation cartesienne, ce qui nous donne %lf (Si cette valeur vaut zero, alors c'est correct)\n\n",verification);

}


