#include "header/basic/dc_device_id.h"
#include "header/basic/exception.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

DC_DEVICE_ID::DC_DEVICE_ID()
{
    clear();
}
DC_DEVICE_ID::DC_DEVICE_ID(const DC_DEVICE_ID& did)
{
    clear();
    set_device_type(did.get_device_type());
    set_device_terminal(did.get_device_terminal());
    set_device_name_index(did.get_device_name_index());
    set_device_identifier_index(did.get_device_identifier_index());
}

DC_DEVICE_ID::~DC_DEVICE_ID()
{
}

void DC_DEVICE_ID::enable_allow_terminal()
{
    allow_terminal = true;
    allow_name = false;
}

void DC_DEVICE_ID::enable_allow_name()
{
    allow_name = true;
    allow_terminal = false;
    allow_identifier = false;
}

void DC_DEVICE_ID::enable_allow_identifier()
{
    if(allow_terminal)
        allow_identifier = true;
}

void DC_DEVICE_ID::disable_allow_identifier()
{
    allow_identifier = false;
}

void DC_DEVICE_ID::initialize_minimum_maximum_terminal_count()
{
    set_minimum_allowed_terminal_count(0);
    set_maximum_allowed_terminal_count(0);
}

void DC_DEVICE_ID::set_minimum_allowed_terminal_count(unsigned int n)
{
    minimum_terminal_count = n;
}

void DC_DEVICE_ID::set_maximum_allowed_terminal_count(unsigned int n)
{
    maximum_terminal_count = n;
}

void DC_DEVICE_ID::set_device_type(STEPS_DEVICE_TYPE device_type)
{
    if(device_type== STEPS_DC_BUS or device_type==STEPS_DC_LINE)
    {
        clear();
        set_device_type_and_allowed_terminal_count(device_type);
    }
}

void DC_DEVICE_ID::set_device_type_and_allowed_terminal_count(STEPS_DEVICE_TYPE device_type)
{
    /*
    device type code:
    0: invalid
    <=1000: terminal + identifier
    1001~2000: name
    9999: general
    */

    if(device_type==STEPS_DC_BUS)
    {
        enable_allow_terminal();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(1);
        set_maximum_allowed_terminal_count(1);
        disable_allow_identifier();

        return;
    }

    if(device_type==STEPS_DC_LINE)
    {
        enable_allow_terminal();
        enable_allow_identifier();

        this->device_type = device_type;
        set_minimum_allowed_terminal_count(2);
        set_maximum_allowed_terminal_count(2);

        return;
    }

    ostringstream osstream;
    osstream<<"Device type '"<<device_type<<"' is not supported when building DC_DEVICE_ID object."<<endl
            <<"Allowed device types are: "<<endl
            <<"DC_BUS, DC_LINE."<<endl
            <<"Device type will be set as blank, and \"NONE\" will be returned if get_device_type() is called.";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    device_type = STEPS_INVALID_DEVICE;
}

void DC_DEVICE_ID::set_device_terminal(const TERMINAL& term)
{
    if(allow_terminal)
    {
        if(is_given_terminal_acceptable(term))
            this->terminal = term;
        else
        {
            ostringstream osstream;
            osstream<<"Invalid terminal is provided for setting DC_DEVICE_ID object. nothing is changed."<<endl
                    <<"Possible device type is "<<get_device_type()<<"."<<endl
                    <<"Given terminal is: "<<term.convert2string()<<".";
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        }
    }
}

bool DC_DEVICE_ID::is_given_terminal_acceptable(const TERMINAL& term)
{
    unsigned int n_terminal = term.get_bus_count();
    if(n_terminal>=get_minimum_allowed_terminal_count() and
       n_terminal<=get_maximum_allowed_terminal_count())
        return true;
    else
        return false;
}

void DC_DEVICE_ID::set_device_identifier_index(unsigned int index){
    if(allow_identifier)
    {
        this->device_identifier_index = index;
    }
}

void DC_DEVICE_ID::set_device_name_index(unsigned int index)
{
    if(allow_name)
    {
        this->device_name_index = index;
    }
}

STEPS_DEVICE_TYPE DC_DEVICE_ID::get_device_type() const
{
    return device_type;
}

TERMINAL DC_DEVICE_ID::get_device_terminal() const
{
    return terminal;
}

string DC_DEVICE_ID::get_device_identifier() const
{
    if(allow_identifier)
        return get_string_of_index(device_identifier_index);
    else
        return "";
}

string DC_DEVICE_ID::get_device_name() const
{
    if(allow_name)
        return get_string_of_index(device_name_index);
    else
        return "";
}

unsigned int DC_DEVICE_ID::get_device_identifier_index() const
{
    return device_identifier_index;
}

unsigned int DC_DEVICE_ID::get_device_name_index() const
{
    return device_name_index;
}

unsigned int DC_DEVICE_ID::get_minimum_allowed_terminal_count() const
{
    return minimum_terminal_count;
}

unsigned int DC_DEVICE_ID::get_maximum_allowed_terminal_count() const
{
    return maximum_terminal_count;
}

string DC_DEVICE_ID::get_compound_device_name() const
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
        if(device_type==STEPS_DC_BUS)
        {
            comp_device_name = device_type_str+" "+num2str(buses[0]);
            return comp_device_name;
        }

        if(device_type==STEPS_DC_LINE)
        {
            if(ident=="")
                comp_device_name = device_type_str + " ";
            else
                comp_device_name = device_type_str + " "+ident+" ";
            comp_device_name += "LINKING BUS "+num2str(buses[0])+" AND "+num2str(buses[1]);
            return comp_device_name;
        }
        return comp_device_name;
    }
    else
        return comp_device_name;
}

bool DC_DEVICE_ID::is_valid() const
{
    if((is_name_allowed() and device_name_index != 0) or
       (is_terminal_allowed() and terminal.get_bus_count()!=0))
        return true;
    else
        return false;
}


bool DC_DEVICE_ID::is_name_allowed() const
{
    return allow_name;
}

bool DC_DEVICE_ID::is_terminal_allowed() const
{
    return allow_terminal;
}

bool DC_DEVICE_ID::is_identifier_allowed() const
{
    return allow_identifier;
}

void DC_DEVICE_ID::clear()
{
    device_type = STEPS_INVALID_DEVICE;
    device_identifier_index = 0;
    device_name_index = 0;
    terminal.clear();
    initialize_minimum_maximum_terminal_count();
    allow_identifier = false;
    allow_name = false;
}

DC_DEVICE_ID& DC_DEVICE_ID::operator= (const DC_DEVICE_ID& DC_DEVICE_ID)
{
    if(this==(&DC_DEVICE_ID)) return *this;

    set_device_type(DC_DEVICE_ID.get_device_type());
    set_device_terminal(DC_DEVICE_ID.get_device_terminal());
    set_device_identifier_index(DC_DEVICE_ID.get_device_identifier_index());
    set_device_name_index(DC_DEVICE_ID.get_device_name_index());

    return (*this);
}

bool DC_DEVICE_ID::operator< (const DC_DEVICE_ID& DC_DEVICE_ID) const
{
    if(this->get_device_type() == DC_DEVICE_ID.get_device_type())
    {
        if(allow_name)
        {
            if(this->get_device_name()<DC_DEVICE_ID.get_device_name())
                return true;
            else
                return false;
        }
        else
        {
            TERMINAL this_terminal = this->get_device_terminal();
            TERMINAL to_compare_terminal = DC_DEVICE_ID.get_device_terminal();

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
                    string to_compare_identifier = DC_DEVICE_ID.get_device_identifier();

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

bool DC_DEVICE_ID::operator==(const DC_DEVICE_ID& DC_DEVICE_ID) const
{
    if(this->get_device_type()==DC_DEVICE_ID.get_device_type())
    {
        if((*this)<DC_DEVICE_ID or DC_DEVICE_ID<(*this))
            return false;
        else
            return true;
    }
    else
        return false;
}

bool DC_DEVICE_ID::operator!=(const DC_DEVICE_ID& DC_DEVICE_ID) const
{
    if(this->get_device_type()==DC_DEVICE_ID.get_device_type())
        return not((*this)==DC_DEVICE_ID);
    else
        return false;
}


DC_DEVICE_ID get_dc_bus_device_id(unsigned int bus_number)
{
    DC_DEVICE_ID did;
    did.set_device_type(STEPS_DC_BUS);

    TERMINAL terminal;
    terminal.append_bus(bus_number);

    did.set_device_terminal(terminal);

    return did;
}

DC_DEVICE_ID get_dc_line_device_id(unsigned int ibus, unsigned int jbus, const string identifier)
{
    DC_DEVICE_ID did;
    did.set_device_type(STEPS_DC_LINE);

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier_index(get_index_of_string(identifier));

    return did;
}
