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
    /*double calc_x = current_node.location_x - current_node_neighbour.location_x;
    double calc_y = current_node.location_y - current_node_neighbour.location_y;

    calc_x *= calc_x;
    calc_y *= calc_y;

    double sum = calc_x + calc_y;
    double distance = sqrt(sum);
    return distance;*/

    // Performance
    return sqrt(pow(current_node.location_x - current_node_neighbour.location_x, 2) + pow(current_node.location_y - current_node_neighbour.location_y, 2));
}

int f_comparison(const void *a, const void *b) {
    node_t *node_a = *(node_t**)a;
    node_t *node_b = *(node_t**)b;

    if (node_a == NULL && node_b == NULL) return 0; // Both are NULL (or NaN), they are equal
    if (node_a == NULL) return 1;             // NULL (or NaN) comes last
    if (node_b == NULL) return -1;            // NULL (or NaN) comes last

    // Sort by value (ascending order)
    if (node_a->f > node_b->f) return 1;
    if (node_a->f < node_b->f) return -1;

    return 0; // Equal values
}

// Pop a node from a list
void pop_node(node_t *node, node_t **list, int size) {
    int index = -1;
    for (int i = 0; i < size; i++) {
        if (list[i] == node) {
            index = i;
            break;
        }
    }

    if (index == -1) return;

    for (int i = index; i < size - 1; i++) {
        list[i] = list[i + 1];
    }

    size--;
    list[size] = NULL;
}

void push_node(node_t *node, node_t **list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i] == NULL) {
            list[i] = node;
            return;
        }
    }
    printf("List is full!\n");
    exit(EXIT_FAILURE);
}

int check_in_list(node_t *node, node_t **list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i] == node) return 1;
    }
    return 0;
}