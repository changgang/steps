#include "header/model/pvu_models/pv_panel_model/pvpnly.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

PVPNLY::PVPNLY(STEPS& toolkit) : PV_PANEL_MODEL(toolkit),
                                 capacitor(toolkit),
                                 Vdc_controller(toolkit)
{
    clear();
}

PVPNLY::PVPNLY(const PVPNLY& model):PV_PANEL_MODEL(model.get_toolkit()),
                                    capacitor(model.get_toolkit()),
                                    Vdc_controller(model.get_toolkit())

{
    copy_from_const_model(model);
}

PVPNLY::~PVPNLY()
{
    ;
}

PVPNLY& PVPNLY::operator=(const PVPNLY& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void PVPNLY::copy_from_const_model(const PVPNLY& model)
{
    set_toolkit(model.get_toolkit());

    clear();

    PV_PANEL_MODEL::copy_from_const_model(model);

}

void PVPNLY::clear()
{
    set_model_float_parameter_count(20);
    set_Im_in_A_per_solar_cell(7.35);
    set_Isc_in_A_per_solar_cell(7.87);
    set_Vm_in_V_per_solar_cell(29.0);
    set_Voc_in_V_per_solar_cell(36.3);
    set_Sref_in_Wpm2(1000.0);
    set_a(0.0025);
    set_b(0.5);
    set_c(0.00288);
    set_eta(1.0);
    set_Nse(500);
    set_Nsh(10);
    set_dc_voltage_control_mode(PV_MPPT_MODE);
    set_Smax_in_Wpm2(INFINITE_THRESHOLD);
    set_initial_Vdc_per_solar_cell_in_V(INFINITE_THRESHOLD);
    current_S_in_Wpm2 = DOUBLE_EPSILON;
    current_T_in_C_deg = DOUBLE_EPSILON;
    current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode = DOUBLE_EPSILON;
    current_pelec_including_loss_per_solar_cell_in_W = DOUBLE_EPSILON;
    current_Vdc_at_mppt_per_solar_cell_in_V = DOUBLE_EPSILON;
    capacitor.set_limiter_type(NO_LIMITER);
    Vdc_controller.set_limiter_type(NO_LIMITER);

}

void PVPNLY::set_current_S_in_Wpm2_and_T_in_C_deg(double S, double T)
{
    if(fabs(get_current_S_in_Wpm2()-S)<DOUBLE_EPSILON and fabs(get_current_T_in_C_deg()-T)<DOUBLE_EPSILON)
        return;

    this->current_S_in_Wpm2 = S;
    this->current_T_in_C_deg = T;
    update_current_Vdc_at_mppt_per_solar_cell_in_V(S,T);
}

void PVPNLY::set_current_pelec_including_loss_per_solar_cell_in_W(double value)
{
    if(fabs(get_current_pelec_including_loss_per_solar_cell_in_W()-value)<DOUBLE_EPSILON)
        return;

    this->current_pelec_including_loss_per_solar_cell_in_W = value;

    update_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(value);
}

void PVPNLY::set_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(double value)
{
    this->current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode = value;
}

void PVPNLY::set_current_Vdc_at_mppt_per_solar_cell_in_V(double value)
{
    this->current_Vdc_at_mppt_per_solar_cell_in_V = value;
}

double PVPNLY::get_current_S_in_Wpm2() const
{
    return this->current_S_in_Wpm2;
}

double PVPNLY::get_current_T_in_C_deg() const
{
    return this->current_T_in_C_deg;
}

double PVPNLY::get_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode() const
{
    return this->current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode;
}

double PVPNLY::get_current_pelec_including_loss_per_solar_cell_in_W() const
{
    return this->current_pelec_including_loss_per_solar_cell_in_W;
}

double PVPNLY::get_current_Vdc_at_mppt_per_solar_cell_in_V() const
{
    return this->current_Vdc_at_mppt_per_solar_cell_in_V;
}

void PVPNLY::set_dc_voltage_control_mode(PV_DC_VOLTAGE_MODE mode)
{
    this->mode = mode;
}

void PVPNLY::set_Im_in_A_per_solar_cell(double I)
{
    this->Im_in_A_per_solar_cell = I;
}

void PVPNLY::set_Isc_in_A_per_solar_cell(double I)
{
    this->Isc_in_A_per_solar_cell = I;
}

void PVPNLY::set_Vm_in_V_per_solar_cell(double v)
{
    this->Vm_in_V_per_solar_cell = v;
}

void PVPNLY::set_Voc_in_V_per_solar_cell(double v)
{
    this->Voc_in_V_per_solar_cell = v;
}

void PVPNLY::set_a(double value)
{
    this->a = value;
}

void PVPNLY::set_b(double value)
{
    this->b = value;
}

void PVPNLY::set_c(double value)
{
    this->c = value;
}

void PVPNLY::set_eta(double value)
{
    this->eta = value;
}

void PVPNLY::set_Nse(unsigned int value)
{
    this->Nse = value;
}

void PVPNLY::set_Nsh(unsigned int value)
{
    this->Nsh = value;
}

void PVPNLY::set_Smax_in_Wpm2(double value)
{
    this->Smax_in_Wpm2 = value;
}

void PVPNLY::set_C_in_F(double value)
{
    capacitor.set_T_in_s(value);
}

void PVPNLY::set_Kpdc(double value)
{
    Vdc_controller.set_Kp(value);
}

void PVPNLY::set_Kidc(double value)
{
    Vdc_controller.set_Ki(value);
}

void PVPNLY::set_Vdcmin_per_solar_cell_in_V(double value)
{
    this->Vdcmin_per_solar_cell_in_V = value;
}

void PVPNLY::set_Vdcmax_per_solar_cell_in_V(double value)
{
    this->Vdcmax_per_solar_cell_in_V = value;
}

PV_DC_VOLTAGE_MODE PVPNLY::get_dc_voltage_control_mode() const
{
    return this->mode;
}

double PVPNLY::get_Im_in_A_per_solar_cell() const
{
    return this->Im_in_A_per_solar_cell;
}

double PVPNLY::get_Isc_in_A() const
{
    return this->Isc_in_A_per_solar_cell;
}

double PVPNLY::get_Vm_in_V_per_solar_cell() const
{
    return this->Vm_in_V_per_solar_cell;
}

double PVPNLY::get_Voc_in_V_per_solar_cell() const
{
    return this->Voc_in_V_per_solar_cell;
}

double PVPNLY::get_a() const
{
    return this->a;
}

double PVPNLY::get_b() const
{
    return this->b;
}

double PVPNLY::get_c() const
{
    return this->c;
}

double PVPNLY::get_eta() const
{
    return this->eta;
}

unsigned int PVPNLY::get_Nse() const
{
    return this->Nse;
}

unsigned int PVPNLY::get_Nsh() const
{
    return this->Nsh;
}

double PVPNLY::get_Smax_in_Wpm2() const
{
    return this->Smax_in_Wpm2;
}

double PVPNLY::get_C_in_F() const
{
    return capacitor.get_T_in_s();
}

double PVPNLY::get_Kpdc() const
{
    return Vdc_controller.get_Kp();
}

double PVPNLY::get_Kidc() const
{
    return Vdc_controller.get_Ki();
}

double PVPNLY::get_Vdcmin_per_solar_cell_in_V() const
{
    return this->Vdcmin_per_solar_cell_in_V;
}

double PVPNLY::get_Vdcmax_per_solar_cell_in_V() const
{
    return this->Vdcmax_per_solar_cell_in_V;
}

double PVPNLY::get_Pref_in_pu_base_on_mbase()
{
    double Pref = Vdc_controller.get_output();
    return Pref;
}

double PVPNLY::get_maximum_available_power_extracted_power_from_irradiance_in_pu_on_mbase()
{
    double S = get_solar_irradiance_in_Wpm2();
    double T = get_temperature_in_C_deg();
    double p_cell = get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(S,T);
    return p_cell*get_Nse()*get_Nsh()*get_number_of_lumped_pv_units()/1e6/get_mbase_in_MVA();
}

string PVPNLY::get_model_name() const
{
    return "PVPNLY";
}

void PVPNLY::set_initial_Vdc_per_solar_cell_in_V(double v)
{
    this->initial_Vdc_per_solar_cell_in_V = v;
}

double PVPNLY::get_initial_Vdc_per_solar_cell_in_V() const
{
    return this->initial_Vdc_per_solar_cell_in_V;
}

double PVPNLY::get_initial_Vdc_in_kV_per_pv_unit()
{
    double Vdc_per_solar_cell_in_V = get_initial_Vdc_per_solar_cell_in_V();
    unsigned int Nse = get_Nse();
    return Vdc_per_solar_cell_in_V*Nse/1e3;
}

double PVPNLY::get_derivative_of_power_extracted_power_from_irradiance_over_dc_voltage(double V, double S, double T)
{
    double P0 = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(V, S, T);
    double P1 = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(V+1e-4, S, T);
    return (P1-P0)*1e4;
}

double PVPNLY::get_Vdc_in_kV_per_pv_unit()
{
    return capacitor.get_output();
}

double PVPNLY::get_Vdc_per_solar_cell_in_V()
{
    return get_Vdc_in_kV_per_pv_unit()*1e3/get_Nse();
}

double PVPNLY::get_Ipv_per_solar_cell_in_A(double V, double S, double T)
{
    if(fabs(S)<=DOUBLE_EPSILON)
       return 0.0;

    double one_over_Sref = 1.0/get_Sref_in_Wpm2();
    double delta_S = S*one_over_Sref-1.0;
    double delta_T = T - get_Tref_in_C_deg();
    double Ichange = S*(1.0+get_a()*delta_T)*one_over_Sref;
    double Isc_new = get_Isc_in_A()* Ichange;
    double Im_new = get_Im_in_A_per_solar_cell()*Ichange;
    double Vchange = (1.0-get_c()*delta_T)*log(exp(1.0)+get_b()*delta_S);
    double Voc_new = get_Voc_in_V_per_solar_cell()* Vchange;
    double Vm_new = get_Vm_in_V_per_solar_cell()* Vchange;
    double C2 = (Vm_new/Voc_new-1.0)/log(1.0-Im_new/Isc_new);
    double C1 = (1.0-Im_new/Isc_new)*exp(-Vm_new/C2/Voc_new);
    double Ipv = Isc_new*(1.0-C1*(exp(V/C2/Voc_new)-1.0));

    if(Ipv<0)
        Ipv = 0.0;

    return Ipv;
}

double PVPNLY::get_Ipv_in_kA_per_pv_unit(double V, double S, double T)
{
    double Ipv_per_solar_cell_in_A = get_Ipv_per_solar_cell_in_A(V,S,T);
    unsigned int Nsh = get_Nsh();
    return Ipv_per_solar_cell_in_A*Nsh/1e3;
}

double PVPNLY::get_power_extracted_power_from_irradiance_per_solar_cell_in_W(double V, double S, double T)
{
    double Ipv = get_Ipv_per_solar_cell_in_A(V, S, T);
    return Ipv*V;
}

double PVPNLY::get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(double S, double T)
{
    double Vdc_at_mppt = get_Vdc_at_mppt_per_solar_cell_in_V(S, T);
    return get_power_extracted_power_from_irradiance_per_solar_cell_in_W(Vdc_at_mppt, S, T);
}

void PVPNLY::update_current_Vdc_at_mppt_per_solar_cell_in_V(double S, double T)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double v_low = get_Vm_in_V_per_solar_cell()*(1.0-get_c()*(T-get_Tref_in_C_deg()))*log(exp(1.0)+get_b()*(S/get_Sref_in_Wpm2()-1.0));
    double v_high = get_Vm_in_V_per_solar_cell()*(1.0-get_c()*(T-get_Tref_in_C_deg()))*log(exp(1.0)+get_b()*(S/get_Sref_in_Wpm2()-1.0));
    double v_step =  get_Voc_in_V_per_solar_cell()/100;

    unsigned int iter_count = 0, iter_max = 100;
    while(true)
    {
        double der = get_derivative_of_power_extracted_power_from_irradiance_over_dc_voltage(v_low, S, T);
        if(der<=0.0)
        {
            if(der==0.0)
                v_low += v_step;
            else
                v_low -= v_step;
        }
        else
            break;
        ++iter_count;
        if(iter_count<=iter_max)
            ;
        else
        {
            osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
    iter_count=0;
    while(true)
    {
        double der = get_derivative_of_power_extracted_power_from_irradiance_over_dc_voltage(v_high, S, T);
        if(der>=0.0)
        {
            if(der==0.0)
                v_high -= v_step;
            else
                v_high += v_step;
        }
        else
            break;
        ++iter_count;
        if(iter_count<=iter_max)
            ;
        else
        {
            if(toolkit.is_detailed_log_enabled())
            {
                osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            break;
        }
    }

    double v_new = 0.0;
    iter_count=0;
    while(true)
    {
        v_new = 0.5*(v_low+v_high);
        double der_new = get_derivative_of_power_extracted_power_from_irradiance_over_dc_voltage(v_new, S, T);
        if(fabs(der_new)>DOUBLE_EPSILON)
        {
            if(der_new>0.0)
                v_low = v_new;
            else
                v_high = v_new;

            ++iter_count;
            if(iter_count<=iter_max)
                ;
            else
            {

                break;
            }

        }
        else
            break;
    }

    set_current_Vdc_at_mppt_per_solar_cell_in_V(v_new);

}

void PVPNLY::update_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(double P)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double pelec = P;

    double S = get_current_S_in_Wpm2();
    double T = get_current_T_in_C_deg();

    double v_mppt = get_Vdc_at_mppt_per_solar_cell_in_V(S,T);

    double pmax = get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(S,T);

    if(pmax>=pelec)
    {
        // pmax>pelec

        double v_low = v_mppt;
        double v_high = v_mppt;
        double p_low, p_high;
        unsigned int iter_count =0, iter_max = 100;

        if(get_dc_voltage_control_mode()==PV_UNDERVOLTAGE_MODE)
        {
            v_high = v_mppt;
            double v_step = v_mppt/iter_max;
            iter_count = 0;
            while(true)
            {
                v_low = v_high - v_step;
                p_low = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(v_low,S,T);
                if(p_low>=pelec)
                    v_high = v_low;
                else
                    break;

                ++iter_count;
                if(iter_count<=iter_max)
                    ;
                else
                {

                    osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    break;
                }
            }
        }
        else
        {

            v_low = v_mppt;
            double v_step = fabs((2.0*get_Voc_in_V_per_solar_cell()-v_mppt)/iter_max);
            iter_count = 0;
            while(true)
            {
                v_high = v_low + v_step;
                p_high = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(v_high,S,T);
                if(p_high>=pelec)
                    v_low = v_high;
                else
                    break;
                ++iter_count;
                if(iter_count<=iter_max)
                    ;
                else
                {
                    osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    break;
                }
            }


        }

        double v = 0.0;
        iter_count = 0;
        while(true)
        {
            double v_new = 0.5*(v_low+v_high);
            double p_new = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(v_new, S, T);
            if(fabs(p_new-pelec)>DOUBLE_EPSILON)
            {
                if(get_dc_voltage_control_mode()==PV_UNDERVOLTAGE_MODE)
                {
                    if(p_new>pelec)
                        v_high = v_new;
                    else
                        v_low = v_new;
                }
                else
                {
                    if(p_new>pelec)
                        v_low = v_new;
                    else
                        v_high = v_new;
                }
                iter_count++;
                if(iter_count<=iter_max)
                    ;
                else
                {
                    v = v_new;
                    break;
                }
            }
            else
            {
                v = v_new;
                break;
            }
        }

        set_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(v);
    }
    else
    {
        set_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(v_mppt);
        return;
    }

}

double PVPNLY::get_Vdc_reference_per_solar_cell_in_V_without_limit(double S, double T, double P)
{
    if(get_dc_voltage_control_mode()==PV_MPPT_MODE)
    {
        return get_Vdc_at_mppt_per_solar_cell_in_V(S,T);
    }
    else
    {
        return get_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode_without_limit(S, T, P);
    }
}

double PVPNLY::get_Vdc_at_mppt_per_solar_cell_in_V(double S, double T)
{
    if(S==0.0)
        return get_Vm_in_V_per_solar_cell()*(1.0-get_c()*(T-get_Tref_in_C_deg()))*log(exp(1.0)+get_b()*(S/get_Sref_in_Wpm2()-1.0));

    if(fabs(get_current_S_in_Wpm2()-S)<DOUBLE_EPSILON and fabs(get_current_T_in_C_deg()-T)<DOUBLE_EPSILON)
        return get_current_Vdc_at_mppt_per_solar_cell_in_V();
    else
    {
        set_current_S_in_Wpm2_and_T_in_C_deg(S,T);//update mppt point
        return get_current_Vdc_at_mppt_per_solar_cell_in_V();
    }
}

double PVPNLY::get_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode_without_limit(double S, double T, double P)
{
    if(fabs(get_current_S_in_Wpm2()-S)<DOUBLE_EPSILON and fabs(get_current_T_in_C_deg()-T)<DOUBLE_EPSILON
       and fabs(get_current_pelec_including_loss_per_solar_cell_in_W()-P)<DOUBLE_EPSILON)
    {
        return get_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode();
    }
    else
    {
        set_current_S_in_Wpm2_and_T_in_C_deg(S,T);//update mppt point
        set_current_pelec_including_loss_per_solar_cell_in_W(P);//update over under Vdc ponit
        return get_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode();
    }
}

double PVPNLY::get_Vdc_reference_per_solar_cell_in_V(double S, double T, double P)
{
    double Vdcmax = get_Vdcmax_per_solar_cell_in_V();
    double Vdcmin = get_Vdcmin_per_solar_cell_in_V();
    double Vdc = get_Vdc_reference_per_solar_cell_in_V_without_limit(S,T,P);
    if(Vdc>Vdcmax)
        Vdc = Vdcmax;
    if(Vdc<Vdcmin)
        Vdc = Vdcmin;

    return Vdc;
}

 double PVPNLY::get_Vdc_reference_in_kV_per_pv_unit()
 {
     double S = get_solar_irradiance_in_Wpm2();
     double T = get_Tref_in_C_deg();
     double pelec_per_solar_cell_in_W = get_active_power_generation_including_stator_loss_in_MW()/get_number_of_lumped_pv_units()/get_Nse()/get_Nsh()/get_eta()*1e6;
     double Vdc_reference_per_solar_cell_in_V = get_Vdc_reference_per_solar_cell_in_V(S,T,pelec_per_solar_cell_in_W);
     unsigned int Nse = get_Nse();
     return Vdc_reference_per_solar_cell_in_V*Nse/1e3;
 }

bool PVPNLY::setup_model_with_steps_string_vector(vector<string>& data)
{

    bool is_successful = false;
    if(data.size()>=23)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            int mode;
            double Im, Isc, Vm, Voc, Sref, Tref,a, b, c, eta,
                    s0, smax, c_in_F, kidc, kpdc,vmax, vmin;
            unsigned int nse, nsh;

            unsigned int i=3;
            mode = get_integer_data(data[i],"0"); i++;
            Im = get_double_data(data[i],"0.0"); i++;
            Isc = get_double_data(data[i],"0.0"); i++;
            Vm = get_double_data(data[i],"0.0"); i++;
            Voc = get_double_data(data[i],"0.0"); i++;
            Sref = get_double_data(data[i],"0.0"); i++;
            Tref = get_double_data(data[i],"0.0"); i++;
            a = get_double_data(data[i],"0.0"); i++;
            b = get_double_data(data[i],"0.0"); i++;
            c = get_double_data(data[i],"0.0"); i++;
            eta = get_double_data(data[i],"0.0"); i++;
            nse = (unsigned int)(get_integer_data(data[i],"0")); i++;
            nsh = (unsigned int)(get_integer_data(data[i],"0")); i++;
            s0 = get_double_data(data[i],"0.0"); i++;
            smax = get_double_data(data[i],"0.0"); i++;
            c_in_F = get_double_data(data[i],"0.0"); i++;
            kpdc = get_double_data(data[i],"0.0"); i++;
            kidc = get_double_data(data[i],"0.0"); i++;
            vmin = get_double_data(data[i],"0.0"); i++;
            vmax = get_double_data(data[i],"0.0"); i++;


            PV_DC_VOLTAGE_MODE pv_mode;
            switch(mode)
            {
                case 0:
                {
                    pv_mode = PV_MPPT_MODE;
                    break;
                }
                case 1:
                {
                    pv_mode = PV_OVERVOLTAGE_MODE;
                    break;
                }
                case -1:
                {
                    pv_mode = PV_UNDERVOLTAGE_MODE;
                    break;
                }
                default:
                {
                    pv_mode = PV_MPPT_MODE;
                    break;
                }
            }

            set_dc_voltage_control_mode(pv_mode);
            set_Im_in_A_per_solar_cell(Im);
            set_Isc_in_A_per_solar_cell(Isc);
            set_Vm_in_V_per_solar_cell(Vm);
            set_Voc_in_V_per_solar_cell(Voc);
            set_Sref_in_Wpm2(Sref);
            set_Tref_in_C_deg(Tref);
            set_a(a);
            set_b(b);
            set_c(c);
            set_eta(eta);
            set_Nse(nse);
            set_Nsh(nsh);
            set_S0_in_Wpm2(s0);
            set_Smax_in_Wpm2(smax);
            set_C_in_F(c_in_F);
            set_Kpdc(kpdc);
            set_Kidc(kidc);
            set_Vdcmin_per_solar_cell_in_V(vmin);
            set_Vdcmax_per_solar_cell_in_V(vmax);



            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool PVPNLY::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PVPNLY::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}

void PVPNLY::setup_block_toolkit_and_parameters()
{
    double Im = get_Im_in_A_per_solar_cell();
    double Isc = get_Isc_in_A();
    double Voc = get_Voc_in_V_per_solar_cell();
    double Vm = get_Vm_in_V_per_solar_cell();
    double eta = get_eta();
    double s0 = get_S0_in_Wpm2();
    double smax = get_Smax_in_Wpm2();
    double vmax = get_Vdcmax_per_solar_cell_in_V();
    double vmin = get_Vdcmin_per_solar_cell_in_V();
    unsigned int nse = get_Nse();
    unsigned int nsh = get_Nsh();

    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(Im<=0)
    {
        osstream<<"Error. The parameter Im ( = "<<Isc<<" A ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(Isc<=0)
    {
        osstream<<"Error. The parameter Isc ( = "<<Isc<<" A ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(Voc<=0)
    {
        osstream<<"Error. The parameter Voc ( = "<<Voc<<" V ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(Vm<=0)
    {
        osstream<<"Error. The parameter Vm ( = "<<Vm<<" V ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(Isc<=Im)
    {
        osstream<<"Error. The parameter Isc ( = "<<Isc<<" A ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than parameter Im ( = "<<Im<<" A ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(Voc<=Vm)
    {
        osstream<<"Error. The parameter Voc ( = "<<Voc<<" V ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than parameter Vm ( = "<<Vm<<" V ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(eta>1.0)
    {
        osstream<<"Error. The parameter eta ( = "<<eta<<" ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is larger than 1.0.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(s0<0)
    {
        osstream<<"Error. The parameter S0 ( = "<<s0<<" W/m2 ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(smax<0)
    {
        osstream<<"Error. The parameter Smax ( = "<<smax<<" W/m2 ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(nse<0)
    {
        osstream<<"Error. The parameter Nse ( = "<<nse<<" ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(nsh<0)
    {
        osstream<<"Error. The parameter Nsh ( = "<<nsh<<" ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is smaller than Zero.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(s0>smax)
    {
        if(get_dc_voltage_control_mode() != PV_MPPT_MODE)
        {
            osstream<<"Error. The parameter S0 ( = "<<s0<<" W/m2 ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is larger than parameter Smax ( = "<<smax<<" W/m2 ).";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    if(vmax<vmin)
    {
        osstream<<"Error. The parameter Vdcmax ( = "<<vmax<<" V ) in the "<<get_model_name()<<" model of "<<get_compound_device_name()<<" is less than parameter Vdcmin ( = "<<vmin<<" V ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void PVPNLY::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<"Start initialize "<<get_model_name()<<" model of "<<get_compound_device_name()<<" in ";
            if(get_dc_voltage_control_mode()==PV_MPPT_MODE)
                osstream<<"PV MPPT MODE.";
            else
            {
                if(get_dc_voltage_control_mode()==PV_UNDERVOLTAGE_MODE)
                    osstream<<"PV UNDERVOLTAGE MODE.";
                else
                    osstream<<"PV OVERVOLTAGE MODE.";
            }
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        setup_block_toolkit_and_parameters();

        double T = get_temperature_in_C_deg();

        double pelec = get_active_power_generation_including_stator_loss_in_MW()/get_number_of_lumped_pv_units()/get_Nse()/get_Nsh()/get_eta()*1e6;

        double pmax = get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(get_Smax_in_Wpm2(), T);

        if(pmax<pelec)
        {
            osstream<<"Error. Maximum available power at Smax ( = "<<get_Smax_in_Wpm2()<<" W/m2 ) per solar pell is "<<pmax<<" W and it is less than required power ( = "<<pelec<<" W )."<<endl
            <<"Initialization of "<<get_model_name()<<" of "<<get_compound_device_name()<<" failed.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            set_S0_in_Wpm2(get_Smax_in_Wpm2());
            set_initial_Vdc_per_solar_cell_in_V(get_Vdc_at_mppt_per_solar_cell_in_V(get_Smax_in_Wpm2(),T));
        }
        else
        {
            switch(get_dc_voltage_control_mode())
            {
                case PV_MPPT_MODE:
                {
                    initialize_S0_in_Wpm2_and_Vdc_in_V_per_solar_cell_with_mppt_mode(T);
                    break;
                }
                case PV_UNDERVOLTAGE_MODE:
                case PV_OVERVOLTAGE_MODE:
                {
                    double S0 = get_S0_in_Wpm2();
                    initialize_Vdc_in_V_per_solar_cell_with_under_or_over_Vdc_mode(S0, T ,pelec);
                    break;
                }
            }
        }

        double pcmd = get_pv_unit_terminal_active_power_generation_in_pu_based_on_mbase();
        Vdc_controller.set_output(pcmd);
        Vdc_controller.initialize();

        double Vdc0 = get_initial_Vdc_in_kV_per_pv_unit();
        capacitor.set_output(Vdc0);
        capacitor.initialize();

        set_flag_model_initialized_as_true();
        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl;

            double Vdc0_in_V = get_initial_Vdc_per_solar_cell_in_V();
            double S0_in_Wpm2 = get_S0_in_Wpm2();
            double der = get_derivative_of_power_extracted_power_from_irradiance_over_dc_voltage(Vdc0_in_V, S0_in_Wpm2, T);
            double Ipv0 = get_Ipv_per_solar_cell_in_A(Vdc0_in_V, S0_in_Wpm2,T);
            double P0 = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(Vdc0_in_V, S0_in_Wpm2,T);


            osstream<<"Initial irradiance = "<<S0_in_Wpm2<<" W/m2."<<endl;
            osstream<<"Initial Vdc per solar cell = "<<Vdc0_in_V<<" V."<<endl;
            osstream<<"Initial Ipv per solar cell = "<<Ipv0<<" A."<<endl;
            osstream<<"Initial power extracted from irradiance per solar cell = "<<P0<<" W."<<endl;
            osstream<<"The initial operating point is located at the ";
            if(fabs(der)<1e-5)
                osstream<<"MPPT point";
            else
            {
                if(der>0.0)
                    osstream<<"ascending side";
                else
                    osstream<<"descending side";
            }

            osstream<<" of Power-Vdc characteristic curve. The near OPs are: "<<endl;

            for(double v = Vdc0_in_V-0.2; v<Vdc0_in_V+0.21; v+=0.1)
            {
                double p = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(v, S0_in_Wpm2, T);
                osstream<<v<<" V, "<<p<<" W "<<endl;
            }
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void PVPNLY::initialize_S0_in_Wpm2_and_Vdc_in_V_per_solar_cell_with_mppt_mode(double T)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    double Smax = get_Smax_in_Wpm2();

    double s_low = 0.0, s_high = 0.0;
    double s_step = Smax/100.0;

    double pelec = get_active_power_generation_including_stator_loss_in_MW()/get_number_of_lumped_pv_units()/get_eta()/get_Nse()/get_Nsh()*1e6;

    if(pelec>FLOAT_EPSILON)
    {
        unsigned int iter_max = 100;
        unsigned int iter = 0;

        while(true)
        {
            s_high = s_low+s_step;
            double pmax_high = get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(s_high, T);
            if(pmax_high<=pelec)
            {
                s_low = s_high;
            }
            else
                break;
            ++iter;
            if(iter>iter_max)
            {
                osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
        iter = 0;
        while(true)
        {
            double s = 0.5*(s_low+s_high);
            double pmax = get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(s, T);
            if(fabs(pelec-pmax)>FLOAT_EPSILON)
            {
                if(pmax>pelec)
                    s_high = s;
                else
                    s_low = s;

                ++iter;
                if(iter>iter_max)
                {
                    set_S0_in_Wpm2(s);
                    set_initial_Vdc_per_solar_cell_in_V(get_Vdc_at_mppt_per_solar_cell_in_V(s,T));

                    osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    break;
                }
            }
            else
            {
                set_S0_in_Wpm2(s);
                set_initial_Vdc_per_solar_cell_in_V(get_Vdc_at_mppt_per_solar_cell_in_V(s,T));
                break;
            }
        }
    }
    else
    {
        set_S0_in_Wpm2(s_low);
        set_initial_Vdc_per_solar_cell_in_V(get_Vdc_at_mppt_per_solar_cell_in_V(s_low,T));
    }
    double Vdc0 = get_initial_Vdc_per_solar_cell_in_V();
    double Vdcmax = get_Vdcmax_per_solar_cell_in_V();
    double Vdcmin = get_Vdcmin_per_solar_cell_in_V();
    if(Vdc0>Vdcmax)
    {
        osstream<<"Initialized irradiance is "<<get_S0_in_Wpm2()<<" W/m2."<<endl
        <<"The initialized Vdc per solar cell is "<<Vdc0<<" V, greater than parameter Vdcmax = "<<Vdcmax<<" V."<<endl
        <<"Initial Vdc per solar cell will be set as Vdcmax ( = "<<Vdcmax<<" V ) and reinitialize irradiance at Vdcmax ( = "<<Vdcmax<<" V ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_initial_Vdc_per_solar_cell_in_V(Vdcmax);
        initialize_S0_in_Wpm2(Vdcmax, T);
        return;

    }
    if(Vdc0<Vdcmin)
    {
        osstream<<"Initialized irradiance is "<<get_S0_in_Wpm2()<<" W/m2."<<endl
        <<"The initialized Vdc per solar cell is "<<Vdc0<<" V, less than parameter Vdcmin = "<<Vdcmin<<" V."<<endl
        <<"Initial Vdc per solar cell will be set as Vdcmin ( = "<<Vdcmin<<" V ) and reinitialize irradiance at Vdcmin ( = "<<Vdcmin<<" V ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_initial_Vdc_per_solar_cell_in_V(Vdcmin);
        initialize_S0_in_Wpm2(Vdcmin, T);
        return;

    }

}

void PVPNLY::initialize_Vdc_in_V_per_solar_cell_with_under_or_over_Vdc_mode(double S, double T, double P)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(toolkit.is_detailed_log_enabled())
    {

        osstream<<"Try to initialize Vdc with parameter S0 = "<<get_S0_in_Wpm2()<<" W/m2.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    double Vdc0 = get_Vdc_reference_per_solar_cell_in_V_without_limit(S,T,P);

    set_initial_Vdc_per_solar_cell_in_V(Vdc0);

    double Vdcmax = get_Vdcmax_per_solar_cell_in_V();
    double Vdcmin = get_Vdcmin_per_solar_cell_in_V();
    if(Vdc0<=Vdcmax and Vdc0>=Vdcmin)
    {
        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<"Initialized Vdc = "<<Vdc0<<" V is ok. ";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        set_initial_Vdc_per_solar_cell_in_V(Vdc0);
        return;
    }

    if(Vdc0>Vdcmax)
    {
        osstream<<"The initialized Vdc per solar cell at S0( = "<<get_S0_in_Wpm2()<<" W/m2 ) is "<<Vdc0<<" V, greater than Vdcmax = "<<Vdcmax<<" V."<<endl
                <<"Initial Vdc per solar cell will be set as Vdcmax ( = "<<Vdcmax<<" V ) and reinitialize irradiance at Vdcmax ( = "<<Vdcmax<<" V ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_initial_Vdc_per_solar_cell_in_V(Vdcmax);
        initialize_S0_in_Wpm2(Vdcmax, T);

    }
    if(Vdc0<Vdcmin)
    {
        osstream<<"The initialized Vdc per solar cell S0( = "<<get_S0_in_Wpm2()<<" W/m2 ) is "<<Vdc0<<" V, less than Vdcmin = "<<Vdcmin<<" V."<<endl
                <<"Initial Vdc per solar cell will be set as Vdcmin ( = "<<Vdcmin<<" V ) and reinitialize irradiance at Vdcmin ( = "<<Vdcmin<<" V ).";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_initial_Vdc_per_solar_cell_in_V(Vdcmin);
        initialize_S0_in_Wpm2(Vdcmin, T);
    }

}

void PVPNLY::initialize_S0_in_Wpm2(double Vdc0, double T)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double pelec = get_active_power_generation_including_stator_loss_in_MW()/get_number_of_lumped_pv_units()/get_Nse()/get_Nsh()/get_eta()*1e6;

    double s_low = 0.0, s_high = 0.0;
    double s_step = get_Smax_in_Wpm2()/100;

    double Vdcmax = get_Vdcmax_per_solar_cell_in_V();
    double Vdcmin = get_Vdcmin_per_solar_cell_in_V();

    double psolar_Smax = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(get_initial_Vdc_per_solar_cell_in_V(), get_Smax_in_Wpm2(), T);

    if(psolar_Smax<pelec)
    {
        osstream<<"Error. Failed to initialize irradiance because Maximum available power per solar cell at ";

        if(fabs(Vdc0-Vdcmax)<=DOUBLE_EPSILON)
            osstream<<"Vdcmax ( = "<<Vdcmax<<" V ) ";
        if(fabs(Vdc0-Vdcmin)<=DOUBLE_EPSILON)
            osstream<<"Vdcmin ( = "<<Vdcmin<<" V ) ";

        osstream<<"less than required power ( = "<<pelec<<" W ). "<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_S0_in_Wpm2(get_Smax_in_Wpm2());
        return;
    }


    double psolar = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(get_initial_Vdc_per_solar_cell_in_V(), s_low, T);

    if(fabs(pelec-psolar)>FLOAT_EPSILON)
    {
        unsigned int iter_count = 0, iter_max = 100;
        while(true)
        {
            s_high = s_low + s_step;
            psolar = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(get_initial_Vdc_per_solar_cell_in_V(), s_high, T);

            if(psolar<pelec)
                s_low = s_high;
            else
                break;
            ++iter_count;
            if(iter_count<=iter_max)
                ;
            else
            {
                osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }

        iter_count = 0;
        while(true)
        {
            double s_new = 0.5*(s_low+s_high);
            double pnew = get_power_extracted_power_from_irradiance_per_solar_cell_in_W(get_initial_Vdc_per_solar_cell_in_V(), s_new, T);

            if(fabs(pnew-pelec)>FLOAT_EPSILON)
            {
                if(pnew>pelec)
                    s_high = s_new;
                if(pnew<pelec)
                    s_low = s_new;

                ++iter_count;
                if(iter_count<=iter_max)
                    ;
                else
                {
                    osstream<<"Error. Failed to get initial irradiance in "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    set_S0_in_Wpm2(s_new);
                    break;
                }
            }
            else
            {
                osstream<<"Initialized irradiance is "<<s_new<<" W/m2 at ";
                if(fabs(Vdc0-Vdcmax)<=DOUBLE_EPSILON)
                    osstream<<"Vdcmax = "<<Vdcmax<<" V.";
                if(fabs(Vdc0-Vdcmin)<=DOUBLE_EPSILON)
                    osstream<<"Vdcmin = "<<Vdcmin<<" V.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                set_S0_in_Wpm2(s_new);
                break;
            }
        }
    }
    else
    {
        osstream<<"Initialized irradiance is "<<s_low<<" W/m2 at ";
        if(fabs(Vdc0-Vdcmax)<=DOUBLE_EPSILON)
            osstream<<"Vdcmax = "<<Vdcmax<<" V.";
        if(fabs(Vdc0-Vdcmin)<=DOUBLE_EPSILON)
            osstream<<"Vdcmin = "<<Vdcmin<<" V.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_S0_in_Wpm2(s_low);
    }
}

void PVPNLY::run(DYNAMIC_MODE mode)
{

    double S = get_solar_irradiance_in_Wpm2();
    double T = get_temperature_in_C_deg();
    double Pelec_per_pv_unit_in_MW = get_pv_unit_terminal_active_power_generation_in_pu_based_on_mbase()*get_mbase_in_MVA()/get_number_of_lumped_pv_units();

    double Vdc_per_solar_cell_in_V = get_Vdc_per_solar_cell_in_V();
    double Vdc_in_kV_per_pv_unit = get_Vdc_in_kV_per_pv_unit();

    double Ipv_in_kA_per_pv_unit = get_Ipv_in_kA_per_pv_unit(Vdc_per_solar_cell_in_V, S, T);

    double input = Ipv_in_kA_per_pv_unit - Pelec_per_pv_unit_in_MW/Vdc_in_kV_per_pv_unit/get_eta();
    capacitor.set_input(input);
    capacitor.run(mode);

    input = capacitor.get_output() - get_Vdc_reference_in_kV_per_pv_unit();

    Vdc_controller.set_input(input);

    Vdc_controller.run(mode);


    if(mode==DYNAMIC_UPDATE_MODE)
    {
        set_flag_model_updated_as_true();
    }
}


void PVPNLY::check()
{
    ostringstream osstream;
}

void PVPNLY::report()
{
    ;
}

void PVPNLY::save()
{
    ;
}

string PVPNLY::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    PV_DC_VOLTAGE_MODE mode = get_dc_voltage_control_mode();
    int pv_mode = (mode==PV_MPPT_MODE)? 0: (mode==PV_UNDERVOLTAGE_MODE? -1 : 1);
    double Im = get_Im_in_A_per_solar_cell();
    double Isc = get_Isc_in_A();
    double Vm = get_Vm_in_V_per_solar_cell();
    double Voc = get_Voc_in_V_per_solar_cell();
    double Sref = get_Sref_in_Wpm2();
    double Tref = get_Tref_in_C_deg();
    double a = get_a();
    double b = get_b();
    double c = get_c();
    double eta = get_eta();
    unsigned int nse = get_Nse();
    unsigned int nsh = get_Nsh();
    double s0 = get_S0_in_Wpm2();
    double smax = get_Smax_in_Wpm2();
    double c_in_F = get_C_in_F();
    double kpdc = get_Kpdc();
    double kidc = get_Kidc();
    double vmin = get_Vdcmin_per_solar_cell_in_V();
    double vmax = get_Vdcmax_per_solar_cell_in_V();


    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<pv_mode<<", "
            <<setw(8)<<setprecision(6)<<Im<<", "
            <<setw(8)<<setprecision(6)<<Isc<<", "
            <<setw(8)<<setprecision(6)<<Vm<<", "
            <<setw(8)<<setprecision(6)<<Voc<<", "
            <<setw(8)<<setprecision(6)<<Sref<<", "
            <<setw(8)<<setprecision(6)<<Tref<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<a<<", "
            <<setw(8)<<setprecision(6)<<b<<", "
            <<setw(8)<<setprecision(6)<<c<<", "
            <<setw(8)<<setprecision(6)<<eta<<", "
            <<setw(8)<<setprecision(6)<<nse<<", "
            <<setw(8)<<setprecision(6)<<nsh<<", "
            <<setw(8)<<setprecision(6)<<s0<<", "
            <<setw(8)<<setprecision(6)<<smax<<", "
            <<setw(8)<<setprecision(6)<<c_in_F<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<kpdc<<", "
            <<setw(8)<<setprecision(6)<<kidc<<", "
            <<setw(8)<<setprecision(6)<<vmin<<", "
            <<setw(8)<<setprecision(6)<<vmax<<" / ";

    return osstream.str();
}

void PVPNLY::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("PV DCVOLTAGE MODE", i); i++;
    add_model_data_name_and_index_pair("IM IN A", i); i++;
    add_model_data_name_and_index_pair("ISC IN A", i); i++;
    add_model_data_name_and_index_pair("VM IN V", i); i++;
    add_model_data_name_and_index_pair("VOC IN V", i); i++;
    add_model_data_name_and_index_pair("SREF IN W/M2", i); i++;
    add_model_data_name_and_index_pair("TREF IN DEG", i); i++;
    add_model_data_name_and_index_pair("A", i); i++;
    add_model_data_name_and_index_pair("B", i); i++;
    add_model_data_name_and_index_pair("C", i); i++;
    add_model_data_name_and_index_pair("INVERTER EFFICIENCY", i); i++;
    add_model_data_name_and_index_pair("NSE", i); i++;
    add_model_data_name_and_index_pair("NSH", i); i++;
    add_model_data_name_and_index_pair("S0 IN W/M2", i); i++;
    add_model_data_name_and_index_pair("SMAX IN W/M2", i); i++;
    add_model_data_name_and_index_pair("C IN F", i); i++;
    add_model_data_name_and_index_pair("KPDC", i); i++;
    add_model_data_name_and_index_pair("KIDC", i); i++;
    add_model_data_name_and_index_pair("VMIN IN V", i); i++;
    add_model_data_name_and_index_pair("VMAX IN V", i); i++;
}

double PVPNLY::get_model_data_with_name(string par_name) const
{
    if(par_name=="PV DCVOLTAGE MODE")
    {
        PV_DC_VOLTAGE_MODE mode = get_dc_voltage_control_mode();
        switch(mode)
        {
            case PV_OVERVOLTAGE_MODE:
                return 1;
            case PV_UNDERVOLTAGE_MODE:
                return -1;
            case PV_MPPT_MODE:
            default:
                return 0;
        }
    }

    if(par_name=="IM IN A")
        return get_Im_in_A_per_solar_cell();
    if(par_name=="ISC IN A")
        return get_Isc_in_A();
    if(par_name=="VM IN V")
        return get_Vm_in_V_per_solar_cell();
    if(par_name=="VOC IN V")
        return get_Voc_in_V_per_solar_cell();
    if(par_name=="SREF IN W/M2")
        return get_Sref_in_Wpm2();
    if(par_name=="TREF IN DEG")
        return get_Tref_in_C_deg();
    if(par_name=="A")
        return get_a();
    if(par_name=="B")
        return get_b();
    if(par_name=="C")
        return get_c();
    if(par_name=="INVERTER EFFICIENCY")
        return get_eta();
    if(par_name=="NSE")
        return get_Nse();
    if(par_name=="NSH")
        return get_Nsh();
    if(par_name=="S0 IN W/M2")
        return get_S0_in_Wpm2();
    if(par_name=="SMAX IN W/M2")
        return get_Smax_in_Wpm2();
    if(par_name=="C IN F")
        return get_C_in_F();
    if(par_name=="KPDC")
        return get_Kpdc();
    if(par_name=="KIDC")
        return get_Kidc();
    if(par_name=="VMIN IN V")
        return get_Vdcmin_per_solar_cell_in_V();
    if(par_name=="VMAX IN V")
        return get_Vdcmax_per_solar_cell_in_V();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void PVPNLY::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name=="PV DCVOLTAGE MODE")
        {
            int ivalue = int(value);
            PV_DC_VOLTAGE_MODE mode;
            switch(ivalue)
            {
                case 1:
                    mode = PV_OVERVOLTAGE_MODE;
                    break;
                case -1:
                    mode = PV_UNDERVOLTAGE_MODE;
                    break;
                case 0:
                default:
                    mode = PV_MPPT_MODE;
                    break;
            }
            set_dc_voltage_control_mode(mode);
            return;
        }
        if(par_name=="IM IN A")
            return set_Im_in_A_per_solar_cell(value);
        if(par_name=="ISC IN A")
            return set_Isc_in_A_per_solar_cell(value);
        if(par_name=="VM IN V")
            return set_Vm_in_V_per_solar_cell(value);
        if(par_name=="VOC IN V")
            return set_Voc_in_V_per_solar_cell(value);
        if(par_name=="SREF IN W/M2")
            return set_Sref_in_Wpm2(value);
        if(par_name=="TREF IN DEG")
            return set_Tref_in_C_deg(value);
        if(par_name=="A")
            return set_a(value);
        if(par_name=="B")
            return set_b(value);
        if(par_name=="C")
            return set_c(value);
        if(par_name=="INVERTER EFFICIENCY")
            return set_eta(value);
        if(par_name=="NSE")
            return set_Nse((unsigned int)(value));
        if(par_name=="NSH")
            return set_Nsh((unsigned int)(value));
        if(par_name=="S0 IN W/M2")
            return set_S0_in_Wpm2(value);
        if(par_name=="SMAX IN W/M2")
            return set_Smax_in_Wpm2(value);
        if(par_name=="C IN F")
            return set_C_in_F(value);
        if(par_name=="KPDC")
            return set_Kpdc(value);
        if(par_name=="KIDC")
            return set_Kidc(value);
        if(par_name=="VMIN IN V")
            return set_Vdcmin_per_solar_cell_in_V(value);
        if(par_name=="VMAX IN V")
            return set_Vdcmax_per_solar_cell_in_V(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double PVPNLY::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void PVPNLY::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("IPV PER SOLAR CELL IN A", i); i++;
    add_model_internal_variable_name_and_index_pair("POWER PER SOLAR CELL IN W", i); i++;
    add_model_internal_variable_name_and_index_pair("VDC PER SOLAR CELL IN V", i); i++;
    add_model_internal_variable_name_and_index_pair("VDC REFERENCE PER SOLAR CELL IN V", i); i++;
    add_model_internal_variable_name_and_index_pair("VDC PER SOLAR CELL AT MPPT IN V", i); i++;
}

double PVPNLY::get_model_internal_variable_with_name(string var_name)
{
    double V = get_Vdc_per_solar_cell_in_V();
    double S = get_solar_irradiance_in_Wpm2();
    double T = get_temperature_in_C_deg();
    double P = get_active_power_generation_including_stator_loss_in_MW()/get_number_of_lumped_pv_units()/get_Nse()/get_Nsh()/get_eta()*1e6;
    var_name = string2upper(var_name);
    if(var_name == "IPV PER SOLAR CELL IN A")
        return get_Ipv_per_solar_cell_in_A(V,S,T);
    if(var_name == "POWER PER SOLAR CELL IN W")
        return get_power_extracted_power_from_irradiance_per_solar_cell_in_W(V,S,T);
    if(var_name == "VDC PER SOLAR CELL IN V")
        return get_Vdc_per_solar_cell_in_V();
    if(var_name == "VDC REFERENCE PER SOLAR CELL IN V")
        return get_Vdc_reference_per_solar_cell_in_V(S,T,P);
    if(var_name == "VDC PER SOLAR CELL AT MPPT IN V")
        return get_Vdc_at_mppt_per_solar_cell_in_V(S,T);

    return 0.0;
}

string PVPNLY::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PVPNLY::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PVPNLY::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
