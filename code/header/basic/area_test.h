#ifndef AREA_TEST_H
#define AREA_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/area.h"
#include "header/power_system_database.h"
using namespace std;

class AREA_TEST : public Test::Suite
{
    public:
        AREA_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_area_number();
        void test_set_get_area_name();
        void test_set_get_area_swing_bus();
        void test_set_get_expected_power_leaving_area_in_MW();
        void test_set_get_area_power_mismatch_tolerance_in_MW();
        void test_is_valid();
        void test_clear();
        void test_copy_with_operator_equal();
    private:
        POWER_SYSTEM_DATABASE* db;
        AREA* area;
};
#endif // AREA_TEST_H
