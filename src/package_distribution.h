// Include header file only ONCE
#pragma once

typedef enum {
    NOT_DELIVERED,
    BEING_DELIVERED,
    DELIVERED
} delivery_status_e;

typedef struct {
    double height; // meters
    double width; // meters
    double length; // meters
    double weight; // kilo
    int priority;
    int node_id;
    int truck_id;
    delivery_status_e delivery_status; // NOT_DELIVERED
} package_t;

typedef struct {
    int location_x;
    int location_y;
    int id;
    package_t packages[30];
} node_t;

typedef struct {
    double max_weight; // 3500 kilo
    double max_volume; // 19 cubic meters
    int average_speed; // 60 kilometers an hour
    int id;
} truck_t;

truck_t create_truck(int id);

package_t create_package(int priority, int node_id, int truck_id, double height, double width, double length, double weight);

node_t create_node(int location_x, int location_y, int id);