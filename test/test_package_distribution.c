#include <time.h>

#include "mtest.h"
#include "delivery_sim.h"

TEST_CASE(test_generate_random_package, {
    srand(time(NULL));

    for (int i = 0; i < 5; i++) {
        package_t mypackage = generate_random_package();

        CHECK_EQ_INT(mypackage.height >= 1 && mypackage.height <= 2, 1);
        CHECK_EQ_INT(mypackage.width >= 1 && mypackage.width <= 2, 1);
        CHECK_EQ_INT(mypackage.length >= 1 && mypackage.length <= 2, 1);
        CHECK_EQ_INT(mypackage.weight >= 1 && mypackage.weight <= 25, 1);
        CHECK_EQ_INT(mypackage.priority >= 1 && mypackage.priority <= 5, 1);
        CHECK_EQ_INT(mypackage.node_id >= 1 && mypackage.node_id <= 5, 1);
        CHECK_EQ_INT(mypackage.truck_id >= 1 && mypackage.truck_id <= 5, 1);
    }
})

MAIN_RUN_TESTS(test_generate_random_package);