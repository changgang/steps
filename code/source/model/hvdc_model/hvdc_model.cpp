#include "header/model/hvdc_model/hvdc_model.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <iostream>

using namespace std;
class HVDC;

HVDC_MODEL::HVDC_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("HVDC");
    set_converter_dynamic_max_alpha_or_gamma_in_deg(RECTIFIER, 90.0);
    set_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER, 0.0);
    set_converter_dynamic_max_alpha_or_gamma_in_deg(INVERTER, 90.0);
    set_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER, 0.0);

    time_when_unblocking = INFINITE_THRESHOLD;
    dc_current_recovered_after_unblocking = false;
    dc_voltage_recovered_after_unblocking = false;

    bypassed = false;
    clear_unbypassing_time();

    mode_switched = false;
    time_when_mode_unswitching = INFINITE_THRESHOLD;

    set_dc_voltage_command_recovery_rate_in_pu_per_second(0.0);
    set_dc_current_command_recovery_rate_in_pu_per_second(0.0);

    set_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing(0.0);
    set_minimum_dc_current_in_kA_following_unblocking(0.0);

    set_minimum_dc_current_command_in_kA(0.0);
    vdcol_limiter.clear();

    set_maximum_count_of_bypassing_before_blocked(999999);

    record_of_bypass_time.clear();

    manual_blocked = false;
    manual_bypassed = false;
}

HVDC_MODEL::~HVDC_MODEL()
{
    ;
}

HVDC* HVDC_MODEL::get_hvdc_pointer() const
{
    return (HVDC*) get_device_pointer();
}

string HVDC_MODEL::get_model_type() const
{
    return "HVDC";
}

double HVDC_MODEL::get_initial_alpha_in_deg() const
{
    ostringstream osstream;
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
    {
        osstream<<"Waring. HVDC dynamic model is not connected to any HVDC device. Initial alpha will be returned as 0.0.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
    return hvdc->get_converter_alpha_or_gamma_in_deg(RECTIFIER);
}

double HVDC_MODEL::get_initial_gamma_in_deg() const
{
    ostringstream osstream;
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
    {
        osstream<<"Waring. HVDC dynamic model is not connected to any HVDC device. Initial gamma will be returned as 0.0.";
        show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }
    return hvdc->get_converter_alpha_or_gamma_in_deg(INVERTER);
}

double HVDC_MODEL::get_auxiliary_signal_in_MW() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    AUXILIARY_SIGNAL_MODEL* auxiliary_signal_model = hvdc->get_auxiliary_signal_model();
    if(auxiliary_signal_model != NULL)
        return auxiliary_signal_model->get_auxiliary_signal_in_MW();
    else
        return 0.0;
}

void HVDC_MODEL::set_converter_dynamic_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, double angle)
{
    if(angle>90.0)
        angle = 90.0;
    if(angle<0.0)
        angle = 0.0;

    firing_angle_max[converter] = angle;
}

void HVDC_MODEL::set_converter_dynamic_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, double angle)
{
    if(angle>90.0)
        angle = 90.0;
    if(angle<0.0)
        angle = 0.0;

    firing_angle_min[converter] = angle;
}

double HVDC_MODEL::get_converter_dynamic_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    return firing_angle_max[converter];
}

double HVDC_MODEL::get_converter_dynamic_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter)  const
{
    return firing_angle_min[converter];
}

void HVDC_MODEL::set_dc_voltage_command_recovery_rate_in_pu_per_second(double rate)
{
    if(rate<0.0)
        rate = -rate;
    dc_voltage_command_recovery_rate_in_pu_per_second = rate;
}

void HVDC_MODEL::set_dc_current_command_recovery_rate_in_pu_per_second(double rate)
{
    if(rate<0.0)
        rate = -rate;
    dc_current_command_recovery_rate_in_pu_per_second = rate;
}

double HVDC_MODEL::get_dc_voltage_command_recovery_rate_in_pu_per_second() const
{
    return dc_voltage_command_recovery_rate_in_pu_per_second;
}

double HVDC_MODEL::get_dc_current_command_recovery_rate_in_pu_per_second() const
{
    return dc_current_command_recovery_rate_in_pu_per_second;
}


void HVDC_MODEL::set_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing(double V)
{
    if(V<0.0)
        V = -V;
    minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing = V;
}

void HVDC_MODEL::set_minimum_dc_current_in_kA_following_unblocking(double I)
{
    if(I<0.0)
        I = -I;
    minimum_dc_current_in_kA_following_unblocking = I;
}

void HVDC_MODEL::set_maximum_count_of_bypassing_before_blocked(size_t n)
{
    if(n==0)
        n = size_t(INFINITE_THRESHOLD);

    max_count_of_bypass_before_blocked = n;
}


void HVDC_MODEL::set_mininum_blocking_time_in_s(double t)
{
    block_timer.set_timer_interval_in_s(t);
}

void HVDC_MODEL::set_mininum_bypassing_time_in_s(double t)
{
    bypass_timer.set_timer_interval_in_s(t);
}

void HVDC_MODEL::set_minimum_time_in_switched_mode_in_s(double t)
{
    mode_switch_timer.set_timer_interval_in_s(t);
}

double HVDC_MODEL::get_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing() const
{
    return minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing;
}

double HVDC_MODEL::get_minimum_dc_current_in_kA_following_unblocking() const
{
    return minimum_dc_current_in_kA_following_unblocking;
}

void HVDC_MODEL::set_minimum_dc_current_command_in_kA(double I)
{
    if(I<0.0)
        I = -I;
    minimum_dc_current_command_in_kA = I;
}

double HVDC_MODEL::get_minimum_dc_current_command_in_kA() const
{
    return minimum_dc_current_command_in_kA;
}

double HVDC_MODEL::get_maximum_count_of_bypassing_before_blocked() const
{
    return max_count_of_bypass_before_blocked;
}

double HVDC_MODEL::get_mininum_blocking_time_in_s() const
{
    return block_timer.get_timer_interval_in_s();
}

double HVDC_MODEL::get_mininum_bypassing_time_in_s() const
{
    return bypass_timer.get_timer_interval_in_s();
}

double HVDC_MODEL::get_minimum_time_in_switched_mode_in_s() const
{
    return mode_switch_timer.get_timer_interval_in_s();
}

void HVDC_MODEL::set_VDCOL(const VDCOL& limiter)
{
    vdcol_limiter = limiter;
}

VDCOL HVDC_MODEL::get_VDCOL() const
{
    return vdcol_limiter;
}

void HVDC_MODEL::set_attached_device_of_common_meters()
{
    HVDC* hvdc = get_hvdc_pointer();
    block_timer.set_attached_device(hvdc);
    bypass_timer.set_attached_device(hvdc);
    mode_switch_timer.set_attached_device(hvdc);
}

double HVDC_MODEL::get_rectifier_dc_current_command_in_kA(double Vdci_measured, double Idc_measured)
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())// blocked
        return 0.0;

    double TIME = get_dynamic_simulation_time_in_s();

    //double Vset = hvdc->get_inverter_nominal_dc_voltage_command_in_kV();
    double Iset = hvdc->get_rectifier_nominal_dc_current_command_in_kA();
    double Pset = hvdc->get_rectifier_nominal_dc_power_command_in_MW();

    double Rdc = hvdc->get_line_resistance_in_ohm();
    //double Rcomp = hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm();

    double Icommand = Iset;

    double Vdcr_measured = Vdci_measured+Idc_measured*Rdc;

    double t_unblock =  get_unblocking_time();
    if(is_unblocking())
    {
        if(not dc_current_recovered_after_unblocking)
        {
            double delt_t = TIME - t_unblock;
            double i_rate = get_dc_current_command_recovery_rate_in_pu_per_second();
            double i_min = get_minimum_dc_current_in_kA_following_unblocking();
            Icommand = i_rate*delt_t*Iset+i_min;
            if(Icommand>Iset)
            {
                Icommand = Iset;
                clear_unblocking_time(RECTIFIER);
            }
        }
    }
    else
    {
        if(is_bypassed())
            return Iset;
        else
        {
            if(is_unbypassing())
                return Iset;
            else
            {
                double P_module = get_auxiliary_signal_in_MW();
                if(hvdc->get_converter_operation_mode(RECTIFIER)==RECTIFIER_CONSTANT_POWER)
                {
                    if(hvdc->get_side_to_hold_dc_power()==RECTIFIER)
                        Icommand = (Pset+P_module)/Vdcr_measured;
                    else
                        Icommand = (Pset+P_module)/Vdci_measured;
                }
                else
                {
                    Icommand = Iset+ (P_module/Vdcr_measured);
                }
            }
        }
    }

    double Imax = vdcol_limiter.get_vocol_maximum_current_command_in_kA_with_inverter_dc_voltage_in_kV(Vdci_measured);
    double Imin = get_minimum_dc_current_command_in_kA();

    if(Icommand>Imax) Icommand = Imax;
    if(Icommand<Imin) Icommand = Imin;

    return Icommand;
}


double HVDC_MODEL::get_inverter_dc_voltage_command_in_kV()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    //double Rcomp = hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm();
    double Vset = hvdc->get_inverter_nominal_dc_voltage_command_in_kV();
    double Vcommand = 0.0;

    if(is_blocked() or is_bypassed())// blocked or bypassed
        return Vcommand;

    double TIME = get_dynamic_simulation_time_in_s();

    double t_unblock =  get_unblocking_time();
    if(is_unblocking())
    {
        if(not dc_voltage_recovered_after_unblocking)
        {
            double delt_t = TIME - t_unblock;
            double v_rate = get_dc_voltage_command_recovery_rate_in_pu_per_second();
            double v_min = get_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing();
            Vcommand = v_rate*delt_t*Vset+v_min;
            if(Vcommand>Vset)
            {
                Vcommand = Vset;
                clear_unblocking_time(INVERTER);
            }
        }
        else
        {
            Vcommand = Vset;
        }
    }
    else
    {
        //double t_unbypass = get_unbypassing_time();
        if(is_unbypassing()) // unbypassing
        {

            Vcommand = Vset;
            clear_unbypassing_time();
            /*
            double delt_t = TIME - t_unbypass;
            double v_rate = get_dc_voltage_command_recovery_rate_in_pu_per_second();
            double v_min = get_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing();
            Vcommand = v_rate*delt_t*Vset+v_min;
            if(Vcommand>Vset)
            {
                Vcommand = Vset;
                clear_unbypassing_time();
            }
            */
        }
        else // mode switched or normal operation
            Vcommand = Vset;
    }
    return Vcommand;
}


void HVDC_MODEL::block_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    if(not is_blocked())
    {
        if(is_bypassed()) // if is bypassed, then clear bypass logic
        {
            unbypass_hvdc();
            clear_unbypassing_time();
            bypass_timer.reset();
        }
        if(is_mode_switched())
        {
            switch_hvdc_mode_back();
            mode_switch_timer.reset();
        }

        hvdc->set_status(false); // block hvdc
        //time_when_blocked = TIME; // set blocked time
        dc_current_recovered_after_unblocking = false; // set unblocking as false
        dc_voltage_recovered_after_unblocking = false;
        time_when_unblocking = INFINITE_THRESHOLD;

        block_timer.start();
        record_of_bypass_time.clear();

        osstream<<hvdc->get_device_name()<<" is blocked at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC_MODEL::unblock_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_manual_blocked())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    if(is_blocked() and is_block_timer_timed_out())
    {
        hvdc->set_status(true);
        //time_when_blocked = INFINITE_THRESHOLD;
        block_timer.reset();
        time_when_unblocking = TIME;

        osstream<<hvdc->get_device_name()<<" is unblocking at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}


void HVDC_MODEL::manual_block_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_manual_blocked())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    osstream<<get_device_name()<<" is manually blocked at time "<<TIME<<" s.";
    show_information_with_leading_time_stamp(osstream);

    if(not is_blocked())
        block_hvdc();

    manual_blocked = true;

}

void HVDC_MODEL::manual_unblock_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(not is_blocked())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    osstream<<get_device_name()<<" is manually unblocked at time "<<TIME<<" s.";
    show_information_with_leading_time_stamp(osstream);

    if(is_manual_blocked())
        manual_blocked = false;

    unblock_hvdc();
}

bool HVDC_MODEL::is_manual_blocked() const
{
    return manual_blocked;
}

bool HVDC_MODEL::is_blocked() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return false;

    return not hvdc->get_status();
}

bool HVDC_MODEL::is_unblocking() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return false;

    double t_unblock = get_unblocking_time();
    if(t_unblock!=INFINITE_THRESHOLD)
        return true;
    else
        return false;
}

double HVDC_MODEL::get_blocked_time() const
{
    return block_timer.get_time_when_started_in_s();
    //return time_when_blocked;
}

double HVDC_MODEL::get_unblocking_time() const
{
    return time_when_unblocking;
}

void HVDC_MODEL::clear_unblocking_time(HVDC_CONVERTER_SIDE converter)
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    if(converter==RECTIFIER)
    {
        dc_current_recovered_after_unblocking = true;

        osstream<<hvdc->get_device_name()<<"'s DC current command has been recovered from blocking at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        dc_voltage_recovered_after_unblocking = true;

        osstream<<hvdc->get_device_name()<<"'s DC voltage command has been recovered from blocking at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
    if(dc_current_recovered_after_unblocking and dc_voltage_recovered_after_unblocking)
    {
        time_when_unblocking = INFINITE_THRESHOLD;

        osstream<<hvdc->get_device_name()<<"'s DC current and voltage commands have been recovered from blocking at time "<<TIME<<" s."<<endl
          <<hvdc->get_device_name()<<" is fully unblocked.";
        show_information_with_leading_time_stamp(osstream);
    }
}

bool HVDC_MODEL::is_block_timer_timed_out() const
{
    return block_timer.is_timed_out();
}

void HVDC_MODEL::bypass_hvdc()
{
    ostringstream osstream;
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    if(not is_bypassed())
    {
        if(is_mode_switched())
        {
            switch_hvdc_mode_back();
            mode_switch_timer.reset();
        }

        bypassed = true;
        //time_when_bypassed = TIME;
        clear_unbypassing_time();
        bypass_timer.start();

        osstream<<hvdc->get_device_name()<<" is bypassed at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);

        record_of_bypass_time.push_back(TIME);
        if(record_of_bypass_time.size()>=get_maximum_count_of_bypassing_before_blocked())
        {
            osstream<<hvdc->get_device_name()<<" will be blocked at time "<<setprecision(5)<<fixed<<TIME
              <<" s since the max count of bypassing is reached.";
            show_information_with_leading_time_stamp(osstream);

            block_hvdc();
        }
    }
}

void HVDC_MODEL::unbypass_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_manual_bypassed())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    if(not is_blocked() and is_bypassed() and is_bypass_timer_timed_out())
    {
        bypassed = false;
        //time_when_bypassed = INFINITE_THRESHOLD;
        time_when_unbypassing = TIME;
        bypass_timer.reset();

        osstream<<hvdc->get_device_name()<<" is unbypassing at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC_MODEL::manual_bypass_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked() or is_bypassed())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    osstream<<get_device_name()<<" is manually bypassed at time "<<TIME<<" s.";
    show_information_with_leading_time_stamp(osstream);

    bypass_hvdc();
    manual_bypassed = true;
}

void HVDC_MODEL::manual_unbypass_hvdc()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    osstream<<get_device_name()<<" is manually unbypassed at time "<<TIME<<" s.";
    show_information_with_leading_time_stamp(osstream);

    if(is_manual_bypassed())
        manual_bypassed = false;

    unbypass_hvdc();
}

bool HVDC_MODEL::is_manual_bypassed() const
{
    return manual_bypassed;
}

bool HVDC_MODEL::is_bypassed() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return false;

    return bypassed;
}

bool HVDC_MODEL::is_unbypassing() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return false;

    double t_unbypass = get_unbypassing_time();
    if(t_unbypass!=INFINITE_THRESHOLD) // unbypassing
        return true;
    else
        return false;
}

double HVDC_MODEL::get_bypassed_time() const
{
    return bypass_timer.get_time_when_started_in_s();
    //return time_when_bypassed;
}

double HVDC_MODEL::get_unbypassing_time() const
{
    return time_when_unbypassing;
}

void HVDC_MODEL::clear_unbypassing_time()
{
    time_when_unbypassing = INFINITE_THRESHOLD;
}

bool HVDC_MODEL::is_bypass_timer_timed_out() const
{
    return bypass_timer.is_timed_out();
}

void HVDC_MODEL::switch_hvdc_mode()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(hvdc->get_converter_operation_mode(RECTIFIER)!=RECTIFIER_CONSTANT_POWER)
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    if((not is_blocked()) and (not is_bypassed()) and (not is_mode_switched()))
    {
        mode_switched = true;
        mode_switch_timer.start();

        osstream<<hvdc->get_device_name()<<" is mode switched at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void HVDC_MODEL::switch_hvdc_mode_back()
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(hvdc->get_converter_operation_mode(RECTIFIER)!=RECTIFIER_CONSTANT_POWER)
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    if((not is_blocked()) and (not is_bypassed()) and is_mode_switched() and is_mode_switch_timer_timed_out())
    {
        mode_switched = false;
        mode_switch_timer.reset();

        osstream<<hvdc->get_device_name()<<"'s mode is switched back at time "<<TIME<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

bool HVDC_MODEL::is_mode_switched() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return false;

    return mode_switched;
}

double HVDC_MODEL::get_mode_switched_time() const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return INFINITE_THRESHOLD;

    return mode_switch_timer.get_time_when_started_in_s();
}

bool HVDC_MODEL::is_mode_switch_timer_timed_out() const
{
    return mode_switch_timer.is_timed_out();
}

void HVDC_MODEL::solve_hvdc_model_without_line_dynamics(double Iset_kA, double Vset_kV)
{
    double Iset_kA_for_bypass = Iset_kA;

    ostringstream osstream;

    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked())
        return;

    double TIME = get_dynamic_simulation_time_in_s();

    //osstream<<"solving "<<get_device_name()<<" with I command = "<<Iset_kA<<"kA, V command = "<<Vset_kV;
    //show_information_with_leading_time_stamp(osstream);

    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();

    size_t bus_r = hvdc->get_converter_bus(RECTIFIER);
    size_t bus_i = hvdc->get_converter_bus(INVERTER);

    double alpha_min_in_rad = deg2rad(get_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER));
    //double alpha_max_in_rad = deg2rad(get_converter_dynamic_max_alpha_or_gamma_in_deg(RECTIFIER));
    double gamma_min_in_rad = deg2rad(get_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER));
    double gamma_max_in_rad = deg2rad(get_converter_dynamic_max_alpha_or_gamma_in_deg(INVERTER));
    if(hvdc->get_converter_max_alpha_or_gamma_in_deg(INVERTER)<=hvdc->get_converter_min_alpha_or_gamma_in_deg(INVERTER)) // in constant gamma mode
    {
        gamma_min_in_rad = deg2rad(hvdc->get_converter_min_alpha_or_gamma_in_deg(INVERTER));
        gamma_max_in_rad = gamma_min_in_rad;
    }

    double alpha_in_rad = 0.0, gamma_in_rad = 0.0;
    double cos_alpha = 0.0, cos_gamma = 0.0;
    double cos_alpha_min = cos(alpha_min_in_rad), cos_gamma_min = cos(gamma_min_in_rad);

    double tap_r = hvdc->get_converter_transformer_tap_in_pu(RECTIFIER);
    double tap_i = hvdc->get_converter_transformer_tap_in_pu(INVERTER);

    double ebase_grid_r = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER);
    double ebase_converter_r = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER);

    double ebase_grid_i = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER);
    double ebase_converter_i = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

    size_t Nr = hvdc->get_converter_number_of_bridge(RECTIFIER);
    size_t Ni = hvdc->get_converter_number_of_bridge(INVERTER);

    complex<double> Zc_r = hvdc->get_converter_transformer_impedance_in_ohm(RECTIFIER);
    complex<double> Zc_i = hvdc->get_converter_transformer_impedance_in_ohm(INVERTER);
    double Rc_r = Zc_r.real(); double Xc_r = Zc_r.imag();
    double Rc_i = Zc_i.real(); double Xc_i = Zc_i.imag();

    double Rdc = hvdc->get_line_resistance_in_ohm();
    double Rcomp = hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm();

    double margin = hvdc->get_current_power_margin();

    double vac_r = psdb->get_bus_voltage_in_kV(bus_r);
    double vac_i = psdb->get_bus_voltage_in_kV(bus_i);

    double eac_r = vac_r/tap_r*ebase_converter_r/ebase_grid_r;
    double eac_i = vac_i/tap_i*ebase_converter_i/ebase_grid_i;

    double vdc0_r = Nr*3.0*sqrt(2.0)/PI*eac_r;
    double vdc0_i = Ni*3.0*sqrt(2.0)/PI*eac_i;

    double rceq_r = Nr*(3.0/PI*Xc_r+2.0*Rc_r);
    double rceq_i = Ni*(3.0/PI*Xc_i+2.0*Rc_i);

    double Vdci = Vset_kV - Rcomp*Iset_kA;
    double Vdcr = Vdci + Rdc*Iset_kA;

    double Idc;
    if(not is_bypassed())
    {
        if(gamma_min_in_rad>=gamma_max_in_rad) // constant gamma mode
        {
            gamma_in_rad = gamma_min_in_rad;
            hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));

            Vdci =vdc0_i*cos_gamma_min-rceq_i*Iset_kA;
            Vdcr = Vdci+Rdc*Iset_kA;

            cos_alpha = (Vdcr+rceq_r*Iset_kA)/vdc0_r;
            if(cos_alpha<cos_alpha_min)
            {
                alpha_in_rad = acos(cos_alpha);
                hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));
            }
            else
            {
                osstream<<"Warning. At time "<<TIME<<" s, "<<get_device_name()<<" inverter is operating in Constant Gamma mode, but Rectifier cannot hold current/power with alpha.("
                       <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                       <<"Minimum alpha is reached."<<endl
                       <<"Hvdc current command will be reduced from "<<Iset_kA<<" kA to ";
                Iset_kA *= (1.0-margin);
                osstream<<Iset_kA<<" kA with margin by "<<margin*100.0<<"%"<<endl;

                Vdci =vdc0_i*cos_gamma_min-rceq_i*Iset_kA;
                Vdcr = Vdci+Rdc*Iset_kA;
                cos_alpha = (Vdcr+rceq_r*Iset_kA)/vdc0_r;
                if(cos_alpha<cos_alpha_min)
                {
                    alpha_in_rad = acos(cos_alpha);
                    osstream<<"Rectifier can hold the reduced current with alpha = "<<rad2deg(alpha_in_rad)<<" deg.";
                    show_information_with_leading_time_stamp(osstream);
                }
                else
                {
                    alpha_in_rad = alpha_min_in_rad;
                    osstream<<"However, rectifier cannot hold the reduced current even with minimum alpha."<<endl
                           <<"Rectifier will be operated with minimum alpha = "<<rad2deg(alpha_min_in_rad)<<" deg.("
                           <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                           <<"Hvdc will be bypassed.";
                   show_information_with_leading_time_stamp(osstream);
                   bypass_hvdc();
                   solve_hvdc_as_bypassed(Iset_kA_for_bypass);
                   return;
                }
                hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));
            }
        }
        else
        {
            cos_alpha = (Vdcr+rceq_r*Iset_kA)/vdc0_r; // test if alpha can hold dc current
            if(cos_alpha<cos_alpha_min) // alpha can hold dc current
            {
                alpha_in_rad = acos(cos_alpha);
                hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));

                cos_gamma = (Vdci+rceq_i*Iset_kA)/vdc0_i; // test if gamma can hold dc voltage
                if(cos_gamma<cos_gamma_min) // gamma can hold dc voltage
                {
                    gamma_in_rad = acos(cos_gamma);
                    hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));
                }
                else // gamma can not hold dc voltage
                {
                    osstream<<"Warning. At time "<<TIME<<" s, "<<get_device_name()<<" inverter cannot hold voltage with gamma.("
                           <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                           <<"Minimum gamma is reached."<<endl
                           <<"Rectifier will try to regulate current as "<<Iset_kA<<" kA."<<endl;
                    show_information_with_leading_time_stamp(osstream);

                    gamma_in_rad = gamma_min_in_rad;
                    hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));
                    // try to reduce dc voltage
                    // vdcr0*cos_alpha-rcr*Iset_kA=vdci0*cos_gamma_min-rci*Iset_kA + Rdc*Iset_kA
                    cos_alpha = (vdc0_i*cos_gamma_min-rceq_i*Iset_kA+Rdc*Iset_kA+rceq_r*Iset_kA)/vdc0_r;
                    if(cos_alpha<cos_alpha_min)
                    {
                        alpha_in_rad = acos(cos_alpha);
                        osstream<<"Rectifier can hold the current with alpha = "<<rad2deg(alpha_in_rad)<<" deg.";
                    }
                    else//
                    {
                        alpha_in_rad = alpha_min_in_rad;
                        osstream<<"However, Rectifier cannot hold current even with minimum alpha.("
                               <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                               <<"Minimum alpha is alos reached."<<endl
                               <<"Hvdc current command will be reduced from "<<Iset_kA<<" kA to ";
                        Iset_kA *= (1.0-margin);
                        osstream<<Iset_kA<<" kA with margin by "<<margin*100.0<<"%"<<endl;

                        cos_alpha = (vdc0_i*cos_gamma_min-rceq_i*Iset_kA+Rdc*Iset_kA+rceq_r*Iset_kA)/vdc0_r;
                        if(cos_alpha<cos_alpha_min)
                        {
                            alpha_in_rad = acos(cos_alpha);
                            osstream<<"Rectifier can hold reduced current with alpha = "<<rad2deg(alpha_in_rad)<<" deg.";
                        }
                        else
                        {
                            alpha_in_rad = alpha_min_in_rad;
                            osstream<<"However, rectifier cannot hold the reduced current even with minimum alpha."<<endl
                                   <<"Rectifier will be operated with minimum alpha = "<<rad2deg(alpha_min_in_rad)<<" deg.("
                                   <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                                   <<"Hvdc link will be bypassed.";
                            show_information_with_leading_time_stamp(osstream);
                            bypass_hvdc();
                            solve_hvdc_as_bypassed(Iset_kA_for_bypass);
                            return;
                        }
                    }
                    hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));
                    show_information_with_leading_time_stamp(osstream);
                }
            }
            else // alpha can not hold dc current
            {
                alpha_in_rad = alpha_min_in_rad;
                hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));

                osstream<<"Warning. At time "<<TIME<<" s, "<<get_device_name()<<" Rectifier cannot hold current with alpha.("
                       <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                       <<"Minimum alpha is reached."<<endl
                       <<"Hvdc current command will be reduced from "<<Iset_kA<<" kA to ";
                Iset_kA *= (1.0-margin);
                osstream<<Iset_kA<<" kA with margin by "<<margin*100.0<<"%"<<endl
                       <<"Hvdc current command will be regulated by inverter."<<endl;

                cos_gamma = (vdc0_r*cos_alpha_min-rceq_r*Iset_kA-Rdc*Iset_kA+rceq_i*Iset_kA)/vdc0_i;
                if(cos_gamma<cos_gamma_min) // gamma can hold the current
                {
                    gamma_in_rad = acos(cos_gamma);
                    osstream<<"Inverter can hold reduced current with gamma = "<<rad2deg(gamma_in_rad)<<" deg.";
                }
                else // gamma can not hold dc current
                {
                    gamma_in_rad = gamma_min_in_rad;
                    osstream<<"However, inverter cannot hold the reduced current even with minimum gamma."<<endl
                           <<"Inverter will be operated with minimum gamma = "<<rad2deg(gamma_min_in_rad)<<" deg. ("
                           <<"line "<<__LINE__<<" of file "<<__FILE__<<")."<<endl
                           <<"Hvdc link will be bypassed.";
                    show_information_with_leading_time_stamp(osstream);
                    bypass_hvdc();
                    solve_hvdc_as_bypassed(Iset_kA_for_bypass);
                    return;
                }
                hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));
                show_information_with_leading_time_stamp(osstream);

            }
        }

        cos_alpha = cos(alpha_in_rad);
        cos_gamma = cos(gamma_in_rad);

        Idc = (vdc0_r*cos_alpha-vdc0_i*cos_gamma)/(Rdc+rceq_r-rceq_i);
        Vdcr = vdc0_r*cos_alpha - rceq_r*Idc;
        Vdci = vdc0_i*cos_gamma - rceq_i*Idc;

        hvdc->set_converter_dc_voltage_in_kV(RECTIFIER, Vdcr);
        hvdc->set_converter_dc_voltage_in_kV(INVERTER, Vdci);
        hvdc->set_line_dc_current_in_kA(Idc);
    }
    else
    {
        solve_hvdc_as_bypassed(Iset_kA_for_bypass);
    }
    //hvdc->show_solved_hvdc_steady_state();
}

void HVDC_MODEL::solve_hvdc_as_bypassed(double Iset_kA)
{
    ostringstream osstream;

    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked())
        return;

    if(not is_bypassed())
        return;

    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();

    size_t bus_r = hvdc->get_converter_bus(RECTIFIER);
    //size_t bus_i = hvdc->get_converter_bus(INVERTER);

    double alpha_min_in_rad = deg2rad(get_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER));
    double alpha_max_in_rad = deg2rad(get_converter_dynamic_max_alpha_or_gamma_in_deg(RECTIFIER));
    /*double gamma_min_in_rad = deg2rad(get_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER));
    double gamma_max_in_rad = deg2rad(get_converter_dynamic_max_alpha_or_gamma_in_deg(INVERTER));
    if(hvdc->get_converter_max_alpha_or_gamma_in_deg(INVERTER)<=hvdc->get_converter_min_alpha_or_gamma_in_deg(INVERTER)) // in constant gamma mode
    {
        gamma_min_in_rad = deg2rad(hvdc->get_converter_min_alpha_or_gamma_in_deg(INVERTER));
        gamma_max_in_rad = gamma_min_in_rad;
    }*/

    double alpha_in_rad = 0.0, gamma_in_rad = 0.0;
    double cos_alpha = 0.0;
    //double cos_gamma = 0.0;
    double cos_alpha_min = cos(alpha_min_in_rad);
    //double cos_gamma_min = cos(gamma_min_in_rad);

    double tap_r = hvdc->get_converter_transformer_tap_in_pu(RECTIFIER);
    //double tap_i = hvdc->get_converter_transformer_tap_in_pu(INVERTER);

    double ebase_grid_r = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER);
    double ebase_converter_r = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER);

    //double ebase_grid_i = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER);
    //double ebase_converter_i = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

    size_t Nr = hvdc->get_converter_number_of_bridge(RECTIFIER);
    //size_t Ni = hvdc->get_converter_number_of_bridge(INVERTER);

    complex<double> Zc_r = hvdc->get_converter_transformer_impedance_in_ohm(RECTIFIER);
    //complex<double> Zc_i = hvdc->get_converter_transformer_impedance_in_ohm(INVERTER);
    double Rc_r = Zc_r.real(); double Xc_r = Zc_r.imag();
    //double Rc_i = Zc_i.real(); double Xc_i = Zc_i.imag();

    double Rdc = hvdc->get_line_resistance_in_ohm();
    //double Rcomp = hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm();

    //double margin = hvdc->get_current_power_margin();

    double vac_r = psdb->get_bus_voltage_in_kV(bus_r);
    //double vac_i = psdb->get_bus_voltage_in_kV(bus_i);

    double eac_r = vac_r/tap_r*ebase_converter_r/ebase_grid_r;
    //double eac_i = vac_i/tap_i*ebase_converter_i/ebase_grid_i;

    double vdc0_r = Nr*3.0*sqrt(2.0)/PI*eac_r;
    //double vdc0_i = Ni*3.0*sqrt(2.0)/PI*eac_i;

    double rceq_r = Nr*(3.0/PI*Xc_r+2.0*Rc_r);
    //double rceq_i = Ni*(3.0/PI*Xc_i+2.0*Rc_i);

    double Vdci = 0.0;
    double Vdcr = Vdci + Rdc*Iset_kA;

    double Idc;
        // bypassed
    Vdci = 0.0;
    Iset_kA = hvdc->get_rectifier_nominal_dc_current_command_in_kA();
    Vdcr = Iset_kA*Rdc;

    cos_alpha = (Vdcr+rceq_r*Iset_kA)/vdc0_r;

    if(cos_alpha>=cos_alpha_min)
        alpha_in_rad = alpha_min_in_rad;
    else
    {
        if(cos_alpha<=cos(alpha_max_in_rad))
            alpha_in_rad = alpha_max_in_rad;
        else
            alpha_in_rad = acos(cos_alpha);
    }
    gamma_in_rad = PI*0.5;

    cos_alpha = cos(alpha_in_rad);
    //cout<<"alpha during bypassed is :"<<rad2deg(alpha_in_rad)<<" deg"<<endl;

    Idc = (vdc0_r*cos_alpha)/(Rdc+rceq_r);
    Vdcr = vdc0_r*cos_alpha - rceq_r*Idc;
    Vdci = 0.0;

    hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));
    hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));

    hvdc->set_converter_dc_voltage_in_kV(RECTIFIER, Vdcr);
    hvdc->set_converter_dc_voltage_in_kV(INVERTER, Vdci);
    hvdc->set_line_dc_current_in_kA(Idc);
}
void HVDC_MODEL::solve_hvdc_model_with_line_dynamics(double Iset_kA, double Vset_kV)
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return;

    if(is_blocked())
        return;

    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();

    size_t bus_r = hvdc->get_converter_bus(RECTIFIER);
    size_t bus_i = hvdc->get_converter_bus(INVERTER);

    double alpha_min_in_rad = deg2rad(get_converter_dynamic_min_alpha_or_gamma_in_deg(RECTIFIER));
    double alpha_max_in_rad = deg2rad(get_converter_dynamic_max_alpha_or_gamma_in_deg(RECTIFIER));
    double gamma_min_in_rad = deg2rad(get_converter_dynamic_min_alpha_or_gamma_in_deg(INVERTER));
    double gamma_max_in_rad = deg2rad(get_converter_dynamic_max_alpha_or_gamma_in_deg(INVERTER));

    double alpha_in_rad = 0.0, gamma_in_rad = 0.0;
    double cos_alpha = 0.0, cos_gamma = 0.0;
    double cos_alpha_min = cos(alpha_min_in_rad), cos_gamma_min = cos(gamma_min_in_rad);

    double tap_r = hvdc->get_converter_transformer_tap_in_pu(RECTIFIER);
    double tap_i = hvdc->get_converter_transformer_tap_in_pu(INVERTER);

    double ebase_grid_r = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(RECTIFIER);
    double ebase_converter_r = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(RECTIFIER);

    double ebase_grid_i = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(INVERTER);
    double ebase_converter_i = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(INVERTER);

    size_t Nr = hvdc->get_converter_number_of_bridge(RECTIFIER);
    size_t Ni = hvdc->get_converter_number_of_bridge(INVERTER);

    complex<double> Zc_r = hvdc->get_converter_transformer_impedance_in_ohm(RECTIFIER);
    complex<double> Zc_i = hvdc->get_converter_transformer_impedance_in_ohm(INVERTER);
    double Rc_r = Zc_r.real(); double Xc_r = Zc_r.imag();
    double Rc_i = Zc_i.real(); double Xc_i = Zc_i.imag();

    double Rdc = hvdc->get_line_resistance_in_ohm();
    double Rcomp = hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm();

    double vac_r = psdb->get_bus_voltage_in_pu(bus_r);
    double vac_i = psdb->get_bus_voltage_in_pu(bus_i);

    double eac_r = vac_r/tap_r*ebase_converter_r/ebase_grid_r;
    double eac_i = vac_i/tap_i*ebase_converter_i/ebase_grid_i;

    double vdc0_r = Nr*3.0*sqrt(2.0)/PI*eac_r;
    double vdc0_i = Ni*3.0*sqrt(2.0)/PI*eac_i;

    double rceq_r = Nr*(3.0/PI*Xc_r+2.0*Rc_r);
    double rceq_i = Ni*(3.0/PI*Xc_i+2.0*Rc_i);

    double Idc = Iset_kA;
    double Vdci = Vset_kV - Rcomp*get_converter_dc_current_in_kA(INVERTER);
    double Vdcr = Vdci + Rdc*Idc;


    if(not is_bypassed())
    {
        if(gamma_min_in_rad>=gamma_max_in_rad) // constant gamma mode
        {
            gamma_in_rad = gamma_min_in_rad;
            hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));

            Vdci =vdc0_i*cos_gamma_min-rceq_i*Idc;
            Vdcr = Vdci+Rdc*Idc;

            cos_alpha = (Vdcr+rceq_r*Idc)/vdc0_r;
            if(cos_alpha>=cos_alpha_min)
                alpha_in_rad = alpha_min_in_rad;
            else
                alpha_in_rad = acos(cos_alpha);
            hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));
        }
        else
        {
            cos_alpha = (Vdcr+rceq_r*Idc)/vdc0_r;
            if(cos_alpha>=cos_alpha_min)
                alpha_in_rad = alpha_min_in_rad;
            else
                alpha_in_rad = acos(cos_alpha);
            hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));

            cos_gamma = (Vdci+rceq_i*Idc)/vdc0_i;
            if(cos_gamma>=cos_gamma_min)
                gamma_in_rad = gamma_min_in_rad;
            else
                gamma_in_rad = acos(cos_gamma);
            hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));
        }

        cos_alpha = cos(alpha_in_rad);
        cos_gamma = cos(gamma_in_rad);

        Idc = (vdc0_r*cos_alpha-vdc0_i*cos_gamma)/(Rdc+rceq_r-rceq_i);
        Vdcr = vdc0_r*cos_alpha - rceq_r*Idc;
        Vdci = vdc0_i*cos_gamma - rceq_i*Idc;

        hvdc->set_converter_dc_voltage_in_kV(RECTIFIER, Vdcr);
        hvdc->set_converter_dc_voltage_in_kV(INVERTER, Vdci);

        hvdc->set_line_dc_current_in_kA(Idc);
    }
    else
    {
        // bypassed
        Vdci = 0.0;
        Idc = hvdc->get_nominal_dc_current_per_pole_in_kA();
        Vdcr = Idc*Rdc;

        cos_alpha = (Vdcr+rceq_r*Idc)/vdc0_r;

        if(cos_alpha>cos_alpha_min)
            alpha_in_rad = alpha_min_in_rad;
        else
        {
            if(cos_alpha<cos(alpha_max_in_rad))
                alpha_in_rad = alpha_max_in_rad;
            else
                alpha_in_rad = acos(cos_alpha);
        }
        gamma_in_rad = PI*0.5;

        cos_alpha = cos(alpha_in_rad);

        Idc = (vdc0_r*cos_alpha)/(Rdc+rceq_r);
        Vdcr = vdc0_r*cos_alpha - rceq_r*Idc;
        Vdci = 0.0;

        hvdc->set_converter_alpha_or_gamma_in_deg(RECTIFIER, rad2deg(alpha_in_rad));
        hvdc->set_converter_alpha_or_gamma_in_deg(INVERTER, rad2deg(gamma_in_rad));

        hvdc->set_converter_dc_voltage_in_kV(RECTIFIER, Vdcr);
        hvdc->set_converter_dc_voltage_in_kV(INVERTER, Vdci);
        hvdc->set_line_dc_current_in_kA(Idc);
    }
}


double HVDC_MODEL::get_converter_dc_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    double vdc = hvdc->get_converter_dc_voltage_in_kV(converter);
    return vdc;
}

double HVDC_MODEL::get_converter_dc_current_in_kA(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    return hvdc->get_line_dc_current_in_kA();
}

double HVDC_MODEL::get_converter_dc_power_in_MW(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    double V = get_converter_dc_voltage_in_kV(converter);
    double I = get_converter_dc_current_in_kA(converter);
    return V*I;
}

double HVDC_MODEL::get_converter_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 90.0;

    return hvdc->get_converter_alpha_or_gamma_in_deg(converter);
}

double HVDC_MODEL::get_converter_commutation_overlap_angle_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();

    double angle = deg2rad(get_converter_alpha_or_gamma_in_deg(converter));
    double idc = get_converter_dc_current_in_kA(converter);
    double xc = hvdc->get_converter_transformer_impedance_in_ohm(converter).imag();
    double vac = psdb->get_bus_voltage_in_kV(hvdc->get_converter_bus(converter));
    double vbase_grid = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(converter);
    double vbase_converter = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(converter);
    double tap = hvdc->get_converter_transformer_tap_in_pu(converter);
    double eac = vac*vbase_converter/vbase_grid/tap;

    double mu = cos(angle)-sqrt(2.0)*idc*xc/eac;
    mu = acos(mu)-angle;

    return rad2deg(mu);
}

complex<double> HVDC_MODEL::get_converter_ac_complex_power_in_MVA(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    double N = hvdc->get_converter_number_of_bridge(converter);
    double Rc = hvdc->get_converter_transformer_impedance_in_ohm(converter).real();
    double Idc = get_converter_dc_current_in_kA(converter);
    double P = get_converter_dc_power_in_MW(converter);
    double Ploss = N*(2.0*Idc*Idc*Rc); // N for bridges, 2 for two phases
    P += Ploss;
    double phi = deg2rad(get_converter_ac_power_factor_angle_in_deg(converter));
    if(phi==0.0)
        return P;

    double Q = P*tan(phi);

    switch(converter)
    {
        case RECTIFIER:
            return complex<double>(P, Q);
        default:
            return complex<double>(-P, Q);
    }
}

double HVDC_MODEL::get_converter_ac_power_factor_angle_in_deg(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    /*
    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();

    size_t bus = hvdc->get_converter_bus(converter);
    double tap = hvdc->get_converter_transformer_tap_in_pu(converter);

    double ebase_grid = hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(converter);
    double ebase_converter = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(converter);

    size_t N = hvdc->get_converter_number_of_bridge(converter);

    double vac = psdb->get_bus_voltage_in_pu(bus);

    double eac = vac/tap*ebase_converter/ebase_grid;

    double vdc0 = N*3.0*sqrt(2.0)/PI*eac;
    double vdc = get_converter_dc_voltage_in_kV(converter);

    return rad2deg(acos(vdc/vdc0));*/


    double angle = deg2rad(get_converter_alpha_or_gamma_in_deg(converter));
    double mu = deg2rad(get_converter_commutation_overlap_angle_in_deg(converter));

    double angle2 = angle*2.0;
    double mu2 = mu*2.0;
    double total_angle_2 = (angle+mu)*2.0;
    double tan_phi = mu2+sin(angle2)-sin(total_angle_2);
    tan_phi /= (cos(angle2)-cos(total_angle_2));

    return rad2deg(atan(tan_phi));
}

complex<double> HVDC_MODEL::get_converter_ac_current_in_pu(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();
    double sbase = psdb->get_system_base_power_in_MVA();

    complex<double> S = get_converter_ac_complex_power_in_MVA(converter)/sbase;

    complex<double> V = psdb->get_bus_complex_voltage_in_pu(hvdc->get_converter_bus(converter));

    return conj(S/V);
}

complex<double> HVDC_MODEL::get_converter_ac_current_in_kA(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    if(is_blocked())
        return 0.0;

    if(converter==INVERTER and is_bypassed())
        return 0.0;

    complex<double> I = get_converter_ac_current_in_pu(converter);
    size_t bus = hvdc->get_converter_bus(converter);
    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();
    double vbase = psdb->get_bus_base_voltage_in_kV(bus);
    double sbase = psdb->get_system_base_power_in_MVA();
    double ibase = sbase/(sqrt(3.0)*vbase);

    return I*ibase;
}


double HVDC_MODEL::get_converter_ac_voltage_in_pu(HVDC_CONVERTER_SIDE converter) const
{
    HVDC* hvdc = get_hvdc_pointer();
    if(hvdc==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = hvdc->get_power_system_database();

    return psdb->get_bus_voltage_in_pu(hvdc->get_converter_bus(converter));
}
