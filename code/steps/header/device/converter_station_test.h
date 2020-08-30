#ifndef CONVERTER_STATION_TEST_H
#define CONVERTER_STATION_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/converter_station.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

using namespace std;

class CONVERTER_STATION_TEST : public Test::Suite
{
    public:
        CONVERTER_STATION_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_name();
        void test_set_get_converter_side();
        void test_clear();
        void test_copy_with_operator_equal();
    private:
        CONVERTER_STATION station;
};

#endif
