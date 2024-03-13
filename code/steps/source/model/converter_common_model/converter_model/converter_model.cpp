#include "header/model/converter_common_models/converter_model/converter_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
CONVERTER_MODEL::CONVERTER_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
}

CONVERTER_MODEL::~CONVERTER_MODEL()
{
    ;
}

void CONVERTER_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& CONVERTER_MODEL::get_toolkit() const
{
    return *toolkit;
}

void CONVERTER_MODEL::set_bus_pointer(BUS* busptr)
{
    this->busptr = busptr;
}

void CONVERTER_MODEL::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

BUS* CONVERTER_MODEL::get_bus_pointer() const
{
    return busptr;
}

NONBUS_DEVICE* CONVERTER_MODEL::get_device_pointer() const
{
    return device;
}

DEVICE_ID CONVERTER_MODEL::get_device_id() const
{
    return device->get_device_id();
}

string CONVERTER_MODEL::get_compound_device_name() const
{
    return get_device_id().get_compound_device_name();
}
