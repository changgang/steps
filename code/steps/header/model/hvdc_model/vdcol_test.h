#ifndef VDCOL_TEST_H
#define VDCOL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"
#include "header/power_system_database.h"
#include "header/model/hvdc_model/vdcol.h"
#include "header/STEPS.h"

using namespace std;

class VDCOL_TEST : public Test::Suite
{
    public:
        VDCOL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_constructor();
        void test_set_get_voltage_current_points();
        void test_get_maximum_current_limit();
        void test_copy_with_constructor();
        void test_copy_with_operator_equal();
    private:
        VDCOL limiter;

};

#endif//VDCOL_TEST_H
