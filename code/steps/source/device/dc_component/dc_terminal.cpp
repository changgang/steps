#include "header/device/dc_component/dc_terminal.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

DC_TERMINAL::DC_TERMINAL(STEPS& toolkit)
{
    set_toolkit(toolkit);
    clear();
}

DC_TERMINAL::~DC_TERMINAL()
{
    ;
}

void DC_TERMINAL::set_toolkit(STEPS& toolkit)
{
    this->toolkit =(&toolkit);
}

STEPS& DC_TERMINAL::get_toolkit() const
{
    return *toolkit;
}


void DC_TERMINAL::clear()
{
    terminal_name = "invalid station";
    terminal_number = 0;
    dcbus_number = 0;
    status = false;
    terminal_mode = DC_P_TERMIANL;
    //dc_stations.clear();
}

DEVICE_ID DC_TERMINAL:: get_device_id() const
{
    DEVICE_ID did;
    return did;
}

void DC_TERMINAL::set_model(MODEL* model)
{
    ;
}

MODEL* DC_TERMINAL::get_model_of_type(string model_type, unsigned int index)
{
    return NULL;
}

void DC_TERMINAL::set_terminal_name(const string name)
 {
     this->terminal_name = name;
 }

void DC_TERMINAL::set_terminal_number(const unsigned int terminal_number)
{
    this->terminal_number = terminal_number;
}

void DC_TERMINAL::set_terminal_dc_bus_number(const unsigned int dcbus)
{
    this->dcbus_number = dcbus;
}

void DC_TERMINAL::set_status(const bool status)
{
    this->status = status;
}

void DC_TERMINAL::set_terminal_mode(const DC_TERMINAL_MODE mode)
{
    this->terminal_mode = mode;
}

string DC_TERMINAL::get_terminal_name() const
{
    return terminal_name;
}

unsigned int DC_TERMINAL::get_terminal_number() const
{
    return terminal_number;
}

unsigned int DC_TERMINAL::get_terminal_dc_bus_number() const
{
    return dcbus_number;
}

bool DC_TERMINAL::get_status() const
{
    return status;
}

DC_TERMINAL_MODE DC_TERMINAL::get_terminal_mode() const
{
    return terminal_mode;
}

/*
vector<DC_STATION*> DC_TERMINAL::get_dc_stations_in_terminal()
{
    vector<DC_STATION*> stations;
    unsigned int n = dc_stations.size();
    for(unsigned int i=0;i!=n;i++)
    {
        stations.push_back(&dc_stations[i]);
    }
    return stations;
}
*/
/*
void DC_TERMINAL::initialize_dc_terminal_stations(vector<DC_STATION*> stations)
{
    dc_stations.clear();
    unsigned int n_station = stations.size();
    dc_stations.reserve(n_station);
    for(unsigned int i=0;i!=n_station;i++)
    {
        DC_STATION station = *stations[i];
        dc_stations.push_back(station);
    }
}
*/
