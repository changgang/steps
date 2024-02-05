#include "header/model/converter_common_models/current_order_limiter_model/convcol.h"
#include "header/basic/utility.h"
CONVCOL0::CONVCOL0(STEPS& toolkit)
{
    set_toolkit(toolkit);
    set_PQ_priority_flag(P_FIRST);
    set_Imax(0);
}

CONVCOL0::~CONVCOL0()
{
}

void CONVCOL0::set_toolkit(STEPS& toolkit)
{
    this->toolkit = &toolkit;
}

STEPS& CONVCOL0::get_toolkit() const
{
    return *toolkit;
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
        osstream<<"Warning. Negative Imax ("<<I<<") is given to set CONVCOL0. Automatically changed to positive value ("<<-I<<").";
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
                Ipmax = steps_sqrt(Imax*Imax - Iq*Iq);
                Ipmin = 0;
                break;
            }
            default: // P_FIRST
            {
                Ipmax = Imax;
                Ipmin = 0;
                Iqmax = steps_sqrt(Imax*Imax - Ip*Ip);
                Iqmin = -Iqmax;
                break;
            }
        }
        if(Ip>Ipmax) Ip = Ipmax;
        else
            if(Ip<Ipmin) Ip = Ipmin;
        if(Iq>Iqmax) Iq = Iqmax;
        else
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
