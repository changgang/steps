#ifndef LCC_STATION_TEST_H
#define LCC_STATION_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/lcc_station.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class LCC_STATION_TEST : public Test::Suite
{
    public:
        LCC_STATION_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_number_of_converters();
        void test_set_get_total_power_percent_of_converter_station();
        void test_clear();
        void test_copy_with_operator_equal();
        void test_is_connected_to_bus();
        void test_is_in_area();
        void test_is_in_zone();
        void test_solve();
    private:
        LCC_STATION station;
};

#endif
