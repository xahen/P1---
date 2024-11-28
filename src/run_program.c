#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "delivery_algorithm.h"
#include "delivery_sim.h"
#include "resource_calculation.h"
#include "create_routes.h"

int main(void) {
    srand(time(NULL));
    graph_t *graph = generate_random_graph();
    graph_t *graph2 = create_graph(4);

    add_edge(graph2, 0, 1, 5);
    add_edge(graph2, 0, 2, 2);
    add_edge(graph2, 1, 2, 2);
    add_edge(graph2, 2, 3, 9);

    display_matrix(graph);
    display_matrix(graph2);
    display_predecessor_matrix(graph2);
    free_matrix(graph);
    free_matrix(graph2);
    return 0;
}
