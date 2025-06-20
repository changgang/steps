#include "header/device/dc_component/dc_bus.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

DC_BUS::DC_BUS(STEPS& toolkit)
{
    clear();
}

DC_BUS::~DC_BUS()
{
    ;
}

void DC_BUS::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}
STEPS& DC_BUS::get_toolkit()
{
    return *toolkit;
}




bool DC_BUS::is_valid() const
{
    return true;
}

void DC_BUS::check()
{
    ;
}

void DC_BUS::clear()
{

    dc_bus_number = 0;
    name = "dc_bus_with_no_data";
    nominal_voltage = 0.0;
    Vdc_kV = 0;
    Idc_kA = 0;
    Pdc_MW = 0;
    dc_bus_area = 0;
    dc_bus_zone =0;
    dc_bus_type = DC_P_NODE;
    dy_dc_bus_type = DYNAMIC_DC_U_BUS ;
}

void DC_BUS::report() const
{
    ;
}


//set
void DC_BUS::set_status(const bool status)
{
    this->status = status;
}

void DC_BUS::set_name(const string name)
{
    this-> name = name;
}

void DC_BUS::set_nominal_voltage(const double vn)
{
    this->nominal_voltage =vn;
}

void DC_BUS::set_dc_bus_number(const unsigned int number)
{
    this->dc_bus_number = number;
}

void DC_BUS::set_dc_bus_voltage_in_kV(const double vdc)
{
    this->Vdc_kV = vdc;
}

void DC_BUS::set_dc_bus_current_in_kA(const double idc)
{
    this->Idc_kA = idc;
}

void DC_BUS::set_dc_bus_power_in_MW(const double pdc)
{
    this->Pdc_MW = pdc;
}

void DC_BUS::set_dc_bus_area(const unsigned int area)
{
    this->dc_bus_area = area;
}

void DC_BUS::set_dc_bus_zone(const unsigned int zone)
{
    this->dc_bus_zone = zone;
}

void DC_BUS::set_dc_bus_type(const DC_BUS_TYPE type)
{
    this->dc_bus_type = type;
}

void DC_BUS::set_dc_bus_dynamic_type(const DYNAMIC_DC_BUS_TYPE dy_type)
{
    this->dy_dc_bus_type = dy_type;
}

//get
bool DC_BUS::get_status() const
{
    return status;
}

string DC_BUS::get_name() const
{
    return name;
}

unsigned int DC_BUS::get_dc_bus_number() const
{
    return dc_bus_number;
}

double DC_BUS::get_nominal_voltage() const
{
    return nominal_voltage;
}

double DC_BUS::get_dc_bus_voltage_in_kV() const
{
    return Vdc_kV;
}

double DC_BUS::get_dc_bus_current_in_kA() const
{
    return Idc_kA;
}

double DC_BUS::get_dc_bus_power_in_MW() const
{
    return Pdc_MW;
}

unsigned int DC_BUS::get_dc_bus_area() const
{
    return dc_bus_area;
}

unsigned int DC_BUS::get_dc_bus_zone() const
{
    return dc_bus_zone;
}

DC_BUS_TYPE DC_BUS::get_dc_bus_type() const
{
    return dc_bus_type;
}

DYNAMIC_DC_BUS_TYPE DC_BUS::get_dc_bus_dynamic_type() const
{
    return dy_dc_bus_type;
}















