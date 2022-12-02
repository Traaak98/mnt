#include <iostream>
#include <bitset>
#include <fstream>
#include <proj.h>
#include <vector>
#include <string>

using namespace std;

void create_image();
float projection(double lon, double lat, double z);
void equation_plan(double xA, double yA,double zA,double xB,double yB,double zB,double xC,double yC,double zC);
void read_file(vector<double> &coord, map<pair<double,double>, double> &hauteur, string filename);

int main()
{
    vector<double> coord;
    map<pair<double,double>, double> hauteur;
    read_file(coord, hauteur, "rade_brest.txt");

    for(double i : coord)
    {
        cout << i << endl;
    }

    for(auto item : hauteur)
    {
    	cout << item.first.first << item.first.second << " " << item.second << endl;
    }

    return EXIT_SUCCESS;
}

// fonction qui permet de sortir un vecteur de coordonnées et une map de hauteur à partir d'un fichier texte de données
void read_file(vector<double> &coord, map<pair<double,double>, double> &hauteur, string filename)
{
    ifstream f(filename);
    string line;
    if(!f.is_open())
    {
        cout << "Erreur d'ouverture de " << filename << endl;
    }
    else
    {
        while(!f.eof()) {
            double x, y;
            getline(f, line, ' ');
            double lat = stod(line);
            getline(f, line, ' ');
            double lon = stod(line);
            getline(f, line);
            double h = stod(line);

            // projection
            [x, y] = projection(lat, lon, h);


            coord.push_back(x);
            coord.push_back(y);
            hauteur[pair<double,double>(x, y)] = h;
        }
    }
}

float projection(double lon, double lat, double z){
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
    //cout << "(" << geo_coord.lpzt.lam << "," << geo_coord.lpzt.phi << ")"
    //     << " -> "
    //     << "(" << cartesian_coord.xy.x << "," << cartesian_coord.xy.y << ")";
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


