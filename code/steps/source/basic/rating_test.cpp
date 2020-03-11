#include "header/basic/test_macro.h"
#include "header/basic/rating_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

RATING_TEST::RATING_TEST()
{
    TEST_ADD(RATING_TEST::test_constructor);
    TEST_ADD(RATING_TEST::test_set_get_rating_A_MVA);
    TEST_ADD(RATING_TEST::test_set_get_rating_B_MVA);
    TEST_ADD(RATING_TEST::test_set_get_rating_C_MVA);
    TEST_ADD(RATING_TEST::test_copy_with_operator_equal);
    TEST_ADD(RATING_TEST::test_operator_bracket);
    TEST_ADD(RATING_TEST::test_clear);
}

void RATING_TEST::setup()
{
    ;
}

void RATING_TEST::tear_down()
{
    rating.clear();

    show_test_end_information();
}

void RATING_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    TEST_ASSERT(fabs(rating.get_rating_A_MVA()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating.get_rating_B_MVA()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating.get_rating_C_MVA()-0.0)<FLOAT_EPSILON);
}

void RATING_TEST::test_set_get_rating_A_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    rating.set_rating_A_MVA(50.0);
    TEST_ASSERT(fabs(rating.get_rating_A_MVA()-50.0)<FLOAT_EPSILON);
}

void RATING_TEST::test_set_get_rating_B_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    rating.set_rating_B_MVA(50.0);
    TEST_ASSERT(fabs(rating.get_rating_B_MVA()-50.0)<FLOAT_EPSILON);
}

void RATING_TEST::test_set_get_rating_C_MVA()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    rating.set_rating_C_MVA(50.0);
    TEST_ASSERT(fabs(rating.get_rating_C_MVA()-50.0)<FLOAT_EPSILON);
}

void RATING_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    rating.set_rating_A_MVA(100.0);
    rating.set_rating_B_MVA(50.0);
    rating.set_rating_C_MVA(0.0);

    RATING rating2 = rating;
    TEST_ASSERT(fabs(rating2.get_rating_A_MVA()-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating2.get_rating_B_MVA()-50.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating2.get_rating_C_MVA()-0.0)<FLOAT_EPSILON);
}

void RATING_TEST::test_operator_bracket()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    rating.set_rating_A_MVA(100.0);
    rating.set_rating_B_MVA(50.0);
    rating.set_rating_C_MVA(0.0);

    TEST_ASSERT(fabs(rating['a']-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating[1]-100.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating['b']-50.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating[2]-50.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating['c']-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating[3]-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating['d']-0.0)<FLOAT_EPSILON);
}

void RATING_TEST::test_clear()
{
    show_test_information_for_function_of_class(__FUNCTION__,"RATING_TEST");

    rating.set_rating_A_MVA(100.0);
    rating.set_rating_B_MVA(50.0);
    rating.set_rating_C_MVA(0.0);

    rating.clear();

    TEST_ASSERT(fabs(rating.get_rating_A_MVA()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating.get_rating_B_MVA()-0.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(rating.get_rating_C_MVA()-0.0)<FLOAT_EPSILON);
}

#endif
