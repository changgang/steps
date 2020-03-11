#ifndef MODEL_TEST_H
#define MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/model/model.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/model/sg_models/sync_generator_model/gencls.h"

using namespace std;

class MODEL_TEST : public Test::Suite
{
    public:
        MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_device_pointer_and_device_id();

        void test_set_get_flag_model_initialized();
        void test_set_get_flag_model_updated();

        void test_activate_and_deactive_model();
    private:
        GENCLS model;
};

#endif//MODEL_TEST_H
