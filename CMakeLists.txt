cmake_minimum_required(VERSION 3.22.1)
project(Projet)

set(CMAKE_CXX_STANDARD 14)

file( # liste des fichiers à compiler
  GLOB_RECURSE # recherche récursive
  source_files # fichiers listés dans une variable source_files
  src/*.cpp # forme des fichiers à rechercher
)


add_executable( # création de l’exécutable binaire du projet
  ${PROJECT_NAME} # contient le nom du binaire
  ${source_files} # liste des fichiers compilés pour le linking
        tests/random_img_pgm.cpp)

target_link_libraries(${PROJECT_NAME} proj)
