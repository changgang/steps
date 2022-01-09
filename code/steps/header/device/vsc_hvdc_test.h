#ifndef VSC_HVDC_TEST_H
#define VSC_HVDC_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/vsc_hvdc.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class VSC_HVDC_TEST : public Test::Suite
{
    public:
        VSC_HVDC_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_identifier();
        void test_set_get_name();

        void test_build_conductance_matrix();

        void test_set_get_vsc_hvdc_model();
    private:
        void prepare_2_terminal_vsc_hvdc_buses();
        void prepare_3_terminal_vsc_hvdc_buses();
        void prepare_4_terminal_vsc_hvdc_buses();

        VSC_HVDC vsc;
};

#endif //VSC_HVDC_TEST_H
