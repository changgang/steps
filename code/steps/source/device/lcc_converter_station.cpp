#include "header/device/lcc_converter_station.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

LCC_CONVERTER_STATION::LCC_CONVERTER_STATION()
{
    converters = nullptr;
    nconverters = 0;
    clear();
}

LCC_CONVERTER_STATION::~LCC_CONVERTER_STATION() { clear(); }

void LCC_CONVERTER_STATION::clear()
{
    if(converters != nullptr)
    {
        free(converters);
        converters = nullptr;
    }
    nconverters = 0;
}


void LCC_CONVERTER_STATION::set_number_of_converters(unsigned int n)
{
    clear();
    nconverters = n;
    converters = (LCC*) malloc(sizeof(LCC)*nconverters);
    for(unsigned int i = 0; i < nconverters; ++i)
    {
        converters[i].clear();
        converters[i].set_toolkit(get_toolkit());
    }
}

void LCC_CONVERTER_STATION::set_converter(unsigned int converter_index, LCC& converter)
{
    if(converter_index < nconverters)
        converters[converter_index] = converter;
}

unsigned int LCC_CONVERTER_STATION::get_number_of_converters() const
{
    return nconverters;
}

const LCC* LCC_CONVERTER_STATION::get_converter(unsigned int converter_index) const
{
    if(converter_index < nconverters)
        return converters+converter_index;
    else
    {
        ostringstream osstream;
        osstream<<"FATAL ERROR. Converter index "<<converter_index<<" exceeds existing converters of LCC converter station "<<get_name();
        get_toolkit().show_information_with_leading_time_stamp(osstream);

        return nullptr;
    }
}

LCC* LCC_CONVERTER_STATION::get_mutable_converter(unsigned int converter_index)
{
    if(converter_index < nconverters)
        return converters+converter_index;
    else
    {
        ostringstream osstream;
        osstream<<"FATAL ERROR. Converter index "<<converter_index<<" exceeds existing converters of LCC converter station "<<get_name();
        get_toolkit().show_information_with_leading_time_stamp(osstream);

        return nullptr;
    }
}

unsigned int LCC_CONVERTER_STATION::get_total_power_percent_of_converter_station() const
{
    double percent = 0.0;
    for(unsigned int i = 0; i < nconverters; ++i)
        percent += converters[i].get_power_percent();
    return percent;
}

void LCC_CONVERTER_STATION::solve_transformer_tap_and_angle(double Vdc, double Idc)
{
    for(unsigned int i = 0; i < nconverters; ++i)
    {
        converters[i].solve_transformer_tap_and_angle(Vdc*converters[i].get_power_percent()*0.01, Idc);
    }
}

double LCC_CONVERTER_STATION::get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle() const
{
    double Vdc0 = 0.0;
    for(unsigned int i = 0; i < nconverters; ++i)
        Vdc0 += converters[i].get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle();
    return Vdc0;
}

double LCC_CONVERTER_STATION::get_dc_voltage_drop_multiple_in_kV_per_A() const
{
    double Vdc_drop_mult = 0.0;
    for(unsigned int i = 0; i < nconverters; ++i)
        Vdc_drop_mult += converters[i].get_dc_voltage_drop_multiple_in_kV_per_A();
    return Vdc_drop_mult;
}

void LCC_CONVERTER_STATION::report() const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"Converters of "<<get_name()<<" at "<<get_converter_side_name()<<" side are:";
    for(unsigned int i=0; i<nconverters; ++i)
        osstream<<"Converter "<<num2str(i+1)<<": "<<converters[i].get_name()<<" at bus "<<converters[i].get_bus()<<endl
                <<"1> bridge number: "<<converters[i].get_number_of_bridge()<<endl
                <<"2> max angle: "<<converters[i].get_max_angle_in_deg()<<" deg\n"
                <<"3> min angle: "<<converters[i].get_min_angle_in_deg()<<" deg\n"
                <<"4> transformer grid side base voltage : "<<converters[i].get_transformer_grid_side_base_voltage_in_kV()<<" kV\n"
                <<"5> transformer valve side base voltage : "<<converters[i].get_transformer_valve_side_base_voltage_in_kV()<<" kV\n"
                <<"6> max tap: "<<converters[i].get_transformer_max_tap_in_pu()<<endl
                <<"7> min tap: "<<converters[i].get_transformer_min_tap_in_pu()<<endl
                <<"8> Rcr+jXcr = "<<converters[i].get_transformer_impedance_in_ohm()<<" ohm";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

LCC_CONVERTER_STATION& LCC_CONVERTER_STATION::operator=(const LCC_CONVERTER_STATION& station)
{
    if(this == (&station)) return *this;
    clear();

    set_toolkit(station.get_toolkit());
    set_name(station.get_name());
    set_converter_side(station.get_converter_side());

    set_number_of_converters(station.get_number_of_converters());
    for(unsigned int converter_index = 0; converter_index < nconverters; ++converter_index)
        converters[converter_index] = *(station.get_converter(converter_index));
    return *this;
}
