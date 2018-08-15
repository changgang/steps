#include "header/model/hvdc_model/vdcol.h"
#include "header/basic/utility.h"
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
    clear();
}


double VDCOL::get_vdcol_voltage_of_last_point_in_kV() const
{
    size_t n = get_vdcol_point_count();
    if(n>0)
        return vdcol_parameters[n-1][0];
    else
        return 0.0;
}

double VDCOL::get_vdcol_current_of_last_point_in_kA() const
{
    size_t n = get_vdcol_point_count();
    if(n>0)
        return vdcol_parameters[n-1][1];
    else
        return 0.0;
}


void VDCOL::append_vdcol_point_in_kV_kA(double V_in_kV, double I_in_kA)
{
    ostringstream osstream;
    size_t n = get_vdcol_point_count();
    if(n == 0)
    {
        vector<double> point;
        point.push_back(V_in_kV);
        point.push_back(I_in_kA);
        vdcol_parameters.push_back(point);
    }
    else
    {
        for(size_t i=0; i!=n; ++i)
        {
            if(V_in_kV==get_vdcol_voltage_of_point_in_kV(i))
            {
                osstream<<"Warning. Cannot set duplicate VDCOL points with the same voltage ("<<V_in_kV<<" kV).";
                show_information_with_leading_time_stamp(osstream);
                return;
            }
        }

        if(V_in_kV>get_vdcol_voltage_of_last_point_in_kV())
        {
            vector<double> point;
            point.push_back(V_in_kV);
            point.push_back(I_in_kA);
            vdcol_parameters.push_back(point);
        }
        else
        {
            size_t index = n;
            for(size_t i=0; i!=n; ++i)
            {
                if(get_vdcol_voltage_of_point_in_kV(i)>V_in_kV)
                {
                    index = i;
                    break;
                }
            }
            vector< vector<double> > temp;
            for(size_t i=0; i!=index; ++i)
            {
                vector<double> point;
                point.push_back(get_vdcol_voltage_of_point_in_kV(i));
                point.push_back(get_vdcol_current_of_point_in_kA(i));
                temp.push_back(point);
            }
            vector<double> point;
            point.push_back(V_in_kV);
            point.push_back(I_in_kA);
            temp.push_back(point);
            for(size_t i=index; i!=n; ++i)
            {
                vector<double> point;
                point.push_back(get_vdcol_voltage_of_point_in_kV(i));
                point.push_back(get_vdcol_current_of_point_in_kA(i));
                temp.push_back(point);
            }
            vdcol_parameters = temp;
        }
    }
}

size_t VDCOL::get_vdcol_point_count() const
{
    return vdcol_parameters.size();
}

double VDCOL::get_vdcol_voltage_of_point_in_kV(size_t index) const
{
    if(index>=get_vdcol_point_count())
        return get_vdcol_voltage_of_last_point_in_kV();
    else
        return vdcol_parameters[index][0];
}

double VDCOL::get_vdcol_current_of_point_in_kA(size_t index) const
{
    if(index>=get_vdcol_point_count())
        return get_vdcol_current_of_last_point_in_kA();
    else
        return vdcol_parameters[index][1];
}

double VDCOL::get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(double Vdci_in_kV) const
{
    ostringstream osstream;

    size_t n = get_vdcol_point_count();
    if(n==0)
        return INFINITE_THRESHOLD;

    if(Vdci_in_kV>=get_vdcol_voltage_of_last_point_in_kV())
        return get_vdcol_current_of_last_point_in_kA();

    if(Vdci_in_kV<=get_vdcol_voltage_of_point_in_kV(0))
        return get_vdcol_current_of_point_in_kA(0);

    size_t n_minus_1 = n-1;
    for(size_t i=0; i!=n_minus_1; ++i)
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
    for(size_t i=0; i!=n; ++i)
        osstream<<"Point "<<i<<": "<<get_vdcol_voltage_of_point_in_kV(i)<<" kV, "<<get_vdcol_current_of_point_in_kA(i)<<" kA"<<endl;
    show_information_with_leading_time_stamp(osstream);

    return 0.0;
}

void VDCOL::clear()
{
    vdcol_parameters.clear();
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

    size_t n = limiter.get_vdcol_point_count();
    for(size_t i=0; i!=n; ++i)
    {
        double V = limiter.get_vdcol_voltage_of_point_in_kV(i);
        double I = limiter.get_vdcol_current_of_point_in_kA(i);
        append_vdcol_point_in_kV_kA(V, I);
    }
}
