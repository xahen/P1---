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
    tree_t unvisited_nodes = {&start_node};
    tree_t visited_nodes = {NULL};

    start_node.g = 0; // Cost from start to current node
    start_node.h = heuristic(start_node, end_node);       // estimated cost from current to the goal
    start_node.f = start_node.g + start_node.h; // Total estimated cost
    start_node.parent = NULL; // For path reconstruction

    int iterations = 0;
    while (unvisited_nodes.root != NULL) { // while list is not empty
        iterations++;
        if (iterations > 1000) {
            printf("Max iterations reached!");
            exit(EXIT_FAILURE);
        }

        node_t *current = find_lowest_f_in_tree(unvisited_nodes.root);
        if (current == NULL) {
            printf("Current is NULL\n");
            exit(EXIT_FAILURE);
        }

        if (current->location_x == end_node.location_x && current->location_y == end_node.location_y) {
            // int *path = reconstruct_path(current, graph->nodes); // Takes in current node and finds parent until start node (reconstructs the path)

            // This for loop goes through all the nodes between start_node and end_node and updates the optimized_matrix with better g values.
            for (int i = start_node.id; i < graph->nodes; i++) {
                node_t *current_node = graph->node_addresses[i];

                if (start_node.id - 1 == current_node->id - 1) {
                    continue;
                }

                int current_matrix_value = graph->adj_matrix[start_node.id - 1][current_node->id - 1];
                if (current_node->g < current_matrix_value || current_matrix_value == 0) {
                    add_edge(a_star_matrix->optimized_matrix, start_node.id - 1, current_node->id - 1, ceil(current_node->g));
                }
            }
            // TODO: ADD EDGES TO THE A_STAR_MATRIX POINTER ARGUMENT
            // TODO add edges to a_star matrixes!!

            // Prepare for next A* run
            // TODO: Make this a function
            unvisited_nodes.root = NULL;
            visited_nodes.root = NULL;
            for (int i = 0; i < graph->nodes; i++) {
                /*if(check_in_tree(graph->node_addresses[i], unvisited_nodes.root)) {
                    remove_node_from_tree(graph->node_addresses[i], &unvisited_nodes);
                }

                if(check_in_tree(graph->node_addresses[i], visited_nodes.root)) {
                    remove_node_from_tree(graph->node_addresses[i], &visited_nodes);
                }*/

                graph->node_addresses[i]->f = DBL_MAX;
                graph->node_addresses[i]->g = DBL_MAX;
                graph->node_addresses[i]->h = DBL_MAX;
                graph->node_addresses[i]->parent = NULL;
                graph->node_addresses[i]->left = NULL;
                graph->node_addresses[i]->right = NULL;
            }

            return;
        }

        remove_node_from_tree(current, &unvisited_nodes); // Remove current from unvisited node binary tree

        add_node_to_tree(current, &visited_nodes); // Add current to visited node binary tree

        // TODO: Check this for all neighbours to the current node.
        for (int i = current->id; i < graph->nodes; ++i) {
            //TODO: Make this work by getting a node_t struct from the graph.
            // Get the right location somehow.
            // -
            // Also this should not create a new node. It should use existing nodes already entered in the matrix.
            // Maybe make a binary tree sorted by ID as well?
            // -
            // We could just get the g score of the node (Thats the value shown in the matrix), then calculate f and find the node.
            // But is the node even in a tree yet?
            // Maybe start off by actually making all of the nodes we put in the matrix.
            // Then instead of setting the 'g' or 'h' value in the matrix then put in a calculated 'f' score of the matrix??
            // -
            // We should probably find a different way to get the node of the place we are located in the matrix.
            // In this instance we are looking for the node on the top of the x-axis.
            // So if we go from A to B we are looking for B as that is in the x-axis.
            // As shown below:
            //   A B
            // A 0 1

            node_t *current_neighbour = graph->node_addresses[i];
            if (current_neighbour == NULL) {
                printf("Current neighbour is NULL\n");
                exit(EXIT_FAILURE);
            }

            // Check if the next index in the array node_addresses
            // TODO: REMEMBER TO CHECK THIS!
            if (graph->adj_matrix[current->id - 1][current_neighbour->id - 1] == 0) {
                continue;
            }


             // You should be able to find all neighbours with this.
            if (check_in_tree(current_neighbour, visited_nodes.root)) { // Check if the current neighbour is in the visited nodes tree
                continue;
            }

            // We use the heuristic function instead of a distance function, since we don't follow real life roads.
            double tentative_g = current->g + graph->adj_matrix[current->id - 1][current_neighbour->id - 1]; // Calculate the tentative_g score

            /*printf("current->g: %lf\n", current->g);
            printf("graph->adj_matrix[][]: %d\n", graph->adj_matrix[current->id - 1][current_neighbour->id - 1]);
            printf("tentative_g: %lf\n", tentative_g);*/

            if (!check_in_tree(current_neighbour, unvisited_nodes.root)) {
                add_node_to_tree(current_neighbour, &unvisited_nodes);
            } else if (tentative_g >= current_neighbour->g) {
                continue; // Path is not better
            }

            // Current neighbour has the best path so far
            current_neighbour->parent = current;
            current_neighbour->g = tentative_g;
            current_neighbour->h = heuristic(*current_neighbour, end_node);
            current_neighbour->f = current_neighbour->g + current_neighbour->h;
        }
    }

    printf("No path exists\n");
    exit(EXIT_FAILURE);
}