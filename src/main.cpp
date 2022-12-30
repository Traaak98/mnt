/**
 * \file main.cpp
 * \author Apolline de Vaulchier du Deschaux
 * \date 08/12/2022
 *
 * Programme qui permet de generer une image 2D representant, par niveau de couleurs, les differentes
 * altitudes d'un terrain 2.5D.
 *
 * Entree :
 * argument 1 --> Nom/adresse du fichier de donnees
 * argument 2 --> Nombre de pixel dans la largeur que l'on souhaite
 */

#include <iostream>
#include <proj.h>
#include <string>
#include "image.h"
#include "My_delaunay.h"
#include <ctime>

using namespace std;

int main(int argc, char *argv[])
{
    // Debut du chrono
    time_t init_t = time(NULL);

    // Variable d'entree
    string name = argv[1];   //Nom du fichier de donnees
    string name_img = name.substr(0, name.size()-4) + ".ppm";  //Nom de l'image de sortie
    int nb_pixel_largeur = stoi(argv[2]); //Nombre de pixel dans la largeur
    int nb_passe = 13;  //Nombre de passe effectue dans l'arbre binaire

    // Creation de l'objet image
    image img(nb_pixel_largeur, nb_passe);

    // Creation de l'objet My_delaunay
    My_delaunay dt;

    // Lecture de fichier et projection des points
    img.read_file(dt, name);

    // Triangulation de l'image
    dt.make_delaunator(img);

    // Passage des triangles dans l'arbre binaire
    dt.build_map(img);

    // Fabrication de l'image avec le calcul pour chaque pixel
    img.build_img(dt, name_img);

    // Fin du chrono
    time_t end_t = time(NULL);
    cout << endl;
    cout << " Temps de calcul " << end_t - init_t << " secondes" << endl;

    return EXIT_SUCCESS;
}


