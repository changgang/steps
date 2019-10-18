#include "header/power_system_database.h"
#include "header/device/device.h"
#include <istream>
#include <iostream>

using namespace std;

DEVICE::DEVICE()
{
    ownership.clear();
}

DEVICE::~DEVICE()
{
    ;
}

void DEVICE::set_ownership(const OWNERSHIP& os)
{
    this->ownership = os;
}

OWNERSHIP DEVICE::get_ownership() const
{
    return this->ownership;
}

size_t DEVICE::get_owner_count() const
{
    return ownership.get_owner_count();
}

size_t DEVICE::get_owner_of_index(size_t index) const
{
    return ownership.get_owner_of_index(index);
}

double DEVICE::get_fraction_of_owner_of_index(size_t index) const
{
    return ownership.get_fraction_of_owner_of_index(index);
}

string DEVICE::get_device_name() const
{
    return get_device_id().get_device_name();
}
