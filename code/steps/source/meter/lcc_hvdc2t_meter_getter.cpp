#include "header/meter/lcc_hvdc2t_meter_getter.h"
#include "header/device/lcc_hvdc2t.h"
#include "header/model/lcc_hvdc2t_model/lcc_hvdc2t_model.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

double lcc_hvdc2t_meter_getter::get_dc_current_in_kA(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_dc_current_in_kA(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_ac_current_in_kA(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return steps_fast_complex_abs(model->get_converter_ac_current_in_kA(side));
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_angle_in_deg(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_alpha_or_gamma_in_deg(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_mu_in_deg(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_commutation_overlap_angle_in_deg(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_dc_voltage_in_kV(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_dc_voltage_in_kV(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_ac_voltage_in_pu(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_ac_voltage_in_pu(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_dc_power_in_MW(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_dc_power_in_MW(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_ac_active_power_in_MW(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_ac_complex_power_in_MVA(side).real();
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_ac_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_ac_complex_power_in_MVA(side).imag();
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_dc_current_in_kA(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_dc_current_in_kA(RECTIFIER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_ac_current_in_kA(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return steps_fast_complex_abs(model->get_converter_ac_current_in_kA(RECTIFIER));
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_dc_current_in_kA(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_dc_current_in_kA(INVERTER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_ac_current_in_kA(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return steps_fast_complex_abs(model->get_converter_ac_current_in_kA(INVERTER));
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_alpha_in_deg(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_alpha_or_gamma_in_deg(RECTIFIER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_gamma_in_deg(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_alpha_or_gamma_in_deg(INVERTER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_mu_in_deg(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        CONVERTER_SIDE side = hvdc->get_converter_side_at_bus(para.metered_side_bus);
        return model->get_converter_commutation_overlap_angle_in_deg(side);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_mu_in_deg(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_commutation_overlap_angle_in_deg(INVERTER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_dc_voltage_in_kV(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_dc_voltage_in_kV(RECTIFIER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_dc_voltage_in_kV(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_dc_voltage_in_kV(INVERTER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_ac_voltage_in_pu(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_ac_voltage_in_pu(RECTIFIER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_ac_voltage_in_pu(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_ac_voltage_in_pu(INVERTER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_dc_power_in_MW(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_dc_power_in_MW(RECTIFIER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_dc_power_in_MW(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_dc_power_in_MW(INVERTER);
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_ac_active_power_in_MW(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_ac_complex_power_in_MVA(RECTIFIER).real();
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_ac_active_power_in_MW(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_ac_complex_power_in_MVA(INVERTER).real();
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_rectifier_ac_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_ac_complex_power_in_MVA(RECTIFIER).imag();
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_inverter_ac_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_converter_ac_complex_power_in_MVA(INVERTER).imag();
    }
    else
        return 0.0;
}

double lcc_hvdc2t_meter_getter::get_lcc_hvdc2t_model_internal_variable(const METER_PARAMETER& para)
{
    LCC_HVDC2T* hvdc = (LCC_HVDC2T*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    LCC_HVDC2T_MODEL* model = hvdc->get_lcc_hvdc2t_model();
    if(model != NULL && hvdc->get_status()==true)
    {
        return model->get_model_internal_variable_with_name(internal_var_name);
    }
    else
        return 0.0;
}
