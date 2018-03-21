#include "header/model/source_model/pe_source_model/pe_source_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>

PE_SOURCE_MODEL::PE_SOURCE_MODEL()
{
    common_constructor();
}

void PE_SOURCE_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("PE SOURCE");

    set_initial_active_current_command_in_pu_based_on_mbase(0.0);
    set_initial_reactive_current_command_in_pu_based_on_mbase(0.0);
    set_initial_reactive_voltage_command_in_pu(0.0);
}

PE_SOURCE_MODEL::~PE_SOURCE_MODEL()
{
    ;
}

string PE_SOURCE_MODEL::get_model_type() const
{
    return "PE SOURCE";
}

void PE_SOURCE_MODEL::update_source_impedance()
{
    PE_SOURCE* pesource = (PE_SOURCE*) get_device_pointer();
    if(pesource==NULL)
        return;
    else
    {
        complex<double> Zs = pesource->get_source_impedance_in_pu();
        set_source_impedance_in_pu_based_on_mbase(Zs);
    }
}

void PE_SOURCE_MODEL::set_source_impedance_in_pu_based_on_mbase(complex<double> zs)
{
    this->Z_source_in_pu = zs;
}

complex<double> PE_SOURCE_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    return this->Z_source_in_pu;
}


void PE_SOURCE_MODEL::set_initial_active_current_command_in_pu_based_on_mbase(double ip_command)
{
    IP_command0 = ip_command;
}

double PE_SOURCE_MODEL::get_initial_active_current_command_in_pu_based_on_mbase() const
{
    return IP_command0;
}

void PE_SOURCE_MODEL::set_initial_reactive_current_command_in_pu_based_on_mbase(double iq_command)
{
    IQ_command0 = iq_command;
}

double PE_SOURCE_MODEL::get_initial_reactive_current_command_in_pu_based_on_mbase() const
{
    return IQ_command0;
}

void PE_SOURCE_MODEL::set_initial_reactive_voltage_command_in_pu(double eq_command)
{
    EQ_command0 = eq_command;
}

double PE_SOURCE_MODEL::get_initial_reactive_voltage_command_in_pu() const
{
    return EQ_command0;
}


double PE_SOURCE_MODEL::get_active_current_command_in_pu_based_on_mbase() const
{
    PE_SOURCE* pesource = (PE_SOURCE*) get_device_pointer();
    if(pesource==NULL)
        return 0.0;

    return get_initial_active_current_command_in_pu_based_on_mbase();
}

double PE_SOURCE_MODEL::get_reactive_current_command_in_pu_based_on_mbase() const
{
    PE_SOURCE* pesource = (PE_SOURCE*) get_device_pointer();
    if(pesource==NULL)
        return 0.0;

    return get_initial_reactive_current_command_in_pu_based_on_mbase();
}

double PE_SOURCE_MODEL::get_reactive_voltage_command_in_pu() const
{
    PE_SOURCE* pesource = (PE_SOURCE*) get_device_pointer();
    if(pesource==NULL)
        return 0.0;

    return get_initial_reactive_voltage_command_in_pu();
}
