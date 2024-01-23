#include "header/model/bus_model/pll_model/pll_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

PLL_MODEL::PLL_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
    bus_ptr = NULL;
}

PLL_MODEL::~PLL_MODEL()
{
    ;
}

void PLL_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& PLL_MODEL::get_toolkit() const
{
    return *toolkit;
}

void PLL_MODEL::set_bus_pointer(BUS* bus)
{
    bus_ptr = bus;
}

BUS* PLL_MODEL::get_bus_pointer() const
{
    return bus_ptr;
}

unsigned int PLL_MODEL::get_bus() const
{
    return bus_ptr->get_bus_number();
}

double PLL_MODEL::get_pll_frequency_deviation_in_pu() const
{
    return get_pll_frequency_deviation_in_Hz()*tbase_s;
}

double PLL_MODEL::get_pll_frequency_in_pu() const
{
    return 1.0+get_pll_frequency_deviation_in_pu();
}

double PLL_MODEL::get_pll_frequency_in_Hz() const
{
    return fbase_Hz+get_pll_frequency_deviation_in_Hz();
}

double PLL_MODEL::get_pll_angle_in_deg() const
{
    return rad2deg(get_pll_angle_in_rad());
}

void PLL_MODEL::initialize_base_frequency_and_base_period()
{
    BUS* bus = get_bus_pointer();
    fbase_Hz = bus->get_base_frequency_in_Hz();
    tbase_s = 1.0/fbase_Hz;
}

double PLL_MODEL::get_base_frequency_in_Hz() const
{
    return fbase_Hz;
}

double PLL_MODEL::get_base_angle_speed_in_radps() const
{
    return DOUBLE_PI*get_base_frequency_in_Hz();
}

double PLL_MODEL::get_base_period_in_s() const
{
    return tbase_s;
}

complex<double> PLL_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double PLL_MODEL::get_terminal_voltage_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}
