#include "header/power_system_database.h"
#include "header/device/nonbus_device.h"
#include <istream>
#include <iostream>

using namespace std;

NONBUS_DEVICE::NONBUS_DEVICE(STEPS& toolkit) : DEVICE(toolkit)
{
    ownership.clear();
}

NONBUS_DEVICE::~NONBUS_DEVICE()
{
    ;
}

void NONBUS_DEVICE::set_ownership(const OWNERSHIP& os)
{
    this->ownership = os;
}

OWNERSHIP NONBUS_DEVICE::get_ownership() const
{
    return this->ownership;
}

unsigned int NONBUS_DEVICE::get_owner_count() const
{
    return ownership.get_owner_count();
}

unsigned int NONBUS_DEVICE::get_owner_of_index(unsigned int index) const
{
    return ownership.get_owner_of_index(index);
}

double NONBUS_DEVICE::get_fraction_of_owner_of_index(unsigned int index) const
{
    return ownership.get_fraction_of_owner_of_index(index);
}
