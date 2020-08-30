#include "header/device/converter_station.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

CONVERTER_STATION::CONVERTER_STATION()
{
    clear();
}

CONVERTER_STATION::~CONVERTER_STATION() { clear(); }

void CONVERTER_STATION::clear()
{
    toolkit = nullptr;
    set_name("");
    set_converter_side(RECTIFIER);
}

void CONVERTER_STATION::copy_from_const_converter_station(const CONVERTER_STATION& station)
{
    clear();
    set_toolkit(station.get_toolkit());
    set_name(station.get_name());
    set_converter_side(station.get_converter_side());
}

void CONVERTER_STATION::set_toolkit(STEPS& toolkit) { this->toolkit = (&toolkit); }
STEPS& CONVERTER_STATION::get_toolkit() const { return *toolkit; }

void CONVERTER_STATION::set_name(string name) { this->name = name; }
string CONVERTER_STATION::get_name() const { return name; }

void CONVERTER_STATION::set_converter_side(CONVERTER_SIDE side) { this->side = side; }
CONVERTER_SIDE CONVERTER_STATION::get_converter_side() const { return side; }

string CONVERTER_STATION::get_converter_side_name() const
{
    if(side==RECTIFIER) return "RECTIFIER";
    else                return "INVERTER";
}
