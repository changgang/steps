#include "header/device/hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include "header/model/hvdc_model/hvdc_models.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

HVDC::HVDC(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
}

HVDC::~HVDC()
{
}

string HVDC::get_converter_side_name(CONVERTER_SIDE converter) const
{
    switch(converter)
    {
        case RECTIFIER:
            return "RECTIFIER";
        default:
            return "INVERTER";
    }
}

void HVDC::set_converter_bus(CONVERTER_SIDE converter, const unsigned int bus)
{
    ostringstream osstream;
    string converter_name = get_converter_side_name(converter);

    if(bus!=0)
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(bus))
        {
            converter_bus[converter] = bus;
            converter_busptr[converter] = psdb.get_bus(bus);
        }
        else
        {
            osstream<<"Bus "<<bus<<" does not exist for setting up "<<converter_name<<" side bus of hvdc link."<<endl
                    <<"0 will be set to indicate invalid hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            converter_bus[converter] = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<converter_name<<" bus of hvdc link."<<endl
                <<"0 will be set to indicate invalid hvdc link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        converter_bus[converter] = bus;
    }
}

void HVDC::set_converter_valve_side_bus_name(CONVERTER_SIDE converter, string name)
{
    name = trim_string(name);
    add_string_to_str_int_map(name);
    converter_valve_bus_name_index[converter] = get_index_of_string(name);
}

void HVDC::set_identifier(const string hvdc_id)
{
    string hvdc_id_temp = trim_string(hvdc_id);
    add_string_to_str_int_map(hvdc_id_temp);
    this->identifier_index = get_index_of_string(hvdc_id_temp);
}

void HVDC::set_name(const string name)
{
    set_identifier(name);
    //this->hvdc_name = trim_string(name);
}

void HVDC::set_status(const bool status)
{
    this->status = status;
}

void HVDC::set_line_resistance_in_ohm(const double R)
{
    if(R>0.0)
        this->line_R_in_ohm = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<R<<" ohm) is not allowed for setting HVDC line resistance of "<<get_compound_device_name()<<endl
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
        osstream<<"Error. Negative ("<<L<<" mH) is not allowed for setting HVDC line inductance of "<<get_compound_device_name()<<endl
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
        osstream<<"Error. Negative ("<<C<<" uF) is not allowed for setting HVDC line capacitance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_C_in_F = 0.0;
    }
}

void HVDC::set_line_smooting_inductance_in_mH(CONVERTER_SIDE converter, const double L)
{
    if(L>=0.0)
        this->smoothing_L_in_H[converter] = L*0.001;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<L<<" mH) is not allowed for setting HVDC smoothing inductance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->smoothing_L_in_H[converter] = 0.0;
    }
}

void HVDC::set_line_smooting_resistance_in_ohm(CONVERTER_SIDE converter, const double R)
{
    if(R>=0.0)
        this->smoothing_R_in_ohm[converter] = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Negative ("<<R<<" ohm) is not allowed for setting HVDC smoothing resistance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->smoothing_R_in_ohm[converter] = 0.0;
    }
}

void HVDC::set_nominal_dc_power_in_MW(const double P)
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
        osstream<<"Error. Non-positive ("<<P<<" MW) is not allowed for setting HVDC nominal dc power per pole of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        demand_power_in_MW = 0.0;
        demand_current_in_kA = 0.0;
    }
}

void HVDC::set_side_to_hold_power(CONVERTER_SIDE side)
{
    hvdc_side_to_hold_power = side;
}


void HVDC::set_nominal_dc_voltage_in_kV(const double V)
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
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting HVDC nominal dc voltage per pole of "<<get_compound_device_name()<<endl
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

void HVDC::set_nominal_dc_current_in_kA(const double I)
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
        osstream<<"Error. Non-positive ("<<I<<" kA) is not allowed for setting HVDC nominal dc current per pole of "<<get_compound_device_name()<<endl
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
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting HVDC threshold dc voltage for constant power and constant current operation mode of "<<get_compound_device_name()<<endl
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

void HVDC::set_meter_end(const CONVERTER_SIDE converter)
{
    meter_end = converter;
}

void HVDC::set_converter_operation_mode(CONVERTER_SIDE converter, const HVDC_OPERATION_MODE mode)
{
    ostringstream osstream;
    string converter_name = get_converter_side_name(converter);

    if(converter==RECTIFIER)
    {
        if(mode == RECTIFIER_CONSTANT_POWER or mode == RECTIFIER_CONSTANT_CURRENT)
            operation_mode[converter] = mode;
        else
        {
            osstream<<"Warning. Neither INVERTER_CONSTANT_VOLTAGE nor INVERTER_CONSTANT_GAMMA is allowed for setting HVDC "<<converter_name<<" operation mode of "<<get_compound_device_name()<<endl
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
            osstream<<"Warning. Neither INVERTER_CONSTANT_VOLTAGE nor INVERTER_CONSTANT_GAMMA is allowed for setting HVDC "<<converter_name<<" operation mode of "<<get_compound_device_name()<<endl
                    <<"INVERTER_CONSTANT_VOLTAGE mode will be set automatically.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            operation_mode[converter] = INVERTER_CONSTANT_VOLTAGE;
        }
    }
}

void HVDC::set_converter_number_of_bridge(CONVERTER_SIDE converter, const unsigned int nbridge)
{
    if(nbridge > 0)
        this->bridge_number[converter] = nbridge;
    else
    {
        string converter_name = get_converter_side_name(converter);
        ostringstream osstream;
        osstream<<"Error. Zero (0) is not allowed for setting HVDC "<<converter_name<<" bridge number of "<<get_compound_device_name()<<endl
                <<"0 will be set to indicate invalid HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->bridge_number[converter] = 0;
    }
}

void HVDC::set_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE converter, const double V)
{
    if(V >= 0.0)
        this->voltage_drop_per_bridge_in_kV[converter] = V;
    else
    {
        string converter_name = get_converter_side_name(converter);
        ostringstream osstream;
        osstream<<"Warning. Negative ("<<V<<" kV) is not allowed for setting HVDC "<<converter_name<<" voltage drop of "<<get_compound_device_name()<<endl
                <<"0 will be set automatically.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->voltage_drop_per_bridge_in_kV[converter] = 0.0;
    }
}

void HVDC::set_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE converter, const double angle)
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

    cos_max_firing_angle[converter] = steps_cos(deg2rad(this->max_firing_angle_in_deg[converter]));
}

void HVDC::set_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE converter, const double angle)
{
    if(angle>=0.0)
        this->min_firing_angle_in_deg[converter] = angle;
    else
        this->min_firing_angle_in_deg[converter] = 0.0;
    cos_min_firing_angle[converter] = steps_cos(deg2rad(this->min_firing_angle_in_deg[converter]));
}

void HVDC::set_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE converter, const double V)
{
    if(V > 0.0)
        this->converter_transformer_grid_side_base_voltage_in_kV[converter] = V;
    else
    {
        ostringstream osstream;
        string converter_name = get_converter_side_name(converter);
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
                osstream<<"Warning. Zero (0.0 kV) is provided for setting HVDC "<<converter_name<<" transformer grid side base voltage of "<<get_compound_device_name()<<endl
                        <<"However, the "<<converter_name<<" bus doesn't exist. 0.0 will be set to indicate as base voltage of "<<converter_name<<" bus.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                this->converter_transformer_grid_side_base_voltage_in_kV[converter] = 0.0;
            }
        }
         else
        {
            osstream<<"Warning. Negative ("<<V<<" kV) is not allowed for setting HVDC "<<converter_name<<" transformer grid side base voltage of "<<get_compound_device_name()<<endl
                    <<"0.0 will be set automatically as base voltage of "<<converter_name<<" bus.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
            this->converter_transformer_grid_side_base_voltage_in_kV[converter] = 0.0;
        }
    }
}

void HVDC::set_converter_transformer_converter_side_base_voltage_in_kV(CONVERTER_SIDE converter, const double V)
{
    if(V > 0.0)
        this->converter_transformer_converter_side_base_voltage_in_kV[converter] = V;
    else
    {
        string converter_name = get_converter_side_name(converter);
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<V<<" kV) is not allowed for setting HVDC "<<converter_name<<" transformer converter side base voltage of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid base voltage.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->converter_transformer_converter_side_base_voltage_in_kV[converter] = 0.0;
    }
}

void HVDC::set_converter_transformer_impedance_in_ohm(CONVERTER_SIDE converter, const complex<double> z)
{
    this->converter_transformer_Z_in_ohm[converter] = z;
}

void HVDC::set_converter_transformer_admittance_in_siemens(CONVERTER_SIDE converter, const complex<double> y)
{
    this->converter_transformer_Y_in_siemens[converter] = y;
}

void HVDC::set_converter_transformer_max_tap_in_pu(CONVERTER_SIDE converter, const double maxtap)
{
    if(maxtap>0.0)
        this->converter_transformer_max_tap_in_pu[converter] = maxtap;
    else
        this->converter_transformer_max_tap_in_pu[converter] = 1.0;
}

void HVDC::set_converter_transformer_min_tap_in_pu(CONVERTER_SIDE converter, const double mintap)
{
    if(mintap>0.0)
        this->converter_transformer_min_tap_in_pu[converter] = mintap;
    else
        this->converter_transformer_min_tap_in_pu[converter] = 1.0;
}

void HVDC::set_converter_transformer_number_of_taps(CONVERTER_SIDE converter, const unsigned int ntap)
{
    if(ntap>1)
        this->converter_transformer_number_of_taps[converter] = ntap;
    else
        this->converter_transformer_number_of_taps[converter] = 1;
}

unsigned int HVDC::get_converter_bus(CONVERTER_SIDE converter) const
{
    return converter_bus[converter];
}

BUS* HVDC::get_bus_pointer(CONVERTER_SIDE converter) const
{
    return converter_busptr[converter];
}

string HVDC::get_converter_valve_side_bus_name(CONVERTER_SIDE converter) const
{
    return get_string_of_index(converter_valve_bus_name_index[converter]);
}

string HVDC::get_identifier() const
{
    return get_string_of_index(identifier_index);
}

string HVDC::get_name() const
{
    return get_identifier();
    return get_string_of_index(hvdc_name_index);
}


unsigned int HVDC::get_identifier_index() const
{
    return identifier_index;
}

unsigned int HVDC::get_name_index() const
{
    return get_identifier_index();
}

bool HVDC::get_status() const
{
    return status;
}

bool HVDC::is_blocked() const
{
    if(get_status()==true) return false;
    else                   return true;
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

double HVDC::get_line_smooting_inductance_in_mH(CONVERTER_SIDE converter) const
{
    return smoothing_L_in_H[converter]*1e3;
}

double HVDC::get_line_smooting_resistance_in_ohm(CONVERTER_SIDE converter) const
{
    return smoothing_R_in_ohm[converter];
}

double HVDC::get_nominal_dc_power_in_MW() const
{
    return demand_power_in_MW;
}

CONVERTER_SIDE HVDC::get_side_to_hold_dc_power() const
{
    return hvdc_side_to_hold_power;
}

double HVDC::get_nominal_dc_voltage_in_kV() const
{
    return demand_voltage_in_kV;
}

double HVDC::get_compensating_resistance_to_hold_dc_voltage_in_ohm() const
{
    return compensating_R_to_hold_dc_voltage_in_ohm;
}

double HVDC::get_nominal_dc_current_in_kA() const
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

CONVERTER_SIDE HVDC::get_meter_end() const
{
    return meter_end;
}

HVDC_OPERATION_MODE HVDC::get_converter_operation_mode(CONVERTER_SIDE converter) const
{
    return operation_mode[converter];
}

unsigned int HVDC::get_converter_number_of_bridge(CONVERTER_SIDE converter) const
{
    return bridge_number[converter];
}

double HVDC::get_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE converter) const
{
    return voltage_drop_per_bridge_in_kV[converter];
}

double HVDC::get_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE converter) const
{
    return max_firing_angle_in_deg[converter];
}

double HVDC::get_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE converter) const
{
    return min_firing_angle_in_deg[converter];
}

double HVDC::get_converter_cos_max_alpha_or_gamma(CONVERTER_SIDE converter) const
{
    return cos_max_firing_angle[converter];
}

double HVDC::get_converter_cos_min_alpha_or_gamma(CONVERTER_SIDE converter) const
{
    return cos_min_firing_angle[converter];
}

double HVDC::get_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE converter) const
{
    string converter_name = get_converter_side_name(converter);
    double V = converter_transformer_grid_side_base_voltage_in_kV[converter];
    if(V != 0.0)
        return V;
    else
    {
        STEPS& toolkit = get_toolkit();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        return psdb.get_bus_base_voltage_in_kV(get_converter_bus(converter));
    }
}

double HVDC::get_converter_transformer_converter_side_base_voltage_in_kV(CONVERTER_SIDE converter) const
{
    return converter_transformer_converter_side_base_voltage_in_kV[converter];
}

complex<double> HVDC::get_converter_transformer_impedance_in_ohm(CONVERTER_SIDE converter) const
{
    return converter_transformer_Z_in_ohm[converter];
}

complex<double> HVDC::get_converter_transformer_admittance_in_siemens(CONVERTER_SIDE converter) const
{
    return converter_transformer_Y_in_siemens[converter];
}

double HVDC::get_converter_transformer_max_tap_in_pu(CONVERTER_SIDE converter) const
{
    return converter_transformer_max_tap_in_pu[converter];
}

double HVDC::get_converter_transformer_min_tap_in_pu(CONVERTER_SIDE converter) const
{
    return converter_transformer_min_tap_in_pu[converter];
}

unsigned int HVDC::get_converter_transformer_number_of_taps(CONVERTER_SIDE converter) const
{
    return converter_transformer_number_of_taps[converter];
}


void HVDC::turn_rectifier_constant_power_mode_into_constant_current_mode()
{
    if(get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_POWER)
    {
        double P = get_nominal_dc_power_in_MW();
        double V = get_nominal_dc_voltage_in_kV();
        set_nominal_dc_current_in_kA(P/V);
        set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
        ostringstream osstream;
        osstream<<get_compound_device_name()<<" operation mode is turned from constant power mode into constant current mode.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC::turn_rectifier_constant_current_mode_into_constant_power_mode()
{
    if(get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_CURRENT)
    {
        double I = get_nominal_dc_current_in_kA();
        double V = get_nominal_dc_voltage_in_kV();
        set_nominal_dc_power_in_MW(V*I);
        set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
        ostringstream osstream;
        osstream<<get_compound_device_name()<<" operation mode is turned from constant current mode into constant power mode.";
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

    double Vn = get_nominal_dc_voltage_in_kV();
    double Rcomp = get_compensating_resistance_to_hold_dc_voltage_in_ohm();

    if(get_converter_operation_mode(RECTIFIER) == RECTIFIER_CONSTANT_POWER) // constant power mode
    {
        double Pn = get_nominal_dc_power_in_MW();
        CONVERTER_SIDE Pside = get_side_to_hold_dc_power();
        if(Pside == RECTIFIER)
            return Pn;
        else // inverter side
        {
            if(fabs(Rcomp)<DOUBLE_EPSILON)
            {
                double Idc = Pn/Vn;
                double Vdcr = Vn+Rdc*Idc;
                return Vdcr*Idc;
            }
            else
            {
                if(fabs(Rdc-Rcomp)<DOUBLE_EPSILON)
                {
                    // (Vn-Idc*Rdc)*Idc = Pn
                    // Rdc*Idc2-Vn*Idc+Pn = 0
                    // Idc = (Vn+-steps_sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    // use
                    // Idc = (Vn-steps_sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    double Idc = (Vn-steps_sqrt(Vn*Vn-4.0*Rdc*Pn))/(2.0*Rdc);
                    double Vdci = Pn/Idc;
                    double Vdcr = Vdci+Rdc*Idc;
                    return Vdcr*Idc;
                }
                else
                {
                    // (Vn-Idc*Rcomp)*Idc = Pn;
                    // use
                    // Idc = (Vn-steps_sqrt(Vn2-4*Rcomp*Pn))/(2*Rcomp)
                    double Idc = (Vn-steps_sqrt(Vn*Vn-4.0*Rcomp*Pn))/(2.0*Rcomp);
                    double Vdci = Pn/Idc;
                    double Vdcr = Vdci+Rdc*Idc;
                    return Vdcr*Idc;
                }
            }
        }
    }
    else // rectifier in constant current mode
    {
        double Idc = get_nominal_dc_current_in_kA();
        if(fabs(Rcomp-Rdc)<DOUBLE_EPSILON)
        {
            return Idc*Vn;
        }
        else
        {
            if(fabs(Rcomp)<DOUBLE_EPSILON)
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
        double Idc = get_nominal_dc_current_in_kA();
        return Idc;
    }
    else // rectifier in constant power mode
    {
        double Pn = get_nominal_dc_power_in_MW();
        CONVERTER_SIDE Pside = get_side_to_hold_dc_power();
        double Vn = get_nominal_dc_voltage_in_kV();
        double Rcomp = get_compensating_resistance_to_hold_dc_voltage_in_ohm();
        double Rdc = get_line_resistance_in_ohm();
        if(Pside == RECTIFIER)
        {
            if(fabs(Rcomp-Rdc)<DOUBLE_EPSILON)
            {
                double Idc = Pn/Vn;
                return Idc;
            }
            else
            {
                if(fabs(Rcomp)<DOUBLE_EPSILON)
                {
                    // Pn = (Vn+Rdc*Idc)*Idc
                    // Rdc*Idc2 +Vn*Idc -Pn = 0
                    double Idc = (-Vn+steps_sqrt(Vn*Vn+4.0*Rdc*Pn))/(2.0*Rdc);
                    return Idc;
                }
                else
                {
                    // Pn = (Vn+(Rdc-Rcomp)*Idc)*Idc;
                    double Idc = (-Vn+steps_sqrt(Vn*Vn+4.0*(Rdc-Rcomp)*Pn))/(2.0*(Rdc-Rcomp));
                    return Idc;
                }
            }
        }
        else // inverter side
        {
            if(fabs(Rcomp)<DOUBLE_EPSILON)
            {
                double Idc = Pn/Vn;
                return Idc;
            }
            else
            {
                if(fabs(Rdc-Rcomp)<DOUBLE_EPSILON)
                {
                    // (Vn-Idc*Rdc)*Idc = Pn
                    // Rdc*Idc2-Vn*Idc+Pn = 0
                    // Idc = (Vn+-steps_sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    // use
                    // Idc = (Vn-steps_sqrt(Vn2-4*Rdc*Pn))/(2*Rdc)
                    double Idc = (Vn-steps_sqrt(Vn*Vn-4.0*Rdc*Pn))/(2.0*Rdc);
                    return Idc;
                }
                else
                {
                    // (Vn-Idc*Rcomp)*Idc = Pn;
                    // use
                    // Idc = (Vn-steps_sqrt(Vn2-4*Rcomp*Pn))/(2*Rcomp)
                    double Idc = (Vn-steps_sqrt(Vn*Vn-4.0*Rcomp*Pn))/(2.0*Rcomp);
                    return Idc;
                }
            }
        }
    }
}

double HVDC::get_inverter_nominal_dc_voltage_command_in_kV() const
{
    double Vn = get_nominal_dc_voltage_in_kV();
    double Rcomp = get_compensating_resistance_to_hold_dc_voltage_in_ohm();
    double Rdc = get_line_resistance_in_ohm();
    if(fabs(Rcomp)<DOUBLE_EPSILON)
    {
        return Vn;
    }
    else
    {
        double Idc = get_rectifier_nominal_dc_current_command_in_kA();
        if(fabs(Rdc-Rcomp)<DOUBLE_EPSILON)
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
    unsigned int bus = converter_bus[0];
    converter_bus[0] = converter_bus[1];
    converter_bus[1] = bus;

    BUS* busptr = converter_busptr[0];
    converter_busptr[0] = converter_busptr[1];
    converter_busptr[1] = busptr;

    unsigned int number = bridge_number[0];
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

    unsigned int n = converter_transformer_number_of_taps[0];
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
    converter_busptr[RECTIFIER] = NULL;
    converter_busptr[INVERTER] = NULL;
    converter_valve_bus_name_index[RECTIFIER]=get_index_of_string("");
    converter_valve_bus_name_index[INVERTER]=get_index_of_string("");
    set_identifier(""); set_name(""); set_status(false);
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
    set_side_to_hold_power(RECTIFIER);

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
    set_converter_transformer_number_of_taps(RECTIFIER, 1);
    set_converter_transformer_number_of_taps(INVERTER, 1);

    set_converter_alpha_or_gamma_in_deg(RECTIFIER, 0.0);
    set_converter_alpha_or_gamma_in_deg(INVERTER, 0.0);
    set_converter_transformer_tap_in_pu(RECTIFIER, 1.0);
    set_converter_transformer_tap_in_pu(INVERTER, 1.0);

    converter_firing_angle_fixed[RECTIFIER] = false;
    converter_firing_angle_fixed[INVERTER] = false;

    hvdc_model = NULL;
    auxiliary_signal_model = NULL;
}

bool HVDC::is_connected_to_bus(unsigned int bus) const
{
    if(get_converter_bus(RECTIFIER)==bus || get_converter_bus(INVERTER)==bus) return true;
    else                                                                      return false;
}

bool HVDC::is_in_area(unsigned int area) const
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

bool HVDC::is_in_zone(unsigned int zone) const
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

    osstream<<"Vdc = "<<get_nominal_dc_voltage_in_kV()<<" kV, "
            <<"Idc = "<<get_nominal_dc_current_in_kA()<<" kA, "
            <<"Pdc = "<<get_nominal_dc_power_in_MW()<<" MW";
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

void HVDC::set_model(MODEL* model)
{
    if(model != NULL and model->has_allowed_device_type(STEPS_HVDC))
    {
        model->set_device_id(get_device_id());
        if(model->get_model_type()=="HVDC")
        {
            set_hvdc_model((HVDC_MODEL*) model);
            return;
        }
        if(model->get_model_type()=="AUXILIARY SIGNAL")
        {
            set_auxiliary_signal_model((AUXILIARY_SIGNAL_MODEL*) model);
            return;
        }
        ostringstream osstream;
        osstream<<"Waring. Neither AUXILIARY SIGNAL model nor HVDC model is given to set dynamic model for "<<get_compound_device_name();
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

MODEL* HVDC::get_model_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="HVDC")
        return get_hvdc_model();
    if(model_type=="AUXILIARY SIGNAL")
        return get_auxiliary_signal_model();
    return NULL;
}

void HVDC::set_hvdc_model(HVDC_MODEL* model)
{
    if(model!=NULL)
        hvdc_model = model;
}

void HVDC::set_auxiliary_signal_model(AUXILIARY_SIGNAL_MODEL* model)
{
    if(model != NULL)
        auxiliary_signal_model = model;
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
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(get_status()==true)
    {
        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                if(toolkit.is_detailed_log_enabled())
                {
                    osstream<<"HVDC data before dynamic initialization";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    show_solved_hvdc_steady_state();
                }
                HVDC_MODEL* hvdc_model = get_hvdc_model();
                if(hvdc_model!=NULL)
                    hvdc_model->initialize();
                else
                {
                    osstream<<"HVDC model is missing for "<<get_compound_device_name();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                break;
            }
            default:
            {
                HVDC_MODEL* hvdc_model = get_hvdc_model();
                if(hvdc_model!=NULL and hvdc_model->is_model_active())
                    hvdc_model->run(mode);
                break;
            }
        }
    }
}

HVDC& HVDC::operator=(const HVDC& hvdc)
{
    if(this == (&hvdc)) return *this;

    set_toolkit(hvdc.get_toolkit());
    clear();

    if(hvdc.get_converter_bus(RECTIFIER)!=0)
        set_converter_bus(RECTIFIER, hvdc.get_converter_bus(RECTIFIER));

    if(hvdc.get_converter_bus(INVERTER)!=0)
        set_converter_bus(INVERTER, hvdc.get_converter_bus(INVERTER));

    set_identifier(hvdc.get_identifier());
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
    set_side_to_hold_power(hvdc.get_side_to_hold_dc_power());
    set_nominal_dc_current_in_kA(hvdc.get_nominal_dc_current_in_kA());
    set_nominal_dc_voltage_in_kV(hvdc.get_nominal_dc_voltage_in_kV());
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

    set_model(hvdc.get_hvdc_model());
    set_auxiliary_signal_model(hvdc.get_auxiliary_signal_model());

    return *this;
}

DEVICE_ID HVDC::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type(STEPS_HVDC);

    TERMINAL terminal;
    terminal.append_bus(get_converter_bus(RECTIFIER));
    terminal.append_bus(get_converter_bus(INVERTER));
    did.set_device_terminal(terminal);
    did.set_device_identifier_index(get_identifier_index());

    return did;
}

void HVDC::solve_steady_state()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    HVDC_OPERATION_MODE r_mode = get_converter_operation_mode(RECTIFIER), i_mode = get_converter_operation_mode(INVERTER);

    if(r_mode != RECTIFIER_CONSTANT_POWER and r_mode != RECTIFIER_CONSTANT_CURRENT)
    {
        osstream<<"Warning. Rectifier of "<<get_compound_device_name()<<" is not in either RECTIFIER_CONSTANT_POWER or"
                " RECTIFIER_CONSTANT_CURRENT mode."<<endl<<"HVDC will not be solved.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(i_mode != INVERTER_CONSTANT_VOLTAGE and r_mode != INVERTER_CONSTANT_GAMMA)
    {
        osstream<<"Warning. Inverter of "<<get_compound_device_name()<<" is not in either INVERTER_CONSTANT_VOLTAGE or"
                " INVERTER_CONSTANT_GAMMA mode."<<endl<<"HVDC will not be solved.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    temp_converter_firing_angle_fixed[RECTIFIER] = false;
    temp_converter_firing_angle_fixed[INVERTER] = false;

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
    if(temp_converter_firing_angle_fixed[RECTIFIER] != converter_firing_angle_fixed[RECTIFIER] or
       temp_converter_firing_angle_fixed[INVERTER] != converter_firing_angle_fixed[INVERTER])
    {
        osstream<<"Firing angle mode is changed for "<<get_compound_device_name()<<":\n";
        if(temp_converter_firing_angle_fixed[RECTIFIER] != converter_firing_angle_fixed[RECTIFIER])
        {
            osstream<<"Rectifier alpha is changed from "<<(converter_firing_angle_fixed[RECTIFIER]==true?"Fixed":"Regulating")<<" to "
                    <<(temp_converter_firing_angle_fixed[RECTIFIER]==true?"Fixed":"Regulating")<<".\n"
                    <<"Rectifier will "<<(temp_converter_firing_angle_fixed[RECTIFIER]==true?"no longer":"return to")<<" regulate "
                    <<(r_mode==RECTIFIER_CONSTANT_POWER?"power":"current")<<".\n"
                    <<"Rectifier firing angle is in "<<(temp_converter_firing_angle_fixed[RECTIFIER]==true?"fixed":"regulating")<<" mode.";
        }
        if(temp_converter_firing_angle_fixed[INVERTER] != converter_firing_angle_fixed[INVERTER])
        {
            osstream<<"Inverter gamma is changed from "<<(converter_firing_angle_fixed[INVERTER]==true?"Fixed":"Regulating")<<" to "
                    <<(temp_converter_firing_angle_fixed[INVERTER]==true?"Fixed":"Regulating")<<".\n";
            if(temp_converter_firing_angle_fixed[INVERTER]==true)
                osstream<<"Inverter will no longer regulate "<<(converter_firing_angle_fixed[RECTIFIER]==true?"current":"voltage")<<".\n";
            else
                osstream<<"Inverter will return to regulate "<<(temp_converter_firing_angle_fixed[RECTIFIER]==true?"current":"voltage")<<".\n";

            osstream<<"Inverter firing angle is in "<<(temp_converter_firing_angle_fixed[INVERTER]==true?"fixed":"regulating")<<" mode.";
        }
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC::show_solved_hvdc_steady_state() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double sbase = toolkit.get_system_base_power_in_MVA();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    ostringstream osstream;
    osstream<<"Solved steady state of HVDC '"<<get_name()<<"':";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Idc = "<<get_line_dc_current_in_kA()<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    CONVERTER_SIDE converter=RECTIFIER;

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
            <<"Vac = "<<psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter))<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Transformer tap = "<<get_converter_transformer_tap_in_pu(converter)<<" pu.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"alpha = "<<get_converter_alpha_or_gamma_in_deg(converter)<<" deg, "
            <<"mu = "<<get_converter_commutating_overlap_angle_in_deg(converter)<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Rectifier AC current = "<<get_converter_ac_current_in_kA(converter)<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    double P = get_converter_ac_active_power_in_MW(converter);
    double Q = get_converter_ac_reactive_power_in_MVar(converter);
    complex<double> S(P,Q);
    complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_converter_bus(converter));
    S *=  one_over_sbase;
    complex<double> I = conj(S/V);
    I *= (sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_converter_bus(converter))));

    osstream<<"Pdc = "<<get_converter_dc_power_in_MW(converter)<<" MW, "
            <<"Pac = "<<get_converter_ac_active_power_in_MW(converter)<<" MW, "
            <<"Qac = "<<get_converter_ac_reactive_power_in_MVar(converter)<<" MVar, "
            <<"Eta = "<<get_converter_ac_power_factor(converter)<<", "
            <<"Iac = "<<get_converter_ac_current_in_kA(converter)<<"kA, or "
            <<I<<"KA and "<<abs(I)<<"kA";
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
            osstream<<"Mode "<<mode<<" is not supported for inverter side of "<<get_compound_device_name()<<endl
                    <<"This line should never appear. If you find it, ask for help.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    osstream<<"Rectifier: "<<modestr;
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Vdc = "<<get_converter_dc_voltage_in_kV(converter)<<" kV, "
            <<"Vac = "<<psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter))<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Transformer tap = "<<get_converter_transformer_tap_in_pu(converter)<<" pu.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"gamma = "<<get_converter_alpha_or_gamma_in_deg(converter)<<" deg, "
            <<"mu = "<<get_converter_commutating_overlap_angle_in_deg(converter)<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Inverter AC current = "<<get_converter_ac_current_in_kA(converter)<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    P = get_converter_ac_active_power_in_MW(converter);
    Q = get_converter_ac_reactive_power_in_MVar(converter);
    S = complex<double>(P,Q);
    V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(get_converter_bus(converter));
    S *=  one_over_sbase;
    I = conj(S/V);
    I *= (sbase/(SQRT3*psdb.get_bus_base_voltage_in_kV(get_converter_bus(converter))));

    osstream<<"Pdc = "<<get_converter_dc_power_in_MW(converter)<<" MW, "
            <<"Pac = "<<get_converter_ac_active_power_in_MW(converter)<<" MW, "
            <<"Qac = "<<get_converter_ac_reactive_power_in_MVar(converter)<<" MVar, "
            <<"Eta = "<<get_converter_ac_power_factor(converter)<<", "
            <<"Iac = "<<get_converter_ac_current_in_kA(converter)<<"kA, or "
            <<I<<"KA and "<<abs(I)<<"kA";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void HVDC::set_converter_alpha_or_gamma_in_deg(CONVERTER_SIDE converter, double angle, double cos_angle)
{
    if(angle>=0.0)
        converter_firing_angle_in_deg[converter] = angle;
    else
        converter_firing_angle_in_deg[converter] = 0.0;

    if(cos_angle!=0.0)
        cos_converter_firing_angle[converter] = cos_angle;
    else
        cos_converter_firing_angle[converter] = steps_cos(deg2rad(converter_firing_angle_in_deg[converter]));
}

double HVDC::get_converter_cos_alpha_or_gamma(CONVERTER_SIDE converter) const
{
    return cos_converter_firing_angle[converter];
}

void HVDC::set_converter_transformer_tap_in_pu(CONVERTER_SIDE converter, double tap)
{
    if(tap>0.0)
        converter_transformer_tap_in_pu[converter] = tap;
    else
        converter_transformer_tap_in_pu[converter] = 1.0;
}

void HVDC::set_converter_dc_voltage_in_kV(CONVERTER_SIDE converter, const double V)
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

void HVDC::set_converter_dc_current_in_kA(CONVERTER_SIDE converter, const double I)
{
    line_dc_current_in_kA[converter] = I;
}

double HVDC::get_converter_alpha_or_gamma_in_deg(CONVERTER_SIDE converter) const
{
    return converter_firing_angle_in_deg[converter];
}

double HVDC::get_converter_transformer_tap_in_pu(CONVERTER_SIDE converter) const
{
    return converter_transformer_tap_in_pu[converter];
}

double HVDC::get_converter_commutating_overlap_angle_in_deg(CONVERTER_SIDE converter) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double Tap = get_converter_transformer_tap_in_pu(converter);
    double Vac = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter));
    double Eac = Vac/(TurnRatio*Tap);

    double Xc = get_converter_transformer_impedance_in_ohm(converter).imag();
    double Idc = get_line_dc_current_in_kA();
    double alpha_gamma = deg2rad(get_converter_alpha_or_gamma_in_deg(converter));

    double mu = steps_acos(cos_converter_firing_angle[converter]-SQRT2*Idc*Xc/Eac)-alpha_gamma;

    return rad2deg(mu);
}

double HVDC::get_converter_dc_voltage_in_kV(CONVERTER_SIDE converter) const
{
    return converter_dc_voltage_in_kV[converter];
}

double HVDC::get_line_dc_current_in_kA() const
{
    return line_dc_current_in_kA[RECTIFIER];
}

double HVDC::get_converter_dc_current_in_kA(CONVERTER_SIDE converter) const
{
    return line_dc_current_in_kA[converter];
}

double HVDC::get_converter_ac_current_in_kA(CONVERTER_SIDE converter) const
{
    unsigned int N = get_converter_number_of_bridge(converter);
    double Idc = get_line_dc_current_in_kA();
    double turnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double tap = get_converter_transformer_tap_in_pu(converter);

    return SQRT6_OVER_PI*N*Idc/(turnRatio*tap);
}

double HVDC::get_converter_dc_power_in_MW(CONVERTER_SIDE converter) const
{
    return get_converter_dc_voltage_in_kV(converter)*get_line_dc_current_in_kA();
}

double HVDC::get_converter_ac_active_power_in_MW(CONVERTER_SIDE converter) const
{
    /*double S = get_converter_ac_apparent_power_in_MVA(converter);
    double pf = get_converter_ac_power_factor(converter);

    return S*pf;*/

    //double Iac = get_converter_ac_current_in_kA(converter);
    double Idc = get_converter_dc_current_in_kA(converter);

    unsigned int N = get_converter_number_of_bridge(converter);
    double R = get_converter_transformer_impedance_in_ohm(converter).real();
    //double loss = SQRT3*Iac*Iac*R/N;
    double loss = 2.0*Idc*Idc*R*N;

    switch(converter)
    {
        case RECTIFIER:
            return get_converter_dc_power_in_MW(converter)+loss;
        default:
            return get_converter_dc_power_in_MW(converter)-loss;
    }
}

double HVDC::get_converter_ac_reactive_power_in_MVar(CONVERTER_SIDE converter) const
{
    double Pac = get_converter_ac_active_power_in_MW(converter);
    double pf = get_converter_ac_power_factor(converter);

    return Pac*steps_sqrt(1.0-pf*pf)/pf;
}

double HVDC::get_converter_ac_apparent_power_in_MVA(CONVERTER_SIDE converter) const
{
    double Pac = get_converter_ac_active_power_in_MW(converter);
    double pf = get_converter_ac_power_factor(converter);
    if(pf!=0.0)
        return Pac/pf;
    else
        return Pac;
/*
    double Iac = get_converter_ac_current_in_kA(converter);
    double U = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter));
    return SQRT3*U*Iac;

    double turnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double tap = get_converter_transformer_tap_in_pu(converter);
    double Eac = U/turnRatio/tap;
    return SQRT3*Eac*Iac;
*/
}

double HVDC::get_converter_ac_power_factor(CONVERTER_SIDE converter) const
{
    double mu = get_converter_commutating_overlap_angle_in_deg(converter);
    mu = deg2rad(mu);
    double alpha_gamma = get_converter_alpha_or_gamma_in_deg(converter);
    alpha_gamma = deg2rad(alpha_gamma);

    double tanPhi = 2*mu+steps_sin(2*alpha_gamma)-steps_sin(2.0*(mu+alpha_gamma));
    tanPhi /= (steps_cos(2.0*alpha_gamma)-steps_cos(2.0*(mu+alpha_gamma)));

    return steps_cos(steps_atan(tanPhi));
}


void HVDC::solve_as_rectifier_regulating_power_and_inverter_regulating_voltage()
{
    temp_converter_firing_angle_fixed[RECTIFIER] = false;
    temp_converter_firing_angle_fixed[INVERTER] = false;

    ostringstream osstream;
    //STEPS& toolkit = get_toolkit();
    //osstream<<"Solve "<<get_compound_device_name()<<" as constant power (R) + constant voltage (I) mode.";
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
        temp_converter_firing_angle_fixed[RECTIFIER] = true;
        /*osstream<<"Minimum alpha reached when trying to solve "<<get_compound_device_name()
                <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.";
        toolkit.show_information_with_leading_time_stamp(osstream);*/
    }
    if(minGammaReached)
    {
        temp_converter_firing_angle_fixed[INVERTER] = true;
        /*osstream<<"Minimum gamma reached when trying to solve "<<get_compound_device_name()
                <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.";
        toolkit.show_information_with_leading_time_stamp(osstream);*/
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
            /*osstream<<"Minimum gamma reached when trying to solve "<<get_compound_device_name()
                    <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.\n"
                    <<"HVDC link will turn into CONSTANT GAMMA mode for inverter.";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);*/

            solve_as_rectifier_regulating_power_and_inverter_regulating_gamma();
        }
        else
        {
            if(minAlphaReached and not(minGammaReached))
            {
                /*osstream<<"Minimum alpha reached when trying to solve "<<get_compound_device_name()
                        <<" with CONSTANT POWER and CONSTANT VOTLAGE mode.\n"
                        <<"HVDC link will turn into reduced (DELTI) CONSTANT CURRENT mode controlled by inverter.";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);*/

                solve_as_rectifier_regulating_alpha_and_inverter_regulating_current();
            }
            else
            {
                osstream<<"Warning. Both minimum alpha and minimum gamma reached when trying to solve "<<get_compound_device_name()
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

bool HVDC::solve_converter_transformer_tap_and_desired_firing_angle(CONVERTER_SIDE converter, double Vdc, double Idc)
{
    ostringstream osstream;
    //cout<<"solving "<<get_compound_device_name()<<" with "<<__FUNCTION__<<": "<<get_converter_side_name(converter)<<" desired Vdc = "<<Vdc<<"kV, Idc = "<<Idc<<endl;
    solve_best_converter_transformer_tap_with_min_angle(converter, Vdc, Idc);
    double Tap = get_converter_transformer_tap_in_pu(converter);

    double cos_angle_min = cos_min_firing_angle[converter];
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
            unsigned int nTap = get_converter_transformer_number_of_taps(converter);
            if(nTap==1 or fabs(Tapmax-Tapmin)<DOUBLE_EPSILON) // tap not changeable
                break;
            if(fabs(Tap-Tapmin)<DOUBLE_EPSILON) //reach the minimum
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
        set_converter_alpha_or_gamma_in_deg(converter, rad2deg(steps_acos(cosAngle)), cosAngle);
    else
        set_converter_alpha_or_gamma_in_deg(converter, get_converter_min_alpha_or_gamma_in_deg(converter), cos_min_firing_angle[converter]);

    //osstream<<get_compound_device_name()<<": trying to solve "<<get_converter_side_name(converter)<<" transformer tap = "<<get_converter_transformer_tap_in_pu(converter)
    //       <<", best firing angle = "<<get_converter_alpha_or_gamma_in_deg(converter)<<" deg";
    //show_information_with_leading_time_stamp(osstream);

    return minAngleReached;
}


void HVDC::solve_best_converter_transformer_tap_with_min_angle(CONVERTER_SIDE converter, double Vdc, double Idc)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int N = get_converter_number_of_bridge(converter);

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double maxTap = get_converter_transformer_max_tap_in_pu(converter);
    double minTap = get_converter_transformer_min_tap_in_pu(converter);
    unsigned int nTap = get_converter_transformer_number_of_taps(converter);

    if(fabs(maxTap-minTap)<DOUBLE_EPSILON or nTap == 1)
    {
        set_converter_transformer_tap_in_pu(converter, minTap);
        return;
    }

    double TapStep = (maxTap-minTap)/(nTap-1);

    complex<double> Z = get_converter_transformer_impedance_in_ohm(converter);

    double Vdrop = get_converter_voltage_drop_per_bridge_in_kV(converter);

    double Vbus = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter));

    double Eac_cosAngle = Vdc/N+THREE_OVER_PI*Z.imag()*Idc+2.0*Z.real()*Idc+Vdrop;
    Eac_cosAngle *= PI_OVER_THREE_SQRT2;
    double Eac = Eac_cosAngle/cos_min_firing_angle[converter];
    double Tap = Vbus/(Eac*TurnRatio); // desired
    Tap = minTap +  TapStep*floor((Tap-minTap)/TapStep); // actual
    if(Tap < minTap) Tap = minTap; // apply limit
    if(Tap > maxTap) Tap = maxTap;

    //osstream<<"Best converter transformer tap with min angle: "<<Tap<<" for "<<get_converter_side_name(converter).c_str()<<endl
    //        <<"Vbus = "<<psdb.get_bus_positive_sequence_voltage_in_pu(get_converter_bus(converter));
    //toolkit.show_information_with_leading_time_stamp(osstream);

    set_converter_transformer_tap_in_pu(converter, Tap);
}

void HVDC::solve_as_rectifier_regulating_current_and_inverter_regulating_voltage()
{
    temp_converter_firing_angle_fixed[RECTIFIER] = false;
    temp_converter_firing_angle_fixed[INVERTER] = false;

    ostringstream osstream;
    //STEPS& toolkit = get_toolkit();
    //os<<"Solve %s as constant current (R) + constant voltage (I) mode.",
    //              get_compound_device_name().c_str());
    //show_information_with_leading_time_stamp(osstream);

    // solve the desired dc voltage and dc current
    double Idc = get_rectifier_nominal_dc_current_command_in_kA();
    double VdcI = get_inverter_nominal_dc_voltage_command_in_kV();
    double R = get_line_resistance_in_ohm();
    double VdcR = VdcI + Idc*R;

    //osstream<<"trying to solve "<<get_compound_device_name()<<" with desired DC voltage rec "<<VdcR<<" kV, inv "<<VdcI<<" kV, "
    //       <<" I "<<Idc<<" kA";
    //toolkit.show_information_with_leading_time_stamp(osstream);

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
            temp_converter_firing_angle_fixed[INVERTER] = true;
            /*osstream<<"Minimum gamma reached when trying to solve "<<get_compound_device_name()
                    <<" with CONSTANT CURRENT and CONSTANT VOTLAGE mode.";
            toolkit.show_information_with_leading_time_stamp(osstream);*/

            solve_as_rectifier_regulating_current_and_inverter_regulating_gamma();
        }
        else
        {
            if(minAlphaReached and not(minGammaReached))
            {
                temp_converter_firing_angle_fixed[RECTIFIER] = true;
                /*osstream<<"Minimum alpha reached when trying to solve "<<get_compound_device_name()
                        <<" with CONSTANT CURRENT and CONSTANT VOTLAGE mode.";
                toolkit.show_information_with_leading_time_stamp(osstream);*/

                solve_as_rectifier_regulating_alpha_and_inverter_regulating_current();
            }
            else
            {
                // error!!!!!
                temp_converter_firing_angle_fixed[INVERTER] = true;
                temp_converter_firing_angle_fixed[RECTIFIER] = true;

                /*osstream<<"Warning. Both minimum alpha and minimum gamma reached when trying to solve "<<get_compound_device_name()
                        <<" with CONSTANT CURRENT and CONSTANT VOTLAGE mode."
                        <<"HVDC link will be with minimum alpha and gamma. The solution may be incorrect.";
                toolkit.show_information_with_leading_time_stamp(osstream);*/

                solve_with_solved_tap_and_firing_angle();
            }
        }
    }
}

void HVDC::solve_as_rectifier_regulating_power_and_inverter_regulating_gamma()
{
    temp_converter_firing_angle_fixed[RECTIFIER] = false;
    temp_converter_firing_angle_fixed[INVERTER] = true;

    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    //osstream<<"Solve "<<get_compound_device_name()<<" as constant power (R) + constant gamma (I) mode.";
    //toolkit.show_information_with_leading_time_stamp(osstream);

    double Pn = get_nominal_dc_power_in_MW();
    CONVERTER_SIDE Pside = get_side_to_hold_dc_power();
    double In = get_nominal_dc_current_in_kA();

    double VdropI = get_converter_voltage_drop_per_bridge_in_kV(INVERTER);
    double R = get_line_resistance_in_ohm();

    double TurnRatioI = get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER)/get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

    complex<double> ZI = get_converter_transformer_impedance_in_ohm(INVERTER);

    unsigned int NI = get_converter_number_of_bridge(INVERTER);


    double VdcR, VdcI, Idc;

    double VbusI = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(INVERTER));

    double EacI, cosAlpha;

    double TapR, TapI;

    set_converter_alpha_or_gamma_in_deg(INVERTER, get_converter_min_alpha_or_gamma_in_deg(INVERTER), cos_min_firing_angle[INVERTER]);
    if(Pside == INVERTER)
    {
        Idc = In; // desired
        VdcI = Pn/Idc; // desired
        //solve TapI with desired Idc, and VdcI
        solve_best_converter_transformer_tap_with_min_angle(INVERTER, VdcI, Idc);
        TapI = get_converter_transformer_tap_in_pu(INVERTER);
        // with solved Tap, solve Idc again
        EacI = VbusI/TurnRatioI/TapI;
        double a = -THREE_OVER_PI*ZI.imag()+2.0*ZI.real();
        double b = THREE_SQRT2_OVER_PI*EacI*cos_min_firing_angle[INVERTER]-VdropI;
        double c = -Pn/NI;
        double Idc1, Idc2;
        Idc1 = (-b+steps_sqrt(b*b-4*a*c))/(2*a);
        Idc2 = (-b-steps_sqrt(b*b-4*a*c))/(2*a);
        Idc = (fabs(Idc1-In)<fabs(Idc2-In)) ? Idc1:Idc2;

        VdcI = solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(INVERTER, Idc, TapI);
        VdcR = VdcI+Idc*R;
        // solve tap with Vdc
        solve_best_converter_transformer_tap_with_min_angle(RECTIFIER, VdcR, Idc);
        TapR =  get_converter_transformer_tap_in_pu(RECTIFIER);
        // with Tap, solve alpha
        cosAlpha = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(RECTIFIER, VdcR, Idc, TapR);
        if(cosAlpha>=cos_min_firing_angle[RECTIFIER])
        {
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER), cos_min_firing_angle[RECTIFIER]);
            temp_converter_firing_angle_fixed[RECTIFIER] = true;
        }
        else//solved
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(steps_acos(cosAlpha)), cosAlpha);
    }
    else //RECTIFIER
    {
        Idc = In; // desired
        VdcR = Pn/Idc; // desired
        VdcI = VdcR - Idc*R;
        //osstream<<"HVDC desired voltage: "<<VdcR<<", "<<VdcI;
        //toolkit.show_information_with_leading_time_stamp(osstream);
        //solve TapR with desired Idc, and VdcR
        solve_best_converter_transformer_tap_with_min_angle(INVERTER, VdcI, Idc);
        TapI = get_converter_transformer_tap_in_pu(INVERTER);

        solve_best_converter_transformer_tap_with_min_angle(RECTIFIER, VdcR, Idc);
        TapR = get_converter_transformer_tap_in_pu(RECTIFIER);
        // solve cosAlpha
        cosAlpha = solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(RECTIFIER, VdcR, Idc, TapR);
        if(cosAlpha>=cos_min_firing_angle[RECTIFIER])
        {
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER), cos_min_firing_angle[RECTIFIER]);
            temp_converter_firing_angle_fixed[RECTIFIER] = true;
        }
        else
            set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(steps_acos(cosAlpha)), cosAlpha);
    }
    solve_with_solved_tap_and_firing_angle();
}

void HVDC::solve_as_rectifier_regulating_current_and_inverter_regulating_gamma()
{
    temp_converter_firing_angle_fixed[RECTIFIER] = false;
    temp_converter_firing_angle_fixed[INVERTER] = true;
    //ostringstream osstream;
    //os<<"Solve %s as constant current (R) + constant gamma (I) mode.",
    //              get_compound_device_name().c_str());
    //show_information_with_leading_time_stamp(osstream);
    set_converter_alpha_or_gamma_in_deg(INVERTER, get_converter_min_alpha_or_gamma_in_deg(INVERTER), cos_min_firing_angle[INVERTER]);

    double Vn = get_nominal_dc_voltage_in_kV();
    double In = get_nominal_dc_current_in_kA();

    double R = get_line_resistance_in_ohm();

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
    if(cosAlpha>=cos_min_firing_angle[RECTIFIER])
    {
        set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER), cos_min_firing_angle[RECTIFIER]);
        temp_converter_firing_angle_fixed[RECTIFIER] = true;
    }
    else//solved
        set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(steps_acos(cosAlpha)), cosAlpha);

    solve_with_solved_tap_and_firing_angle();
}

void HVDC::solve_as_rectifier_regulating_alpha_and_inverter_regulating_current()
{
    temp_converter_firing_angle_fixed[RECTIFIER] = true;
    temp_converter_firing_angle_fixed[INVERTER] = false;
    //ostringstream osstream;
    //os<<"Solve %s as constant alpha (R) + constant current (I) mode.",
    //              get_compound_device_name().c_str());
    //show_information_with_leading_time_stamp(osstream);
    set_converter_alpha_or_gamma_in_deg(RECTIFIER, get_converter_min_alpha_or_gamma_in_deg(RECTIFIER), cos_min_firing_angle[RECTIFIER]);

    double In = get_rectifier_nominal_dc_current_command_in_kA();
    double margin = get_current_power_margin();
    double Idc = In*(1.0-margin); // desired

    double VdcI = get_inverter_nominal_dc_voltage_command_in_kV();

    double R = get_line_resistance_in_ohm();

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

    if(cosGamma>=cos_min_firing_angle[INVERTER])
    {
        set_converter_alpha_or_gamma_in_deg(INVERTER, get_converter_min_alpha_or_gamma_in_deg(INVERTER), cos_min_firing_angle[INVERTER]);
        temp_converter_firing_angle_fixed[INVERTER] = true;
    }
    else//solved
        set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(steps_acos(cosGamma)), cosGamma);

    solve_with_solved_tap_and_firing_angle();
}

void HVDC::solve_with_solved_tap_and_firing_angle()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    //osstream<<"solving "<<get_compound_device_name()<<" with solved tap rec "<<get_converter_transformer_tap_in_pu(RECTIFIER)<<", "
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

    complex<double> ZR = get_converter_transformer_impedance_in_ohm(RECTIFIER);
    complex<double> ZI = get_converter_transformer_impedance_in_ohm(INVERTER);

    unsigned int NR = get_converter_number_of_bridge(RECTIFIER);
    unsigned int NI = get_converter_number_of_bridge(INVERTER);

    double VbusR = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(RECTIFIER));
    double VbusI = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(INVERTER));

    double EacR = VbusR/(TurnRatioR*TapR);
    double EacI = VbusI/(TurnRatioI*TapI);

    double Vdc0_r = THREE_SQRT2_OVER_PI*NR*EacR;
    double Vdc0_i = THREE_SQRT2_OVER_PI*NI*EacI;

    double Rceq_r = NR*(THREE_OVER_PI*ZR.imag()+2.0*ZR.real());
    double Rceq_i = NI*(THREE_OVER_PI*ZI.imag()+2.0*ZI.real());

    double Idc = Vdc0_r*cos_converter_firing_angle[RECTIFIER]-NR*VdropR - (Vdc0_i*cos_converter_firing_angle[INVERTER]-NI*VdropI);
    Idc /= (R+Rceq_r-Rceq_i);

    double VdcR = Vdc0_r*cos_converter_firing_angle[RECTIFIER]-Rceq_r*Idc-NR*VdropR;
    double VdcI = Vdc0_i*cos_converter_firing_angle[INVERTER]-Rceq_i*Idc-NI*VdropI;

    //double Idc = NR*(THREE_SQRT2_OVER_PI*EacR*cos_converter_firing_angle[RECTIFIER] - VdropR) - NI*(THREE_SQRT2_OVER_PI*EacI*cos_converter_firing_angle[INVERTER] - VdropI);
    //Idc /= (R+NR*ZR.imag()*THREE_OVER_PI-NI*ZI.imag()*THREE_OVER_PI+NR*2.0*ZR.real()+NI*2.0*ZI.real());

    //double VdcR = NR*(THREE_SQRT2_OVER_PI*EacR*cos_converter_firing_angle[RECTIFIER] - ZR.imag()*THREE_OVER_PI*Idc - 2.0*ZR.real()*Idc- VdropR);
    //double VdcI = NI*(THREE_SQRT2_OVER_PI*EacI*cos_converter_firing_angle[INVERTER] - ZI.imag()*THREE_OVER_PI*Idc + 2.0*ZI.real()*Idc- VdropI);

    set_line_dc_current_in_kA(Idc);
    set_converter_dc_voltage_in_kV(RECTIFIER, VdcR);
    set_converter_dc_voltage_in_kV(INVERTER, VdcI);

    //osstream<<"DC current "<<get_line_dc_current_in_kA()<<" kA, DC voltage "<<get_converter_dc_voltage_in_kV(RECTIFIER)
    //       <<" kV, "<<get_converter_dc_voltage_in_kV(INVERTER)<<" kV";
    //show_information_with_leading_time_stamp(osstream);
}


double HVDC::solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(CONVERTER_SIDE converter, double Vdc, double Idc, double Tap)
{
    // solve best alpha and gamma
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Vdrop = get_converter_voltage_drop_per_bridge_in_kV(converter);

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    complex<double> Z = get_converter_transformer_impedance_in_ohm(converter);

    unsigned int N = get_converter_number_of_bridge(converter);

    double Vbus = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter));
    double Eac = Vbus/(TurnRatio*Tap); // actual EacR
    double Eac_cosAngle = Vdc/N+THREE_OVER_PI*Z.imag()*Idc+2.0*Z.real()*Idc+Vdrop;
           Eac_cosAngle *= PI_OVER_THREE_SQRT2;
    double cosAngle = Eac_cosAngle/Eac; // desired alpha

    return cosAngle;
}

double HVDC::solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(CONVERTER_SIDE converter, double Idc, double Tap)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double Vdrop = get_converter_voltage_drop_per_bridge_in_kV(converter);

    double TurnRatio = get_converter_transformer_grid_side_base_voltage_in_kV(converter)/get_converter_transformer_converter_side_base_voltage_in_kV(converter);

    complex<double> Z = get_converter_transformer_impedance_in_ohm(converter);

    unsigned int N = get_converter_number_of_bridge(converter);

    double Vbus = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_bus(converter));
    double Eac = Vbus/(TurnRatio*Tap);
    double Vdc = N*(THREE_SQRT2_OVER_PI*Eac*cos_min_firing_angle[RECTIFIER]-THREE_OVER_PI*Z.imag()*Idc-2.0*Z.real()*Idc-Vdrop);
    return Vdc;
}

complex<double> HVDC::get_converter_dynamic_current_in_pu_based_on_system_base_power(CONVERTER_SIDE converter)
{
    if(get_status() == true)
    {
        HVDC_MODEL* model = get_hvdc_model();
        if(model!=NULL)
            return model->get_converter_ac_current_in_pu(converter);
        else
            return 0.0;
    }
    else
        return 0.0;
}
