#ifndef VSC_HVDC_TEST_H
#define VSC_HVDC_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/vsc_hvdc.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class VSC_HVDC_TEST : public Test::Suite
{
    public:
        VSC_HVDC_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_identifier();
        void test_set_get_name();
        void test_set_get_converter_count();
        void test_set_get_dc_bus_count();
        void test_set_get_dc_line_count();
        void test_set_get_set_ac_converter_bus_with_dc_voltage_control();

        void test_set_get_converter_ac_bus();
        void test_set_get_converter_active_power_operation_mode();
        void test_set_get_converter_reactive_power_operation_mode();
        void test_set_get_converter_nominal_dc_power_command_in_MW();
        void test_set_get_converter_nominal_dc_voltage_command_in_kV();
        void test_set_get_converter_nominal_ac_voltage_command_in_pu();
        void test_set_get_converter_nominal_ac_power_factor_command();
        void test_set_get_converter_loss_factor_A_in_kW();
        void test_set_get_converter_loss_factor_B_in_kW_per_amp();
        void test_set_get_converter_transformer_impedance_in_pu();
        void test_set_get_converter_minimum_loss_in_kW();
        void test_set_get_converter_rated_capacity_in_MVA();
        void test_set_get_converter_rated_current_in_A();
        void test_set_get_converter_Qmax_in_MVar();
        void test_set_get_converter_Qmin_in_MVar();
        void test_set_get_converter_remote_bus_to_regulate();
        void test_set_get_converter_remote_regulation_percent();

        void test_set_get_dc_bus_number();
        void test_set_get_dc_bus_name();
        void test_set_get_dc_bus_area();
        void test_set_get_dc_bus_zone();
        void test_set_get_dc_bus_ac_bus_number();
        void test_set_get_dc_bus_owner_number();
        void test_set_get_dc_bus_ground_resistance_in_ohm();
        void test_set_get_dc_bus_generation_power_in_MW();
        void test_set_get_dc_bus_load_power_in_MW();

        void test_set_get_dc_sending_side_bus();
        void test_set_get_dc_receiving_side_bus();
        void test_set_get_meter_end_bus();
        void test_set_get_line_resistance_in_ohm();
        void test_set_get_line_inductance_in_mH();

        void test_get_device_id();
        void test_is_connected_to_bus();
        void test_is_valid();

        void test_build_inphno();
        void test_build_conductance_matrix();
        void test_build_jacobian();
        void test_initialize_Udc_vector();

        void test_initialize_Pdc_command();
        void test_update_bus_current();
        void test_get_P_mismatch_vector_with_5_terminal_data();
        void test_solve_dc_network_with_5_terminal_data();

        void test_set_get_vsc_hvdc_model();
    private:
        void prepare_2_terminal_vsc_hvdc();
        void prepare_3_terminal_vsc_hvdc();
        void prepare_4_terminal_vsc_hvdc();
        void prepare_5_terminal_vsc_hvdc();

        void prepare_2_terminal_vsc_hvdc_converter_ac_buses();
        void prepare_3_terminal_vsc_hvdc_converter_ac_buses();
        void prepare_4_terminal_vsc_hvdc_converter_ac_buses();
        void prepare_5_terminal_vsc_hvdc_converter_ac_buses();
        void prepare_7_terminal_vsc_hvdc_converter_ac_buses();

        VSC_HVDC vsc;
};

#endif //VSC_HVDC_TEST_H
