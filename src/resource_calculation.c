#include <stdio.h>

#include "resource_calculation.h"
#include "delivery_sim.h"

// This can be a different value. Should we not just calculate it in calculate_trucks()?
#define truck_volume 41.472

// Maybe we should take weight into account as well.
void calculate_trucks(package_t package, double *volume_filled, int *truck_amount) {
    double volume = package.height * package.width * package.length;

    if (*volume_filled + volume < truck_volume) {
        *volume_filled += volume;
    } else {
        *volume_filled = 0;
        *volume_filled += volume;
        *truck_amount += 1;
    }
}

void truck_test(int orders) {
    double volume_filled = 0;
    int truck_amount = 0;

    for (int i = 0; i < orders; i++) {
        package_t package = generate_random_package();
        calculate_trucks(package, &volume_filled, &truck_amount);
    }

    printf("%d", truck_amount);
}