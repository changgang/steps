#ifndef DYNAMICS_SIMULATOR_TEST_H
#define DYNAMICS_SIMULATOR_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"

using namespace std;

class DYNAMICS_SIMULATOR_TEST : public Test::Suite
{
    public:
        DYNAMICS_SIMULATOR_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_set_get_csv_file_export_enable_flag();
        void test_set_get_json_file_export_enable_flag();

        //void test_set_get_current_simulation_time();

        void test_set_get_max_DAE_iteration();
        void test_set_get_max_network_iteration();
        void test_set_get_max_update_event_iteration();
        void test_set_get_allowed_max_power_imbalance_in_MVA();
        void test_set_get_iteration_accelerator();
        void test_set_get_rotor_angle_stability_survilliance_flag();
        void test_set_get_rotor_angle_stability_threshold();

        void test_append_and_get_meter();
        void test_get_meter_count();
        void test_get_all_meters_value();
        void test_clear_meters();

        void test_set_get_output_file();

        void test_events();

        void test_start();

        void test_single_machine_model_GENCLS_IEEL();
        void test_single_machine_model_GENROU();
        void test_single_machine_model_IEEET1();
        void test_single_machine_model_IEEEG1();


        void test_run_IEEE_9_bus_model_classic_trip_bus();

        void test_run_IEEE_9_bus_model_classic();

        void test_run_IEEE_9_bus_model_classic_with_rotor_angle_survilliance();

        void test_run_IEEE_9_bus_model_complete();
        void test_run_IEEE_39_bus_model_GENROU();
        void test_run_IEEE_39_bus_model_GENSAL();
        void test_run_IEEE_39_bus_model_GENROU_IEEET1();
        void test_run_IEEE_39_bus_model_GENROU_IEEEG1();
        void test_run_IEEE_39_bus_model_GENROU_IEEET1_IEEEG1();
        void test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1();
        void test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_LCFB1();
        void test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_without_UFLS();
        void test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_UFLS();
        void test_run_IEEE_39_bus_model_GENROU_SEXS_IEEEG1_PUFLS();
        void test_get_admittance_matrix_from_IEEE_39_bus_model_GENROU();
        void test_run_bench_shandong_100_bus_model_with_dc_GENROU_CDC4T();

        void test_run_IEEE_9_bus_model_with_WT3_models();
        void test_run_IEEE_9_bus_model_with_all_WT3_models();

    private:
        void run_single_machine_model_for_model_test();
};

#endif//DYNAMICS_SIMULATOR_TEST_H
