#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "delivery_algorithm.h"
#include "delivery_sim.h"
#include "resource_calculation.h"
#include "create_routes.h"

int main(void) {
    int start_time = time(NULL);
    int seed = time(NULL);
    //int seed = 1733942317;
    srand(seed);

    graph_t *graph = generate_random_graph();
    graph_t optimized_matrix = *graph;

    a_star_matrix_t a_star_matrix = {
        create_graph(graph->nodes),
        &optimized_matrix
    };

    display_matrix(graph);

    for (int i = 0; i < graph->nodes - 1; i++) {
        for (int j = i + 1; j < graph->nodes; j++) {
            if(graph->adj_matrix[i][j] == 0) {
                a_star(graph, &a_star_matrix, graph->node_addresses[i], graph->node_addresses[j]);
            }
        }
    }

    display_matrix(a_star_matrix.optimized_matrix);
    display_predecessor_matrix(a_star_matrix.predecessor_matrix);

    int depot = 0;

    int *routes = (int*)calloc(graph->nodes, sizeof(int));

    int **route_order = (int**)calloc(graph->nodes, sizeof(int*));
    for (int i = 0; i < graph->nodes; i++) {
        route_order[i] = (int*)calloc(graph->nodes, sizeof(int));
    }

    clarke_wright_algorithm(a_star_matrix, depot, routes, route_order);

    printf("\n");
    printf("Time run: %lld\n", time(NULL) - start_time);
    printf("Seed: %d\n", seed);
    printf("Press enter to run simulation...\n");
    getchar();

    run_simulation(a_star_matrix, route_order[routes[1]], depot);

    free_matrix(graph);
    free(graph);
    return 0;
}