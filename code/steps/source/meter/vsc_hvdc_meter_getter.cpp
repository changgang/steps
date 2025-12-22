#include "header/meter/vsc_hvdc_meter_getter.h"
#include "header/device/vsc_hvdc.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/vsc_hvdc_network_model.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

double vsc_hvdc_meter_getter::get_converter_ac_voltage_in_pu(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return steps_fast_complex_abs(vsc_hvdc->get_converter_ac_voltage_in_pu_with_ac_bus_number(metered_bus));
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_ac_voltage_in_kV(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return steps_fast_complex_abs(vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(metered_bus));
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_ac_current_in_kA(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return steps_fast_complex_abs(vsc_hvdc->get_converter_ac_current_in_kA_with_ac_bus_number(metered_bus));
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_dc_voltage_in_kV(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_converter_dc_voltage_in_kV_with_ac_bus_number(metered_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_dc_current_in_kA(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_converter_dc_current_in_kA_with_ac_bus_number(metered_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_dc_power_in_MW(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_converter_dc_power_in_MW_with_ac_bus_number(metered_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_ac_active_power_in_MW(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_converter_dynamic_ac_active_power_in_MW_with_ac_bus_number(metered_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_converter_ac_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_converter_dynamic_ac_reactive_power_in_MVar_with_ac_bus_number(metered_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_dc_bus_voltage_in_kV(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int dc_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_dc_bus_Vdc_in_kV_with_dc_bus_number(dc_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_dc_line_current_in_kA(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int dc_bus = para.metered_side_bus;
    DC_DEVICE_ID dc_did = para.sub_dc_did;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_dc_line_current_in_kA(dc_did, dc_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_dc_line_power_in_MW(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    unsigned int dc_bus = para.metered_side_bus;
    DC_DEVICE_ID dc_did = para.sub_dc_did;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            return vsc_hvdc->get_dc_line_power_in_MW(dc_did, dc_bus);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_vsc_hvdc_network_model_internal_variable(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    string internal_var_name = para.internal_var_name;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            VSC_HVDC_NETWORK_MODEL* vsc_hvdc_network_model = vsc_hvdc->get_vsc_hvdc_network_model();
            if(vsc_hvdc_network_model != NULL)
                return vsc_hvdc_network_model->get_model_internal_variable_with_name(internal_var_name);
        }
    }
    return 0.0;
}

double vsc_hvdc_meter_getter::get_vsc_hvdc_converter_model_internal_variable(const METER_PARAMETER& para)
{
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    unsigned int metered_bus = para.metered_side_bus;

    if(vsc_hvdc != NULL)
    {
        if(vsc_hvdc->get_status()==true)
        {
            vector<VSC_HVDC_CONVERTER_MODEL*> vsc_hvdc_converter_models = vsc_hvdc->get_vsc_hvdc_converter_models();
            unsigned int index = vsc_hvdc->get_converter_index_with_ac_bus(metered_bus);
            if(index < vsc_hvdc_converter_models.size() && vsc_hvdc_converter_models[index] != NULL)
                return vsc_hvdc_converter_models[index]->get_model_internal_variable_with_name(internal_var_name);
        }
    }
    return 0.0;
}
