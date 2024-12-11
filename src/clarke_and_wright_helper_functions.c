#include <stdio.h>
#include <stdlib.h>

#include "clarke_and_wright_helper_functions.h"
#include "astar_helper_functions.h"

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
    savings_t savings_a = *(savings_t*)a;
    savings_t savings_b = *(savings_t*)b;

    // Sorting in descending order
    if(savings_a.savings > savings_b.savings) return -1;
    if(savings_a.savings < savings_b.savings) return 1;

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

int is_interior(node_t *node, node_t **list, int size) {
    int index = get_index(list, node, size);

    // Return false if node is not in route
    if (index == -1) return 0;

    // Return false if node is first or last in route
    if (index == 0 || index == size - 1 || list[index + 1] == NULL) return 0;

    return 1;
}

node_t **merge_routes(node_t **route_one, node_t **route_two, int size, savings_t savings_struct) {
    node_t **list = (node_t**)calloc(size, sizeof(node_t*));

    int index_i = get_index(route_one, savings_struct.i, size);
    int index_j = get_index(route_two, savings_struct.j, size);

    if (!(index_i == size - 1 || list[index_i + 1] == NULL)) {
        reverse_list(route_one, size);
    }

    if (index_j != 0) {
        reverse_list(route_two, size);
    }

    int size_of_route_one = size_of_list_without_null(route_one, size);
    int size_of_route_two = size_of_list_without_null(route_two, size);
    for (int i = 0; i < size_of_route_one; i++) {
        list[i] = route_one[i];
    }

    for (int i = 0; i < size_of_route_two; i++) {
        list[i + size_of_route_one - 1] = route_two[i];
    }

    return list;
}

int get_index(node_t **list, node_t *node, int size) {
    int index = -1;

    for (int i = 0; i < size; i++) {
        if (list[i] == node) index = i;
        if (list[i] == NULL) break;
    }

    return index;
}

void reverse_list(node_t **list, int size) {
    node_t **temp = (node_t**)calloc(size, sizeof(node_t*));
    int count = 0;
    for (int i = size-1; i >= 0; i--) {
        if (list[i] == NULL) continue;
        temp[count] = list[i];
        count++;
    }

    for (int i = 0; i < size; i++) {
        list[i] = temp[i];
    }
    free(temp);
}

int size_of_list_without_null(node_t **list, int size) {
    for (int i = 0; i < size; i++) {
        if (list[i] == NULL) return i;
    }
    return size;
}