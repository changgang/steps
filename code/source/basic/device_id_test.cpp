#include "header/basic/device_id_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
using namespace std;

DEVICE_ID_TEST::DEVICE_ID_TEST()
{
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_generator);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_wt_generator);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_pv_source);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_battery);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_load);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_fixedshunt);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_switchedshunt);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_equivalent_device);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_line);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_transformer);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_hvdc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_vschvdc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_facts);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_multidc);
    TEST_ADD(DEVICE_ID_TEST::test_constructor_and_get_device_type_arbitrary);
    TEST_ADD(DEVICE_ID_TEST::test_set_get_device_identifier);
    TEST_ADD(DEVICE_ID_TEST::test_get_device_name);
    TEST_ADD(DEVICE_ID_TEST::test_is_valid);
    TEST_ADD(DEVICE_ID_TEST::test_clear);
    TEST_ADD(DEVICE_ID_TEST::test_copy_with_operator_equal);
    TEST_ADD(DEVICE_ID_TEST::test_compare_less);
    TEST_ADD(DEVICE_ID_TEST::test_compare_equal_not_equal);
}
void DEVICE_ID_TEST::setup()
{
    device_id = new DEVICE_ID;

    terminal_1_bus.append_bus(1);

    terminal_2_bus = terminal_1_bus;
    terminal_2_bus.append_bus(2);

    terminal_3_bus = terminal_2_bus;
    terminal_3_bus.append_bus(3);

    terminal_4_bus = terminal_3_bus;
    terminal_4_bus.append_bus(4);

    terminal_100_bus = terminal_4_bus;
    for(size_t i=5; i<=100; ++i)
        terminal_100_bus.append_bus(i);

    terminal_101_bus = terminal_100_bus;
    terminal_101_bus.append_bus(101);
}
void DEVICE_ID_TEST::tear_down()
{
    delete device_id;

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

    device_id->set_device_type("generator");
    TEST_ASSERT(device_id->get_device_type()=="GENERATOR");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_wt_generator()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("wt generator");
    TEST_ASSERT(device_id->get_device_type()=="WT GENERATOR");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_pv_source()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("pv source");
    TEST_ASSERT(device_id->get_device_type()=="PV SOURCE");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_battery()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("battery");
    TEST_ASSERT(device_id->get_device_type()=="BATTERY");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_load()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("load");
    TEST_ASSERT(device_id->get_device_type()=="LOAD");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_fixedshunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("fixed shunt");
    TEST_ASSERT(device_id->get_device_type()=="FIXED SHUNT");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_switchedshunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("switched shunt");
    TEST_ASSERT(device_id->get_device_type()=="SWITCHED SHUNT");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_equivalent_device()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("equivalent device");
    TEST_ASSERT(device_id->get_device_type()=="EQUIVALENT DEVICE");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==1);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==1);

    device_id->set_device_terminal(terminal_1_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==1);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_line()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("line");
    TEST_ASSERT(device_id->get_device_type()=="LINE");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==2);

    device_id->set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_transformer()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("transformer");
    TEST_ASSERT(device_id->get_device_type()=="TRANSFORMER");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==3);

    device_id->set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);

    device_id->set_device_terminal(terminal_3_bus);
    terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==3);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_hvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("hvdc");
    TEST_ASSERT(device_id->get_device_type()=="HVDC");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==2);

    device_id->set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_vschvdc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("vsc hvdc");
    TEST_ASSERT(device_id->get_device_type()=="VSC HVDC");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==2);

    device_id->set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_facts()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("facts");
    TEST_ASSERT(device_id->get_device_type()=="FACTS");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==2);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==2);

    device_id->set_device_terminal(terminal_2_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==2);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_multidc()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("multi dc");
    TEST_ASSERT(device_id->get_device_type()=="MULTI DC");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==3);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==100);

    device_id->set_device_terminal(terminal_3_bus);
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==3);

    device_id->set_device_terminal(terminal_4_bus);
    terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==4);

    device_id->set_device_terminal(terminal_100_bus);
    terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==100);
}

void DEVICE_ID_TEST::test_constructor_and_get_device_type_arbitrary()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    ostringstream osstream;
    osstream<<"Following will display error information of invalid device type \"ARB\". If not, error occurs.";
    show_information_with_leading_time_stamp(osstream);

    device_id->set_device_type("arb");
    TEST_ASSERT(device_id->get_device_type()=="NONE");

    TEST_ASSERT(device_id->get_minimum_allowed_terminal_count()==0);
    TEST_ASSERT(device_id->get_maximum_allowed_terminal_count()==0);
}

void DEVICE_ID_TEST::test_set_get_device_identifier()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("generator");
    device_id->set_device_identifier("ab");
    TEST_ASSERT(device_id->get_device_identifier()=="ab");

    device_id->set_device_identifier("xy");
    TEST_ASSERT(device_id->get_device_identifier()=="xy");
}

void DEVICE_ID_TEST::test_get_device_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    TERMINAL terminal;

    device_id->set_device_type("BUS");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    TEST_ASSERT(device_id->get_device_name()=="BUS 1");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("LINE");
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="LINE #1 LINKING BUS 1 AND 2");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("HVDC");
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="HVDC #1 LINKING BUS 1 AND 2");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("VSC HVDC");
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="VSC HVDC #1 LINKING BUS 1 AND 2");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("TRANSFORMER");
    terminal.append_bus(1);
    terminal.append_bus(2);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="TRANSFORMER #1 LINKING BUS 1 AND 2");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("TRANSFORMER");
    terminal.append_bus(1);
    terminal.append_bus(2);
    terminal.append_bus(3);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="TRANSFORMER #1 LINKING BUS 1 2 AND 3");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("GENERATOR");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="GENERATOR #1 AT BUS 1");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("WT GENERATOR");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="WT GENERATOR #1 AT BUS 1");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("LOAD");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="LOAD #1 AT BUS 1");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("FIXED SHUNT");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="FIXED SHUNT #1 AT BUS 1");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("SWITCHED SHUNT");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="SWITCHED SHUNT #1 AT BUS 1");

    device_id->clear();
    terminal.clear();
    device_id->set_device_type("EQUIVALENT DEVICE");
    terminal.append_bus(1);
    device_id->set_device_terminal(terminal);
    device_id->set_device_identifier("#1");
    TEST_ASSERT(device_id->get_device_name()=="EQUIVALENT DEVICE #1 AT BUS 1");
}

void DEVICE_ID_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("generator");
    device_id->set_device_terminal(terminal_1_bus);
    device_id->set_device_identifier("ab");

    TEST_ASSERT(device_id->is_valid()==true);

    device_id->set_device_type("");
    TEST_ASSERT(device_id->is_valid()==false);

    device_id->clear();
    TEST_ASSERT(device_id->is_valid()==false);
}

void DEVICE_ID_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("generator");
    device_id->set_device_terminal(terminal_1_bus);
    device_id->set_device_identifier("ab");

    device_id->clear();

    TEST_ASSERT(device_id->get_device_type()=="NONE");
    TERMINAL terminal = device_id->get_device_terminal();
    TEST_ASSERT(terminal.get_bus_count()==0);
    TEST_ASSERT(device_id->get_device_identifier()=="");
}

void DEVICE_ID_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("generator");
    device_id->set_device_terminal(terminal_1_bus);
    device_id->set_device_identifier("ab");

    DEVICE_ID newdevice_id = (*device_id);

    TEST_ASSERT(newdevice_id.get_device_type()=="GENERATOR");
    TERMINAL terminal = newdevice_id.get_device_terminal();
    TEST_ASSERT(terminal[0]==1);
    TEST_ASSERT(terminal.get_bus_count()==1);
    TEST_ASSERT(newdevice_id.get_device_identifier()=="ab");
}


void DEVICE_ID_TEST::test_compare_less()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("GENERATOR");
    device_id->set_device_terminal(terminal_1_bus);
    device_id->set_device_identifier("ab");

    DEVICE_ID newdevice_id;
    newdevice_id.set_device_type("GENERATOR");
    newdevice_id.set_device_terminal(terminal_1_bus);
    newdevice_id.set_device_identifier("ac");

    TEST_ASSERT((*device_id)<newdevice_id);
    newdevice_id.set_device_identifier("ab");
    TEST_ASSERT(not((*device_id)<newdevice_id));
    TEST_ASSERT(not(newdevice_id<(*device_id)));

    terminal_1_bus.clear();
    terminal_1_bus.append_bus(2);
    newdevice_id.set_device_terminal(terminal_1_bus);
    TEST_ASSERT((*device_id)<newdevice_id);
}

void DEVICE_ID_TEST::test_compare_equal_not_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_ID_TEST");

    device_id->set_device_type("GENERATOR");
    device_id->set_device_terminal(terminal_1_bus);
    device_id->set_device_identifier("ab");

    DEVICE_ID newdevice_id;
    newdevice_id.set_device_type("GENERATOR");
    newdevice_id.set_device_terminal(terminal_1_bus);
    newdevice_id.set_device_identifier("ab");

    TEST_ASSERT((*device_id)==newdevice_id);

    terminal_1_bus.clear();
    terminal_1_bus.append_bus(2);
    newdevice_id.set_device_terminal(terminal_1_bus);
    newdevice_id.set_device_identifier("ab");
    TEST_ASSERT((*device_id)!=newdevice_id);
}



