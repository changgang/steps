#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "header/basic/test_macro.h"

#include "header/basic/constants_test.h"
#include "header/basic/utility.h"

#ifdef ENABLE_STEPS_TEST
using namespace std;
CONSTANTS_TEST::CONSTANTS_TEST()
{
    TEST_ADD(CONSTANTS_TEST::test_constants);
}

void CONSTANTS_TEST::setup()
{
    ;
}

void CONSTANTS_TEST::tear_down()
{
    show_test_end_information();
}

void CONSTANTS_TEST::test_constants()
{
    show_test_information_for_function_of_class(__FUNCTION__,"CONSTANTS_TEST");
    TEST_ASSERT(fabs(PI-3.141592653589793)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(PI2-PI*2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(HALF_PI-0.5*PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(QUARTER_PI-0.25*PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(THIRD_PI-0.3333333333333*PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(FLOAT_EPSILON)<1e-8);
    TEST_ASSERT(fabs(INDEX_NOT_EXIST)>1e8);
    TEST_ASSERT(fabs(MAX_TOOLKIT_SIZE)<INDEX_NOT_EXIST);
}

#endif
