#include "package_distribution.h"

typedef struct {
    node_t *i;
    node_t *j;
    int savings;
} savings_t;

int calculate_savings(int i, int j, a_star_matrix_t a_star_matrix, int depot);

int compare_savings(const void* a, const void* b);

int can_merge(int i, int j, int* routes);

int respects_predecessor(int i, int j, int* routes, a_star_matrix_t a_star_matrix);