#ifndef LCFB1_TEST_H
#define LCFB1_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model_test.h"

#include "header/model/sg_models/turbine_load_controller_model/lcfb1.h"
#include "header/power_system_database.h"
using namespace std;

class LCFB1_TEST : public TURBINE_LOAD_CONTROLLER_MODEL_TEST
{
    public:
        LCFB1_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_name();
        void test_set_get_parameters();
    private:
};

#endif//LCFB1_TEST_H
