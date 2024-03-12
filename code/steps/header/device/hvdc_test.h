#ifndef LCC_HVDC2T_TEST_H
#define LCC_HVDC2T_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/device/hvdc.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class LCC_HVDC2T_TEST : public Test::Suite
{
    public:
        LCC_HVDC2T_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();
        void test_set_get_converter_bus();
        void test_set_get_converter_valve_side_bus_name();
        void test_set_get_identifier();
        void test_set_get_name();
        void test_set_get_status();
        void test_is_blocked();
        void test_set_get_line_resistance();
        void test_set_get_line_inductance();
        void test_set_get_line_capacitance();
        void test_set_get_line_smoothing_inductance();
        void test_set_get_line_smoothing_resistance();
        void test_set_get_nominal_dc_power();
        void test_set_get_side_to_hold_power();
        void test_set_get_nominal_dc_current();
        void test_set_get_nominal_dc_voltage();
        void test_set_get_compensating_resistance_to_hold_voltage();
        void test_set_get_dc_voltage_threshold_for_mode_switching();
        void test_set_get_dc_current_power_margin();
        void test_set_get_meter_end();
        void test_set_get_converter_operation_mode();
        void test_set_get_converter_number_of_bridge();
        void test_set_get_converter_voltage_drop_per_bridge();
        void test_set_get_converter_firing_angle_and_limit();
        void test_set_get_converter_transformer_base_voltage();
        void test_set_get_converter_transformer_impedance();
        void test_set_get_converter_transformer_admittance();
        void test_set_get_converter_transformer_tap_and_limit();
        void test_set_get_converter_transformer_number_of_taps();

        void test_get_rectifier_dc_power_command();
        void test_get_rectifier_dc_current_command();
        void test_get_inverter_dc_voltage_command();

        void test_reverse_converters();

        void test_is_valid();
        void test_clear();
        void test_is_connected_to_bus();
        void test_copy_with_operator_equal();

        void test_get_device_id();

        void test_solve_hvdc();
        void test_set_get_dc_current();
        void test_set_get_converter_dc_current();
        void test_set_get_converter_dc_voltage();
        void test_get_converter_dc_power();
        void test_get_converter_ac_active_power();
        void test_get_converter_ac_reactive_power();
        void test_get_converter_ac_apparent_power();
        void test_get_converter_ac_power_factor();
        void test_get_converter_commutating_overlap_angle();

        void test_set_get_hvdc_model();

    private:
        void prepare_hvdc_buses();
        LCC_HVDC2T hvdc;
};

#endif //LCC_HVDC2T_TEST_H
