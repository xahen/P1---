#include <stdio.h>
#include <stdlib.h>

#include "clarke_and_wright_helper_functions.h"

savings_t *savings(node_t *distribution_node, graph_t *optimized_matrix) {
    int amount_of_pairs = (optimized_matrix->nodes - 1) * (optimized_matrix->nodes - 2) / 2;
    savings_t *savings_list = (savings_t*)calloc(amount_of_pairs, sizeof(savings_t));

    savings_t savings_value = {NULL, NULL, 0};
    for (int i = 0; i < optimized_matrix->nodes; i++) {
        for (int j = i; j < optimized_matrix->nodes; j++) {
            if (i != j && i != distribution_node->id-1 && j != distribution_node->id-1) {
                savings_value.i = optimized_matrix->node_addresses[i];
                savings_value.j = optimized_matrix->node_addresses[j];

                int Coi = optimized_matrix->adj_matrix[distribution_node->id-1][i];
                int Coj = optimized_matrix->adj_matrix[distribution_node->id - 1][j];
                int Cij = optimized_matrix->adj_matrix[i][j];
                savings_value.savings = Coi + Coj - Cij;

                push_savings(savings_value, savings_list, amount_of_pairs);
            }
        }
    }
    qsort(savings_list, amount_of_pairs, sizeof(savings_t), savings_compare);

    return savings_list;
}

int savings_compare(const void *a, const void *b) {
    savings_t *savings_a = *(savings_t**)a;
    savings_t *savings_b = *(savings_t**)b;

    // Sorting in descending order
    if(savings_a->savings > savings_b->savings) return -1;
    if(savings_a->savings < savings_b->savings) return 1;

    // Equal
    return 0;
}

void push_savings(savings_t savings, savings_t *list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i].i == NULL && list[i].j == NULL && list[i].savings == 0) {
            list[i] = savings;
            return;
        }
    }
    printf("List is full!\n");
    exit(EXIT_FAILURE);
}