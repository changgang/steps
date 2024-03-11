#include "header/model/converter_common_models/source_converter_model/source_converter_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;
SOURCE_CONVERTER_MODEL::SOURCE_CONVERTER_MODEL(STEPS& toolkit)
{
    set_toolkit(toolkit);
}

SOURCE_CONVERTER_MODEL::~SOURCE_CONVERTER_MODEL()
{
    ;
}

void SOURCE_CONVERTER_MODEL::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& SOURCE_CONVERTER_MODEL::get_toolkit() const
{
    return *toolkit;
}

void SOURCE_CONVERTER_MODEL::set_bus_pointer(BUS* busptr)
{
    this->busptr = busptr;
}

void SOURCE_CONVERTER_MODEL::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

BUS* SOURCE_CONVERTER_MODEL::get_bus_pointer() const
{
    return busptr;
}

NONBUS_DEVICE* SOURCE_CONVERTER_MODEL::get_device_pointer() const
{
    return device;
}

DEVICE_ID SOURCE_CONVERTER_MODEL::get_device_id() const
{
    return device->get_device_id();
}

string SOURCE_CONVERTER_MODEL::get_compound_device_name() const
{
    return get_device_id().get_compound_device_name();
}
