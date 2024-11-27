#include "package_distribution.h"

void add_node_to_tree(node_t *new_node, node_t *tree_root);
node_t *find_lowest_f_in_tree(node_t *new_node, node_t *tree_root);
int check_in_tree(node_t *node, tree_t *tree);
int *reconstruct_path(node_t *node, int nodes_amount);
void reconstruct_path_rec(node_t *node_parent, int *list, int *count);

double heuristic(node_t current_node, node_t current_node_neighbour);

a_star_matrix_t *a_star(graph_t *graph, node_t start_node, node_t end_node);