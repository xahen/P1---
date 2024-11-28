#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "delivery_algorithm.h"
#include "delivery_sim.h"
#include "resource_calculation.h"
#include "create_routes.h"

#include <math.h>

double heuristic(node_t current_node, node_t current_node_neighbour) {
    double calc_x = pow(current_node.location_x - current_node_neighbour.location_x, 2);
    double calc_y = pow(current_node.location_y - current_node_neighbour.location_y, 2);

    double sum = calc_x + calc_y;
    double distance = sqrt(sum);
    return distance;
}

int main(void) {
    srand(time(NULL));
    graph_t *graph = generate_random_graph();
    graph_t *graph2 = create_graph(4);

    graph2->node_addresses[0] = create_node(0, 0, 1);
    graph2->node_addresses[1] = create_node(10, 20, 2);
    graph2->node_addresses[2] = create_node(20, 30, 3);
    graph2->node_addresses[3] = create_node(5, 10, 4);

    for (int i = 0; i < 4; i++) {
        printf("Node address: %d\n", graph2->node_addresses[i]->id);
    }


    for (int i = 0; i < 4; i++) {
        add_edge(
        graph2,
        graph2->node_addresses[0]->id-1,
        graph2->node_addresses[i]->id-1,
        ceil(heuristic(*graph2->node_addresses[0], *graph2->node_addresses[i])));
    }

    display_matrix(graph);
    display_matrix(graph2);
    display_predecessor_matrix(graph2);
    free_matrix(graph);
    free_matrix(graph2);
    free(graph);
    free(graph2);
    return 0;
}
