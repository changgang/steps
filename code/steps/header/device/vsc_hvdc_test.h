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

        void test_set_get_converter_bus();
        void test_set_get_converter_dc_operation_mode();
        void test_set_get_converter_ac_operation_mode();
        void test_set_get_converter_nominal_dc_power_command_in_MW();
        void test_set_get_converter_nominal_dc_voltage_command_in_kV();
        void test_set_get_converter_nominal_ac_voltage_command_in_pu();
        void test_set_get_converter_nominal_ac_power_factor_command();
        void test_set_get_converter_loss_factor_A_in_kW();
        void test_set_get_converter_loss_factor_B_in_kW_per_amp();
        void test_set_get_converter_minimum_loss_in_kW();
        void test_set_get_converter_rated_capacity_in_MVA();
        void test_set_get_converter_current_rating_in_amp();
        void test_set_get_converter_power_weighting_factor();
        void test_set_get_converter_Qmax_in_MVar();
        void test_set_get_converter_Qmin_in_MVar();
        void test_set_get_converter_remote_bus_to_regulate();
        void test_set_get_converter_remote_regulation_percent();

        void test_set_get_dc_bus_number();
        void test_set_get_dc_bus_name();
        void test_set_get_dc_bus_area();
        void test_set_get_dc_bus_zone();
        void test_set_get_ac_bus_number_of_dc_bus();
        void test_set_get_owner_number();
        void test_set_get_ground_resistance_in_ohm();
        void test_set_get_dc_bus_generation_power_in_MW();
        void test_set_get_dc_bus_load_power_in_MW();

        void test_set_get_dc_sending_side_bus();
        void test_set_get_dc_receiving_side_bus();
        void test_set_get_meter_end_bus();
        void test_set_get_line_resistance_in_ohm();
        void test_set_get_line_inductance_in_mH();

        void test_build_conductance_matrix();

        void test_set_get_vsc_hvdc_model();
    private:
        void prepare_2_terminal_vsc_hvdc_buses();
        void prepare_3_terminal_vsc_hvdc_buses();
        void prepare_4_terminal_vsc_hvdc_buses();
        void prepare_5_terminal_vsc_hvdc_buses();

        VSC_HVDC vsc;
};

#endif //VSC_HVDC_TEST_H
