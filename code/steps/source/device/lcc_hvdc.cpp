#include "header/device/lcc_hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include "header/model/hvdc_model/hvdc_models.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

LCC_HVDC::LCC_HVDC(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    station[RECTIFIER].set_toolkit(toolkit);
    station[INVERTER].set_toolkit(toolkit);
    clear();
}

LCC_HVDC::~LCC_HVDC()
{
}

void LCC_HVDC::set_name(const string name)
{
    this->name = trim_string(name);
}


void LCC_HVDC::set_converter_count(CONVERTER_SIDE side, const unsigned int n)
{
    station[side].set_number_of_converters(n);
}

const LCC* LCC_HVDC::get_immutable_converter(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    return station[side].get_converter(converter_index);
}

LCC* LCC_HVDC::get_mutable_converter(CONVERTER_SIDE side, const unsigned int converter_index)
{
    return station[side].get_mutable_converter(converter_index);
}

void LCC_HVDC::set_converter_bus(CONVERTER_SIDE side, const unsigned int converter_index, const unsigned int bus)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_bus(bus);
}

void LCC_HVDC::set_converter_valve_side_bus_name(CONVERTER_SIDE side, const unsigned int converter_index, string name)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_valve_side_bus_name(name);
}

void LCC_HVDC::set_converter_power_percent(CONVERTER_SIDE side, const unsigned int converter_index, double percent)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_power_percent(percent);
}

void LCC_HVDC::set_line_resistance_in_ohm(const double R)
{
    if(R>0.0)
        this->line_R_in_ohm = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<R<<" ohm) is not allowed for setting LCC_HVDC line resistance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_R_in_ohm = 0.0;
    }
}

void LCC_HVDC::set_line_inductance_in_mH(const double L)
{
    if(L>=0.0)
        this->line_L_in_H = L*1e-3;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<L<<" mH) is not allowed for setting LCC_HVDC line inductance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_L_in_H = 0.0;
    }
}

void LCC_HVDC::set_line_capacitance_in_uF(const double C)
{
    if(C>=0.0)
        this->line_C_in_F = C*1e-6;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<C<<" uF) is not allowed for setting LCC_HVDC line capacitance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_C_in_F = 0.0;
    }
}

void LCC_HVDC::set_line_smooting_inductance_in_mH(CONVERTER_SIDE side, const double L)
{
    if(L>=0.0)
        this->smoothing_L_in_H[side] = L*0.001;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<L<<" mH) is not allowed for setting LCC_HVDC smoothing inductance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->smoothing_L_in_H[side] = 0.0;
    }
}

void LCC_HVDC::set_line_smooting_resistance_in_ohm(CONVERTER_SIDE side, const double R)
{
    if(R>=0.0)
        this->smoothing_R_in_ohm[side] = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<R<<" ohm) is not allowed for setting LCC_HVDC smoothing resistance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->smoothing_R_in_ohm[side] = 0.0;
    }
}

void LCC_HVDC::set_nominal_dc_power_in_MW(const double P)
{
    if(P>0.0)
    {
        demand_power_in_MW = P;
        if(get_nominal_dc_voltage_in_kV()!=0.0)
            demand_current_in_kA = P/get_nominal_dc_voltage_in_kV();
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<P<<" MW) is not allowed for setting LCC_HVDC nominal dc power per pole of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_power_in_MW = 0.0;
        demand_current_in_kA = 0.0;
    }
}

void LCC_HVDC::set_nominal_dc_voltage_in_kV(const double V)
{
    if(V>0.0)
    {
        demand_voltage_in_kV = V;
        if(get_nominal_dc_current_in_kA()!=0.0 and get_nominal_dc_power_in_MW()==0.0)
            demand_power_in_MW = get_nominal_dc_current_in_kA()*V;
        if(get_nominal_dc_current_in_kA()==0.0 and get_nominal_dc_power_in_MW()!=0.0)
            demand_current_in_kA = get_nominal_dc_power_in_MW()/V;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting LCC_HVDC nominal dc voltage per pole of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_voltage_in_kV = 0.0;
    }
}


void LCC_HVDC::set_nominal_dc_current_in_kA(const double I)
{
    if(I>0.0)
    {
        demand_current_in_kA = I;
        if(get_nominal_dc_voltage_in_kV()!=0.0)
            demand_power_in_MW = I*get_nominal_dc_voltage_in_kV();
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<I<<" kA) is not allowed for setting LCC_HVDC nominal dc current per pole of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_power_in_MW = 0.0;
        demand_current_in_kA = 0.0;
    }
}

void LCC_HVDC::set_compensating_resistance_to_hold_dc_voltage_in_ohm(const double R)
{
    compensating_R_to_hold_dc_voltage_in_ohm = R;
}
void LCC_HVDC::set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(const double V)
{
    if(V>0.0)
        mode_switching_voltage_in_kV = V;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting LCC_HVDC threshold dc voltage for constant power and constant current operation mode of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid LCC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        mode_switching_voltage_in_kV = 0.0;
    }
}
void LCC_HVDC::set_current_power_margin(const double margin)
{
    if(margin>0.0 and margin<1.0)
        this->dc_current_power_margin = margin;
    else
        this->dc_current_power_margin = 0.15;
}

void LCC_HVDC::set_meter_end(const CONVERTER_SIDE side)
{
    meter_end = side;
}

void LCC_HVDC::set_operation_mode(const LCC_HVDC_OPERATION_MODE mode)
{
    this->mode = mode;
}

void LCC_HVDC::set_converter_number_of_bridge(CONVERTER_SIDE side, const unsigned int converter_index, const unsigned int nbridge)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_number_of_bridge(nbridge);
}

void LCC_HVDC::set_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE side, const unsigned int converter_index, const double V)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_voltage_drop_per_bridge_in_kV(V);
}

void LCC_HVDC::set_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index, const double angle)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_max_angle_in_deg(angle);
}

void LCC_HVDC::set_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index, const double angle)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_min_angle_in_deg(angle);
}

void LCC_HVDC::set_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index, const double V)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_grid_side_base_voltage_in_kV(V);
}

void LCC_HVDC::set_converter_transformer_valve_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index, const double V)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_valve_side_base_voltage_in_kV(V);
}

void LCC_HVDC::set_converter_transformer_impedance_in_ohm(CONVERTER_SIDE side, const unsigned int converter_index, const complex<double> z)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_impedance_in_ohm(z);
}

void LCC_HVDC::set_converter_transformer_admittance_in_siemens(CONVERTER_SIDE side, const unsigned int converter_index, const complex<double> y)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_admittance_in_siemens(y);
}

void LCC_HVDC::set_converter_transformer_max_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index, const double maxtap)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_max_tap_in_pu(maxtap);
}

void LCC_HVDC::set_converter_transformer_min_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index, const double mintap)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_min_tap_in_pu(mintap);
}

void LCC_HVDC::set_converter_transformer_number_of_taps(CONVERTER_SIDE side, const unsigned int converter_index, const unsigned int ntap)
{
    LCC* converter = get_mutable_converter(side, converter_index);
    if(converter!=nullptr) converter->set_transformer_number_of_taps(ntap);
}


string LCC_HVDC::get_name() const
{
    return name;
}

unsigned int LCC_HVDC::get_converter_count(CONVERTER_SIDE side) const
{
    return station[side].get_number_of_converters();
}

unsigned int LCC_HVDC::get_converter_bus(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    return converter->get_bus();
}

/*BUS* LCC_HVDC::get_converter_bus_pointer(CONVERTER_SIDE side, unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter != nullptr)
        return converter->get_bus_pointer();
    else
        return nullptr;
}*/
string LCC_HVDC::get_converter_valve_side_bus_name(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    return converter->get_valve_side_bus_name();
}

double LCC_HVDC::get_converter_power_percent(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    return converter->get_power_percent();
}

double LCC_HVDC::get_line_resistance_in_ohm() const
{
    return line_R_in_ohm;
}

double LCC_HVDC::get_line_inductance_in_mH() const
{
    return line_L_in_H*1e3;
}

double LCC_HVDC::get_line_capacitance_in_uF() const
{
    return line_C_in_F*1e6;
}

double LCC_HVDC::get_line_smooting_inductance_in_mH(CONVERTER_SIDE side) const
{
    return smoothing_L_in_H[side]*1e3;
}

double LCC_HVDC::get_line_smooting_resistance_in_ohm(CONVERTER_SIDE side) const
{
    return smoothing_R_in_ohm[side];
}

double LCC_HVDC::get_nominal_dc_power_in_MW() const
{
    return demand_power_in_MW;
}

double LCC_HVDC::get_nominal_dc_voltage_in_kV() const
{
    return demand_voltage_in_kV;
}

double LCC_HVDC::get_compensating_resistance_to_hold_dc_voltage_in_ohm() const
{
    return compensating_R_to_hold_dc_voltage_in_ohm;
}

double LCC_HVDC::get_nominal_dc_current_in_kA() const
{
    return demand_current_in_kA;
}

double LCC_HVDC::get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV() const
{
    return mode_switching_voltage_in_kV;
}

double LCC_HVDC::get_current_power_margin() const
{
    return dc_current_power_margin;
}

CONVERTER_SIDE LCC_HVDC::get_meter_end() const
{
    return meter_end;
}

LCC_HVDC_OPERATION_MODE LCC_HVDC::get_operation_mode() const
{
    return mode;
}

unsigned int LCC_HVDC::get_converter_number_of_bridge(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_number_of_bridge();
    else return 0;
}

double LCC_HVDC::get_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_voltage_drop_per_bridge_in_kV();
    else return 0;
}

double LCC_HVDC::get_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_max_angle_in_deg();
    else return 0;
}

double LCC_HVDC::get_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_min_angle_in_deg();
    else return 0;
}

double LCC_HVDC::get_converter_cos_max_alpha_or_gamma(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_cos_max_angle();
    else return 0;
}

double LCC_HVDC::get_converter_cos_min_alpha_or_gamma(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_cos_min_angle();
    else return 0;
}

double LCC_HVDC::get_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_grid_side_base_voltage_in_kV();
    else return 0;
}

double LCC_HVDC::get_converter_transformer_valve_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_valve_side_base_voltage_in_kV();
    else return 0;
}

complex<double> LCC_HVDC::get_converter_transformer_impedance_in_ohm(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_impedance_in_ohm();
    else return 0;
}

complex<double> LCC_HVDC::get_converter_transformer_admittance_in_siemens(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_admittance_in_siemens();
    else return 0;
}

double LCC_HVDC::get_converter_transformer_max_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_max_tap_in_pu();
    else return 0;
}

double LCC_HVDC::get_converter_transformer_min_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_min_tap_in_pu();
    else return 0;
}

unsigned int LCC_HVDC::get_converter_transformer_number_of_taps(CONVERTER_SIDE side, const unsigned int converter_index) const
{
    const LCC* converter = get_immutable_converter(side, converter_index);
    if(converter!=nullptr)  return converter->get_transformer_number_of_taps();
    else return 0;
}

const LCC_STATION& LCC_HVDC::get_station(CONVERTER_SIDE side) const
{
    return station[side];
}

string LCC_HVDC::get_converter_side_name(CONVERTER_SIDE side) const
{
    switch(side)
    {
        case RECTIFIER:
            return "RECTIFIER";
        default:
            return "INVERTER";
    }
}

void LCC_HVDC::set_status(const bool status)
{
    this->status = status;
}

bool LCC_HVDC::get_status() const
{
    return status;
}

bool LCC_HVDC::is_blocked() const
{
    if(get_status()==true) return false;
    else                   return true;
}


double LCC_HVDC::get_rectifier_nominal_dc_power_command_in_MW() const
{
    double Vdci = get_inverter_nominal_dc_voltage_command_in_kV();
    double Idc = get_rectifier_nominal_dc_current_command_in_kA();
    double Rdc = get_line_resistance_in_ohm();

    return Idc*(Vdci+Rdc*Idc);
}

double LCC_HVDC::get_rectifier_nominal_dc_current_command_in_kA() const
{
    return get_nominal_dc_current_in_kA();
}

double LCC_HVDC::get_inverter_nominal_dc_voltage_command_in_kV() const
{
    LCC_HVDC_OPERATION_MODE mode = get_operation_mode();
    if(mode==RECTIFIER_CONSTANT_POWER_INVERTER_CONSTANT_VOLTAGE)
        return get_nominal_dc_voltage_in_kV();
    else
    {
        if(mode==RECTIFIER_CONSTANT_POWER_AND_VOLTAGE)
        {
            double Pdc = get_nominal_dc_power_in_MW();
            double Vdcr = get_nominal_dc_voltage_in_kV();
            double Idc = Pdc/Vdcr;
            double Rdc = get_line_resistance_in_ohm();
            return Vdcr-Idc*Rdc;
        }
        else
        {
            if(mode==RECTIFIER_CONSTANT_POWER_AND_CURRENT)
            {
                double Pdc = get_nominal_dc_power_in_MW();
                double Idc = get_nominal_dc_current_in_kA();
                double Vdcr = Pdc/Idc;
                double Rdc = get_line_resistance_in_ohm();
                return Vdcr-Idc*Rdc;
            }
            else
            {
                STEPS& toolkit = get_toolkit();
                ostringstream osstream;
                osstream<<"Fatal Error. Operation mode is not supported for ";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return 0.0;
            }
        }
    }
}

void LCC_HVDC::reverse_converters()
{
    LCC_STATION temp_station = station[RECTIFIER];
    station[RECTIFIER] = station[INVERTER];
    station[INVERTER] = temp_station;
}

bool LCC_HVDC::is_valid() const
{
    if(get_name() != "") return true;
    else return false;
}

void LCC_HVDC::check()
{
    ;
}

void LCC_HVDC::clear()
{
    set_name("");
    set_status(false);
    line_R_in_ohm = 0.0;
    set_line_inductance_in_mH(0.0);
    set_line_capacitance_in_uF(0.0);
    set_line_smooting_inductance_in_mH(RECTIFIER, 0.0);
    set_line_smooting_inductance_in_mH(INVERTER, 0.0);
    set_line_smooting_resistance_in_ohm(RECTIFIER, 0.0);
    set_line_smooting_resistance_in_ohm(INVERTER, 0.0);
	set_compensating_resistance_to_hold_dc_voltage_in_ohm(0.0);
	set_current_power_margin(0.15);
    demand_power_in_MW = 0.0; demand_current_in_kA = 0.0; demand_voltage_in_kV = 0.0;
    mode_switching_voltage_in_kV = 0.0;
    set_meter_end(INVERTER);

    station[RECTIFIER].clear();
    station[INVERTER].clear();
}

bool LCC_HVDC::is_connected_to_bus(unsigned int bus) const
{
    for(unsigned int converter=0; converter<2; ++converter)
    {
        CONVERTER_SIDE side = RECTIFIER;
        if(converter==1) side = INVERTER;

        if(station[side].is_connected_to_bus(bus))
            return true;
    }
    return false;
}

bool LCC_HVDC::is_in_area(unsigned int area) const
{
    for(unsigned int converter=0; converter<2; ++converter)
    {
        CONVERTER_SIDE side = RECTIFIER;
        if(converter==1) side = INVERTER;

        if(station[side].is_in_area(area))
            return true;
    }
    return false;
}

bool LCC_HVDC::is_in_zone(unsigned int zone) const
{
    for(unsigned int converter=0; converter<2; ++converter)
    {
        CONVERTER_SIDE side = RECTIFIER;
        if(converter==1) side = INVERTER;

        if(station[side].is_in_zone(zone))
            return true;
    }
    return false;
}

void LCC_HVDC::report() const
{
    ostringstream osstream;
    osstream<<"LCC_HVDC '"<<get_name()<<"': Rdc = "<<get_line_resistance_in_ohm()<<" ohm";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Vdc = "<<get_nominal_dc_voltage_in_kV()<<" kV, "
            <<"Idc = "<<get_nominal_dc_current_in_kA()<<" kA, "
            <<"Pdc = "<<get_nominal_dc_power_in_MW()<<" MW";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Compensating R = "<<get_compensating_resistance_to_hold_dc_voltage_in_ohm()<<" ohm";
    toolkit.show_information_with_leading_time_stamp(osstream);

    station[RECTIFIER].report();
    station[INVERTER].report();
}

void LCC_HVDC::save() const
{
    ;
}

LCC_HVDC& LCC_HVDC::operator=(const LCC_HVDC& hvdc)
{
    if(this == (&hvdc)) return *this;

    set_toolkit(hvdc.get_toolkit());
    clear();

    set_name(hvdc.get_name());
    set_status(hvdc.get_status());
    set_line_resistance_in_ohm(hvdc.get_line_resistance_in_ohm());
    set_line_inductance_in_mH(hvdc.get_line_inductance_in_mH());
    set_line_capacitance_in_uF(hvdc.get_line_capacitance_in_uF());
    set_line_smooting_inductance_in_mH(RECTIFIER, hvdc.get_line_smooting_inductance_in_mH(RECTIFIER));
    set_line_smooting_inductance_in_mH(INVERTER, hvdc.get_line_smooting_inductance_in_mH(INVERTER));
    set_line_smooting_resistance_in_ohm(RECTIFIER, hvdc.get_line_smooting_resistance_in_ohm(RECTIFIER));
    set_line_smooting_resistance_in_ohm(INVERTER, hvdc.get_line_smooting_resistance_in_ohm(INVERTER));
    set_nominal_dc_power_in_MW(hvdc.get_nominal_dc_power_in_MW());
    set_nominal_dc_current_in_kA(hvdc.get_nominal_dc_current_in_kA());
    set_nominal_dc_voltage_in_kV(hvdc.get_nominal_dc_voltage_in_kV());
    set_compensating_resistance_to_hold_dc_voltage_in_ohm(hvdc.get_compensating_resistance_to_hold_dc_voltage_in_ohm());
    set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(hvdc.get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV());
    set_current_power_margin(hvdc.get_current_power_margin());

    station[RECTIFIER] = hvdc.get_station(RECTIFIER);
    station[INVERTER] = hvdc.get_station(INVERTER);

    return *this;
}

DEVICE_ID LCC_HVDC::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("LCC_HVDC");

    TERMINAL terminal;
    unsigned int n = get_converter_count(RECTIFIER);
    for(unsigned int converter_index=0; converter_index<n; ++converter_index)
        terminal.append_bus(get_converter_bus(RECTIFIER, converter_index));
    n = get_converter_count(INVERTER);
    for(unsigned int converter_index=0; converter_index<n; ++converter_index)
        terminal.append_bus(get_converter_bus(INVERTER, converter_index));
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_name());

    return did;
}

void LCC_HVDC::solve_steady_state()
{
    unsigned int n = 0;
    for(unsigned int converter=0; converter<2; ++converter)
    {
        CONVERTER_SIDE side = RECTIFIER;
        if(converter==1) side = INVERTER;
        n = get_converter_count(side);

        double total_percent = 0.0;
        for(unsigned int converter_index = 0; converter_index<n; ++converter_index)
            total_percent += get_converter_power_percent(side, converter_index);
        if(fabs(total_percent-1)>FLOAT_EPSILON)
        {
            for(unsigned int converter_index = 0; converter_index<n; ++converter_index)
            {
                double newpercent = get_converter_power_percent(side, converter_index)/total_percent;
                set_converter_power_percent(side, converter_index, newpercent);
            }
        }
    }

    double Idc = get_rectifier_nominal_dc_current_command_in_kA();
    double Vdci = get_inverter_nominal_dc_voltage_command_in_kV();
    double Rdc = get_line_resistance_in_ohm();
    double Vdcr = Vdci+Idc*Rdc;

    double Vdc[2]={Vdcr, Vdci};

    for(unsigned int converter=0; converter<2; ++converter)
    {
        CONVERTER_SIDE side = RECTIFIER;
        if(converter==1) side = INVERTER;

        station[side].solve_with_desired_dc_voltage_and_current(Vdc[side], Idc);
    }
}

void LCC_HVDC::show_solved_hvdc_steady_state() const
{
    STEPS& toolkit = get_toolkit();
    double sbase = toolkit.get_system_base_power_in_MVA();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    ostringstream osstream;
    osstream<<"Solved steady state of LCC_HVDC '"<<get_name()<<"':";
    toolkit.show_information_with_leading_time_stamp(osstream);

    LCC_HVDC_OPERATION_MODE  mode = get_operation_mode();
    string modestr;
    switch(mode)
    {
        case RECTIFIER_CONSTANT_POWER_AND_CURRENT:
            modestr = "rectifier constant power & current";
            break;
        case RECTIFIER_CONSTANT_POWER_AND_VOLTAGE:
            modestr = "rectifier constant power & voltage";
            break;
        case RECTIFIER_CONSTANT_POWER_INVERTER_CONSTANT_VOLTAGE:
            modestr = "rectifier constant power & inverter constant voltage";
            break;
        default:
            modestr = "Invalid mode";
            break;
    }
    osstream<<"Mode: "<<modestr;
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Idc = "<<get_line_dc_current_in_kA()<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    for(unsigned int converter=0; converter<2; ++converter)
    {
        CONVERTER_SIDE side = RECTIFIER;
        if(converter==1) side = INVERTER;

        osstream<<get_converter_side_name(side)<<":";
        toolkit.show_information_with_leading_time_stamp(osstream);

        unsigned int n = get_converter_count(side);
        for(unsigned int converter_index=0; converter_index<n; ++converter_index)
        {
            const LCC* converter = get_immutable_converter(side, converter_index);

            osstream<<"Converter "<<converter_index<<" at bus "<<converter->get_bus();
            toolkit.show_information_with_leading_time_stamp(osstream);

            osstream<<"Vdc = "<<converter->get_dc_voltage_in_kV()<<" kV, "
                    <<"Vac = "<<converter->get_bus_positive_sequence_voltage_in_kV()<<" kV";
            toolkit.show_information_with_leading_time_stamp(osstream);

            osstream<<"Transformer tap = "<<converter->get_transformer_tap_in_pu()<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            osstream<<"alpha/gamma = "<<converter->get_angle_in_deg()<<" deg, "
                    <<"mu = "<<converter->get_commutating_overlap_angle_in_deg()<<" deg";
            toolkit.show_information_with_leading_time_stamp(osstream);

            osstream<<"AC current = "<<converter->get_ac_current_in_kA()<<" kA.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            double P = converter->get_ac_active_power_in_MW();
            double Q = converter->get_ac_reactive_power_in_MVar();
            complex<double> S(P,Q);
            complex<double> V = converter->get_bus_positive_sequence_complex_voltage_in_pu();
            S *=  one_over_sbase;
            complex<double> I = conj(S/V);
            I *= (sbase/(SQRT3*converter->get_bus_base_voltage_in_kV()));

            osstream<<"Pdc = "<<converter->get_dc_power_in_MW()<<" MW, "
                    <<"Pac = "<<converter->get_ac_active_power_in_MW()<<" MW, "
                    <<"Qac = "<<converter->get_ac_reactive_power_in_MVar()<<" MVar, "
                    <<"Eta = "<<converter->get_ac_power_factor()<<", "
                    <<"Iac = "<<converter->get_ac_current_in_kA()<<"kA, or "
                    <<I<<"KA and "<<abs(I)<<"kA";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

double LCC_HVDC::get_line_dc_current_in_kA() const
{
    return get_nominal_dc_current_in_kA();
}


void LCC_HVDC::set_model(MODEL* model)
{
    return;
}

MODEL* LCC_HVDC::get_model_of_type(string model_type)
{
    return nullptr;
}
void LCC_HVDC::run(DYNAMIC_MODE mode)
{
    ;
}
