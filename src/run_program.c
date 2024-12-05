#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "delivery_algorithm.h"
#include "delivery_sim.h"
#include "resource_calculation.h"
#include "create_routes.h"

#include <math.h>

int main(void) {
    srand(time(NULL));
    graph_t *graph = generate_random_graph();

    a_star_matrix_t a_star_matrix = {
        create_graph(graph->nodes),
        create_graph(graph->nodes)
    };

    display_matrix(graph);

    for (int i = 0; i < graph->nodes - 1; i++) {
        a_star(graph, &a_star_matrix, *graph->node_addresses[i], *graph->node_addresses[graph->nodes - 1]);
    }


    display_matrix(a_star_matrix.optimized_matrix);

    display_predecessor_matrix(a_star_matrix.predecessor_matrix);
    free_matrix(graph);
    free(graph);
    return 0;
}
