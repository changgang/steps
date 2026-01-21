#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_lcc_model/hlcc_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

HLCC_MODEL::HLCC_MODEL(STEPS& toolkit) : MT_HVDC_MODEL(toolkit)
{
    ;
}

HLCC_MODEL::~HLCC_MODEL()
{
    ;
}

void HLCC_MODEL::set_hlcc_pointer(HLCC* lcc)
{
    hlcc_ptr = lcc;
}

HLCC* HLCC_MODEL::get_hlcc_pointer() const
{
    return hlcc_ptr;
}

HLCC* HLCC_MODEL::get_hlcc_pointer_with_name(string name) const
{
    MT_HVDC* mtdc_ptr = get_mt_hvdc_pointer();
    MT_HVDC& mtdc = *mtdc_ptr;
    HLCC* lcc = mtdc.get_lcc_converter_with_name(name);
    return lcc;
}

string HLCC_MODEL::get_model_type() const
{
    return "MT HVDC LCC MODEL";
}
void HLCC_MODEL::set_model_converter_name(const string name)
{
    hlcc_name = name;
}
string HLCC_MODEL::get_model_converter_name() const
{
    return hlcc_name;
}

void HLCC_MODEL::set_bypass_flag(const bool flag)
{
    is_bypassed = flag;
}

void HLCC_MODEL::set_block_flag(const bool flag)
{
    is_blocked = flag;
}

bool HLCC_MODEL::get_bypass_flag()
{
    return is_bypassed;
}

bool HLCC_MODEL::get_block_flag()
{
    return is_blocked;
}

void HLCC_MODEL::block_lcc()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_blocked)
    {
        is_blocked = true;
        double TIME = toolkit.get_dynamic_simulation_time_in_s();
        osstream<<"LCC CONVERTER: "<<get_model_converter_name()<<" is BLOCKED in time: "<<TIME<<" s ";
        toolkit.show_information_with_leading_time_stamp(osstream);;
    }
}

void HLCC_MODEL::bypass_lcc()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_blocked)
    {
        if(not is_bypassed)
        {
            is_bypassed = true;
            double TIME = toolkit.get_dynamic_simulation_time_in_s();
            osstream<<"LCC CONVERTER: "<<get_model_converter_name()<<" is BYPASSED in time: "<<TIME<<" s ";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void HLCC_MODEL::unblock_lcc()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(is_blocked)
    {
        is_blocked = false;
        double TIME = toolkit.get_dynamic_simulation_time_in_s();
        osstream<<"LCC CONVERTER: "<<get_model_converter_name()<<" is UNBLOCKED in time: "<<TIME<<" s ";
        toolkit.show_information_with_leading_time_stamp(osstream);;
    }
}

void HLCC_MODEL::unbypass_lcc()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_blocked)
    {
        if(is_bypassed)
        {
            is_bypassed = false;
            double TIME = toolkit.get_dynamic_simulation_time_in_s();
            osstream<<"LCC CONVERTER: "<<get_model_converter_name()<<" is UNBYPASSED in time: "<<TIME<<" s ";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}


double HLCC_MODEL::get_lcc_converter_ueq_in_kV() const
{
    ostringstream osstream;
    HLCC* lcc_ptr = get_hlcc_pointer();
    HLCC& lcc = *lcc_ptr;
    double ueq = 0.0;
    if(not is_blocked)
    {
        if(not is_bypassed)
        {
            double Nbg = lcc.get_brideg_number();
            double Vac = lcc.get_ac_bus_vlotage_in_kV();
            double vbase_grid = lcc.get_converter_transformer_grid_side_base_voltage_in_kV();
            double vbase_converter = lcc.get_converter_transformer_converter_side_base_voltage_in_kV();
            double k = vbase_grid/vbase_converter;
            double tap = lcc.get_converter_transformer_tap_in_pu();
            double Eac = Vac/k/tap;

            double vdrop = lcc.get_voltage_drop_per_bridge_in_kV();
            double ang_in_deg = lcc.get_firing_angle_in_deg();
            double ang_in_rad = deg2rad(ang_in_deg);
            double cos_ang = steps_cos(ang_in_rad);

            ueq = Nbg*(THREE_SQRT2_OVER_PI*Eac*cos_ang-vdrop);
            /*
            double time = toolkit.get_dynamic_simulation_time_in_s();
            if((time>=1.049) and (time<=1.051))
            {
                osstream<<" EAC = "<<Eac<<"  cos angle="<<cos_ang<<" with angle = "<<ang_in_deg;
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            }
            */
        }
    }

    return ueq;
}
double HLCC_MODEL::get_lcc_converter_req_ohm() const
{
    HLCC* lcc_ptr = get_hlcc_pointer();
    HLCC& lcc = *lcc_ptr;
    double req = 0.0;
    if(not is_blocked)
    {
        if(not is_bypassed)
        {
            double Nbg = lcc.get_brideg_number();
            complex<double> Z = lcc.get_converter_transformer_Z_in_ohm();
            double R = Z.real();
            double X = Z.imag();
            req = Nbg*(THREE_OVER_PI*X+2*R);
        }
    }
    return req;
}

double HLCC_MODEL::get_converter_dc_poewr_in_MW()
{
    ostringstream osstream;
    HLCC* lcc_ptr = get_hlcc_pointer();
    HLCC& lcc = *lcc_ptr;
    double Pdc = 0.0;
    if(not is_blocked)
    {
        if(not is_bypassed)
        {
            double Udc = lcc.get_udc_in_kV();
            double Idc = lcc.get_idc_in_kA();
            Pdc = Udc*Idc;
            return Pdc;
        }
        else
        {
            return 0.0;
        }
    }
    else
        return 0.0;
}

complex<double> HLCC_MODEL::get_converter_ac_complex_power_in_MVA()
{
    ostringstream osstream;
    HLCC* lcc_ptr = get_hlcc_pointer();
    HLCC& lcc = *lcc_ptr;
    complex<double> S = complex<double>(0.0 , 0.0);
    if(not is_blocked)
    {
        if(not is_bypassed)
        {
            double Pac = lcc.get_converter_P_2_ac_bus();
            double Qac = lcc.get_converter_Q_2_ac_bus();
            S = complex<double>(Pac , Qac);
            return S;
        }
        else
        {
            return complex<double>(0.0 , 0.0);
        }
    }
    else
        return complex<double>(0.0 , 0.0);
}

double HLCC_MODEL::get_converter_ac_power_factor()
{
    return 0.0;
}

complex<double> HLCC_MODEL::get_converter_ac_current_in_kA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HLCC* lcc_ptr = get_hlcc_pointer();
    HLCC& lcc = *lcc_ptr;

    complex<double> S = get_converter_ac_complex_power_in_MVA();
    complex<double> Vac = psdb.get_bus_positive_sequence_complex_voltage_in_kV(lcc.get_converter_ac_bus());

    complex<double> Ic = conj(S/SQRT3/Vac);
    return Ic;
}

complex<double> HLCC_MODEL::get_converter_ac_current_in_pu_on_system_base()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HLCC* lcc_ptr = get_hlcc_pointer();
    HLCC& lcc = *lcc_ptr;
    double Sbase = psdb.get_system_base_power_in_MVA();

    complex<double> S = get_converter_ac_complex_power_in_MVA()/Sbase;
    complex<double> Vac = psdb.get_bus_positive_sequence_complex_voltage_in_pu(lcc.get_converter_ac_bus());

    complex<double> Ic = conj(S/Vac);


    return Ic;
}
