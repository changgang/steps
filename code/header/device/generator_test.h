#ifndef GENERATOR_TEST_H
#define GENERATOR_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/generator.h"
#include "header/power_system_database.h"

using namespace std;

class GENERATOR_TEST : public Test::Suite
{
    public:
        GENERATOR_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_set_get_generator_bus();
        void test_set_get_generator_impedance();

        void test_set_get_sync_generator_model();
        void test_set_get_compensator_model();
        void test_set_get_exciter_model();
        void test_set_get_stabilizer_model();
        void test_set_get_turbine_governor_model();

        void test_copy_with_operator_equal();

        void test_get_device_id();

    private:
        GENERATOR generator;
};

#endif //GENERATOR_TEST_H
