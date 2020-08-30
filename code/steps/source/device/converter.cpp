#include "header/device/converter.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

CONVERTER::CONVERTER(STEPS& toolkit)
{
    clear();
    set_toolkit(toolkit);
}

CONVERTER::~CONVERTER() { ; }

void CONVERTER::set_toolkit(STEPS& toolkit) { this->toolkit = (&toolkit); }
STEPS& CONVERTER::get_toolkit() const { return *toolkit; }

void CONVERTER::clear()
{
    set_side(RECTIFIER);

    bus = 0;
    busptr = nullptr;
    set_name("");
    set_valve_side_bus_name("");
    set_power_percent(1.0);

    set_dc_voltage_in_kV(0.0);
    set_dc_current_in_kA(0.0);

    set_transformer_grid_side_base_voltage_in_kV(1.0);
    set_transformer_valve_side_base_voltage_in_kV(1.0);
    set_transformer_impedance_in_ohm(1.0);
    set_transformer_admittance_in_siemens(1.0);
    set_transformer_max_tap_in_pu(1.0);
    set_transformer_min_tap_in_pu(1.0);
    set_transformer_number_of_taps(1);
    set_transformer_tap_in_pu(1.0);
}

void CONVERTER::copy_from_const_converter(const CONVERTER& converter)
{
    set_toolkit(converter.get_toolkit());

    set_side(converter.get_side());
    set_bus(converter.get_bus());
    set_valve_side_bus_name(converter.get_valve_side_bus_name());
    set_name(converter.get_name());
    set_power_percent(converter.get_power_percent());

    set_dc_voltage_in_kV(converter.get_dc_voltage_in_kV());
    set_dc_current_in_kA(converter.get_dc_current_in_kA());

    set_transformer_grid_side_base_voltage_in_kV(converter.get_transformer_grid_side_base_voltage_in_kV());
    set_transformer_valve_side_base_voltage_in_kV(converter.get_transformer_valve_side_base_voltage_in_kV());
    set_transformer_impedance_in_ohm(converter.get_transformer_impedance_in_ohm());
    set_transformer_admittance_in_siemens(converter.get_transformer_admittance_in_siemens());
    set_transformer_max_tap_in_pu(converter.get_transformer_max_tap_in_pu());
    set_transformer_min_tap_in_pu(converter.get_transformer_min_tap_in_pu());
    set_transformer_number_of_taps(converter.get_transformer_number_of_taps());
    set_transformer_tap_in_pu(converter.get_transformer_tap_in_pu());
}

void CONVERTER::set_side(CONVERTER_SIDE side)
{
    this->side = side;
}

CONVERTER_SIDE CONVERTER::get_side() const
{
    return side;
}

void CONVERTER::set_bus(const unsigned int bus)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_bus_exist(bus))
    {
        this->bus = bus;
        busptr = psdb.get_bus(bus);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Bus "<<bus<<" does not exist in power system database "<<psdb.get_system_name()<<" when setting converter bus.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void CONVERTER::set_valve_side_bus_name(string name) { valve_bus_name = name; }
void CONVERTER::set_name(string name) { converter_name = name; }
void CONVERTER::set_power_percent(double percent) { power_percent = percent; }

void CONVERTER::set_dc_voltage_in_kV(double V) {dc_voltage_in_kV = V;}
void CONVERTER::set_dc_current_in_kA(double I) {dc_current_in_kA = I;}

void CONVERTER::set_transformer_grid_side_base_voltage_in_kV(const double V) { transformer_grid_side_base_voltage_in_kV = V; try_to_set_transformer_grid2valve_turn_ratio(); }
void CONVERTER::set_transformer_valve_side_base_voltage_in_kV(const double V) { transformer_valve_side_base_voltage_in_kV = V; try_to_set_transformer_grid2valve_turn_ratio(); }
void CONVERTER::set_transformer_impedance_in_ohm(const complex<double> z) { if(z!=0.0) {transformer_Z_in_ohm = z; transformer_Y_in_siemens = 1.0/z;} }
void CONVERTER::set_transformer_admittance_in_siemens(const complex<double> y) { if(y!=0.0) {transformer_Y_in_siemens = y; transformer_Z_in_ohm = 1.0/y;} }
void CONVERTER::set_transformer_max_tap_in_pu(const double maxtap) { transformer_max_tap_in_pu = maxtap; try_to_set_transformer_tap_step(); try_to_set_transformer_tap_fixed_flag(); }
void CONVERTER::set_transformer_min_tap_in_pu(const double mintap) { transformer_min_tap_in_pu = mintap; try_to_set_transformer_tap_step(); try_to_set_transformer_tap_fixed_flag(); }
void CONVERTER::set_transformer_number_of_taps(const unsigned int n) { transformer_number_of_taps = n; try_to_set_transformer_tap_step(); try_to_set_transformer_tap_fixed_flag(); }
void CONVERTER::set_transformer_tap_in_pu(double tap) { transformer_tap_in_pu = tap; }


void CONVERTER::try_to_set_transformer_grid2valve_turn_ratio()
{
    if(get_transformer_valve_side_base_voltage_in_kV() != 0.0)
        transformer_turn_ratio = get_transformer_grid_side_base_voltage_in_kV()/get_transformer_valve_side_base_voltage_in_kV();
}

void CONVERTER::try_to_set_transformer_tap_step()
{
    if(get_transformer_number_of_taps() > 1)
        transformer_tap_step_in_pu = (get_transformer_max_tap_in_pu()-get_transformer_min_tap_in_pu())/(get_transformer_number_of_taps()-1);
}

void CONVERTER::try_to_set_transformer_tap_fixed_flag()
{
    double Tapmax = get_transformer_max_tap_in_pu();
    double Tapmin = get_transformer_min_tap_in_pu();
    unsigned int nTap = get_transformer_number_of_taps();
    if(nTap <= 1 or fabs(Tapmax-Tapmin)<DOUBLE_EPSILON) // tap not changeable
        transformer_tap_fixed = true;
    else
        transformer_tap_fixed = false;
}

unsigned int CONVERTER::get_bus() const { return bus; }
BUS* CONVERTER::get_bus_pointer() const { return busptr; }
string CONVERTER::get_valve_side_bus_name() const { return valve_bus_name; }
string CONVERTER::get_name() const { return converter_name; }
double CONVERTER::get_power_percent() const { return power_percent; }

double CONVERTER::get_dc_voltage_in_kV() const {return dc_voltage_in_kV;}
double CONVERTER::get_dc_current_in_kA() const {return dc_current_in_kA;}

double CONVERTER::get_transformer_grid_side_base_voltage_in_kV() const { return transformer_grid_side_base_voltage_in_kV; }
double CONVERTER::get_transformer_valve_side_base_voltage_in_kV() const { return transformer_valve_side_base_voltage_in_kV; }
double CONVERTER::get_transformer_grid2valve_turn_ratio() const { return transformer_turn_ratio; }
complex<double> CONVERTER::get_transformer_impedance_in_ohm() const { return transformer_Z_in_ohm; }
complex<double> CONVERTER::get_transformer_admittance_in_siemens() const { return transformer_Y_in_siemens; }
double CONVERTER::get_transformer_max_tap_in_pu() const { return transformer_max_tap_in_pu; }
double CONVERTER::get_transformer_min_tap_in_pu() const { return transformer_min_tap_in_pu; }
unsigned int CONVERTER::get_transformer_number_of_taps() const { return transformer_number_of_taps; }
double CONVERTER::get_transformer_tap_step_in_pu() const { return transformer_tap_step_in_pu; }
double CONVERTER::get_transformer_tap_in_pu() const { return transformer_tap_in_pu; }
bool CONVERTER::is_transformer_tap_fixed() const { return transformer_tap_fixed; }

double CONVERTER::get_bus_positive_sequence_voltage_in_pu() const { return get_bus_pointer()->get_positive_sequence_voltage_in_pu(); }
double CONVERTER::get_bus_positive_sequence_voltage_in_kV() const { return get_bus_pointer()->get_positive_sequence_voltage_in_kV(); }
complex<double> CONVERTER::get_bus_positive_sequence_complex_voltage_in_pu() const { return get_bus_pointer()->get_positive_sequence_complex_voltage_in_pu(); }
double CONVERTER::get_bus_base_voltage_in_kV() const { return get_bus_pointer()->get_base_voltage_in_kV(); }

bool CONVERTER::is_connected_to_bus(unsigned int bus) const {return get_bus() == bus;}
bool CONVERTER::is_in_area(unsigned int area) const {return get_bus_pointer()->is_in_area(area); }
bool CONVERTER::is_in_zone(unsigned int zone) const {return get_bus_pointer()->is_in_zone(zone); }
