#include "header/block/saturation_block_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/basic/constants.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

SATURATION_BLOCK_TEST::SATURATION_BLOCK_TEST()
{
    TEST_ADD(SATURATION_BLOCK_TEST::test_constructor);
    TEST_ADD(SATURATION_BLOCK_TEST::test_set_get_saturation_type);
    TEST_ADD(SATURATION_BLOCK_TEST::test_set_get_V1);
    TEST_ADD(SATURATION_BLOCK_TEST::test_set_get_V2);
    TEST_ADD(SATURATION_BLOCK_TEST::test_set_get_S1);
    TEST_ADD(SATURATION_BLOCK_TEST::test_set_get_S2);
    TEST_ADD(SATURATION_BLOCK_TEST::test_is_saturation_considered);
    TEST_ADD(SATURATION_BLOCK_TEST::test_get_saturation_quadratic);
    TEST_ADD(SATURATION_BLOCK_TEST::test_get_saturation_exponential);
}

void SATURATION_BLOCK_TEST::setup()
{
    block = new SATURATION_BLOCK;
}

void SATURATION_BLOCK_TEST::tear_down()
{
    delete block;

    show_test_end_information();
}

void SATURATION_BLOCK_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    TEST_ASSERT(block->get_saturation_type()==QUADRATIC_SATURATION_TYPE);
    TEST_ASSERT(fabs(block->get_S1()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(block->get_S2()-0.0)<FLOAT_EPSILON);
}

void SATURATION_BLOCK_TEST::test_set_get_saturation_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_saturation_type(QUADRATIC_SATURATION_TYPE);
    TEST_ASSERT(block->get_saturation_type()==QUADRATIC_SATURATION_TYPE);

    block->set_saturation_type(EXPONENTIAL_SATURATION_TYPE);
    TEST_ASSERT(block->get_saturation_type()==EXPONENTIAL_SATURATION_TYPE);
}

void SATURATION_BLOCK_TEST::test_set_get_V1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_V1(1.1);
    TEST_ASSERT(fabs(block->get_V1()-1.1)<FLOAT_EPSILON);
}

void SATURATION_BLOCK_TEST::test_set_get_V2()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_V2(1.2);
    TEST_ASSERT(fabs(block->get_V2()-1.2)<FLOAT_EPSILON);
}

void SATURATION_BLOCK_TEST::test_set_get_S1()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_S1(0.1);
    TEST_ASSERT(fabs(block->get_S1()-0.1)<FLOAT_EPSILON);
}

void SATURATION_BLOCK_TEST::test_set_get_S2()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_S2(0.2);
    TEST_ASSERT(fabs(block->get_S2()-0.2)<FLOAT_EPSILON);
}

void SATURATION_BLOCK_TEST::test_is_saturation_considered()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_S1(0.0);
    block->set_S2(0.0);

    TEST_ASSERT(block->is_saturation_considered()==false);

    block->set_S1(0.0);
    block->set_S2(0.1);

    TEST_ASSERT(block->is_saturation_considered()==true);

    block->set_S1(0.1);
    block->set_S2(0.0);

    TEST_ASSERT(block->is_saturation_considered()==true);

    block->set_S1(0.1);
    block->set_S2(0.1);

    TEST_ASSERT(block->is_saturation_considered()==true);
}

void SATURATION_BLOCK_TEST::test_get_saturation_quadratic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_saturation_type(QUADRATIC_SATURATION_TYPE);

    double A = 0.5, B=2.0;
    double V1 = 1.1, V2 = 1.5;
    double S1=B*(V1-A)*(V1-A)/V1, S2=B*(V2-A)*(V2-A)/V2;

    block->set_V1(V1);
    block->set_V2(V2);
    block->set_S1(S1);
    block->set_S2(S2);

    double V = 0.8;
    TEST_ASSERT(fabs(block->get_saturation(V)-(B*(V-A)*(V-A)/V))<FLOAT_EPSILON);

    V = 1.8;
    TEST_ASSERT(fabs(block->get_saturation(V)-(B*(V-A)*(V-A)/V))<FLOAT_EPSILON);
}

void SATURATION_BLOCK_TEST::test_get_saturation_exponential()
{
    show_test_information_for_function_of_class(__FUNCTION__,"SATURATION_BLOCK_TEST");

    block->set_saturation_type(EXPONENTIAL_SATURATION_TYPE);

    double A = 0.5, X=2.0;
    double V1 = 1.1, V2 = 1.5;
    double S1=A*pow(V1, X), S2=A*pow(V2, X);

    block->set_V1(V1);
    block->set_V2(V2);
    block->set_S1(S1);
    block->set_S2(S2);

    double V = 0.8;
    TEST_ASSERT(fabs(block->get_saturation(V)-(A*pow(V,X)))<FLOAT_EPSILON);

    V = 1.8;
    TEST_ASSERT(fabs(block->get_saturation(V)-(A*pow(V,X)))<FLOAT_EPSILON);
}
