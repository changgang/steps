#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

WT_GENERATOR_MODEL::WT_GENERATOR_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    set_current_source_flag(true);
    set_simplified_model_flag(false);
}

WT_GENERATOR_MODEL::~WT_GENERATOR_MODEL()
{
    ;
}

string WT_GENERATOR_MODEL::get_model_type() const
{
    return "WT GENERATOR";
}


void WT_GENERATOR_MODEL::set_current_source_flag(bool flag)
{
    current_source_flag = flag;
}

bool WT_GENERATOR_MODEL::get_current_source_flag() const
{
    return current_source_flag;
}

bool WT_GENERATOR_MODEL::is_current_source() const
{
    return current_source_flag;
}

bool WT_GENERATOR_MODEL::is_voltage_source() const
{
    return not is_current_source();
}

void WT_GENERATOR_MODEL::set_simplified_model_flag(bool flag)
{
    simplified_model_flag = flag;
}

bool WT_GENERATOR_MODEL::is_simplified_model() const
{
    return simplified_model_flag;
}

double WT_GENERATOR_MODEL::get_active_current_command_in_pu_based_on_mbase()
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    WT_ELECTRICAL_MODEL* elec_model = wt_generator->get_wt_electrical_model();
    WT_VRT_MODEL* vrt_model = wt_generator->get_wt_vrt_model();

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

double WT_GENERATOR_MODEL::get_reactive_current_command_in_pu_based_on_mbase()
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    WT_ELECTRICAL_MODEL* elec_model = wt_generator->get_wt_electrical_model();
    WT_VRT_MODEL* vrt_model = wt_generator->get_wt_vrt_model();

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

double WT_GENERATOR_MODEL::get_reactive_voltage_command_in_pu()
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    WT_ELECTRICAL_MODEL* elec_model = wt_generator->get_wt_electrical_model();
    WT_VRT_MODEL* vrt_model = wt_generator->get_wt_vrt_model();

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

double WT_GENERATOR_MODEL::get_active_power_command_in_pu_based_on_mbase()
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    WT_ELECTRICAL_MODEL* elec_model = wt_generator->get_wt_electrical_model();
    WT_VRT_MODEL* vrt_model = wt_generator->get_wt_vrt_model();

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

double WT_GENERATOR_MODEL::get_reactive_power_command_in_pu_based_on_mbase()
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    WT_ELECTRICAL_MODEL* elec_model = wt_generator->get_wt_electrical_model();
    WT_VRT_MODEL* vrt_model = wt_generator->get_wt_vrt_model();

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

complex<double> WT_GENERATOR_MODEL::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> WT_GENERATOR_MODEL::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double WT_GENERATOR_MODEL::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double WT_GENERATOR_MODEL::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double WT_GENERATOR_MODEL::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double WT_GENERATOR_MODEL::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

complex<double> WT_GENERATOR_MODEL::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    return Ixy*(mbase*one_over_sbase);
}

double WT_GENERATOR_MODEL::get_terminal_current_in_pu_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    double I = get_terminal_current_in_pu_based_on_mbase();
    return I*(mbase*one_over_sbase);
}

double WT_GENERATOR_MODEL::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double WT_GENERATOR_MODEL::get_active_power_generation_including_stator_loss_in_MW()
{
    return get_active_power_generation_including_stator_loss_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

complex<double> WT_GENERATOR_MODEL::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double mbase = get_mbase_in_MVA();
    complex<double> I = get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
    return I*(mbase*one_over_sbase);
}
