#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/basic/device_index_map_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;


DEVICE_INDEX_MAP_TEST::DEVICE_INDEX_MAP_TEST()
{
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_constructor);
    //TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_bus_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_generator_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_load_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_fixed_shunt_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_switched_shunt_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_line_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_transformer_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_set_get_energy_storage_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_empty);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_clear_index);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_get_index_of_device_and_operator_bracket);
    TEST_ADD(DEVICE_INDEX_MAP_TEST::test_decrease_index_by_1_for_device_with_index_greater_than);

    //TEST_ADD(DEVICE_INDEX_MAP_TEST::test_index_performance_with_many_buses);
}

void DEVICE_INDEX_MAP_TEST::setup()
{
    ;
}
void DEVICE_INDEX_MAP_TEST::tear_down()
{
    device_index_map.clear();

    show_test_end_information();
}

TERMINAL DEVICE_INDEX_MAP_TEST::prepare_terminal(unsigned int bus)
{
    TERMINAL terminal;
    terminal.append_bus(bus);
    return terminal;
}

TERMINAL DEVICE_INDEX_MAP_TEST::prepare_terminal(unsigned int ibus,unsigned int jbus)
{
    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    return terminal;
}

TERMINAL DEVICE_INDEX_MAP_TEST::prepare_terminal(unsigned int ibus,unsigned int jbus, unsigned int kbus)
{
    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    terminal.append_bus(kbus);
    return terminal;
}

void DEVICE_INDEX_MAP_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    TEST_ASSERT(device_index_map.empty() == true);
}

void DEVICE_INDEX_MAP_TEST::test_set_get_bus_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_BUS);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 1);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 3);

    terminal = prepare_terminal(6);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(4);// set bus 4 twice is not allowed.
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 5);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==1);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==3);

    terminal = prepare_terminal(6);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(5);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==INDEX_NOT_EXIST);
}

void DEVICE_INDEX_MAP_TEST::test_set_get_generator_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_GENERATOR);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}

void DEVICE_INDEX_MAP_TEST::test_set_get_load_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_LOAD);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}

void DEVICE_INDEX_MAP_TEST::test_set_get_fixed_shunt_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_FIXED_SHUNT);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}

void DEVICE_INDEX_MAP_TEST::test_set_get_switched_shunt_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_SWITCHED_SHUNT);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}


void DEVICE_INDEX_MAP_TEST::test_set_get_line_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_LINE);

    terminal = prepare_terminal(1,2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2,4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3,6);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4,3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1,2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2,4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3,6);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4,3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}


void DEVICE_INDEX_MAP_TEST::test_set_get_transformer_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_TRANSFORMER);

    terminal = prepare_terminal(1,2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2,4,5);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3,4,6);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4,3,1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1,2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2,4,5);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3,4,6);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4,3,1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}


void DEVICE_INDEX_MAP_TEST::test_set_get_energy_storage_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_ENERGY_STORAGE);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);


    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map[device_id]==2);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map[device_id]==4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map[device_id]==6);
}

void DEVICE_INDEX_MAP_TEST::test_decrease_index_by_1_for_device_with_index_greater_than()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_GENERATOR);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    device_index_map.set_device_index(device_id, INDEX_NOT_EXIST);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    device_index_map.set_device_index(device_id, 4);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    device_index_map.set_device_index(device_id, 6);

    device_index_map.decrease_index_by_1_for_device_with_index_greater_than(3);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==0)

    terminal = prepare_terminal(2);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("1#"));
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==INDEX_NOT_EXIST)

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("4#"));
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==3)

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_id.set_device_identifier_index(get_index_of_string("2#"));
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==5)
}

void DEVICE_INDEX_MAP_TEST::test_empty()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    TEST_ASSERT(device_index_map.empty() == true);

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_BUS);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 0);

    TEST_ASSERT(device_index_map.empty() == false);
}

void DEVICE_INDEX_MAP_TEST::test_clear_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_BUS);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 1);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 3);

    terminal = prepare_terminal(6);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 2);

    device_index_map.clear();

    TEST_ASSERT(device_index_map.empty() == true);
}

void DEVICE_INDEX_MAP_TEST::test_get_index_of_device_and_operator_bracket()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    TERMINAL terminal;

    device_id.set_device_type(STEPS_BUS);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 0);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 1);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 3);

    terminal = prepare_terminal(6);
    device_id.set_device_terminal(terminal);
    device_index_map.set_device_index(device_id, 2);

    terminal = prepare_terminal(1);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==0);
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==0);

    terminal = prepare_terminal(3);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==1);
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==1);

    terminal = prepare_terminal(4);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==3);
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==3);

    terminal = prepare_terminal(6);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==2);
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==2);

    terminal = prepare_terminal(5);
    device_id.set_device_terminal(terminal);
    TEST_ASSERT(device_index_map[device_id]==INDEX_NOT_EXIST);
    TEST_ASSERT(device_index_map.get_index_of_device(device_id)==INDEX_NOT_EXIST);
}



void DEVICE_INDEX_MAP_TEST::test_index_performance_with_many_buses()
{
    show_test_information_for_function_of_class(__FUNCTION__,"DEVICE_INDEX_MAP_TEST");

    DEVICE_ID device_id;
    device_id.set_device_type(STEPS_TRANSFORMER);

    TERMINAL terminal;
    unsigned int MAX_TRANSFORMERS = 10000;

    unsigned int index;

    for(unsigned int i=1; i<=MAX_TRANSFORMERS; ++i)
    {
        terminal = prepare_terminal(i,i+1,i+2);
        device_id.set_device_terminal(terminal);
        device_index_map.set_device_index(device_id, i-1);
    }

    ostringstream osstream;
    osstream<<"now performing indexing";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    for(unsigned int i=1; i<=MAX_TRANSFORMERS; ++i)
    {
        terminal = prepare_terminal(i,i+1,i+2);
        device_id.set_device_terminal(terminal);
        index = device_index_map[device_id];
        TEST_ASSERT(index == i-1);
    }
}




#endif
