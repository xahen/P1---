#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "delivery_sim.h"

#include <delivery_algorithm.h>
#include <iso646.h>
#include <time.h>

#include "create_routes.h"
#include "astar_helper_functions.h"

char* get_delivery_status(delivery_status_e delivery_status) {
    switch (delivery_status) {
        case 0: return "NOT_DELIVERED";
        case 1: return "BEING_DELIVERED";
        case 2: return "DELIVERED";
        default: return NULL;
    }
}

void print_package(int selector, package_t package) {
    switch(selector) {
        case 0: printf("Package height: %.2lf meters\n", package.height);
            break;
        case 1: printf("Package width: %.2lf meters\n", package.width);
            break;
        case 2: printf("Package length: %.2lf meters\n", package.length);
            break;
        case 3: printf("Package weight: %.2lf kilo\n", package.weight);
            break;
        case 4: printf("Package priority: %d\n", package.priority);
            break;
        case 5: printf("Package node_id: %d\n", package.node_id);
            break;
        case 6: printf("Package truck_id: %d\n", package.truck_id);
            break;
        case 7: printf("Package delivery_status: %s\n", get_delivery_status(package.delivery_status));
            break;
        default:
            for (int i = 0; i < 8; i++) {
                print_package(i, package);
            }
    }
}


void print_node(int selector, node_t node) {
    switch(selector) {
        case 0: printf("Node location_x: %d\n", node.location_x);
            break;
        case 1: printf("Node location_y: %d\n", node.location_y);
            break;
        case 2: printf("Node id: %d\n", node.id);
            break;
        case 3:
            printf("Node packages: ");
            printf("----------\n");
            for (int i = 0; i < 30; i++) {
                print_package(-1, node.packages[i]);
            }
            printf("----------\n");
            break;
        default:
            for (int i = 0; i < 4; i++) {
                print_node(i, node);
            }
    }
}

void print_truck(int spacing) {
    for (int i = 0; i < spacing; i++) printf(" ");
    printf(" _______\n");
    for (int i = 0; i < spacing; i++) printf(" ");
    printf("|       |\\\n");
    for (int i = 0; i < spacing; i++) printf(" ");
    printf("|_______|_|\n");
    for (int i = 0; i < spacing; i++) printf(" ");
    printf(" O     O\n");

    for (int i = 0; i < 90; i++) {
        printf("%c", 238);
    }
    printf("\n");
}

void run_simulation(a_star_matrix_t a_star_matrix, int *routes, int depot) {
    int average_speed = 60;

    int time_till_node[a_star_matrix.optimized_matrix->nodes][4];

    time_till_node[0][0] = depot;
    time_till_node[0][1] = routes[0];
    //                                                                                         KM/T            Min  Sek
    double time_spent = (double)a_star_matrix.optimized_matrix->adj_matrix[depot][routes[0]] / average_speed * 60 /** 60*/;
    time_till_node[0][2] = time_spent;
    time_till_node[0][3] = time_spent;
    for (int i = 1; i < a_star_matrix.optimized_matrix->nodes - 1; i++) {
        time_till_node[i][0] = routes[i - 1];
        time_till_node[i][1] = routes[i];
        time_till_node[i][2] = (double)a_star_matrix.optimized_matrix->adj_matrix[routes[i]][routes[i - 1]] / average_speed * 60 /** 60*/;
        time_till_node[i][3] = time_till_node[i][2] + time_till_node[i-1][3];
    }
    time_till_node[a_star_matrix.optimized_matrix->nodes - 1][0] = routes[a_star_matrix.optimized_matrix->nodes - 2];
    time_till_node[a_star_matrix.optimized_matrix->nodes - 1][1] = depot;
    time_till_node[a_star_matrix.optimized_matrix->nodes - 1][2] = (double)a_star_matrix.optimized_matrix->adj_matrix[depot][routes[a_star_matrix.optimized_matrix->nodes - 2]] / average_speed * 60 /** 60*/;
    time_till_node[a_star_matrix.optimized_matrix->nodes - 1][3] = time_till_node[a_star_matrix.optimized_matrix->nodes - 1][2] + time_till_node[a_star_matrix.optimized_matrix->nodes - 2][3];

    int total_time_left = 0;
    for (int i = 0; i < a_star_matrix.optimized_matrix->nodes; i++) {
        total_time_left += time_till_node[i][2];
    }

    for (int i = 0; i < a_star_matrix.optimized_matrix->nodes; i++) {
        printf("%c %c %d\n", 'A' + time_till_node[i][0], 'A' + time_till_node[i][1], time_till_node[i][2]);
    }

    printf("Time left: %d\n", total_time_left);
    getchar();

    int original_total_time = total_time_left;
    int control_points_reached = 0;
    int temp_time_till_node = time_till_node[0][2];
    int delivery_point_reached = 0;
    int current_delay = 0;
    int original_schedule = total_time_left;
    int truck_movement = 0;
    int truck_movement_trigger = floor(original_schedule / 60);
    int time_till_next_control_point = 0;
    int clock = 0;

    for (int i = 0; i < a_star_matrix.optimized_matrix->nodes; i++) {
        int control_point_not_reached = 1;
        if (delivery_point_reached == 1) {
            temp_time_till_node = time_till_node[i][2];
            delivery_point_reached = 0;
            control_points_reached = 0;
        }
        time_till_next_control_point = ceil(time_till_node[i][2] / 4);
        while (control_point_not_reached) {
            system("cls");

            printf("Route: \t\t\t\t\t");
            printf("%c -> ", depot + 'A');
            for (int i = 0; routes[i] != -1; i++) {
                printf("%c -> ", 'A' + routes[i]);
            }
            printf("%c\n", depot + 'A');

            printf("Current route: \t\t\t\t");
            for (int j = 0; j < i * 5; j++) {
                printf(" ");
            }
            printf("%c -> %c\n", time_till_node[i][0] + 'A', time_till_node[i][1] + 'A');

            printf("\n");

            if (original_schedule <= 0) {
                printf("Original time left (no delays): \tROUTE ENDED\n", original_schedule);
            } else {
                printf("Original time left (no delays): \t%d minutes\n", original_schedule);
            }

            printf("Total time left: \t\t\t%d minutes\n", total_time_left);
            printf("Behind schedule: \t\t\t%d minutes\n", total_time_left - original_schedule);
            printf("\n");

            printf("Control points reached: \t\t%d\n", control_points_reached);
            if (control_points_reached != 3) {
                printf("Time till next control point: \t\t%d minutes\n", time_till_next_control_point);
            } else {
                printf("Time till next control point: \t\tALL CONTROL POINTS REACHED\n");
            }

            printf("Time till next node: \t\t\t%d minutes\n", temp_time_till_node);
            printf("\n");

            printf("Delay added this control point: \t%d minutes\n", current_delay);

            printf("\n\n");

            if (original_schedule % truck_movement_trigger == 0) {
                truck_movement++;
            }

            print_truck(truck_movement);

            printf("%c: \t\t\t\t\tSTARTED\n", 'A' + depot);
            for (int j = 0; routes[j] != -1; j++) {
                if (time_till_node[j][3] <= 0) {
                    printf("%c: \t\t\t\t\tDELIVERED\n", 'A' + routes[j]);
                } else {
                    printf("%c: \t\t\t\t\t%d\n", 'A' + routes[j], time_till_node[j][3]);
                }
            }
            if (total_time_left <= 1) {
                printf("%c: \t\t\t\t\tENDED\n", 'A' + depot);
            } else {
                printf("%c: \t\t\t\t\t%d\n", 'A' + depot, total_time_left);
            }

            total_time_left--;
            original_schedule--;
            temp_time_till_node--;
            time_till_next_control_point--;
            clock++;


            if (temp_time_till_node < 1) {
                delivery_point_reached = 1;
                control_point_not_reached = 0;
            } else if (time_till_next_control_point < 1 && control_points_reached < 3) {
                control_point_not_reached = 0;
                control_points_reached++;
            }

            sleep(1);
        }

        if (temp_time_till_node > 0) {
            // Add possible delays.
            if (rand() % 100 <= 50) {
                current_delay = rand() % 11;
                total_time_left += current_delay;
                temp_time_till_node += current_delay;
                time_till_node[i][2] += current_delay;

                for (int j = 0; routes[j] != -1; j++) {
                    time_till_node[j][3] += current_delay;
                }
            } else {
                current_delay = 0;
            }

            for (int j = 0; routes[j] != -1; j++) {
                time_till_node[j][3] -= clock;
            }
            time_till_node[i][3] = temp_time_till_node;
            clock = 0;

            i--;
        } else {
            for (int j = 0; routes[j] != -1; j++) {
                time_till_node[j][3] -= clock;
            }
            clock = 0;
            current_delay = 0;
        }

        if (total_time_left <= 1) {
            total_time_left = 0;
        }
    }

    printf("\n\nFinished delivery to all nodes!\n");
}

package_t *generate_random_package() {
    int priority = rand() % 5 + 1;
    int node_id = rand() % 5 + 1;
    int truck_id = rand() % 5 + 1;

    double height = (rand() % 200 + 1) / 100.0;
    double width = (rand() % 200 + 1) / 100.0;
    double length = (rand() % 200 + 1) / 100.0;
    double weight = (rand() % 2500 + 1) / 100.0;

    package_t *package = create_package(priority, node_id, truck_id, height, width, length, weight);

    return package;
}

node_t *generate_random_node() {
    int id = 0;

    int position_x = rand() % 100;
    int position_y = rand() % 100;

    node_t *node = create_node(position_x, position_y, id);

    return node;
}

// Should this really create a pointer?
// The same goes for the create_graph function...
graph_t *generate_random_graph(int amount_of_nodes) {
    //int node_amount = rand() % 22 + 4;
    int node_amount = amount_of_nodes;

    graph_t *graph = create_graph(node_amount);

    for (int i = 0; i < node_amount; i++) {
        node_t *node = generate_random_node();
        node->id = i + 1;
        graph->node_addresses[i] = node;
    }

    for (int i = 0; i < node_amount * (node_amount - 2); i++) {
        int random_node_src = rand() % node_amount;
        int random_node_dst = rand() % node_amount;
        if (random_node_src == random_node_dst) {
            continue;
        }
        add_edge(graph, random_node_src, random_node_dst, ceil(heuristic(*graph->node_addresses[random_node_src], *graph->node_addresses[random_node_dst])));
    }

    for (int i = 0; i < node_amount - 1; i++) {
        add_edge(graph, i, i + 1, ceil(heuristic(*graph->node_addresses[i], *graph->node_addresses[i+1])));
    }

    return graph;
}