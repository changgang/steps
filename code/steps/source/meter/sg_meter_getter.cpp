#include "header/meter/sg_meter_getter.h"
#include "header/device/generator.h"
#include "header/model/sg_models/sync_generator_model/sync_generator_model.h"
#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

double sg_meter_getter::get_rotor_angle_in_deg(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_rotor_angle_in_deg() : 0.0;
}

double sg_meter_getter::get_rotor_angle_in_rad(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_rotor_angle_in_rad() : 0.0;
}

double sg_meter_getter::get_rotor_speed_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_rotor_speed_in_pu() : 0.0;
}

double sg_meter_getter::get_rotor_speed_in_Hz(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        unsigned int bus = gen->get_generator_bus();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        double fbase = psdb.get_bus_base_frequency_in_Hz(bus);
        return fbase * model->get_rotor_speed_in_pu();
    }
    else
        return 0.0;
}

double sg_meter_getter::get_rotor_speed_deviation_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_rotor_speed_deviation_in_pu() : 0.0;
}

double sg_meter_getter::get_rotor_speed_deviation_in_Hz(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        unsigned int bus = gen->get_generator_bus();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        double fbase = psdb.get_bus_base_frequency_in_Hz(bus);
        return fbase * model->get_rotor_speed_deviation_in_pu();
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_voltage_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? steps_fast_complex_abs(model->get_terminal_voltage_in_pu()) : 0.0;
}

double sg_meter_getter::get_internal_voltage_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? steps_fast_complex_abs(model->get_internal_voltage_in_pu_in_dq_axis()) : 0.0;
}

double sg_meter_getter::get_terminal_current_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_terminal_current_in_pu_based_on_mbase() : 0.0;
}

double sg_meter_getter::get_terminal_current_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_terminal_current_in_pu_based_on_sbase() : 0.0;
}

double sg_meter_getter::get_terminal_current_in_kA(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        unsigned int bus = gen->get_generator_bus();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        double vbase = psdb.get_bus_base_voltage_in_kV(bus);
        double sbase = psdb.get_system_base_power_in_MVA();
        double ibase = sbase / (SQRT3 * vbase);
        return ibase * model->get_terminal_current_in_pu_based_on_sbase();
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_active_power_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        double one_over_mbase = gen->get_one_over_mbase_in_one_over_MVA();
        return model->get_terminal_active_power_in_MW() * one_over_mbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_active_power_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_terminal_active_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_active_power_in_MW(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_terminal_active_power_in_MW() : 0.0;
}

double sg_meter_getter::get_terminal_reactive_power_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        double one_over_mbase = gen->get_one_over_mbase_in_one_over_MVA();
        return model->get_terminal_reactive_power_in_MVar() * one_over_mbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_terminal_reactive_power_in_MVar() : 0.0;
}

double sg_meter_getter::get_terminal_apparent_power_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_pu_based_on_mbase();
        double q = model->get_terminal_reactive_power_in_pu_based_on_mbase();
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_apparent_power_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_pu_based_on_mbase();
        double q = model->get_terminal_reactive_power_in_pu_based_on_mbase();
        double mbase = gen->get_mbase_in_MVA();
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return steps_sqrt(p * p + q * q) * (mbase * one_over_sbase);
    }
    else
        return 0.0;
}

double sg_meter_getter::get_terminal_apparent_power_in_MVA(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_pu_based_on_mbase();
        double q = model->get_terminal_reactive_power_in_pu_based_on_mbase();
        double mbase = gen->get_mbase_in_MVA();
        return steps_sqrt(p * p + q * q) * mbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_airgap_power_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_air_gap_power_in_pu_based_on_mbase() : 0.0;
}

double sg_meter_getter::get_airgap_power_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_air_gap_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_airgap_power_in_MW(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_air_gap_power_in_MW() : 0.0;
}

double sg_meter_getter::get_accelerating_power_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_accelerating_power_in_pu_based_on_mbase() : 0.0;
}

double sg_meter_getter::get_accelerating_power_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_accelerating_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_accelerating_power_in_MW(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_accelerating_power_in_MW() : 0.0;
}

double sg_meter_getter::get_mechanical_power_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_mechanical_power_in_pu_based_on_mbase() : 0.0;
}

double sg_meter_getter::get_mechanical_power_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    if (model != NULL)
    {
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_mechanical_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_mechanical_power_in_MW(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_mechanical_power_in_MW() : 0.0;
}

double sg_meter_getter::get_mechanical_power_reference_in_pu_mbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    TURBINE_GOVERNOR_MODEL* model = gen->get_turbine_governor_model();
    return model ? model->get_mechanical_power_reference_in_pu_based_on_mbase() : 0.0;
}

double sg_meter_getter::get_mechanical_power_reference_in_pu_sbase(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    TURBINE_GOVERNOR_MODEL* model = gen->get_turbine_governor_model();
    if (model != NULL)
    {
        double mbase = gen->get_mbase_in_MVA();
        STEPS& toolkit = gen->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_mechanical_power_reference_in_pu_based_on_mbase() * (mbase * one_over_sbase);
    }
    else
        return 0.0;
}

double sg_meter_getter::get_mechanical_power_reference_in_MW(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    TURBINE_GOVERNOR_MODEL* model = gen->get_turbine_governor_model();
    if (model != NULL)
    {
        double mbase = gen->get_mbase_in_MVA();
        return model->get_mechanical_power_reference_in_pu_based_on_mbase() * mbase;
    }
    else
        return 0.0;
}

double sg_meter_getter::get_compensated_voltage_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    EXCITER_MODEL* model = gen->get_exciter_model();
    return model ? model->get_compensated_voltage_in_pu() : 0.0;
}

double sg_meter_getter::get_voltage_reference_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    EXCITER_MODEL* model = gen->get_exciter_model();
    return model ? model->get_voltage_reference_in_pu() : 0.0;
}

double sg_meter_getter::get_stabilizing_signal_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    EXCITER_MODEL* model = gen->get_exciter_model();
    return model ? model->get_stabilizing_signal_in_pu() : 0.0;
}

double sg_meter_getter::get_excitation_voltage_in_pu(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_excitation_voltage_in_pu() : 0.0;
}

double sg_meter_getter::get_sync_generator_model_internal_variable(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    SYNC_GENERATOR_MODEL* model = gen->get_sync_generator_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double sg_meter_getter::get_compensator_model_internal_variable(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    COMPENSATOR_MODEL* model = gen->get_compensator_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double sg_meter_getter::get_stabilizer_model_internal_variable(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    STABILIZER_MODEL* model = gen->get_stabilizer_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double sg_meter_getter::get_exciter_model_internal_variable(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    EXCITER_MODEL* model = gen->get_exciter_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double sg_meter_getter::get_turbine_governor_model_internal_variable(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    TURBINE_GOVERNOR_MODEL* model = gen->get_turbine_governor_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double sg_meter_getter::get_turbine_load_controller_model_internal_variable(const METER_PARAMETER& para)
{
    GENERATOR* gen = (GENERATOR*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    TURBINE_LOAD_CONTROLLER_MODEL* model = gen->get_turbine_load_controller_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}
