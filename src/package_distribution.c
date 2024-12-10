#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "package_distribution.h"

truck_t *create_truck(int id) {
    truck_t *truck = (truck_t*)malloc(sizeof(truck_t));
    truck->id = id;

    truck->average_speed = 60;
    truck->max_volume = 19;
    truck->max_weight = 3500;

    return truck;
}

package_t *create_package(int priority, int node_id, int truck_id, double height, double width, double length, double weight) {
    package_t *package = (package_t*)malloc(sizeof(package_t));
    package->priority = priority;
    package->node_id = node_id;
    package->truck_id = truck_id;
    package->height = height;
    package->width = width;
    package->length = length;
    package->weight = weight;

    package->delivery_status = NOT_DELIVERED;

    return package;
}

node_t *create_node(int location_x, int location_y, int id) {
    node_t *node = (node_t*)malloc(sizeof(node_t));
    node->location_x = location_x;
    node->location_y = location_y;
    node->id = id;

    for (int i = 0; i < 30; i++) {
        node->packages[i] = *create_package(-1, -1, -1, 0, 0, 0, 0);
    }

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->f = DBL_MAX;
    node->g = DBL_MAX;
    node->h = 0;

    return node;
}

graph_t *create_graph(int nodes_amount) {
    graph_t *graph = (graph_t*)malloc(sizeof(graph_t));


    graph->nodes = nodes_amount;
    graph->adj_matrix = (int**)malloc(nodes_amount * sizeof(int*));

    for (int i = 0; i < nodes_amount; i++) {
        graph->adj_matrix[i] = (int*)calloc(nodes_amount, sizeof(int));
    }

    graph->node_addresses = (node_t**)malloc(nodes_amount * sizeof(node_t));

    return graph;
}