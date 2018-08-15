#include "header/basic/terminal_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

TERMINAL_TEST::TERMINAL_TEST()
{
    TEST_ADD(TERMINAL_TEST::test_constructor);
    TEST_ADD(TERMINAL_TEST::test_append_and_get_bus);
    TEST_ADD(TERMINAL_TEST::test_has_bus);
    TEST_ADD(TERMINAL_TEST::test_clear);
    TEST_ADD(TERMINAL_TEST::test_copy_with_operator_equal);
    TEST_ADD(TERMINAL_TEST::test_operator_bracket);
}

void TERMINAL_TEST::setup()
{
    terminal = new TERMINAL;
}

void TERMINAL_TEST::tear_down()
{
    delete terminal;

    show_test_end_information();
}

void TERMINAL_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TERMINAL_TEST");

    TEST_ASSERT(terminal->get_bus_count()==0);
}
void TERMINAL_TEST::test_append_and_get_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TERMINAL_TEST");

    terminal->append_bus(1);
    terminal->append_bus(2);
    terminal->append_bus(0);
    terminal->append_bus(5);
    terminal->append_bus(3);
    terminal->append_bus(4);
    terminal->append_bus(4);
    terminal->append_bus(2);

    vector<size_t> buses = terminal->get_buses();
    TEST_ASSERT(terminal->get_bus_count()==5)
    TEST_ASSERT(buses.size()==5)
    TEST_ASSERT(buses[0]==1);
    TEST_ASSERT(buses[1]==2);
    TEST_ASSERT(buses[2]==3);
    TEST_ASSERT(buses[3]==4);
    TEST_ASSERT(buses[4]==5);
}

void TERMINAL_TEST::test_has_bus()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TERMINAL_TEST");

    terminal->append_bus(1);
    terminal->append_bus(2);
    terminal->append_bus(3);
    terminal->append_bus(4);
    terminal->append_bus(5);
    TEST_ASSERT(terminal->has_bus(1)==true);
    TEST_ASSERT(terminal->has_bus(2)==true);
    TEST_ASSERT(terminal->has_bus(3)==true);
    TEST_ASSERT(terminal->has_bus(4)==true);
    TEST_ASSERT(terminal->has_bus(5)==true);
    TEST_ASSERT(terminal->has_bus(6)==false);
    TEST_ASSERT(terminal->has_bus(0)==false);
    TEST_ASSERT(terminal->has_bus(-1)==false);
}

void TERMINAL_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TERMINAL_TEST");

    terminal->append_bus(1);
    terminal->append_bus(2);
    terminal->append_bus(3);
    terminal->append_bus(4);
    terminal->append_bus(5);
    terminal->clear();
    TEST_ASSERT(terminal->get_bus_count()==0);
}

void TERMINAL_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TERMINAL_TEST");

    terminal->append_bus(1);
    terminal->append_bus(2);
    terminal->append_bus(3);
    terminal->append_bus(4);
    terminal->append_bus(5);
    TERMINAL newterminal;
    newterminal = (*terminal);
    TEST_ASSERT(newterminal.get_bus_count()==5);
    TEST_ASSERT(newterminal.has_bus(1)==true);
    TEST_ASSERT(newterminal.has_bus(2)==true);
    TEST_ASSERT(newterminal.has_bus(3)==true);
    TEST_ASSERT(newterminal.has_bus(4)==true);
    TEST_ASSERT(newterminal.has_bus(5)==true);
}

void TERMINAL_TEST::test_operator_bracket()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TERMINAL_TEST");

    terminal->append_bus(1);
    terminal->append_bus(2);
    terminal->append_bus(3);
    terminal->append_bus(4);
    terminal->append_bus(5);
    TEST_ASSERT((*terminal)[0]==1);
    TEST_ASSERT((*terminal)[1]==2);
    TEST_ASSERT((*terminal)[2]==3);
    TEST_ASSERT((*terminal)[3]==4);
    TEST_ASSERT((*terminal)[4]==5);
}
