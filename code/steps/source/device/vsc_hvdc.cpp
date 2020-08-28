#include "header/device/vsc_hvdc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_models.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

VSC_HVDC::VSC_HVDC(STEPS& toolkit) : NONBUS_DEVICE(toolkit)
{
    clear();
}

VSC_HVDC::~VSC_HVDC()
{
}

string VSC_HVDC::get_converter_side_name(CONVERTER_SIDE converter) const
{
    switch(converter)
    {
        case RECTIFIER:
            return "RECTIFIER";
        default:
            return "INVERTER";
    }
}

void VSC_HVDC::set_converter_bus(CONVERTER_SIDE converter, const unsigned int bus)
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
            osstream<<"Bus "<<bus<<" does not exist for setting up "<<converter_name<<" side bus of vsc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            converter_bus[converter] = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<converter_name<<" bus of vsc-hvdc link."<<endl
                <<"0 will be set to indicate invalid vsc-hvdc link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        converter_bus[converter] = bus;
    }
}


void VSC_HVDC::set_identifier(const string hvdc_id)
{
    this->identifier = hvdc_id;
}

void VSC_HVDC::set_name(const string name)
{
    this->vsc_hvdc_name = trim_string(name);
}

void VSC_HVDC::set_status(const bool status)
{
    this->status = status;
}

void VSC_HVDC::set_line_resistance_in_ohm(const double R)
{
    if(R>0.0)
        this->line_R_in_ohm = R;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Non-positive ("<<R<<" ohm) is not allowed for setting VSC_HVDC line resistance of "<<get_compound_device_name()<<endl
          <<"0.0 will be set to indicate invalid VSC_HVDC link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        this->line_R_in_ohm = 0.0;
    }
}

void VSC_HVDC::set_converter_dc_operation_mode(CONVERTER_SIDE converter, const VSC_HVDC_DC_CONTROL_MODE mode)
{
    dc_control_mode[converter] =  mode;
    if(get_converter_dc_operation_mode(RECTIFIER)==VSC_DC_POWER_CONTORL and
       get_converter_dc_operation_mode(INVERTER)==VSC_DC_POWER_CONTORL)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. It is invalid to set both converters to be operated in DC power control mode for "<<get_compound_device_name()<<endl
                <<"Check input of VSC-HVDC DC operation mode. "<<endl
                <<"The converter "<<get_converter_side_name(converter)<<" will be set as DC voltage control mode.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        dc_control_mode[converter] = VSC_DC_VOLTAGE_CONTORL;
    }
}

void VSC_HVDC::set_converter_ac_operation_mode(CONVERTER_SIDE converter, const VSC_HVDC_AC_CONTROL_MODE mode)
{
    ac_control_mode[converter] =  mode;
}

void VSC_HVDC::set_converter_nominal_dc_power_command_in_MW(CONVERTER_SIDE converter, double P)
{
    nominal_dc_power_in_MW[converter] = P;
}

void VSC_HVDC::set_converter_nominal_dc_voltage_command_in_kV(CONVERTER_SIDE converter, double V)
{
    nominal_dc_voltage_in_kV[converter] = V;
}

void VSC_HVDC::set_converter_nominal_ac_voltage_command_in_pu(CONVERTER_SIDE converter, double V)
{
    nominal_ac_voltage_in_pu[converter] = V;
}

void VSC_HVDC::set_converter_nominal_ac_power_factor_command(CONVERTER_SIDE converter, double pf)
{
    nominal_ac_power_factor[converter] = pf;
    actual_ac_power_factor_command[converter] = pf;
}

void VSC_HVDC::set_converter_loss_factor_A_in_kW(CONVERTER_SIDE converter, double A)
{
    converter_loss_coefficient_A_in_kW[converter] = A;
}

void VSC_HVDC::set_converter_loss_factor_B_in_kW_per_amp(CONVERTER_SIDE converter, double B)
{
    converter_loss_coefficient_B_in_kW_per_amp[converter]  = B;
}

void VSC_HVDC::set_converter_minimum_loss_in_kW(CONVERTER_SIDE converter, double P)
{
    min_converter_loss_in_kW[converter] = P;
}

void VSC_HVDC::set_converter_MVA_rating_in_MVA(CONVERTER_SIDE converter, double S)
{
    converter_MVA_rating_in_MVA[converter] = S;
}

void VSC_HVDC::set_converter_current_rating_in_amp(CONVERTER_SIDE converter, double I)
{
    converter_current_rating_in_amp[converter] = I;
}

void VSC_HVDC::set_converter_power_weighting_factor(CONVERTER_SIDE converter, double pwf)
{
    power_weighting_factor[converter] = pwf;
}

void VSC_HVDC::set_converter_Qmax_in_MVar(CONVERTER_SIDE converter, double Q)
{
    Qmax_MVar[converter] = Q;
}

void VSC_HVDC::set_converter_Qmin_in_MVar(CONVERTER_SIDE converter, double Q)
{
    Qmin_MVar[converter] = Q;
}

void VSC_HVDC::set_converter_remote_bus_to_regulate(CONVERTER_SIDE converter, unsigned int bus)
{
    remote_bus_to_regulate[converter] = bus;
}

void VSC_HVDC::set_converter_remote_regulation_percent(CONVERTER_SIDE converter, double rmpct)
{
    remote_regulation_percent[converter] = rmpct;
}

unsigned int VSC_HVDC::get_converter_bus(CONVERTER_SIDE converter) const
{
    return converter_bus[converter];
}

BUS* VSC_HVDC::get_converter_bus_pointer(CONVERTER_SIDE converter) const
{
    return converter_busptr[converter];
}

string VSC_HVDC::get_identifier() const
{
    return identifier;
}

string VSC_HVDC::get_name() const
{
    return vsc_hvdc_name;
}

bool VSC_HVDC::get_status() const
{
    return status;
}

double VSC_HVDC::get_line_resistance_in_ohm() const
{
    return line_R_in_ohm;
}

VSC_HVDC_DC_CONTROL_MODE VSC_HVDC::get_converter_dc_operation_mode(CONVERTER_SIDE converter) const
{
    return dc_control_mode[converter];
}

VSC_HVDC_AC_CONTROL_MODE VSC_HVDC::get_converter_ac_operation_mode(CONVERTER_SIDE converter) const
{
    return ac_control_mode[converter];
}

double VSC_HVDC::get_converter_nominal_dc_power_command_in_MW(CONVERTER_SIDE converter) const
{
    return nominal_dc_power_in_MW[converter];
}

double VSC_HVDC::get_converter_nominal_dc_voltage_command_in_kV(CONVERTER_SIDE converter) const
{
    return nominal_dc_voltage_in_kV[converter];
}

double VSC_HVDC::get_converter_nominal_ac_voltage_command_in_pu(CONVERTER_SIDE converter) const
{
    return nominal_ac_voltage_in_pu[converter];
}

double VSC_HVDC::get_converter_nominal_ac_power_factor_command(CONVERTER_SIDE converter) const
{
    return nominal_ac_power_factor[converter];
}

double VSC_HVDC::get_converter_loss_factor_A_in_kW(CONVERTER_SIDE converter) const
{
    return converter_loss_coefficient_A_in_kW[converter];
}

double VSC_HVDC::get_converter_loss_factor_B_in_kW_per_amp(CONVERTER_SIDE converter) const
{
    return converter_loss_coefficient_B_in_kW_per_amp[converter];
}

double VSC_HVDC::get_converter_minimum_loss_in_kW(CONVERTER_SIDE converter) const
{
    return min_converter_loss_in_kW[converter];
}

double VSC_HVDC::get_converter_MVA_rating_in_MVA(CONVERTER_SIDE converter) const
{
    return converter_MVA_rating_in_MVA[converter];
}

double VSC_HVDC::get_converter_current_rating_in_amp(CONVERTER_SIDE converter) const
{
    return converter_current_rating_in_amp[converter];
}

double VSC_HVDC::get_converter_power_weighting_factor(CONVERTER_SIDE converter) const
{
    return power_weighting_factor[converter];
}

double VSC_HVDC::get_converter_Qmax_in_MVar(CONVERTER_SIDE converter) const
{
    return Qmax_MVar[converter];
}

double VSC_HVDC::get_converter_Qmin_in_MVar(CONVERTER_SIDE converter) const
{
    return Qmin_MVar[converter];
}

unsigned int VSC_HVDC::get_converter_remote_bus_to_regulate(CONVERTER_SIDE converter) const
{
    return remote_bus_to_regulate[converter];
}

double VSC_HVDC::get_converter_remote_regulation_percent(CONVERTER_SIDE converter) const
{
    return remote_regulation_percent[converter];
}

void VSC_HVDC::initialize_dc_power_and_voltage_command()
{
    VSC_HVDC_DC_CONTROL_MODE rec_mode = get_converter_dc_operation_mode(RECTIFIER);
    VSC_HVDC_DC_CONTROL_MODE inv_mode = get_converter_dc_operation_mode(INVERTER);

    double R = get_line_resistance_in_ohm();
    double I;
    if(rec_mode == VSC_DC_VOLTAGE_CONTORL and inv_mode == VSC_DC_VOLTAGE_CONTORL) // both VSC_DC_VOLTAGE_CONTORL
    {
        double rec_V = get_converter_nominal_dc_voltage_command_in_kV(RECTIFIER);
        double inv_V = get_converter_nominal_dc_voltage_command_in_kV(INVERTER);
        I = fabs(rec_V-inv_V)/R;
        set_nominal_dc_current_command_in_kA(I);
        if(rec_V>inv_V)
        {
            set_converter_nominal_dc_power_command_in_MW(RECTIFIER, -(rec_V*I));
            set_converter_nominal_dc_power_command_in_MW(INVERTER, inv_V*I);
        }
        else
        {
            set_converter_nominal_dc_power_command_in_MW(RECTIFIER, rec_V*I);
            set_converter_nominal_dc_power_command_in_MW(INVERTER, -(inv_V*I));
        }
    }
    else
    {
        if(rec_mode == VSC_DC_POWER_CONTORL and inv_mode == VSC_DC_VOLTAGE_CONTORL)
        {
            double rec_P = get_converter_nominal_dc_power_command_in_MW(RECTIFIER);
            double inv_V = get_converter_nominal_dc_voltage_command_in_kV(INVERTER);
            if(rec_P>=0.0) // into ac network
            {
                // rec_P = (inv_V-IR)*I
                // R*I^2 - inv_V*I + rec_P = 0
                I =(inv_V-steps_sqrt(inv_V*inv_V-4.0*R*rec_P))/(2.0*R);
                set_converter_nominal_dc_voltage_command_in_kV(RECTIFIER, inv_V-I*R);
                set_converter_nominal_dc_power_command_in_MW(INVERTER, -(rec_P+I*I*R));
                set_nominal_dc_current_command_in_kA(I);
            }
            else // withdrawn from ac network
            {
                rec_P = -rec_P;
                // rec_P = (inv_V+IR)*I
                // R*I^2 + inv_V*I - rec_P = 0
                I =(-inv_V+steps_sqrt(inv_V*inv_V+4.0*R*rec_P))/(2.0*R);
                set_converter_nominal_dc_voltage_command_in_kV(RECTIFIER, inv_V+I*R);
                set_converter_nominal_dc_power_command_in_MW(INVERTER, rec_P-I*I*R);
                set_nominal_dc_current_command_in_kA(I);
            }
        }
        else
        {
            if(rec_mode == VSC_DC_VOLTAGE_CONTORL and inv_mode == VSC_DC_POWER_CONTORL)
            {
                double rec_V = get_converter_nominal_dc_voltage_command_in_kV(RECTIFIER);
                double inv_P = get_converter_nominal_dc_power_command_in_MW(INVERTER);
                if(inv_P>=0.0) // into ac network
                {
                    // inv_P = (rec_V-IR)*I
                    // R*I^2 - rec_V*I + inv_P = 0
                    I =(rec_V-steps_sqrt(rec_V*rec_V-4.0*R*inv_P))/(2.0*R);
                    set_converter_nominal_dc_voltage_command_in_kV(INVERTER, rec_V-I*R);
                    set_converter_nominal_dc_power_command_in_MW(RECTIFIER, -(inv_P+I*I*R));
                    set_nominal_dc_current_command_in_kA(I);
                }
                else // withdrawn from ac network
                {
                    inv_P = -inv_P;
                    // inv_P = (rec_V+IR)*I
                    // R*I^2 + rec_V*I - inv_P = 0
                    I =(-rec_V+steps_sqrt(rec_V*rec_V+4.0*R*inv_P))/(2.0*R);
                    set_converter_nominal_dc_voltage_command_in_kV(INVERTER, rec_V+I*R);
                    set_converter_nominal_dc_power_command_in_MW(RECTIFIER, inv_P-I*I*R);
                    set_nominal_dc_current_command_in_kA(I);
                }
            }
            else // both VSC_DC_POWER_CONTORL
            {
                STEPS& toolkit = get_toolkit();
                ostringstream osstream;
                osstream<<"Error. Both converters are operated in DC power control mode for "<<get_compound_device_name()<<".\n"
                        <<"Check input data";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    set_converter_actual_dc_power_command_in_MW(RECTIFIER, get_converter_nominal_dc_power_command_in_MW(RECTIFIER));
    set_converter_actual_dc_power_command_in_MW(INVERTER, get_converter_nominal_dc_power_command_in_MW(INVERTER));
    set_converter_actual_dc_voltage_command_in_kV(RECTIFIER, get_converter_nominal_dc_voltage_command_in_kV(RECTIFIER));
    set_converter_actual_dc_voltage_command_in_kV(INVERTER, get_converter_nominal_dc_voltage_command_in_kV(INVERTER));
    set_actual_dc_current_command_in_kA(get_nominal_dc_current_command_in_kA());
    set_converter_actual_ac_power_factor_command(RECTIFIER, get_converter_nominal_ac_power_factor_command(RECTIFIER));
    set_converter_actual_ac_power_factor_command(INVERTER, get_converter_nominal_ac_power_factor_command(INVERTER));

    nominal_dc_setpoints_initialized = true;
}

void VSC_HVDC::set_nominal_dc_current_command_in_kA(double I)
{
    nominal_dc_current_in_amp = I*1000.0;
}

void VSC_HVDC::set_converter_actual_dc_power_command_in_MW(CONVERTER_SIDE converter, double P)
{
    actual_dc_power_command_MW[converter] = P;
}

void VSC_HVDC::set_converter_actual_dc_voltage_command_in_kV(CONVERTER_SIDE converter, double V)
{
    actual_dc_voltage_command_kV[converter] = V;
}

void VSC_HVDC::set_actual_dc_current_command_in_kA(double I)
{
    actual_dc_current_command_in_amp = I*1000.0;
}

void VSC_HVDC::set_converter_actual_ac_power_factor_command(CONVERTER_SIDE converter, double eta)
{
    actual_ac_power_factor_command[converter] = eta;
}


double VSC_HVDC::get_nominal_dc_current_command_in_kA() const
{
    return nominal_dc_current_in_amp*0.001;
}

double VSC_HVDC::get_converter_actual_dc_power_command_in_MW(CONVERTER_SIDE converter) const
{
    return actual_dc_power_command_MW[converter];
}

double VSC_HVDC::get_converter_actual_dc_voltage_command_in_kV(CONVERTER_SIDE converter) const
{
    return actual_dc_voltage_command_kV[converter];
}

double VSC_HVDC::get_actual_dc_current_command_in_kA() const
{
    return actual_dc_current_command_in_amp*0.001;
}

double VSC_HVDC::get_converter_actual_ac_power_factor_command(CONVERTER_SIDE converter) const
{
    return actual_ac_power_factor_command[converter];
}

void VSC_HVDC::reverse_converters()
{
    unsigned int bus = converter_bus[0];
    converter_bus[0] = converter_bus[1];
    converter_bus[1] = bus;

    BUS* busptr = converter_busptr[0];
    converter_busptr[0] = converter_busptr[1];
    converter_busptr[1] = busptr;

    VSC_HVDC_DC_CONTROL_MODE dc_mode;
    dc_mode = dc_control_mode[0];
    dc_control_mode[0] = dc_control_mode[1];
    dc_control_mode[1] = dc_mode;

    VSC_HVDC_AC_CONTROL_MODE ac_mode;
    ac_mode = ac_control_mode[0];
    ac_control_mode[0] = ac_control_mode[1];
    ac_control_mode[1] = ac_mode;

    double power_command;
    power_command = nominal_dc_power_in_MW[0];
    nominal_dc_power_in_MW[0] = nominal_dc_power_in_MW[1];
    nominal_dc_power_in_MW[1] = power_command;

    double voltage_command;
    voltage_command = nominal_dc_voltage_in_kV[0];
    nominal_dc_voltage_in_kV[0] = nominal_dc_voltage_in_kV[1];
    nominal_dc_voltage_in_kV[1] = voltage_command;

    voltage_command = nominal_ac_voltage_in_pu[0];
    nominal_ac_voltage_in_pu[0] = nominal_ac_voltage_in_pu[1];
    nominal_ac_voltage_in_pu[1] = voltage_command;

    double pf_command;
    pf_command = nominal_ac_power_factor[0];
    nominal_ac_power_factor[0] = nominal_ac_power_factor[1];
    nominal_ac_power_factor[1] = pf_command;

    double A;
    A = converter_loss_coefficient_A_in_kW[0];
    converter_loss_coefficient_A_in_kW[0] = converter_loss_coefficient_A_in_kW[1];
    converter_loss_coefficient_A_in_kW[1] = A;

    double B;
    B = converter_loss_coefficient_B_in_kW_per_amp[0];
    converter_loss_coefficient_B_in_kW_per_amp[0] = converter_loss_coefficient_B_in_kW_per_amp[1];
    converter_loss_coefficient_B_in_kW_per_amp[1] = B;

    double minloss;
    minloss = min_converter_loss_in_kW[0];
    min_converter_loss_in_kW[0] = min_converter_loss_in_kW[1];
    min_converter_loss_in_kW[1] = minloss;

    double smax;
    smax = converter_MVA_rating_in_MVA[0];
    converter_MVA_rating_in_MVA[0] = converter_MVA_rating_in_MVA[1];
    converter_MVA_rating_in_MVA[1] = smax;

    double imax;
    imax = converter_current_rating_in_amp[0];
    converter_current_rating_in_amp[0] = converter_current_rating_in_amp[1];
    converter_current_rating_in_amp[1] = imax;

    double pwf;
    pwf = power_weighting_factor[0];
    power_weighting_factor[0] = power_weighting_factor[1];
    power_weighting_factor[1] = pwf;

    double qmax;
    qmax = Qmax_MVar[0];
    Qmax_MVar[0] = Qmax_MVar[1];
    Qmax_MVar[1] = qmax;

    double qmin;
    qmin = Qmin_MVar[0];
    Qmin_MVar[0] = Qmin_MVar[1];
    Qmin_MVar[1] = qmin;

    unsigned int rbus;
    rbus = remote_bus_to_regulate[0];
    remote_bus_to_regulate[0] = remote_bus_to_regulate[1];
    remote_bus_to_regulate[1] = rbus;

    double rmpct;
    rmpct = remote_regulation_percent[0];
    remote_regulation_percent[0] = remote_regulation_percent[1];
    remote_regulation_percent[1] = rmpct;
}

bool VSC_HVDC::is_valid() const
{
    if(get_converter_bus(RECTIFIER)!=0 and get_converter_bus(INVERTER)!=0) return true;
    else return false;
}

void VSC_HVDC::check()
{
    ;
}

void VSC_HVDC::clear()
{
    converter_bus[RECTIFIER] = 0;
    converter_bus[INVERTER] = 0;
    converter_busptr[RECTIFIER] = NULL;
    converter_busptr[INVERTER] = NULL;
    set_identifier(""); set_name(""); set_status(false);
    line_R_in_ohm = 0.0;

    dc_control_mode[0] = VSC_DC_VOLTAGE_CONTORL;      dc_control_mode[1] = VSC_DC_VOLTAGE_CONTORL;
    ac_control_mode[0] = VSC_AC_POWER_FACTOR_CONTROL; ac_control_mode[1] = VSC_AC_POWER_FACTOR_CONTROL;

    nominal_dc_voltage_in_kV[0] = 0.0; nominal_dc_voltage_in_kV[1] = 0.0;
    nominal_dc_power_in_MW[0] = 0.0; nominal_dc_power_in_MW[1] = 0.0;
    nominal_dc_current_in_amp = 0.0;
    nominal_ac_voltage_in_pu[0] = 1.0; nominal_ac_voltage_in_pu[1] = 1.0;
    nominal_ac_power_factor[0] = 1.0; nominal_ac_power_factor[1] = 1.0;

    converter_loss_coefficient_A_in_kW[0] = 0.0; converter_loss_coefficient_A_in_kW[1] = 0.0;
    converter_loss_coefficient_B_in_kW_per_amp[0] = 0.0; converter_loss_coefficient_B_in_kW_per_amp[1] = 0.0;
    min_converter_loss_in_kW[0] = 0.0; min_converter_loss_in_kW[1] = 0.0;

    converter_MVA_rating_in_MVA[0] = 0.0; converter_MVA_rating_in_MVA[1] = 0.0;
    converter_current_rating_in_amp[0] = 0.0; converter_current_rating_in_amp[1] = 0.0;

    power_weighting_factor[0] = 0.0; power_weighting_factor[1] = 0.0;
    Qmax_MVar[0] = 0.0; Qmax_MVar[1] = 0.0;
    Qmin_MVar[0] = 0.0; Qmin_MVar[1] = 0.0;

    remote_bus_to_regulate[0] = 0; remote_bus_to_regulate[1] = 0;
    remote_regulation_percent[0] = 100.0; remote_regulation_percent[1] = 100.0;

    vsc_hvdc_model = NULL;
    auxiliary_signal_model = NULL;

    nominal_dc_setpoints_initialized = false;
    actual_dc_power_command_MW[0] = 0.0; actual_dc_power_command_MW[1] = 0.0;
    actual_dc_voltage_command_kV[0] = 0.0; actual_dc_voltage_command_kV[1] = 0.0;
    actual_dc_current_command_in_amp = 0.0;
    actual_ac_power_factor_command[0] = 1.0; actual_ac_power_factor_command[1] = 1.0;
}

bool VSC_HVDC::is_connected_to_bus(unsigned int bus) const
{
    if(get_converter_bus(RECTIFIER)==bus || get_converter_bus(INVERTER)==bus) return true;
    else                                                                      return false;
}

bool VSC_HVDC::is_in_area(unsigned int area) const
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

bool VSC_HVDC::is_in_zone(unsigned int zone) const
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

void VSC_HVDC::report() const
{
    ostringstream osstream;
    osstream<<"VSC_HVDC '"<<get_name()<<"': Rdc = "<<get_line_resistance_in_ohm()<<" ohm";
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);

    /*
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
    */
}


void VSC_HVDC::save() const
{
    ;
}

void VSC_HVDC::set_model(const MODEL* model)
{
    if(model->get_model_type()=="VSC_HVDC")
        set_vsc_hvdc_model((VSC_HVDC_MODEL*) model);
    else
    {
        if(model->get_model_type()=="AUXILIARY SIGNAL")
            set_auxiliary_signal_model((AUXILIARY_SIGNAL_MODEL*) model);
        else
        {
            ostringstream osstream;
            osstream<<"Waring. Neither AUXILIARY SIGNAL model nor VSC_HVDC model is given to set dynamic model for "<<get_compound_device_name();
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


MODEL* VSC_HVDC::get_model_of_type(string model_type)
{
    model_type = string2upper(model_type);
    if(model_type=="VSC HVDC")
        return get_vsc_hvdc_model();
    if(model_type=="AUXILIARY SIGNAL")
        return get_auxiliary_signal_model();
    return NULL;
}

void VSC_HVDC::set_vsc_hvdc_model(VSC_HVDC_MODEL* model)
{
    if(model!=NULL)
        vsc_hvdc_model = model;
}

void VSC_HVDC::set_auxiliary_signal_model(AUXILIARY_SIGNAL_MODEL* model)
{
    if(model != NULL)
        auxiliary_signal_model = model;
}

VSC_HVDC_MODEL* VSC_HVDC::get_vsc_hvdc_model() const
{
    return vsc_hvdc_model;
}

AUXILIARY_SIGNAL_MODEL* VSC_HVDC::get_auxiliary_signal_model() const
{
    return auxiliary_signal_model;
}

void VSC_HVDC::run(DYNAMIC_MODE mode)
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
                    osstream<<"VSC_HVDC data before dynamic initialization";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    show_solved_hvdc_steady_state();
                }
                VSC_HVDC_MODEL* vsc_hvdc_model = get_vsc_hvdc_model();
                if(vsc_hvdc_model!=NULL)
                    vsc_hvdc_model->initialize();
                else
                {
                    osstream<<"VSC_HVDC model is missing for "<<get_compound_device_name();
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                break;
            }
            default:
            {
                VSC_HVDC_MODEL* vsc_hvdc_model = get_vsc_hvdc_model();
                if(vsc_hvdc_model!=NULL)
                    vsc_hvdc_model->run(mode);
                break;
            }
        }
    }
}

VSC_HVDC& VSC_HVDC::operator=(const VSC_HVDC& vsc_hvdc)
{
    if(this == (&vsc_hvdc)) return *this;

    set_toolkit(vsc_hvdc.get_toolkit());
    clear();

    if(vsc_hvdc.get_converter_bus(RECTIFIER)!=0)
        set_converter_bus(RECTIFIER, vsc_hvdc.get_converter_bus(RECTIFIER));

    if(vsc_hvdc.get_converter_bus(INVERTER)!=0)
        set_converter_bus(INVERTER, vsc_hvdc.get_converter_bus(INVERTER));

    set_identifier(vsc_hvdc.get_identifier());
    set_name(vsc_hvdc.get_name());
    set_status(vsc_hvdc.get_status());
    set_line_resistance_in_ohm(vsc_hvdc.get_line_resistance_in_ohm());

    set_converter_dc_operation_mode(RECTIFIER, vsc_hvdc.get_converter_dc_operation_mode(RECTIFIER));
    set_converter_dc_operation_mode(INVERTER, vsc_hvdc.get_converter_dc_operation_mode(INVERTER));

    set_converter_nominal_dc_power_command_in_MW(RECTIFIER, vsc_hvdc.get_converter_nominal_dc_power_command_in_MW(RECTIFIER));
    set_converter_nominal_dc_power_command_in_MW(INVERTER, vsc_hvdc.get_converter_nominal_dc_power_command_in_MW(INVERTER));

    set_converter_nominal_dc_voltage_command_in_kV(RECTIFIER, vsc_hvdc.get_converter_nominal_dc_voltage_command_in_kV(RECTIFIER));
    set_converter_nominal_dc_voltage_command_in_kV(INVERTER, vsc_hvdc.get_converter_nominal_dc_voltage_command_in_kV(INVERTER));

    set_converter_ac_operation_mode(RECTIFIER, vsc_hvdc.get_converter_ac_operation_mode(RECTIFIER));
    set_converter_ac_operation_mode(INVERTER, vsc_hvdc.get_converter_ac_operation_mode(INVERTER));

    set_converter_nominal_ac_voltage_command_in_pu(RECTIFIER, vsc_hvdc.get_converter_nominal_ac_voltage_command_in_pu(RECTIFIER));
    set_converter_nominal_ac_voltage_command_in_pu(INVERTER, vsc_hvdc.get_converter_nominal_ac_voltage_command_in_pu(INVERTER));

    set_converter_nominal_ac_power_factor_command(RECTIFIER, vsc_hvdc.get_converter_nominal_ac_power_factor_command(RECTIFIER));
    set_converter_nominal_ac_power_factor_command(INVERTER, vsc_hvdc.get_converter_nominal_ac_power_factor_command(INVERTER));

    set_converter_loss_factor_A_in_kW(RECTIFIER, vsc_hvdc.get_converter_loss_factor_A_in_kW(RECTIFIER));
    set_converter_loss_factor_A_in_kW(INVERTER, vsc_hvdc.get_converter_loss_factor_A_in_kW(INVERTER));

    set_converter_loss_factor_B_in_kW_per_amp(RECTIFIER, vsc_hvdc.get_converter_loss_factor_B_in_kW_per_amp(RECTIFIER));
    set_converter_loss_factor_B_in_kW_per_amp(INVERTER, vsc_hvdc.get_converter_loss_factor_B_in_kW_per_amp(INVERTER));

    set_converter_minimum_loss_in_kW(RECTIFIER, vsc_hvdc.get_converter_minimum_loss_in_kW(RECTIFIER));
    set_converter_minimum_loss_in_kW(INVERTER, vsc_hvdc.get_converter_minimum_loss_in_kW(INVERTER));

    set_converter_MVA_rating_in_MVA(RECTIFIER, vsc_hvdc.get_converter_MVA_rating_in_MVA(RECTIFIER));
    set_converter_MVA_rating_in_MVA(INVERTER, vsc_hvdc.get_converter_MVA_rating_in_MVA(INVERTER));

    set_converter_current_rating_in_amp(RECTIFIER, vsc_hvdc.get_converter_current_rating_in_amp(RECTIFIER));
    set_converter_current_rating_in_amp(INVERTER, vsc_hvdc.get_converter_current_rating_in_amp(INVERTER));

    set_converter_power_weighting_factor(RECTIFIER, vsc_hvdc.get_converter_power_weighting_factor(RECTIFIER));
    set_converter_power_weighting_factor(INVERTER, vsc_hvdc.get_converter_power_weighting_factor(INVERTER));

    set_converter_Qmax_in_MVar(RECTIFIER, vsc_hvdc.get_converter_Qmax_in_MVar(RECTIFIER));
    set_converter_Qmax_in_MVar(INVERTER, vsc_hvdc.get_converter_Qmax_in_MVar(INVERTER));

    set_converter_Qmin_in_MVar(RECTIFIER, vsc_hvdc.get_converter_Qmin_in_MVar(RECTIFIER));
    set_converter_Qmin_in_MVar(INVERTER, vsc_hvdc.get_converter_Qmin_in_MVar(INVERTER));

    set_converter_remote_bus_to_regulate(RECTIFIER, vsc_hvdc.get_converter_remote_bus_to_regulate(RECTIFIER));
    set_converter_remote_bus_to_regulate(INVERTER, vsc_hvdc.get_converter_remote_bus_to_regulate(INVERTER));

    set_converter_remote_regulation_percent(RECTIFIER, vsc_hvdc.get_converter_remote_regulation_percent(RECTIFIER));
    set_converter_remote_regulation_percent(INVERTER, vsc_hvdc.get_converter_remote_regulation_percent(INVERTER));

    return *this;
}

DEVICE_ID VSC_HVDC::get_device_id() const
{
    DEVICE_ID did;
    did.set_device_type("VSC_HVDC");

    TERMINAL terminal;
    terminal.append_bus(get_converter_bus(RECTIFIER));
    terminal.append_bus(get_converter_bus(INVERTER));
    did.set_device_terminal(terminal);
    did.set_device_identifier(get_identifier());

    return did;
}

void VSC_HVDC::calculate_converter_power_factor_in_ac_voltage_control_mode(CONVERTER_SIDE converter, double Qerror, double Qderivative)
{
    if(get_converter_ac_operation_mode(converter)==VSC_AC_VOLTAGE_CONTROL)
    {
        double smax = get_converter_MVA_rating_in_MVA(converter);
        double imax = get_converter_current_rating_in_amp(converter)*0.001;
        BUS* bus = get_converter_bus_pointer(converter);
        double v = bus->get_positive_sequence_voltage_in_kV();
        double smax2 = SQRT3*imax*v;
        if(smax2<smax) smax = smax2;
    }
}

void VSC_HVDC::solve_steady_state()
{
    if(nominal_dc_setpoints_initialized==false)
        initialize_dc_power_and_voltage_command();
}

void VSC_HVDC::show_solved_hvdc_steady_state() const
{
}


complex<double> VSC_HVDC::get_converter_dynamic_current_in_pu_based_on_system_base_power(CONVERTER_SIDE converter)
{
    if(get_status() == true)
    {
        VSC_HVDC_MODEL* model = get_vsc_hvdc_model();
        if(model!=NULL)
            return 0.0;
        else
            return 0.0;
    }
    else
        return 0.0;
}
