#ifndef ES_ELECTRICAL_MODEL_TEST_H
#define ES_ELECTRICAL_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "header/model/pvu_models/pvu_model_test.h"

#include "header/power_system_database.h"
using namespace std;

class PV_ELECTRICAL_MODEL_TEST : public PVU_MODEL_TEST
{
    public:
        PV_ELECTRICAL_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    protected:
        void test_get_model_type();
        void test_get_pv_unit_terminal_generation();
        void test_get_terminal_bus_voltage();
        void test_get_terminal_bus_frequency();
        void test_set_get_bus_to_regulate();

        void test_set_get_voltage_reference();
        void test_set_get_frequency_reference();
        void test_set_get_active_power_reference();
        void test_set_get_reactive_power_reference();
        void test_set_get_power_factor_reference();
        void test_set_get_var_control_mode();

        void test_get_standard_psse_string();

        void test_step_response_with_voltage_drop();
        void test_step_response_with_frequency_drop();

        virtual void test_get_model_name() = 0;
        virtual void test_set_get_parameters() = 0;
        virtual void test_initialize() = 0;
    private:
        void apply_voltage_drop_of_10_percent();
        void apply_frequency_drop_of_5_percent();

        void initialize_models();
        void run_to_time(double tend);

        void export_meter_title();
        void export_meter_values();
};

#endif//ES_ELECTRICAL_MODEL_TEST_H
