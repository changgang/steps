#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "cpptest.h"
#include "header/basic/test_macro.h"
#include "header/basic/inphno_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;


INPHNO_TEST::INPHNO_TEST()
{
    TEST_ADD(INPHNO_TEST::test_constructor);
    TEST_ADD(INPHNO_TEST::test_copy_constructor);
    TEST_ADD(INPHNO_TEST::test_set_get_physical_internal_bus_number_pair);
    TEST_ADD(INPHNO_TEST::test_update_with_new_internal_bus_permutation);
    TEST_ADD(INPHNO_TEST::test_is_table_full);
    TEST_ADD(INPHNO_TEST::test_clear);
    TEST_ADD(INPHNO_TEST::test_operator_equal);
}

void INPHNO_TEST::setup()
{
    ;
}

void INPHNO_TEST::tear_down()
{
    inphno.clear();

    show_test_end_information();
}

void INPHNO_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");
}

void INPHNO_TEST::test_copy_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    inphno.set_physical_internal_bus_number_pair(1,0);
    inphno.set_physical_internal_bus_number_pair(2,5);
    inphno.set_physical_internal_bus_number_pair(3,1);
    inphno.set_physical_internal_bus_number_pair(4,4);
    inphno.set_physical_internal_bus_number_pair(5,3);
    inphno.set_physical_internal_bus_number_pair(6,2);

    INPHNO newinphno = inphno;

    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(1)==0);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(2)==5);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(3)==1);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(4)==4);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(5)==3);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(6)==2);
}

void INPHNO_TEST::test_set_get_physical_internal_bus_number_pair()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    inphno.set_physical_internal_bus_number_pair(1,0);
    inphno.set_physical_internal_bus_number_pair(2,5);
    inphno.set_physical_internal_bus_number_pair(3,1);
    inphno.set_physical_internal_bus_number_pair(4,4);
    inphno.set_physical_internal_bus_number_pair(5,3);
    inphno.set_physical_internal_bus_number_pair(6,2);

    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(1)==0);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(2)==5);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(3)==1);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(4)==4);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(5)==3);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(6)==2);

    TEST_ASSERT(inphno.get_physical_bus_number_of_internal_bus_number(0)==1);
    TEST_ASSERT(inphno.get_physical_bus_number_of_internal_bus_number(1)==3);
    TEST_ASSERT(inphno.get_physical_bus_number_of_internal_bus_number(2)==6);
    TEST_ASSERT(inphno.get_physical_bus_number_of_internal_bus_number(3)==5);
    TEST_ASSERT(inphno.get_physical_bus_number_of_internal_bus_number(4)==4);
    TEST_ASSERT(inphno.get_physical_bus_number_of_internal_bus_number(5)==2);
}

void INPHNO_TEST::test_update_with_new_internal_bus_permutation()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    inphno.set_physical_internal_bus_number_pair(1,0);
    inphno.set_physical_internal_bus_number_pair(2,5);
    inphno.set_physical_internal_bus_number_pair(3,1);
    inphno.set_physical_internal_bus_number_pair(4,4);
    inphno.set_physical_internal_bus_number_pair(5,3);
    inphno.set_physical_internal_bus_number_pair(6,2);

    vector<unsigned int> P;
    //bus old new
    // 1   0   0
    // 2   5   1
    // 3   1   2
    // 4   4   3
    // 5   3   4
    // 6   2   5
    P.push_back(0);
    P.push_back(5);
    P.push_back(1);
    P.push_back(4);
    P.push_back(3);
    P.push_back(2);

    inphno.update_with_new_internal_bus_permutation(P);

    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(1)==0);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(2)==1);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(3)==2);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(4)==3);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(5)==4);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(6)==5);
}

void INPHNO_TEST::test_is_table_full()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    inphno.clear();
    inphno.set_physical_internal_bus_number_pair(1,0);
    inphno.set_physical_internal_bus_number_pair(2,1);
    inphno.set_physical_internal_bus_number_pair(3,2);
    inphno.set_physical_internal_bus_number_pair(4,3);
    inphno.set_physical_internal_bus_number_pair(5,4);
    inphno.set_physical_internal_bus_number_pair(6,5);
    inphno.set_physical_internal_bus_number_pair(7,6);
    inphno.set_physical_internal_bus_number_pair(9,8);

    TEST_ASSERT(inphno.is_table_full()==false);
    inphno.set_physical_internal_bus_number_pair(8,7);

    TEST_ASSERT(inphno.is_table_full()==true);
    inphno.clear();
}

void INPHNO_TEST::test_is_emtpy()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    TEST_ASSERT(inphno.empty()==true);
    inphno.set_physical_internal_bus_number_pair(1, 2);
    TEST_ASSERT(inphno.empty()==false);
    inphno.clear();
    TEST_ASSERT(inphno.empty()==true);
}

void INPHNO_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    inphno.set_physical_internal_bus_number_pair(1,0);
    inphno.set_physical_internal_bus_number_pair(2,5);
    inphno.set_physical_internal_bus_number_pair(3,1);
    inphno.set_physical_internal_bus_number_pair(4,4);
    inphno.set_physical_internal_bus_number_pair(5,3);
    inphno.set_physical_internal_bus_number_pair(6,2);
    inphno.set_physical_internal_bus_number_pair(7,2);
    inphno.set_physical_internal_bus_number_pair(8,2);
    inphno.set_physical_internal_bus_number_pair(9,2);

    inphno.clear();

    bool result;

    unsigned int nmax = 1000;
    unsigned int n = 1000;

    for(unsigned int i=1; i<=nmax; ++i)
    {
        result = (inphno.get_internal_bus_number_of_physical_bus_number(i) == INDEX_NOT_EXIST);
        TEST_ASSERT(result);
    }
    for(unsigned int i=1; i<=n; ++i)
    {
        result = (inphno.get_physical_bus_number_of_internal_bus_number(i-1) == INDEX_NOT_EXIST);
        TEST_ASSERT(result);
    }
}


void INPHNO_TEST::test_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"INPHNO_TEST");

    inphno.set_physical_internal_bus_number_pair(1,0);
    inphno.set_physical_internal_bus_number_pair(2,5);
    inphno.set_physical_internal_bus_number_pair(3,1);
    inphno.set_physical_internal_bus_number_pair(4,4);
    inphno.set_physical_internal_bus_number_pair(5,3);
    inphno.set_physical_internal_bus_number_pair(6,2);

    INPHNO newinphno;
    newinphno = inphno;

    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(1)==0);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(2)==5);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(3)==1);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(4)==4);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(5)==3);
    TEST_ASSERT(inphno.get_internal_bus_number_of_physical_bus_number(6)==2);
}

#endif
