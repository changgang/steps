#include "header/device/dc_component/hlcc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

HLCC::HLCC(STEPS& toolkit)
{
    set_toolkit(toolkit);
    clear();
}

HLCC::~HLCC()
{
    ;
}

void HLCC::set_toolkit(STEPS& toolkit)
{
    this->toolkit =(&toolkit);
}
STEPS& HLCC::get_toolkit() const
{
    return *toolkit;
}


void HLCC::clear()
{

    converter_ac_bus = 0;
    converter_station_number=0;
    converter_dc_bus = 0;
    converter_name = "converter_with_no_data";
    status = true;
    set_operating_mode(LCC_RECTIFIER);

    set_control_mode(LCC_CONSTANT_POWER_MODE);
    command_Pdc_in_MW = 0.0;
    command_Udc_in_kV = 0.0;
    command_Idc_in_kA = 0.0;
    command_firing_angle_in_deg = 0.0;

    bridge_number = 0;
    voltage_drop_per_bridge_in_kV = 0.0;
    smoothing_L_in_H = 0.0;
    smoothing_R_in_ohm = 0.0;

    converter_transformer_grid_side_base_voltage_in_kV = 0.0;
    converter_transformer_converter_side_base_voltage_in_kV = 0.0;
    //converter_transformer_R_in_ohm = 0.0;
    //converter_transformer_X_in_ohm = 0.0;
    converter_transformer_Z_in_ohm = complex<double>(0.0,0.0);

    converter_transformer_max_tap_in_pu = 0.0;
    converter_transformer_min_tap_in_pu = 0.0;
    converter_transformer_number_of_taps =0;
    max_firing_angle_in_deg = 0.0;
    min_firing_angle_in_deg = 0.0;

    converter_transformer_tap_in_pu = 0.0;
    firing_angle_in_deg = 0.0;
    cos_firing_angle = 0.0;

    idc_in_kA = 0.0;
    udc_in_kV = 0.0;
    pdc_in_MW = 0.0;
    P2ac = 0.0;
    Q2ac = 0.0;

    Sbase_MVA = 100;
    Vbase_kV = 100;
    margin_pu = 0.15;

    converter_firing_angle_fixed = false;
    is_control_mode_changed = false;
    hlcc_model = NULL;

}

bool HLCC::is_connected_to_bus(unsigned int bus) const
{
    if(get_converter_ac_bus() == bus)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool HLCC::is_valid() const
{
    if(get_converter_ac_bus()==0)
        return false;
    else
        return true;
}

void HLCC::check()
{
    ;
}

void HLCC::report() const
{
    ;
}

void HLCC::save() const
{
    ;
}

void HLCC::set_model(HLCC_MODEL* model)
{
    if(model != NULL)
    {
        hlcc_model = model;
    }
    else
    {
        ostringstream osstream;
        osstream<<"Waring. None HLCC_CONVERTER model is given to set dynamic model for VSC:"<<get_converter_name();
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

HLCC_MODEL* HLCC::get_model_of_type(string model_type, unsigned int index)
{
    model_type = string2upper(model_type);
    if(model_type=="MT HVDC LCC MODEL")
        return get_hlcc_model();
    return NULL;
}

HLCC_MODEL* HLCC::get_hlcc_model() const
{
    return hlcc_model;
}

void HLCC::set_status(const bool status)
{
    this->status = status;
}

void HLCC::set_converter_ac_bus(const unsigned int bus)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if (psdb.is_bus_exist(bus))
    {
        converter_ac_bus = bus;
        lcc_ac_busptr = psdb.get_bus(bus);
    }
    else
    {
        ostringstream osstream;
        osstream<<bus<<"is not exist for lcc converter:"<<get_converter_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void HLCC::set_converter_station_number(const unsigned int station)
{
    converter_station_number = station;
}

void HLCC::set_converter_dc_bus(const unsigned int bus)
{
    converter_dc_bus = bus;
}

void HLCC::set_converter_name(const string name)
{
    converter_name = name;
}

void HLCC::set_operating_mode(const HLCC_OPERATION_MODE omode)
{
    operating_mode = omode;
}

void HLCC::set_brideg_number(const unsigned int nbridge)
{
   bridge_number = nbridge;
}

void HLCC::set_voltage_drop_per_bridge_in_kV(const double vdrop)
{
    voltage_drop_per_bridge_in_kV = vdrop;
}

void HLCC::set_line_smoothing_L_in_H(const double lsmooth)
{
    smoothing_L_in_H = lsmooth;
}

void HLCC::set_line_smoothing_R_in_ohm(const double rsmooth)
{
    smoothing_R_in_ohm = rsmooth;
}

void HLCC::set_converter_transformer_grid_side_base_voltage_in_kV(const double vbase)
{
    converter_transformer_grid_side_base_voltage_in_kV = vbase;
}

void HLCC::set_converter_transformer_converter_side_base_voltage_in_kV(const double vbase)
{
    converter_transformer_converter_side_base_voltage_in_kV = vbase;
}

void HLCC::set_converter_transformer_R_in_ohm(const double rt)
{
    converter_transformer_Z_in_ohm.real(rt);
}

void HLCC::set_converter_transformer_X_in_ohm(const double xt)
{
    converter_transformer_Z_in_ohm.imag(xt);
}

void HLCC::set_converter_transformer_Z_in_ohm(const complex<double> zt)
{
    converter_transformer_Z_in_ohm = zt;
}

void HLCC::set_converter_transformer_tap_in_pu(const double tap)
{
    converter_transformer_tap_in_pu = tap;
}

void HLCC::set_converter_transformer_max_tap_in_pu(const double tapmax)
{
    converter_transformer_max_tap_in_pu = tapmax;
}

void HLCC::set_converter_transformer_min_tap_in_pu(const double tapmin)
{
    converter_transformer_min_tap_in_pu = tapmin;
}

void HLCC::set_converter_transformer_number_of_taps(const unsigned int ntap)
{
    converter_transformer_number_of_taps =ntap;
}

void HLCC::set_firing_angle_in_deg(const double fangle)
{
    firing_angle_in_deg = fangle;
}

void HLCC::set_cos_firing_angle(const double cosf)
{
    cos_firing_angle = cosf;
    double fangle = steps_acos(cosf);
    set_firing_angle_in_deg(fangle);
}

void HLCC::set_max_firing_angle_in_deg(const double maxangle)
{
    max_firing_angle_in_deg = maxangle;
}

void HLCC::set_min_firing_angle_in_deg(const double minangle)
{
    min_firing_angle_in_deg = minangle;
}

void HLCC::set_udc_in_kV(const double udc)
{
    udc_in_kV = udc;
}

void HLCC::set_idc_in_kA(const double idc)
{
    idc_in_kA = idc;
}

void HLCC::set_pdc_in_MW(const double pdc)
{
    pdc_in_MW = pdc;
}

void HLCC::set_command_udc_in_kV(const double udc)
{
    command_Udc_in_kV = udc;
}

void HLCC::set_command_idc_in_kA(const double idc)
{
    command_Idc_in_kA = idc;
}

void HLCC::set_command_pdc_in_MW(const double pdc)
{
    command_Pdc_in_MW = pdc;
}

void HLCC::set_command_firing_angle(const double fangle)
{
    command_firing_angle_in_deg = fangle;
}

void HLCC::set_converter_P_2_ac_bus(const double Pac)
{
    P2ac = Pac;
}

void HLCC::set_converter_Q_2_ac_bus(const double Qac)
{
    Q2ac = Qac;
}

void HLCC::set_control_mode(const HLCC_CONTROL_MODE mode)
{
    hlcc_control_mode = mode;
}

void HLCC::set_control_mode_change_flag(bool flag)
{
    is_control_mode_changed = flag;
}

void HLCC::set_converter_rated_capacity_in_MVA(double Sbase)
{
    Sbase_MVA = Sbase;
}

void HLCC::set_converter_base_voltage_in_kV(double Vbase)
{
    Vbase_kV = Vbase;
}

void HLCC::set_converter_current_margin_in_pu(double margin)
{
    margin_pu = margin;
}

bool HLCC::get_status() const
{
    return status;
}

unsigned int HLCC::get_converter_ac_bus() const
{
    return converter_ac_bus;
}

unsigned int HLCC::get_converter_station_number() const
{
    return converter_station_number;
}

unsigned int HLCC::get_converter_dc_bus() const
{
    return converter_dc_bus;
}

string HLCC::get_converter_name() const
{
    return converter_name;
}

HLCC_OPERATION_MODE HLCC::get_converter_operating_mode() const
{
    return operating_mode;
    /*
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double udc = get_udc_in_kV();
    double idc = get_idc_in_kA();
    double pdc = udc*idc;
    HLCC_OPERATION_MODE omode;
    string mode_str = "";
    if(pdc>DOUBLE_EPSILON)
    {
        omode = LCC_RECTIFIER;
        mode_str = "RECTIFIER";
    }
    else
    {
        omode = LCC_INVERTER;
        mode_str = "INVERTER";
    }

    if(omode==operating_mode)
        return omode;
    else
    {
        //set_operating_mode(omode);
        //osstream<<"OPERATION MODE OF LCC CONVERTER: "<<get_converter_name()<<" is changed to "<<mode_str;
        //toolkit.show_information_with_leading_time_stamp(osstream);
        return omode;
    }
    */
}


unsigned int HLCC::get_brideg_number() const
{
    return bridge_number;
}

double HLCC::get_voltage_drop_per_bridge_in_kV() const
{
    return voltage_drop_per_bridge_in_kV;
}

double HLCC::get_line_smoothing_L_in_H() const
{
    return smoothing_L_in_H;
}

double HLCC::get_line_smoothing_R_in_ohm() const
{
    return smoothing_R_in_ohm;
}

double HLCC::get_ac_bus_vlotage_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    double vac = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_ac_bus());
    return vac;
}

complex<double> HLCC::get_complex_vac_in_kV() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    complex<double> vac = psdb.get_bus_positive_sequence_complex_voltage_in_kV(get_converter_ac_bus());
    return vac;
}

double HLCC::get_converter_transformer_grid_side_base_voltage_in_kV() const
{
    return converter_transformer_grid_side_base_voltage_in_kV;
}

double HLCC::get_converter_transformer_converter_side_base_voltage_in_kV() const
{
    return converter_transformer_converter_side_base_voltage_in_kV;
}

double HLCC::get_converter_transformer_R_in_ohm() const
{
    complex<double> z = get_converter_transformer_Z_in_ohm();
    return z.real();
}

double HLCC::get_converter_transformer_X_in_ohm() const
{
    complex<double> z = get_converter_transformer_Z_in_ohm();
    return z.imag();
}

complex<double> HLCC::get_converter_transformer_Z_in_ohm() const
{
    return converter_transformer_Z_in_ohm;
}

double HLCC::get_converter_transformer_tap_in_pu() const
{
    return converter_transformer_tap_in_pu;
}

double HLCC::get_converter_transformer_max_tap_in_pu() const
{
    return converter_transformer_max_tap_in_pu;
}

double HLCC::get_converter_transformer_min_tap_in_pu() const
{
    return converter_transformer_min_tap_in_pu;
}

unsigned int HLCC::get_converter_transformer_number_of_taps() const
{
    return converter_transformer_number_of_taps;
}

double HLCC::get_firing_angle_in_deg() const
{
    if(get_status()==true)
        return firing_angle_in_deg;
    else
        return 0.0;
}

double HLCC::get_cos_firing_angle() const
{
    if(get_status()==true)
    {
        double fangle = get_firing_angle_in_deg();
        double cosf = steps_cos(fangle);
        return cosf;
    }
    else
        return 0.0;

}

double HLCC::get_max_firing_angle_in_deg() const
{
    return max_firing_angle_in_deg;
}

double HLCC::get_min_firing_angle_in_deg() const
{
    return min_firing_angle_in_deg;
}

double HLCC::get_udc_in_kV() const
{
    if(get_status()==true)
        return udc_in_kV;
    else
        return 0.0;
}

double HLCC::get_idc_in_kA() const
{
    if(get_status()==true)
        return idc_in_kA;
    else
        return 0.0;
}

double HLCC::get_pdc_in_MW() const
{
    double udc = get_udc_in_kV();
    double idc = get_idc_in_kA();
    return udc*idc;
}

double HLCC::get_command_udc_in_kV() const
{
    if(get_status()==true)
        return command_Udc_in_kV;
    else
        return 0.0;
}

double HLCC::get_command_idc_in_kA() const
{
    if(get_status()==true)
        return command_Idc_in_kA;
    else
        return 0.0;
}

double HLCC::get_command_pdc_in_MW() const
{
    if(get_status()==true)
        return command_Pdc_in_MW;
    else
        return 0.0;
}

double HLCC::get_command_firing_angle() const
{
    return command_firing_angle_in_deg;
}

double HLCC::get_converter_P_2_ac_bus() const
{
    if(get_status()==true)
        return P2ac;
    else
        return 0.0;
}

double HLCC::get_converter_Q_2_ac_bus() const
{
    if(get_status()==true)
        return Q2ac;
    else
        return 0.0;
}

complex<double> HLCC::get_lcc_converter_power_to_ac()
{
    double p = get_converter_P_2_ac_bus();
    double q = get_converter_Q_2_ac_bus();
    q=abs(q);
    return complex<double> (p,q);
}


HLCC_CONTROL_MODE HLCC::get_control_mode() const
{
    return hlcc_control_mode;
}

bool HLCC::get_control_mode_changed_flag()
{
    return is_control_mode_changed;
}

bool HLCC::is_converter_firing_angle_fixed()
{
    return converter_firing_angle_fixed;
}

double HLCC::get_converter_rated_capacity_in_MVA() const
{
    return Sbase_MVA;
}

double HLCC::get_converter_base_voltage_in_kV() const
{
    return Vbase_kV;
}

double HLCC::get_converter_current_margin_in_pu() const
{
    return margin_pu;
}

void HLCC::solve_converter_to_AC(double udc,double idc)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(get_status()==false)
    {
        set_converter_P_2_ac_bus(0.0);
        set_converter_Q_2_ac_bus(0.0);
        return;
    }

    is_control_mode_changed = false;
    HLCC_CONTROL_MODE mode = get_control_mode();
    if(mode == LCC_CONSTANR_FIRING_ANGLE_MODE)
    {
        set_firing_angle_in_deg(get_command_firing_angle());
        solve_lcc_with_tap_and_angle(udc,idc);
    }
    else
    {
        solve_best_transformer_tap_with_min_angle(udc,idc);
        double ang_in_deg = solve_angle_with_fixed_tap(udc,idc);
        //osstream << "the angle solved with best tap is: "<<ang_in_deg;
        //toolkit.show_information_with_leading_time_stamp(osstream);

        double ang_min = get_min_firing_angle_in_deg();
        if(ang_in_deg < ang_min)
        {
            osstream << "Min angle reached of LCC converter:"<<get_converter_name();
            converter_firing_angle_fixed = true;
            set_control_mode(LCC_CONSTANR_FIRING_ANGLE_MODE);
            set_command_firing_angle(ang_min);
            set_control_mode_change_flag(true);
            toolkit.show_information_with_leading_time_stamp(osstream);
            osstream << "The control mode of LCC:"<<get_converter_name()<<" is changed to min firing angle control";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        else
        {
            set_firing_angle_in_deg(ang_in_deg);
            solve_lcc_with_tap_and_angle(udc,idc);
        }
        /*
        double angle = solve_angle_with_fixed_tap(udc,idc);
        if(angle>=min_firing_angle_in_deg)
        {
            solve_lcc_with_tap_and_angle(vdc,idc,tap,angle);
        }
        else
        {
            set_control_mode(LCC_CONSTANR_FIRING_ANGLE_MODE);
            is_control_mode_changed = true;
        }
        */
    }
}

void HLCC::solve_best_transformer_tap_with_min_angle(double vdc, double idc)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;
    //osstream<< "**********************solve best tap with min angle **********************";
    //toolkit.show_information_with_leading_time_stamp(osstream);
    //idc = abs(idc);

    double Vac = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_ac_bus());

    double Vacn_grid = get_converter_transformer_grid_side_base_voltage_in_kV();
    double Vacn_converter = get_converter_transformer_converter_side_base_voltage_in_kV();
    double K = Vacn_grid/Vacn_converter;

    unsigned int N = get_brideg_number();
    double vdrop = get_voltage_drop_per_bridge_in_kV();

    complex<double> Z = get_converter_transformer_Z_in_ohm();
    double R = Z.real();
    double X = Z.imag();

    double angle_min_in_deg = get_min_firing_angle_in_deg();
    double angle_min_in_rad = deg2rad(angle_min_in_deg);
    double cos_angle_min = steps_cos(angle_min_in_rad);

    double tap_max = get_converter_transformer_max_tap_in_pu();
    double tap_min = get_converter_transformer_min_tap_in_pu();
    unsigned int ntap = get_converter_transformer_number_of_taps();

    if(fabs(tap_max-tap_min)<DOUBLE_EPSILON)
    {
        set_converter_transformer_tap_in_pu(tap_min);
        return;
    }

    double Eac_cosangle = vdc/N+THREE_OVER_PI*idc*X+2*idc*R+vdrop;
    Eac_cosangle *= PI_OVER_THREE_SQRT2;

    double tap_desired = Vac*cos_angle_min/K/Eac_cosangle;

    double Tap_step = (tap_max-tap_min)/(ntap-1);
    double Tap_in_pu = tap_min+Tap_step*floor((tap_desired-tap_min)/Tap_step);

    if(Tap_in_pu>tap_max)
    {
        Tap_in_pu = tap_max;
    }
    if(Tap_in_pu<tap_min)
    {
        Tap_in_pu = tap_min;
    }
    set_converter_transformer_tap_in_pu(Tap_in_pu);

    //osstream<< "solved best tap of converter "<<get_converter_name()<<" = "<<tap_desired<<"  normal tap = "<<Tap_in_pu;
    //toolkit.show_information_with_leading_time_stamp(osstream);
}

double HLCC::solve_angle_with_fixed_tap(double vdc, double idc)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    ostringstream osstream;
    //osstream<< "**********************solve angle with tap with udc: "<<vdc<<" idc: "<<idc<<"**********************";
    //toolkit.show_information_with_leading_time_stamp(osstream);
    //idc = abs(idc);

    double Vac = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_ac_bus());

    double Vacn_grid = get_converter_transformer_grid_side_base_voltage_in_kV();
    double Vacn_converter = get_converter_transformer_converter_side_base_voltage_in_kV();
    double tap = get_converter_transformer_tap_in_pu();
    double K = Vacn_grid/Vacn_converter;

    double Eac = Vac/(K*tap);

    unsigned int N = get_brideg_number();
    double vdrop = get_voltage_drop_per_bridge_in_kV();

    complex<double> Z = get_converter_transformer_Z_in_ohm();
    double R = Z.real();
    double X = Z.imag();

    double Eac_cosangle = vdc/N+idc*THREE_OVER_PI*X+2*R*idc+vdrop;

    Eac_cosangle *= PI_OVER_THREE_SQRT2;

    double cosangle = Eac_cosangle/Eac;

    double angle_in_rad = steps_acos(cosangle);
    double angle_in_deg = rad2deg(angle_in_rad);

    //osstream<< "solved angle of converter: "<<get_converter_name()<<" = "<<angle_in_deg<<"  Vac= "<<Vac;
    //toolkit.show_information_with_leading_time_stamp(osstream);

    return angle_in_deg;
}

void HLCC::solve_lcc_with_tap_and_angle(double vdc, double idc)
{
    if(vdc!=0)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        double Vac = psdb.get_bus_positive_sequence_voltage_in_kV(get_converter_ac_bus());

        double tap = get_converter_transformer_tap_in_pu();
        double angle = get_firing_angle_in_deg();

        double Vacn_grid = get_converter_transformer_grid_side_base_voltage_in_kV();
        double Vacn_converter = get_converter_transformer_converter_side_base_voltage_in_kV();
        double K = Vacn_grid/Vacn_converter;
        double Eac = Vac/K/tap;

        unsigned int N =get_brideg_number();
        complex<double> Z = get_converter_transformer_Z_in_ohm();
        double R = Z.real();
        double X = Z.imag();

        double Pdc = vdc*idc;
        double Ploss = 2*idc*idc*R*N;
        HLCC_OPERATION_MODE omode = get_converter_operating_mode();
        double Pac = 0.0;
        Pac = Pdc+Ploss;
        double angle_in_rad = deg2rad(angle);
        double u1 = 0.0;
        double u = 0.0;

        if(omode == LCC_RECTIFIER)
        {
            u1 = steps_cos(angle_in_rad)-SQRT2*idc*X/Eac;
            u = steps_acos(u1)-angle_in_rad;
        }
        else
        {
            u1 = steps_cos(angle_in_rad)+SQRT2*idc*X/Eac;
            u = steps_acos(u1)-angle_in_rad;
        }

        double theta = (2*u+steps_sin(2*angle_in_rad)-steps_sin(2*u+2*angle_in_rad))/(steps_cos(2*angle_in_rad)-steps_cos(2*u+2*angle_in_rad));
        double Qac = abs(Pac*theta);

        set_converter_P_2_ac_bus(Pac);
        set_converter_Q_2_ac_bus(Qac);

        //osstream <<"AFTER SOLVE LCC: : "<<get_converter_name()<<" Pac to ac system is: "<<Pac<<"Qac to ac system: "<<Qac;
        //toolkit.show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        set_converter_P_2_ac_bus(0.0);
        set_converter_Q_2_ac_bus(0.0);
    }
}

double HLCC::solve_inverter_beta_in_rad_with_gamma_in_deg_and_idc_in_kA(double gamma,double idc)
{
    HLCC_OPERATION_MODE omode = get_converter_operating_mode();
    if(omode==LCC_INVERTER)
    {
        double gamma_in_rad = deg2rad(gamma);
        double vac = get_ac_bus_vlotage_in_kV();
        double vbase_grid = get_converter_transformer_grid_side_base_voltage_in_kV();
        double vbase_converter = get_converter_transformer_converter_side_base_voltage_in_kV();
        double k_trans = vbase_grid/vbase_converter;
        double tap = get_converter_transformer_tap_in_pu();
        double Eac = vac/k_trans/tap;
        complex<double> Z = get_converter_transformer_Z_in_ohm();
        double X = Z.imag();

        double cos_beta = steps_cos(gamma_in_rad)+SQRT2*idc*X/Eac;
        double beta_in_rad = steps_acos(cos_beta);

        return beta_in_rad;
    }
    else
    {
        return 0.0;
    }
}

double HLCC::get_converter_dc_power_in_MW()
{
    double udc = get_udc_in_kV();
    double idc = get_idc_in_kA();
    double pdc = udc*idc;
    return pdc;
}

double HLCC::get_lcc_converter_ueq_in_kV() const
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_lcc_converter_ueq_in_kV();
}
double HLCC::get_lcc_converter_req_in_ohm() const
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_lcc_converter_req_ohm();
}

complex<double> HLCC::get_converter_dynamic_ac_current_in_kA()
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_converter_ac_current_in_kA();
}

complex<double> HLCC::get_converter_dynamic_ac_current_in_pu_on_system_base()
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_converter_ac_current_in_pu_on_system_base();
}

double HLCC::get_dynamic_ang_ref_in_deg_of_cc()
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_converter_angle_ref_in_deg_of_cc();
}

double HLCC::get_dynamic_ang_ref_in_deg_of_cea()
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_converter_angle_ref_in_deg_of_cea();
}

double HLCC::get_dynamic_ang_ref_in_deg_of_cv()
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_converter_angle_ref_in_deg_of_cv();
}

double HLCC::get_dynamic_idc_ref_in_kA()
{
    HLCC_MODEL& hlcc_model_ref = *hlcc_model;
    return hlcc_model_ref.get_converter_idc_ref_in_kA();
}
