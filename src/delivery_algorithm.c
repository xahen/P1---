#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "delivery_algorithm.h"
#include "create_routes.h"
#include "astar_helper_functions.h"
#include "clarke_and_wright_helper_functions.h"

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
            // Add edges to optimized matrix and reconstruct path
            int matrix_value = graph->adj_matrix[start_node->id-1][end_node->id-1];
            if (end_node->g < matrix_value || matrix_value == 0) {
                add_edge(a_star_matrix->optimized_matrix, start_node->id - 1, end_node->id - 1, end_node->g);
                // Reconstruct path
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
            current_neighbours[i]->h = heuristic(*current_neighbours[i], *end_node);
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

void clarke_wright_algorithm(a_star_matrix_t a_star_matrix, int depot, int *routes, int **route_order) {
    int num_nodes = a_star_matrix.optimized_matrix->nodes;
    //int routes[num_nodes]; // Stores the route, that each node belongs to
    //int route_order[num_nodes][num_nodes]; // Stores the sequence of nodes in each route
    savings_t savings_list[(num_nodes - 1) * (num_nodes - 2) / 2]; // Max number of saving pairs

    // Initialize routes (all nodes are initially in their own route)
    for (int i = 0; i < num_nodes; i++) {
        routes[i] = i;
        // Initialize the route sequence with only the node itself and mark the end with -1
        for (int j = 0; j < num_nodes; j++) {
            route_order[i][j] = -1;
        }
        route_order[i][0] = i; // The route starts with the node itself.
    }

    // Generate all possible savings
    int savings_count = 0;
    for (int i = 1; i < num_nodes; i++) {
        for (int j = i + 1; j < num_nodes; j++) {
            savings_list[savings_count].i = a_star_matrix.optimized_matrix->node_addresses[i];
            savings_list[savings_count].j = a_star_matrix.optimized_matrix->node_addresses[j];
            savings_list[savings_count].savings = calculate_savings(i, j, a_star_matrix, depot);
            savings_count++;
        }
    }

    // Sort the savings list in descending order
    qsort(savings_list, savings_count, sizeof(savings_t), compare_savings);

    // Perform the savings algorithm
    for (int s = 0; s < savings_count; s++) {
        int i = savings_list[s].i->id - 1;
        int j = savings_list[s].j->id - 1;

        if (can_merge(i, j, routes) && respects_predecessor(i, j, routes, a_star_matrix)) {
            int route_i = routes[i];
            int route_j = routes[j];

            // Merge the routes (appending j's route to i's route)
            int index_i = 0;
            while (route_order[route_i][index_i] != -1) {
                index_i++;
            }

            int index_j = 0;
            while (route_order[route_j][index_j] != -1) {
                index_j++;
            }

            // Append all nodes from route j into route i
            for (int k = 0; k < index_j; k++) {
                route_order[route_i][index_i + k] = route_order[route_j][k];
            }
            route_order[route_i][index_i + index_j] = -1; // Mark the end of the merged route

            // Update the route information for all nodes in route j
            for (int k = 0; k < num_nodes; k++) {
                if (routes[k] == route_j) {
                    routes[k] = route_i; // All nodes from route j now belong to route i
                }
            }

            // Output the current merge
            printf("Merged route: %c <-> %c\n", 'A' + i, 'A' +j);
        }
    }

    // Print the final merged route in the current order
    printf("\nFinal route:\n");

    // Start from the depot
    printf("%c -> ", depot + 'A');
    int first_route = routes[1]; // Pick a route to print
    for (int i = 0; route_order[first_route][i] != -1; i++) {
        printf("%c -> ", 'A' + route_order[first_route][i]);
    }
    printf("%c\n", depot + 'A');
}