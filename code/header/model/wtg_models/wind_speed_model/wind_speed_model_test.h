#ifndef WIND_SPEED_MODEL_TEST_H
#define WIND_SPEED_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/wtg_models/wtg_model_test.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class WIND_SPEED_MODEL_TEST : public WTG_MODEL_TEST
{
    public:
        WIND_SPEED_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_get_nominal_wind_speed();

        void test_get_standard_psse_string();

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize() = 0;
        virtual void test_get_wind_speed() = 0;
        virtual void test_get_wind_direction() = 0;
    private:
};

#endif//WIND_SPEED_MODEL_TEST_H
