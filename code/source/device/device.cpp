#include "header/power_system_database.h"
#include "header/device/device.h"
#include <iostream>

using namespace std;

DEVICE::DEVICE()
{
    ownership.clear();

    set_power_system_database(NULL);
}

DEVICE::~DEVICE()
{
    ;
}

double DEVICE::get_dynamic_simulator_time_in_s() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb!=NULL)
        return psdb->get_dynamic_simulator_time_in_s();
    else
        return 0.0;
}

void DEVICE::set_ownership(const OWNERSHIP& os)
{
    this->ownership = os;
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
