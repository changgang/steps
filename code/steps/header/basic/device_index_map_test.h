#ifndef DEVICE_INDEX_MAP_TEST_H
#define DEVICE_INDEX_MAP_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/device_index_map.h"
#include "header/STEPS.h"

using namespace std;

class DEVICE_INDEX_MAP_TEST : public Test::Suite
{
    public:
        DEVICE_INDEX_MAP_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_bus_index();
        void test_set_get_generator_index();
        void test_set_get_load_index();
        void test_set_get_fixed_shunt_index();
        void test_set_get_switched_shunt_index();
        void test_set_get_ac_line_index();
        void test_set_get_transformer_index();
        void test_set_get_energy_storage_index();

        void test_decrease_index_by_1_for_device_with_index_greater_than();

        void test_empty();
        void test_clear_index();
        void test_get_index_of_device_and_operator_bracket();

        void test_index_performance_with_many_buses();

        TERMINAL prepare_terminal(unsigned int bus);
        TERMINAL prepare_terminal(unsigned int ibus,unsigned int jbus);
        TERMINAL prepare_terminal(unsigned int ibus,unsigned int jbus, unsigned int kbus);
    private:
        DEVICE_INDEX_MAP device_index_map;

};

#endif //DEVICE_INDEX_MAP_TEST_H
