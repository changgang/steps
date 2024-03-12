#include "header/model/lcc_hvdc2t_model/vdcol.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

VDCOL::VDCOL()
{
    clear();
}


VDCOL::VDCOL(const VDCOL& limiter)
{
    clear();
    copy_from_const_vdcol(limiter);
}

VDCOL::~VDCOL()
{
}


double VDCOL::get_vdcol_voltage_of_last_point_in_kV() const
{
    unsigned int n = get_vdcol_point_count();
    if(n>0)
        return vdcol_parameters[n-1][0];
    else
        return 0.0;
}

double VDCOL::get_vdcol_current_of_last_point_in_kA() const
{
    unsigned int n = get_vdcol_point_count();
    if(n>0)
        return vdcol_parameters[n-1][1];
    else
        return 0.0;
}


void VDCOL::append_vdcol_point_in_kV_kA(double V_in_kV, double I_in_kA)
{
    ostringstream osstream;
    unsigned int n = get_vdcol_point_count();
    if(n==STEPS_MAX_VDCOL_TABLE_SIZE)
    {
        osstream<<"Warning. VDCOL table is full ("<<n<<"). No more record will be appended.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }
    if(n == 0)
    {
        vdcol_parameters[n][0] = V_in_kV;
        vdcol_parameters[n][1] = I_in_kA;
    }
    else
    {
        for(unsigned int i=0; i!=n; ++i)
        {
            if(V_in_kV==get_vdcol_voltage_of_point_in_kV(i))
            {
                osstream<<"Warning. Cannot set duplicate VDCOL points with the same voltage ("<<V_in_kV<<" kV).";
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
                return;
            }
        }

        vdcol_parameters[n][0] = V_in_kV;
        vdcol_parameters[n][1] = I_in_kA;

        ++n;
        for(unsigned int i=0; i<n-1; ++i)
        {
            bool swapped = false;
            for(unsigned int j=0; j<n-1; ++j)
            {
                if(vdcol_parameters[j][0]>vdcol_parameters[j+1][0])
                {
                    double vtemp = vdcol_parameters[j][0];
                    double itemp = vdcol_parameters[j][1];

                    vdcol_parameters[j][0] = vdcol_parameters[j+1][0];
                    vdcol_parameters[j][1] = vdcol_parameters[j+1][1];
                    vdcol_parameters[j+1][0] = vtemp;
                    vdcol_parameters[j+1][1] = itemp;
                    swapped = true;
                }
            }
            if(swapped==false)
                break;
        }
    }
}

unsigned int VDCOL::get_vdcol_point_count() const
{
    unsigned int n = 0;
    for(unsigned int i=0; i<STEPS_MAX_VDCOL_TABLE_SIZE; ++i)
    {
        if(vdcol_parameters[i][0]==0.0)
        {
            n = i;
            break;
        }
    }
    return n;
}

double VDCOL::get_vdcol_voltage_of_point_in_kV(unsigned int index) const
{
    if(index>=get_vdcol_point_count())
        return get_vdcol_voltage_of_last_point_in_kV();
    else
        return vdcol_parameters[index][0];
}

double VDCOL::get_vdcol_current_of_point_in_kA(unsigned int index) const
{
    if(index>=get_vdcol_point_count())
        return get_vdcol_current_of_last_point_in_kA();
    else
        return vdcol_parameters[index][1];
}

double VDCOL::get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(double Vdci_in_kV) const
{
    ostringstream osstream;
    unsigned int n = get_vdcol_point_count();
    if(n==0)
        return INFINITE_THRESHOLD;

    if(Vdci_in_kV>=get_vdcol_voltage_of_last_point_in_kV())
        return get_vdcol_current_of_last_point_in_kA();

    if(Vdci_in_kV<=get_vdcol_voltage_of_point_in_kV(0))
        return get_vdcol_current_of_point_in_kA(0);

    unsigned int n_minus_1 = n-1;
    for(unsigned int i=0; i!=n_minus_1; ++i)
    {
        double V_i = get_vdcol_voltage_of_point_in_kV(i);
        double V_i_1 = get_vdcol_voltage_of_point_in_kV(i+1);
        double I_i = get_vdcol_current_of_point_in_kA(i);
        double I_i_1 = get_vdcol_current_of_point_in_kA(i+1);
        if(Vdci_in_kV>=V_i and Vdci_in_kV<=V_i_1)
        {
            double slope = (I_i_1-I_i)/(V_i_1-V_i);
            return I_i + slope*(Vdci_in_kV-V_i);
        }
    }
    osstream<<"This warning information should never be displayed. Otherwise, the following VDCOL in invalid:"<<endl;
    for(unsigned int i=0; i!=n; ++i)
        osstream<<"Point "<<i<<": "<<get_vdcol_voltage_of_point_in_kV(i)<<" kV, "<<get_vdcol_current_of_point_in_kA(i)<<" kA"<<endl;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

    return 0.0;
}

void VDCOL::clear()
{
    for(unsigned int i=0; i<STEPS_MAX_VDCOL_TABLE_SIZE; ++i)
    {
        vdcol_parameters[i][0] = 0.0;
        vdcol_parameters[i][1] = 0.0;
    }
}

bool VDCOL::is_valid() const
{
    return true;
}

void VDCOL::check()
{
    ;
}

VDCOL& VDCOL::operator=(const VDCOL& limiter)
{
    if(this == &limiter)
        return *this;

    clear();

    copy_from_const_vdcol(limiter);

    return *this;
}

void VDCOL::copy_from_const_vdcol(const VDCOL& limiter)
{
    clear();
    unsigned int n = limiter.get_vdcol_point_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        double V = limiter.get_vdcol_voltage_of_point_in_kV(i);
        double I = limiter.get_vdcol_current_of_point_in_kA(i);
        append_vdcol_point_in_kV_kA(V, I);
    }
}
