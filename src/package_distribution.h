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

typedef struct node_t {
    int location_x;
    int location_y;
    int id;
    package_t packages[30];
    //Variables for A* algorithm;
    struct node_t* parent;
    double g;
    double h;
    double f;
    struct node_t* left;
    struct node_t* right;
} node_t;

typedef struct {
    double max_weight; // 3500 kilo
    double max_volume; // 19 cubic meters
    int average_speed; // 60 kilometers an hour
    int id;
} truck_t;

typedef struct {
    int nodes;
    int **adj_matrix;
    node_t **node_addresses;
} graph_t;

typedef struct {
    graph_t *predecessor_matrix;
    graph_t *optimized_matrix;

} a_star_matrix_t;

typedef struct {
    node_t *root;
} tree_t;


truck_t *create_truck(int id);

package_t *create_package(int priority, int node_id, int truck_id, double height, double width, double length, double weight);

node_t *create_node(int location_x, int location_y, int id);

graph_t *create_graph(int nodes_amount);