#include <stdio.h>
#include <stdlib.h>

#include "clarke_and_wright_helper_functions.h"
#include "delivery_algorithm.h"


int calculate_savings(int i, int j, a_star_matrix_t a_star_matrix, int depot) {
    return a_star_matrix.optimized_matrix->adj_matrix[depot][i] + a_star_matrix.optimized_matrix->adj_matrix[depot][j] - a_star_matrix.optimized_matrix->adj_matrix[i][j];
}

int compare_savings(const void* a, const void* b) {
    return ((savings_t*)b)->savings - ((savings_t*)a)->savings;
}

int can_merge(int i, int j, int* routes) {
    return routes[i] != routes[j];
}

int respects_predecessor(int i, int j, int* routes, a_star_matrix_t a_star_matrix) {
    if (a_star_matrix.predecessor_matrix->adj_matrix[i][j] != 0 && a_star_matrix.predecessor_matrix->adj_matrix[i][j] != j && routes[i] != routes[j]) {
        //printf("Predecessor constraints!\n");
        return 0;
    }
    return 1;
}