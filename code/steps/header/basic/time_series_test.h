#ifndef TIME_SERIES_TEST_H
#define TIME_SERIES_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"
#include "header/basic/time_series.h"
using namespace std;

class TIME_SERIES_TEST : public Test::Suite
{
    public:
        TIME_SERIES_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_get_values_at_time();
        void test_get_value_of_name_at_time();
        void test_copy_with_operator_equal();
    private:
        TIME_SERIES ts;
};
#endif // TIME_SERIES_TEST_H
