#include <stdio.h>
#include <stdlib.h>

#include "delivery_algorithm.h"

// Binary tree
void add_node_to_tree(node_t *new_node, node_t *tree_root) {
    if (tree_root == NULL) {
        tree_root = new_node;
        return;
    }
    if (new_node->f < tree_root->f) {
        if (tree_root->left == NULL) {
            tree_root->left = new_node;
        } else {
            add_node_to_tree(new_node, tree_root->left);
        }
        return;
    }
    if (new_node->f >= tree_root->f) {
        if (tree_root->right == NULL) {
            tree_root->right = new_node;
        } else {
            add_node_to_tree(new_node, tree_root->right);
        }
    }
}

// A* algorithm
void a_star(graph_t *graph, node_t start_node, node_t end_node) {
    tree_t unvisited_nodes = {&start_node};
    tree_t visited_nodes = {NULL};

    start_node.g = 0; // Cost from start to current node
    start_node.h = heuristic(start_node, end_node);       // estimated cost from current to the goal
    start_node.f = start_node.g + start_node.h; // Total estimated cost
    start_node.parent = NULL; // For path reconstruction

    while (unvisited_nodes.root != NULL) { // while list is not empty
        node_t current = find_lowest_f_in_tree(unvisited_nodes);

        if (current.location_x == end_node.location_x && current.location_y == end_node.location_y) {
            return reconstruct_path(current); // Takes in current node and finds parent until start node (reconstructs the path)
        }

        add_node_to_tree(current, visited_nodes); // Add current to visited node binary tree

        remove_node_from_tree(current, unvisited_nodes); // Remove current from unvisited node binary tree

        // TODO: Check this for all neighbours to the current node.
        if (check_in_tree(current_neighbour, visited_nodes)) { // Check if the current neighbour is in the visited nodes tree
            continue;
        }

        // We use the heuristic function instead of a distance function, since we don't follow real life roads.
        int tentative_g = current.g + heuristic(current, current_neigbour); // Calculate the tentative_g score

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
        // TODO ENDS HERE
    }

    printf("No path exists\n");
    exit(EXIT_FAILURE);
}