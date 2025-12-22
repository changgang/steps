#include "header/meter/pvu_meter_getter.h"
#include "header/device/pv_unit.h"
#include "header/model/pvu_models/pv_converter_model/pv_converter_model.h"
#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include "header/model/pvu_models/pv_electrical_model/pv_electrical_model.h"
#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_model.h"
#include "header/model/pvu_models/pv_vrt_model/pv_vrt_model.h"
#include "header/model/pvu_models/pv_relay_model/pv_relay_model.h"
#include "header/model/pvu_models/pv_temperature_model/pv_temperature_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

double pvu_meter_getter::get_terminal_current_in_pu_mbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_terminal_current_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_terminal_current_in_pu_sbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = pv_unit->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        double mbase = pv_unit->get_mbase_in_MVA();
        return model->get_terminal_current_in_pu_based_on_mbase() * (mbase * one_over_sbase);
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_terminal_current_in_kA(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = pv_unit->get_toolkit();
        unsigned int bus = pv_unit->get_unit_bus();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        double vbase = psdb.get_bus_base_voltage_in_kV(bus);
        double mbase = pv_unit->get_mbase_in_MVA();
        double ibase = mbase / (SQRT3 * vbase);
        return model->get_terminal_current_in_pu_based_on_mbase() * ibase;
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_terminal_active_power_in_pu_mbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_terminal_active_power_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_terminal_active_power_in_pu_sbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = pv_unit->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_terminal_active_power_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_terminal_active_power_in_MW(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_terminal_active_power_in_MW() : 0.0;
}

double pvu_meter_getter::get_terminal_reactive_power_in_pu_mbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_terminal_reactive_power_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = pv_unit->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_terminal_reactive_power_in_MVar() : 0.0;
}

double pvu_meter_getter::get_terminal_apparent_power_in_pu_mbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_pu_based_on_mbase();
        double q = model->get_terminal_reactive_power_in_pu_based_on_mbase();
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_terminal_apparent_power_in_pu_sbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        STEPS& toolkit = pv_unit->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        double p = model->get_terminal_active_power_in_MW() * one_over_sbase;
        double q = model->get_terminal_reactive_power_in_MVar() * one_over_sbase;
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_terminal_apparent_power_in_MVA(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    if (model != NULL)
    {
        double p = model->get_terminal_active_power_in_MW();
        double q = model->get_terminal_reactive_power_in_MVar();
        return steps_sqrt(p * p + q * q);
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_active_current_command_in_pu(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_active_current_command_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_reactive_current_command_in_pu(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_reactive_current_command_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_active_power_command_in_pu(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_active_power_command_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_reactive_power_command_in_pu(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_reactive_power_command_in_pu_based_on_mbase() : 0.0;
}

double pvu_meter_getter::get_reactive_voltage_command_in_pu(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_reactive_voltage_command_in_pu() : 0.0;
}

double pvu_meter_getter::get_solar_irradiance_pu_S0(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_solar_irradiance_in_pu_based_on_S0() : 0.0;
}

double pvu_meter_getter::get_solar_irradiance_pu_Sref(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_solar_irradiance_in_pu_based_on_Sref() : 0.0;
}

double pvu_meter_getter::get_solar_irradiance_wpm2(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_solar_irradiance_in_Wpm2() : 0.0;
}

double pvu_meter_getter::get_temperature_c_deg(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_TEMPERATURE_MODEL* model = pv_unit->get_pv_temperature_model();
    return model ? model->get_Tref_in_C_deg() : 0.0;
}

double pvu_meter_getter::get_max_available_power_irradiance_pu_mbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_maximum_available_power_extracted_power_from_irradiance_in_pu_on_mbase() : 0.0;
}

double pvu_meter_getter::get_max_available_power_irradiance_pu_sbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    if (model != NULL)
    {
        STEPS& toolkit = pv_unit->get_toolkit();
        double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
        return model->get_maximum_available_power_extracted_power_from_irradiance_in_MW() * one_over_sbase;
    }
    else
        return 0.0;
}

double pvu_meter_getter::get_max_available_power_irradiance_MW(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_maximum_available_power_extracted_power_from_irradiance_in_MW() : 0.0;
}

double pvu_meter_getter::get_panel_power_in_pu_mbase(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_pv_panel_power_in_pu_on_mbase() : 0.0;
}

double pvu_meter_getter::get_panel_power_in_MW(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_pv_panel_power_in_MW() : 0.0;
}

double pvu_meter_getter::get_converter_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double pvu_meter_getter::get_panel_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_PANEL_MODEL* model = pv_unit->get_pv_panel_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double pvu_meter_getter::get_electrical_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double pvu_meter_getter::get_irradiance_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_IRRADIANCE_MODEL* model = pv_unit->get_pv_irradiance_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double pvu_meter_getter::get_temperature_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_TEMPERATURE_MODEL* model = pv_unit->get_pv_temperature_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double pvu_meter_getter::get_vrt_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_VRT_MODEL* model = pv_unit->get_pv_vrt_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}

double pvu_meter_getter::get_relay_model_internal_variable(const METER_PARAMETER& para)
{
    PV_UNIT* pv_unit = (PV_UNIT*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    PV_RELAY_MODEL* model = pv_unit->get_pv_relay_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}
