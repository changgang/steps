#include "header/model/source_model/source_model.h"
#include "header/device/source.h"
#include "header/power_system_database.h"
#include <cstdio>
SOURCE_MODEL::SOURCE_MODEL()
{
    ;
}

SOURCE_MODEL::~SOURCE_MODEL()
{
    ;
}


double SOURCE_MODEL::get_mbase_in_MVA() const
{
    SOURCE* source = (SOURCE*) get_device_pointer();
    if(source!=NULL)
        return source->get_mbase_in_MVA();
    else
        return 0.0;
}

complex<double> SOURCE_MODEL::get_terminal_complex_voltage_in_pu() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    SOURCE* source = (SOURCE*) get_device_pointer();
    size_t bus = source->get_source_bus();

    complex<double> Vxy = psdb->get_bus_complex_voltage_in_pu(bus);
    return Vxy;
}
