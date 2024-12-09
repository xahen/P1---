#include "package_distribution.h"

int *reconstruct_path(node_t *node, int nodes_amount);
void reconstruct_path_rec(node_t *node_parent, int *list, int *count);

double heuristic(node_t current_node, node_t current_node_neighbour);