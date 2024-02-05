#include "header/basic/timer.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

TIMER::TIMER()
{
    clear();
}

TIMER::TIMER(const TIMER& timer)
{
    this->set_timer_interval_in_s(timer.get_timer_interval_in_s());
    reset();
}

TIMER::~TIMER()
{
    ;
}

void TIMER::clear()
{
    set_timer_interval_in_s(INFINITE_THRESHOLD);
    reset();
}

void TIMER::set_timer_interval_in_s(double t)
{
    if(t<0.0)
        t = -t;
    time_interval_in_s = t;
}

void TIMER::start()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    time_when_timer_is_started_in_s = toolkit.get_dynamic_simulation_time_in_s();
}

void TIMER::reset()
{
    time_when_timer_is_started_in_s = INFINITE_THRESHOLD;
}

bool TIMER::is_started() const
{
    if(fabs(time_when_timer_is_started_in_s-INFINITE_THRESHOLD)<DOUBLE_EPSILON)
        return false;
    else
        return true;
}

bool TIMER::is_timed_out() const
{
    if(time_when_timer_is_started_in_s!=INFINITE_THRESHOLD)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        double TIME = toolkit.get_dynamic_simulation_time_in_s();

        if(TIME>(time_when_timer_is_started_in_s+time_interval_in_s-DOUBLE_EPSILON))
            return true;
        else
            return false;
    }
    else
        return false;
}

double TIMER::get_timer_interval_in_s() const
{
    return time_interval_in_s;
}

double TIMER::get_time_when_started_in_s() const
{
    return time_when_timer_is_started_in_s;
}

bool TIMER::is_valid() const
{
    if(get_timer_interval_in_s()!=INFINITE_THRESHOLD)
        return true;
    else
        return false;
}

void TIMER::check()
{
    ;
}

void TIMER::report() const
{
    ;
}

TIMER& TIMER::operator=(const TIMER& timer)
{
    if(this==&timer)
        return *this;

    this->set_timer_interval_in_s(timer.get_timer_interval_in_s());
    reset();
    return *this;
}
