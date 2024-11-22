#include <stdio.h>
#include <stdlib.h>

#include "package_distribution.h"

truck_t create_truck(int id) {
    truck_t truck;
    truck.id = id;

    truck.average_speed = 60;
    truck.max_volume = 19;
    truck.max_weight = 3500;

    return truck;
}

package_t create_package(int priority, int node_id, int truck_id, double height, double width, double length, double weight) {
    package_t package;
    package.priority = priority;
    package.node_id = node_id;
    package.truck_id = truck_id;
    package.height = height;
    package.width = width;
    package.length = length;
    package.weight = weight;

    package.delivery_status = NOT_DELIVERED;

    return package;
}

node_t create_node(int location_x, int location_y, int id) {
    node_t node;
    node.location_x = location_x;
    node.location_y = location_y;
    node.id = id;

    for (int i = 0; i < 30; i++) {
        package_t package = create_package(-1, -1, -1, 0, 0, 0, 0);
        node.packages[i] = package;
    }

    return node;
}