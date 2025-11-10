#include "header/model/svs_model/svs_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
SVS_MODEL::SVS_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_SV_SOURCE);
}

SVS_MODEL::~SVS_MODEL()
{
    ;
}

SV_SOURCE* SVS_MODEL::get_sv_source_pointer() const
{
    return (SV_SOURCE*) get_device_pointer();
}

double SVS_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

complex<double> SVS_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}
