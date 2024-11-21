#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "resource_calculation.h"

#define truck_volume 41.472

package_t temp_package() {
    package_t package;
    package.height = 0.5; // meters
    package.width = 0.5; // meters
    package.length = 0.5; // meters
    package.weight = 1; // kilo
    package.priority = 1; // how fast it needs to be delivered
    package.node_id = 1; // the id of the node it needs to be delivered to
    package.truck_id = 1; // the id of the truck it's being delivered in
    package.delivery_status = NOT_DELIVERED; // the status of it's delivery

    return package;
}


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