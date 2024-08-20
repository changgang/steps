#include "header/model/converter_common_model/current_order_limiter_model/convcol.h"
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

void CONVCOL0::set_device_pointer(NONBUS_DEVICE* device)
{
    this->device = device;
}

NONBUS_DEVICE* CONVCOL0::get_device_pointer() const
{
    return device;
}

DEVICE_ID CONVCOL0::get_device_id() const
{
    return device->get_device_id();
}

STEPS_DEVICE_TYPE CONVCOL0::get_device_type() const
{
    return get_device_id().get_device_type();
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
