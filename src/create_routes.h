#include "package_distribution.h"

void add_edge(graph_t *graph, int node_src, int node_dest, int weight);

void display_matrix(graph_t *graph);

void display_predecessor_matrix(graph_t *graph);

void free_matrix(graph_t *graph);

void free_a_star_matrix(a_star_matrix_t *matrix);