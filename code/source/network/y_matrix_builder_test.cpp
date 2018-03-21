#include "header/network/y_matrix_builder_test.h"
#include "header/network/network_optimizer.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"

#include <cstdio>

using namespace std;


Y_MATRIX_BUILDER_TEST::Y_MATRIX_BUILDER_TEST()
{
    TEST_ADD(Y_MATRIX_BUILDER_TEST::test_build_and_show_network_matrix);
    //TEST_ADD(Y_MATRIX_BUILDER_TEST::test_remove_line_from_network_matrix);
    //TEST_ADD(Y_MATRIX_BUILDER_TEST::test_remove_transformer_from_network_matrix);
}

void Y_MATRIX_BUILDER_TEST::setup()
{
    prepare_IEEE_9_bus_model();
}

void Y_MATRIX_BUILDER_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    db->clear_database();
}

void Y_MATRIX_BUILDER_TEST::test_build_and_show_network_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"Y_MATRIX_BUILDER_TEST");

    char info[256];

    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    STEPS::inphno.clear();
    STEPS::inphno.set_physical_internal_bus_number_pair(4,0);
    STEPS::inphno.set_physical_internal_bus_number_pair(7,1);
    STEPS::inphno.set_physical_internal_bus_number_pair(9,2);
    STEPS::inphno.set_physical_internal_bus_number_pair(5,3);
    STEPS::inphno.set_physical_internal_bus_number_pair(6,4);
    STEPS::inphno.set_physical_internal_bus_number_pair(8,5);
    STEPS::inphno.set_physical_internal_bus_number_pair(1,6);
    STEPS::inphno.set_physical_internal_bus_number_pair(2,7);
    STEPS::inphno.set_physical_internal_bus_number_pair(3,8);

    size_t nbus = db->get_bus_count();
    sprintf(info,"Initial physical bus : internal bus pair:");
    show_information_with_leading_time_stamp(info);

    for(size_t i=0; i<nbus; i++)
    {
        sprintf(info,"%-8u : %-8u",get_physical_bus_number_of_internal_bus(i),i);
        show_information_with_leading_time_stamp(info);
    }

    Y_MATRIX_BUILDER builder;
    SPARSE_MATRIX Y = builder.build_network_matrix();

    sprintf(info,"Network Y matrix with initial physical bus : internal bus pair:");
    show_information_with_leading_time_stamp(info);

    Y.report_brief_with_bus_number();

    NETWORK_OPTIMIZER optimizer;
    optimizer.optimize_network_ordering();

    Y = builder.build_network_matrix();

    sprintf(info,"Network Y matrix with new physical bus : internal bus pair:");
    show_information_with_leading_time_stamp(info);

    Y.report_brief_with_bus_number();
}

/*
void Y_MATRIX_BUILDER_TEST::test_remove_line_from_network_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"Y_MATRIX_BUILDER_TEST");

    char info[256];

    STEPS::inphno.initialize();
    STEPS::inphno.set_physical_internal_bus_number_pair(4,0);
    STEPS::inphno.set_physical_internal_bus_number_pair(7,1);
    STEPS::inphno.set_physical_internal_bus_number_pair(9,2);
    STEPS::inphno.set_physical_internal_bus_number_pair(5,3);
    STEPS::inphno.set_physical_internal_bus_number_pair(6,4);
    STEPS::inphno.set_physical_internal_bus_number_pair(8,5);
    STEPS::inphno.set_physical_internal_bus_number_pair(1,6);
    STEPS::inphno.set_physical_internal_bus_number_pair(2,7);
    STEPS::inphno.set_physical_internal_bus_number_pair(3,8);
    set_clock_when_physical_internal_bus_pair_set();

    Y_MATRIX_BUILDER builder;
    builder.build_network_matrix();

    sprintf(info,"Initial network Y matrix");
    show_information_with_leading_time_stamp(info);

    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,3) != 0.0);

    builder.show_network_matrix();

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type("LINE");
    terminal.append_bus(4);
    terminal.append_bus(5);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier("1");

    size_t index = get_line_index(device_id);
    if(index != INDEX_NOT_EXIST)
        builder.remove_line_from_network(STEPS::Line[index]);

    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,3) == 0.0);

    sprintf(info,"Network Y matrix after removing line 4-5 '1'");
    show_information_with_leading_time_stamp(info);
    builder.show_network_matrix();

    if(index != INDEX_NOT_EXIST)
        builder.add_line_to_network(STEPS::Line[index]);

    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,3) != 0.0);

    sprintf(info,"Network Y matrix after adding line 4-5 '1'");
    show_information_with_leading_time_stamp(info);
    builder.show_network_matrix();
}



void Y_MATRIX_BUILDER_TEST::test_remove_transformer_from_network_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"Y_MATRIX_BUILDER_TEST");

    char info[256];

    STEPS::inphno.initialize();
    STEPS::inphno.set_physical_internal_bus_number_pair(4,0);
    STEPS::inphno.set_physical_internal_bus_number_pair(7,1);
    STEPS::inphno.set_physical_internal_bus_number_pair(9,2);
    STEPS::inphno.set_physical_internal_bus_number_pair(5,3);
    STEPS::inphno.set_physical_internal_bus_number_pair(6,4);
    STEPS::inphno.set_physical_internal_bus_number_pair(8,5);
    STEPS::inphno.set_physical_internal_bus_number_pair(1,6);
    STEPS::inphno.set_physical_internal_bus_number_pair(2,7);
    STEPS::inphno.set_physical_internal_bus_number_pair(3,8);
    set_clock_when_physical_internal_bus_pair_set();

    Y_MATRIX_BUILDER builder;
    builder.build_network_matrix();

    sprintf(info,"Initial network Y matrix");
    show_information_with_leading_time_stamp(info);

    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,3) != 0.0);

    builder.show_network_matrix();


    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,6) != 0.0);

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type("TRANSFORMER");
    terminal.append_bus(1);
    terminal.append_bus(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier("1");

    size_t index = get_transformer_index(device_id);
    if(index != INDEX_NOT_EXIST)
        builder.remove_transformer_from_network(STEPS::Transformer[index]);

    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,6) == 0.0);

    sprintf(info,"Network Y matrix after removing transformer 1-4 '1'");
    show_information_with_leading_time_stamp(info);
    builder.show_network_matrix();

    if(index != INDEX_NOT_EXIST)
        builder.add_transformer_to_network(STEPS::Transformer[index]);

    TEST_ASSERT(STEPS::network_Y_matrix.get_entry_value(0,6) != 0.0);

    sprintf(info,"Network Y matrix after adding transformer 1-4 '1'");
    show_information_with_leading_time_stamp(info);
    builder.show_network_matrix();


}

*/
