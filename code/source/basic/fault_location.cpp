#include "header/basic/fault_location.h"
#include <cmath>

FAULT_LOCATION::FAULT_LOCATION()
{
    location_in_per_thousand = 0;
}

void FAULT_LOCATION::set_fault_location_in_pu(double location)
{
    if(location<0.0 or location>1.0) return;

    this->location_in_per_thousand = round(location*1000.0);
}

double FAULT_LOCATION::get_fault_location_in_pu() const
{
    return 0.001*location_in_per_thousand;
}

bool FAULT_LOCATION::operator<(const FAULT_LOCATION& fault_location) const
{
    double epsilon = 0.001; // if two faults are

    if((fault_location.get_fault_location_in_pu() - this->get_fault_location_in_pu())>(epsilon-FLOAT_EPSILON))
        return true;
    else
        return false;
}
bool FAULT_LOCATION::operator==(const FAULT_LOCATION& fault_location) const
{
    if(faul)
}
