#ifndef SOURCE_TEST_H
#define SOURCE_TEST_H

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

class SOURCE_TEST : public Test::Suite
{
    public:
        SOURCE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_set_get_source_bus();
        void test_set_get_identifier();
        void test_set_get_status();
        void test_set_get_mbase();
        void test_set_get_p_generation();
        void test_set_get_q_generation();
        void test_set_get_p_max_min();
        void test_set_get_q_max_min();
        void test_set_get_regulating_mode();
        void test_set_get_v_to_regulate();
        void test_set_get_bus_to_regulate();
        void test_set_get_source_impedance();

        void test_is_valid();
        void test_clear();
        void test_is_connected_to_bus();
    private:
        POWER_SYSTEM_DATABASE* db;
        GENERATOR* generator;
};

#endif //GENERATOR_AND_SOURCE_TEST_H
