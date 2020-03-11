#include "header/basic/test_macro.h"
#include "header/basic/time_series_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

#ifdef ENABLE_STEPS_TEST
using namespace std;

TIME_SERIES_TEST::TIME_SERIES_TEST()
{
    TEST_ADD(TIME_SERIES_TEST::test_get_values_at_time);
    TEST_ADD(TIME_SERIES_TEST::test_get_value_of_name_at_time);
    TEST_ADD(TIME_SERIES_TEST::test_copy_with_operator_equal);
}

void TIME_SERIES_TEST::setup()
{
    ts.load_time_series_from_file("../../../bench/time_series.csv");
}

void TIME_SERIES_TEST::tear_down()
{
    ts.clear();
    show_test_end_information();
}

void TIME_SERIES_TEST::test_get_values_at_time()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIME_SERIES_TEST");

    vector<double> v;
    v = ts.get_values_at_time(0.0);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = ts.get_values_at_time(-0.01);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = ts.get_values_at_time(-0.03);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = ts.get_values_at_time(0.05);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = ts.get_values_at_time(0.5);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.9)<FLOAT_EPSILON);

    v = ts.get_values_at_time(0.6);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.9)<FLOAT_EPSILON);

    v = ts.get_values_at_time(0.15);
    TEST_ASSERT(fabs(v[0]-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.05)<FLOAT_EPSILON);

    v = ts.get_values_at_time(0.3);
    TEST_ASSERT(fabs(v[0]-(1.1-0.1/0.3*0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-(2.1+0.1/0.3*0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-(10.1+0.4/0.3*0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-(12.1+0.8/0.3*0.1))<FLOAT_EPSILON);
}

void TIME_SERIES_TEST::test_get_value_of_name_at_time()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIME_SERIES_TEST");

    double v;

    v = ts.get_value_of_name_at_time("signal 1", 0.6);
    TEST_ASSERT(fabs(v-1.0)<FLOAT_EPSILON);
    v = ts.get_value_of_name_at_time("signal 2", 0.6);
    TEST_ASSERT(fabs(v-2.2)<FLOAT_EPSILON);
    v = ts.get_value_of_name_at_time("signal x", 0.6);
    TEST_ASSERT(fabs(v-10.5)<FLOAT_EPSILON);
    v = ts.get_value_of_name_at_time("signal z", 0.6);
    TEST_ASSERT(fabs(v-12.9)<FLOAT_EPSILON);

    v = ts.get_value_of_name_at_time("signal 1", 0.3);
    TEST_ASSERT(fabs(v-(1.1-0.1/0.3*0.1))<FLOAT_EPSILON);
    v = ts.get_value_of_name_at_time("signal 2", 0.3);
    TEST_ASSERT(fabs(v-(2.1+0.1/0.3*0.1))<FLOAT_EPSILON);
    v = ts.get_value_of_name_at_time("signal x", 0.3);
    TEST_ASSERT(fabs(v-(10.1+0.4/0.3*0.1))<FLOAT_EPSILON);
    v = ts.get_value_of_name_at_time("signal z", 0.3);
    TEST_ASSERT(fabs(v-(12.1+0.8/0.3*0.1))<FLOAT_EPSILON);
}

void TIME_SERIES_TEST::test_copy_with_operator_equal()
{
    show_test_information_for_function_of_class(__FUNCTION__,"TIME_SERIES_TEST");

    TIME_SERIES new_ts = ts;

    vector<double> v;
    v = new_ts.get_values_at_time(0.0);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(-0.01);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(-0.03);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(0.05);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.0)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(0.5);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.9)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(0.6);
    TEST_ASSERT(fabs(v[0]-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.2)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.5)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.9)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(0.15);
    TEST_ASSERT(fabs(v[0]-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-2.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-10.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-12.05)<FLOAT_EPSILON);

    v = new_ts.get_values_at_time(0.3);
    TEST_ASSERT(fabs(v[0]-(1.1-0.1/0.3*0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[1]-(2.1+0.1/0.3*0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[2]-(10.1+0.4/0.3*0.1))<FLOAT_EPSILON);
    TEST_ASSERT(fabs(v[3]-(12.1+0.8/0.3*0.1))<FLOAT_EPSILON);
}

#endif
