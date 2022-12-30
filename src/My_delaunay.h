/**
 * \class My_delaunay
 * \author Apolline de Vaulchier du Deschaux
 * \date 08/12/2022
 *
 * Permet de creer un objet My_delaunay qui comprend la creation d'un objet delaunator
 * avec l'ajout d'autre variable de classe comme hauteurs, vect_triangles et zones sans
 * devoir modifier la class delaunator issu de l'auteur delfrrr (github).
 */

#ifndef PROJET_MY_DELAUNAY_H
#define PROJET_MY_DELAUNAY_H

#include<vector>
#include<map>
#include"image.h"
#include<list>
#include <string>

class image;

class My_delaunay{

public:
    /** \brief Constructeur : Sans entree, creer l'objet My_delaunay */
    My_delaunay();

    /** \brief Variable : vecteur contenant les indices des triangles. Indice à utiliser avec le vecteur points de l'objet image*/
    std::vector<long unsigned int> vect_triangles;

    /** \brief Variable : map qui pour une zone renvoie les indices des triangles dans cette zone.
     * Elle est construit à partir de l'arbre binaire. */
    std::map<std::string, std::list<long unsigned int>> zones;

    /** \brief Strcuture de class :
     * - nb passe : ID qui identifie à quel niveau de l'arbre l'intervale se situe
     * - xmin, xmax, ymin, ymax : borne de l'intervale
     * NB : En fonction de la découpe dans l'arbre seuls les x ou seuls les y sont utiles*/
    struct intervale{
        int nb_passe;
        double xmin;
        double xmax;
        double ymin;
        double ymax;
    };

    /** \brief Fonction : A partir d'une image, et en particulier de l'attribut point de image, effectue
     * une triangulation. La classe delaunator est utiliser pour faire cette triangulation.
     * Voir : https://github.com/delfrrr/delaunator-cpp */
    void make_delaunator(image &img);

    /** \brief Fonction : Identifie les paramètres de l'équation de plan associe au triangle d'entree*/
    void find_plane(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double &a, double &b, double &c, double &d);

    /** \brief Fonction : Booleen qui exprime si le point se situe dans le triangle ou non*/
    bool in_triangle(double px, double py, double x1, double y1, double x2, double y2, double x3, double y3);

    /** \brief Fonction : Bouleen qui exprime si le triangle en entree est dans l'intervale choisit*/
    bool in_intervale(intervale &inter, bool cote, double &range_x, double &range_y, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3);

    /** \brief Fonction : Fonction recursive qui represente l'arbre binaire. Associe le triangle d'entree à des espaces sur l'image.*/
    bool find_zone(intervale inter, std::string &key, double &x1, double &y1, double &x2, double &y2, double &x3, double &y3, image &img, int &i);

    /** \brief Fonction : Creation de la map zones à partir de la fonction find_zone pour chaque triangle*/
    void build_map(image &img);

    /** \brief Fonction : Calcul l'air d'un triangle*/
    void area(double x1, double y1, double x2, double y2, double x3, double y3, double &area);

    /** \brief Destructeur : */
    ~My_delaunay();
};
#endif //PROJET_MY_DELAUNAY_H
