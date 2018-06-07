#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include <cstdio>
#include "header/basic/utility.h"
STABILIZER_MODEL::STABILIZER_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
    for(size_t slot=0; slot!=MAX_STABILIZER_INPUT_SIGNAL_SLOT; slot++)
        signals[slot].clear();
}

STABILIZER_MODEL::~STABILIZER_MODEL()
{
    for(size_t slot=0; slot!=MAX_STABILIZER_INPUT_SIGNAL_SLOT; slot++)
        signals[slot].clear();
}

string STABILIZER_MODEL::get_model_type() const
{
    return "STABILIZER";
}

void STABILIZER_MODEL::set_input_signal_at_slot(size_t slot, SIGNAL& signal)
{
    ostringstream osstream;
    if(not signal.is_valid())
    {
        osstream<<"Warning. Invalid signal ("<<signal.get_meter_name()<<") is not allowed when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(slot>=MAX_STABILIZER_INPUT_SIGNAL_SLOT)
    {
        osstream<<"Warning. Signal slot "<<slot<<" is beyond the capacity of slots when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<".";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    if(signals[slot].is_valid())
    {
        osstream<<"Warning. Signal slot "<<slot<<" has already been assigned to signal "<<signals[slot].get_meter_name()<<" when setting up "<<get_model_type()<<" model '"<<get_model_name()<<"' for "<<get_device_name()<<"."<<endl
          <<"It will be deleted and new signal ("<<signal.get_meter_name()<<") will be set.";
        show_information_with_leading_time_stamp(osstream);
        signals[slot].clear();
    }
    signals[slot] = signal;
}

SIGNAL STABILIZER_MODEL::get_input_signal_at_slot(size_t slot) const
{
    SIGNAL signal;
    if(slot>=MAX_STABILIZER_INPUT_SIGNAL_SLOT)
        return signal;
    else
        return signals[slot];
}

bool STABILIZER_MODEL::is_slot_valid(size_t slot) const
{
    if(slot>=MAX_STABILIZER_INPUT_SIGNAL_SLOT)
        return false;
    else
    {
        if(signals[slot].is_valid())
            return true;
        else
            return false;
    }
}

double STABILIZER_MODEL::get_signal_value_of_slot(size_t slot) const
{
    if(not this->is_slot_valid(slot))
        return 0.0;

    SIGNAL signal = signals[slot];
    return signal.get_meter_value();
}

size_t STABILIZER_MODEL::convert_signal_type_string_to_number(string signal_type) const
{
    signal_type = string2upper(signal_type);
    if(signal_type=="GENERATOR ROTOR SPEED DEVIATION IN PU")
        return 1;
    if(signal_type=="BUS FREQUENCY DEVIATION IN PU")
        return 2;
    if(signal_type=="GENERATOR TERMINAL ACTIVE POWER IN PU ON MBASE")
        return 3;
    if(signal_type=="GENERATOR ACCELERATING POWER IN PU ON MBASE")
        return 4;
    if(signal_type=="BUS VOLTAGE IN PU")
        return 5;
    if(signal_type=="BUS ROCOV IN PU/S")
        return 6;
    return INDEX_NOT_EXIST;
}

string STABILIZER_MODEL::convert_signal_type_number_to_string(size_t signal_type) const
{
    switch(signal_type)
    {
        case 1:
            return "GENERATOR ROTOR SPEED DEVIATION IN PU";
        case 2:
            return "BUS FREQUENCY DEVIATION IN PU";
        case 3:
            return "GENERATOR TERMINAL ACTIVE POWER IN PU ON MBASE";
        case 4:
            return "GENERATOR ACCELERATING POWER IN PU ON MBASE";
        case 5:
            return "BUS VOLTAGE IN PU";
        case 6:
            return "BUS ROCOV IN PU/S";
        default:
            return "";
    }
}

SIGNAL STABILIZER_MODEL::prepare_signal_with_signal_type_and_bus(size_t signal_type, size_t bus)
{
    SIGNAL signal;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
        return signal;

    if(bus==0)
        return signal;

    switch(signal_type)
    {
        case 1:
        {
            vector<GENERATOR*> gens = psdb->get_generators_connecting_to_bus(bus);
            if(gens.size()==0)
                break;
            GENERATOR* gen = gens[0];
            DEVICE_ID did = gen->get_device_id();
            signal.set_device_id(did);
            signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
            break;
        }
        case 2:
        {
            BUS* busptr = psdb->get_bus(bus);
            if(busptr==NULL)
                break;
            DEVICE_ID did = busptr->get_device_id();
            signal.set_device_id(did);
            signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
            break;
        }
        case 3:
        {
            vector<GENERATOR*> gens = psdb->get_generators_connecting_to_bus(bus);
            if(gens.size()==0)
                break;
            GENERATOR* gen = gens[0];
            DEVICE_ID did = gen->get_device_id();
            signal.set_device_id(did);
            signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
            break;
        }
        case 4:
        {
            vector<GENERATOR*> gens = psdb->get_generators_connecting_to_bus(bus);
            if(gens.size()==0)
                break;
            GENERATOR* gen = gens[0];
            DEVICE_ID did = gen->get_device_id();
            signal.set_device_id(did);
            signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
            break;
        }
        case 5:
        {
            BUS* busptr = psdb->get_bus(bus);
            if(busptr==NULL)
                break;
            DEVICE_ID did = busptr->get_device_id();
            signal.set_device_id(did);
            signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
            break;
        }
        case 6:
        {
            BUS* busptr = psdb->get_bus(bus);
            if(busptr==NULL)
                break;
            DEVICE_ID did = busptr->get_device_id();
            signal.set_device_id(did);
            signal.set_meter_type(convert_signal_type_number_to_string(signal_type));
            break;
        }
        default:
            break;
    }
    return signal;
}
