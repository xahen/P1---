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
    //graph_t *graph2 = create_graph(5);

    //graph2->node_addresses[0] = create_node(0, 0, 1);
    //graph2->node_addresses[1] = create_node(10, 20, 2);
    //graph2->node_addresses[2] = create_node(15, 15, 3);
    //graph2->node_addresses[3] = create_node(5, 10, 4);
    //graph2->node_addresses[4] = create_node(13, 12, 5);

    //for (int i = 0; i < 4; i++) {
    //    add_edge(
    //    graph2,
    //    graph2->node_addresses[i]->id - 1,
    //    graph2->node_addresses[i+1]->id - 1,
    //    ceil(heuristic(*graph2->node_addresses[i], *graph2->node_addresses[i+1])));
    //}
    //add_edge(graph2, graph2->node_addresses[0]->id-1, graph2->node_addresses[2]->id-1, 22);

    //display_matrix(graph);
    a_star_matrix_t a_star_matrix = {
        create_graph(graph->nodes),
        create_graph(graph->nodes)
    };

    display_matrix(graph);

    for (int i = 0; i < graph->nodes - 1; i++) {
        a_star(graph, &a_star_matrix, *graph->node_addresses[i], *graph->node_addresses[graph->nodes - 1]);
    }

    // There is a problem when running a_star multiple times. We might need to add the optimized route edges to the graph2->adj_matrix
    // This problem persists and is probably caused by the id of the node_addresses
    // Under current_neighbour
    //a_star(graph2, &a_star_matrix, *graph2->node_addresses[0], *graph2->node_addresses[graph2->nodes - 1]);
    //a_star(graph2, &a_star_matrix, *graph2->node_addresses[1], *graph2->node_addresses[graph2->nodes - 1]);
    //a_star(graph2, &a_star_matrix, *graph2->node_addresses[2], *graph2->node_addresses[graph2->nodes - 1]);


    display_matrix(a_star_matrix.optimized_matrix);

    display_predecessor_matrix(a_star_matrix.predecessor_matrix);
    free_matrix(graph);
    //free_matrix(graph2);
    free(graph);
    //free(graph2);
    return 0;
}
