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
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("PV UNIT");

    set_current_source_flag(true);
    set_initial_active_current_command_in_pu_based_on_mbase(0.0);
    set_initial_reactive_current_command_in_pu_based_on_mbase(0.0);
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

void PV_CONVERTER_MODEL::set_initial_active_current_command_in_pu_based_on_mbase(double ip_command)
{
    IP_command0 = ip_command;
}

double PV_CONVERTER_MODEL::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return IP_command0;
}

void PV_CONVERTER_MODEL::set_initial_reactive_current_command_in_pu_based_on_mbase(double iq_command)
{
    IQ_command0 = iq_command;
}

double PV_CONVERTER_MODEL::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return IQ_command0;
}

void PV_CONVERTER_MODEL::set_initial_reactive_voltage_command_in_pu(double eq_command)
{
    EQ_command0 = eq_command;
}

double PV_CONVERTER_MODEL::get_initial_reactive_voltage_command_in_pu() const
{
    return EQ_command0;
}

double PV_CONVERTER_MODEL::get_active_current_command_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_active_current_command_in_pu_based_on_mbase();
        else
            return get_initial_active_current_command_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}

double PV_CONVERTER_MODEL::get_reactive_current_command_in_pu_based_on_mbase()
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_reactive_current_command_in_pu_based_on_mbase();
        else
            return get_initial_reactive_current_command_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}


double PV_CONVERTER_MODEL::get_reactive_voltage_command_in_pu() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        PV_ELECTRICAL_MODEL* model = pv_unit->get_pv_electrical_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_reactive_voltage_command_in_pu();
        else
            return get_initial_reactive_voltage_command_in_pu();
    }
    else
        return 0.0;
}
