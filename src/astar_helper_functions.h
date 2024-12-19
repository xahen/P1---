#include "package_distribution.h"

double heuristic(node_t current_node, node_t current_node_neighbour);

int f_comparison(const void *a, const void *b);

void pop_node(node_t *node, node_t **list, int size);
void push_node(node_t *node, node_t **list, int size);
int check_in_list(node_t *node, node_t **list, int size);