#include <stdio.h>
#include <stdlib.h>

#include "bst_functions.h"

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

// Function that deletes a specific node in the node BST (Binary Search Tree)
// First argument is the node that gets deleted.
// The second argument is the tree that it gets deleted from. (might be unnecessary?)
void remove_node_from_tree(node_t *node, tree_t *tree) {
    // Use the find_successor() function to find the node that you will swap into the deleted node's space.
    node_t *successor = find_successor(node);
    // Using the find_parent() function to find the parent of the node we want to delete.
    node_t *parent = find_parent(node, tree);

    if (successor != NULL) {
        // Set successors left node pointer to the deleted node's left pointer.
        successor->left = node->left;

        if (parent != NULL) {
            if (parent->left == node) {
                parent->left = node->right;
            } else {
                parent->right = node->right;
            }
        } else {
            tree->root = node->right;
        }

        // Reset the deleted node's pointers
        node->left = NULL;
        node->parent = NULL;
        node->right = NULL;
    } else {
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