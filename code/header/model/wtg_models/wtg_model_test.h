#ifndef WTG_MODEL_TEST_H
#define WTG_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/power_system_database.h"

using namespace std;

class WTG_MODEL_TEST : public Test::Suite
{
    public:
        WTG_MODEL_TEST();
        WT_GENERATOR* get_test_wt_generator();
        WT_GENERATOR_MODEL* get_test_wt_generator_model();
        WT_AERODYNAMIC_MODEL* get_test_wt_aerodynamic_model();
        WT_ELECTRICAL_MODEL* get_test_wt_electrical_model();
        WT_TURBINE_MODEL* get_test_wt_turbine_model();
        WT_PITCH_MODEL* get_test_wt_pitch_model();
        WIND_SPEED_MODEL* get_test_wind_speed_model();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
};

#endif//WTG_MODEL_TEST_H
