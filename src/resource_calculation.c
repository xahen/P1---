#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "resource_calculation.h"

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


double calculate_trucks(int orders, package_t package) {

    double volume = package.height * package.width * package.length;

    double truck_volume = 41.472;

    double max_packages = truck_volume / volume; ///spytter ud mændge af pakker

    double trucks_amount = orders / max_packages;

    return trucks_amount;

    


}