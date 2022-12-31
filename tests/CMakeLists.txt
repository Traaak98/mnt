project(test_constructeur_image_1)
file( # liste des fichiers à compiler
        GLOB_RECURSE # recherche récursive
        source_files # fichiers listés dans une variable source_files
        ../src/image.cpp
        ../src/My_delaunay.cpp
        ../src/delaunator.hpp
        ../tests/test_constructeur.cpp
        )
add_executable( # création de l’exécutable binaire du projet
        ${PROJECT_NAME} # contient le nom du binaire
        ${source_files} # liste des fichiers compilés pour le linking
        )
target_link_libraries(${PROJECT_NAME} proj)

project(test_update_min_max)
file( # liste des fichiers à compiler
        GLOB_RECURSE # recherche récursive
        source_files # fichiers listés dans une variable source_files
        ../src/image.cpp
        ../src/My_delaunay.cpp
        ../src/delaunator.hpp
        ../tests/test_update_min_max.cpp
        )
add_executable( # création de l’exécutable binaire du projet
        ${PROJECT_NAME} # contient le nom du binaire
        ${source_files} # liste des fichiers compilés pour le linking
        )
target_link_libraries(${PROJECT_NAME} proj)

project(test_update_densite)
file( # liste des fichiers à compiler
        GLOB_RECURSE # recherche récursive
        source_files # fichiers listés dans une variable source_files
        ../src/image.cpp
        ../src/My_delaunay.cpp
        ../src/delaunator.hpp
        ../tests/test_update_densite.cpp
        )
add_executable( # création de l’exécutable binaire du projet
        ${PROJECT_NAME} # contient le nom du binaire
        ${source_files} # liste des fichiers compilés pour le linking
        )
target_link_libraries(${PROJECT_NAME} proj)

ADD_TEST(test_constructeur_image_1 test_constructeur_image_1)
ADD_TEST(test_update_min_max test_update_min_max)
ADD_TEST(test_update_densite test_update_densite)