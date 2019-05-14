#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "header/basic/fault_test.h"
#include "header/basic/utility.h"

using namespace std;

FAULT_TEST::FAULT_TEST()
{
    TEST_ADD(FAULT_TEST::test_constructor);
    TEST_ADD(FAULT_TEST::test_set_get_fault_type);
    TEST_ADD(FAULT_TEST::test_set_get_fault_shunt);
    TEST_ADD(FAULT_TEST::test_is_faulted);
    TEST_ADD(FAULT_TEST::test_clear);
    TEST_ADD(FAULT_TEST::test_copy_with_opeartor_equal);
}

void FAULT_TEST::setup()
{
    ;
}

void FAULT_TEST::tear_down()
{
    fault.clear();

    show_test_end_information();
}

void FAULT_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FAULT_TEST");
    TEST_ASSERT(fault.is_faulted()==false);
}

void FAULT_TEST::test_set_get_fault_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FAULT_TEST");
    fault.set_fault_type(SINGLE_PHASE_GROUNDED_FAULT);
    TEST_ASSERT(fault.get_fault_type()==SINGLE_PHASE_GROUNDED_FAULT);
    TEST_ASSERT(fault.get_fault_type_string()=="SINGLE PHASE GROUNDED FAULT");
    fault.set_fault_type(DOUBLE_PHASES_FAULT);
    TEST_ASSERT(fault.get_fault_type()==DOUBLE_PHASES_FAULT);
    TEST_ASSERT(fault.get_fault_type_string()=="DOUBLE PHASES FAULT");
    fault.set_fault_type(DOUBLE_PHASES_GROUNDED_FAULT);
    TEST_ASSERT(fault.get_fault_type()==DOUBLE_PHASES_GROUNDED_FAULT);
    TEST_ASSERT(fault.get_fault_type_string()=="DOUBLE PHASES GROUNDED FAULT");
    fault.set_fault_type(THREE_PHASES_FAULT);
    TEST_ASSERT(fault.get_fault_type()==THREE_PHASES_FAULT);
    TEST_ASSERT(fault.get_fault_type_string()=="THREE PHASES FAULT");
}
void FAULT_TEST::test_set_get_fault_shunt()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FAULT_TEST");
    complex<double> y = 0.0;

    y = complex<double>(0.2, -2e8);
    fault.set_fault_shunt_in_pu(y);
    TEST_ASSERT(fault.get_fault_shunt_in_pu()==y);

    y = complex<double>(0.0, 0.0);
    fault.set_fault_shunt_in_pu(y);
    TEST_ASSERT(fault.get_fault_shunt_in_pu()==0.0);
}

void FAULT_TEST::test_is_faulted()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FAULT_TEST");
    fault.set_fault_type(THREE_PHASES_FAULT);
    TEST_ASSERT(fault.is_faulted()==false);

    complex<double> y = 0.0;

    y = complex<double>(0.2, -2e10);
    fault.set_fault_shunt_in_pu(y);
    TEST_ASSERT(fault.is_faulted()==true);

    fault.set_fault_type(THREE_PHASES_FAULT);
    y = complex<double>(0.0, 0.0);
    fault.set_fault_shunt_in_pu(y);
    TEST_ASSERT(fault.is_faulted()==false);
}


void FAULT_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FAULT_TEST");
    fault.set_fault_type(SINGLE_PHASE_GROUNDED_FAULT);

    complex<double> y = 0.0;
    y = complex<double>(0.2, -2e10);
    fault.set_fault_shunt_in_pu(y);
    TEST_ASSERT(fault.is_faulted()==true);

    fault.clear();

    TEST_ASSERT(fault.get_fault_type()==SINGLE_PHASE_GROUNDED_FAULT);
    TEST_ASSERT(fault.get_fault_shunt_in_pu()==0.0);
    TEST_ASSERT(fault.is_faulted()==false);
}



void FAULT_TEST::test_copy_with_opeartor_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FAULT_TEST");
    fault.set_fault_type(THREE_PHASES_FAULT);
    complex<double> y = 0.0;
    y = complex<double>(0.2, -2e10);
    fault.set_fault_shunt_in_pu(y);

    FAULT newfault = fault;

    TEST_ASSERT(newfault.get_fault_type()==THREE_PHASES_FAULT);
    TEST_ASSERT(newfault.get_fault_shunt_in_pu()==y);
}

