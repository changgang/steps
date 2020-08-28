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
    set_number_of_bridge(1);
    set_voltage_drop_per_bridge_in_kV(0.0);
    set_max_angle_in_deg(90.0);
    set_min_angle_in_deg(0.0);

    set_transformer_grid_side_base_voltage_in_kV(1.0);
    set_transformer_valve_side_base_voltage_in_kV(1.0);
    set_transformer_impedance_in_ohm(1.0);
    set_transformer_max_tap_in_pu(1.0);
    set_transformer_min_tap_in_pu(1.0);
    set_transformer_number_of_taps(1);

    set_angle_fixed_flag(false);
}

void LCC::set_number_of_bridge(const unsigned int n) { bridge_number = n; }
void LCC::set_voltage_drop_per_bridge_in_kV(const double V) { voltage_drop_per_bridge_in_kV = V; }
void LCC::set_max_angle_in_deg(const double angle) { max_angle_in_deg = angle; cos_max_angle = cos(deg2rad(angle)); }
void LCC::set_min_angle_in_deg(const double angle) { min_angle_in_deg = angle; cos_min_angle = cos(deg2rad(angle)); }
void LCC::set_transformer_grid_side_base_voltage_in_kV(const double V) { transformer_grid_side_base_voltage_in_kV = V; try_to_set_transformer_grid2valve_turn_ratio(); }
void LCC::set_transformer_valve_side_base_voltage_in_kV(const double V) { transformer_valve_side_base_voltage_in_kV = V; try_to_set_transformer_grid2valve_turn_ratio(); }
void LCC::set_transformer_impedance_in_ohm(const complex<double> z) { if(z!=0.0) {transformer_Z_in_ohm = z; transformer_Y_in_siemens = 1.0/z;} }
void LCC::set_transformer_admittance_in_siemens(const complex<double> y) { if(y!=0.0) {transformer_Y_in_siemens = y; transformer_Z_in_ohm = 1.0/y;} }
void LCC::set_transformer_max_tap_in_pu(const double maxtap) { transformer_max_tap_in_pu = maxtap; try_to_set_transformer_tap_step(); try_to_set_transformer_tap_fixed_flag(); }
void LCC::set_transformer_min_tap_in_pu(const double mintap) { transformer_min_tap_in_pu = mintap; try_to_set_transformer_tap_step(); try_to_set_transformer_tap_fixed_flag(); }
void LCC::set_transformer_number_of_taps(const unsigned int n) { transformer_number_of_taps = n; try_to_set_transformer_tap_step(); try_to_set_transformer_tap_fixed_flag(); }
void LCC::set_angle_fixed_flag(bool flag) { angle_fixed = flag; }

void LCC::try_to_set_transformer_grid2valve_turn_ratio()
{
    if(get_transformer_valve_side_base_voltage_in_kV() != 0.0)
        transformer_turn_ratio = get_transformer_grid_side_base_voltage_in_kV()/get_transformer_valve_side_base_voltage_in_kV();
}

void LCC::try_to_set_transformer_tap_step()
{
    if(get_transformer_number_of_taps() > 1)
        transformer_tap_step_in_pu = (get_transformer_max_tap_in_pu()-get_transformer_min_tap_in_pu())/(get_transformer_number_of_taps()-1);
}

void LCC::try_to_set_transformer_tap_fixed_flag()
{
    double Tapmax = get_transformer_min_tap_in_pu();
    double Tapmin = get_transformer_min_tap_in_pu();
    unsigned int nTap = get_transformer_number_of_taps();
    if(nTap <= 1 or fabs(Tapmax-Tapmin)<DOUBLE_EPSILON) // tap not changeable
        transformer_tap_fixed = true;
    else
        transformer_tap_fixed = false;
}

unsigned int LCC::get_number_of_bridge() const { return bridge_number; }
double LCC::get_voltage_drop_per_bridge_in_kV() const { return voltage_drop_per_bridge_in_kV; }
double LCC::get_max_angle_in_deg() const { return max_angle_in_deg; }
double LCC::get_min_angle_in_deg() const { return min_angle_in_deg; }
double LCC::get_cos_max_angle() const { return cos_max_angle; }
double LCC::get_cos_min_angle() const { return cos_min_angle; }
double LCC::get_transformer_grid_side_base_voltage_in_kV() const { return transformer_grid_side_base_voltage_in_kV; }
double LCC::get_transformer_valve_side_base_voltage_in_kV() const { return transformer_valve_side_base_voltage_in_kV; }
double LCC::get_transformer_grid2valve_turn_ratio() const { return transformer_turn_ratio; }
complex<double> LCC::get_transformer_impedance_in_ohm() const { return transformer_Z_in_ohm; }
complex<double> LCC::get_transformer_admittance_in_siemens() const { return transformer_Y_in_siemens; }
double LCC::get_transformer_max_tap_in_pu() const { return transformer_max_tap_in_pu; }
double LCC::get_transformer_min_tap_in_pu() const { return transformer_min_tap_in_pu; }
unsigned int LCC::get_transformer_number_of_taps() const { return transformer_number_of_taps; }
double LCC::get_transformer_tap_step_in_pu() const { return transformer_tap_step_in_pu; }
bool LCC::is_angle_fixed() const { return angle_fixed; }
bool LCC::is_transformer_tap_fixed() const { return transformer_tap_fixed; }
double LCC::get_bus_positive_sequence_voltage_in_kV() const { return get_bus_pointer()->get_positive_sequence_voltage_in_kV(); }
double LCC::get_angle_in_deg() const { return angle_in_deg; }
double LCC::get_cos_angle() const { return cos_angle; }
void LCC::set_transformer_tap_in_pu(double tap) { transformer_tap_in_pu = tap; }
double LCC::get_transformer_tap_in_pu() const { return transformer_tap_in_pu; }

void LCC::set_angle_in_deg(double angle, double cos_angle)
{
    if(angle>=0.0) angle_in_deg = angle;
    else angle_in_deg = 0.0;
    if(cos_angle!=0.0) cos_angle = cos_angle;
    else cos_angle = steps_cos(deg2rad(angle_in_deg));
}

LCC& LCC::operator=(const LCC& converter)
{
    if(this == (&converter)) return *this;
    clear();

    set_toolkit(converter.get_toolkit());

    set_bus(converter.get_bus());
    set_valve_side_bus_name(converter.get_valve_side_bus_name());
    set_name(converter.get_name());
    set_power_percent(converter.get_power_percent());

    set_number_of_bridge(converter.get_number_of_bridge());
    set_voltage_drop_per_bridge_in_kV(converter.get_voltage_drop_per_bridge_in_kV());
    set_max_angle_in_deg(converter.get_max_angle_in_deg());
    set_min_angle_in_deg(converter.get_min_angle_in_deg());

    set_transformer_grid_side_base_voltage_in_kV(converter.get_transformer_grid_side_base_voltage_in_kV());
    set_transformer_valve_side_base_voltage_in_kV(converter.get_transformer_valve_side_base_voltage_in_kV());
    set_transformer_impedance_in_ohm(converter.get_transformer_impedance_in_ohm());
    set_transformer_max_tap_in_pu(converter.get_transformer_max_tap_in_pu());
    set_transformer_min_tap_in_pu(converter.get_transformer_min_tap_in_pu());
    set_transformer_number_of_taps(converter.get_transformer_number_of_taps());

    set_angle_fixed_flag(converter.is_angle_fixed());

    return *this;
}

void LCC::solve_transformer_tap_and_angle(double Vdc, double Idc)
{
    double Tap = solve_best_transformer_tap_with_min_angle(Vdc, Idc);
    double cosAngle = solve_desired_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(Vdc, Idc, Tap);
    if(cosAngle>cos_min_angle) // minimum angle is reached
        set_angle_in_deg(get_min_angle_in_deg(), cos_min_angle);
    else
        set_angle_in_deg(rad2deg(steps_acos(cosAngle)), cosAngle);
}


double LCC::solve_best_transformer_tap_with_min_angle(double Vdc, double Idc)
{
    if(is_transformer_tap_fixed())
    {
        set_transformer_tap_in_pu(transformer_min_tap_in_pu);
        return transformer_min_tap_in_pu;
    }
    else
    {
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
        Tap = transformer_min_tap_in_pu +  TapStep*floor((Tap-transformer_min_tap_in_pu)/TapStep); // actual
        if(Tap < transformer_min_tap_in_pu) Tap = transformer_min_tap_in_pu; // apply limit
        else
            if(Tap > transformer_max_tap_in_pu) Tap = transformer_max_tap_in_pu;

        set_transformer_tap_in_pu(Tap);
        return Tap;
    }
}

double LCC::solve_desired_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(double Vdc, double Idc, double Tap)
{
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

double LCC::solve_dc_voltage_in_kV_with_dc_current_and_transformer_tap(double Idc)
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
