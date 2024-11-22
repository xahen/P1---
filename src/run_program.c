#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "delivery_algorithm.h"
#include "delivery_sim.h"
#include "resource_calculation.h"

int main(void) {
    srand(time(NULL));

    for (int i = 0; i < 15; i++) {
        print_package(3, generate_random_package());
    }

    return 0;
}
