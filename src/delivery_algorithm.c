#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    }
    if (new_node->f >= tree_root->f) {
        if (tree_root->right == NULL) {
            tree_root->right = new_node;
        } else {
            add_node_to_tree(new_node, tree_root->right);
        }
    }
}

node_t *find_lowest_f_in_tree(node_t *tree_root) {
    if (tree_root == NULL) {
        printf("The tree is empty\n");
        exit(EXIT_FAILURE);
    }

    if (tree_root->left == NULL) {
        return tree_root;
    }

    return find_lowest_f_in_tree(tree_root->left);
}

int check_in_tree(node_t node, node_t *tree_root) {
    if (tree_root == NULL) {
        return 0;
    }

    if (node.f < tree_root->f) {
        if (tree_root->left == NULL) {
            return 0;
        }
        return check_in_tree(node, tree_root->left);
    }

    if (node.f >= tree_root->f && &node != tree_root) {
        if (tree_root->right == NULL) {
            return 0;
        }
        return check_in_tree(node, tree_root->right);
    }

    if (&node == tree_root) {
        return 1;
    }
    return 0;
}

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
    double calc_current_node = pow(current_node.location_x - current_node.location_y, 2);
    double calc_current_neighbour_node = pow(current_node_neighbour.location_x - current_node_neighbour.location_y, 2);

    double sum = calc_current_node + calc_current_neighbour_node;
    double distance = sqrt(sum);
    return distance;
}

// A* algorithm
a_star_matrix_t *a_star(graph_t *graph, node_t start_node, node_t end_node) {
    tree_t unvisited_nodes = {&start_node};
    tree_t visited_nodes = {NULL};
    a_star_matrix_t a_star_result = {
        create_graph(graph->nodes),
        create_graph(graph->nodes)
    };

    start_node.g = 0; // Cost from start to current node
    start_node.h = heuristic(start_node, end_node);       // estimated cost from current to the goal
    start_node.f = start_node.g + start_node.h; // Total estimated cost
    start_node.parent = NULL; // For path reconstruction

    while (unvisited_nodes.root != NULL) { // while list is not empty
        node_t *current = find_lowest_f_in_tree(unvisited_nodes.root);

        if (current->location_x == end_node.location_x && current->location_y == end_node.location_y) {
            int *path = reconstruct_path(current, graph->nodes); // Takes in current node and finds parent until start node (reconstructs the path)
            // TODO add edges to a_star matrixes!!
            return &a_star_result;
        }

        remove_node_from_tree(current, &unvisited_nodes); // Remove current from unvisited node binary tree

        add_node_to_tree(current, visited_nodes.root); // Add current to visited node binary tree

        // TODO: Check this for all neighbours to the current node.
        if (check_in_tree(*current_neighbour, visited_nodes.root)) { // Check if the current neighbour is in the visited nodes tree
            continue;
        }

        // We use the heuristic function instead of a distance function, since we don't follow real life roads.
        double tentative_g = current->g + heuristic(current, current_neigbour); // Calculate the tentative_g score

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