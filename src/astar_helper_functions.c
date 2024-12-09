#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "astar_helper_functions.h"

int *reconstruct_path(node_t *node, int nodes_amount) {
    int count = 0;
    int *list = (int*)calloc(nodes_amount, sizeof(int));
    list[count] = node->id;
    reconstruct_path_rec(node->parent, list, &count);
    return list;
}

void reconstruct_path_rec(node_t *node_parent, int *list, int *count) {
    if (node_parent == NULL) {
        return;
    }

    *count++;
    list[*count] = node_parent->id;
    reconstruct_path_rec(node_parent->parent, list, count);
}

// Heuristic function - calculates distance as a direct line between two nodes
double heuristic(node_t current_node, node_t current_node_neighbour) {
    double calc_x = pow(current_node.location_x - current_node_neighbour.location_x, 2);
    double calc_y = pow(current_node.location_y - current_node_neighbour.location_y, 2);

    double sum = calc_x + calc_y;
    double distance = sqrt(sum);
    return distance;
}