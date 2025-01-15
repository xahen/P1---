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
    //int seed = 1734538595;
    srand(seed);

    int amount_of_nodes = 0;
    while (amount_of_nodes < 4 || amount_of_nodes > 26) {
        printf("Input amount of nodes (4-26): \n");
        scanf("%d", &amount_of_nodes);
    }
    int amount_of_trucks = 0;
    while (amount_of_trucks < 1) {
        printf("Input amount of trucks (>=1): \n");
        scanf("%d", &amount_of_trucks);
    }
    int amount_of_staff = 0;
    while (amount_of_staff < amount_of_trucks) {
        printf("Input amount of staff (>=%d): \n", amount_of_trucks);
        scanf("%d", &amount_of_staff);
    }
    int amount_of_packages = 0;
    while (amount_of_packages < amount_of_nodes) {
        printf("Input amount of packages (>=%d): \n", amount_of_nodes);
        scanf("%d", &amount_of_packages);
    }

    double volume_filled = 0;
    double weight_filled = 0;
    int truck_amount = 0;
    for (int i = 0; i < amount_of_packages; i++) {
        package_t *package = generate_random_package();
        calculate_trucks(*package, &volume_filled, &weight_filled, &truck_amount);
        if (truck_amount > 1) {
            printf("The trucks can't hold any more packages!\n");
            printf("Packages left in the terminal: %d\n", amount_of_packages - i);
            break;
        }
    }
    printf("Press any key to continue...\n");
    printf("\n\n");
    getchar();
    getchar();


    graph_t *graph = generate_random_graph(amount_of_nodes);
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
