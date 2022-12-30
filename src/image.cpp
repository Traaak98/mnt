/**
 * \class image
 * \author Apolline de Vaulchier du Deschaux
 * \date 08/12/2022
 *
 * Permet de creer un objet image. Cet objet permet de travailler plus facilement sur l'image en pixel et de garder les paramètres
 * clés comme les coordonnés des points, le nombre de pixels, les bornes, etc.
 */

#include "image.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <proj.h>
#include "My_delaunay.h"

using namespace std;

// Constructeur
image::image(int nb_pixels, int nb_zones) : nb_pixel_l(nb_pixels), nb_zone(nb_zones) {
    nb_pixel_h = 0;
    densite = 0;
    max_x = -1e10;
    max_y = -1e10;
    min_x = 1e10;
    min_y = 1e10;
    max_z = -1e10;
    min_z = 1e10;
    max_area = 1;

}

// Deuxieme constructeur pour les tests
image::image(int nb_pixels, int nb_zones, double max_x, double max_y, double min_x, double min_y) :
            nb_pixel_l(nb_pixels), nb_zone(nb_zones) , min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y){
    max_z = -1e10;
    min_z = 1e10;
    update_densite();
    update_nb_pixel_h();
}


void image::update_min_max(double x, double y, double z){
    // Mise a jour des bornes
    if(x<min_x){min_x = floor(x);}
    if(x>max_x){max_x = ceil(x);}
    if(y<min_y){min_y = floor(y);}
    if(y>max_y){max_y = ceil(y);}
    if(z<min_z){min_z = floor(z);}
    if(z>max_z){max_z = ceil(z);}

}

void image::update_densite()
{
    // Calcul densite (pixel/m)
    densite = nb_pixel_l/(max_x - min_x);
}

void image::update_nb_pixel_h()
{
    // Calcul nombre de pixel en hauteur en fonction de la densite et des bornes
    nb_pixel_h = ceil(densite * (max_y - min_y));
}

PJ* image::init_proj(){
    // Initialisation de la projection
    PJ* P = proj_create_crs_to_crs(
            PJ_DEFAULT_CTX,
            "+proj=longlat +datum=WGS84",
            "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
            NULL);

    if (P == 0) {
        // Identifie si la projection est bien initialisee
        cout << "Failed to create transformation object." << endl;
    }
    return P;
}

void image::projection(double lon, double lat, double z, double &x, double &y, PJ* P){

    // Deux coordonnees a exprimer dans des referentiels differents
    PJ_COORD geo_coord, cartesian_coord;

    // Position geographique en latitude/longitude de l'ENSTA
    geo_coord.lpzt.lam = lon; // longitude
    geo_coord.lpzt.phi = lat; // latitude
    geo_coord.lpzt.z = z; // le z dans le referentiel ellipsoidale n'est pas necessaire pour la projection

    // Projection geographique
    cartesian_coord = proj_trans(P, PJ_FWD, geo_coord);

    x = cartesian_coord.xy.x;
    y = cartesian_coord.xy.y;
}

void image::read_file(My_delaunay &dt, string filename){
    // Initialisation des listes (push_bach plus efficace que pour les vectors)
    list<double> pts;
    list<double> hts;

    //Ouverture du fichier de donnnee
    ifstream f(filename);
    string line;

    // Initialisation de la projection
    PJ* P = init_proj();

    if(!f.is_open())
    {
        // Verification de l'ouverture du fichier de donneee
        cout << "Erreur d'ouverture de " << filename << endl;
    }
    else
    {
        while(!f.eof()) {
            // On parcours le fichier
            cout << "\rLecture du fichier /" << flush;

            double x, y;
            getline(f, line, ' ');
            double lat = stod(line); // latitude
            getline(f, line, ' ');
            double lon = stod(line); // longitude
            getline(f, line);
            double h = abs(stod(line)); // hauteur

            projection(lon, lat, h, x, y, P); // Projection realisee pour obtenir x et y

            // Mise a jour des listes
            pts.push_back(x);
            pts.push_back(y);
            hts.push_back(h);

            // Mise a jour des bornes
            update_min_max(x, y, h);

            cout << "\rLecture du fichier -" << flush;

        }

        // Transformation des listes en vecteur
        vector<double> transforme_pts(pts.begin(), pts.end());
        vector<double> transforme_hts(hts.begin(), hts.end());

        // Mise a jour des variables points, hauteurs, densite et nb_pixel_h de la class image
        points = transforme_pts;
        hauteurs = transforme_hts;
        update_densite();
        update_nb_pixel_h();
    }

    proj_destroy(P); // Destruction de la projection
    f.close(); // Ferme le fichier de donnee

}

void image::find_color(double pz, int &val1, int &val2, int &val3, double &shadow){
    // Pour une heuteur et une ombre, calcul les indices associes a la bonne couleur dans la colormap haxby.
    // Puis renvoie les bonnes couleurs RVB
    int pos = round(255*(pz-min_z)/(max_z-min_z)); // indice
    val1 = round(haxby[pos][0]*255*shadow); // R
    val2 = round(haxby[pos][1]*255*shadow); // V
    val3 = round(haxby[pos][2]*255*shadow); // B
}

bool image::find_zone(intervale inter, string &key, double &x, double &y, My_delaunay &dt, int &val1, int &val2, int &val3, double &shadow){

    // Initialisation de la cle de la map
    if(inter.nb_passe == 0){
        string key = "";
    }

    // Variables qui correspond à la partie tronquee de l'intervale
    double range_x;
    double range_y;

    //Condition arret:
    if(nb_zone == inter.nb_passe){

        // Parametre du plan associe a un triangle
        double a;
        double b;
        double c;
        double d;

        // si la zone existe alors :
        if(dt.zones.count(key)>0){
            // Transformation de la liste en vecteur pour avoir acces au indices facilement
            vector<long unsigned int> transforme_map_list(dt.zones[key].begin(), dt.zones[key].end());

            // On parcours les triangles de la zone et on regarde si le pixel est dedans :
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
                    double z1 = hauteurs[transforme_map_list[i]];
                    double z2 = hauteurs[transforme_map_list[i+1]];
                    double z3 = hauteurs[transforme_map_list[i+2]];

                    // Calcul du plan associé au triangle
                    dt.find_plane(x1, y1, z1, x2, y2, z2, x3, y3, z3, a, b, c, d);

                    // Calcul de la hauteur du pixel
                    double z = (-d + a * x + b * y) / c;
                    if (z < min_z) { z = min_z; }
                    if (z > max_z) { z = max_z; }

                    // Calcul de l'ombrage
                    find_shadow(shadow, x1, y1, z1, x2, y2, z2, x3, y3, z3);

                    // On trouve la couleur associe a ce pixel
                    find_color(z, val1, val2, val3, shadow);
                    break; // Sortie de la boucle
                }
            }
        }
        return true;
    }

    //Quel cote on coupe : Soit intervale en X, soit intervale en Y
    if(inter.nb_passe%2==0){range_x = (inter.xmax-inter.xmin)/2; range_y=0;}
    else{range_y = (inter.ymax-inter.ymin)/2; range_x=0;}

    //On se balade dans l'arbre
    for(int k=0; k<2; k++){

        // Indique le cote
        bool cote;
        if(k==0){cote=true;}
        if(k==1){cote=false;}

        // Creation du nouvel intervale
        intervale new_int;
        new_int.xmin = inter.xmin+k*range_x;
        new_int.xmax = inter.xmax-(1-k)*range_x;
        new_int.ymin = inter.ymin+k*range_y;
        new_int.ymax = inter.ymax-(1-k)*range_y;
        new_int.nb_passe = inter.nb_passe+1;

        // Est ce que le pixel se trouve dans le nouvel intervale ?
        if(in_intervale(new_int, cote, range_x, range_y, x, y)){
            if(k==0){key.append("a");}
            if(k==1){key.append("b");}
            if(find_zone(new_int, key, x, y,dt, val1, val2, val3, shadow)){}
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

    // Affichage de la progression
    cout<<endl;
    int count = 0;

    // Ouverture et initialisation du fichier associe a l'image
    ofstream f(filename, ios::binary);
    f << "P6\n" << nb_pixel_l << " " << nb_pixel_h << "\n" <<"255\n";

    // Boucle sur tous les pixels de l'image
    for(int py = 0; py < nb_pixel_h; py++) {
        for (int px = 0; px < nb_pixel_l; px++) {

            // Affichage de la progression
            cout.precision(3);
            float p = (float(count + 1 +1)/float(nb_pixel_l*nb_pixel_h))*100;
            cout<<"\rPourcentage de pixel realise : "<<p<<" % "<<flush;
            count=count+1;

            // Initialisation de la couleur sur le noir
            int val1 = 1;
            int val2 = 1;
            int val3 = 1;

            double shadow; // Initialisation de l'ombrage

            // Conversion pixel en m
            double x = px/densite+min_x;
            double y = max_y-py/densite;

            string key; // Initialisation de la cle de la map

            // Initialisation de l'intervale de depart
            intervale inter_init;
            inter_init.xmin = min_x;
            inter_init.xmax = max_x;
            inter_init.ymin = min_y;
            inter_init.ymax = max_y;
            inter_init.nb_passe = 0;

            find_zone(inter_init, key, x, y, dt, val1, val2, val3, shadow);

            // Ecriture de la couleur dans le fichier associe a l'image
            f << reinterpret_cast<char*>(&val1) << reinterpret_cast<char*>(&val2) << reinterpret_cast<char*>(&val3);
        }
    }
}

void image::find_shadow(double &shadow, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3){

    // CHoix du cote d'ou vient la lumiere
    double altitude = (90-45)*M_PI/180;
    double azimuth = (360-315+90)*M_PI/180;

    // Calcul du vecteur normal au triangle
    double n1 = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
    double n2 = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
    double n3 = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);

    shadow = (cos(azimuth)*cos(altitude)*n1+sin(azimuth)*cos(altitude)*n2-sin(altitude)*n3)
            /(sqrt(pow(n1,2)+pow(n2, 2)+pow(n3,2))*sqrt(pow(cos(azimuth)*cos(altitude),2)+pow(sin(azimuth)*cos(altitude),2)+pow(-sin(altitude),2)));
    shadow = (1+shadow)/2; // Normalisation

    if(shadow<0){shadow=0;}
    if(shadow>1){shadow=1;}
}

// Destructeur
image::~image(){}