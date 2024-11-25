#include <stdio.h>
#include <stdlib.h>

#include "delivery_algorithm.h"

void add_edge(graph_t *graph, int node_src, int node_dest) {
    if (node_src >= graph->nodes || node_dest >= graph->nodes) {
        printf("Invalid nodes:\n");
        exit(EXIT_FAILURE);
    }

    graph->adj_matrix[node_src][node_dest] = 1;
    graph->adj_matrix[node_dest][node_src] = 1;
}

void display_matrix(graph_t *graph) {
    printf("  ");
    for (int i = 0; i < graph->nodes; i++) {
        printf("\x1b[36m"); // Switch the text color in the terminal to blue
        printf("%c ", i + 65);
    }
    printf("\n");
    for (int i = 0; i < graph->nodes; i++) {
        printf("\x1b[36m"); // Switch the text color in the terminal to blue
        printf("%c ", i + 65);
        printf("\x1b[0m"); // Reset the text color back to white (for the numbers)
        for (int j = 0; j < graph->nodes; j++) {
            printf("%d ", graph->adj_matrix[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(graph_t *graph) {
    for(int i = 0; i < graph->nodes; i++) {
        free(graph->adj_matrix[i]);
    }
    free(graph->adj_matrix);
}