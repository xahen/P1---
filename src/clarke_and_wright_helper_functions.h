#include "package_distribution.h"

typedef struct {
    node_t *i;
    node_t *j;
    int savings;
} savings_t;

int savings_compare(const void *a, const void *b);
savings_t *savings(node_t *distribution_node, graph_t *optimized_matrix);
void push_savings(savings_t savings, savings_t *list, int size);

int is_interior(node_t *node, node_t **list, int size);
node_t **merge_routes(node_t **route_one, node_t **route_two, int size, savings_t savings_struct);
int get_index(node_t **list, node_t *node, int size);
void reverse_list(node_t **list, int size);
int size_of_list_without_null(node_t **list, int size);