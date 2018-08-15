#ifndef VIRTUAL_GENERATOR_LOAD_PAIR_TEST_H
#define VIRTUAL_GENERATOR_LOAD_PAIR_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/device/virtual_generator_load_pair.h"
#include "header/power_system_database.h"

using namespace std;

class VIRTUAL_GENERATOR_LOAD_PAIR_TEST : public Test::Suite
{
    public:
        VIRTUAL_GENERATOR_LOAD_PAIR_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_generator_power_system_name();
        void test_set_get_load_power_system_name();
        void test_set_get_generator_power_system_index();
        void test_set_get_load_power_system_index();

        void test_set_get_virtual_generator();
        void test_set_get_virtual_load();

        void test_update();

        void test_is_valid();
        void test_clear();
        void test_is_connected_to_bus();


        void test_get_device_id();

        /*void test_initialize();

        void test_run();

        void test_clear_all_models();*/

        void build_pair();
    private:
        POWER_SYSTEM_DATABASE* db;
        VIRTUAL_GENERATOR_LOAD_PAIR* virtual_gen_load_pair;
};

#endif //VIRTUAL_GENERATOR_LOAD_PAIR_TEST_H
