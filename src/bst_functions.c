#include <stdio.h>
#include <stdlib.h>

#include "bst_functions.h"

// Binary tree
void add_node_to_tree(node_t *new_node, tree_t *tree) {
    if (new_node == NULL) {
        printf("Error: Attempting to add a NULL node to the tree\n");
        exit(EXIT_FAILURE);
    }

    if (tree->root == NULL) {
        tree->root = new_node;
        return;
    }

    add_node_to_tree_rec(new_node, tree->root);
}

void add_node_to_tree_rec(node_t *new_node, node_t *tree_node) {
    if (new_node == NULL) {
        printf("Error: Attempting to add a NULL node to the tree\n");
        exit(EXIT_FAILURE);
    }

    if (new_node->f < tree_node->f) {
        if (tree_node->left == NULL) {
            tree_node->left = new_node;
        } else {
            add_node_to_tree_rec(new_node, tree_node->left);
        }
    } else { // Runs if the f value is >= to the tree_node->f value.
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
    if (node == NULL) {
        printf("Error: Attempting to check if a NULL node is in the tree\n");
        exit(EXIT_FAILURE);
    }

    if (node == tree_root) {
        return 1;
    }

    if (tree_root == NULL) {
        return 0;
    }

    if (node->f < tree_root->f) {
        return check_in_tree(node, tree_root->left);
    }

    if (node->f >= tree_root->f) {
        return check_in_tree(node, tree_root->right);
    }

    return 0;
}

// Function that deletes a specific node in the node BST (Binary Search Tree)
// First argument is the node that gets deleted.
// The second argument is the tree that it gets deleted from. (might be unnecessary?)
void remove_node_from_tree(node_t *node, tree_t *tree) {
    if (node == NULL) {
        return; // Nothing to remove if node is null
    }
    // Case 1: Node has no children
    if (node->left == NULL && node->right == NULL) {
        node_t *parent = find_parent(node, tree);
        if (parent) {
            if (parent->left == node) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else {
            tree->root = NULL; // Node is root
        }
        free(node);
        return;

        // Case 2: Node has only one child
    } else if (node->left == NULL || node->right == NULL) {
        node_t *child = (node->left != NULL) ? node->left : node->right;
        node_t *parent = find_parent(node, tree);
        if (parent) {
            if (parent->left == node) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        } else {
            tree->root = child; // Root is child
        }
        free(node);
        return;
    }
    // Case 3: Node has two children
    // Replace the node with its in-order successor
    node_t *successor = find_successor(node);
    if (successor != NULL) {
        // Copy successor's values into the current node
        node->f = successor->f;
        node->location_x = successor->location_x;
        node->location_y = successor->location_y;

        // Recursively remove the successor node
        remove_node_from_tree(successor, tree);
    }
}


// Function that finds the "successor" of a specific node.
// The successor is the closest node that is still higher (based in what the list is sorted by).
// It is found by descending once to the right, then to the left until the end is reached.
node_t *find_successor(node_t *node) {
    if (node == NULL) {
        printf("Error: Attempting to find a NULL node in the tree\n");
        exit(EXIT_FAILURE);
    }

    if (node->right == NULL) return NULL;
    node = node->right;
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }

    if (node == NULL) {
        printf("Error: Attempting to return a NULL node from the tree\n");
        exit(EXIT_FAILURE);
    }

    return node;
}

// Function that finds the "parent" of a specific node.
// The parent is the node immediately before a node.
// It is found by going from the root, then using the nodes 'f' value to navigate the tree until the parent is reached.
node_t *find_parent(node_t *node, tree_t *tree) {
    if (node == NULL) {
        printf("Error: Attempting to find a NULL node in the tree\n");
        exit(EXIT_FAILURE);
    }

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