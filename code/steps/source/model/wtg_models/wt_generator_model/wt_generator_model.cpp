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
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_WT_GENERATOR);

    set_current_source_flag(true);
    set_initial_active_current_command_in_pu_based_on_mbase(0.0);
    set_initial_reactive_current_command_in_pu_based_on_mbase(0.0);
    set_initial_active_power_command_in_pu_based_on_mbase(0.0);
    set_initial_reactive_power_command_in_pu_based_on_mbase(0.0);
    set_initial_reactive_voltage_command_in_pu(0.0);
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

void WT_GENERATOR_MODEL::set_initial_active_current_command_in_pu_based_on_mbase(double ip_command)
{
    IP_command0 = ip_command;
}

double WT_GENERATOR_MODEL::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return IP_command0;
}

void WT_GENERATOR_MODEL::set_initial_reactive_current_command_in_pu_based_on_mbase(double iq_command)
{
    IQ_command0 = iq_command;
}

double WT_GENERATOR_MODEL::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return IQ_command0;
}

void WT_GENERATOR_MODEL::set_initial_reactive_voltage_command_in_pu(double eq_command)
{
    EQ_command0 = eq_command;
}

double WT_GENERATOR_MODEL::get_initial_reactive_voltage_command_in_pu() const
{
    return EQ_command0;
}

void WT_GENERATOR_MODEL::set_initial_active_power_command_in_pu_based_on_mbase(double P_command)
{
    P_command0 = P_command;
}

double WT_GENERATOR_MODEL::get_initial_active_power_command_in_pu_based_on_mbase() const
{
    return P_command0;
}

void WT_GENERATOR_MODEL::set_initial_reactive_power_command_in_pu_based_on_mbase(double Q_command)
{
    Q_command0 = Q_command;
}

double WT_GENERATOR_MODEL::get_initial_reactive_power_command_in_pu_based_on_mbase() const
{
    return Q_command0;
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
        if(vrt_model!=NULL and vrt_model->is_model_initialized())
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
        if(vrt_model!=NULL and vrt_model->is_model_initialized())
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
        if(vrt_model!=NULL and vrt_model->is_model_initialized())
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
        if(vrt_model!=NULL and vrt_model->is_model_initialized())
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
        if(vrt_model!=NULL and vrt_model->is_model_initialized())
            return vrt_model->get_reactive_power_command_in_pu_based_on_mbase();
        else
            return get_initial_reactive_power_command_in_pu_based_on_mbase();
    }
}
