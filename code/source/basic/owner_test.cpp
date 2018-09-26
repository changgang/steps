#include "header/basic/owner_test.h"
#include "header/basic/utility.h"
#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

OWNER_TEST::OWNER_TEST()
{
    TEST_ADD(OWNER_TEST::test_constructor);
    TEST_ADD(OWNER_TEST::test_set_get_owner_number);
    TEST_ADD(OWNER_TEST::test_set_get_owner_name);
    TEST_ADD(OWNER_TEST::test_is_valid);
    TEST_ADD(OWNER_TEST::test_clear);
    TEST_ADD(OWNER_TEST::test_copy_with_operator_equal);
}

void OWNER_TEST::setup()
{
    ;
}

void OWNER_TEST::tear_down()
{
    owner.clear();

    show_test_end_information();
}

void OWNER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNER_TEST");

    TEST_ASSERT(owner.get_owner_number()==0);
    TEST_ASSERT(owner.get_owner_name()=="");
}

void OWNER_TEST::test_set_get_owner_number()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNER_TEST");

    TEST_ASSERT(owner.get_owner_number()==0);
    owner.set_owner_number(1);
    TEST_ASSERT(owner.get_owner_number()==1);
    owner.set_owner_number(2);
    TEST_ASSERT(owner.get_owner_number()==2);
    owner.set_owner_number(0);
    TEST_ASSERT(owner.get_owner_number()==0);
}

void OWNER_TEST::test_set_get_owner_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNER_TEST");

    TEST_ASSERT(owner.get_owner_name()=="");
    owner.set_owner_name("owner 1");
    TEST_ASSERT(owner.get_owner_name()=="owner 1");
}


void OWNER_TEST::test_is_valid()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNER_TEST");

    TEST_ASSERT(owner.is_valid()==false);
    owner.set_owner_number(1);
    TEST_ASSERT(owner.is_valid()==true);
}

void OWNER_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNER_TEST");

    owner.set_owner_number(1);
    owner.set_owner_name("owner 1");
    owner.clear();

    test_constructor();
}

void OWNER_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNER_TEST");

    owner.set_owner_number(1);
    owner.set_owner_name("owner 1");

    OWNER newowner = owner;

    TEST_ASSERT(newowner.get_owner_number()==1);
    TEST_ASSERT(newowner.get_owner_name()=="owner 1");
}
