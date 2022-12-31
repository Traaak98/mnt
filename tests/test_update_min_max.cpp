//
// Created by apolline on 30/12/22.
//

#include <string>
#include <stdexcept>

# include "image_test.h"

void testBasic(){
    test_update_min_max(1e11, 0, 0);
    test_update_min_max(-1e11, 0, 0);
    test_update_min_max(0, 1e11, 0);
    test_update_min_max(0, -1e11, 0);
    test_update_min_max(0, 0, 1e11);
    test_update_min_max(0, 0, -1e11);
    test_update_min_max(7, 9.6, 1.111);
}

int main(int, char**){
    testBasic();
}