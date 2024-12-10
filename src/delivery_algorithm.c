#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "delivery_algorithm.h"
#include "create_routes.h"
#include "bst_functions.h"
#include "astar_helper_functions.h"

// A* algorithm
void a_star(graph_t *graph, a_star_matrix_t *a_star_matrix, node_t *start_node, node_t *end_node) {
    node_t **unvisited_nodes = (node_t**)calloc(graph->nodes, sizeof(node_t*));
    node_t **visited_nodes = (node_t**)calloc(graph->nodes, sizeof(node_t*));

    // Insert start_node into the unvisited nodes list.
    unvisited_nodes[0] = start_node;

    start_node->g = 0;
    start_node->h = heuristic(*start_node, *end_node); // Minimum estimated cost from current node to end node
    start_node->f = start_node->g + start_node->h; // Total estimated cost
    start_node->parent = NULL; // For path reconstruction

    while(unvisited_nodes[0] != NULL) {
        node_t *current = unvisited_nodes[0];
        if (current == NULL) {
            printf("Current is NULL\n");
            exit(EXIT_FAILURE);
        }

        if (current == end_node) {
            // Reconstruct path

            // Add edges to optimized matrix
            int matrix_value = graph->adj_matrix[start_node->id-1][end_node->id-1];
            if (end_node->g < matrix_value || matrix_value == 0) {
                add_edge(a_star_matrix->optimized_matrix, start_node->id - 1, end_node->id - 1, end_node->g);
                add_edge(a_star_matrix->predecessor_matrix, start_node->id - 1, end_node->id - 1, end_node->parent->id);
            }

            free(unvisited_nodes);
            free(visited_nodes);
            return;
        }

        pop_node(current, unvisited_nodes, graph->nodes);

        push_node(current, visited_nodes, graph->nodes);

        node_t **current_neighbours = (node_t**)calloc(graph->nodes, sizeof(node_t*));

        for (int i = 0; i < graph->nodes; i++) {
            if (graph->adj_matrix[current->id - 1][i] != 0) {
                push_node(graph->node_addresses[i], current_neighbours, graph->nodes);
            }
        }

        for (int i = 0; i < graph->nodes; i++) {
            // If we reach the end of the list of neighbours, then break (See push_node function)
            if (current_neighbours[i] == NULL) break;

            // Check if the current node is in the visited_nodes list
            if (check_in_list(current_neighbours[i], visited_nodes, graph->nodes)) continue;

            // Set tentative_g score
            double tentative_g = ceil(current->g + heuristic(*current, *current_neighbours[i]));

            // Check if the current neighbour is in the unvisited nodes. Add to unvisited nodes if false
            if (check_in_list(current_neighbours[i], unvisited_nodes, graph->nodes) == 0) {
                push_node(current_neighbours[i], unvisited_nodes, graph->nodes);
            } else if (tentative_g >= current_neighbours[i]->g) {
                continue; // Path is not better
            }

            current_neighbours[i]->parent = current;
            current_neighbours[i]->g = tentative_g;
            //current_neighbours[i]->h = heuristic(*current_neighbours[i], *end_node);
            current_neighbours[i]->h = 0;
            current_neighbours[i]->f = current_neighbours[i]->g + current_neighbours[i]->h;
        }
        free(current_neighbours);

        qsort(unvisited_nodes, graph->nodes, sizeof(node_t *), f_comparison);
    }

    free(unvisited_nodes);
    free(visited_nodes);

    printf("No route exists!\n");
    exit(EXIT_FAILURE);
}