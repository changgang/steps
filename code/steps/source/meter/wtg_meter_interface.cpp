#include "header/meter/wtg_meter_interface.h"
#include "header/device/wt_generator.h"
#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model.h"
#include "header/model/wtg_models/wt_vrt_model/wt_vrt_model.h"
#include "header/model/wtg_models/wt_relay_model/wt_relay_model.h"
#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"
#include "header/model/wtg_models/wt_dbr_model/wt_dbr_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

double wtg_meter_interface::get_terminal_current_in_pu_mbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_terminal_current_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_terminal_current_in_pu_sbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_terminal_current_in_pu_based_on_mbase() * gen->get_mbase_over_sbase() : 0.0;
}

double wtg_meter_interface::get_terminal_current_in_kA(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if (model != NULL)
    {
        unsigned int bus = gen->get_generator_bus();
        double vbase = gen->get_toolkit().get_power_system_database().get_bus_base_voltage_in_kV(bus);
        double mbase = gen->get_mbase_in_MVA();
        double ibase = mbase / (SQRT3 * vbase);

        return model->get_terminal_current_in_pu_based_on_mbase() * ibase;
    }
    else
        return 0.0;
}

double wtg_meter_interface::get_terminal_active_power_in_pu_mbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_terminal_active_power_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_terminal_active_power_in_pu_sbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if (!model) return 0.0;

    double one_over_sbase = gen->get_toolkit().get_one_over_system_base_power_in_one_over_MVA();
    return model->get_terminal_active_power_in_MW() * one_over_sbase;
}

double wtg_meter_interface::get_terminal_active_power_in_MW(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_terminal_active_power_in_MW() : 0.0;
}

double wtg_meter_interface::get_terminal_reactive_power_in_pu_mbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_terminal_reactive_power_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if (!model) return 0.0;

    STEPS& toolkit = gen->get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
}

double wtg_meter_interface::get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_terminal_reactive_power_in_MVar() : 0.0;
}

double wtg_meter_interface::get_terminal_apparent_power_in_pu_mbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if (!model) return 0.0;

    double p = model->get_terminal_active_power_in_pu_based_on_mbase();
    double q = model->get_terminal_reactive_power_in_pu_based_on_mbase();
    return steps_sqrt(p * p + q * q);
}

double wtg_meter_interface::get_terminal_apparent_power_in_pu_sbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if (!model) return 0.0;

    STEPS& toolkit = gen->get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double p = model->get_terminal_active_power_in_MW() * one_over_sbase;
    double q = model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
    return steps_sqrt(p * p + q * q);
}

double wtg_meter_interface::get_terminal_apparent_power_in_MVA(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if (!model) return 0.0;

    double p = model->get_terminal_active_power_in_MW();
    double q = model->get_terminal_reactive_power_in_MVar();
    return steps_sqrt(p * p + q * q);
}

double wtg_meter_interface::get_mechanical_power_in_pu_mbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    if (!model) return 0.0;

    double one_over_mbase = gen->get_one_over_mbase_in_one_over_MVA();
    return model->get_turbine_mechanical_power_in_MW() * one_over_mbase;
}

double wtg_meter_interface::get_mechanical_power_in_pu_sbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    if (!model) return 0.0;

    STEPS& toolkit = gen->get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    return model->get_turbine_mechanical_power_in_MW() * one_over_sbase;
}

double wtg_meter_interface::get_mechanical_power_in_MW(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    return model ? model->get_turbine_mechanical_power_in_MW() : 0.0;
}

double wtg_meter_interface::get_max_available_mechanical_power_in_pu_mbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    if (!model) return 0.0;

    double vwind = model->get_wind_speed_in_mps();
    double pmax = model->get_maximum_available_mechanical_power_per_wt_generator_in_MW_considering_gear_efficiency(vwind);
    unsigned int n = gen->get_number_of_lumped_wt_generators();
    double one_over_mbase = gen->get_one_over_mbase_in_one_over_MVA();

    return pmax * n * one_over_mbase;
}

double wtg_meter_interface::get_max_available_mechanical_power_in_pu_sbase(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    if (!model) return 0.0;

    double vwind = model->get_wind_speed_in_mps();
    double pmax = model->get_maximum_available_mechanical_power_per_wt_generator_in_MW_considering_gear_efficiency(vwind);
    unsigned int n = gen->get_number_of_lumped_wt_generators();

    STEPS& toolkit = gen->get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return pmax * n * one_over_sbase;
}

double wtg_meter_interface::get_max_available_mechanical_power_in_MW(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    if (!model) return 0.0;

    double vwind = model->get_wind_speed_in_mps();
    double pmax = model->get_maximum_available_mechanical_power_per_wt_generator_in_MW_considering_gear_efficiency(vwind);
    unsigned int n = gen->get_number_of_lumped_wt_generators();

    return pmax * n;
}

double wtg_meter_interface::get_speed_reference_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    return model ? model->get_turbine_reference_speed_in_pu() : 0.0;
}

double wtg_meter_interface::get_speed_reference_in_rad_s(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    return model ? model->get_turbine_reference_speed_in_rad_per_s() : 0.0;
}

double wtg_meter_interface::get_turbine_speed_deviation_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_turbine_speed_in_pu() - 1.0 : 0.0;
}

double wtg_meter_interface::get_turbine_speed_deviation_in_Hz(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* turbine_model = gen->get_wt_turbine_model();
    WT_AERODYNAMIC_MODEL* aerd_model = gen->get_wt_aerodynamic_model();

    if (!turbine_model || !aerd_model) return 0.0;

    double fn = aerd_model->get_nominal_turbine_speed_in_rad_per_s() * ONE_OVER_DOUBLE_PI;
    return (turbine_model->get_turbine_speed_in_pu() - 1.0) * fn;
}

double wtg_meter_interface::get_turbine_speed_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_turbine_speed_in_pu() : 0.0;
}

double wtg_meter_interface::get_turbine_speed_in_Hz(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* turbine_model = gen->get_wt_turbine_model();
    WT_AERODYNAMIC_MODEL* aerd_model = gen->get_wt_aerodynamic_model();

    if (!turbine_model || !aerd_model) return 0.0;

    double wn = aerd_model->get_nominal_turbine_speed_in_rad_per_s() * ONE_OVER_DOUBLE_PI;
    return turbine_model->get_turbine_speed_in_pu() * wn;
}

double wtg_meter_interface::get_rotor_speed_deviation_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_generator_speed_in_pu() - 1.0 : 0.0;
}

double wtg_meter_interface::get_rotor_speed_deviation_in_Hz(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* turbine_model = gen->get_wt_turbine_model();
    WT_AERODYNAMIC_MODEL* aerd_model = gen->get_wt_aerodynamic_model();

    if (!turbine_model || !aerd_model) return 0.0;

    STEPS& toolkit = gen->get_toolkit();
    unsigned int bus = gen->get_generator_bus();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double fbase = psdb.get_bus_base_frequency_in_Hz(bus);
    double fn = fbase / aerd_model->get_number_of_pole_pairs();

    return (turbine_model->get_generator_speed_in_pu() - 1.0) * fn;
}

double wtg_meter_interface::get_rotor_speed_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_generator_speed_in_pu() : 0.0;
}

double wtg_meter_interface::get_rotor_speed_in_Hz(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* turbine_model = gen->get_wt_turbine_model();
    WT_AERODYNAMIC_MODEL* aerd_model = gen->get_wt_aerodynamic_model();

    if (!turbine_model || !aerd_model) return 0.0;

    STEPS& toolkit = gen->get_toolkit();
    unsigned int bus = gen->get_generator_bus();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double fbase = psdb.get_bus_base_frequency_in_Hz(bus);
    double fn = fbase / aerd_model->get_number_of_pole_pairs();

    return turbine_model->get_generator_speed_in_pu() * fn;
}

double wtg_meter_interface::get_rotor_angle_in_deg(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_rotor_angle_in_deg() : 0.0;
}

double wtg_meter_interface::get_rotor_angle_in_rad(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_rotor_angle_in_rad() : 0.0;
}

double wtg_meter_interface::get_active_current_command_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_active_current_command_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_reactive_current_command_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_reactive_current_command_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_active_power_command_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_active_power_command_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_reactive_power_command_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_reactive_power_command_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_reactive_voltage_command_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_reactive_voltage_command_in_pu() : 0.0;
}

double wtg_meter_interface::get_pitch_angle_in_deg(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    return model ? model->get_pitch_angle_in_deg() : 0.0;
}

double wtg_meter_interface::get_wind_speed_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WIND_SPEED_MODEL* windspeed_model = gen->get_wind_speed_model();
    WT_AERODYNAMIC_MODEL* aerd_model = gen->get_wt_aerodynamic_model();

    if (windspeed_model)
        return windspeed_model->get_wind_speed_in_pu();
    else if (aerd_model)
        return aerd_model->get_wind_speed_in_mps() / aerd_model->get_nominal_wind_speed_in_mps();
    else
        return 0.0;
}

double wtg_meter_interface::get_wind_speed_mps(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WIND_SPEED_MODEL* windspeed_model = gen->get_wind_speed_model();
    WT_AERODYNAMIC_MODEL* aerd_model = gen->get_wt_aerodynamic_model();

    if (windspeed_model)
        return windspeed_model->get_wind_speed_in_mps();
    else if (aerd_model)
        return aerd_model->get_wind_speed_in_mps();
    else
        return 0.0;
}

double wtg_meter_interface::get_braking_resistor_power_in_pu(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    WT_DBR_MODEL* model = gen->get_wt_dbr_model();
    return model ? model->get_braking_resistor_power_in_pu_based_on_mbase() : 0.0;
}

double wtg_meter_interface::get_generator_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_aerodynamic_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_AERODYNAMIC_MODEL* model = gen->get_wt_aerodynamic_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_turbine_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_electrical_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_ELECTRICAL_MODEL* model = gen->get_wt_electrical_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_pitch_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_PITCH_MODEL* model = gen->get_wt_pitch_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_vrt_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_VRT_MODEL* model = gen->get_wt_vrt_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_relay_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_RELAY_MODEL* model = gen->get_wt_relay_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_windspeed_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WIND_SPEED_MODEL* model = gen->get_wind_speed_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double wtg_meter_interface::get_dbr_model_internal_variable(const METER_PARAMETER& para)
{
    WT_GENERATOR* gen = (WT_GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    WT_DBR_MODEL* model = gen->get_wt_dbr_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}
