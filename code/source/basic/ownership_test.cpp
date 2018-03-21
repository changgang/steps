#include "header/basic/ownership_test.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

using namespace std;

OWNERSHIP_TEST::OWNERSHIP_TEST()
{
    TEST_ADD(OWNERSHIP_TEST::test_constructor);
    TEST_ADD(OWNERSHIP_TEST::test_append_and_get_ownership);
    TEST_ADD(OWNERSHIP_TEST::test_delete_owner_and_its_fraction);
    TEST_ADD(OWNERSHIP_TEST::test_normalize);
    TEST_ADD(OWNERSHIP_TEST::test_get_owner_count);
    TEST_ADD(OWNERSHIP_TEST::test_is_empty);
    TEST_ADD(OWNERSHIP_TEST::test_get_all_owners);
    TEST_ADD(OWNERSHIP_TEST::test_get_all_fraction);
    TEST_ADD(OWNERSHIP_TEST::test_get_fraction_of_owner);
    TEST_ADD(OWNERSHIP_TEST::test_get_owner_of_index);
    TEST_ADD(OWNERSHIP_TEST::test_get_fraction_of_index);
    TEST_ADD(OWNERSHIP_TEST::test_clear);
    TEST_ADD(OWNERSHIP_TEST::test_copy_with_operator_equal);
}

void OWNERSHIP_TEST::setup()
{
    ownership = new OWNERSHIP;
}

void OWNERSHIP_TEST::tear_down()
{
    delete ownership;

    show_test_end_information();
}

void OWNERSHIP_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    TEST_ASSERT(ownership->empty()==true);
}

void OWNERSHIP_TEST::test_append_and_get_ownership()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(1,0.5);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.0)<FLOAT_EPSILON);

    ownership->append_owner_and_its_fraction(2,0.2);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.0)<FLOAT_EPSILON);

    ownership->append_owner_and_its_fraction(1,0.2);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.0)<FLOAT_EPSILON);

    ownership->append_owner_and_its_fraction(3,0.05);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.05)<FLOAT_EPSILON);

    ownership->append_owner_and_its_fraction(3,0.1);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.15)<FLOAT_EPSILON);

    ownership->append_owner_and_its_fraction(4,0.1);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.15)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(4)-0.1)<FLOAT_EPSILON);
}

void OWNERSHIP_TEST::test_delete_owner_and_its_fraction()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(1,0.1);
    ownership->append_owner_and_its_fraction(2,0.1);
    ownership->append_owner_and_its_fraction(2,0.2);
    ownership->append_owner_and_its_fraction(3,0.06);

    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.3)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.06)<FLOAT_EPSILON);
    TEST_ASSERT(ownership->get_owner_count()==3);

    ownership->delete_owner_and_its_fraction(2);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.06)<FLOAT_EPSILON);
    TEST_ASSERT(ownership->get_owner_count()==2);

    ownership->delete_owner_and_its_fraction(1);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.06)<FLOAT_EPSILON);
    TEST_ASSERT(ownership->get_owner_count()==1);

    ownership->delete_owner_and_its_fraction(4);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.06)<FLOAT_EPSILON);
    TEST_ASSERT(ownership->get_owner_count()==1);

    ownership->delete_owner_and_its_fraction(3);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(ownership->get_owner_count()==0);
}

void OWNERSHIP_TEST::test_normalize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(1,0.5);
    ownership->append_owner_and_its_fraction(2,0.4);
    ownership->append_owner_and_its_fraction(3,0.5);

    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.5)<FLOAT_EPSILON);

    ownership->normalize();

    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.5/1.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.4/1.4)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(3)-0.5/1.4)<FLOAT_EPSILON);
}

void OWNERSHIP_TEST::test_get_owner_count()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    TEST_ASSERT(ownership->get_owner_count()==0);

    ownership->append_owner_and_its_fraction(1,0.1);
    TEST_ASSERT(ownership->get_owner_count()==1);

    ownership->append_owner_and_its_fraction(2,0.1);
    TEST_ASSERT(ownership->get_owner_count()==2);

    ownership->append_owner_and_its_fraction(2,0.2);
    TEST_ASSERT(ownership->get_owner_count()==2);

    ownership->append_owner_and_its_fraction(3,0.06);
    TEST_ASSERT(ownership->get_owner_count()==3);

    ownership->delete_owner_and_its_fraction(3);
    TEST_ASSERT(ownership->get_owner_count()==2);

    ownership->delete_owner_and_its_fraction(2);
    TEST_ASSERT(ownership->get_owner_count()==1);

    ownership->delete_owner_and_its_fraction(4);
    TEST_ASSERT(ownership->get_owner_count()==1);

    ownership->delete_owner_and_its_fraction(1);
    TEST_ASSERT(ownership->get_owner_count()==0);
}

void OWNERSHIP_TEST::test_is_empty()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    TEST_ASSERT(ownership->empty()==true);
    ownership->append_owner_and_its_fraction(1,0.1);
    TEST_ASSERT(ownership->empty()==false);
    ownership->append_owner_and_its_fraction(2,0.1);
    TEST_ASSERT(ownership->empty()==false);

    ownership->delete_owner_and_its_fraction(2);
    TEST_ASSERT(ownership->empty()==false);

    ownership->delete_owner_and_its_fraction(1);
    TEST_ASSERT(ownership->empty()==true);
}

void OWNERSHIP_TEST::test_get_all_owners()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    vector<size_t> owners;
    owners = ownership->get_all_owners();
    TEST_ASSERT(owners.size()==0);

    ownership->append_owner_and_its_fraction(2,0.1);
    owners = ownership->get_all_owners();
    TEST_ASSERT(owners.size()==1);
    TEST_ASSERT(owners[0]==2);

    ownership->append_owner_and_its_fraction(1,0.1);
    owners = ownership->get_all_owners();
    TEST_ASSERT(owners.size()==2);
    TEST_ASSERT(owners[0]==1);
    TEST_ASSERT(owners[1]==2);

    ownership->delete_owner_and_its_fraction(2);
    owners = ownership->get_all_owners();
    TEST_ASSERT(owners.size()==1);
    TEST_ASSERT(owners[0]==1);

    ownership->delete_owner_and_its_fraction(1);
    owners = ownership->get_all_owners();
    TEST_ASSERT(owners.size()==0);
}

void OWNERSHIP_TEST::test_get_all_fraction()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    vector<size_t> owners;
    vector<double> fraction;

    fraction = ownership->get_all_fraction();
    TEST_ASSERT(fraction.size()==0);

    ownership->append_owner_and_its_fraction(2,0.1);
    fraction = ownership->get_all_fraction();
    TEST_ASSERT(fraction.size()==1);
    TEST_ASSERT(fabs(fraction[0]-0.1)<FLOAT_EPSILON);

    ownership->append_owner_and_its_fraction(1,0.2);
    fraction = ownership->get_all_fraction();
    TEST_ASSERT(fraction.size()==2);
    TEST_ASSERT(fabs(fraction[0]-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(fraction[1]-0.1)<FLOAT_EPSILON);

    ownership->delete_owner_and_its_fraction(2);
    fraction = ownership->get_all_fraction();
    TEST_ASSERT(fraction.size()==1);
    TEST_ASSERT(fabs(fraction[0]-0.2)<FLOAT_EPSILON);

    ownership->delete_owner_and_its_fraction(1);
    fraction = ownership->get_all_fraction();
    TEST_ASSERT(fraction.size()==0);
}
void OWNERSHIP_TEST::test_get_fraction_of_owner()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(2,0.1);
    ownership->append_owner_and_its_fraction(1,0.2);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(1)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner(2)-0.1)<FLOAT_EPSILON);
}

void OWNERSHIP_TEST::test_get_owner_of_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(2,0.1);
    ownership->append_owner_and_its_fraction(1,0.2);
    TEST_ASSERT(ownership->get_owner_of_index(0)==1);
    TEST_ASSERT(ownership->get_owner_of_index(1)==2);
    TEST_ASSERT(ownership->get_owner_of_index(2)==0);
}
void OWNERSHIP_TEST::test_get_fraction_of_index()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(2,0.1);
    ownership->append_owner_and_its_fraction(1,0.2);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner_of_index(0)-0.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner_of_index(1)-0.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ownership->get_fraction_of_owner_of_index(2)-0.0)<FLOAT_EPSILON);
}
void OWNERSHIP_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(2,0.1);
    ownership->append_owner_and_its_fraction(1,0.2);
    ownership->clear();

    TEST_ASSERT(ownership->empty()==true);
}

void OWNERSHIP_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"OWNERSHIP_TEST");

    ownership->append_owner_and_its_fraction(2,0.1);
    ownership->append_owner_and_its_fraction(1,0.23);

    OWNERSHIP new_ownership = *(ownership);
    TEST_ASSERT(new_ownership.get_owner_count()==2);
    TEST_ASSERT(fabs(new_ownership.get_fraction_of_owner(1)-0.23)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(new_ownership.get_fraction_of_owner(2)-0.1)<FLOAT_EPSILON);
}
