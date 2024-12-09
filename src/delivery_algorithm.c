#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "delivery_algorithm.h"
#include "create_routes.h"
#include "bst_functions.h"
#include "astar_helper_functions.h"

// A* algorithm
// TODO: Give an a_star_matrix_t pointer as argument instead of returning a struct.
//  Makes the function able to edit the same predecessor matrix and optimized matrix for multiple runs.
//  This would also make the a_star function return void.
void a_star(graph_t *graph, a_star_matrix_t *a_star_matrix, node_t start_node, node_t end_node) {
    /*tree_t unvisited_nodes = {&start_node};
    tree_t visited_nodes = {NULL};*/
    node_t **unvisited_nodes = (node_t**)calloc(graph->nodes, graph->nodes * sizeof(node_t*));
    node_t **visited_nodes = (node_t**)calloc(graph->nodes, graph->nodes * sizeof(node_t*));

    // Insert start_node into the unvisited nodes list.
    unvisited_nodes[0] = &start_node;

    start_node.g = 0;
    start_node.h = heuristic(start_node, end_node); // Minimum estimated cost from current node to end node
    start_node.f = start_node.g + start_node.h; // Total estimated cost
    start_node.parent = NULL; // For path reconstruction

    while(unvisited_nodes[0] != NULL) {
        for (int i = 0; i < graph->nodes; i++) {
            qsort(unvisited_nodes, graph->nodes, sizeof(node_t*), f_comparison);
        }

        node_t *current = unvisited_nodes[0];
        if (current == NULL) {
            printf("Current is NULL\n");
            exit(EXIT_FAILURE);
        }

        if (current->location_x == end_node.location_x && current->location_y == end_node.location_y) {
            // Reconstruct path

            
        }

    }

    free(unvisited_nodes);
    free(visited_nodes);
}