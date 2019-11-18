#include "header/basic/test_macro.h"
#include "header/network/network_matrix_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

NETWORK_MATRIX_TEST::NETWORK_MATRIX_TEST()
{
    TEST_ADD(NETWORK_MATRIX_TEST::test_build_and_get_network_Y_matrix);
    TEST_ADD(NETWORK_MATRIX_TEST::test_build_and_get_decoupled_network_B_matrix);
    TEST_ADD(NETWORK_MATRIX_TEST::test_build_and_get_dynamic_network_Y_matrix);
    TEST_ADD(NETWORK_MATRIX_TEST::test_optimize_network_ordering);
    TEST_ADD(NETWORK_MATRIX_TEST::test_check_network_connectivity);
    TEST_ADD(NETWORK_MATRIX_TEST::test_get_islands);
    TEST_ADD(NETWORK_MATRIX_TEST::test_save_network_Y_matrix_to_file);
    TEST_ADD(NETWORK_MATRIX_TEST::test_build_network_with_bus_out_of_service);
}

void NETWORK_MATRIX_TEST::setup()
{
    network_matrix.set_toolkit(default_toolkit);
    prepare_IEEE_9_bus_model();
}

void NETWORK_MATRIX_TEST::tear_down()
{
    network_matrix.clear();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();

    show_test_end_information();
}


void NETWORK_MATRIX_TEST::test_build_and_get_network_Y_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    ostringstream osstream;

    network_matrix.build_network_Y_matrix();

    STEPS_SPARSE_MATRIX& Y = network_matrix.get_network_Y_matrix();

    osstream<<"Network Y matrix with initial physical bus : internal bus pair:";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    network_matrix.report_network_matrix();

    network_matrix.optimize_network_ordering();

    network_matrix.build_network_Y_matrix();

    Y = network_matrix.get_network_Y_matrix();

    network_matrix.report_network_matrix();
}

void NETWORK_MATRIX_TEST::test_build_and_get_decoupled_network_B_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    network_matrix.build_decoupled_network_B_matrix();

    STEPS_SPARSE_MATRIX& BP = network_matrix.get_decoupled_network_BP_matrix();
    STEPS_SPARSE_MATRIX& BQ = network_matrix.get_decoupled_network_BQ_matrix();

    network_matrix.report_decoupled_network_matrix();

    network_matrix.optimize_network_ordering();

    network_matrix.build_decoupled_network_B_matrix();

    BP = network_matrix.get_decoupled_network_BP_matrix();
    BQ = network_matrix.get_decoupled_network_BQ_matrix();

    network_matrix.report_decoupled_network_matrix();
}

void NETWORK_MATRIX_TEST::test_build_and_get_dynamic_network_Y_matrix()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    ostringstream osstream;

    network_matrix.build_dynamic_network_Y_matrix();

    STEPS_SPARSE_MATRIX Y = network_matrix.get_dynamic_network_Y_matrix();

    osstream<<"Dynamic network Y matrix with initial physical bus : internal bus pair:";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    network_matrix.report_dynamic_network_matrix();

    network_matrix.optimize_network_ordering();

    network_matrix.build_dynamic_network_Y_matrix();

    Y = network_matrix.get_dynamic_network_Y_matrix();

    osstream<<"Dynamic network Y matrix with new physical bus : internal bus pair:";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    network_matrix.report_network_matrix();
}

void NETWORK_MATRIX_TEST::test_optimize_network_ordering()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    network_matrix.build_dynamic_network_Y_matrix();

    size_t nbus = psdb.get_in_service_bus_count();
    osstream<<"Initial physical bus : internal bus pair:";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    size_t physical_bus;
    for(size_t i=0; i!=nbus; ++i)
    {
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        osstream<<setw(8)<<physical_bus<<" : "<<setw(8)<<i;
        default_toolkit.show_information_with_leading_time_stamp(osstream);
    }

    network_matrix.optimize_network_ordering();

    osstream<<"Optimized physical bus : internal bus pair:";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    for(size_t i=0; i!=nbus; ++i)
    {
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        osstream<<setw(8)<<physical_bus<<" : "<<setw(8)<<i;
        default_toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void NETWORK_MATRIX_TEST::test_check_network_connectivity()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    network_matrix.build_network_Y_matrix();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    network_matrix.check_network_connectivity();

    ostringstream osstream;
    osstream<<"Islands when tripping line 4-5 and 4-6";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    DEVICE_ID device_id;
    device_id.set_device_type("LINE");
    device_id.set_device_identifier("1");
    TERMINAL terminal;
    terminal.append_bus(4);
    terminal.append_bus(5);

    device_id.set_device_terminal(terminal);

    LINE* line;
    line = psdb.get_line(device_id);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    terminal.clear();
    terminal.append_bus(4);
    terminal.append_bus(6);

    device_id.set_device_terminal(terminal);

    line = psdb.get_line(device_id);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    network_matrix.check_network_connectivity();

    osstream<<"Islands when tripping line 4-5, 4-6, and 8-9";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    terminal.clear();
    terminal.append_bus(8);
    terminal.append_bus(9);

    device_id.set_device_terminal(terminal);

    line = psdb.get_line(device_id);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    network_matrix.check_network_connectivity();
}

void NETWORK_MATRIX_TEST::test_get_islands()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    network_matrix.build_network_Y_matrix();

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    vector< vector<size_t> > islands_internal = network_matrix.get_islands_with_internal_bus_number();
    vector< vector<size_t> > islands_physical = network_matrix.get_islands_with_physical_bus_number();

    size_t n = islands_internal.size();
    TEST_ASSERT(n==1);
    n = islands_physical.size();
    TEST_ASSERT(n==1);

    DEVICE_ID did;
    TERMINAL terminal;

    did.set_device_type("LINE");
    terminal.clear();
    terminal.append_bus(4);
    terminal.append_bus(5);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    LINE* line = psdb.get_line(did);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    network_matrix.build_network_Y_matrix();

    islands_internal = network_matrix.get_islands_with_internal_bus_number();
    islands_physical = network_matrix.get_islands_with_physical_bus_number();
    n = islands_internal.size();
    TEST_ASSERT(n==1);
    n = islands_physical.size();
    TEST_ASSERT(n==1);

    did.set_device_type("LINE");
    terminal.clear();
    terminal.append_bus(4);
    terminal.append_bus(6);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    line = psdb.get_line(did);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    network_matrix.build_network_Y_matrix();

    islands_internal = network_matrix.get_islands_with_internal_bus_number();
    islands_physical = network_matrix.get_islands_with_physical_bus_number();
    n = islands_internal.size();
    TEST_ASSERT(n==2);
    n = islands_physical.size();
    TEST_ASSERT(n==2);

    did.set_device_type("LINE");
    terminal.clear();
    terminal.append_bus(7);
    terminal.append_bus(8);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    line = psdb.get_line(did);
    line->set_sending_side_breaker_status(false);
    line->set_receiving_side_breaker_status(false);

    network_matrix.build_network_Y_matrix();

    islands_internal = network_matrix.get_islands_with_internal_bus_number();
    islands_physical = network_matrix.get_islands_with_physical_bus_number();
    n = islands_internal.size();
    TEST_ASSERT(n==3);
    n = islands_physical.size();
    TEST_ASSERT(n==3);

    for(size_t i=0; i!=n; ++i)
    {
        TEST_ASSERT(islands_internal[i].size()==islands_physical[i].size());
        size_t m = islands_internal[i].size();
        for(size_t j=0; j!=m; ++j)
            TEST_ASSERT(network_matrix.get_physical_bus_number_of_internal_bus(islands_internal[i][j])==islands_physical[i][j]);
    }

    // 1,4
    // 2, 7, 5
    // 3, 6, 8, 9
    for(size_t i=0; i!=n; ++i)
    {
        bool island_of_bus_1 = false;
        bool island_of_bus_2 = false;
        bool island_of_bus_3 = false;

        for(size_t j=0; j!= islands_physical[i].size(); ++j)
        {
            if(islands_physical[i][j]==1)
            {
                island_of_bus_1=true;
                break;
            }
            if(islands_physical[i][j]==2)
            {
                island_of_bus_2=true;
                break;
            }
            if(islands_physical[i][j]==3)
            {
                island_of_bus_3=true;
                break;
            }
        }
        TEST_ASSERT(island_of_bus_1 or island_of_bus_2 or island_of_bus_3);

        if(island_of_bus_1)
        {
            TEST_ASSERT(islands_physical[i].size()==2);
            for(size_t j=0; j!= islands_physical[i].size(); ++j)
                TEST_ASSERT(islands_physical[i][j]==1 or islands_physical[i][j]==4);
            continue;
        }
        if(island_of_bus_2)
        {
            TEST_ASSERT(islands_physical[i].size()==3);
            for(size_t j=0; j!= islands_physical[i].size(); ++j)
                TEST_ASSERT(islands_physical[i][j]==2 or islands_physical[i][j]==5 or islands_physical[i][j]==7);
            continue;
        }
        if(island_of_bus_3)
        {
            TEST_ASSERT(islands_physical[i].size()==4);
            for(size_t j=0; j!= islands_physical[i].size(); ++j)
                TEST_ASSERT(islands_physical[i][j]==3 or islands_physical[i][j]==6 or
                            islands_physical[i][j]==8 or islands_physical[i][j]==9);
            continue;
        }
    }
}

void NETWORK_MATRIX_TEST::test_save_network_Y_matrix_to_file()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    network_matrix.build_network_Y_matrix();
    network_matrix.save_network_Y_matrix_to_file("test_log/network_matrix_exported.csv");
}


void NETWORK_MATRIX_TEST::test_build_network_with_bus_out_of_service()
{
    show_test_information_for_function_of_class(__FUNCTION__,"NETWORK_MATRIX_TEST");

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(4);
    busptr->set_bus_type(OUT_OF_SERVICE);

    network_matrix.build_network_Y_matrix();
    network_matrix.save_network_Y_matrix_to_file("test_log/network_matrix_with_bus_out_of_service_exported.csv");

    network_matrix.optimize_network_ordering();
    network_matrix.build_network_Y_matrix();
    network_matrix.save_network_Y_matrix_to_file("test_log/network_matrix_with_bus_out_of_service_optimized_exported.csv");

    network_matrix.check_network_connectivity();
}

#endif
