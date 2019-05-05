#include "header/device/hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include "header/model/hvdc_model/hvdc_models.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

HVDC::HVDC()
{
    clear();
}

HVDC::~HVDC()
{
    if(hvdc_model!=NULL)
        delete hvdc_model;

    if(auxiliary_signal_model!=NULL)
        delete auxiliary_signal_model;
}

string HVDC::get_converter_side_name(HVDC_CONVERTER_SIDE converter) const
{
    switch(converter)
    {
        case RECTIFIER:
            return "RECTIFIER";
        default:
            return "INVERTER";
    }
}

void HVDC::set_converter_bus(HVDC_CONVERTER_SIDE converter, const size_t bus)
{
    ostringstream osstream;
    string converter_name = get_converter_side_name(converter);

    if(bus==0)
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<converter_name<<" bus of HVDC link."<<endl
          <<"0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        converter_bus[converter] = bus;
        return;
    }

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(not psdb.is_bus_exist(bus))
    {
        osstream<<"Bus "<<bus<<" does not exist for setting up "<<converter_name<<" side bus of hvdc link."<<endl
           <<"0 will be set to indicate invalid hvdc link.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        converter_bus[converter] = 0;
        return;
    }
    converter_bus[converter] = bus;
}

void HVDC::set_converter_valve_side_bus_name(HVDC_CONVERTER_SIDE converter, string name)
{
    converter_valve_bus_name[converter] = trim_string(name);
}

void HVDC::set_identifier(const string hvdc_id)
{
    this->identifier = hvdc_id;
}

void HVDC::set_name(const string name)
{
    this->hvdc_name = trim_string(name);
}

void HVDC::set_status(const bool status)
{
    this->status = status;
}

void HVDC::set_number_of_poles(const HVDC_POLE npole)
{
    this->pole_number = npole;
}

void HVDC::set_line_resistance_in_ohm(const double R)
{
    if(R>0.0)
        this->line_R_in_ohm = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<R<<" ohm) is not allowed for setting HVDC line resistance of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_R_in_ohm = 0.0;
    }
}

void HVDC::set_line_inductance_in_mH(const double L)
{
    if(L>=0.0)
        this->line_L_in_H = L*1e-3;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<L<<" mH) is not allowed for setting HVDC line inductance of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_L_in_H = 0.0;
    }
}

void HVDC::set_line_capacitance_in_uF(const double C)
{
    if(C>=0.0)
        this->line_C_in_F = C*1e-6;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<C<<" uF) is not allowed for setting HVDC line capacitance of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_C_in_F = 0.0;
    }
}

void HVDC::set_line_smooting_inductance_in_mH(HVDC_CONVERTER_SIDE converter, const double L)
{
    if(L>=0.0)
        this->smoothing_L_in_H[converter] = L*0.001;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<L<<" mH) is not allowed for setting HVDC smoothing inductance of "<<get_device_name()<<endl
          <<"0.0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->smoothing_L_in_H[converter] = 0.0;
    }
}

void HVDC::set_line_smooting_resistance_in_ohm(HVDC_CONVERTER_SIDE converter, const double R)
{
    if(R>=0.0)
        this->smoothing_R_in_ohm[converter] = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<R<<" ohm) is not allowed for setting HVDC smoothing resistance of "<<get_device_name()<<endl
          <<"0.0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->smoothing_R_in_ohm[converter] = 0.0;
    }
}

void HVDC::set_nominal_dc_power_per_pole_in_MW(const double P)
{
    if(P>0.0)
    {
        demand_power_in_MW = P;
        if(get_nominal_dc_voltage_per_pole_in_kV()!=0.0)
            demand_current_in_kA = P/get_nominal_dc_voltage_per_pole_in_kV();
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<P<<" MW) is not allowed for setting HVDC nominal dc power per pole of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_power_in_MW = 0.0;
        demand_current_in_kA = 0.0;
    }
}

void HVDC::set_side_to_hold_power(HVDC_CONVERTER_SIDE side)
{
    hvdc_side_to_hold_power = side;
}


void HVDC::set_nominal_dc_voltage_per_pole_in_kV(const double V)
{
    if(V>0.0)
    {
        demand_voltage_in_kV = V;
        if(get_nominal_dc_current_per_pole_in_kA()!=0.0 and get_nominal_dc_power_per_pole_in_MW()==0.0)
            demand_power_in_MW = get_nominal_dc_current_per_pole_in_kA()*V;
        if(get_nominal_dc_current_per_pole_in_kA()==0.0 and get_nominal_dc_power_per_pole_in_MW()!=0.0)
            demand_current_in_kA = get_nominal_dc_power_per_pole_in_MW()/V;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting HVDC nominal dc voltage per pole of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_voltage_in_kV = 0.0;
    }
}

void HVDC::set_compensating_resistance_to_hold_dc_voltage_in_ohm(const double R)
{
    compensating_R_to_hold_dc_voltage_in_ohm = R;
}

void HVDC::set_nominal_dc_current_per_pole_in_kA(const double I)
{
    if(I>0.0)
    {
        demand_current_in_kA = I;
        if(get_nominal_dc_voltage_per_pole_in_kV()!=0.0)
            demand_power_in_MW = I*get_nominal_dc_voltage_per_pole_in_kV();
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<I<<" kA) is not allowed for setting HVDC nominal dc current per pole of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_power_in_MW = 0.0;
        demand_current_in_kA = 0.0;
    }
}

void HVDC::set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(const double V)
{
    if(V>0.0)
        mode_switching_voltage_in_kV = V;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting HVDC threshold dc voltage for constant power and constant current operation mode of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        mode_switching_voltage_in_kV = 0.0;
    }
}
void HVDC::set_current_power_margin(const double margin)
{
    if(margin>0.0 and margin<1.0)
        this->dc_current_power_margin = margin;
    else
        this->dc_current_power_margin = 0.15;
}

void HVDC::set_meter_end(const HVDC_CONVERTER_SIDE converter)
{
    meter_end = converter;
}

void HVDC::set_converter_operation_mode(HVDC_CONVERTER_SIDE converter, const HVDC_OPERATION_MODE mode)
{
    ostringstream osstream;
    string converter_name = get_converter_side_name(converter);

    if(converter==RECTIFIER)
    {
        if(mode == RECTIFIER_CONSTANT_POWER or mode == RECTIFIER_CONSTANT_CURRENT)
            operation_mode[converter] = mode;
        else
        {
            osstream<<"Warning. Neither INVERTER_CONSTANT_VOLTAGE nor INVERTER_CONSTANT_GAMMA is allowed for setting HVDC "<<converter_name<<" operation mode of "<<get_device_name()<<endl
              <<"RECTIFIER_CONSTANT_POWER mode will be set automatically.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            operation_mode[converter] = RECTIFIER_CONSTANT_POWER;
        }
    }
    else
    {
        if(mode == INVERTER_CONSTANT_VOLTAGE or mode == INVERTER_CONSTANT_GAMMA)
            operation_mode[converter] = mode;
        else
        {
            osstream<<"Warning. Neither INVERTER_CONSTANT_VOLTAGE nor INVERTER_CONSTANT_GAMMA is allowed for setting HVDC "<<converter_name<<" operation mode of "<<get_device_name()<<endl
              <<"INVERTER_CONSTANT_VOLTAGE mode will be set automatically.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            operation_mode[converter] = INVERTER_CONSTANT_VOLTAGE;
        }
    }
}

void HVDC::set_converter_number_of_bridge(HVDC_CONVERTER_SIDE converter, const size_t nbridge)
{
    string converter_name = get_converter_side_name(converter);
    if(nbridge > 0)
        this->bridge_number[converter] = nbridge;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Zero (0) is not allowed for setting HVDC "<<converter_name<<" bridge number of "<<get_device_name()<<endl
          <<"0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bridge_number[converter] = 0;
    }
}

void HVDC::set_converter_voltage_drop_per_bridge_in_kV(HVDC_CONVERTER_SIDE converter, const double V)
{
    string converter_name = get_converter_side_name(converter);
    if(V >= 0.0)
        this->voltage_drop_per_bridge_in_kV[converter] = V;
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Negative ("<<V<<" kV) is not allowed for setting HVDC "<<converter_name<<" voltage drop of "<<get_device_name()<<endl
          <<"0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->voltage_drop_per_bridge_in_kV[converter] = 0.0;
    }
}

void HVDC::set_converter_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, const double angle)
{
    if(angle>=0.0)
    {
        if(angle<=90.0)
            this->max_firing_angle_in_deg[converter] = angle;
        else
            this->max_firing_angle_in_deg[converter] = 90.0;
    }
    else
        this->max_firing_angle_in_deg[converter] = 0.0;
}

void HVDC::set_converter_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, const double angle)
{
    if(angle>=0.0)
        this->min_firing_angle_in_deg[converter] = angle;
    else
        this->min_firing_angle_in_deg[converter] = 0.0;
}

void HVDC::set_converter_transformer_grid_side_base_voltage_in_kV(HVDC_CONVERTER_SIDE converter, const double V)
{
    ostringstream osstream;
    string converter_name = get_converter_side_name(converter);

    if(V > 0.0)
        this->converter_transformer_grid_side_base_voltage_in_kV[converter] = V;
    else
    {
        if(V == 0.0)
        {
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

            if(psdb.is_bus_exist(get_converter_bus(converter)))
            {
                double base_voltage = psdb.get_bus_base_voltage_in_kV(get_converter_bus(converter));
                this->converter_transformer_grid_side_base_voltage_in_kV[converter] = base_voltage;
            }
            else
            {
                osstream<<"Warning. Zero (0.0 kV) is provided for setting HVDC "<<converter_name<<" transformer grid side base voltage of "<<get_device_name()<<endl
                  <<"However, the "<<converter_name<<" bus doesn't exist. 0.0 will be set to indicate as base voltage of "<<converter_name<<" bus.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                this->converter_transformer_grid_side_base_voltage_in_kV[converter] = 0.0;
            }
        }
         else
        {
            osstream<<"Warning. Negative ("<<V<<" kV) is not allowed for setting HVDC "<<converter_name<<" transformer grid side base voltage of "<<get_device_name()<<endl
              <<"0.0 will be set automatically as base voltage of "<<converter_name<<" bus.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            this->converter_transformer_grid_side_base_voltage_in_kV[converter] = 0.0;
        }
    }
}

void HVDC::set_converter_transformer_converter_side_base_voltage_in_kV(HVDC_CONVERTER_SIDE converter, const double V)
{
    ostringstream osstream;
    string converter_name = get_converter_side_name(converter);

    if(V > 0.0)
        this->converter_transformer_converter_side_base_voltage_in_kV[converter] = V;
    else
    {
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting HVDC "<<converter_name<<" transformer converter side base voltage of "<<get_device_name()<<endl
          <<"0.0 will be set to indicate invalid base voltage.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->converter_transformer_converter_side_base_voltage_in_kV[converter] = 0.0;
    }
}

void HVDC::set_converter_transformer_impedance_in_ohm(HVDC_CONVERTER_SIDE converter, const complex<double> z)
{
    this->converter_transformer_Z_in_ohm[converter] = z;
}

void HVDC::set_converter_transformer_admittance_in_siemens(HVDC_CONVERTER_SIDE converter, const complex<double> y)
{
    this->converter_transformer_Y_in_siemens[converter] = y;
}

void HVDC::set_converter_transformer_max_tap_in_pu(HVDC_CONVERTER_SIDE converter, const double maxtap)
{
    if(maxtap>0.0)
        this->converter_transformer_max_tap_in_pu[converter] = maxtap;
    else
        this->converter_transformer_max_tap_in_pu[converter] = 1.0;
}

void HVDC::set_converter_transformer_min_tap_in_pu(HVDC_CONVERTER_SIDE converter, const double mintap)
{
    if(mintap>0.0)
        this->converter_transformer_min_tap_in_pu[converter] = mintap;
    else
        this->converter_transformer_min_tap_in_pu[converter] = 1.0;
}

void HVDC::set_converter_transformer_number_of_taps(HVDC_CONVERTER_SIDE converter, const size_t ntap)
{
    if(ntap>1)
        this->converter_transformer_number_of_taps[converter] = ntap;
    else
        this->converter_transformer_number_of_taps[converter] = 1;
}

size_t HVDC::get_converter_bus(HVDC_CONVERTER_SIDE converter) const
{
    return converter_bus[converter];
}

string HVDC::get_converter_valve_side_bus_name(HVDC_CONVERTER_SIDE converter) const
{
    return converter_valve_bus_name[converter];
}

string HVDC::get_identifier() const
{
    return identifier;
}

string HVDC::get_name() const
{
    return hvdc_name;
}

bool HVDC::get_status() const
{
    return status;
}

bool HVDC::is_blocked() const
{
    if(get_status()==true) return false;
    else return true;
}

HVDC_POLE HVDC::get_number_of_poles() const
{
    return pole_number;
}

double HVDC::get_line_resistance_in_ohm() const
{
    return line_R_in_ohm;
}

double HVDC::get_line_inductance_in_mH() const
{
    return line_L_in_H*1e3;
}

double HVDC::get_line_capacitance_in_uF() const
{
    return line_C_in_F*1e6;
}

double HVDC::get_line_smooting_inductance_in_mH(HVDC_CONVERTER_SIDE converter) const
{
    return smoothing_L_in_H[converter]*1e3;
}

double HVDC::get_line_smooting_resistance_in_ohm(HVDC_CONVERTER_SIDE converter) const
{
    return smoothing_R_in_ohm[converter];
}

double HVDC::get_nominal_dc_power_per_pole_in_MW() const
{
    return demand_power_in_MW;
}

HVDC_CONVERTER_SIDE HVDC::get_side_to_hold_dc_power() const
{
    return hvdc_side_to_hold_power;
}

double HVDC::get_nominal_dc_voltage_per_pole_in_kV() const
{
    return demand_voltage_in_kV;
}

double HVDC::get_compensating_resistance_to_hold_dc_voltage_in_ohm() const
{
    return compensating_R_to_hold_dc_voltage_in_ohm;
}

double HVDC::get_nominal_dc_current_per_pole_in_kA() const
{
    return demand_current_in_kA;
}

double HVDC::get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV() const
{
    return mode_switching_voltage_in_kV;
}

double HVDC::get_current_power_margin() const
{
    return dc_current_power_margin;
}

HVDC_CONVERTER_SIDE HVDC::get_meter_end() const
{
    return meter_end;
}

HVDC_OPERATION_MODE HVDC::get_converter_operation_mode(HVDC_CONVERTER_SIDE converter) const
{
    return operation_mode[converter];
}

size_t HVDC::get_converter_number_of_bridge(HVDC_CONVERTER_SIDE converter) const
{
    return bridge_number[converter];
}

double HVDC::get_converter_voltage_drop_per_bridge_in_kV(HVDC_CONVERTER_SIDE converter) const
{
    return voltage_drop_per_bridge_in_kV[converter];
}

double HVDC::get_converter_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    return max_firing_angle_in_deg[converter];
}

double HVDC::get_converter_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    return min_firing_angle_in_deg[converter];
}

double HVDC::get_converter_transformer_grid_side_base_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const
{
    string converter_name = get_converter_side_name(converter);
    double V = converter_transformer_grid_side_base_voltage_in_kV[converter];
    if(V != 0.0)
        return V;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    return psdb.get_bus_base_voltage_in_kV(get_converter_bus(converter));
}

double HVDC::get_converter_transformer_converter_side_base_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_converter_side_base_voltage_in_kV[converter];
}

complex<double> HVDC::get_converter_transformer_impedance_in_ohm(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_Z_in_ohm[converter];
}

complex<double> HVDC::get_converter_transformer_admittance_in_siemens(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_Y_in_siemens[converter];
}

double HVDC::get_converter_transformer_max_tap_in_pu(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_max_tap_in_pu[converter];
}

double HVDC::get_converter_transformer_min_tap_in_pu(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_min_tap_in_pu[converter];
}

size_t HVDC::get_converter_transformer_number_of_taps(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_number_of_taps[converter];
}


void HVDC::turn_rectifier_constant_power_mode_into_constant_current_mode()
{
    if(get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_POWER)
    {
        double P = get_nominal_dc_power_per_pole_in_MW();
        double V = get_nominal_dc_voltage_per_pole_in_kV();
        set_nominal_dc_current_per_pole_in_kA(P/V);
        set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
        ostringstream osstream;
        osstream<<get_device_name()<<" operation mode is turned from constant power mode into constant current mode.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC::turn_rectifier_constant_current_mode_into_constant_power_mode()
{
    if(get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_CURRENT)
    {
        double I = get_nominal_dc_current_per_pole_in_kA();
        double V = get_nominal_dc_voltage_per_pole_in_kV();
        set_nominal_dc_power_per_pole_in_MW(V*I);
        set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
        ostringstream osstream;
        osstream<<get_device_name()<<" operation mode is turned from constant current mode into constant power mode.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

double HVDC::get_rectifier_nominal_dc_power_command_in_MW() const
{
    double Vdci = get_inverter_nominal_dc_voltage_command_in_kV();
    double Idc = get_rectifier_nominal_dc_current_command_in_kA();
    double Rdc = get_line_resistance_in_ohm();

    return Idc*(Vdci+Rdc*Idc);

    double Vn = get_nominal_dc_voltage_per_pole_in_kV();
    double Rcomp = get_compensating_resistance_to_hold_dc_voltage_in_ohm();

    if(get_converter_operation_mode(RECTIFIER) == RECTIFIER_CONSTANT_POWER) // constant power mode
    {
        double Pn = get_nominal_dc_power_per_pole_in_MW();
        HVDC_CONVERTER_SIDE Pside = get_side_to_hold_dc_power();
        if(Pside == RECTIFIER)
            return Pn;
        else // inverter side
        {
            if(fabs(Rcomp)<FLOAT_EPSILON)
            {
                double Idc = Pn/Vn;
                double Vdcr = Vn+Rdc*Idc;
                return Vdcr*Idc;
            }
            else
            {
                if(fabs(Rdc-Rcomp)<FLOAT_EPSILON)
                {
                    // (Vn-Idc*Rdc)*Idc = Pn
                    // Rdc*Idc2-Vn*Idc+Pn = 0
                    // Idc = (Vn+-sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    // use
                    // Idc = (Vn-sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    double Idc = (Vn-sqrt(Vn*Vn-4.0*Rdc*Pn))/(2.0*Rdc);
                    double Vdci = Pn/Idc;
                    double Vdcr = Vdci+Rdc*Idc;
                    return Vdcr*Idc;
                }
                else
                {
                    // (Vn-Idc*Rcomp)*Idc = Pn;
                    // use
                    // Idc = (Vn-sqrt(Vn2-4*Rcomp*Pn))/(2*Rcomp)
                    double Idc = (Vn-sqrt(Vn*Vn-4.0*Rcomp*Pn))/(2.0*Rcomp);
                    double Vdci = Pn/Idc;
                    double Vdcr = Vdci+Rdc*Idc;
                    return Vdcr*Idc;
                }
            }
        }
    }
    else // rectifier in constant current mode
    {
        double Idc = get_nominal_dc_current_per_pole_in_kA();
        if(fabs(Rcomp-Rdc)<FLOAT_EPSILON)
        {
            return Idc*Vn;
        }
        else
        {
            if(fabs(Rcomp)<FLOAT_EPSILON)
            {
                double Vdcr = Vn+Rdc*Idc;
                return Vdcr*Idc;
            }
            else
            {
                double Vdcr = Vn + (Rdc-Rcomp)*Idc;
                return Vdcr*Idc;
            }
        }
    }
}

double HVDC::get_rectifier_nominal_dc_current_command_in_kA() const
{
    if(get_converter_operation_mode(RECTIFIER) == RECTIFIER_CONSTANT_CURRENT) // constant current mode
    {
        double Idc = get_nominal_dc_current_per_pole_in_kA();
        return Idc;
    }
    else // rectifier in constant power mode
    {
        double Pn = get_nominal_dc_power_per_pole_in_MW();
        HVDC_CONVERTER_SIDE Pside = get_side_to_hold_dc_power();
        double Vn = get_nominal_dc_voltage_per_pole_in_kV();
        double Rcomp = get_compensating_resistance_to_hold_dc_voltage_in_ohm();
        double Rdc = get_line_resistance_in_ohm();
        if(Pside == RECTIFIER)
        {
            if(fabs(Rcomp-Rdc)<FLOAT_EPSILON)
            {
                double Idc = Pn/Vn;
                return Idc;
            }
            else
            {
                if(fabs(Rcomp)<FLOAT_EPSILON)
                {
                    // Pn = (Vn+Rdc*Idc)*Idc
                    // Rdc*Idc2 +Vn*Idc -Pn = 0
                    double Idc = (-Vn+sqrt(Vn*Vn+4.0*Rdc*Pn))/(2.0*Rdc);
                    return Idc;
                }
                else
                {
                    // Pn = (Vn+(Rdc-Rcomp)*Idc)*Idc;
                    double Idc = (-Vn+sqrt(Vn*Vn+4.0*(Rdc-Rcomp)*Pn))/(2.0*(Rdc-Rcomp));
                    return Idc;
                }
            }
        }
        else // inverter side
        {
            if(fabs(Rcomp)<FLOAT_EPSILON)
            {
                double Idc = Pn/Vn;
                return Idc;
            }
            else
            {
                if(fabs(Rdc-Rcomp)<FLOAT_EPSILON)
                {
                    // (Vn-Idc*Rdc)*Idc = Pn
                    // Rdc*Idc2-Vn*Idc+Pn = 0
                    // Idc = (Vn+-sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    // use
                    // Idc = (Vn-sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    double Idc = (Vn-sqrt(Vn*Vn-4.0*Rdc*Pn))/(2.0*Rdc);
                    return Idc;
                }
                else
                {
                    // (Vn-Idc*Rcomp)*Idc = Pn;
                    // use
                    // Idc = (Vn-sqrt(Vn2-4*Rcomp*Pn))/(2*Rcomp)
                    double Idc = (Vn-sqrt(Vn*Vn-4.0*Rcomp*Pn))/(2.0*Rcomp);
                    return Idc;
                }
            }
        }
    }
}

double HVDC::get_inverter_nominal_dc_voltage_command_in_kV() const
{
    double Vn = get_nominal_dc_voltage_per_pole_in_kV();
    double Rcomp = get_compensating_resistance_to_hold_dc_voltage_in_ohm();
    double Rdc = get_line_resistance_in_ohm();
    if(fabs(Rcomp)<FLOAT_EPSILON)
    {
        return Vn;
    }
    else
    {
        double Idc = get_rectifier_nominal_dc_current_command_in_kA();
        if(fabs(Rdc-Rcomp)<FLOAT_EPSILON)
        {
            double Vdci = Vn - Rdc*Idc;
            return Vdci;
        }
        else
        {
            double Vdci = Vn - Rcomp*Idc;
            return Vdci;
        }
    }
}



void HVDC::reverse_converters()
{
    size_t bus = converter_bus[0];
    converter_bus[0] = converter_bus[1];
    converter_bus[1] = bus;

    size_t number = bridge_number[0];
    bridge_number[0] = bridge_number[1];
    bridge_number[1] = number;

    double voltage = voltage_drop_per_bridge_in_kV[0];
    voltage_drop_per_bridge_in_kV[0] = voltage_drop_per_bridge_in_kV[1];
    voltage_drop_per_bridge_in_kV[1] = voltage;

    double angle = max_firing_angle_in_deg[0];
    max_firing_angle_in_deg[0] = max_firing_angle_in_deg[1];
    max_firing_angle_in_deg[1] = angle;

    angle = min_firing_angle_in_deg[0];
    min_firing_angle_in_deg[0] = min_firing_angle_in_deg[1];
    min_firing_angle_in_deg[1] = angle;


    voltage = converter_transformer_grid_side_base_voltage_in_kV[0];
    converter_transformer_grid_side_base_voltage_in_kV[0] = converter_transformer_grid_side_base_voltage_in_kV[1];
    converter_transformer_grid_side_base_voltage_in_kV[1] = voltage;

    voltage = converter_transformer_converter_side_base_voltage_in_kV[0];
    converter_transformer_converter_side_base_voltage_in_kV[0] = converter_transformer_converter_side_base_voltage_in_kV[1];
    converter_transformer_converter_side_base_voltage_in_kV[1] = voltage;

    complex<double> Z = converter_transformer_Z_in_ohm[0];
    converter_transformer_Z_in_ohm[0] = converter_transformer_Z_in_ohm[1];
    converter_transformer_Z_in_ohm[1] = Z;

    complex<double> Y = converter_transformer_Y_in_siemens[0];
    converter_transformer_Y_in_siemens[0] = converter_transformer_Y_in_siemens[1];
    converter_transformer_Y_in_siemens[1] = Y;

    double tap = converter_transformer_max_tap_in_pu[0];
    converter_transformer_max_tap_in_pu[0] = converter_transformer_max_tap_in_pu[1];
    converter_transformer_max_tap_in_pu[1] = tap;

    tap = converter_transformer_min_tap_in_pu[0];
    converter_transformer_min_tap_in_pu[0] = converter_transformer_min_tap_in_pu[1];
    converter_transformer_min_tap_in_pu[1] = tap;

    size_t n = converter_transformer_number_of_taps[0];
    converter_transformer_number_of_taps[0] = converter_transformer_number_of_taps[1];
    converter_transformer_number_of_taps[1] = n;

    angle = converter_firing_angle_in_deg[0];
    converter_firing_angle_in_deg[0] = converter_firing_angle_in_deg[1];
    converter_firing_angle_in_deg[1] = angle;

    tap = converter_transformer_tap_in_pu[0];
    converter_transformer_tap_in_pu[0] = converter_transformer_tap_in_pu[1];
    converter_transformer_tap_in_pu[1] = tap;

    voltage = converter_dc_voltage_in_kV[0];
    converter_dc_voltage_in_kV[0] = converter_dc_voltage_in_kV[1];
    converter_dc_voltage_in_kV[1] = voltage;
}

bool HVDC::is_valid() const
{
    if(get_converter_bus(RECTIFIER)!=0 and get_converter_bus(INVERTER)!=0) return true;
    else return false;
}

void HVDC::check()
{
    ;
}

void HVDC::clear()
{
    converter_bus[RECTIFIER] = 0;
    converter_bus[INVERTER] = 0;
    converter_valve_bus_name[RECTIFIER]="";
    converter_valve_bus_name[INVERTER]="";
    set_identifier(""); set_name(""); set_status(false);
    set_number_of_poles(SINGLE_POLE);
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

    set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    set_converter_operation_mode(INVERTER, INVERTER_CONSTANT_VOLTAGE);
    bridge_number[RECTIFIER] = 0; bridge_number[INVERTER] = 0;
    set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, 0.0);
    set_converter_voltage_drop_per_bridge_in_kV(INVERTER, 0.0);
    set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, 90.0);
    set_converter_max_alpha_or_gamma_in_deg(INVERTER, 90.0);
    set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, 5.0);
    set_converter_min_alpha_or_gamma_in_deg(INVERTER, 5.0);

    converter_transformer_grid_side_base_voltage_in_kV[RECTIFIER] = 0.0;
    converter_transformer_grid_side_base_voltage_in_kV[INVERTER] = 0.0;
    converter_transformer_converter_side_base_voltage_in_kV[RECTIFIER] = 0.0;
    converter_transformer_converter_side_base_voltage_in_kV[INVERTER] = 0.0;
    set_converter_transformer_impedance_in_ohm(RECTIFIER, 0.0);
    set_converter_transformer_impedance_in_ohm(INVERTER, 0.0);
    set_converter_transformer_admittance_in_siemens(RECTIFIER, 0.0);
    set_converter_transformer_admittance_in_siemens(INVERTER, 0.0);
    set_converter_transformer_max_tap_in_pu(RECTIFIER, 0.0);
    set_converter_transformer_max_tap_in_pu(INVERTER, 0.0);
    set_converter_transformer_min_tap_in_pu(RECTIFIER, 0.0);
    set_converter_transformer_min_tap_in_pu(INVERTER, 0.0);
    set_converter_transformer_number_of_taps(RECTIFIER, 0);
    set_converter_transformer_number_of_taps(INVERTER, 0);

    set_converter_alpha_or_gamma_in_deg(RECTIFIER, 0.0);
    set_converter_alpha_or_gamma_in_deg(INVERTER, 0.0);
    set_converter_transformer_tap_in_pu(RECTIFIER, 1.0);
    set_converter_transformer_tap_in_pu(INVERTER, 1.0);

    hvdc_model = NULL;
    auxiliary_signal_model = NULL;
}

bool HVDC::is_connected_to_bus(size_t bus) const
{
    if(get_converter_bus(RECTIFIER)==bus || get_converter_bus(INVERTER)==bus) return true;
    else return false;
}

bool HVDC::is_in_area(size_t area) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr_rec = psdb.get_bus(get_converter_bus(RECTIFIER));
    BUS* busptr_inv = psdb.get_bus(get_converter_bus(INVERTER));
    if(busptr_rec!=NULL or busptr_inv!=NULL)
    {
        bool in_area = false;
        if(busptr_rec!=NULL)
            in_area = in_area or busptr_rec->is_in_area(area);
        if(busptr_inv!=NULL)
            in_area = in_area or busptr_inv->is_in_area(area);
        return in_area;
    }
    else
        return false;
}

bool HVDC::is_in_zone(size_t zone) const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr_rec = psdb.get_bus(get_converter_bus(RECTIFIER));
    BUS* busptr_inv = psdb.get_bus(get_converter_bus(INVERTER));
    if(busptr_rec!=NULL or busptr_inv!=NULL)
    {
        bool in_zone = false;
        if(busptr_rec!=NULL)
            in_zone = in_zone or busptr_rec->is_in_zone(zone);
        if(busptr_inv!=NULL)
            in_zone = in_zone or busptr_inv->is_in_zone(zone);
        return in_zone;
    }
    else
        return false;
}

void HVDC::report() const
{
    ostringstream osstream;

    osstream<<"HVDC '"<<get_name()<<"': Rdc = "<<get_line_resistance_in_ohm()<<" ohm";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Vdc = "<<get_nominal_dc_voltage_per_pole_in_kV()<<" kV, "
      <<"Idc = "<<get_nominal_dc_current_per_pole_in_kA()<<" kA, "
      <<"Pdc = "<<get_nominal_dc_power_per_pole_in_MW()<<" MW";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Compensating R = "<<get_compensating_resistance_to_hold_dc_voltage_in_ohm()<<" ohm";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Rectifier at bus "<<get_converter_bus(RECTIFIER)<<": "<<get_converter_number_of_bridge(RECTIFIER)<<" bridges";
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"AlphaMax = "<<get_converter_max_alpha_or_gamma_in_deg(RECTIFIER)<<" deg, AlphaMin = "<<get_converter_min_alpha_or_gamma_in_deg(RECTIFIER)<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"Transformer grid side base = "<<get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER)<<" kV, "
      <<"converter side base = "<<get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER)<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"Tap max = "<<get_converter_transformer_max_tap_in_pu(RECTIFIER)<<", tap min = "<<get_converter_transformer_min_tap_in_pu(RECTIFIER);
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"Rcr+jXcr = "<<get_converter_transformer_impedance_in_ohm(RECTIFIER)<<" ohm";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Inverter at bus "<<get_converter_bus(INVERTER)<<": "<<get_converter_number_of_bridge(INVERTER)<<" bridges";
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"GammaMax = "<<get_converter_max_alpha_or_gamma_in_deg(INVERTER)<<" deg, GammaMin = "<<get_converter_min_alpha_or_gamma_in_deg(INVERTER)<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"Transformer grid side base = "<<get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)<<" kV, "
      <<"converter side base = "<<get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER)<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"Tap max = "<<get_converter_transformer_max_tap_in_pu(INVERTER)<<", tap min = "<<get_converter_transformer_min_tap_in_pu(INVERTER);
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"Rci+jXci = "<<get_converter_transformer_impedance_in_ohm(INVERTER)<<" ohm";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void HVDC::save() const
{
    ;
}

void HVDC::set_model(const MODEL* model)
{
    ostringstream osstream;
    if(model->get_model_type()=="AUXILIARY SIGNAL")
        set_auxiliary_signal_model((AUXILIARY_SIGNAL_MODEL*) model);
    else
    {
        if(model->get_model_type()=="HVDC")
            set_hvdc_model((HVDC_MODEL*) model);
        else
        {
            osstream<<"Waring. Neither AUXILIARY SIGNAL model nor HVDC model is given to set dynamic model for "<<get_device_name();
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void HVDC::set_hvdc_model(const HVDC_MODEL* model)
{
    STEPS& toolkit = get_toolkit();

    ostringstream osstream;
    if(model->get_allowed_device_type()!="HVDC")
    {
        osstream<<"Waring. Dynamic model of device type "<<model->get_allowed_device_type()<<" is not allowed to set up HVDC model for "<<get_device_name()<<endl
          <<"No HVDC model is set.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(model->get_model_type()!="HVDC")
    {
        osstream<<"Waring. "<<model->get_model_type()<<" model is not allowed to set up HVDC model for "<<get_device_name()<<endl
          <<"No HVDC model is set.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    HVDC_MODEL* oldmodel = get_hvdc_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        hvdc_model = NULL;
    }

    HVDC_MODEL *new_model = NULL;
    string model_name = model->get_model_name();
    if(model_name=="CDC4T")
    {
        CDC4T* smodel = (CDC4T*) (model);
        new_model = (HVDC_MODEL*) new CDC4T(*smodel);
    }
    if(model_name=="CDC6T")
    {
        CDC6T* smodel = (CDC6T*) (model);
        new_model = (HVDC_MODEL*) new CDC6T(*smodel);
    }

    if(new_model!=NULL)
    {
        new_model->set_toolkit(toolkit);
        new_model->set_device_id(get_device_id());
        hvdc_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append HVDC model of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC::set_auxiliary_signal_model(const AUXILIARY_SIGNAL_MODEL* model)
{
    STEPS& toolkit = get_toolkit();

    ostringstream osstream;
    if(model->get_model_type()!="AUXILIARY SIGNAL")
    {
        osstream<<"Waring. "<< model->get_model_type()<<" model is not allowed to set up AUXILIARY SIGNAL model for "<<get_device_name()<<endl
          <<"No AUXILIARY SIGNAL model is set.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    AUXILIARY_SIGNAL_MODEL* oldmodel = get_auxiliary_signal_model();
    if(oldmodel!=NULL)
    {
        delete oldmodel;
        auxiliary_signal_model = NULL;
    }

    AUXILIARY_SIGNAL_MODEL *new_model = NULL;
    string model_name = model->get_model_name();

    if(new_model!=NULL)
    {
        new_model->set_toolkit(toolkit);
        new_model->set_device_id(get_device_id());
        auxiliary_signal_model = new_model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Model '"<<model_name<<"' is not supported when append AUXILIARY SIGNAL model of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void HVDC::clear_hvdc_model()
{
    if(hvdc_model!=NULL)
    {
        delete hvdc_model;
        hvdc_model = NULL;
    }
}

void HVDC::clear_auxiliary_signal_model()
{
    if(auxiliary_signal_model!=NULL)
    {
        delete auxiliary_signal_model;
        auxiliary_signal_model = NULL;
    }
}

HVDC_MODEL* HVDC::get_hvdc_model() const
{
    return hvdc_model;
}

AUXILIARY_SIGNAL_MODEL* HVDC::get_auxiliary_signal_model() const
{
    return auxiliary_signal_model;
}

void HVDC::run(DYNAMIC_MODE mode)
{
    if(get_status()==false)
        return;

    switch(mode)
    {
        case INITIALIZE_MODE:
        {
            HVDC_MODEL* hvdc_model = get_hvdc_model();
            if(hvdc_model==NULL)
                return;
            hvdc_model->initialize();

            break;
        }
        default:
        {
            HVDC_MODEL* hvdc_model = get_hvdc_model();
            if(hvdc_model==NULL)
                return;
            hvdc_model->run(mode);

            break;
        }
    }
}

HVDC& HVDC::operator=(const HVDC& hvdc)
{
    if(this == (&hvdc)) return *this;

    clear();

    set_toolkit(hvdc.get_toolkit());

    if(hvdc.get_converter_bus(RECTIFIER)!=0)
        set_converter_bus(RECTIFIER, hvdc.get_converter_bus(RECTIFIER));

    if(hvdc.get_converter_bus(INVERTER)!=0)
        set_converter_bus(INVERTER, hvdc.get_converter_bus(INVERTER));

    set_identifier(hvdc.get_identifier());
    set_name(hvdc.get_name());
    set_status(hvdc.get_status());
    set_number_of_poles(hvdc.get_number_of_poles());
    set_line_resistance_in_ohm(hvdc.get_line_resistance_in_ohm());
    set_line_inductance_in_mH(hvdc.get_line_inductance_in_mH());
    set_line_capacitance_in_uF(hvdc.get_line_capacitance_in_uF());
    set_line_smooting_inductance_in_mH(RECTIFIER, hvdc.get_line_smooting_inductance_in_mH(RECTIFIER));
    set_line_smooting_inductance_in_mH(INVERTER, hvdc.get_line_smooting_inductance_in_mH(INVERTER));
    set_line_smooting_resistance_in_ohm(RECTIFIER, hvdc.get_line_smooting_resistance_in_ohm(RECTIFIER));
    set_line_smooting_resistance_in_ohm(INVERTER, hvdc.get_line_smooting_resistance_in_ohm(INVERTER));
    set_nominal_dc_power_per_pole_in_MW(hvdc.get_nominal_dc_power_per_pole_in_MW());
    set_side_to_hold_power(hvdc.get_side_to_hold_dc_power());
    set_nominal_dc_current_per_pole_in_kA(hvdc.get_nominal_dc_current_per_pole_in_kA());
    set_nominal_dc_voltage_per_pole_in_kV(hvdc.get_nominal_dc_voltage_per_pole_in_kV());
    set_compensating_resistance_to_hold_dc_voltage_in_ohm(hvdc.get_compensating_resistance_to_hold_dc_voltage_in_ohm());
    set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(hvdc.get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV());
    set_current_power_margin(hvdc.get_current_power_margin());

    set_converter_operation_mode(RECTIFIER, hvdc.get_converter_operation_mode(RECTIFIER));
    set_converter_operation_mode(INVERTER, hvdc.get_converter_operation_mode(INVERTER));
    set_converter_number_of_bridge(RECTIFIER, hvdc.get_converter_number_of_bridge(RECTIFIER));
    set_converter_number_of_bridge(INVERTER, hvdc.get_converter_number_of_bridge(INVERTER));
    set_converter_voltage_drop_per_bridge_in_kV(RECTIFIER, hvdc.get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER));
    set_converter_voltage_drop_per_bridge_in_kV(INVERTER, hvdc.get_converter_voltage_drop_per_bridge_in_kV(INVERTER));
    set_converter_max_alpha_or_gamma_in_deg(RECTIFIER, hvdc.get_converter_max_alpha_or_gamma_in_deg(RECTIFIER));
    set_converter_max_alpha_or_gamma_in_deg(INVERTER, hvdc.get_converter_max_alpha_or_gamma_in_deg(INVERTER));
    set_converter_min_alpha_or_gamma_in_deg(RECTIFIER, hvdc.get_converter_min_alpha_or_gamma_in_deg(RECTIFIER));
    set_converter_min_alpha_or_gamma_in_deg(INVERTER, hvdc.get_converter_min_alpha_or_gamma_in_deg(INVERTER));

    set_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER, hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER));
    set_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER, hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER));
    set_converter_transformer_grid_side_base_voltage_in_kV(INVERTER, hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER));
    set_converter_transformer_converter_side_base_voltage_in_kV(INVERTER, hvdc.get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER));
    set_converter_transformer_impedance_in_ohm(RECTIFIER, hvdc.get_converter_transformer_impedance_in_ohm(RECTIFIER));
    set_converter_transformer_impedance_in_ohm(INVERTER, hvdc.get_converter_transformer_impedance_in_ohm(INVERTER));
    set_converter_transformer_admittance_in_siemens(RECTIFIER, hvdc.get_converter_transformer_admittance_in_siemens(RECTIFIER));
    set_converter_transformer_admittance_in_siemens(INVERTER, hvdc.get_converter_transformer_admittance_in_siemens(INVERTER));
    set_converter_transformer_max_tap_in_pu(RECTIFIER, hvdc.get_converter_transformer_max_tap_in_pu(RECTIFIER));
    set_converter_transformer_max_tap_in_pu(INVERTER, hvdc.get_converter_transformer_max_tap_in_pu(INVERTER));
    set_converter_transformer_min_tap_in_pu(RECTIFIER, hvdc.get_converter_transformer_min_tap_in_pu(RECTIFIER));
    set_converter_transformer_min_tap_in_pu(INVERTER, hvdc.get_converter_transformer_min_tap_in_pu(INVERTER));
    set_converter_transformer_number_of_taps(RECTIFIER, hvdc.get_converter_transformer_number_of_taps(RECTIFIER));
    set_converter_transformer_number_of_taps(INVERTER, hvdc.get_converter_transformer_number_of_taps(INVERTER));

    return *this;
}

DEVICE_ID HVDC::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("HVDC");

    TERMINAL terminal;
    terminal.append_bus(get_converter_bus(RECTIFIER));
    terminal.append_bus(get_converter_bus(INVERTER));
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

/*string HVDC::get_device_name() const
{
    return get_device_id().get_device_name();
}*/


void HVDC::solve_steady_state()
{
    ostringstream osstream;

    //os<<"Solve "<<get_device_name()<<endl;
    //show_information_with_leading_time_stamp(osstream);

    HVDC_OPERATION_MODE r_mode = get_converter_operation_mode(RECTIFIER), i_mode = get_converter_operation_mode(INVERTER);

    if(r_mode != RECTIFIER_CONSTANT_POWER and r_mode != RECTIFIER_CONSTANT_CURRENT)
    {
        osstream<<"Warning. Rectifier of "<<get_device_name()<<" is not in either RECTIFIER_CONSTANT_POWER or"
                     " RECTIFIER_CONSTANT_CURRENT mode."<<endl<<"HVDC will not be solved.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(i_mode != INVERTER_CONSTANT_VOLTAGE and r_mode != INVERTER_CONSTANT_GAMMA)
    {
        osstream<<"Warning. Inverter of "<<get_device_name()<<" is not in either INVERTER_CONSTANT_VOLTAGE or"
                     " INVERTER_CONSTANT_GAMMA mode."<<endl<<"HVDC will not be solved.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(r_mode == RECTIFIER_CONSTANT_POWER)
    {
        if(i_mode == INVERTER_CONSTANT_VOLTAGE)
            solve_as_rectifier_regulating_power_and_inverter_regulating_voltage();
        else
            solve_as_rectifier_regulating_power_and_inverter_regulating_gamma();
    }
    else// RECTIFIER_CONSTANT_CURRENT
    {
        if(i_mode == INVERTER_CONSTANT_VOLTAGE)
            solve_as_rectifier_regulating_current_and_inverter_regulating_voltage();
        else
            solve_as_rectifier_regulating_current_and_inverter_regulating_gamma();
    }
    //os<<"AC power at rectifier side: "<<get_converter_ac_active_power_in_MW(RECTIFIER)<<" MW, "<<get_converter_ac_reactive_power_in_MVar(RECTIFIER)<<" MVar."<<endl;
    //os<<"AC power at inverter side: "<<get_converter_ac_active_power_in_MW(INVERTER)<<" MW, "<<get_converter_ac_reactive_power_in_MVar(INVERTER)<<" MVar."<<endl;
    //show_information_with_leading_time_stamp(osstream);
}

void HVDC::show_solved_hvdc_steady_state() const
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    osstream<<"Solved steady state of HVDC '"<<get_name()<<"':";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Idc = "<<get_line_dc_current_in_kA()<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    HVDC_CONVERTER_SIDE converter=RECTIFIER;

    HVDC_OPERATION_MODE  mode = get_converter_operation_mode(converter);
    string modestr;
    switch(mode)
    {
        case RECTIFIER_CONSTANT_POWER:
            modestr = "constant power";
            if(get_side_to_hold_dc_power()==RECTIFIER)
                modestr += " at rectifier side";
            else
                modestr += " at inverter side";
            break;
        case RECTIFIER_CONSTANT_CURRENT:
            modestr = "constant current";
            break;
        case INVERTER_CONSTANT_VOLTAGE:
            modestr = "constant voltage";
            if(get_compensating_resistance_to_hold_dc_voltage_in_ohm()==0.0)
                modestr += " at inverter side";
            else
            {
                if(get_compensating_resistance_to_hold_dc_voltage_in_ohm()==get_line_resistance_in_ohm())
                    modestr += " at rectifier side";
                else
                    modestr += " at location of compensating R";
            }
            break;
        case INVERTER_CONSTANT_GAMMA:
            modestr = "constant firing angle";
            break;
    }
    osstream<<"Rectifier: "<<modestr;
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Vdc = "<<get_converter_dc_voltage_in_kV(converter)<<" kV, "
      <<"Vac = "<<psdb.get_bus_voltage_in_kV(get_converter_bus(converter))<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Transformer tap = "<<get_converter_transformer_tap_in_pu(converter)<<" pu.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"alpha = "<<get_converter_alpha_or_gamma_in_deg(converter)<<" deg, "
      <<"mu = "<<get_converter_commutating_overlap_angle_in_deg(converter)<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Rectifier AC current = "<<get_converter_ac_current_in_kA(converter)<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Pdc = "<<get_converter_dc_power_in_MW(converter)<<" MW, "
      <<"Pac = "<<get_converter_ac_active_power_in_MW(converter)<<" MW, "
      <<"Qac = "<<get_converter_ac_reactive_power_in_MVar(converter)<<" MVar, "
      <<"Eta = "<<get_converter_ac_power_factor(converter);
    toolkit.show_information_with_leading_time_stamp(osstream);


    converter = INVERTER;
    mode = get_converter_operation_mode(converter);
    switch(mode)
    {
        case INVERTER_CONSTANT_VOLTAGE:
        {
            modestr = "constant voltage";
            if(get_compensating_resistance_to_hold_dc_voltage_in_ohm()==0.0)
                modestr += " at inverter side";
            else
            {
                if(get_compensating_resistance_to_hold_dc_voltage_in_ohm()==get_line_resistance_in_ohm())
                    modestr += " at rectifier side";
                else
                    modestr += " at location of compensating R";
            }
            break;
        }
        case INVERTER_CONSTANT_GAMMA:
        {
            modestr = "constant firing angle";
            break;
        }
        default:
        {
            osstream<<"Mode "<<mode<<" is not supported for inverter side of "<<get_device_name()<<endl
                    <<"This line should never appear. If you find it, ask for help.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

    }
    osstream<<"Rectifier: "<<modestr;
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Vdc = "<<get_converter_dc_voltage_in_kV(converter)<<" kV, "
      <<"Vac = "<<psdb.get_bus_voltage_in_kV(get_converter_bus(converter))<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Transformer tap = "<<get_converter_transformer_tap_in_pu(converter)<<" pu.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"gamma = "<<get_converter_alpha_or_gamma_in_deg(converter)<<" deg, "
      <<"mu = "<<get_converter_commutating_overlap_angle_in_deg(converter)<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Inverter AC current = "<<get_converter_ac_current_in_kA(converter)<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Pdc = "<<get_converter_dc_power_in_MW(converter)<<" MW, "
      <<"Pac = "<<get_converter_ac_active_power_in_MW(converter)<<" MW, "
      <<"Qac = "<<get_converter_ac_reactive_power_in_MVar(converter)<<" MVar, "
      <<"Eta = "<<get_converter_ac_power_factor(converter);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void HVDC::set_converter_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, double angle)
{
    if(angle>=0.0)
        converter_firing_angle_in_deg[converter] = angle;
    else
        converter_firing_angle_in_deg[converter] = 0.0;
}

void HVDC::set_converter_transformer_tap_in_pu(HVDC_CONVERTER_SIDE converter, double tap)
{
    if(tap>0.0)
        converter_transformer_tap_in_pu[converter] = tap;
    else
        converter_transformer_tap_in_pu[converter] = 1.0;
}


void HVDC::set_converter_dc_voltage_in_kV(HVDC_CONVERTER_SIDE converter, const double V)
{
    if(V>=0.0)
        converter_dc_voltage_in_kV[converter] = V;
    else
        converter_dc_voltage_in_kV[converter] = 0.0;
}


void HVDC::set_line_dc_current_in_kA(const double I)
{
    set_converter_dc_current_in_kA(RECTIFIER, I);
    set_converter_dc_current_in_kA(INVERTER, I);
}

void HVDC::set_converter_dc_current_in_kA(HVDC_CONVERTER_SIDE converter, const double I)
{
    line_dc_current_in_kA[converter] = I;
}

double HVDC::get_converter_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    return converter_firing_angle_in_deg[converter];
}

double HVDC::get_converter_transformer_tap_in_pu(HVDC_CONVERTER_SIDE converter) const
{
    return converter_transformer_tap_in_pu[converter];
}


double HVDC::get_converter_commutating_overlap_angle_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double Tap = get_converter_transformer_tap_in_pu(converter);
    double Vac = psdb.get_bus_voltage_in_kV(get_converter_bus(converter));
    double Eac = Vac/TurnRatio/Tap;

    double Xc = get_converter_transformer_impedance_in_ohm(converter).imag();
    double Idc = get_line_dc_current_in_kA();
    double alpha_gamma = deg2rad(get_converter_alpha_or_gamma_in_deg(converter));

    double mu = acos(cos(alpha_gamma)-sqrt(2.0)*Idc*Xc/Eac)-alpha_gamma;

    return rad2deg(mu);
}

double HVDC::get_converter_dc_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const
{
    return converter_dc_voltage_in_kV[converter];
}

double HVDC::get_line_dc_current_in_kA() const
{
    return line_dc_current_in_kA[RECTIFIER];
}

double HVDC::get_converter_dc_current_in_kA(HVDC_CONVERTER_SIDE converter) const
{
    return line_dc_current_in_kA[converter];
}

double HVDC::get_converter_ac_current_in_kA(HVDC_CONVERTER_SIDE converter) const
{
    size_t N = get_converter_number_of_bridge(converter);
    double Idc = get_line_dc_current_in_kA();
    double turnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double tap = get_converter_transformer_tap_in_pu(converter);

    return sqrt(6.0)*N/PI*Idc/(turnRatio*tap);
}

double HVDC::get_converter_dc_power_in_MW(HVDC_CONVERTER_SIDE converter) const
{
    return get_converter_dc_voltage_in_kV(converter)*get_line_dc_current_in_kA();
}

double HVDC::get_converter_ac_active_power_in_MW(HVDC_CONVERTER_SIDE converter) const
{
    /*double S = get_converter_ac_apparent_power_in_MVA(converter);
    double pf = get_converter_ac_power_factor(converter);

    return S*pf;*/

    double Iac = get_converter_ac_current_in_kA(converter);

    size_t N = get_converter_number_of_bridge(converter);
    double R = get_converter_transformer_impedance_in_ohm(converter).real();
    double loss = sqrt(3.0)*Iac*Iac*R/N;

    switch(converter)
    {
        case RECTIFIER:
            return get_converter_dc_power_in_MW(converter)+loss;
        default:
            return get_converter_dc_power_in_MW(converter)-loss;
    }
}

double HVDC::get_converter_ac_reactive_power_in_MVar(HVDC_CONVERTER_SIDE converter) const
{
    double Pac = get_converter_ac_active_power_in_MW(converter);
    double pf = get_converter_ac_power_factor(converter);

    return Pac*sqrt(1.0-pf*pf)/pf;
}

double HVDC::get_converter_ac_apparent_power_in_MVA(HVDC_CONVERTER_SIDE converter) const
{
    double Pac = get_converter_ac_active_power_in_MW(converter);
    double pf = get_converter_ac_power_factor(converter);
    if(pf==0.0)
        return Pac;
    else
        return Pac/pf;
/*
    double Iac = get_converter_ac_current_in_kA(converter);
    double U = psdb.get_bus_voltage_in_kV(get_converter_bus(converter));
    return sqrt(3.0)*U*Iac;

    double turnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double tap = get_converter_transformer_tap_in_pu(converter);
    double Eac = U/turnRatio/tap;
    return sqrt(3.0)*Eac*Iac;
*/
}

double HVDC::get_converter_ac_power_factor(HVDC_CONVERTER_SIDE converter) const
{
    double mu = get_converter_commutating_overlap_angle_in_deg(converter);
    mu = deg2rad(mu);
    double alpha_gamma = get_converter_alpha_or_gamma_in_deg(converter);
    alpha_gamma = deg2rad(alpha_gamma);

    double tanPhi = 2*mu+sin(2*alpha_gamma)-sin(2.0*(mu+alpha_gamma));
    tanPhi /= (cos(2.0*alpha_gamma)-cos(2.0*(mu+alpha_gamma)));

    return cos(atan(tanPhi));
}


void HVDC::solve_as_rectifier_regulating_power_and_inverter_regulating_voltage()
{
    ostringstream osstream;
    //osstream<<"Solve "<<get_device_name()<<" as constant power (R) + constant voltage (I) mode.";
    //show_information_with_leading_time_stamp(osstream);

    // solve desired dc voltage and dc current
    double VdcR=0.0, VdcI=0.0, Idc=0.0;
    //solve_deisired_dc_voltage_in_V_and_dc_current_in_RECTIFIER_CONSTANT_POWER_INVERTER_CONSTANT_VOLTAGE(VdcR, VdcI, Idc);

    VdcI = get_inverter_nominal_dc_voltage_command_in_kV();
    Idc = get_rectifier_nominal_dc_current_command_in_kA();
    double Rdc = get_line_resistance_in_ohm();
    VdcR = VdcI+Idc*Rdc;
    //os<< "Desired VdcR = %f kV, VdcI = %f kV, Idc = %f kA.", VdcR, VdcI, Idc);
    //show_information_with_leading_time_stamp(osstream);

    // solve desired alpha and best tap
    bool minAlphaReached = solve_converter_transformer_tap_and_desired_firing_angle(RECTIFIER, VdcR, Idc);
    bool minGammaReached = solve_converter_transformer_tap_and_desired_firing_angle(INVERTER, VdcI, Idc);

    if(minAlphaReached)
    {
        osstream<<"Minimum alpha reached when trying to solve "<<get_device_name()
          <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(minGammaReached)
    {
        osstream<<"Minimum gamma reached when trying to solve "<<get_device_name()
          <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(not(minAlphaReached) and not(minGammaReached))
    {
        // solved!
        solve_with_solved_tap_and_firing_angle();
    }
    else
    {
        if(not(minAlphaReached) and minGammaReached)
        {
            osstream<<"Minimum gamma reached when trying to solve "<<get_device_name()
              <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.\n"
              <<"HVDC link will turn into CONSTANT GAMMA mode for inverter.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);

            solve_as_rectifier_regulating_power_and_inverter_regulating_gamma();
        }
        else
        {
            if(minAlphaReached and not(minGammaReached))
            {
                osstream<<"Minimum alpha reached when trying to solve "<<get_device_name()
                  <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.\n"
                  <<"HVDC link will turn into reduced (DELTI) CONSTANT CURRENT mode controlled by inverter.";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);

                solve_as_rectifier_regulating_alpha_and_inverter_regulating_current();
            }
            else
            {
                osstream<<"Warning. Both minimum alpha and minimum gamma reached when trying to solve "<<get_device_name()
                  <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.\n"
                  <<"HVDC link will turn into reduced (DELTI) CONSTANT CURRENT mode controlled by inverter.";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);

                //solve_with_solved_tap_and_firing_angle();
                solve_as_rectifier_regulating_alpha_and_inverter_regulating_current();
            }
        }
    }
}

bool HVDC::solve_converter_transformer_tap_and_desired_firing_angle(HVDC_CONVERTER_SIDE converter, double Vdc, double Idc)
{
    ostringstream osstream;
    //cout<<"solving "<<get_device_name()<<" with "<<__FUNCTION__<<": "<<get_converter_side_name(converter)<<" desired Vdc = "<<Vdc<<"kV, Idc = "<<Idc<<endl;
    solve_best_converter_transformer_tap_with_min_angle(converter, Vdc, Idc);
    double Tap = get_converter_transformer_tap_in_pu(converter);

    double angle_min = get_converter_min_alpha_or_gamma_in_deg(converter);
    angle_min = deg2rad(angle_min);
    double cos_angle_min = cos(angle_min);
    bool minAngleReached = false;
    double cosAngle =  0.0;
    while(true)// try to decrease tap
    {
        if(converter==RECTIFIER)
            cosAngle = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(RECTIFIER, Vdc, Idc, Tap);
        else
            cosAngle = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(INVERTER, Vdc, Idc, Tap);
        //cout<<"desired cosAngle is :"<<cosAngle<<endl;
        if(cosAngle>cos_angle_min)
        {
            minAngleReached = true;
            double Tapmax = get_converter_transformer_min_tap_in_pu(converter);
            double Tapmin = get_converter_transformer_min_tap_in_pu(converter);
            size_t nTap = get_converter_transformer_number_of_taps(converter);
            if(nTap==1 or fabs(Tapmax-Tapmin)<FLOAT_EPSILON) // tap not changeable
                break;
            if(fabs(Tap-Tapmin)<FLOAT_EPSILON) //reach the minimum
                break;
            double TapStep = (Tapmax-Tapmin)/(nTap-1);
            Tap -= TapStep;
            set_converter_transformer_tap_in_pu(converter, Tap);
        }
        else
        {
            minAngleReached = false;
            break;
        }
    }
    // set firing angle
    if(not minAngleReached)
        set_converter_alpha_or_gamma_in_deg(converter, rad2deg(acos(cosAngle)));
    else
        set_converter_alpha_or_gamma_in_deg(converter, get_converter_min_alpha_or_gamma_in_deg(converter));

    //osstream<<get_device_name()<<": trying to solve "<<get_converter_side_name(converter)<<" transformer tap = "<<get_converter_transformer_tap_in_pu(converter)
    //       <<", best firing angle = "<<get_converter_alpha_or_gamma_in_deg(converter)<<" deg";
    //show_information_with_leading_time_stamp(osstream);

    return minAngleReached;
}


void HVDC::solve_best_converter_transformer_tap_with_min_angle(HVDC_CONVERTER_SIDE converter, double Vdc, double Idc)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t N = get_converter_number_of_bridge(converter);

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double maxTap = get_converter_transformer_max_tap_in_pu(converter);
    double minTap = get_converter_transformer_min_tap_in_pu(converter);
    size_t nTap = get_converter_transformer_number_of_taps(converter);

    if(fabs(maxTap-minTap)<FLOAT_EPSILON or nTap == 1)
    {
        set_converter_transformer_tap_in_pu(converter, minTap);
        return;
    }

    double TapStep = (maxTap-minTap)/(nTap-1);

    complex<double> Z = get_converter_transformer_impedance_in_ohm(converter);

    double Vdrop = get_converter_voltage_drop_per_bridge_in_kV(converter);

    double angle_min = get_converter_min_alpha_or_gamma_in_deg(converter);
    angle_min = deg2rad(angle_min);

    double Vbus = psdb.get_bus_voltage_in_kV(get_converter_bus(converter));

    double Eac_cosAngle = Vdc/N+3.0*Z.imag()/PI*Idc+2.0*Z.real()*Idc+Vdrop;
    Eac_cosAngle /= (3.0*sqrt(2.0)/PI);
    double Eac = Eac_cosAngle/cos(angle_min);
    double Tap = (Vbus/Eac)/TurnRatio; // desired
    Tap = minTap +  TapStep*floor((Tap-minTap)/TapStep); // actual
    if(Tap < minTap) Tap = minTap; // apply limit
    if(Tap > maxTap) Tap = maxTap;

    //os<<"Best converter transformer tap with min angle: %f for %s", Tap, get_converter_side_name(converter).c_str());
    //show_information_with_leading_time_stamp(osstream);

    set_converter_transformer_tap_in_pu(converter, Tap);
    return;
}

void HVDC::solve_as_rectifier_regulating_current_and_inverter_regulating_voltage()
{
    ostringstream osstream;
    //os<<"Solve %s as constant current (R) + constant voltage (I) mode.",
    //              get_device_name().c_str());
    //show_information_with_leading_time_stamp(osstream);

    // solve the desired dc voltage and dc current
    double Idc = get_rectifier_nominal_dc_current_command_in_kA();
    double VdcI = get_inverter_nominal_dc_voltage_command_in_kV();
    double R = get_line_resistance_in_ohm();
    double VdcR = VdcI + Idc*R;

    osstream<<"trying to solve "<<get_device_name()<<" with desired DC voltage rec "<<VdcR<<" kV, inv "<<VdcI<<" kV, "
           <<" I "<<Idc<<" kA";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    bool minAlphaReached = solve_converter_transformer_tap_and_desired_firing_angle(RECTIFIER, VdcR, Idc);
    bool minGammaReached = solve_converter_transformer_tap_and_desired_firing_angle(INVERTER, VdcI, Idc);

    if(not(minAlphaReached) and not(minGammaReached))
    {
        // solved!
        solve_with_solved_tap_and_firing_angle();
    }
    else
    {
        if(not(minAlphaReached) and minGammaReached)
        {
            osstream<<"Minimum gamma reached when trying to solve "<<get_device_name()
              <<" with CONSTANT CURRENT and CONSTANT VOTLAGE mode.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            solve_as_rectifier_regulating_current_and_inverter_regulating_gamma();
        }
        else
        {
            if(minAlphaReached and not(minGammaReached))
            {
                osstream<<"Minimum alpha reached when trying to solve "<<get_device_name()
                  <<" with CONSTANT CURRENT and CONSTANT VOTLAGE mode.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                solve_as_rectifier_regulating_alpha_and_inverter_regulating_current();
            }
            else
            {
                // error!!!!!
                osstream<<"Warning. Both minimum alpha and minimum gamma reached when trying to solve "<<get_device_name()
                  <<" with CONSTANT CURRENT and CONSTANT VOTLAGE mode."
                  <<"HVDC link will be with minimum alpha and gamma. The solution may be incorrect.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                solve_with_solved_tap_and_firing_angle();
            }
        }
    }
}

void HVDC::solve_as_rectifier_regulating_power_and_inverter_regulating_gamma()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    //osstream<<"Solve "<<get_device_name()<<" as constant power (R) + constant gamma (I) mode.";
    //show_information_with_leading_time_stamp(osstream);

    double Pn = get_nominal_dc_power_per_pole_in_MW();
    HVDC_CONVERTER_SIDE Pside = get_side_to_hold_dc_power();
    double In = get_nominal_dc_current_per_pole_in_kA();

    double VdropI = get_converter_voltage_drop_per_bridge_in_kV(INVERTER);
    double R = get_line_resistance_in_ohm();

    double TurnRatioI = get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)/get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

    complex<double> ZI = get_converter_transformer_impedance_in_ohm(INVERTER);

    double alpha_min = get_converter_min_alpha_or_gamma_in_deg(RECTIFIER); alpha_min = deg2rad(alpha_min);
    double gamma_min = get_converter_min_alpha_or_gamma_in_deg(INVERTER);  gamma_min = deg2rad(gamma_min);

    size_t NI = get_converter_number_of_bridge(INVERTER);


    double VdcR, VdcI, Idc;

    double VbusI = psdb.get_bus_voltage_in_kV(get_converter_bus(INVERTER));

    double EacI, cosAlpha;

    double TapR, TapI;

    set_converter_alpha_or_gamma_in_deg(INVERTER, get_converter_min_alpha_or_gamma_in_deg(INVERTER));
    if(Pside == INVERTER)
    {
        Idc = In; // desired
        VdcI = Pn/Idc; // desired
        //solve TapI with desired Idc, and VdcI
        solve_best_converter_transformer_tap_with_min_angle(INVERTER, VdcI, Idc);
        TapI = get_converter_transformer_tap_in_pu(INVERTER);
        // with solved Tap, solve Idc again
        EacI = VbusI/TurnRatioI/TapI;
        double a = -3.0*ZI.imag()/PI+2.0*ZI.real();
        double b = 3.0*sqrt(2.0)/PI*EacI*cos(gamma_min)-VdropI;
        double c = -Pn/NI;
        double Idc1, Idc2;
        Idc1 = (-b+sqrt(b*b-4*a*c))/(2*a);
        Idc2 = (-b-sqrt(b*b-4*a*c))/(2*a);
        Idc = (fabs(Idc1-In)<fabs(Idc2-In)) ? Idc1:Idc2;

        VdcI = solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(INVERTER, Idc, TapI);
        VdcR = VdcI+Idc*R;
        // solve tap with Vdc
        solve_best_converter_transformer_tap_with_min_angle(RECTIFIER, VdcR, Idc);
        TapR =  get_converter_transformer_tap_in_pu(RECTIFIER);
        // with Tap, solve alpha
        cosAlpha = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(RECTIFIER, VdcR, Idc, TapR);
        if(cosAlpha<cos(alpha_min))
            //solved
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(acos(cosAlpha)));
        else
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER));
    }
    else //RECTIFIER
    {
        Idc = In; // desired
        VdcR = Pn/Idc; // desired
        VdcI = VdcR - Idc*R;
        //solve TapR with desired Idc, and VdcR
        solve_best_converter_transformer_tap_with_min_angle(INVERTER, VdcI, Idc);
        TapI = get_converter_transformer_tap_in_pu(INVERTER);

        solve_best_converter_transformer_tap_with_min_angle(RECTIFIER, VdcR, Idc);
        TapR = get_converter_transformer_tap_in_pu(RECTIFIER);
        // solve cosAlpha
        cosAlpha = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(RECTIFIER, VdcR, Idc, TapR);
        if(cosAlpha<cos(alpha_min))
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(cosAlpha));
        else
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER));
    }
    solve_with_solved_tap_and_firing_angle();
}

void HVDC::solve_as_rectifier_regulating_current_and_inverter_regulating_gamma()
{
    //ostringstream osstream;
    //os<<"Solve %s as constant current (R) + constant gamma (I) mode.",
    //              get_device_name().c_str());
    //show_information_with_leading_time_stamp(osstream);
    set_converter_alpha_or_gamma_in_deg(INVERTER, get_converter_min_alpha_or_gamma_in_deg(INVERTER));

    double Vn = get_nominal_dc_voltage_per_pole_in_kV();
    double In = get_nominal_dc_current_per_pole_in_kA();

    double R = get_line_resistance_in_ohm();
    double alpha_min = get_converter_min_alpha_or_gamma_in_deg(RECTIFIER); alpha_min = deg2rad(alpha_min);
    double gamma_min = get_converter_min_alpha_or_gamma_in_deg(INVERTER);  gamma_min = deg2rad(gamma_min);

    double VdcR, VdcI, Idc, cosAlpha;

    Idc = In; // desired
    VdcI = Vn; // desired
    //solve TapI with desired Idc, and VdcI
    solve_best_converter_transformer_tap_with_min_angle(INVERTER, VdcI, Idc);
    double TapI = get_converter_transformer_tap_in_pu(INVERTER);

    // with solved Tap, solve Idc again
    VdcI = solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(INVERTER, Idc, TapI);

    VdcR = VdcI+Idc*R;
    // solve tap with Vdc
    solve_best_converter_transformer_tap_with_min_angle(RECTIFIER, VdcR, Idc);
    double TapR = get_converter_transformer_tap_in_pu(RECTIFIER);
    // with Tap, solve alpha
    cosAlpha = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(RECTIFIER, VdcR, Idc, TapR);
    if(cosAlpha<cos(alpha_min))
        //solved
        set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(acos(cosAlpha)));
    else
        set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER));

    solve_with_solved_tap_and_firing_angle();
}

void HVDC::solve_as_rectifier_regulating_alpha_and_inverter_regulating_current()
{
    //ostringstream osstream;
    //os<<"Solve %s as constant alpha (R) + constant current (I) mode.",
    //              get_device_name().c_str());
    //show_information_with_leading_time_stamp(osstream);
    set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER));

    double In = get_rectifier_nominal_dc_current_command_in_kA();
    double margin = get_current_power_margin();
    double Idc = In*(1.0-margin); // desired

    double VdcI = get_inverter_nominal_dc_voltage_command_in_kV();

    double R = get_line_resistance_in_ohm();
    double alpha_min = get_converter_min_alpha_or_gamma_in_deg(RECTIFIER); alpha_min = deg2rad(alpha_min);
    double gamma_min = get_converter_min_alpha_or_gamma_in_deg(INVERTER);  gamma_min = deg2rad(gamma_min);

    double VdcR = VdcI+Idc*R;
    //solve TapR with desired Idc, and VdcR
    solve_best_converter_transformer_tap_with_min_angle(RECTIFIER, VdcR, Idc);
    double TapR = get_converter_transformer_tap_in_pu(RECTIFIER);
    // with solved Tap, solve Vdc again
    VdcR = solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(RECTIFIER, Idc, TapR);
    VdcI = VdcR - Idc*R;
    // solve tap
    solve_best_converter_transformer_tap_with_min_angle(INVERTER, VdcI, Idc);
    double TapI = get_converter_transformer_tap_in_pu(INVERTER);
    // with Tap, solve alpha
    double cosGamma = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(INVERTER, VdcI, Idc, TapI);

    if(cosGamma<cos(gamma_min))
        //solved
        set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(acos(cosGamma)));
    else
        set_converter_alpha_or_gamma_in_deg(INVERTER, get_converter_min_alpha_or_gamma_in_deg(INVERTER));


    solve_with_solved_tap_and_firing_angle();
}

void HVDC::solve_with_solved_tap_and_firing_angle()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    //osstream<<"solving "<<get_device_name()<<" with solved tap rec "<<get_converter_transformer_tap_in_pu(RECTIFIER)<<", "
    //       <<"inv "<<get_converter_transformer_tap_in_pu(INVERTER)<<", angle rec "<<get_converter_alpha_or_gamma_in_deg(RECTIFIER)<<" deg, "
    //       <<" inv "<<get_converter_alpha_or_gamma_in_deg(INVERTER)<<" deg";
    //show_information_with_leading_time_stamp(osstream);

    double VdropR = get_converter_voltage_drop_per_bridge_in_kV(RECTIFIER);
    double VdropI = get_converter_voltage_drop_per_bridge_in_kV(INVERTER);
    double R = get_line_resistance_in_ohm();

    double TurnRatioR = get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER)/get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER);
    double TurnRatioI = get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)/get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

    double TapR = get_converter_transformer_tap_in_pu(RECTIFIER);
    double TapI = get_converter_transformer_tap_in_pu(INVERTER);

    double alpha = get_converter_alpha_or_gamma_in_deg(RECTIFIER); alpha = deg2rad(alpha);
    double gamma = get_converter_alpha_or_gamma_in_deg(INVERTER);  gamma = deg2rad(gamma);

    complex<double> ZR = get_converter_transformer_impedance_in_ohm(RECTIFIER);
    complex<double> ZI = get_converter_transformer_impedance_in_ohm(INVERTER);

    size_t NR = get_converter_number_of_bridge(RECTIFIER);
    size_t NI = get_converter_number_of_bridge(INVERTER);

    double VbusR = psdb.get_bus_voltage_in_kV(get_converter_bus(RECTIFIER));
    double VbusI = psdb.get_bus_voltage_in_kV(get_converter_bus(INVERTER));

    double EacR = VbusR/TurnRatioR/TapR;
    double EacI = VbusI/TurnRatioI/TapI;

    double Vdc0_r = NR*3.0*sqrt(2.0)/PI*EacR;
    double Vdc0_i = NI*3.0*sqrt(2.0)/PI*EacI;

    double Rceq_r = NR*(3.0*ZR.imag()/PI+2.0*ZR.real());
    double Rceq_i = NI*(3.0*ZI.imag()/PI+2.0*ZI.real());

    double Idc = Vdc0_r*cos(alpha)-NR*VdropR - (Vdc0_i*cos(gamma)-NI*VdropI);
    Idc /= (R+Rceq_r-Rceq_i);

    double VdcR = Vdc0_r*cos(alpha)-Rceq_r*Idc-NR*VdropR;
    double VdcI = Vdc0_i*cos(gamma)-Rceq_i*Idc-NI*VdropI;

    //double Idc = NR*(3.0*sqrt(2.0)/PI*EacR*cos(alpha) - VdropR) - NI*(3.0*sqrt(2.0)/PI*EacI*cos(gamma) - VdropI);
    //Idc /= (R+NR*3.0*ZR.imag()/PI-NI*3.0*ZI.imag()/PI+NR*2.0*ZR.real()+NI*2.0*ZI.real());

    //double VdcR = NR*(3.0*sqrt(2.0)/PI*EacR*cos(alpha) - 3.0*ZR.imag()/PI*Idc - 2.0*ZR.real()*Idc- VdropR);
    //double VdcI = NI*(3.0*sqrt(2.0)/PI*EacI*cos(gamma) - 3.0*ZI.imag()/PI*Idc + 2.0*ZI.real()*Idc- VdropI);

    set_line_dc_current_in_kA(Idc);
    set_converter_dc_voltage_in_kV(RECTIFIER, VdcR);
    set_converter_dc_voltage_in_kV(INVERTER, VdcI);

    //osstream<<"DC current "<<get_line_dc_current_in_kA()<<" kA, DC voltage "<<get_converter_dc_voltage_in_kV(RECTIFIER)
    //       <<" kV, "<<get_converter_dc_voltage_in_kV(INVERTER)<<" kV";
    //show_information_with_leading_time_stamp(osstream);
}


double HVDC::solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(HVDC_CONVERTER_SIDE converter, double Vdc, double Idc, double Tap)
{
    // solve best alpha and gamma
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Vdrop = get_converter_voltage_drop_per_bridge_in_kV(converter);

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    complex<double> Z = get_converter_transformer_impedance_in_ohm(converter);

    size_t N = get_converter_number_of_bridge(converter);

    double Vbus = psdb.get_bus_voltage_in_kV(get_converter_bus(converter));
    double Eac = Vbus/TurnRatio/Tap; // actual EacR
    double Eac_cosAngle = Vdc/N+3.0*Z.imag()/PI*Idc+2.0*Z.real()*Idc+Vdrop;
           Eac_cosAngle /= (3.0*sqrt(2.0)/PI);
    double cosAngle = Eac_cosAngle/Eac; // desired alpha

    return cosAngle;
}

double HVDC::solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(HVDC_CONVERTER_SIDE converter, double Idc, double Tap)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Vdrop = get_converter_voltage_drop_per_bridge_in_kV(converter);

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);

    complex<double> Z = get_converter_transformer_impedance_in_ohm(converter);

    double angle_min = get_converter_min_alpha_or_gamma_in_deg(converter);
    angle_min = deg2rad(angle_min);

    size_t N = get_converter_number_of_bridge(converter);

    double Vbus = psdb.get_bus_voltage_in_kV(get_converter_bus(converter));
    double Eac = Vbus/TurnRatio/Tap;
    double Vdc = N*(3.0*sqrt(2.0)/PI*Eac*cos(angle_min)-3.0*Z.imag()/PI*Idc-2.0*Z.real()*Idc-Vdrop);
    return Vdc;
}

complex<double> HVDC::get_converter_dynamic_current_in_pu_based_on_system_base_power(HVDC_CONVERTER_SIDE converter)
{
    if(get_status()==false)
        return 0.0;

    HVDC_MODEL* model = get_hvdc_model();
    if(model==NULL)
        return 0.0;

    return model->get_converter_ac_current_in_pu(converter);
}
