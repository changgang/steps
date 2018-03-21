#include "header/basic/transient_security_table.h"


TRANSIENT_SECURITY_TABLE::TRANSIENT_SECURITY_TABLE()
{
    set_threshold(0.0);
    set_time_span(0.0);
    set_check_for_greater_flag(false);
}

TRANSIENT_SECURITY_TABLE::~TRANSIENT_SECURITY_TABLE()
{
    ;
}

TRANSIENT_SECURITY_TABLE::TRANSIENT_SECURITY_TABLE(const TRANSIENT_SECURITY_TABLE& table)
{
    copy_from_const_table(table);
}

TRANSIENT_SECURITY_TABLE& TRANSIENT_SECURITY_TABLE::operator=(const TRANSIENT_SECURITY_TABLE& table)
{
    if(this==(&table))
        return *this;

    copy_from_const_table(table);
    return *this;
}

void TRANSIENT_SECURITY_TABLE::copy_from_const_table(const TRANSIENT_SECURITY_TABLE& table)
{
    set_threshold(table.get_threshold());
    set_time_span(table.get_threshold());
    set_check_for_greater_flag(table.get_check_for_greater_flag());
}

void TRANSIENT_SECURITY_TABLE::set_threshold(double threshold)
{
    this->threshold = threshold;
}

void TRANSIENT_SECURITY_TABLE::set_time_span(double time)
{
    this->time_span = time;
}

void TRANSIENT_SECURITY_TABLE::set_check_for_greater_flag(bool logic)
{
    check_for_greater = logic;
}

double TRANSIENT_SECURITY_TABLE::get_threshold() const
{
    return threshold;
}

double TRANSIENT_SECURITY_TABLE::get_time_span() const
{
    return time_span;
}

bool TRANSIENT_SECURITY_TABLE::get_check_for_greater_flag() const
{
    return check_for_greater;
}

