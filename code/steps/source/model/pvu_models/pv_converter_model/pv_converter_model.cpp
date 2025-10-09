#include "header/model/pvu_models/pv_converter_model/pv_converter_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

PV_CONVERTER_MODEL::PV_CONVERTER_MODEL(STEPS& toolkit) : PVU_MODEL(toolkit)
{
    set_current_source_flag(true);
    set_simplified_model_flag(false);
}

PV_CONVERTER_MODEL::~PV_CONVERTER_MODEL()
{
    ;
}

string PV_CONVERTER_MODEL::get_model_type() const
{
    return "PV CONVERTER";
}


void PV_CONVERTER_MODEL::set_current_source_flag(bool flag)
{
    current_source_flag = flag;
}

bool PV_CONVERTER_MODEL::get_current_source_flag() const
{
    return current_source_flag;
}

bool PV_CONVERTER_MODEL::is_current_source() const
{
    return current_source_flag;
}

bool PV_CONVERTER_MODEL::is_voltage_source() const
{
    return not is_current_source();
}

void PV_CONVERTER_MODEL::set_simplified_model_flag(bool flag)
{
    simplified_model_flag = flag;
}

bool PV_CONVERTER_MODEL::is_simplified_model() const
{
    return simplified_model_flag;
}

double PV_CONVERTER_MODEL::get_active_current_command_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_ELECTRICAL_MODEL* elec_model = pv_unit->get_pv_electrical_model();
    PV_VRT_MODEL* vrt_model = pv_unit->get_pv_vrt_model();

    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_active_current_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_active_current_command_in_pu_based_on_mbase();
        else
            return get_initial_active_current_command_in_pu_based_on_mbase();
    }
}

double PV_CONVERTER_MODEL::get_reactive_current_command_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_ELECTRICAL_MODEL* elec_model = pv_unit->get_pv_electrical_model();
    PV_VRT_MODEL* vrt_model = pv_unit->get_pv_vrt_model();
    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_reactive_current_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_reactive_current_command_in_pu_based_on_mbase();
        else
            return get_initial_reactive_current_command_in_pu_based_on_mbase();
    }
}

double PV_CONVERTER_MODEL::get_reactive_voltage_command_in_pu()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_ELECTRICAL_MODEL* elec_model = pv_unit->get_pv_electrical_model();
    PV_VRT_MODEL* vrt_model = pv_unit->get_pv_vrt_model();
    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_reactive_voltage_command_in_pu();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_reactive_voltage_command_in_pu();
        else
            return get_initial_reactive_voltage_command_in_pu();
    }
}

double PV_CONVERTER_MODEL::get_active_power_command_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_ELECTRICAL_MODEL* elec_model = pv_unit->get_pv_electrical_model();
    PV_VRT_MODEL* vrt_model = pv_unit->get_pv_vrt_model();
    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_active_power_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_active_power_command_in_pu_based_on_mbase();
        else
            return get_initial_active_power_command_in_pu_based_on_mbase();
    }
}

double PV_CONVERTER_MODEL::get_reactive_power_command_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_ELECTRICAL_MODEL* elec_model = pv_unit->get_pv_electrical_model();
    PV_VRT_MODEL* vrt_model = pv_unit->get_pv_vrt_model();
    if(elec_model!=NULL and elec_model->is_model_initialized() and not elec_model->is_model_bypassed())
        return elec_model->get_reactive_power_command_in_pu_based_on_mbase();
    else
    {
        if(vrt_model!=NULL and vrt_model->is_model_initialized() and vrt_model->is_in_vrt_status())
            return vrt_model->get_reactive_power_command_in_pu_based_on_mbase();
        else
            return get_initial_reactive_power_command_in_pu_based_on_mbase();
    }
}

complex<double> PV_CONVERTER_MODEL::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> PV_CONVERTER_MODEL::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double PV_CONVERTER_MODEL::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double PV_CONVERTER_MODEL::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double PV_CONVERTER_MODEL::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double PV_CONVERTER_MODEL::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

complex<double> PV_CONVERTER_MODEL::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    return Ixy*(mbase*one_over_sbase);
}

double PV_CONVERTER_MODEL::get_terminal_current_in_pu_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    double I = get_terminal_current_in_pu_based_on_mbase();
    return I*(mbase*one_over_sbase);
}

double PV_CONVERTER_MODEL::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double PV_CONVERTER_MODEL::get_active_power_generation_including_stator_loss_in_MW()
{
    return get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

complex<double> PV_CONVERTER_MODEL::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> I = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
    return I*(mbase*one_over_sbase);
}
