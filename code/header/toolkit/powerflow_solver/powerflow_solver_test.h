#ifndef POWERFLOW_SOLVER_TEST_H
#define POWERFLOW_SOLVER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class POWERFLOW_SOLVER_TEST : public Test::Suite
{
    public:
        POWERFLOW_SOLVER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

        void test_set_get_max_iteration();
        void test_set_get_allowed_max_active_power_imbalance_in_MW();
        void test_set_get_allowed_max_reactive_power_imbalance_in_MVar();
        void test_set_get_flat_start_logic();
        void test_set_get_transformer_tap_adjustment_logic();
        void test_set_get_non_divergent_solution_logic();
        void test_set_get_export_jacobian_matrix_step_by_step_logic();

        void check_Arthur_R_Bergen_3_bus_model_powerflow_result();
        void check_IEEE_9_bus_model_powerflow_result();
        void check_ISO_New_England_39_bus_model_powerflow_result();
        void check_NPCC_140_bus_model_powerflow_result();

        void test_solve_Arthur_R_Bergen_3_bus_model_with_full_Newton_Raphson_solution();
        void test_solve_IEEE_9_bus_model_with_full_Newton_Raphson_solution();
        void test_solve_ISO_New_England_39_bus_model_with_full_Newton_Raphson_solution();
        void test_solve_NPCC_140_bus_model_with_full_Newton_Raphson_solution();
        void test_solve_Shandong_2000_bus_model_with_full_Newton_Raphson_solution();

        void test_solve_Arthur_R_Bergen_3_bus_model_with_fast_decoupled_solution();
        void test_solve_IEEE_9_bus_model_with_fast_decoupled_solution();
        void test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_solution();
        void test_solve_NPCC_140_bus_model_with_fast_decoupled_solution();

        void test_solve_Shandong_2000_bus_model_with_fast_decoupled_solution();
        void test_solve_Shandong_2000_bus_model_with_HVDC_with_fast_decoupled_solution();

        void test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution();
        void test_solve_NPCC_140_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution();
        void test_solve_ISO_New_England_39_bus_model_and_Shandong_2000_bus_model_with_fast_decoupled_solution();


        void test_solve_Shandong_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution();
        void test_solve_Northwest_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution();
        void test_solve_Yunnan_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution();

        void test_solve_IEEE_9_bus_model_with_WTG_with_full_Newton_Raphson_solution();
        void test_solve_IEEE_39_bus_model_with_WTG_with_full_Newton_Raphson_solution();
    private:
};

#endif
