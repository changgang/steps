#include "header/basic/device_id.h"
#include "header/basic/exception.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

DEVICE_ID::DEVICE_ID()
{
    clear();
}
DEVICE_ID::DEVICE_ID(const DEVICE_ID& did)
{
    clear();
    set_device_type(did.get_device_type());
    set_device_terminal(did.get_device_terminal());
    set_device_name_index(did.get_device_name_index());
    set_device_identifier_index(did.get_device_identifier_index());
}

DEVICE_ID::~DEVICE_ID()
{
}

void DEVICE_ID::enable_allow_terminal()
{
    allow_terminal = true;
    allow_name = false;
}

void DEVICE_ID::enable_allow_name()
{
    allow_name = true;
    allow_terminal = false;
    allow_identifier = false;
}

void DEVICE_ID::enable_allow_identifier()
{
    if(allow_terminal)
        allow_identifier = true;
}

void DEVICE_ID::disable_allow_identifier()
{
    allow_identifier = false;
}

void DEVICE_ID::initialize_minimum_maximum_terminal_count()
{
    set_minimum_allowed_terminal_count(0);
    set_maximum_allowed_terminal_count(0);
}

void DEVICE_ID::set_minimum_allowed_terminal_count(unsigned int n)
{
    minimum_terminal_count = n;
}

void DEVICE_ID::set_maximum_allowed_terminal_count(unsigned int n)
{
    maximum_terminal_count = n;
}

void DEVICE_ID::set_device_type(STEPS_DEVICE_TYPE device_type)
{
    clear();

    set_device_type_and_allowed_terminal_count(device_type);
}

void DEVICE_ID::set_device_type_and_allowed_terminal_count(STEPS_DEVICE_TYPE device_type)
{
    if(device_type==STEPS_BUS         ||
       device_type==STEPS_GENERATOR   ||
       device_type==STEPS_WT_GENERATOR ||
       device_type==STEPS_PV_UNIT   ||
       device_type==STEPS_ENERGY_STORAGE     ||
       device_type==STEPS_LOAD        ||
       device_type==STEPS_FIXED_SHUNT ||
       device_type==STEPS_SWITCHED_SHUNT ||
       device_type==STEPS_EQUIVALENT_DEVICE)
    {
        enable_allow_terminal();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(1);
        set_maximum_allowed_terminal_count(1);

        if(device_type == STEPS_BUS)
            disable_allow_identifier();
        else
            enable_allow_identifier();

        return;
    }

    if(device_type==STEPS_AC_LINE    ||
       device_type==STEPS_LCC_HVDC2T    ||
       device_type==STEPS_FACTS)
    {
        enable_allow_terminal();
        enable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(2);
        set_maximum_allowed_terminal_count(2);

        return;
    }

    if(device_type==STEPS_TRANSFORMER)
    {
        enable_allow_terminal();
        enable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(2);
        set_maximum_allowed_terminal_count(3);

        return;
    }

    if(device_type==STEPS_VSC_HVDC)
    {
        enable_allow_terminal();
        enable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(2);
        set_maximum_allowed_terminal_count(100);

        return;
    }

    if(device_type==STEPS_HYBRID_DC)
    {
        enable_allow_terminal();
        enable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(3);
        set_maximum_allowed_terminal_count(100);

        return;
    }

    if(device_type==STEPS_LCC_HVDC)
    {
        enable_allow_name();
        disable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(0);
        set_maximum_allowed_terminal_count(0);

        return;
    }

    if(device_type==STEPS_GENERAL_DEVICE)
    {
        enable_allow_terminal();
        enable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(0);
        set_maximum_allowed_terminal_count(100);

        return;
    }

    ostringstream osstream;
    osstream<<"Device type '"<<device_type<<"' is not supported when building DEVICE_ID object."<<endl
            <<"Allowed device types are: "<<endl
            <<"GENERATOR, WT GENERATOR, PV UNIT, ENERGY STORAGE, LOAD, FIXED SHUNT, SWITCHED SHUNT"<<endl
            <<"AC LINE, TRANSFORMER, 2T LCC HVDC, VSC HVDC, FACTS, LCC HVDC, HYBRID DC, EQUIVALENT DEVICE, and GENERAL DEVICE."<<endl
            <<"Device type will be set as blank, and \"NONE\" will be returned if get_device_type() is called.";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    device_type = STEPS_INVALID_DEVICE;
}

void DEVICE_ID::set_device_terminal(const TERMINAL& term)
{
    if(allow_terminal)
    {
        if(is_given_terminal_acceptable(term))
            this->terminal = term;
        else
        {
            ostringstream osstream;
            osstream<<"Invalid terminal is provided for setting DEVICE_ID object. nothing is changed."<<endl
                    <<"Possible device type is "<<get_device_type()<<"."<<endl
                    <<"Given terminal is: "<<term.convert2string()<<".";
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        }
    }
}

bool DEVICE_ID::is_given_terminal_acceptable(const TERMINAL& term)
{
    unsigned int n_terminal = term.get_bus_count();
    if(n_terminal>=get_minimum_allowed_terminal_count() and
       n_terminal<=get_maximum_allowed_terminal_count())
        return true;
    else
        return false;
}

/*
void DEVICE_ID::set_device_identifier(string identifier)
{
    if(allow_identifier)
    {
        identifier = trim_string(identifier);
        add_string_to_str_int_map(identifier);
        this->device_identifier_index = get_index_of_string(identifier);
    }
    else
        this->device_identifier_index = get_index_of_string("");
}

void DEVICE_ID::set_device_name(string name)
{
    if(allow_name)
    {
        name = trim_string(name);
        add_string_to_str_int_map(name);
        this->device_name_index = get_index_of_string(name);
    }
    else
        this->device_name_index = get_index_of_string("");
}
*/

void DEVICE_ID::set_device_identifier_index(unsigned int index){
    if(allow_identifier)
    {
        this->device_identifier_index = index;
    }
}

void DEVICE_ID::set_device_name_index(unsigned int index)
{
    if(allow_name)
    {
        this->device_name_index = index;
    }
}

STEPS_DEVICE_TYPE DEVICE_ID::get_device_type() const
{
    return device_type;
}

TERMINAL DEVICE_ID::get_device_terminal() const
{
    return terminal;
}

string DEVICE_ID::get_device_identifier() const
{
    if(allow_identifier)
        return get_string_of_index(device_identifier_index);
    else
        return "";
}

string DEVICE_ID::get_device_name() const
{
    if(allow_name)
        return get_string_of_index(device_name_index);
    else
        return "";
}

unsigned int DEVICE_ID::get_device_identifier_index() const
{
    return device_identifier_index;
}

unsigned int DEVICE_ID::get_device_name_index() const
{
    return device_name_index;
}

unsigned int DEVICE_ID::get_minimum_allowed_terminal_count() const
{
    return minimum_terminal_count;
}

unsigned int DEVICE_ID::get_maximum_allowed_terminal_count() const
{
    return maximum_terminal_count;
}

string DEVICE_ID::get_compound_device_name() const
{
    string comp_device_name = "INVALID DEVICE (possible of type "+num2str(get_device_type())+")";
    if(is_valid())
    {
        STEPS_DEVICE_TYPE device_type = get_device_type();
        string device_type_str = device_type2string(device_type);
        string ident = get_device_identifier();
        string device_name = get_device_name();
        TERMINAL term = get_device_terminal();
        vector<unsigned int> buses = term.get_buses();
        unsigned int bus_count = term.get_bus_count();
        if(device_type==STEPS_BUS)
        {
            comp_device_name = device_type_str+" "+num2str(buses[0]);
            return comp_device_name;
        }

        if(device_type==STEPS_AC_LINE or device_type==STEPS_LCC_HVDC2T)
        {
            if(ident=="")
                comp_device_name = device_type_str + " ";
            else
                comp_device_name = device_type_str + " "+ident+" ";
            comp_device_name += "LINKING BUS "+num2str(buses[0])+" AND "+num2str(buses[1]);
            return comp_device_name;
        }

        if(device_type==STEPS_VSC_HVDC)
        {
            if(ident=="")
                comp_device_name = device_type_str + " ";
            else
                comp_device_name = device_type_str + " "+ident+" ";
            comp_device_name += "LINKING BUS ";
            for(unsigned int i=0; i<bus_count-1; ++i)
                comp_device_name +=num2str(buses[i])+" ";
            comp_device_name +="AND "+num2str(buses[bus_count-1]);

            return comp_device_name;
        }

        if(device_type==STEPS_TRANSFORMER)
        {
            if(ident=="")
                comp_device_name = device_type_str + " ";
            else
                comp_device_name = device_type_str + " "+ident+" ";
            if(bus_count==2)
                comp_device_name += "LINKING BUS "+num2str(buses[0])+" AND "+num2str(buses[1]);
            else
                comp_device_name += "LINKING BUS "+num2str(buses[0])+" "+num2str(buses[1])+" AND "+num2str(buses[2]);
            return comp_device_name;
        }

        if(device_type==STEPS_GENERATOR or device_type==STEPS_WT_GENERATOR or device_type==STEPS_PV_UNIT or
           device_type==STEPS_ENERGY_STORAGE or device_type==STEPS_LOAD or device_type==STEPS_FIXED_SHUNT or
           device_type==STEPS_SWITCHED_SHUNT or device_type==STEPS_EQUIVALENT_DEVICE)
        {
            if(ident=="")
                comp_device_name = device_type_str + " ";
            else
                comp_device_name = device_type_str + " "+ident+" ";
            comp_device_name += "AT BUS "+num2str(buses[0]);
            return comp_device_name;
        }

        if(device_type==STEPS_LCC_HVDC)
        {
            if(device_name=="")
                comp_device_name = device_type_str + " INVALID";
            else
                comp_device_name = device_type_str + " "+device_name;

            return comp_device_name;
        }

        if(device_type==STEPS_GENERAL_DEVICE)
        {
            if(ident=="")
                comp_device_name = device_type_str + " ";
            else
                comp_device_name = device_type_str + " "+ident+" ";
            switch(bus_count)
            {
                case 0:
                    comp_device_name += " AT SUPER CONTROL CENTER";
                    break;
                case 1:
                    comp_device_name += " AT BUS "+num2str(buses[0]);
                    break;
                default:
                    comp_device_name += " LINKING BUSES ("+num2str(buses[0]);
                    for(unsigned int i=1; i<bus_count; ++i)
                        comp_device_name += ", "+num2str(buses[i]);
                    comp_device_name += ")";
                    break;
            }
            return comp_device_name;
        }
        return comp_device_name;
    }
    else
        return comp_device_name;
}

bool DEVICE_ID::is_valid() const
{
    if((is_name_allowed() and device_name_index != 0) or
       (is_terminal_allowed() and ((device_type != STEPS_GENERAL_DEVICE and terminal.get_bus_count()!=0) or
                                   device_type == STEPS_GENERAL_DEVICE)))
        return true;
    else
        return false;
}


bool DEVICE_ID::is_name_allowed() const
{
    return allow_name;
}

bool DEVICE_ID::is_terminal_allowed() const
{
    return allow_terminal;
}

bool DEVICE_ID::is_identifier_allowed() const
{
    return allow_identifier;
}

void DEVICE_ID::clear()
{
    device_type = STEPS_INVALID_DEVICE;
    device_identifier_index = 0;
    device_name_index = 0;
    terminal.clear();
    initialize_minimum_maximum_terminal_count();
    allow_identifier = false;
    allow_name = false;
}

DEVICE_ID& DEVICE_ID::operator= (const DEVICE_ID& device_id)
{
    if(this==(&device_id)) return *this;

    set_device_type(device_id.get_device_type());
    set_device_terminal(device_id.get_device_terminal());
    set_device_identifier_index(device_id.get_device_identifier_index());
    set_device_name_index(device_id.get_device_name_index());

    return (*this);
}

bool DEVICE_ID::operator< (const DEVICE_ID& device_id) const
{
    if(this->get_device_type() == device_id.get_device_type())
    {
        if(allow_name)
        {
            if(this->get_device_name()<device_id.get_device_name())
                return true;
            else
                return false;
        }
        else
        {
            TERMINAL this_terminal = this->get_device_terminal();
            TERMINAL to_compare_terminal = device_id.get_device_terminal();

            vector<unsigned int> this_buses = this_terminal.get_buses();
            vector<unsigned int> to_compare_buses = to_compare_terminal.get_buses();

            unsigned int this_size = this_buses.size();
            unsigned int to_compare_size = to_compare_buses.size();

            unsigned int max_size = max(this_size, to_compare_size);

            this_buses.resize(max_size, 0);
            to_compare_buses.resize(max_size, 0);

            int compare_sign = 0;

            for(unsigned int i=0; compare_sign==0 and i!=max_size; ++i)
                compare_sign = (int)this_buses[i]-(int)to_compare_buses[i];

            if(compare_sign!=0)
                return compare_sign<0;
            else
            {
                if(allow_identifier)
                {
                    string this_identifier = this->get_device_identifier();
                    string to_compare_identifier = device_id.get_device_identifier();

                    compare_sign = (this_identifier<to_compare_identifier?-1:(this_identifier>to_compare_identifier?1:0));
                    return compare_sign<0;
                }
                else
                    return false;
            }
        }
    }
    else// of different types, no comparison
        return false;
}

bool DEVICE_ID::operator==(const DEVICE_ID& device_id) const
{
    if(this->get_device_type()==device_id.get_device_type())
    {
        if((*this)<device_id or device_id<(*this))
            return false;
        else
            return true;
    }
    else
        return false;
}

bool DEVICE_ID::operator!=(const DEVICE_ID& device_id) const
{
    if(this->get_device_type()==device_id.get_device_type())
        return not((*this)==device_id);
    else
        return false;
}


DEVICE_ID get_bus_device_id(unsigned int bus_number)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_BUS);

    TERMINAL terminal;
    terminal.append_bus(bus_number);

    did.set_device_terminal(terminal);

    return did;
}

DEVICE_ID get_generator_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_GENERATOR);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_wt_generator_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_WT_GENERATOR);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_pv_unit_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_PV_UNIT);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_energy_storage_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_ENERGY_STORAGE);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_load_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_LOAD);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_fixed_shunt_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_FIXED_SHUNT);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_ac_line_device_id(unsigned int ibus, unsigned int jbus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_AC_LINE);

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_2t_lcc_hvdc_device_id(unsigned int ibus, unsigned int jbus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_LCC_HVDC2T);

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_vsc_hvdc_device_id(const vector<unsigned int>& bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_VSC_HVDC);

    TERMINAL terminal;
    unsigned int nbus = bus.size();
    for(unsigned int i=0; i!=nbus; ++i)
        terminal.append_bus(bus[i]);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_transformer_device_id(unsigned int ibus, unsigned int jbus, unsigned int kbus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_TRANSFORMER);

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    terminal.append_bus(kbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_equivalent_device_id(unsigned int bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_EQUIVALENT_DEVICE);

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}

DEVICE_ID get_lcc_hvdc_device_id(const string& name)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_LCC_HVDC);
    did.set_device_name_index(get_index_of_string(name));
    return did;
}

DEVICE_ID get_general_device_id(const vector<unsigned int>& bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type(STEPS_GENERAL_DEVICE);

    TERMINAL terminal;
    unsigned int n = bus.size();
    for(unsigned int i=0; i<n; ++i)
        terminal.append_bus(bus[i]);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}
