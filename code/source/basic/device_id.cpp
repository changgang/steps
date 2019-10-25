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
    set_device_identifier(did.get_device_identifier());
}

DEVICE_ID::~DEVICE_ID()
{
}

void DEVICE_ID::initialize_minimum_maximum_terminal_count()
{
    set_minimum_allowed_terminal_count(0);
    set_maximum_allowed_terminal_count(0);
}

void DEVICE_ID::set_minimum_allowed_terminal_count(size_t n)
{
    minimum_terminal_count = n;
}

void DEVICE_ID::set_maximum_allowed_terminal_count(size_t n)
{
    maximum_terminal_count = n;
}

void DEVICE_ID::set_device_type(string device_type)
{
    clear();

    set_device_type_and_allowed_terminal_count(device_type);
}

void DEVICE_ID::set_device_type_and_allowed_terminal_count(string device_type)
{
    device_type = string2upper(device_type);
    if(device_type=="BUS"         ||
       device_type=="GENERATOR"   ||
       device_type=="WT GENERATOR" ||
       device_type=="PV UNIT"   ||
       device_type=="ENERGY STORAGE"     ||
       device_type=="LOAD"        ||
       device_type=="FIXED SHUNT" ||
       device_type=="SWITCHED SHUNT" ||
       device_type=="EQUIVALENT DEVICE")
    {
        this->device_type = device_type;
        set_minimum_allowed_terminal_count(1);
        set_maximum_allowed_terminal_count(1);

        if(device_type == "BUS")
            allow_identifier = false;
        else
            allow_identifier = true;

        if(device_type=="BUS") device_type_code = 1;
        if(device_type_code==0 and device_type=="GENERATOR") device_type_code = 2;
        if(device_type_code==0 and device_type=="WT GENERATOR") device_type_code = 3;
        if(device_type_code==0 and device_type=="PV UNIT") device_type_code = 4;
        if(device_type_code==0 and device_type=="ENERGY STORAGE") device_type_code = 5;
        if(device_type_code==0 and device_type=="LOAD") device_type_code = 6;
        if(device_type_code==0 and device_type=="FIXED SHUNT") device_type_code = 7;
        if(device_type_code==0 and device_type=="SWITCHED SHUNT") device_type_code = 8;
        if(device_type_code==0 and device_type=="EQUIVALENT DEVICE") device_type_code = 9;

        return;
    }

    if(device_type=="LINE"    ||
       device_type=="HVDC"    ||
       device_type=="VSC HVDC" ||
       device_type=="FACTS")
    {
        this->device_type = device_type;
        set_minimum_allowed_terminal_count(2);
        set_maximum_allowed_terminal_count(2);

        allow_identifier = true;

        if(device_type=="LINE") device_type_code = 10;
        if(device_type_code==0 and device_type=="HVDC") device_type_code = 11;
        if(device_type_code==0 and device_type=="VSC HVDC") device_type_code = 12;
        if(device_type_code==0 and device_type=="FACTS") device_type_code = 13;

        return;
    }

    if(device_type=="TRANSFORMER")
    {
        this->device_type = device_type;
        set_minimum_allowed_terminal_count(2);
        set_maximum_allowed_terminal_count(3);
        allow_identifier = true;

        device_type_code = 14;

        return;
    }

    if(device_type=="MULTI DC")
    {
        this->device_type = device_type;
        set_minimum_allowed_terminal_count(3);
        set_maximum_allowed_terminal_count(100);
        allow_identifier = true;

        device_type_code = 15;

        return;
    }

    if(device_type=="GENERAL DEVICE")
    {
        this->device_type = device_type;
        set_minimum_allowed_terminal_count(0);
        set_maximum_allowed_terminal_count(100);
        allow_identifier = true;

        device_type_code = 16;

        return;
    }

    ostringstream osstream;
    osstream<<"Device type '"<<device_type<<"' is not supported when building DEVICE_ID object."<<endl
      <<"Allowed device types are: "<<endl
      <<"GENERATOR, WT GENERATOR, PV UNIT, ENERGY STORAGE, LOAD, FIXED SHUNT, SWITCHED SHUNT"<<endl
      <<"LINE, TRANSFORMER, HVDC, VSC HVDC, FACTS, MULTI DC, EQUIVALENT DEVICE, and GENERAL DEVICE."<<endl
      <<"Device type will be set as blank, and \"NONE\" will be returned if get_device_type() is called.";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    device_type = "";
}

void DEVICE_ID::set_device_terminal(const TERMINAL& term)
{
    if(is_given_terminal_acceptable(term))
        this->terminal = term;
    else
    {
        ostringstream osstream;
        osstream<<"Invalid terminal is provided for setting DEVICE_ID object. nothing is changed."<<endl
          <<"Possible device type is "<<get_device_type()<<".";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
}

bool DEVICE_ID::is_given_terminal_acceptable(const TERMINAL& term)
{
    size_t n_terminal = term.get_bus_count();
    if(n_terminal>=get_minimum_allowed_terminal_count() and
       n_terminal<=get_maximum_allowed_terminal_count())
        return true;
    else
        return false;
}

void DEVICE_ID::set_device_identifier(string identifier)
{
    if(allow_identifier)
    {
        identifier = trim_string(identifier);
        this->device_identifier = identifier;
    }
    else
        this->device_identifier = "";
}

string DEVICE_ID::get_device_type() const
{
    if(device_type != "")
        return device_type;
    else
        return "NONE";
}

size_t DEVICE_ID::get_device_type_code() const
{
    return device_type_code;
}

TERMINAL DEVICE_ID::get_device_terminal() const
{
    return terminal;
}

string DEVICE_ID::get_device_identifier() const
{
    if(allow_identifier)
        return device_identifier;
    else
        return "";
}

size_t DEVICE_ID::get_minimum_allowed_terminal_count() const
{
    return minimum_terminal_count;
}

size_t DEVICE_ID::get_maximum_allowed_terminal_count() const
{
    return maximum_terminal_count;
}

string DEVICE_ID::get_device_name() const
{
    string device_name = "INVALID DEVICE (possible of type "+get_device_type()+")";
    if(is_valid())
    {
        string name = get_device_type();
        string ident = get_device_identifier();
        TERMINAL term = get_device_terminal();
        vector<size_t> buses = term.get_buses();
        size_t bus_count = term.get_bus_count();
        if(name=="BUS")
        {
            device_name = name+" "+num2str(buses[0]);
            return device_name;
        }

        if(name=="LINE" or name=="HVDC" or name=="VSC HVDC")
        {
            if(ident=="")
                device_name = name + " ";
            else
                device_name = name + " "+ident+" ";
            device_name += "LINKING BUS "+num2str(buses[0])+" AND "+num2str(buses[1]);
            return device_name;
        }

        if(name=="TRANSFORMER")
        {
            if(ident=="")
                device_name = name + " ";
            else
                device_name = name + " "+ident+" ";
            if(bus_count==2)
                device_name += "LINKING BUS "+num2str(buses[0])+" AND "+num2str(buses[1]);
            else
                device_name += "LINKING BUS "+num2str(buses[0])+" "+num2str(buses[1])+" AND "+num2str(buses[2]);
            return device_name;
        }

        if(name=="GENERATOR" or name=="WT GENERATOR" or name=="PV UNIT" or name=="ENERGY STORAGE" or
           name=="LOAD" or name=="FIXED SHUNT" or name=="SWITCHED SHUNT" or name=="EQUIVALENT DEVICE")
        {
            if(ident=="")
                device_name = name + " ";
            else
                device_name = name + " "+ident+" ";
            device_name += "AT BUS "+num2str(buses[0]);
            return device_name;
        }

        if(name=="GENERAL DEVICE")
        {
            if(ident=="")
                device_name = name + " ";
            else
                device_name = name + " "+ident+" ";
            switch(bus_count)
            {
                case 0:
                    device_name += " AT SUPER CONTROL CENTER";
                    break;
                case 1:
                    device_name += " AT BUS "+num2str(buses[0]);
                    break;
                default:
                    device_name += " LINKING BUSES ("+num2str(buses[0]);
                    for(size_t i=1; i<bus_count; ++i)
                        device_name += ", "+num2str(buses[i]);
                    device_name += ")";
                    break;
            }
            return device_name;
        }
        return device_name;
    }
    else
        return device_name;
}

bool DEVICE_ID::is_valid() const
{
    //if(get_device_type()=="GENERAL DEVICE" or (get_device_type()!="" and terminal.get_bus_count()!=0))
    size_t device_type_code = get_device_type_code();
    if(device_type_code==16 or (device_type_code!=0 and terminal.get_bus_count()!=0))
        return true;
    else
        return false;
}

void DEVICE_ID::clear()
{
    device_type = "";
    device_type_code = 0;
    device_identifier = "";
    terminal.clear();
    initialize_minimum_maximum_terminal_count();
    allow_identifier = false;
}

DEVICE_ID& DEVICE_ID::operator= (const DEVICE_ID& device_id)
{
    if(this==(&device_id)) return *this;

    set_device_type(device_id.get_device_type());
    set_device_terminal(device_id.get_device_terminal());
    set_device_identifier(device_id.get_device_identifier());

    return (*this);
}

bool DEVICE_ID::operator< (const DEVICE_ID& device_id) const
{
    if(this->get_device_type_code() == device_id.get_device_type_code())
    {
        TERMINAL this_terminal = this->get_device_terminal();
        TERMINAL to_compare_terminal = device_id.get_device_terminal();

        vector<size_t> this_buses = this_terminal.get_buses();
        vector<size_t> to_compare_buses = to_compare_terminal.get_buses();

        size_t this_size = this_buses.size();
        size_t to_compare_size = to_compare_buses.size();

        size_t max_size = max(this_size, to_compare_size);

        this_buses.resize(max_size, 0);
        to_compare_buses.resize(max_size, 0);

        int compare_sign = 0;

        for(size_t i=0; compare_sign==0 and i!=max_size; ++i)
            compare_sign = (int)this_buses[i]-(int)to_compare_buses[i];

        if(compare_sign!=0)
            return compare_sign<0;
        else
        {
            string this_identifier = this->get_device_identifier();
            string to_compare_identifier = device_id.get_device_identifier();

            compare_sign = (this_identifier<to_compare_identifier?-1:(this_identifier>to_compare_identifier?1:0));
            return compare_sign<0;
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


DEVICE_ID get_bus_device_id(size_t bus_number)
{
    DEVICE_ID did;
    did.set_device_type("BUS");

    TERMINAL terminal;
    terminal.append_bus(bus_number);

    did.set_device_terminal(terminal);

    return did;
}

DEVICE_ID get_generator_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("GENERATOR");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_wt_generator_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_pv_unit_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("PV UNIT");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_energy_storage_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("ENERGY STORAGE");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_load_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("LOAD");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_fixed_shunt_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("FIXED SHUNT");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_line_device_id(size_t ibus, size_t jbus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("LINE");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_hvdc_device_id(size_t ibus, size_t jbus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("HVDC");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_transformer_device_id(size_t ibus, size_t jbus, size_t kbus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");

    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    terminal.append_bus(kbus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}

DEVICE_ID get_equivalent_device_id(size_t bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("EQUIVALENT DEVICE");

    TERMINAL terminal;
    terminal.append_bus(bus);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}
DEVICE_ID get_general_device_id(const vector<size_t>& bus, const string& identifier)
{
    DEVICE_ID did;
    did.set_device_type("GENERAL DEVICE");

    TERMINAL terminal;
    size_t n = bus.size();
    for(size_t i=0; i<n; ++i)
        terminal.append_bus(bus[i]);

    did.set_device_terminal(terminal);

    did.set_device_identifier(identifier);

    return did;
}
