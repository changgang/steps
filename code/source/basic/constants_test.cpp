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
    TEST_ASSERT(fabs(DOUBLE_PI-PI*2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(HALF_PI-0.5*PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(QUARTER_PI-0.25*PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(THIRD_PI-0.3333333333333*PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ONE_OVER_PI-1.0/PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(THREE_OVER_PI-3.0/PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ONE_OVER_DOUBLE_PI-1.0/(2.0*PI))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(PI_OVER_180-PI/180.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(ONE_EIGHTY_OVER_PI-180.0/PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(SQRT2-sqrt(2.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(SQRT3-sqrt(3.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(SQRT6-sqrt(6.0))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(THREE_SQRT2_OVER_PI-3.0*sqrt(2.0)/PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(PI_OVER_THREE_SQRT2-PI/(3.0*sqrt(2.0)))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(SQRT6_OVER_PI-sqrt(6.0)/PI)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(DOUBLE_EPSILON)<1e-9);
    TEST_ASSERT(fabs(FLOAT_EPSILON)<1e-5);
    TEST_ASSERT(fabs(INDEX_NOT_EXIST)>1e8);
    TEST_ASSERT(fabs(STEPS_MAX_TOOLKIT_SIZE)<INDEX_NOT_EXIST);
}

#endif
