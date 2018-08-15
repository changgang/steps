#ifndef CCT_SEARCHER_TEST_H
#define CCT_SEARCHER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/toolkit/cct_searcher/cct_searcher.h"

using namespace std;

class CCT_SEARCHER_TEST : public Test::Suite
{
    public:
        CCT_SEARCHER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_and_get_power_system_database_maximum_bus_number();

        void test_set_get_search_title();
        void test_set_get_powerflow_data_filename();
        void test_set_get_dynamic_data_filename();
        void test_set_get_fault_device();
        void test_set_get_fault_side_bus();
        void test_set_get_fault_location_to_fault_bus();
        void test_set_get_fault_shunt();
        void test_set_get_fault_time();
        void test_set_get_minimum_clearint_time();
        void test_set_get_maximum_clearint_time();
        void test_set_get_flag_trip_line_after_clearing_fault();
        void test_set_get_simulation_time_span();
        void test_set_get_angle_difference_threshold();
        void test_set_get_simulator_max_iteration();
        void test_set_get_simulator_allowed_max_power_imbalance_in_MVA();
        void test_set_get_simulator_iteration_accelerator();

        void test_seach_cct_for_IEEE_9_bus_model_classic();
    private:
        void run_dynamics_with_cct(double cct);
        CCT_SEARCHER* searcher;
};

#endif//CCT_SEARCHER_TEST_H
