#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include <cstdio>
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>

using namespace std;
STABILIZER_MODEL::STABILIZER_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
    for(size_t slot=0; slot!=MAX_STABILIZER_INPUT_SIGNAL_SLOT; ++slot)
        signals[slot] = new SIGNAL;
}

STABILIZER_MODEL::~STABILIZER_MODEL()
{
    for(size_t slot=0; slot!=MAX_STABILIZER_INPUT_SIGNAL_SLOT; ++slot)
        delete signals[slot];
}

string STABILIZER_MODEL::get_model_type() const
{
    return "STABILIZER";
}

void STABILIZER_MODEL::set_input_signal_at_slot(size_t slot, SIGNAL& signal)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(not signal.is_valid())
    {
        osstream<<"Warning. Invalid signal ("<<signal.get_meter_name()<<") is not allowed when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(slot>=MAX_STABILIZER_INPUT_SIGNAL_SLOT)
    {
        osstream<<"Warning. Signal slot "<<slot<<" is beyond the capacity of slots when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(signals[slot]->is_valid())
    {
        osstream<<"Warning. Signal slot "<<slot<<" has already been assigned to signal "<<signals[slot]->get_meter_name()<<" when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<"."<<endl
          <<"It will be deleted and new signal ("<<signal.get_meter_name()<<") will be set.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        signals[slot]->clear();
    }
    (*signals[slot]) = signal;
}

SIGNAL STABILIZER_MODEL::get_input_signal_at_slot(size_t slot) const
{
    SIGNAL signal;
    if(slot<MAX_STABILIZER_INPUT_SIGNAL_SLOT)
        return (*signals[slot]);
    else
        return signal;
}

bool STABILIZER_MODEL::is_slot_valid(size_t slot) const
{
    if(slot<MAX_STABILIZER_INPUT_SIGNAL_SLOT)
    {
        if(signals[slot]->is_valid())
            return true;
        else
            return false;
    }
    else
        return false;
}

double STABILIZER_MODEL::get_signal_value_of_slot(size_t slot) const
{
    if(this->is_slot_valid(slot))
    {
        SIGNAL *signal = signals[slot];
        return signal->get_meter_value();
    }
    else
        return 0.0;
}

size_t STABILIZER_MODEL::convert_signal_type_string_to_number(string& signal_type) const
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

string STABILIZER_MODEL::convert_signal_type_number_to_string(size_t signal_type) const
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

SIGNAL STABILIZER_MODEL::prepare_signal_with_signal_type_and_bus(size_t signal_type, size_t bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    SIGNAL signal;
    signal.set_toolkit(toolkit);

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

SIGNAL STABILIZER_MODEL::prepare_signal_with_signal_type_and_device_id(size_t signal_type, DEVICE_ID did)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    SIGNAL signal;
    signal.set_toolkit(toolkit);

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
                vector<size_t> buses = terminal.get_buses();
                size_t bus = buses[0];
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
