#include "header/block/block.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

bool BLOCK::automatic_large_step_logic = false;

BLOCK::BLOCK(STEPS& toolkit)
{
    set_toolkit(toolkit);
    set_limiter_type(NO_LIMITER);
    disable_automatic_large_step_logic();
    state = 0.0;
    store  = 0.0;
    upper_limit = 0.0;
    lower_limit = 0.0;
    input = 0.0;
    output = 0.0;
}

BLOCK::~BLOCK()
{
    ;
}

void BLOCK::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& BLOCK::get_toolkit() const
{
    return *toolkit;
}

void BLOCK::set_state(double value)
{
    state = value;
}

void BLOCK::set_state_WITH_CAUTION(double value)
{
    set_state(value);
}

void BLOCK::set_store(double value)
{
    store = value;
}

double BLOCK::get_state() const
{
    return state;
}

double BLOCK::get_store() const
{
    return store;
}

void BLOCK::set_limiter_type(LIMITER_TYPE limiter)
{
    this->limiter_type = limiter;
}

void BLOCK::set_upper_limit(double vmax)
{
    if(limiter_type == NO_LIMITER)
        this->upper_limit = 0.0;
    else
        this->upper_limit = vmax;
}

void BLOCK::set_lower_limit(double vmin)
{
    if(limiter_type == NO_LIMITER)
        this->lower_limit = 0.0;
    else
        this->lower_limit = vmin;
}

LIMITER_TYPE BLOCK::get_limiter_type() const
{
    return limiter_type;
}

double BLOCK::get_upper_limit() const
{
    if(limiter_type == NO_LIMITER) return 0.0;
    return upper_limit;
}

double BLOCK::get_lower_limit() const
{
    if(limiter_type == NO_LIMITER) return 0.0;
    return lower_limit;
}

void BLOCK::set_input(double x)
{
    this->input = x;
}

void BLOCK::set_output(double y)
{
    this->output = y;
}

double BLOCK::get_input() const
{
    return input;
}

double BLOCK::get_output() const
{
    return output;
}

void BLOCK::enable_automatic_large_step_logic()
{
    automatic_large_step_logic = true;
}

void BLOCK::disable_automatic_large_step_logic()
{
    automatic_large_step_logic = false;
}

bool BLOCK::get_automatic_large_step_logic()
{
    return automatic_large_step_logic;
}

void BLOCK::check_limiter() const
{
    if(limiter_type != NO_LIMITER)
    {
        if(upper_limit<lower_limit)
        {
            ostringstream osstream;
            osstream<<"Error. Limiter upper bound ("<<upper_limit<<") is less than lower bound ("<<lower_limit<<").";
            STEPS& toolkit = get_toolkit();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}
