cmake_minimum_required(VERSION 3.22.1)
project(create_raster)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "-g")

file( # liste des fichiers à compiler
  GLOB_RECURSE # recherche récursive
  source_files # fichiers listés dans une variable source_files
  src/*.cpp # forme des fichiers à rechercher
)


add_executable( # création de l’exécutable binaire du projet
  ${PROJECT_NAME} # contient le nom du binaire
  ${source_files} # liste des fichiers compilés pour le linking
        )

target_link_libraries(${PROJECT_NAME} proj)

ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)
