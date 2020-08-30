#include "header/device/lcc.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

LCC::LCC(STEPS& toolkit) : CONVERTER(toolkit)
{
    this->clear();
}

LCC::~LCC() { ; }

void LCC::clear()
{
    CONVERTER::clear();

    set_number_of_bridge(2);
    set_voltage_drop_per_bridge_in_kV(0.0);
    set_max_angle_in_deg(90.0);
    set_min_angle_in_deg(0.0);

    set_angle_in_deg(0.0);

    set_angle_fixed_flag(false);
}

void LCC::set_number_of_bridge(const unsigned int n) { bridge_number = n; }
void LCC::set_voltage_drop_per_bridge_in_kV(const double V) { voltage_drop_per_bridge_in_kV = V; }
void LCC::set_max_angle_in_deg(const double angle) { max_angle_in_deg = angle; cos_max_angle = cos(deg2rad(angle)); }
void LCC::set_min_angle_in_deg(const double angle) { min_angle_in_deg = angle; cos_min_angle = cos(deg2rad(angle)); }
void LCC::set_angle_fixed_flag(bool flag) { angle_fixed = flag; }

unsigned int LCC::get_number_of_bridge() const { return bridge_number; }
double LCC::get_voltage_drop_per_bridge_in_kV() const { return voltage_drop_per_bridge_in_kV; }
double LCC::get_max_angle_in_deg() const { return max_angle_in_deg; }
double LCC::get_min_angle_in_deg() const { return min_angle_in_deg; }
double LCC::get_cos_max_angle() const { return cos_max_angle; }
double LCC::get_cos_min_angle() const { return cos_min_angle; }
bool LCC::is_angle_fixed() const { return angle_fixed; }
double LCC::get_angle_in_deg() const { return angle_in_deg; }
double LCC::get_cos_angle() const { return cos_angle; }

void LCC::set_angle_in_deg(double angle, double cos_angle)
{
    if(angle>=0.0) angle_in_deg = angle;
    else angle_in_deg = 0.0;
    if(cos_angle!=0.0) this->cos_angle = cos_angle;
    else this->cos_angle = steps_cos(deg2rad(angle_in_deg));
}

LCC& LCC::operator=(const LCC& converter)
{
    if(this == (&converter)) return *this;
    clear();

    CONVERTER::copy_from_const_converter(converter);

    set_number_of_bridge(converter.get_number_of_bridge());
    set_voltage_drop_per_bridge_in_kV(converter.get_voltage_drop_per_bridge_in_kV());
    set_max_angle_in_deg(converter.get_max_angle_in_deg());
    set_min_angle_in_deg(converter.get_min_angle_in_deg());

    set_angle_fixed_flag(converter.is_angle_fixed());

    return *this;
}

void LCC::solve_with_desired_dc_voltage_and_current(double Vdc, double Idc)
{
    double Vdcmax = get_maximum_avaiable_dc_voltage_in_kV();
    if(Vdc<=Vdcmax and Vdc>Vdcmax*0.5)
    {
        solve_transformer_tap_and_angle(Vdc, Idc);
        set_dc_voltage_in_kV(get_actual_dc_voltage_in_kV_with_dc_current(Idc));
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        if(Vdc>Vdcmax)
        {
            osstream<<"Error. The give DC voltage "<<Vdc<<"kV exceeds maximum available DC voltage "<<Vdcmax<<"kV"<<endl
                    <<"No solution is attempted for LCC "<<get_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            osstream<<"Error. The give DC voltage "<<Vdc<<"kV is less than 50% of maximum available DC voltage "<<Vdcmax<<"kV"<<endl
                    <<"No solution is attempted for LCC "<<get_name();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

double LCC::get_maximum_avaiable_dc_voltage_in_kV()
{
    double tap = get_transformer_tap_in_pu();
    double angle = get_angle_in_deg();
    double cos_angle = get_cos_angle();

    set_transformer_tap_in_pu(get_transformer_min_tap_in_pu());
    set_angle_in_deg(get_min_angle_in_deg(), get_cos_min_angle());
    double Vdcmax = get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle();

    set_transformer_tap_in_pu(tap);
    set_angle_in_deg(angle, cos_angle);

    return Vdcmax;
}

void LCC::solve_transformer_tap_and_angle(double Vdc, double Idc)
{
    solve_best_transformer_tap_with_min_angle(Vdc, Idc);
    double cosAngle = get_desired_cosAngle_with_desired_dc_voltage_current(Vdc, Idc);
    if(cosAngle>cos_min_angle) // minimum angle is reached
        set_angle_in_deg(get_min_angle_in_deg(), cos_min_angle);
    else
        set_angle_in_deg(rad2deg(steps_acos(cosAngle)), cosAngle);
}

void LCC::solve_best_transformer_tap_with_min_angle(double Vdc, double Idc)
{
    double tap_min = get_transformer_min_tap_in_pu();

    if(is_transformer_tap_fixed())
    {
        set_transformer_tap_in_pu(tap_min);
    }
    else
    {
        double tap_max = get_transformer_max_tap_in_pu();

        double Vdrop = get_voltage_drop_per_bridge_in_kV();
        double TurnRatio = get_transformer_grid2valve_turn_ratio();
        double TapStep = get_transformer_tap_step_in_pu();
        unsigned int N = get_number_of_bridge();
        complex<double> Z = get_transformer_impedance_in_ohm();

        double Vbus = get_bus_positive_sequence_voltage_in_kV();
        double Eac_cosAngle = Vdc/N+THREE_OVER_PI*Z.imag()*Idc+2.0*Z.real()*Idc+Vdrop;
        Eac_cosAngle *= PI_OVER_THREE_SQRT2;
        double Eac = Eac_cosAngle/cos_min_angle;
        double Tap = Vbus/(Eac*TurnRatio); // desired
        Tap = tap_min +  TapStep*floor((Tap-tap_min)/TapStep); // actual
        if(Tap < tap_min) Tap = tap_min; // apply limit
        else
            if(Tap > tap_max) Tap = tap_max;

        set_transformer_tap_in_pu(Tap);
    }
}

double LCC::get_desired_cosAngle_with_desired_dc_voltage_current(double Vdc, double Idc)
{
    double Tap = get_transformer_tap_in_pu();
    double Vdrop = get_voltage_drop_per_bridge_in_kV();
    double TurnRatio = get_transformer_grid2valve_turn_ratio();
    complex<double> Z = get_transformer_impedance_in_ohm();
    unsigned int N = get_number_of_bridge();

    double Vbus = get_bus_positive_sequence_voltage_in_kV();
    double Eac = Vbus/(TurnRatio*Tap); // actual Eac at valve side
    double Eac_cosAngle = Vdc/N+THREE_OVER_PI*Z.imag()*Idc+2.0*Z.real()*Idc+Vdrop;
           Eac_cosAngle *= PI_OVER_THREE_SQRT2;
    double cosAngle = Eac_cosAngle/Eac; // desired cosine alpha
    return cosAngle;
}

void LCC::show_steady_state()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    double sbase = toolkit.get_system_base_power_in_MVA();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();


    osstream<<"Converter at bus "<<get_bus();
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Vdc = "<<get_dc_voltage_in_kV()<<" kV, "
            <<"Vac = "<<get_bus_positive_sequence_voltage_in_kV()<<" kV";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Transformer tap = "<<get_transformer_tap_in_pu()<<" pu.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"alpha/gamma = "<<get_angle_in_deg()<<" deg, "
            <<"mu = "<<get_commutating_overlap_angle_in_deg()<<" deg";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"DC current = "<<get_dc_current_in_kA()<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"AC current = "<<get_ac_current_in_kA()<<" kA.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    double P = get_ac_active_power_in_MW();
    double Q = get_ac_reactive_power_in_MVar();
    complex<double> S(P,Q);
    complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
    S *=  one_over_sbase;
    complex<double> I = conj(S/V);
    I *= (sbase/(SQRT3*get_bus_base_voltage_in_kV()));

    osstream<<"Pdc = "<<get_dc_power_in_MW()<<" MW, "
            <<"Pac = "<<get_ac_active_power_in_MW()<<" MW, "
            <<"Qac = "<<get_ac_reactive_power_in_MVar()<<" MVar, "
            <<"Eta = "<<get_ac_power_factor()<<", "
            <<"Iac = "<<get_ac_current_in_kA()<<"kA, or "
            <<I<<"KA and "<<abs(I)<<"kA";
    toolkit.show_information_with_leading_time_stamp(osstream);
}
double LCC::get_actual_dc_voltage_in_kV_with_dc_current(double Idc)
{
    double Vdrop = get_voltage_drop_per_bridge_in_kV();
    double TurnRatio = get_transformer_grid2valve_turn_ratio();
    complex<double> Z = get_transformer_impedance_in_ohm();
    unsigned int N = get_number_of_bridge();
    double Tap = get_transformer_tap_in_pu();

    double Vbus = get_bus_positive_sequence_voltage_in_kV();
    double Eac = Vbus/(TurnRatio*Tap);
    double Vdc = N*(THREE_SQRT2_OVER_PI*Eac*cos_angle-THREE_OVER_PI*Z.imag()*Idc-2.0*Z.real()*Idc-Vdrop);
    return Vdc;
}


double LCC::get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle() const
{
    double Vdrop = get_voltage_drop_per_bridge_in_kV();
    double TurnRatio = get_transformer_grid2valve_turn_ratio();
    unsigned int N = get_number_of_bridge();
    double Tap = get_transformer_tap_in_pu();

    double Vbus = get_bus_positive_sequence_voltage_in_kV();
    double Eac = Vbus/(TurnRatio*Tap);
    double Vdc0 = N*(THREE_SQRT2_OVER_PI*Eac*cos_angle-Vdrop);
    return Vdc0;
}

double LCC::get_dc_voltage_drop_multiple_in_kV_per_A() const
{
    complex<double> Z = get_transformer_impedance_in_ohm();
    unsigned int N = get_number_of_bridge();
    double mult_vdrop = N*(THREE_OVER_PI*Z.imag()+2.0*Z.real());
    return mult_vdrop;
}


double LCC::get_commutating_overlap_angle_in_deg() const
{
    ostringstream osstream;

    double TurnRatio = get_transformer_grid2valve_turn_ratio();
    double Tap = get_transformer_tap_in_pu();
    double Vac = get_bus_positive_sequence_voltage_in_kV();
    double Eac = Vac/(TurnRatio*Tap);

    double Xc = get_transformer_impedance_in_ohm().imag();
    double Idc = get_dc_current_in_kA();
    double alpha_gamma = deg2rad(get_angle_in_deg());

    double mu = steps_acos(cos_angle-SQRT2*Idc*Xc/Eac)-alpha_gamma;

    return rad2deg(mu);
}

double LCC::get_dc_power_in_MW() const
{
    return get_dc_voltage_in_kV()*get_dc_current_in_kA();
}
double LCC::get_ac_active_power_in_MW() const
{

    double Idc = get_dc_current_in_kA();

    unsigned int N = get_number_of_bridge();
    double R = get_transformer_impedance_in_ohm().real();
    //double loss = SQRT3*Iac*Iac*R/N;
    double loss = 2.0*Idc*Idc*R*N;

    if(get_side()==RECTIFIER)
        return get_dc_power_in_MW()+loss;
    else
        return get_dc_power_in_MW()-loss;
}

double LCC::get_ac_reactive_power_in_MVar() const
{
    double Pac = get_ac_active_power_in_MW();
    double pf = get_ac_power_factor();

    return Pac*steps_sqrt(1.0-pf*pf)/pf;
}

double LCC::get_ac_apparent_power_in_MVA() const
{
    double Pac = get_ac_active_power_in_MW();
    double pf = get_ac_power_factor();
    if(pf!=0.0)
        return Pac/pf;
    else
        return Pac;
}

double LCC::get_ac_power_factor() const
{
    double mu = get_commutating_overlap_angle_in_deg();
    mu = deg2rad(mu);
    double angle = get_angle_in_deg();
    angle = deg2rad(angle);

    double tanPhi = 2*mu+steps_sin(2*angle)-steps_sin(2.0*(mu+angle));
    tanPhi /= (steps_cos(2.0*angle)-steps_cos(2.0*(mu+angle)));

    return steps_cos(steps_atan(tanPhi));
}

double LCC::get_ac_current_in_kA() const
{
    unsigned int N = get_number_of_bridge();
    double Idc = get_dc_current_in_kA();
    double turnRatio = get_transformer_grid2valve_turn_ratio();
    double tap = get_transformer_tap_in_pu();

    return SQRT6_OVER_PI*N*Idc/(turnRatio*tap);
}
