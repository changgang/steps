#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include <cstdio>
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>

using namespace std;
STABILIZER_MODEL::STABILIZER_MODEL(STEPS& toolkit) : SG_MODEL(toolkit),
                                                     signal_0(toolkit),signal_1(toolkit),
                                                     signal_2(toolkit),signal_3(toolkit),
                                                     signal_4(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
    for(unsigned int slot=0; slot<STEPS_MAX_STABILIZER_INPUT_SIGNAL_SLOT; ++slot)
    {
        SIGNAL* signal = get_nonconst_pointer_of_signal_at_slot(slot);
        signal->clear();
    }
}

STABILIZER_MODEL::~STABILIZER_MODEL()
{
}

string STABILIZER_MODEL::get_model_type() const
{
    return "STABILIZER";
}

const SIGNAL* STABILIZER_MODEL::get_const_pointer_of_signal_at_slot(unsigned int slot) const
{
    const SIGNAL* signal = nullptr;
    switch(slot)
    {
        case 0:
        {
            signal = &signal_0;
            break;
        }
        case 1:
        {
            signal = &signal_1;
            break;
        }
        case 2:
        {
            signal = &signal_2;
            break;
        }
        case 3:
        {
            signal = &signal_3;
            break;
        }
        case 4:
        {
            signal = &signal_4;
            break;
        }
        default:
            break;
    }
    return signal;
}

SIGNAL* STABILIZER_MODEL::get_nonconst_pointer_of_signal_at_slot(unsigned int slot)
{
    SIGNAL* signal = nullptr;
    switch(slot)
    {
        case 0:
        {
            signal = &signal_0;
            break;
        }
        case 1:
        {
            signal = &signal_1;
            break;
        }
        case 2:
        {
            signal = &signal_2;
            break;
        }
        case 3:
        {
            signal = &signal_3;
            break;
        }
        case 4:
        {
            signal = &signal_4;
            break;
        }
        default:
            break;
    }
    return signal;
}

void STABILIZER_MODEL::set_input_signal_at_slot(unsigned int slot, SIGNAL& signal)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not signal.is_valid())
    {
        osstream<<"Warning. Invalid signal ("<<signal.get_meter_name()<<") is not allowed when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(slot>=STEPS_MAX_STABILIZER_INPUT_SIGNAL_SLOT)
    {
        osstream<<"Warning. Signal slot "<<slot<<" is beyond the capacity of slots when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    SIGNAL* signal_ptr = get_nonconst_pointer_of_signal_at_slot(slot);
    if(signal_ptr->is_valid())
    {
        osstream<<"Warning. Signal slot "<<slot<<" has already been assigned to signal "<<signal_ptr->get_meter_name()<<" when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<"."<<endl
                <<"It will be updated to new signal ("<<signal.get_meter_name()<<").";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    (*signal_ptr) = signal;
}

SIGNAL STABILIZER_MODEL::get_input_signal_at_slot(unsigned int slot) const
{
    SIGNAL signal(get_toolkit());
    if(slot<STEPS_MAX_STABILIZER_INPUT_SIGNAL_SLOT)
    {
        const SIGNAL* signal_ptr = get_const_pointer_of_signal_at_slot(slot);
        if(signal_ptr->is_valid())
            return *signal_ptr;
        else
            return signal;
    }
    else
        return signal;
}

bool STABILIZER_MODEL::is_slot_valid(unsigned int slot) const
{
    if(slot<STEPS_MAX_STABILIZER_INPUT_SIGNAL_SLOT)
    {
        const SIGNAL* signal_ptr = (const SIGNAL*) get_const_pointer_of_signal_at_slot(slot);
        if(signal_ptr->is_valid())
            return true;
        else
            return false;
    }
    else
        return false;
}

double STABILIZER_MODEL::get_signal_value_of_slot(unsigned int slot) const
{
    if(is_slot_valid(slot))
    {
        const SIGNAL* signal_ptr = get_const_pointer_of_signal_at_slot(slot);
        return signal_ptr->get_meter_value();
    }
    else
        return 0.0;
}

unsigned int STABILIZER_MODEL::convert_signal_type_string_to_number(string& signal_type) const
{
    signal_type = string2upper(signal_type);
    if(signal_type=="ROTOR SPEED DEVIATION IN PU")
        return 1;
    if(signal_type=="FREQUENCY DEVIATION IN PU")
        return 2;
    if(signal_type=="TERMINAL ACTIVE POWER IN PU ON MBASE")
        return 3;
    if(signal_type=="ACCELERATING POWER IN PU ON MBASE")
        return 4;
    if(signal_type=="VOLTAGE IN PU")
        return 5;
    if(signal_type=="ROCOV IN PU/S")
        return 6;
    if(signal_type=="MECHANICAL POWER IN PU ON MBASE")
        return 7;
    return INDEX_NOT_EXIST;
}

string STABILIZER_MODEL::convert_signal_type_number_to_string(unsigned int signal_type) const
{
    switch(signal_type)
    {
        case 1:
            return "ROTOR SPEED DEVIATION IN PU";
        case 2:
            return "FREQUENCY DEVIATION IN PU";
        case 3:
            return "TERMINAL ACTIVE POWER IN PU ON MBASE";
        case 4:
            return "ACCELERATING POWER IN PU ON MBASE";
        case 5:
            return "VOLTAGE IN PU";
        case 6:
            return "ROCOV IN PU/S";
        case 7:
            return "MECHANICAL POWER IN PU ON MBASE";
        default:
            return "";
    }
}

SIGNAL STABILIZER_MODEL::prepare_signal_with_signal_type_and_bus(unsigned int signal_type, unsigned int bus)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    SIGNAL signal(toolkit);

    if(bus!=0)
    {
        switch(signal_type)
        {
            case 1:
            {
                vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(bus);
                if(gens.size()!=0)
                {
                    GENERATOR* gen = gens[0];
                    DEVICE_ID did = gen->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 2:
            {
                BUS* busptr = psdb.get_bus(bus);
                if(busptr!=NULL)
                {
                    DEVICE_ID did = busptr->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 3:
            {
                vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(bus);
                if(gens.size()!=0)
                {
                    GENERATOR* gen = gens[0];
                    DEVICE_ID did = gen->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 4:
            {
                vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(bus);
                if(gens.size()!=0)
                {
                    GENERATOR* gen = gens[0];
                    DEVICE_ID did = gen->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 5:
            {
                BUS* busptr = psdb.get_bus(bus);
                if(busptr!=NULL)
                {
                    DEVICE_ID did = busptr->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 6:
            {
                BUS* busptr = psdb.get_bus(bus);
                if(busptr!=NULL)
                {
                    DEVICE_ID did = busptr->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 7:
            {
                vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(bus);
                if(gens.size()!=0)
                {
                    GENERATOR* gen = gens[0];
                    DEVICE_ID did = gen->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            default:
                break;
        }
    }
    return signal;
}

SIGNAL STABILIZER_MODEL::prepare_signal_with_signal_type_and_device_id(unsigned int signal_type, DEVICE_ID did)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    SIGNAL signal(toolkit);

    if(did.is_valid())
    {
        switch(signal_type)
        {
            case 1:
            case 3:
            case 4:
            case 7:
            {
                GENERATOR* gen = psdb.get_generator(did);
                if(gen!=NULL)
                {
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            case 2:
            case 5:
            case 6:
            {
                TERMINAL terminal= did.get_device_terminal();
                vector<unsigned int> buses = terminal.get_buses();
                unsigned int bus = buses[0];
                BUS* busptr = psdb.get_bus(bus);
                if(busptr!=NULL)
                {
                    DEVICE_ID did = busptr->get_device_id();
                    signal.set_device_id(did);
                    signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
                }
                break;
            }
            default:
                break;
        }
    }
    return signal;
}
