#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "delivery_algorithm.h"
#include "create_routes.h"

// Binary tree
void add_node_to_tree(node_t *new_node, tree_t *tree) {
    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }
    add_node_to_tree_rec(new_node, tree->root);
}

void add_node_to_tree_rec(node_t *new_node, node_t *tree_node) {
    if (new_node->f < tree_node->f) {
        if (tree_node->left == NULL) {
            tree_node->left = new_node;
        } else {
            add_node_to_tree_rec(new_node, tree_node->left);
        }
    }
    if (new_node->f >= tree_node->f) {
        if (tree_node->right == NULL) {
            tree_node->right = new_node;
        } else {
            add_node_to_tree_rec(new_node, tree_node->right);
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

int check_in_tree(node_t *node, node_t *tree_root) {
    if (tree_root == NULL) {
        return 0;
    }

    if (node->f < tree_root->f) {
        if (tree_root->left == NULL) {
            return 0;
        }
        return check_in_tree(node, tree_root->left);
    }

    if (node->f >= tree_root->f && node != tree_root) {
        if (tree_root->right == NULL) {
            return 0;
        }
        return check_in_tree(node, tree_root->right);
    }

    if (node == tree_root) {
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
    double calc_x = pow(current_node.location_x - current_node_neighbour.location_x, 2);
    double calc_y = pow(current_node.location_y - current_node_neighbour.location_y, 2);

    double sum = calc_x + calc_y;
    double distance = sqrt(sum);
    return distance;
}

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

    while (unvisited_nodes.root != NULL) { // while list is not empty
        node_t *current = find_lowest_f_in_tree(unvisited_nodes.root);

        if (current->location_x == end_node.location_x && current->location_y == end_node.location_y) {
            // int *path = reconstruct_path(current, graph->nodes); // Takes in current node and finds parent until start node (reconstructs the path)

            // This for loop goes through all the nodes between start_node and end_node and updates the optimized_matrix with better g values.
            for (int i = 0; i < graph->nodes; i++) {
                node_t *current_node = graph->node_addresses[i];

                if (start_node.id - 1 == current_node->id - 1) {
                    continue;
                }

                int current_matrix_value = graph->adj_matrix[start_node.id - 1][current_node->id - 1];
                if (ceil(current_node->g) < current_matrix_value || current_matrix_value == 0) {
                    printf("Adding edge\n"); // TODO: REMOVE
                    add_edge(a_star_matrix->optimized_matrix, start_node.id - 1, current_node->id - 1, current_node->g);
                    display_matrix(a_star_matrix->optimized_matrix); // TODO: REMOVE
                }
            }
            // TODO: ADD EDGES TO THE A_STAR_MATRIX POINTER ARGUMENT
            // TODO add edges to a_star matrixes!!
            return;
        }

        remove_node_from_tree(current, &unvisited_nodes); // Remove current from unvisited node binary tree

        add_node_to_tree(current, &visited_nodes); // Add current to visited node binary tree

        // TODO: Check this for all neighbours to the current node.
        for (int i = 1; i < graph->nodes; ++i) {
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

            // Check if the next index in the array node_addresses
            if (graph->adj_matrix[current->id - 1][current_neighbour->id - 1] == 0) {
                continue;
            }

             // You should be able to find all neighbours with this.
            if (check_in_tree(current_neighbour, visited_nodes.root)) { // Check if the current neighbour is in the visited nodes tree
                continue;
            }

            // We use the heuristic function instead of a distance function, since we don't follow real life roads.
            double tentative_g = current->g + heuristic(*current, *current_neighbour); // Calculate the tentative_g score

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

// Function that deletes a specific node in the node BST (Binary Search Tree)
// First argument is the node that gets deleted.
// The second argument is the tree that it gets deleted from. (might be unnecessary?)
void remove_node_from_tree(node_t *node, tree_t *tree) {

    // Use the find_successor() function to find the node that you will swap into the deleted node's space.
    node_t *successor = find_successor(node);

    if (successor != NULL) {
        // Set the "deleted" node's values to the successors values.
        node->location_x = successor->location_x;
        node->location_y = successor->location_y;
        node->id = successor->id;
        node->g = successor->g;
        node->h = successor->h;
        node->f = successor->f;

        // Set the parent node of the successor to point to NULL, instead of pointing to the successor.
        if (node->right == successor) {
            if (node->right->right != NULL) {
                node->right = node->right->right;
            } else {
                node->right = NULL;
            }
        } else {
            node = node->right;
            while (node->left != successor) {
                node = node->left;
            }
            node->left = NULL;
        }
    } else {
        // Use the find_parent() function to find the parent to the deleted node, you can now set a null pointer for it.
        node_t *parent = find_parent(node, tree);

        // find_parent() returns NULL if the root node was given as its argument.
        if (parent != NULL) {
            // Set the pointer for the deleted node to NULL.
            if (parent->right == node) parent->right = NULL;
            else parent->left = NULL;
        } else {
            tree->root = NULL;
        }
    }
}

// Function that finds the "successor" of a specific node.
// The successor is the closest node that is still higher (based in what the list is sorted by).
// It is found by descending once to the right, then to the left until the end is reached.
node_t *find_successor(node_t *node) {
    if (node->right == NULL) return NULL;
    node = node->right;
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }

    return node;
}

// Function that finds the "parent" of a specific node.
// The parent is the node immediately before a node.
// It is found by going from the root, then using the nodes 'f' value to navigate the tree until the parent is reached.
node_t *find_parent(node_t *node, tree_t *tree) {
    node_t *parent = tree->root;

    if (node == parent) return NULL;

    while (parent->right != node && parent->left != node) {
        if (node->f < parent->f) {
            parent = parent->left;
        } else {
            parent = parent->right;
        }
    }

    return parent;
}