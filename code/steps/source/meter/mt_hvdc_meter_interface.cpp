#include "header/meter/mt_hvdc_meter_interface.h"
#include "header/device/mt_hvdc.h"
#include "header/device/dc_component/dc_bus.h"
#include "header/device/dc_component/dc_line.h"
#include "header/basic/steps_enum.h"
#include "header/basic/utility.h"
#include "header/meter/meter.h"


HLCC* get_lcc_converter_of_mt_hvdc_from_meter(const METER_PARAMETER& para)
{
    MT_HVDC* mtdc = (MT_HVDC*) para.device_pointer;
    if(mtdc != NULL)
    {
        if(mtdc->get_status() == true)
        {
            string lcc_name = para.sub_device_name;
            return mtdc->get_lcc_converter_with_name(lcc_name);
        }
    }
    return NULL;
}

HVSC* get_vsc_converter_of_mt_hvdc_from_meter(const METER_PARAMETER& para)
{
    MT_HVDC* mtdc = (MT_HVDC*) para.device_pointer;
    if(mtdc != NULL)
    {
        if(mtdc->get_status() == true)
        {
            string vsc_name = para.sub_device_name;
            return mtdc->get_vsc_converter_with_name(vsc_name);
        }
    }
    return NULL;
}

double mt_hvdc_meter_interface::get_lcc_converter_dc_current_in_kA(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_idc_in_kA();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_dc_voltage_in_kV(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_udc_in_kV();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_dc_power_in_MW(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_converter_dc_power_in_MW();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_ac_current_in_kA(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
    {
        complex<double> Iac_pu = lcc->get_converter_dynamic_ac_current_in_pu_on_system_base();
        return abs(Iac_pu);
    }
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_firing_angle_in_deg(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_firing_angle_in_deg();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_active_power_in_MW(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_converter_P_2_ac_bus();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_converter_Q_2_ac_bus();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_reference_angle_of_cc_in_deg(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_dynamic_ang_ref_in_deg_of_cc();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_reference_angle_of_cea_in_deg(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_dynamic_ang_ref_in_deg_of_cea();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_reference_angle_of_cv_in_deg(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_dynamic_ang_ref_in_deg_of_cv();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_lcc_converter_idc_ref_in_kA(const METER_PARAMETER& para)
{
    HLCC* lcc = get_lcc_converter_of_mt_hvdc_from_meter(para);
    if(lcc != nullptr)
        return lcc->get_dynamic_idc_ref_in_kA();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_dc_current_in_kA(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_idc_in_kA();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_dc_voltage_in_kV(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_udc_in_kV();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_dc_power_in_MW(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_converter_pdc_in_MW();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_ac_current_in_kA(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
    {
        complex<double> Ixy = vsc->get_converter_dynamic_ac_current_in_kA();
        return abs(Ixy);
    }
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_active_power_in_MW(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_converter_dynamic_active_power_in_MW();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_converter_dynamic_reactive_power_in_Mvar();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_id_in_kA(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_converter_dynamic_d_axis_current_in_pu();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_vsc_converter_iq_in_kA(const METER_PARAMETER& para)
{
    HVSC* vsc = get_vsc_converter_of_mt_hvdc_from_meter(para);
    if(vsc != nullptr)
        return vsc->get_converter_dynamic_q_axis_current_in_pu();
    else
        return 0.0;
}

double mt_hvdc_meter_interface::get_dc_bus_voltage_in_kV(const METER_PARAMETER& para)
{
    MT_HVDC* mtdc = (MT_HVDC*) para.device_pointer;
    if(mtdc != NULL)
    {
        if(mtdc->get_status() == true)
        {
            string dcbus_name = para.sub_device_name;
            DC_BUS* bus = mtdc->get_dc_bus_with_name(dcbus_name);
            return bus->get_dc_bus_voltage_in_kV();
        }
    }
    return 0.0;
}

double mt_hvdc_meter_interface::get_dc_line_current_in_kA(const METER_PARAMETER& para)
{
    MT_HVDC* mtdc = (MT_HVDC*) para.device_pointer;
    if(mtdc != NULL)
    {
        if(mtdc->get_status() == true)
        {
            string line_name = para.sub_device_name;
            unsigned int side_bus = para.metered_side_bus;
            DC_LINE* line = mtdc->get_dc_line_with_line_name(line_name);
            if(line->get_sending_side_bus_number() == side_bus)
                return line->get_i_RL_in_kA_on_sending_side();
            else if(line->get_receiving_side_bus_number() == side_bus)
                return line->get_i_RL_in_kA_on_receiving_side();
        }
    }
    return 0.0;
}

double mt_hvdc_meter_interface::get_hlcc_converter_model_internal_variable(const METER_PARAMETER& para)
{
    return 0.0;
}

double mt_hvdc_meter_interface::get_hvsc_converter_model_internal_variable(const METER_PARAMETER& para)
{
    return 0.0;
}

double mt_hvdc_meter_interface::get_dc_network_model_internal_variable(const METER_PARAMETER& para)
{
    return 0.0;
}
