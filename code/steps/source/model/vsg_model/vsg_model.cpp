#include "header/model/vsg_model/vsg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
VSG_MODEL::VSG_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_WT_GENERATOR);
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_PV_UNIT);
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_ENERGY_STORAGE);
}

VSG_MODEL::~VSG_MODEL()
{
    ;
}

string VSG_MODEL::get_model_type() const
{
    return "VSG MODEL";
}

SOURCE* VSG_MODEL::get_source_pointer() const
{
    NONBUS_DEVICE* device = get_device_pointer();
    if(device != nullptr)
    {
        DEVICE_ID did = device->get_device_id();
        switch(did.get_device_type())
        {
            case STEPS_WT_GENERATOR:
            case STEPS_PV_UNIT:
            case STEPS_ENERGY_STORAGE:
                return (SOURCE*) device;
            default:
            {
                ostringstream osstream;
                osstream<<"Error when returning device pointer as SOURCE in "<<get_model_name()<<" model for "<<did.get_compound_device_name();
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);
                return  nullptr;
            }
        }
    }
    else
        return nullptr;
}

WT_GENERATOR* VSG_MODEL::get_wt_generator_pointer() const
{
    NONBUS_DEVICE* device = get_device_pointer();
    if(device != nullptr)
    {
        DEVICE_ID did = device->get_device_id();
        if(did.get_device_type()==STEPS_WT_GENERATOR)
            return (WT_GENERATOR*) device;
        else
        {
            ostringstream osstream;
            osstream<<"Error when returning device pointer as WT_GENERATOR in "<<get_model_name()<<" model for "<<did.get_compound_device_name();
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            return  nullptr;
        }
    }
    else
        return nullptr;
}

PV_UNIT* VSG_MODEL::get_pv_unit_pointer() const
{
    NONBUS_DEVICE* device = get_device_pointer();
    if(device != nullptr)
    {
        DEVICE_ID did = device->get_device_id();
        if(did.get_device_type()==STEPS_PV_UNIT)
            return (PV_UNIT*) device;
        else
        {
            ostringstream osstream;
            osstream<<"Error when returning device pointer as PV_UNIT in "<<get_model_name()<<" model for "<<did.get_compound_device_name();
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            return  nullptr;
        }
    }
    else
        return nullptr;
}

ENERGY_STORAGE* VSG_MODEL::get_enerage_storage_pointer() const
{
    NONBUS_DEVICE* device = get_device_pointer();
    if(device != nullptr)
    {
        DEVICE_ID did = device->get_device_id();
        if(did.get_device_type()==STEPS_ENERGY_STORAGE)
            return (ENERGY_STORAGE*) device;
        else
        {
            ostringstream osstream;
            osstream<<"Error when returning device pointer as ENERGY_STORAGE in "<<get_model_name()<<" model for "<<did.get_compound_device_name();
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            return  nullptr;
        }
    }
    else
        return nullptr;
}

double VSG_MODEL::get_mbase_in_MVA() const
{
    SOURCE* source = get_source_pointer();
    return source->get_mbase_in_MVA();
}

double VSG_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    return 1.0/get_mbase_in_MVA();
}

complex<double> VSG_MODEL::get_terminal_complex_power_in_pu_based_on_mbase() const
{
    NONBUS_DEVICE* device = get_device_pointer();
    if(device != nullptr)
    {
        DEVICE_ID did = device->get_device_id();
        if(did.get_device_type()==STEPS_WT_GENERATOR)
        {
            WT_GENERATOR* gen = (WT_GENERATOR*) device;
            WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
            return genmodel->get_terminal_complex_power_in_pu_based_on_mbase();
        }
        else
        {
            if(did.get_device_type()==STEPS_PV_UNIT)
            {
                PV_UNIT* pvu = (PV_UNIT*) device;
                PV_CONVERTER_MODEL* conmodel = pvu->get_pv_converter_model();
                return conmodel->get_terminal_complex_power_in_pu_based_on_mbase();
            }
            else
            {
                ENERGY_STORAGE* es = (ENERGY_STORAGE*) device;
                ENERGY_STORAGE_MODEL* esmodel = es->get_energy_storage_model();
                return esmodel->get_terminal_complex_power_in_pu_based_on_mbase();
            }
        }
    }
    else
        return 0.0;
}

double VSG_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

complex<double> VSG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double VSG_MODEL::get_terminal_voltage_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}

double VSG_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

double VSG_MODEL::get_bus_base_angle_speed_in_radps() const
{
    return DOUBLE_PI*get_bus_base_frequency_in_Hz();
}

complex<double> VSG_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    SOURCE* source = get_source_pointer();
    return source->get_source_impedance_in_pu();
}

void VSG_MODEL::set_Pref_in_pu_based_on_mbase(double P)
{
    Pref = P;
}

void VSG_MODEL::set_Qref_in_pu_based_on_mbase(double Q)
{
    Qref = Q;
}

void VSG_MODEL::set_Vref_in_pu(double V)
{
    Vref = V;
}

double VSG_MODEL::get_Pref_in_pu_based_on_mbase() const
{
    return Pref;
}

double VSG_MODEL::get_Qref_in_pu_based_on_mbase() const
{
    return Qref;
}

double VSG_MODEL::get_Vref_in_pu() const
{
    return Vref;
}

double VSG_MODEL::get_virtual_speed_in_pu() const
{
    return 1.0+get_virtual_speed_deviation_in_pu();
}
