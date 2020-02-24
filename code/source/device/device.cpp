#include "header/power_system_database.h"
#include "header/device/device.h"
#include <istream>
#include <iostream>

using namespace std;

DEVICE::DEVICE(STEPS& toolkit)
{
    set_toolkit(toolkit);
}

DEVICE::~DEVICE()
{
    ;
}

void DEVICE::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& DEVICE::get_toolkit() const
{
    return (*toolkit);
}

string DEVICE::get_device_name() const
{
    return get_device_id().get_device_name();
}
