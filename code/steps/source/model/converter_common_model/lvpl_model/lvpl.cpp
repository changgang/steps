#include "header/model/converter_common_models/lvpl_model/lvpl.h"
#include "header/basic/constants.h"
#include <cmath>

LVPL::LVPL()
{
    set_low_voltage_in_pu(0.0);
    set_high_voltage_in_pu(0.0);
    set_gain_at_high_voltage(INFINITE_THRESHOLD);
}

LVPL::~LVPL()
{
    ;
}

LVPL::LVPL(const LVPL& lvpl)
{
    copy_from_const_object(lvpl);
}

LVPL& LVPL::operator=(const LVPL& lvpl)
{
    if(this==(&lvpl))
        return *this;

    copy_from_const_object(lvpl);
    return *this;
}

void LVPL::copy_from_const_object(const LVPL& lvpl)
{
    set_low_voltage_in_pu(lvpl.get_low_voltage_in_pu());
    set_high_voltage_in_pu(lvpl.get_high_voltage_in_pu());
    set_gain_at_high_voltage(lvpl.get_gain_at_hig_voltage());
}

void LVPL::set_low_voltage_in_pu(double v)
{
    v_low = v;
}

void LVPL::set_high_voltage_in_pu(double v)
{
    v_high = v;
}

void LVPL::set_gain_at_high_voltage(double g)
{
    g_high = g;
}

double LVPL::get_low_voltage_in_pu() const
{
    return v_low;
}

double LVPL::get_high_voltage_in_pu() const
{
    return v_high;
}

double LVPL::get_gain_at_hig_voltage() const
{
    return g_high;
}

double LVPL::get_LVPL_order(double v) const
{
    double v1 = get_low_voltage_in_pu();
    double v2 = get_high_voltage_in_pu();
    double g = get_gain_at_hig_voltage();

    if(v1>v2)
    {
        double v = v1;
        v1 = v2;
        v2 = v;
    }

    if(v1!=0.0 or v2!=0.0)
    {
        if(fabs(v1-v2)>DOUBLE_EPSILON)
        {
            if(v>v1 and v<v2)
            {
                double slope = g/(v2-v1);
                double order = v1+(v-v1)*slope;
                return order;
            }
            else
            {
                if(v<=v1)
                    return 0.0;
                else
                    return INFINITE_THRESHOLD;
            }
        }
        else
        {
            if(v<v1)
                return 0.0;
            else
                return INFINITE_THRESHOLD;
        }
    }
    else
        return INFINITE_THRESHOLD;
}
