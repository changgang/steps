#ifndef PV_UNIT_TEST_H
#define PV_UNIT_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/pv_unit.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class PV_UNIT_TEST : public Test::Suite
{
    public:
        PV_UNIT_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_unit_bus();
        void test_set_get_source_impedance();
        void test_copy_with_operator_equal();
        void test_get_device_id();

    private:
        PV_UNIT pv_unit;
};

#endif //PV_UNIT_TEST_H
