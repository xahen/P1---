#include "package_distribution.h"

void add_node_to_tree(node_t *new_node, tree_t *tree);
void add_node_to_tree_rec(node_t *new_node, node_t *tree_node);
node_t *find_lowest_f_in_tree(node_t *tree_root);
int check_in_tree(node_t *node, node_t *tree_root);
void remove_node_from_tree(node_t *node, tree_t *root);
node_t *find_successor(node_t *node);
node_t *find_parent(node_t *node, tree_t *tree);