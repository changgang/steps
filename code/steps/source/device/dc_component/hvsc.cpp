#include "header/device/dc_component/hvsc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

HVSC::HVSC(STEPS& toolkit)
{
    set_toolkit(toolkit);
    clear();
}

HVSC::~HVSC()
{
    ;
}

void HVSC::set_toolkit(STEPS& toolkit)
{
    this->toolkit =(&toolkit);
}
STEPS& HVSC::get_toolkit() const
{
    return *toolkit;
}

bool HVSC::is_connected_to_bus(unsigned int bus) const
{
    if(get_converter_ac_bus()==bus)
    {
        return true;
    }
    else
        return false;
}

bool HVSC::is_valid() const
{
    return true;
}
void HVSC::check()
{
    ;
}
void HVSC::clear()
{
    converter_ac_bus = 0;
    converter_dc_bus = 0;

    name = "";
    status = true;

    d_control_mode = VSC_CONSTANT_AC_ACTIVE_POWER_MODE;
    q_control_mode = VSC_CONSTANT_AC_REACTIVE_POWER_MODE;

    converter_loss_coefficient_A_in_kW =0.0;
    converter_loss_coefficient_B_in_kW_per_amp = 0.0;
    converter_loss_coefficient_C_in_kW_per_amp_squard = 0.0;
    min_converter_loss_in_kW = 0.0;

    converter_rated_capacity_in_MVA = 0.0;
    converter_base_voltage_in_kV = 0.0;
    converter_rated_current_in_amp = 0.0;
    converter_commutating_impedance = complex<double>(0,0);
    P_to_AC_bus_MW = 0.0;
    Q_to_AC_bus_MVar = 0.0;
    Ploss_MW = 0.0;
    Udc_in_kV = 0.0;
    Idc_in_kA = 0.0;

    Pmax_MW = 0.0;
    Pmin_MW = 0.0;
    Qmax_MVar = 0.0;
    Qmin_MVar = 0.0;
    Udmax_kV = 0.0;
    Udmin_kV = 0.0;


    Pac_command_in_MW = 0.0;
    Udc_command_in_kV = 0.0;
    Qac_command_in_Mvar = 0.0;
    Us_command_in_kV = 0.0;
    Pdc_command = 0.0;

    hvsc_model = NULL;
}

void HVSC::report() const
{
    ;
}

void HVSC::save() const
{
    ;
}

DEVICE_ID HVSC::get_device_id() const
{
    DEVICE_ID did;
    return did;
}

void HVSC::set_model(HVSC_MODEL* model)
{
    if(model != NULL)
    {
        this->hvsc_model = model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Waring. None HVSC_CONVERTER model is given to set dynamic model for VSC:"<<get_converter_name();
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

double HVSC::get_converter_pdc_in_MW()
{
    double udc = get_udc_in_kV();
    double idc = get_idc_in_kA();
    double pdc = udc*idc;
    return pdc;
}

HVSC_MODEL* HVSC::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="MT HVDC VSC MODEL")
        return get_hvsc_model();
    return NULL;
}

HVSC_MODEL* HVSC::get_hvsc_model()
{
    return hvsc_model;
}

void HVSC::set_converter_ac_bus(const unsigned int acbus)
{
    this->converter_ac_bus = acbus;
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    converter_busptr = psdb.get_bus(acbus);
}

void HVSC::set_converter_station_number(const unsigned int station)
{
    this->converter_station_number = station;
}

void HVSC::set_converter_dc_bus(const unsigned int dcbus)
{
    this->converter_dc_bus = dcbus;
}
void HVSC::set_converter_name(const string name)
{
    this->name = name;
}

void HVSC::set_converter_status(const bool status)
{
    this->status = status;
}

void HVSC::set_priority(const unsigned int priority)
{
    this->priority = priority;
}

void HVSC::set_converter_d_axis_control_mode(const HVSC_D_CONTROL_MODE dmode)
{
    this->d_control_mode = dmode;
}

void HVSC::set_converter_q_axis_control_mode(const HVSC_Q_CONTROL_MODE qmode)
{
    this->q_control_mode = qmode;
}

void HVSC::set_converter_loss_coefficient_A_in_kW(const double a)
{
    this->converter_loss_coefficient_A_in_kW = a;
}

void HVSC::set_converter_loss_coefficient_B_in_kW_per_amp(const double b)
{
    this->converter_loss_coefficient_B_in_kW_per_amp = b;
}

void HVSC::set_converter_loss_coefficient_C_in_kW_per_amp_squard(const double c)
{
    this->converter_loss_coefficient_C_in_kW_per_amp_squard = c;
}

void HVSC::set_converter_min_loss_in_kW(const double min_loss)
{
    this->min_converter_loss_in_kW = min_loss;
}
void HVSC::set_converter_commutating_impedance(const complex<double> Z)
{
    this->converter_commutating_impedance = Z;
}

void HVSC::set_converter_commutating_impedance_RC(const double R)
{
    this->converter_commutating_impedance.real(R);
}

void HVSC::set_converter_commutating_impedance_XC(const double X)
{
    this->converter_commutating_impedance.imag(X);
}

void HVSC::set_udc_in_kV(const double udc)
{
    this->Udc_in_kV = udc;
}

void HVSC::set_idc_in_kA(const double idc)
{
    this->Idc_in_kA = idc;
}

void HVSC::set_converter_P_to_AC_bus_MW(const double P2AC)
{
    this->P_to_AC_bus_MW = P2AC;
}
void HVSC::set_converter_Q_to_AC_bus_Mvar(const double Q2AC)
{
    this->Q_to_AC_bus_MVar = Q2AC;
}

void HVSC::set_converter_P_to_DC_bus_MW(const double P2DC)
{
    this->P_to_DC_bus_MW = P2DC;
}

void HVSC::set_converter_Ploss_MW(const double Ploss)
{
    this->Ploss_MW = Ploss;
}

void HVSC::set_converter_Pac_command(const double pac)
{
    this->Pac_command_in_MW = pac;
}
void HVSC::set_converter_Udc_command(const double udc)
{
    this->Udc_command_in_kV = udc;
}

void HVSC::set_converter_Qac_command(const double qac)
{
    this->Qac_command_in_Mvar = qac;
}

void HVSC::set_converter_Us_command(const double us)
{
    this->Us_command_in_kV = us;
}

void HVSC::set_converter_rated_capacity_in_MVA(const double Sbase)
{
    this->converter_rated_capacity_in_MVA = Sbase;
}

void HVSC::set_converter_base_voltage_in_kV(const double Vbase)
{
    converter_base_voltage_in_kV = Vbase;
}

void HVSC::set_max_iteration_of_voltage_station_solution(const unsigned int n)
{
    this->max_iteration_of_voltage_station_solution = n;
}

void HVSC::set_allowed_max_p_in_MW_of_voltage_station_solution(const double p)
{
    this->allowed_max_p_in_MW_of_voltage_station_solution = p;
}
//

 unsigned int HVSC::get_converter_ac_bus() const
 {
     return converter_ac_bus;
 }

 BUS* HVSC::get_converter_ac_bus_pointer() const
 {
     return converter_busptr;
 }

unsigned int HVSC::get_converter_station_number() const
{
    return converter_station_number;
}

unsigned int HVSC::get_converter_dc_bus() const
{
    return converter_dc_bus;
}

string HVSC::get_converter_name() const
{
    return name;
}
bool HVSC::get_status() const
{
    return status;
}

unsigned int HVSC::get_priority() const
{
    return priority;
}

HVSC_D_CONTROL_MODE HVSC::get_converter_d_axis_control_mode() const
{
    return d_control_mode;
}

HVSC_Q_CONTROL_MODE HVSC::get_converter_q_axis_control_mode() const
{
    return q_control_mode;
}

double HVSC::get_converter_loss_coefficient_A_in_kW() const
{
    return converter_loss_coefficient_A_in_kW;
}

double HVSC::get_converter_loss_coefficient_B_in_kW_per_amp() const
{
    return converter_loss_coefficient_B_in_kW_per_amp;
}

double HVSC::get_converter_loss_coefficient_C_in_kW_per_amp_squard() const
{
    return converter_loss_coefficient_C_in_kW_per_amp_squard;
}

double HVSC::get_converter_min_loss_in_kW() const
{
    return min_converter_loss_in_kW;
}

complex<double> HVSC::get_converter_commutating_impedance() const
{
    return converter_commutating_impedance;
}

double HVSC::get_converter_commutating_impedance_RC() const
{
    complex<double> Z = get_converter_commutating_impedance();
    return Z.real();
}

double HVSC::get_converter_commutating_impedance_XC() const
{
    complex<double> Z = get_converter_commutating_impedance();
    return Z.imag();
}

double HVSC::get_converter_P_to_AC_bus_MW() const
{
    return P_to_AC_bus_MW;
}

double HVSC::get_converter_Q_to_AC_bus_Mvar() const
{
    return Q_to_AC_bus_MVar;
}

double HVSC::get_converter_P_to_DC_bus_MW() const
{
    return P_to_DC_bus_MW;
}

double HVSC::get_converter_Ploss_MW()
{
    return Ploss_MW;
}

complex<double> HVSC::get_converter_AC_bus_compelx_voltage_in_kV()const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    complex<double> Us = psdb.get_bus_positive_sequence_complex_voltage_in_kV(get_converter_ac_bus());
    return Us;
}
double HVSC::get_converter_AC_bus_voltage_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double Us = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_ac_bus());
    return Us;
}

double HVSC::get_converter_AC_bus_voltage_in_pu() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double Us = psdb.get_bus_positive_sequence_voltage_in_pu(get_converter_ac_bus());
    return Us;
}

double  HVSC::get_udc_in_kV() const
{
    return Udc_in_kV;
}

double  HVSC::get_idc_in_kA() const
{
    return Idc_in_kA;
}

double HVSC::get_converter_Pac_command() const
{
    return Pac_command_in_MW;
}

double HVSC::get_converter_Udc_command() const
{
    return Udc_command_in_kV;
}
double HVSC::get_converter_Qac_command() const
{
    return Qac_command_in_Mvar;
}

double HVSC::get_converter_Us_command() const
{
    return Us_command_in_kV;
}

double HVSC::get_converter_nominal_ac_voltage_to_regulate() const
{
    double us = get_converter_Us_command();
    BUS* bus = get_converter_ac_bus_pointer();
    double vbase = bus->get_base_voltage_in_kV();
    return us/vbase;
}

double HVSC::get_converter_rated_capacity_in_MVA() const
{
    return converter_rated_capacity_in_MVA;
}

double HVSC::get_converter_base_voltage_in_kV() const
{
    return converter_base_voltage_in_kV;
}

unsigned int HVSC::get_max_iteration_of_voltage_station_solution() const
{
    return max_iteration_of_voltage_station_solution;
}

double HVSC::get_allowed_max_p_in_MW_of_voltage_station_solution() const
{
    return allowed_max_p_in_MW_of_voltage_station_solution;
}


void HVSC::initialize_power_stations()
{
    //STEPS& toolkit = get_toolkit();
    //POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    HVSC_D_CONTROL_MODE dmode = get_converter_d_axis_control_mode();
    HVSC_Q_CONTROL_MODE qmode = get_converter_q_axis_control_mode();

    if(dmode==VSC_CONSTANT_AC_ACTIVE_POWER_MODE)
    {
        if(qmode==VSC_CONSTANT_AC_REACTIVE_POWER_MODE)   //PQ
        {
           set_converter_P_to_AC_bus_MW(get_converter_Pac_command());
           set_converter_Q_to_AC_bus_Mvar(get_converter_Qac_command());
        }
        else  //PV
        {
            set_converter_P_to_AC_bus_MW(get_converter_Pac_command());
            set_converter_Q_to_AC_bus_Mvar(get_converter_Q_to_AC_bus_Mvar());

        }
    }

    else if(dmode==VSC_CONSTANT_DC_VOLTAGE_MODE)
    {
        if(qmode==VSC_CONSTANT_AC_REACTIVE_POWER_MODE)   //Q
        {
           set_converter_P_to_AC_bus_MW(get_converter_P_to_AC_bus_MW());
           set_converter_Q_to_AC_bus_Mvar(get_converter_Qac_command());
        }
        else  //V
        {
            set_converter_P_to_AC_bus_MW(get_converter_P_to_AC_bus_MW());
            set_converter_Q_to_AC_bus_Mvar(get_converter_Q_to_AC_bus_Mvar());
        }
    }

    else
    {
        set_converter_P_to_AC_bus_MW(get_converter_Pac_command());
        set_converter_Q_to_AC_bus_Mvar(get_converter_Qac_command());;
    }
}

void HVSC::update_converter_ac_variables()
{
    //STEPS& toolkit = get_toolkit();
    //POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVSC_D_CONTROL_MODE dmode = get_converter_d_axis_control_mode();
    HVSC_Q_CONTROL_MODE qmode = get_converter_q_axis_control_mode();
    if(dmode==VSC_CONSTANT_AC_ACTIVE_POWER_MODE)
    {
        if(qmode==VSC_CONSTANT_AC_REACTIVE_POWER_MODE)
        {
            ;//no change
        }
        else
        {
            ;//update in power flow solver
        }
    }
}

double HVSC::solve_dc_power_of_p_control(double P, double Q)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    //HVSC_D_CONTROL_MODE dmode = get_converter_d_axis_control_mode();
    double Pdc = 0.0;
    complex<double> Vac = psdb.get_bus_positive_sequence_complex_voltage_in_kV(get_converter_ac_bus());
    //complex<double> Vac = complex<double> (100,0);
    //double P = get_converter_P_to_AC_bus_MW();
    //double Q = get_converter_Q_to_AC_bus_Mvar();
    complex<double> S(P,Q);
    complex<double> It = conj(S/Vac/SQRT3);

    complex<double> Z = get_converter_commutating_impedance();
    complex<double> Uc = Vac - It*Z*SQRT3;

    double Pc = (SQRT3*Uc*conj(It)).real();

    double a = get_converter_loss_coefficient_A_in_kW();
    double b = get_converter_loss_coefficient_B_in_kW_per_amp();
    double c = get_converter_loss_coefficient_C_in_kW_per_amp_squard();

    double I = abs(It)*1000;

    double ploss = (a+b*I+c*I*I)/1000;
    double ploss_min = get_converter_min_loss_in_kW()/1000;

    if(ploss<ploss_min)
    {
        ploss = ploss_min;
    }

    Pdc = Pc-ploss;

    set_converter_Ploss_MW(ploss);
    set_converter_P_to_DC_bus_MW(Pdc);

    //osstream<<" Converter Power loss of  "<< get_converter_name()<<" is: "<<ploss;
    //toolkit.show_information_with_leading_time_stamp(osstream);
    return Pdc;
}

double HVSC::solve_ac_active_power_of_udc_control(double Pdc)
{
    double Pac = 0.0;
    ostringstream osstream;
    //osstream<<"Pdc of voltage station is : "<<Pdc;
    //show_information_with_leading_time_stamp_with_default_toolkit(osstream);

    if(Pdc>0)
    {
        Pac = solve_Pac_with_positive_Pdc(Pdc);
    }
    else
    {
        Pac = solve_Pac_with_negetaive_Pdc(Pdc);
    }
    return Pac;
}

double HVSC::solve_Pac_with_negetaive_Pdc(double Pdc)
{
    //STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double Pdc0 = Pdc;
    double Pac_high = 0.95*Pdc0;
    double Pac_low = 1.05*Pdc0;

    double Qac = get_converter_Q_to_AC_bus_Mvar();
    //osstream<<"Qac of voltage converter "<< get_converter_name()<<" is: "<<Qac;
    //toolkit.show_information_with_leading_time_stamp(osstream);

    double Pdc_high = solve_dc_power_of_p_control(Pac_high,Qac);
    double Pdc_low = solve_dc_power_of_p_control(Pac_low,Qac);

    //unsigned int n = get_max_iteration_of_voltage_station_solution();
    //double P_mis = get_allowed_max_p_in_MW_of_voltage_station_solution();
    unsigned int n = 10;
    double P_mis = 0.0001;

    for(unsigned int i=0;i!=n;i++)
    {
        ostringstream osstream;
        //osstream<<"Pac high is: "<<Pac_high<<" Pac low is: "<<Pac_low;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);

        double slope = (Pdc_high-Pdc_low)/(Pac_high-Pac_low);
        double Pac_new = Pac_low+(Pdc0-Pdc_low)/slope;
        double Pdc_new = solve_dc_power_of_p_control(Pac_new,Qac);

        //osstream<<"Pac new is: "<<Pac_new;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        //osstream<<"Pdc new is: "<<Pdc_new;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        if(Pdc_new>Pdc0)
        {
            Pac_high = Pac_new;
            Pdc_high = Pdc_new;
        }
        else
        {
            Pac_low = Pac_new;
            Pdc_low = Pdc_new;
        }

        if(fabs(Pdc_high-Pdc0)<P_mis)
        {
            //osstream<<"Power to AC of voltage station of "<< get_converter_name()<<" is: "<<Pac_high;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            set_converter_P_to_AC_bus_MW(Pac_high);
            return Pac_high;
        }
        if(fabs(Pdc_low-Pdc0)<P_mis)
        {
            //osstream<<"Power to AC of voltage station of "<< get_converter_name()<<" is: "<<Pac_low;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            set_converter_P_to_AC_bus_MW(Pac_low);
            return Pac_low;
        }

    }
    double P_ustation = 0.5*(Pac_high+Pac_low);
    set_converter_P_to_AC_bus_MW(P_ustation);
    //osstream<<"Power to AC of voltage station of "<< get_converter_name()<<" is: "<<P_ustation;
    //toolkit.show_information_with_leading_time_stamp(osstream);

    return P_ustation;
}

double HVSC::solve_Pac_with_positive_Pdc(double Pdc)
{
    //STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double Pdc0 = Pdc;
    double Pac_high = 1.05*Pdc0;
    double Pac_low = 0.95*Pdc0;

    double Qac = get_converter_Q_to_AC_bus_Mvar();
    //osstream<<"Qac of voltage converter "<< get_converter_name()<<" is: "<<Qac;
    //toolkit.show_information_with_leading_time_stamp(osstream);

    double Pdc_high = solve_dc_power_of_p_control(Pac_high,Qac);
    double Pdc_low = solve_dc_power_of_p_control(Pac_low,Qac);



    //unsigned int n = get_max_iteration_of_voltage_station_solution();
    //double P_mis = get_allowed_max_p_in_MW_of_voltage_station_solution();
    unsigned int n = get_max_iteration_of_voltage_station_solution();
    double P_mis = get_max_iteration_of_voltage_station_solution();

    for(unsigned int i=0;i!=n;i++)
    {
        ostringstream osstream;
        //osstream<<"Pac high is: "<<Pac_high<<" Pac low is: "<<Pac_low;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);

        double slope = (Pdc_high-Pdc_low)/(Pac_high-Pac_low);
        double Pac_new = Pac_low+(Pdc0-Pdc_low)/slope;
        double Pdc_new = solve_dc_power_of_p_control(Pac_new,Qac);

        //osstream<<"Pac new is: "<<Pac_new;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        //osstream<<"Pdc new is: "<<Pdc_new;
        //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        if(fabs(Pdc_low-Pdc0)<fabs(Pdc_high-Pdc0))
        {
            Pac_high = Pac_new;
            Pdc_high = Pdc_new;
        }
        else
        {
            Pac_low = Pac_new;
            Pdc_low = Pdc_new;
        }

        if(fabs(Pdc_high-Pdc0)<P_mis)
        {
            //osstream<<"Power to AC of voltage station of "<< get_converter_name()<<" is: "<<Pac_high;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            set_converter_P_to_AC_bus_MW(Pac_high);
            return Pac_high;
        }
        if(fabs(Pdc_low-Pdc0)<P_mis)
        {
            //osstream<<"Power to AC of voltage station of "<< get_converter_name()<<" is: "<<Pac_low;
            //toolkit.show_information_with_leading_time_stamp(osstream);
            set_converter_P_to_AC_bus_MW(Pac_low);
            return Pac_low;
        }

    }
    double P_ustation = 0.5*(Pac_high+Pac_low);
    set_converter_P_to_AC_bus_MW(P_ustation);
    //osstream<<"Power to AC of voltage station of "<< get_converter_name()<<" is: "<<P_ustation;
    //toolkit.show_information_with_leading_time_stamp(osstream);
    return P_ustation;
}


complex<double> HVSC::get_converter_initial_power_of_converter_AC_side()
{
    double P = get_converter_P_to_AC_bus_MW();
    double Q = get_converter_Q_to_AC_bus_Mvar();
    complex<double> S_in_MVA = complex<double>(P,Q);
    return S_in_MVA;
}

complex<double> HVSC::get_converter_initial_ac_current_in_kA_in_xy_axis()
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(get_converter_ac_bus());
    complex<double>  S = get_converter_initial_power_of_converter_AC_side();
    complex<double> Vac = psdb.get_bus_positive_sequence_complex_voltage_in_kV(get_converter_ac_bus());
    complex<double> It = conj(S/Vac/SQRT3);

    //osstream<<"VSC:"<<get_converter_name()<<" initialize AC CURRENT: "<<It.real()<<" +j"<<It.imag()<<" kA";
    //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    return It;
}

complex<double> HVSC::get_converter_initial_ac_current_in_pu_on_system_base_in_xy_axis()
{
    complex<double> It = get_converter_initial_ac_current_in_kA_in_xy_axis();
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(get_converter_ac_bus());

    double Sbase = psdb.get_system_base_power_in_MVA();
    double Vbase = bus->get_base_voltage_in_kV();
    double Ibase = Sbase/Vbase/SQRT3;
    complex<double> It_in_pu = It/Ibase;
    return It_in_pu;
}

complex<double> HVSC::get_converter_initial_ac_current_in_pu_on_converter_base_in_xy_axis()
{
    ostringstream osstream;
    complex<double> It = get_converter_initial_ac_current_in_kA_in_xy_axis();
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(get_converter_ac_bus());

    double Sbase = get_converter_rated_capacity_in_MVA();
    double Vbase = bus->get_base_voltage_in_kV();
    double Ibase = Sbase/Vbase/SQRT3;
    complex<double> It_in_pu = It/Ibase;

    //osstream<<"VSC:"<<get_converter_name()<<" initialize AC CURRENT on converter base: "<<It_in_pu.real()<<" +j"<<It_in_pu.imag()<<" pu"<<endl
           // <<"SBASE = "<<Sbase<<" VBASE = "<<Vbase;
    //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    return It_in_pu;
}

complex<double> HVSC::get_converter_initial_ac_current_in_pu_on_system_base_in_dq_axis()
{
    complex<double> Ixy_in_pu = get_converter_initial_ac_current_in_pu_on_system_base_in_xy_axis();
    complex<double> Idq_in_pu = complex<double>(0.0,0.0);
    double pll_angle_in_rad = get_initial_pll_angle_in_rad();
    Idq_in_pu = ab2dq_with_angle_in_rad(Ixy_in_pu,pll_angle_in_rad);
    return Idq_in_pu;
}

complex<double> HVSC::get_converter_initial_ac_current_in_pu_on_converter_base_in_dq_axis()
{
    ostringstream osstream;
    complex<double> Ixy_in_pu = get_converter_initial_ac_current_in_pu_on_converter_base_in_xy_axis();
    complex<double> Idq_in_pu = complex<double>(0.0,0.0);
    double pll_angle_in_rad = get_initial_pll_angle_in_rad();
    Idq_in_pu = ab2dq_with_angle_in_rad(Ixy_in_pu,pll_angle_in_rad);


     // osstream<<"VSC:"<<get_converter_name()<<" initialize AC CURRENT on converter base on dq: "<<Idq_in_pu.real()<<" +j"<<Idq_in_pu.imag()<<" pu";
    //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    return Idq_in_pu;
}

double HVSC::get_initial_pll_angle_in_rad()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* bus = psdb.get_bus(get_converter_ac_bus());
    double angle_in_rad = bus->get_positive_sequence_angle_in_rad();
    return angle_in_rad;
}

double HVSC::get_initial_pll_angle_in_deg()
{
    double angle_in_rad = get_initial_pll_angle_in_rad();
    double angle_in_deg = rad2deg(angle_in_rad);
    return angle_in_deg;
}

double HVSC::get_vsc_converter_ueq_in_kV() const
{
    return hvsc_model->get_vsc_converter_ueq_in_kV();
}
double HVSC::get_vsc_converter_req_in_ohm() const
{
    return hvsc_model->get_vsc_converter_ueq_in_kV();
}

void HVSC::set_converter_Pdc_command_in_MW(double Pdc)
{
    Pdc_command = Pdc;
}

double HVSC::get_converter_Pdc_command_in_MW()
{
    return Pdc_command;
}

complex<double> HVSC::get_converter_dynamic_ac_current_in_kA()
{
    return hvsc_model->get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA();
}

complex<double> HVSC::get_converter_dynamic_ac_current_in_pu_on_system_base()
{
    return hvsc_model->get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_system_base();
}

complex<double> HVSC::get_converter_dynamic_complex_power_in_MVA()
{
    return hvsc_model->get_converter_dynamic_ac_complex_power();
}

double HVSC::get_converter_dynamic_active_power_in_MW()
{
    return hvsc_model->get_converter_dynamic_active_power_in_MW();
}

double HVSC::get_converter_dynamic_reactive_power_in_Mvar()
{
    return hvsc_model->get_converter_dynamic_reactive_power_in_Mvar();
}

double HVSC::get_converter_dynamic_d_axis_current_in_pu()
{
    complex<double> idq = hvsc_model->get_converter_dynamic_current_from_ac_bus_to_converter_in_dq_axis_in_pu_on_converter_base();
    return idq.real();
}
double HVSC::get_converter_dynamic_q_axis_current_in_pu()
{
    complex<double> idq = hvsc_model->get_converter_dynamic_current_from_ac_bus_to_converter_in_dq_axis_in_pu_on_converter_base();
    return idq.imag();
}
