#include <stdio.h>
#include <stdlib.h>

#include "create_routes.h"

void add_edge(graph_t *graph, int node_src, int node_dest, int weight) {
    if (node_src >= graph->nodes || node_dest >= graph->nodes) {
        printf("Invalid nodes:\n");
        exit(EXIT_FAILURE);
    }

    graph->adj_matrix[node_src][node_dest] = weight;
    graph->adj_matrix[node_dest][node_src] = weight;
}

void display_matrix(graph_t *graph) {
    printf("   ");
    for (int i = 0; i < graph->nodes; i++) {
        printf("\x1b[36m"); // Switch the text color in the terminal to blue
        printf("%c   ", i + 65);
    }
    printf("\n\n");
    for (int i = 0; i < graph->nodes; i++) {
        printf("\x1b[36m"); // Switch the text color in the terminal to blue
        printf("%c  ", i + 65);
        printf("\x1b[0m"); // Reset the text color back to white (for the numbers)
        for (int j = 0; j < graph->nodes; j++) {
            if (graph->adj_matrix[i][j] < 99 && graph->adj_matrix[i][j] > 9) {
                printf("%d  ", graph->adj_matrix[i][j]);
            } else if (graph->adj_matrix[i][j] > 99) {
                printf("%d ", graph->adj_matrix[i][j]);
            } else {
                printf("%d   ", graph->adj_matrix[i][j]);
            }
        }
        printf("\n\n");
    }
    printf("\n");
}

void display_predecessor_matrix(graph_t *graph) {
    printf("   ");
    for (int i = 0; i < graph->nodes; i++) {
        printf("\x1b[36m"); // Switch the text color in the terminal to blue
        printf("%c   ", i + 65);
    }
    printf("\n\n");
    for (int i = 0; i < graph->nodes; i++) {
        printf("\x1b[36m"); // Switch the text color in the terminal to blue
        printf("%c  ", i + 65);
        printf("\x1b[0m"); // Reset the text color back to white (for the numbers)
        for (int j = 0; j < graph->nodes; j++) {
            if (graph->adj_matrix[i][j] != 0) {
                printf("%c   ", graph->adj_matrix[i][j] + 64);
            } else {
                printf("-   ");
            }
        }
        printf("\n\n");
    }
    printf("\n");
}

void free_matrix(graph_t *graph) {
    for(int i = 0; i < graph->nodes; i++) {
        free(graph->adj_matrix[i]);
        free(graph->node_addresses[i]);
    }
    free(graph->adj_matrix);
    graph->adj_matrix = NULL;
}

void free_a_star_matrix(a_star_matrix_t *matrix) {
    free_matrix(matrix->predecessor_matrix);
    free_matrix(matrix->optimized_matrix);
}