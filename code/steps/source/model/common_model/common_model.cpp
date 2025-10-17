#include "header/model/common_model/common_model.h"

COMMON_MODEL::COMMON_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
}

COMMON_MODEL::~COMMON_MODEL()
{
    ;
}

void COMMON_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& COMMON_MODEL::get_toolkit() const
{
    return *toolkit;
}

void COMMON_MODEL::set_bus_pointer(BUS* bus)
{
    this->bus = bus;
}

void COMMON_MODEL::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

unsigned int COMMON_MODEL::get_bus_number() const
{
    return bus->get_bus_number();
}

NONBUS_DEVICE* COMMON_MODEL::get_device_pointer() const
{
    return device;
}

DEVICE_ID COMMON_MODEL::get_device_id() const
{
    return device->get_device_id();
}

STEPS_DEVICE_TYPE COMMON_MODEL::get_device_type() const
{
    return get_device_id().get_device_type();
}

string COMMON_MODEL::get_compound_device_name() const
{
    return get_device_id().get_compound_device_name();
}

double COMMON_MODEL::get_bus_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

complex<double> COMMON_MODEL::get_bus_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double COMMON_MODEL::get_bus_voltage_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}

double COMMON_MODEL::get_bus_frequency_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_in_pu();
}

double COMMON_MODEL::get_bus_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_in_Hz();
}

double COMMON_MODEL::get_bus_frequency_deviation_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

double COMMON_MODEL::get_bus_frequency_deviation_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_Hz();
}

double COMMON_MODEL::get_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

double COMMON_MODEL::get_base_angle_speed_in_radps() const
{
    return DOUBLE_PI*get_base_frequency_in_Hz();
}

double COMMON_MODEL::get_base_period_in_s() const
{
    return 1.0/get_base_frequency_in_Hz();
}

