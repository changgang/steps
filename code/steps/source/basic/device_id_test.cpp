#include "header/basic/test_macro.h"
#include "header/basic/device_id_test.h"
#include "header/basic/utility.h"
#include "header/basic/test_macro.h"
#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

DEVICE_ID_TEST::DEVICE_ID_TEST()
{
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_generator);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_wt_generator);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_pv_unit);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_energy_storage);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_load);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_fixedshunt);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_switchedshunt);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_equivalent_device);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_line);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_transformer);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_hvdc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_vsc_hvdc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_facts);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_multidc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_lcc_hvdc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_general);
    TEST_ADD(DEVICE_ID_TEST::test_set_get_device_identifier);
    TEST_ADD(DEVICE_ID_TEST::test_get_compound_device_name);
    TEST_ADD(DEVICE_ID_TEST::test_is_valid);
    TEST_ADD(DEVICE_ID_TEST::test_clear);
    TEST_ADD(DEVICE_ID_TEST::test_copy_with_operator_equal);
    TEST_ADD(DEVICE_ID_TEST::test_compare_less);
    TEST_ADD(DEVICE_ID_TEST::test_compare_equal_not_equal);
}

void DEVICE_ID_TEST::setup()
{
    terminal_1_bus.append_bus(1);

    terminal_2_bus = terminal_1_bus;
    terminal_2_bus.append_bus(2);

    terminal_3_bus = terminal_2_bus;
    terminal_3_bus.append_bus(3);

    terminal_4_bus = terminal_3_bus;
    terminal_4_bus.append_bus(4);

    terminal_100_bus = terminal_4_bus;
    for(unsigned int i=5; i<=100; ++i)
        terminal_100_bus.append_bus(i);

    terminal_101_bus = terminal_100_bus;
    terminal_101_bus.append_bus(101);
}

void DEVICE_ID_TEST::tear_down()
{
    device_id.clear();

    terminal_1_bus.clear();
    terminal_2_bus.clear();
    terminal_3_bus.clear();
    terminal_4_bus.clear();
    terminal_100_bus.clear();
    terminal_101_bus.clear();

    show_test_end_information();
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    TEST_ASSERT(device_id.get_device_type()==STEPS_GENERATOR);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_wt_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_WT_GENERATOR);
    TEST_ASSERT(device_id.get_device_type()==STEPS_WT_GENERATOR);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_pv_unit()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_PV_UNIT);
    TEST_ASSERT(device_id.get_device_type()==STEPS_PV_UNIT);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_energy_storage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_ENERGY_STORAGE);
    TEST_ASSERT(device_id.get_device_type()==STEPS_ENERGY_STORAGE);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_LOAD);
    TEST_ASSERT(device_id.get_device_type()==STEPS_LOAD);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_fixedshunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_FIXED_SHUNT);
    TEST_ASSERT(device_id.get_device_type()==STEPS_FIXED_SHUNT);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_switchedshunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_SWITCHED_SHUNT);
    TEST_ASSERT(device_id.get_device_type()==STEPS_SWITCHED_SHUNT);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_equivalent_device()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_EQUIVALENT_DEVICE);
    TEST_ASSERT(device_id.get_device_type()==STEPS_EQUIVALENT_DEVICE);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==1);

    device_id.set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_line()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_LINE);
    TEST_ASSERT(device_id.get_device_type()==STEPS_LINE);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==2);

    device_id.set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_transformer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_TRANSFORMER);
    TEST_ASSERT(device_id.get_device_type()==STEPS_TRANSFORMER);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==3);

    device_id.set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);

    device_id.set_device_terminal(terminal_3_bus);
    terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==3);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_HVDC);
    TEST_ASSERT(device_id.get_device_type()==STEPS_HVDC);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==2);

    device_id.set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_vsc_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_VSC_HVDC);
    TEST_ASSERT(device_id.get_device_type()==STEPS_VSC_HVDC);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==100);

    device_id.set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);

    device_id.set_device_terminal(terminal_4_bus);
    terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==4);

    device_id.set_device_terminal(terminal_100_bus);
    terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==100);

    device_id.set_device_terminal(terminal_101_bus);
    terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==100);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_facts()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_FACTS);
    TEST_ASSERT(device_id.get_device_type()==STEPS_FACTS);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==2);

    device_id.set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_multidc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_HYBRID_DC);
    TEST_ASSERT(device_id.get_device_type()==STEPS_HYBRID_DC);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==3);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==100);

    device_id.set_device_terminal(terminal_3_bus);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==3);

    device_id.set_device_terminal(terminal_4_bus);
    terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==4);

    device_id.set_device_terminal(terminal_100_bus);
    terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==100);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_lcc_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_LCC_HVDC);
    TEST_ASSERT(device_id.get_device_type()==STEPS_LCC_HVDC);

    TEST_ASSERT(device_id.is_name_allowed()==true);
    TEST_ASSERT(device_id.is_terminal_allowed()==false);
    TEST_ASSERT(device_id.is_identifier_allowed()==false);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==0);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==0);

    add_string_to_str_int_map("LCC HVDC A");
    device_id.set_device_name_index(get_index_of_string("LCC HVDC A"));
    TEST_ASSERT(device_id.get_device_name()=="LCC HVDC A");
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_general()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERAL_DEVICE);
    TEST_ASSERT(device_id.get_device_type()==STEPS_GENERAL_DEVICE);

    TEST_ASSERT(device_id.is_name_allowed()==false);
    TEST_ASSERT(device_id.is_terminal_allowed()==true);
    TEST_ASSERT(device_id.is_identifier_allowed()==true);

    TEST_ASSERT(device_id.get_minimum_allowed_terminal_count()==0);
    TEST_ASSERT(device_id.get_maximum_allowed_terminal_count()==100);
}

void DEVICE_ID_TEST::test_set_get_device_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    device_id.set_device_identifier_index(0);
    TEST_ASSERT(device_id.get_device_identifier_index()==0);

    add_string_to_str_int_map("xy");
    device_id.set_device_identifier_index(get_index_of_string("xy"));
    TEST_ASSERT(device_id.get_device_identifier_index()==get_index_of_string("xy"));
}

void DEVICE_ID_TEST::test_get_compound_device_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    TERMINAL terminal;

    device_id.set_device_type(STEPS_BUS);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_id.get_compound_device_name()=="BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_LINE);
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id.set_device_terminal(terminal);
    add_string_to_str_int_map("#1");
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="LINE #1 LINKING BUS 1 AND 2");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_HVDC);
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="HVDC #1 LINKING BUS 1 AND 2");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_VSC_HVDC);
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="VSC HVDC #1 LINKING BUS 1 AND 2");

    terminal.append_bus(3);
    terminal.append_bus(5);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="VSC HVDC #1 LINKING BUS 1 2 3 AND 5");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_TRANSFORMER);
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="TRANSFORMER #1 LINKING BUS 1 AND 2");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_TRANSFORMER);
    terminal.append_bus(1);
    terminal.append_bus(2);
    terminal.append_bus(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="TRANSFORMER #1 LINKING BUS 1 2 AND 3");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_GENERATOR);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="GENERATOR #1 AT BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_WT_GENERATOR);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="WT GENERATOR #1 AT BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_LOAD);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="LOAD #1 AT BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_FIXED_SHUNT);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="FIXED SHUNT #1 AT BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_SWITCHED_SHUNT);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="SWITCHED SHUNT #1 AT BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_EQUIVALENT_DEVICE);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="EQUIVALENT DEVICE #1 AT BUS 1");

    device_id.clear();
    terminal.clear();
    device_id.set_device_type(STEPS_ENERGY_STORAGE);
    terminal.append_bus(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("#1"));
    TEST_ASSERT(device_id.get_compound_device_name()=="ENERGY STORAGE #1 AT BUS 1");

    device_id.clear();
    device_id.set_device_type(STEPS_LCC_HVDC);
    add_string_to_str_int_map("LCC-A");
    device_id.set_device_name_index(get_index_of_string("LCC-A"));
    TEST_ASSERT(device_id.get_compound_device_name()=="LCC HVDC LCC-A");
}

void DEVICE_ID_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    device_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ab");
    device_id.set_device_identifier_index(get_index_of_string("ab"));

    TEST_ASSERT(device_id.is_valid()==true);

    device_id.set_device_type(STEPS_GENERAL_DEVICE);
    TEST_ASSERT(device_id.is_valid()==true);

    device_id.clear();
    TEST_ASSERT(device_id.is_valid()==false);
}

void DEVICE_ID_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    device_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ab");
    device_id.set_device_identifier_index(get_index_of_string("ab"));

    device_id.clear();

    TEST_ASSERT(device_id.get_device_type()==STEPS_INVALID_DEVICE);
    TERMINAL terminal = device_id.get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==0);
    TEST_ASSERT(device_id.get_device_identifier()=="");
    TEST_ASSERT(device_id.get_device_identifier_index()==0);
    TEST_ASSERT(device_id.get_device_name()=="");
    TEST_ASSERT(device_id.get_device_name_index()==0);
}

void DEVICE_ID_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    device_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ab");
    device_id.set_device_identifier_index(get_index_of_string("ab"));

    DEVICE_ID newdevice_id = device_id;

    TEST_ASSERT(newdevice_id.get_device_type()==STEPS_GENERATOR);
    TERMINAL terminal = newdevice_id.get_device_terminal();
    TEST_ASSERT(terminal[0]==1);
    TEST_ASSERT(terminal.get_bus_count()==1);
    TEST_ASSERT(newdevice_id.get_device_identifier()=="ab");
}


void DEVICE_ID_TEST::test_compare_less()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    device_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ab");
    device_id.set_device_identifier_index(get_index_of_string("ab"));

    DEVICE_ID newdevice_id;
    newdevice_id.set_device_type(STEPS_GENERATOR);
    newdevice_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ac");
    newdevice_id.set_device_identifier_index(get_index_of_string("ac"));

    TEST_ASSERT(device_id<newdevice_id);
    newdevice_id.set_device_identifier_index(get_index_of_string("ab"));
    TEST_ASSERT(not(device_id<newdevice_id));
    TEST_ASSERT(not(newdevice_id<device_id));

    terminal_1_bus.clear();
    terminal_1_bus.append_bus(2);
    newdevice_id.set_device_terminal(terminal_1_bus);
    TEST_ASSERT(device_id<newdevice_id);
}

void DEVICE_ID_TEST::test_compare_equal_not_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id.set_device_type(STEPS_GENERATOR);
    device_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ab");
    device_id.set_device_identifier_index(get_index_of_string("ab"));


    DEVICE_ID newdevice_id;
    newdevice_id.set_device_type(STEPS_GENERATOR);
    newdevice_id.set_device_terminal(terminal_1_bus);
    add_string_to_str_int_map("ab");
    newdevice_id.set_device_identifier_index(get_index_of_string("ab"));


    TEST_ASSERT(device_id==newdevice_id);

    terminal_1_bus.clear();
    terminal_1_bus.append_bus(2);
    newdevice_id.set_device_terminal(terminal_1_bus);
    device_id.set_device_identifier_index(get_index_of_string("ab"));
    TEST_ASSERT(device_id!=newdevice_id);
}




#endif
