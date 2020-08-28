#include "header/device/converter.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

CONVERTER::CONVERTER(STEPS& toolkit)
{
    clear();
    set_toolkit(toolkit);
}

CONVERTER::~CONVERTER() { ; }

void CONVERTER::set_toolkit(STEPS& toolkit) { this->toolkit = (&toolkit); }
STEPS& CONVERTER::get_toolkit() const { return *toolkit; }

void CONVERTER::clear()
{
    bus = 0;
    busptr = nullptr;
    set_valve_side_bus_name("");
    set_power_percent(0.0);
}

void CONVERTER::set_bus(const unsigned int bus)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_bus_exist(bus))
    {
        this->bus = bus;
        busptr = psdb.get_bus(bus);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Bus "<<bus<<" does not exist in power system database "<<psdb.get_system_name()<<" when setting converter bus.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void CONVERTER::set_valve_side_bus_name(string name) { valve_bus_name = name; }
void CONVERTER::set_name(string name) { converter_name = name; }
void CONVERTER::set_power_percent(double percent) { power_percent = percent; }

unsigned int CONVERTER::get_bus() const { return bus; }
BUS* CONVERTER::get_bus_pointer() const { return busptr; }
string CONVERTER::get_valve_side_bus_name() const { return valve_bus_name; }
string CONVERTER::get_name() const { return converter_name; }
double CONVERTER::get_power_percent() const { return power_percent; }
