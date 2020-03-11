#include "header/model/pvu_models/pvu_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
PVU_MODEL::PVU_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("PV UNIT");
}

PVU_MODEL::~PVU_MODEL()
{
    ;
}

PV_UNIT* PVU_MODEL::get_pv_unit_pointer() const
{
    return (PV_UNIT*) get_device_pointer();
}

double PVU_MODEL::get_mbase_in_MVA() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    return pv_unit->get_mbase_in_MVA();
}

double PVU_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    return pv_unit->get_one_over_mbase_in_one_over_MVA();
}

unsigned int PVU_MODEL::get_number_of_lumped_pv_units() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    return pv_unit->get_number_of_lumped_pv_units();
}

double PVU_MODEL::get_rated_power_per_pv_unit_in_MW() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    return pv_unit->get_rated_power_per_pv_unit_in_MW();
}

double PVU_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

complex<double> PVU_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double PVU_MODEL::get_terminal_voltage_angle_in_rad() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_angle_in_rad();
}

double PVU_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

complex<double> PVU_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    return pv_unit->get_source_impedance_in_pu();
}
