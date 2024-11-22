#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mtest.h"
#include "delivery_algorithm.h"
#include "delivery_sim.h"
#include "resource_calculation.h"

TEST_CASE(test_generate_random_package, {
    srand(time(NULL));

    for (int i = 0; i < 5; i++) {
        package_t package = generate_random_package();

        CHECK_TRUE(package.height > 0 && package.height <= 2);
        CHECK_TRUE(package.width > 0 && package.width <= 2);
        CHECK_TRUE(package.length > 0 && package.length <= 2);
        CHECK_TRUE(package.weight >= 1 && package.weight <= 25);
        CHECK_TRUE(package.priority >= 1 && package.priority <= 5);
        CHECK_TRUE(package.node_id >= 1 && package.node_id <= 5);
        CHECK_TRUE(package.truck_id >= 1 && package.truck_id <= 5);
    }
})

TEST_CASE(test_create_truck, {
    int truck_id = 1;

    truck_t truck = create_truck(truck_id);

    CHECK_TRUE(truck.id == truck_id);
    CHECK_TRUE(truck.max_weight == 3500);
    CHECK_TRUE(truck.average_speed == 60);
    CHECK_TRUE(truck.max_volume == 19);
})

TEST_CASE(test_create_package, {
    int package_priority = 1;
    int node_id = 2;
    int truck_id = 3;
    int height = 4;
    int width = 5;
    int length = 6;
    int weight = 7;

    package_t package = create_package(package_priority, node_id, truck_id, height, width, length, weight);

    CHECK_TRUE(package.priority == package_priority);
    CHECK_TRUE(package.node_id == node_id);
    CHECK_TRUE(package.truck_id == truck_id);
    CHECK_TRUE(package.height == height);
    CHECK_TRUE(package.width == width);
    CHECK_TRUE(package.length == length);
    CHECK_TRUE(package.weight == weight);
})

TEST_CASE(test_create_node, {
    int area = 1;
    int location_x = 2;
    int location_y = 3;
    int id = 4;

    node_t node = create_node(area, location_x, location_y, id);

    CHECK_TRUE(node.area == area);
    CHECK_TRUE(node.location_x == location_x);
    CHECK_TRUE(node.location_y == location_y);
    CHECK_TRUE(node.id == id);

    // Check if packages are initialized
    for (int i = 0; i < 30; i++) {
        CHECK_TRUE(node.packages[i].priority == -1);
        CHECK_TRUE(node.packages[i].node_id == -1);
        CHECK_TRUE(node.packages[i].truck_id == -1);
        CHECK_TRUE(node.packages[i].height == 0);
        CHECK_TRUE(node.packages[i].width == 0);
        CHECK_TRUE(node.packages[i].length == 0);
        CHECK_TRUE(node.packages[i].weight == 0);
    }
})

TEST_CASE(test_get_delivery_status, {
    CHECK_EQ_STRING(get_delivery_status(0), "NOT_DELIVERED");
    CHECK_EQ_STRING(get_delivery_status(1), "BEING_DELIVERED");
    CHECK_EQ_STRING(get_delivery_status(2), "DELIVERED");
    CHECK_FALSE(get_delivery_status(3) == "(null)");
})

TEST_CASE(test_calculate_trucks, {
    package_t package = create_package(1, 1, 1, 2, 2, 2, 5);
    double volume_filled = 0;
    int trucks_needed = 0;

    for (int i = 0; i < 20; i++) {
        trucks_needed += calculate_trucks(package, &volume_filled, 0);
    }

    CHECK_EQ_INT(trucks_needed, 3);
})

// TODO:
// TEST PRINT FUNCTIONS
// THIS IS NOT POSSIBLE WITHOUT RETURNING A STRING (THESE FUNCTIONS DO NOT NEED TESTS ANYWAY)

MAIN_RUN_TESTS(test_generate_random_package, test_create_truck, test_create_package, test_create_node, test_get_delivery_status, test_calculate_trucks);