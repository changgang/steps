#ifndef TURBINE_LOAD_CONTROLLER_TEST_H
#define TURBINE_LOAD_CONTROLLER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/sg_model_test.h"

#include "header/model/sg_models/turbine_load_controller_model/lcfb1.h"
#include "header/power_system_database.h"
using namespace std;

class TURBINE_LOAD_CONTROLLER_MODEL_TEST : public SG_MODEL_TEST
{
    public:
        TURBINE_LOAD_CONTROLLER_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_get_initial_mechanical_power_reference();
        void test_get_terminal_active_power();
        void test_get_mechanical_power_reference();
    private:
};

#endif//TURBINE_LOAD_CONTROLLER_TEST_H
