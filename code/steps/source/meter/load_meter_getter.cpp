#include "header/meter/load_meter_getter.h"
#include "header/device/load.h"
#include "header/model/load_model/load_model.h"
#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include "header/model/load_relay_model/load_voltage_relay_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <complex>

double load_meter_getter::get_active_power_MW(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_dynamic_load_in_MVA().real();
}

double load_meter_getter::get_active_power_pu(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_dynamic_load_in_pu().real();
}

double load_meter_getter::get_reactive_power_MVar(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_dynamic_load_in_MVA().imag();
}

double load_meter_getter::get_reactive_power_pu(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_dynamic_load_in_pu().imag();
}

double load_meter_getter::get_current_kA(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    STEPS& toolkit = load->get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    double vbase = psdb.get_bus_base_voltage_in_kV(load->get_load_bus());
    double ibase = sbase/(SQRT3*vbase);
    return ibase * steps_fast_complex_abs(load->get_dynamics_load_current_in_pu_based_on_system_base_power());
}

double load_meter_getter::get_current_pu(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return steps_fast_complex_abs(load->get_dynamics_load_current_in_pu_based_on_system_base_power());
}

double load_meter_getter::get_load_model_internal_variable(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    LOAD_MODEL* model = load->get_load_model();
    return model->get_model_internal_variable_with_name(internal_var_name);
}

double load_meter_getter::get_frequency_relay_model_internal_variable(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    LOAD_FREQUENCY_RELAY_MODEL* model = load->get_load_frequency_relay_model();
    return model->get_model_internal_variable_with_name(internal_var_name);
}

double load_meter_getter::get_voltage_relay_model_internal_variable(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    LOAD_VOLTAGE_RELAY_MODEL* model = load->get_load_voltage_relay_model();
    return model->get_model_internal_variable_with_name(internal_var_name);
}

double load_meter_getter::get_total_scale_in_pu(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_load_total_scale_factor_in_pu();
}

double load_meter_getter::get_manually_scale_in_pu(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_load_manually_scale_factor_in_pu();
}

double load_meter_getter::get_relay_shed_scale_in_pu(const METER_PARAMETER& para)
{
    LOAD* load = (LOAD*) para.device_pointer;
    return load->get_load_relay_shed_scale_factor_in_pu();
}
