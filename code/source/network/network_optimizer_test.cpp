#include "header/network/network_optimizer_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/network/y_matrix_builder.h"
#include "header/steps_namespace.h"

#include <cstdio>

using namespace std;


NETWORK_OPTIMIZER_TEST::NETWORK_OPTIMIZER_TEST()
{
    TEST_ADD(NETWORK_OPTIMIZER_TEST::test_optimize_network_ordering);
    TEST_ADD(NETWORK_OPTIMIZER_TEST::test_check_network_connectivity);
}

void NETWORK_OPTIMIZER_TEST::setup()
{
    prepare_IEEE_9_bus_model();
}

void NETWORK_OPTIMIZER_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    db->clear_database();
}

void NETWORK_OPTIMIZER_TEST::test_optimize_network_ordering()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_OPTIMIZER_TEST");

    NETWORK_OPTIMIZER optimizer;
    optimizer.optimize_network_ordering();

    Y_MATRIX_BUILDER builder;
    SPARSE_MATRIX Y = builder.build_network_matrix();
    Y.report_with_bus_number();
}

void NETWORK_OPTIMIZER_TEST::test_check_network_connectivity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_OPTIMIZER_TEST");

    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    NETWORK_OPTIMIZER optimizer;
    optimizer.optimize_network_ordering();

    optimizer.check_newtork_connectivity();

    char info[256];
    sprintf(info,"Islands when tripping line 4-5 and 4-6");
    show_information_with_leading_time_stamp(info);

    DEVICE_ID device_id;
    device_id.set_device_type("LINE");
    device_id.set_device_identifier("1");
    TERMINAL terminal;
    terminal.append_bus(4);
    terminal.append_bus(5);

    device_id.set_device_terminal(terminal);

    LINE* line;
    line = db->get_line(device_id);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    terminal.clear();
    terminal.append_bus(4);
    terminal.append_bus(6);

    device_id.set_device_terminal(terminal);

    line = db->get_line(device_id);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    optimizer.check_newtork_connectivity();

    sprintf(info,"Islands when tripping line 4-5, 4-6, and 8-9");
    show_information_with_leading_time_stamp(info);

    terminal.clear();
    terminal.append_bus(8);
    terminal.append_bus(9);

    device_id.set_device_terminal(terminal);

    line = db->get_line(device_id);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    optimizer.check_newtork_connectivity();
}
