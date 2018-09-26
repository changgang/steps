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
    set_attached_device(NULL);
    set_timer_interval_in_s(INFINITE_THRESHOLD);
    reset();
}

void TIMER::set_attached_device(DEVICE* device)
{
    device_ptr = device;
}

DEVICE* TIMER::get_attached_device() const
{
    return device_ptr;
}

void TIMER::set_timer_interval_in_s(double t)
{
    if(t<0.0)
        t = -t;
    time_interval_in_s = t;
}

void TIMER::start()
{
    DEVICE* device = get_attached_device();
    if(device==NULL)
        return;

    time_when_timer_is_started_in_s = get_dynamic_simulation_time_in_s();
}

void TIMER::reset()
{
    time_when_timer_is_started_in_s = INFINITE_THRESHOLD;
}

bool TIMER::is_started() const
{
    if(fabs(time_when_timer_is_started_in_s-INFINITE_THRESHOLD)<FLOAT_EPSILON)
        return false;
    else
        return true;
}

bool TIMER::is_timed_out() const
{
    if(time_when_timer_is_started_in_s==INFINITE_THRESHOLD)
        return false;
    else
    {
        double TIME = get_dynamic_simulation_time_in_s();

        if(TIME>(time_when_timer_is_started_in_s+time_interval_in_s-FLOAT_EPSILON))
            return true;
        else
            return false;
    }
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
    if(get_timer_interval_in_s()==INFINITE_THRESHOLD)
        return false;
    else
        return true;
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
