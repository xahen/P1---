#include "package_distribution.h"

char* get_delivery_status(delivery_status_e delivery_status);
void print_package(int selector, package_t package);
void print_node(int selector, node_t node);
void print_truck(int spacing);
void run_simulation(a_star_matrix_t a_star_matrix, int* routes, int depot);

package_t *generate_random_package();
node_t *generate_random_node();
graph_t *generate_random_graph();