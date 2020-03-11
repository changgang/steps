#include "header/basic/fault.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

FAULT::FAULT()
{
    clear();
}

FAULT::~FAULT()
{
    ;
}

void FAULT::set_fault_type(FAULT_TYPE fault)
{
    this->fault_type = fault;
}

void FAULT::set_fault_shunt_in_pu(const complex<double>& y)
{
    fault_shunt_in_pu = y;
}

FAULT_TYPE FAULT::get_fault_type() const
{
    return fault_type;
}

string FAULT::get_fault_type_string() const
{
    string fault_type_str;
    switch(get_fault_type())
    {
        case SINGLE_PHASE_GROUNDED_FAULT:
            fault_type_str = "SINGLE PHASE GROUNDED FAULT";
            break;
        case DOUBLE_PHASES_FAULT:
            fault_type_str = "DOUBLE PHASES FAULT";
            break;
        case DOUBLE_PHASES_GROUNDED_FAULT:
            fault_type_str = "DOUBLE PHASES GROUNDED FAULT";
            break;
        default:
            fault_type_str = "THREE PHASES FAULT";
            break;
    }
    return fault_type_str;
}
complex<double> FAULT::get_fault_shunt_in_pu() const
{
    return fault_shunt_in_pu;
}

bool FAULT::is_faulted() const
{
    if(steps_fast_complex_abs(get_fault_shunt_in_pu())>DOUBLE_EPSILON)
        return true;
    else
        return false;
}

void FAULT::clear()
{
    set_fault_shunt_in_pu(0.0);
}


FAULT& FAULT::operator=(const FAULT& fault)
{
    if(this == &fault) return *this;

    clear();
    set_fault_type(fault.get_fault_type());
    set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu());

    return *this;
}

