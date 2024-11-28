#include <stdio.h>

#include "resource_calculation.h"
#include "delivery_sim.h"

// This can be a different value. Should we not just calculate it in calculate_trucks()?
const double TRUCK_MAX_VOLUME = 20.0;
const double TRUCK_MAX_WEIGHT = 1950.0;

// Maybe we should take weight into account as well.
void calculate_trucks(package_t package, double *volume_filled, double *weight_filled, int *truck_amount) {
    double package_volume = package.height * package.width * package.length;

    if (*volume_filled + package_volume < TRUCK_MAX_VOLUME && *weight_filled + package.weight < TRUCK_MAX_WEIGHT) {
        *volume_filled += package_volume;
        *weight_filled += package.weight;
    } else {
        *volume_filled = 0;
        *weight_filled = 0;
        *volume_filled += package_volume;
        *truck_amount += 1;
    }
}