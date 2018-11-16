#ifndef SG_MODEL_TEST_H
#define SG_MODEL_TEST_H

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

class SG_MODEL_TEST : public Test::Suite
{
    public:
        SG_MODEL_TEST();
        GENERATOR* get_test_generator();
        SYNC_GENERATOR_MODEL* get_test_sync_generator_model();
        EXCITER_MODEL* get_test_exciter_model();
        STABILIZER_MODEL* get_test_stabilizer_model();
        COMPENSATOR_MODEL* get_test_compensator_model();
        TURBINE_GOVERNOR_MODEL* get_test_turbine_governor_model();
        TURBINE_LOAD_CONTROLLER_MODEL* get_test_turbine_load_controller_model();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
};

#endif//SG_MODEL_TEST_H
