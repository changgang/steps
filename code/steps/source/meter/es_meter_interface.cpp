#include "header/meter/es_meter_interface.h"
#include "header/device/energy_storage.h"
#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/model/energy_storage_model/es_soc_model/es_soc_model.h"
#include "header/model/energy_storage_model/es_electrical_model/es_electrical_model.h"
#include "header/model/energy_storage_model/es_vrt_model/es_vrt_model.h"
#include "header/model/energy_storage_model/es_relay_model/es_relay_model.h"
#include "header/model/energy_storage_model/es_primary_model/es_primary_model.h"
#include "header/model/energy_storage_model/es_rotor_model/es_rotor_model.h"
#include "header/model/energy_storage_model/es_auxiliary_model/es_auxiliary_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

double es_meter_interface::get_terminal_current_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_terminal_current_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_terminal_current_in_pu_sbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = es->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        double mbase = es->get_mbase_in_MVA();
        return model->get_terminal_current_in_pu_based_on_mbase() * (mbase * one_over_sbase);
    }
    else
        return 0.0;
}

double es_meter_interface::get_terminal_current_in_kA(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = es->get_toolkit();
        unsigned int bus = es->get_energy_storage_bus();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        double vbase = psdb.get_bus_base_voltage_in_kV(bus);
        double mbase = es->get_mbase_in_MVA();
        double ibase = mbase / (SQRT3 * vbase);
        return model->get_terminal_current_in_pu_based_on_mbase() * ibase;
    }
    else
        return 0.0;
}

double es_meter_interface::get_terminal_active_power_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_terminal_active_power_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_terminal_active_power_in_pu_sbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = es->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_terminal_active_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double es_meter_interface::get_terminal_active_power_in_MW(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_terminal_active_power_in_MW() : 0.0;
}

double es_meter_interface::get_terminal_reactive_power_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_terminal_reactive_power_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = es->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
    }
    else
        return 0.0;
}

double es_meter_interface::get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_terminal_reactive_power_in_MVar() : 0.0;
}

double es_meter_interface::get_terminal_apparent_power_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_pu_based_on_mbase();
        double q = model->get_terminal_reactive_power_in_pu_based_on_mbase();
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double es_meter_interface::get_terminal_apparent_power_in_pu_sbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = es->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        double p = model->get_terminal_active_power_in_MW() * one_over_sbase;
        double q = model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double es_meter_interface::get_terminal_apparent_power_in_MVA(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_MW();
        double q = model->get_terminal_reactive_power_in_MVar();
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double es_meter_interface::get_active_current_command_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_active_current_command_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_reactive_current_command_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_reactive_current_command_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_active_power_command_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_active_power_command_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_reactive_power_command_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_reactive_power_command_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_reactive_voltage_command_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_reactive_voltage_command_in_pu() : 0.0;
}

double es_meter_interface::get_state_of_charge_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_SOC_MODEL* model = es->get_es_soc_model();
    return model ? model->get_state_of_charge_in_pu() : 0.0;
}

double es_meter_interface::get_soc_power_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_SOC_MODEL* model = es->get_es_soc_model();
    return model ? model->get_soc_power_in_pu_based_on_mbase() : 0.0;
}

double es_meter_interface::get_soc_power_in_pu_sbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_SOC_MODEL* model = es->get_es_soc_model();
    if (model != NULL)
    {
        STEPS& toolkit = es->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_soc_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double es_meter_interface::get_soc_power_in_MW(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_SOC_MODEL* model = es->get_es_soc_model();
    return model ? model->get_soc_power_in_MW() : 0.0;
}

double es_meter_interface::get_mechanical_power_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_PRIMARY_MODEL* model = es->get_es_primary_model();
    return model ? model->get_mechanical_power_in_pu_on_mbase() : 0.0;
}

double es_meter_interface::get_rotor_speed_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_ROTOR_MODEL* model = es->get_es_rotor_model();
    if (model != NULL)
    {
        double speed = model->get_rotor_speed_in_pu();
        if (speed <= MINIMUM_ROTOR_SPEED_THRESHOLD && speed >= -MINIMUM_ROTOR_SPEED_THRESHOLD)
            speed = 0.0;
        return speed;
    }
    else
        return 0.0;
}

double es_meter_interface::get_rotor_speed_reference_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_PRIMARY_MODEL* model = es->get_es_primary_model();
    return model ? model->get_rotor_speed_reference_in_pu() : 0.0;
}

double es_meter_interface::get_valve_reference_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_PRIMARY_MODEL* model = es->get_es_primary_model();
    return model ? model->get_valve_reference_in_pu() : 0.0;
}

double es_meter_interface::get_valve_in_pu(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_PRIMARY_MODEL* model = es->get_es_primary_model();
    return model ? model->get_valve_in_pu() : 0.0;
}

double es_meter_interface::get_active_power_reference_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_AUXILIARY_MODEL* model = es->get_es_auxiliary_model();
    return model ? model->get_active_power_reference_in_pu() : 0.0;
}

double es_meter_interface::get_frequency_regulation_power_in_pu_mbase(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    ES_ELECTRICAL_MODEL* model = es->get_es_electrical_model();
    return model ? model->get_frequency_regulation_power_in_pu() : 0.0;
}

double es_meter_interface::get_converter_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_CONVERTER_MODEL* model = es->get_es_converter_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_soc_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_SOC_MODEL* model = es->get_es_soc_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_electrical_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_ELECTRICAL_MODEL* model = es->get_es_electrical_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_vrt_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_VRT_MODEL* model = es->get_es_vrt_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_relay_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_RELAY_MODEL* model = es->get_es_relay_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_primary_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_PRIMARY_MODEL* model = es->get_es_primary_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_rotor_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_ROTOR_MODEL* model = es->get_es_rotor_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double es_meter_interface::get_auxiliary_model_internal_variable(const METER_PARAMETER& para)
{
    ENERGY_STORAGE* es = (ENERGY_STORAGE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    ES_AUXILIARY_MODEL* model = es->get_es_auxiliary_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}
