#ifndef IEESGO_TEST_H
#define IEESGO_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/sg_models/turbine_governor_model/IEESGO.h"
#include "header/model/sg_models/turbine_governor_model/turbine_governor_model_test.h"
#include "header/STEPS.h"
using namespace std;

class IEESGO_TEST : public TURBINE_GOVERNOR_MODEL_TEST
{
    public:
        IEESGO_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        virtual void test_get_model_type();
        virtual void test_get_model_name();
        virtual void test_set_get_parameters();
        virtual void test_initialize();
        virtual void test_set_get_mechanical_power_reference();
        virtual void test_set_get_mechanical_power_upper_limit();
        virtual void test_set_get_mechanical_power_lower_limit();
        virtual void test_step_response();
        virtual void test_get_standard_psse_string();
    private:
};

#endif//IEESGO_TEST_H
