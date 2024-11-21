#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "resource_calculation.h"

// This can be a different value. Should we not just calculate it in calculate_trucks()?
#define truck_volume 41.472

// Maybe we should take weight into account as well.
int calculate_trucks(package_t package, double *volume_filled, int truck_amount) {

    double volume = package.height * package.width * package.length;

    if (*volume_filled + volume < truck_volume) {
        *volume_filled += volume;
    } else {
        *volume_filled = 0;
        *volume_filled += volume;
        truck_amount++;
    }

    //printf("%lf\n", *volume_filled);


    return truck_amount;
}