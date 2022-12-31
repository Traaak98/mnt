//
// Created by apolline on 30/12/22.
//

#ifndef CREATE_RASTER_BASIC_H
#define CREATE_RASTER_BASIC_H

#include <string>
#include <iostream>
#include <stdexcept>
#include "../src/image.h"
#include <cmath>

#define test_image_1(nb_pixels, nb_passe){ \
  image img(nb_pixels, nb_passe);\
  if( ( img.nb_pixel_l ) != ( nb_pixels ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( img.nb_pixel_l ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( nb_pixels ) )             \
    );                                                              \
  }                                        \
  if( ( img.nb_zone ) != ( nb_passe ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( img.nb_zone ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( nb_passe ) )             \
    );                                                              \
  }\
}

#define test_update_min_max(x, y, z){ \
  image img(10, 5);                   \
  image img_new(10,5);\
  img_new.update_min_max(x, y, z);                                    \
  if( ( img.min_x ) > ( x ) && (img_new.min_x) != (floor(x)) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en min x" )                 \
    );                                                              \
  }                                   \
  if( ( img.min_x ) < ( x ) && (img_new.min_x) != (img.min_x) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en min x" )                 \
    );                                                              \
  }                                   \
  if( ( img.max_x ) > ( x ) && (img_new.max_x) != (img.max_x) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en max x" )                 \
    );                                                              \
  }                                   \
  if( ( img.max_x ) < ( x ) && (img_new.max_x) != (ceil(x)) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en max x" )                 \
    );                                                              \
  }                                   \
    if( ( img.min_y ) > ( y ) && (img_new.min_y) != (floor(y)) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en min y" )                 \
    );                                                              \
  }                                   \
  if( ( img.min_y ) < ( y ) && (img_new.min_y) != (img.min_y) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en min y" )                 \
    );                                                              \
  }                                   \
  if( ( img.max_y ) > ( y ) && (img_new.max_y) != (img.max_y) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en max y" )                 \
    );                                                              \
  }                                   \
  if( ( img.max_y ) < ( y ) && (img_new.max_y) != (ceil(y)) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en max y" )                 \
    );                                                              \
  }                                   \
    if( ( img.min_z ) > ( z ) && (img_new.min_z) != (floor(z)) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en min z" )                 \
    );                                                              \
  }                                   \
  if( ( img.min_z ) < ( z ) && (img_new.min_z) != (img.min_z) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en min z" )                 \
    );                                                              \
  }                                   \
  if( ( img.max_z ) > ( z ) && (img_new.max_z) != (img.max_z) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en max z" )                 \
    );                                                              \
  }                                   \
  if( ( img.max_z ) < ( z ) && (img_new.max_z) != (ceil(z)) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": Pas les bonnes bornes en max z" )                 \
    );                                                              \
  }                                      \
}

#define test_update_densite(nb_pixels, nb_passe, xmax, xmin, valeur){ \
  image img(nb_pixels, nb_passe);                                       \
  img.max_x = xmax;                                                    \
  img.min_x = xmin;                                                    \
  img.update_densite();\
  if( ( img.densite ) != ( valeur ) )                                              \
  {                                                                 \
    throw std::runtime_error(   std::string( __FILE__ )             \
                              + std::string( ":" )                  \
                              + std::to_string( __LINE__ )          \
                              + std::string( " in " )               \
                              + std::string( __PRETTY_FUNCTION__ )  \
                              + std::string( ": " )                 \
                              + std::to_string( ( img.densite ) )             \
                              + std::string( " != " )               \
                              + std::to_string( ( valeur ) )             \
    );                                                              \
  }                                        \
}

#endif //CREATE_RASTER_BASIC_H
