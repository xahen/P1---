#include <stdio.h>
#include <stdlib.h>

#include "delivery_algorithm.h"

// A* algorithm
void a_star(graph_t *graph, node_t start_node, node_t end_node) {
    node_t *unvisited_nodes = (node_t*)malloc(sizeof(node_t) * graph->nodes); // Change to linked list as a binary tree
    node_t *visited_nodes = (node_t*)malloc(sizeof(node_t) * graph->nodes);   // Change to linked list as a binary tree

    unvisited_nodes[0] = start_node;


    start_node.g = 0; // Cost from start to current node
    start_node.h = heuristic(start_node, end_node);       // estimated cost from current to the goal
    start_node.f = g + h; // Total estimated cost
    start_node.parent = NULL; // For path reconstruction
    // CHANGE NODE STRUCT

    while (unvisited_nodes > 0) { // while list is not empty
        node_t current = find_lowest_f_in_tree(unvisited_nodes);

        if (current.location_x == end_node.location_x && current.location_y == end_node.location_y) {
            free(unvisited_nodes);
            free(visited_nodes);
            return reconstruct_path(current); // Takes in current node and finds parent until start node (reconstructs the path)
        }

        add_node_to_tree(current, visited_nodes); // Add current to visited node binary tree

        remove_node_from_tree(current, unvisited_nodes); // Remove current from unvisited node binary tree

        // Check for all neighbours to current
        if (check_in_tree(current_neighbour, visited_nodes)) { // Check if the current neighbour is in the visited nodes tree
            continue;
        }

        int tentative_g = g + distance(current, current_neigbour); // Calculate the tentative_g score

        if (!check_in_tree(current_neighbour, unvisited_nodes)) {
            add_node_to_tree(current_neighbour, unvisited_nodes);
        } else if (tentative_g >= current_neighbour.g) {
            continue; // Path is not better
        }

        // Current neighbour has the best path so far
        current_neighbour.parent = current;
        current_neighbour.g = tentative_g;
        current_neighbour.h = heuristic(current_neighbour, end_node);
        current_neighbour.f = current_neighbour.g + current_neighbour.h;
    }

    printf("No path exists\n");
    exit(EXIT_FAILURE);
}