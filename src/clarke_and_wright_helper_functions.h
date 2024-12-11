#include "package_distribution.h"

typedef struct {
    node_t *i;
    node_t *j;
    int savings;
} savings_t;

int savings_compare(const void *a, const void *b);

savings_t *savings(node_t *distribution_node, graph_t *optimized_matrix);

void push_savings(savings_t savings, savings_t *list, int size);
