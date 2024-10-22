#include "header/model/common_model/converter_common_model/current_order_limiter_model/convcol.h"

CONVCOL0::CONVCOL0(STEPS& toolkit):COMMON_MODEL(toolkit)
{
    set_PQ_priority_flag(P_FIRST);
    set_Imax(0);
}

CONVCOL0::~CONVCOL0()
{
}

string CONVCOL0::get_model_name() const
{
    return "CONVCOL0";
}

void CONVCOL0::set_PQ_priority_flag(COL_PQ_PRIORITY pq_flag)
{
    pq_priority = pq_flag;
}

void CONVCOL0::set_Imax(double I)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(I<0.0)
    {
        osstream<<"Warning. Negative Imax ("<<I<<") is given to set "<<get_model_name()<<" of "<<get_compound_device_name()<<". Automatically changed to positive value ("<<-I<<").";
        toolkit.show_information_with_leading_time_stamp(osstream);
        I = -I;
    }
    Imax = I;
}

COL_PQ_PRIORITY CONVCOL0::get_PQ_priority_flag() const
{
    return pq_priority;
}

double CONVCOL0::get_Imax() const
{
    return Imax;
}

complex<double> CONVCOL0::get_limited_PQ_current_order(complex<double> I) const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double Imax = get_Imax();
    if(Imax > 0)
    {
        double Ip = I.real();
        double Iq = I.imag();
        double Ipmax, Ipmin, Iqmax, Iqmin;
        switch(get_PQ_priority_flag())
        {
            case Q_FIRST:
            {
                Iqmax = Imax;
                Iqmin = -Imax;
                if(fabs(Iq)<Imax)
                    Ipmax = steps_sqrt(Imax*Imax - Iq*Iq);
                else
                    Ipmax = 0.0;

                Ipmin = 0;
                if(get_device_type()==STEPS_ENERGY_STORAGE)
                {
                    Ipmin = -Ipmax;
                }
                break;
            }
            default: // P_FIRST
            {
                Ipmax = Imax;
                Ipmin = 0;
                if(get_device_type()==STEPS_ENERGY_STORAGE)
                {
                    Ipmin = -Ipmax;
                }
                if(fabs(Ip)<Imax)
                    Iqmax = steps_sqrt(Imax*Imax - Ip*Ip);
                else
                    Iqmax = 0.0;

                Iqmin = -Iqmax;
                break;
            }
        }
        if(Ip>Ipmax) Ip = Ipmax;
        if(Ip<Ipmin) Ip = Ipmin;
        if(Iq>Iqmax) Iq = Iqmax;
        if(Iq<Iqmin) Iq = Iqmin;
        return complex<double>(Ip, Iq);
    }
    else
    {
        osstream<<"Warning. CONVCOL0 has ZERO Imax. No current order will be limited. Maybe need data check.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return I;
    }
}
