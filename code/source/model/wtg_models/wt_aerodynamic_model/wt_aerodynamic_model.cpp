#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

WT_AERODYNAMIC_MODEL::WT_AERODYNAMIC_MODEL()
{
    set_number_of_pole_pairs(2);
    set_generator_to_turbine_gear_ratio(100.0);
    set_gear_efficiency(1.0);
    set_nominal_wind_speed_in_mps(13.0);
    set_nominal_air_density_in_kgpm3(1.22);

    set_max_steady_state_turbine_speed_in_pu(1.2);
    set_min_steady_state_turbine_speed_in_pu(0.7);

    set_initial_pitch_angle_in_deg(0.0);
    set_initial_turbine_speed_in_rad_per_s(0.0);

    current_pitch_angle_in_deg = -1e10;
    current_lambda_at_cpmax = 0.0;
    current_pelec_including_loss_per_turbine = -1e10;
    current_turbine_speed_reference_without_limit_in_rad_per_s = 0.0;

    //set_air_density_in_kgpm3(get_nominal_air_density_in_kgpm3());
    //set_initial_pitch_angle_in_deg(0.0);
    //set_initial_wind_speed_in_mps(get_nominal_wind_speed_in_mps());
    //set_initial_turbine_speed_in_rad_per_s(get_nominal_turbine_speed_in_rad_per_s());
}

WT_AERODYNAMIC_MODEL::WT_AERODYNAMIC_MODEL(const WT_AERODYNAMIC_MODEL& model)
{
    copy_from_const_model(model);
}

WT_AERODYNAMIC_MODEL::~WT_AERODYNAMIC_MODEL()
{

}

WT_AERODYNAMIC_MODEL& WT_AERODYNAMIC_MODEL::operator=(const WT_AERODYNAMIC_MODEL& model)
{
    if(this==(&model))
        return *this;

    copy_from_const_model(model);
    return *this;
}

void WT_AERODYNAMIC_MODEL::copy_from_const_model(const WT_AERODYNAMIC_MODEL& model)
{
    set_number_of_pole_pairs(model.get_number_of_pole_pairs());
    set_generator_to_turbine_gear_ratio(model.get_generator_to_turbine_gear_ratio());
    set_gear_efficiency(model.get_gear_efficiency());
    set_turbine_blade_radius_in_m(model.get_turbine_blade_radius_in_m());
    set_nominal_wind_speed_in_mps(model.get_nominal_wind_speed_in_mps());
    set_nominal_air_density_in_kgpm3(model.get_nominal_air_density_in_kgpm3());

    set_air_density_in_kgpm3(model.get_air_density_in_kgpm3());
    set_turbine_speed_mode(model.get_turbine_speed_mode());

    set_max_steady_state_turbine_speed_in_pu(model.get_max_steady_state_turbine_speed_in_pu());
    set_min_steady_state_turbine_speed_in_pu(model.get_min_steady_state_turbine_speed_in_pu());

    set_initial_pitch_angle_in_deg(model.get_initial_pitch_angle_in_deg());
    set_initial_turbine_speed_in_rad_per_s(model.get_initial_turbine_speed_in_rad_per_s());

    set_current_pitch_angle_in_deg(model.get_current_pitch_angle_in_deg());
    set_current_pelec_including_loss_per_turbine_in_MW(model.get_current_pelec_including_loss_per_turbine_in_MW());
}

string WT_AERODYNAMIC_MODEL::get_model_type() const
{
    return "WT AERODYNAMIC";
}

double WT_AERODYNAMIC_MODEL::get_damping_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WT_TURBINE_MODEL* turbine = gen->get_wt_turbine_model();
        if(turbine!=NULL)
            return turbine->get_damping_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;
}

void WT_AERODYNAMIC_MODEL::set_number_of_pole_pairs(size_t n)
{
    if(n==0)
        n = 1;
    number_of_pole_pairs = n;
}

void WT_AERODYNAMIC_MODEL::set_generator_to_turbine_gear_ratio(double r)
{
    if(r<FLOAT_EPSILON)
        r = FLOAT_EPSILON;
    generator_to_turbine_gear_ratio = r;
}

void WT_AERODYNAMIC_MODEL::set_gear_efficiency(double eta)
{
    if(eta>1.0)
        eta = 1.0;
    if(eta<FLOAT_EPSILON)
        eta = FLOAT_EPSILON;
    gear_efficiency = eta;
}

void WT_AERODYNAMIC_MODEL::set_turbine_blade_radius_in_m(double r)
{
    if(r<FLOAT_EPSILON)
        r = FLOAT_EPSILON;
    turbine_blade_radius_in_m = r;
}

void WT_AERODYNAMIC_MODEL::set_nominal_wind_speed_in_mps(double v)
{
    if(v<FLOAT_EPSILON)
        v = FLOAT_EPSILON;
    nominal_wind_speed_in_mps = v;
}

void WT_AERODYNAMIC_MODEL::set_nominal_air_density_in_kgpm3(double rou)
{
    if(rou<FLOAT_EPSILON)
        rou = FLOAT_EPSILON;
    nominal_air_density_in_kgpm3 = rou;
}

size_t WT_AERODYNAMIC_MODEL::get_number_of_pole_pairs() const
{
    return number_of_pole_pairs;
}

double WT_AERODYNAMIC_MODEL::get_generator_to_turbine_gear_ratio() const
{
    return generator_to_turbine_gear_ratio;
}

double WT_AERODYNAMIC_MODEL::get_gear_efficiency() const
{
    return gear_efficiency;
}

double WT_AERODYNAMIC_MODEL::get_turbine_blade_radius_in_m() const
{
    return turbine_blade_radius_in_m;
}

double WT_AERODYNAMIC_MODEL::get_nominal_wind_speed_in_mps() const
{
    return nominal_wind_speed_in_mps;
}

double WT_AERODYNAMIC_MODEL::get_nominal_air_density_in_kgpm3() const
{
    return nominal_air_density_in_kgpm3;
}

double WT_AERODYNAMIC_MODEL::get_nominal_turbine_speed_in_rad_per_s() const
{
    double fbase = get_bus_base_frequency_in_Hz();
    size_t npair = get_number_of_pole_pairs();
    double tratio = get_generator_to_turbine_gear_ratio();
    double fn = fbase/(npair*tratio);
    return hz2radps(fn);
}


void WT_AERODYNAMIC_MODEL::set_initial_pitch_angle_in_deg(double pitch)
{
    initial_pitch_angle_in_deg = pitch;
}

void WT_AERODYNAMIC_MODEL::set_initial_turbine_speed_in_rad_per_s(double w)
{
    initial_turbine_speed_in_rad_per_s = w;
}

double WT_AERODYNAMIC_MODEL::get_initial_pitch_angle_in_deg() const
{
    return initial_pitch_angle_in_deg;
}

double WT_AERODYNAMIC_MODEL::get_initial_turbine_speed_in_rad_per_s() const
{
    return initial_turbine_speed_in_rad_per_s;
}

double WT_AERODYNAMIC_MODEL::get_initial_turbine_speed_in_pu() const
{
    return get_initial_turbine_speed_in_rad_per_s()/get_nominal_turbine_speed_in_rad_per_s();
}

void WT_AERODYNAMIC_MODEL::set_air_density_in_kgpm3(double rou)
{
    if(rou<FLOAT_EPSILON)
        rou = FLOAT_EPSILON;
    air_density_in_kgpm3 = rou;
}

void WT_AERODYNAMIC_MODEL::set_turbine_speed_mode(WTG_TURBINE_SPEED_MODE mode)
{
    speed_mode = mode;
}

void WT_AERODYNAMIC_MODEL::set_max_steady_state_turbine_speed_in_pu(double w)
{
    max_steady_state_turbine_speed_in_pu = w;
}

void WT_AERODYNAMIC_MODEL::set_min_steady_state_turbine_speed_in_pu(double w)
{
    min_steady_state_turbine_speed_in_pu = w;
}

double WT_AERODYNAMIC_MODEL::get_air_density_in_kgpm3() const
{
    return air_density_in_kgpm3;
}

WTG_TURBINE_SPEED_MODE WT_AERODYNAMIC_MODEL::get_turbine_speed_mode() const
{
    return speed_mode;
}

double WT_AERODYNAMIC_MODEL::get_max_steady_state_turbine_speed_in_pu() const
{
    return max_steady_state_turbine_speed_in_pu;
}

double WT_AERODYNAMIC_MODEL::get_min_steady_state_turbine_speed_in_pu() const
{
    return min_steady_state_turbine_speed_in_pu;
}

double WT_AERODYNAMIC_MODEL::get_wind_speed_in_mps() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WIND_SPEED_MODEL* model = gen->get_wind_speed_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_wind_speed_in_mps();
        else
            return get_nominal_wind_speed_in_mps();
    }
    else
        return 0.0;

}


double WT_AERODYNAMIC_MODEL::get_pitch_angle_in_deg() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WT_PITCH_MODEL* model = gen->get_wt_pitch_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_pitch_angle_in_deg();
        else
            return get_initial_pitch_angle_in_deg();
    }
    else
        return 0.0;
}

double WT_AERODYNAMIC_MODEL::get_turbine_speed_in_rad_per_s() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WT_TURBINE_MODEL* model = gen->get_wt_turbine_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_turbine_speed_in_pu()*get_nominal_turbine_speed_in_rad_per_s();
        else
            return get_initial_turbine_speed_in_rad_per_s();
    }
    else
        return 0.0;
}

double WT_AERODYNAMIC_MODEL::get_turbine_frequency_in_Hz() const
{
    return radps2hz(get_turbine_speed_in_rad_per_s());
}

double WT_AERODYNAMIC_MODEL::get_total_wind_power_per_wt_generator_in_MW(double vwind) const
{
    double rou = get_air_density_in_kgpm3();
    double r = get_turbine_blade_radius_in_m();
    double r2 = r*r;
    double v3 = vwind*vwind*vwind;
    return 0.5*rou*PI*r2*v3*1e-6;
}


void WT_AERODYNAMIC_MODEL::set_block_toolkit()
{
    ;
}

void WT_AERODYNAMIC_MODEL::initialize()
{
    ostringstream osstream;
    osstream<<"Now go initialize "<<get_model_name()<<" model of "<<get_device_name()<<endl;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
        if(genmodel!=NULL)
        {
            if(not genmodel->is_model_initialized())
                genmodel->initialize();

            set_block_toolkit();

            set_cpmax_at_zero_pitch();

            initialize_wind_turbine_blade_radius_and_gear_ratio();
            //current_turbine_speed_reference_without_limit_in_rad_per_s = get_turbine_reference_speed_in_rad_per_s_without_speed_limit();

            double pmax = get_maximum_available_mechanical_power_per_wt_generator_in_MW(get_wind_speed_in_mps());
            double cp_max = get_cpmax_at_zero_pitch();
            double pmech = gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()/get_gear_efficiency();
            if(pmax<pmech)
            {
                osstream<<"Initialization error. Wind speed "<<get_wind_speed_in_mps()<<" m/s is not enough to generate power for "<<get_device_name()
                       <<endl
                       <<"Maximum available mechanical power is "<<pmax<<" MW with Cpmax = "<<cp_max<<" and MPPT lambda = "<<get_lambda_at_Cpmax(0.0)<<" at 0.0 pitch angle.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            initialize_pitch_angle_and_turbine_speed();

            set_flag_model_initialized_as_true();

            osstream<<get_model_name()<<" model of "<<get_device_name()<<" is initialized."<<endl
                    <<"(1) Turbine radius is "<<get_turbine_blade_radius_in_m()<<" m"<<endl
                    <<"(2) Gear turn ratio is "<<get_generator_to_turbine_gear_ratio()<<endl
                    <<"(3) Initial turbine speed is "<<get_initial_turbine_speed_in_pu()<<" pu"<<endl
                    <<"(4) Initial pitch angle is "<<get_initial_pitch_angle_in_deg()<<" deg"<<endl;

            double lambda = get_initial_turbine_speed_in_rad_per_s()*get_turbine_blade_radius_in_m()/get_wind_speed_in_mps();
            double pitch = get_initial_pitch_angle_in_deg();
            double der = get_derivative_of_Cp_over_lambda(lambda, pitch);
            osstream<<"With initial pitch angle and rotor speed, the initial operating point is located at the ";
            if(fabs(der)<1e-5)
                osstream<<"MPPT";
            else
            {
                if(der>0.0)
                    osstream<<"ascending";
                else
                    osstream<<"descending";
            }
            osstream<<" side of Cp curve (Cp v.s. w)";
            double w0 = get_initial_turbine_speed_in_rad_per_s();
            osstream<<"With pitch = "<<pitch<<" deg and initial w = "<<w0<<" rad/s, the near OPs are: "<<endl;
            for(double w = w0-0.2; w<w0+0.21; w+=0.1)
            {
                double pmax = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
                osstream<<pitch<<"deg, "<<w<<"rad/s, "<<pmax*get_number_of_lumped_wt_generators()<<"MW"<<endl;
            }
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT_AERODYNAMIC_MODEL::initialize_wind_turbine_blade_radius_and_gear_ratio()
{
    initialize_turbine_blade_radius_with_nominal_parameters();
    initialize_generator_to_turbine_gear_ratio();
}

void WT_AERODYNAMIC_MODEL::initialize_turbine_blade_radius_with_nominal_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(get_rated_power_per_wt_generator_in_MW()<=0.0)
    {
        osstream<<"Error. Wind turbine nominal power is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(get_bus_base_frequency_in_Hz()<=0.0)
    {
        osstream<<"Error. Wind turbine nominal frequency is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(get_number_of_pole_pairs()==0)
    {
        osstream<<"Error. Wind turbine number of pole pairs is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(get_generator_to_turbine_gear_ratio()<=0.0)
    {
        osstream<<"Error. Wind turbine gear ratio is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(get_nominal_air_density_in_kgpm3()<=0.0)
    {
        osstream<<"Error. Wind turbine nominal air density is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(get_nominal_wind_speed_in_mps()<=0.0)
    {
        osstream<<"Error. Wind turbine nominal wind speed is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    if(get_gear_efficiency()<=0.0)
    {
        osstream<<"Error. Gear efficiency is not properly set before setup turbine blade radius.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    double pn = get_rated_power_per_wt_generator_in_MW()*1e6;
    double eta = get_gear_efficiency();
    pn /= eta;

    double v = get_nominal_wind_speed_in_mps();
    double v3 =  v*v*v;

    double rou = get_nominal_air_density_in_kgpm3();

    double cp_max = get_cpmax_at_zero_pitch();

    double blade_area = 2.0*pn/(cp_max*rou*v3);
    double blade_radius = sqrt(blade_area/PI);
    set_turbine_blade_radius_in_m(blade_radius);

    osstream<<"Turbine blade radius is initialized as: "<<blade_radius<<"m";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return;
}

void WT_AERODYNAMIC_MODEL::initialize_generator_to_turbine_gear_ratio()
{
    ostringstream osstream;

    double lambda_mppt = get_lambda_at_Cpmax(0.0);
    double vwind = get_nominal_wind_speed_in_mps();
    double radius = get_turbine_blade_radius_in_m();

    double wt = lambda_mppt*vwind/radius;

    size_t n = get_number_of_pole_pairs();
    double fbase = get_bus_base_frequency_in_Hz();
    double wg = 2.0*PI*fbase/n;
    double turnratio = wg/wt;
    set_generator_to_turbine_gear_ratio(turnratio);

    osstream<<"Turbine gear ratio is initialized as: "<<turnratio;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return;
}


void WT_AERODYNAMIC_MODEL::set_cpmax_at_zero_pitch()
{
    cpmax_at_zero_pitch = get_Cpmax(0.0);
}

double WT_AERODYNAMIC_MODEL::get_cpmax_at_zero_pitch() const
{
    return cpmax_at_zero_pitch;
}

void WT_AERODYNAMIC_MODEL::initialize_pitch_angle_and_turbine_speed()
{
    ostringstream osstream;

    osstream<<"Now go initialize WT generator pitch angle and turbine speed."<<endl;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    double pmax = get_maximum_available_mechanical_power_per_wt_generator_in_MW(get_wind_speed_in_mps());

    WT_GENERATOR* genptr = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* wtgenmodel = genptr->get_wt_generator_model();
    double pelec = wtgenmodel->get_terminal_active_power_in_MW()/get_number_of_lumped_wt_generators();
    osstream<<"Maximum available power per WT generator is "<<pmax<<" MW, and set generation is "<<pelec<<" MW"<<endl
            <<"Current wind speed is "<<get_wind_speed_in_mps()<<" m/s";
    toolkit.show_information_with_leading_time_stamp(osstream);

    switch(get_turbine_speed_mode())
    {
        case WT_MPPT_MODE:
        {
            initialize_pitch_angle_and_turbine_speed_with_mppt_mode();
            break;
        }
        case WT_UNDERSPEED_MODE:
        case WT_OVERSPEED_MODE:
        {
            initialize_pitch_angle_and_turbine_speed_with_underspeed_or_overspeed_mode();
            break;
        }
    }
}

void WT_AERODYNAMIC_MODEL::initialize_pitch_angle_and_turbine_speed_with_underspeed_or_overspeed_mode()
{
    ostringstream osstream;

    osstream<<"Now go initialize pitch angle and turbine speed of "<<get_device_name()<<" with turbine speed mode: ";
    if(get_turbine_speed_mode()==WT_UNDERSPEED_MODE)
        osstream<<"UNDERSPEED mode";
    else
    {
        if(get_turbine_speed_mode()==WT_OVERSPEED_MODE)
            osstream<<"OVERSPEED mode";
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    set_initial_pitch_angle_in_deg(0.0);
    initialize_turbine_speed();

    double wn = get_nominal_turbine_speed_in_rad_per_s();
    double w = get_initial_turbine_speed_in_rad_per_s();
    osstream<<"Step 1: with initial pitch angle = 0.0 deg, initialized turbine speed is "<<w<<" rad/s ("
            <<get_initial_turbine_speed_in_pu()<<" pu) (wn = "<<wn<<" rad/s)"<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    double wmax = get_max_steady_state_turbine_speed_in_pu()*wn;
    double wmin = get_min_steady_state_turbine_speed_in_pu()*wn;
    if(w >= wmin and w <= wmax)
    {
        osstream<<"Initialized turbine speed is OK.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    if(w>wmax)
    {
        osstream<<"Initialized turbine speed exceed maximum value "<<wmax<<" rad/s.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_initial_turbine_speed_in_rad_per_s(wmax);
    }
    else
    {
        osstream<<"Initialized turbine speed exceed minimum value "<<wmin<<" rad/s.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        set_initial_turbine_speed_in_rad_per_s(wmin);
    }
    osstream<<"Step 2: turbine speed is initialized as "<<get_initial_turbine_speed_in_rad_per_s()<<" rad/s.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    osstream<<"Step 3: Initialize pitch angle with turbine speed "<<get_initial_turbine_speed_in_rad_per_s()<<" rad/s.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    initialize_pitch_angle();
    osstream<<"Initialized pitch angle is "<<get_initial_pitch_angle_in_deg()<<" deg.";
    toolkit.show_information_with_leading_time_stamp(osstream);

}

void WT_AERODYNAMIC_MODEL::initialize_pitch_angle_and_turbine_speed_with_mppt_mode()
{
    ostringstream osstream;

    osstream<<"Now go initialize pitch angle and turbine speed of "<<get_device_name()<<" with turbine speed mode: MPPT mode";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        double D = get_damping_in_pu();
        double mbase = get_mbase_in_MVA();
        complex<double> zsource = get_source_impedance_in_pu_based_on_mbase();

        complex<double> selec = gen->get_complex_generation_in_MVA();
        complex<double> vterm = get_terminal_complex_voltage_in_pu();

        selec /= mbase;
        double iterm = steps_fast_complex_abs(selec)/ steps_fast_complex_abs(vterm);
        selec += (iterm*iterm*zsource);

        selec *= mbase;

        double pelec = selec.real();
        size_t n = get_number_of_lumped_wt_generators();
        pelec /= n;

        double vwind = get_wind_speed_in_mps();

        double pmax = get_total_wind_power_per_wt_generator_in_MW(vwind);

        double wn = get_nominal_turbine_speed_in_rad_per_s();

        double pitch_low = 0.0, pitch_high = 0.0;
        double pitch_step = 2.0;
        double w_mppt_low = 0.0, w_mppt_high = 0.0;

        w_mppt_low = get_mppt_speed_in_rad_per_s(pitch_low);

        double w_low = (w_mppt_low-wn)/wn;
        double cpmax_low = get_Cpmax(pitch_low);
        //osstream<<"pitch_low = "<<pitch_low<<", w_mppt_low = "<<w_mppt_low<<", w_low = "<<w_low<<", pmechmax = "<<pmax*cpmax_low;
        //show_information_with_leading_time_stamp(osstream);
        double pmech_low = pelec+(D*w_low)*(w_mppt_low/wn)*(mbase/n);
        if(pmech_low<=pmax*cpmax_low)
        {
            if(fabs(pmech_low-pmax*cpmax_low)>FLOAT_EPSILON)
            {
                size_t iter_max = 100;
                size_t iter = 0;

                while(true)
                {
                    pitch_high = pitch_low+pitch_step;
                    w_mppt_high = get_mppt_speed_in_rad_per_s(pitch_high);

                    double w_high = (w_mppt_high-wn)/wn;
                    double cpmax_high = get_Cpmax(pitch_high);
                    double pmech_high = pelec+(D*w_high)*(w_mppt_high/wn)*(mbase/n);
                    if(pmech_high<=pmax*cpmax_high)
                    {
                        pitch_low = pitch_high;
                        w_mppt_low = w_mppt_high;
                    }
                    else
                        break;
                    ++iter;
                    if(iter>iter_max)
                    {
                        if(toolkit.is_detailed_log_enabled())
                        {
                            osstream<<"Warning. Failed to exit loop within "<<iter_max<<" iterations @ line "<<__LINE__<<" of "<<__FILE__;
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                        break;
                    }
                }
                iter = 0;
                while(true)
                {
                    double pitch = 0.5*(pitch_low+pitch_high);
                    double w_mppt = get_mppt_speed_in_rad_per_s(pitch);
                    double w = (w_mppt-wn)/wn;
                    double pmech = pelec+(D*w)*(w_mppt/wn)*(mbase/n);
                    double cpmax = get_Cpmax(pitch);

                    //osstream<<"pitch = "<<pitch<<", w_mppt = "<<w_mppt<<", w = "<<w<<", pmechmax = "<<pmax*cpmax;
                    //show_information_with_leading_time_stamp(osstream);
                    if(fabs(pmech-pmax*cpmax)>FLOAT_EPSILON)
                    {
                        if(pmech>pmax*cpmax)
                            pitch_high = pitch;
                        else
                            pitch_low = pitch;

                        ++iter;
                        if(iter>iter_max)
                        {
                            osstream<<"Warning. Failed to initialize pitch angle and turbine speed within "<<iter_max<<" iterations when turbine speed mode is WT_MPPT_MODE."<<endl
                                    <<"Initial pitch angle is set as "<<pitch<<" deg, and initial turbine speed is set as "<<w_mppt<<" rad/s";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                            set_initial_pitch_angle_in_deg(pitch);
                            set_initial_turbine_speed_in_rad_per_s(w_mppt);
                            break;
                        }
                    }
                    else
                    {
                        set_initial_pitch_angle_in_deg(pitch);
                        set_initial_turbine_speed_in_rad_per_s(w_mppt);
                        break;
                    }
                }

                double w = get_initial_turbine_speed_in_rad_per_s();
                double wmax = get_max_steady_state_turbine_speed_in_pu();
                double wmin = get_min_steady_state_turbine_speed_in_pu();
                if(w>wmax*wn)
                {
                    osstream<<"In WT_MPPT_MODE, initialized turbine speed = "<<w<<" rad/s, greater than wmax = "<<wmax*wn<<" rad/s."<<endl
                            <<"Initial turbine speed will be set as wmax = "<<wmax*wn<<" rad/s";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    set_initial_turbine_speed_in_rad_per_s(wmax*wn);
                    initialize_pitch_angle();
                    return;
                }
                if(w<wmin*wn)
                {
                    osstream<<"In WT_MPPT_MODE, initialized turbine speed = "<<w<<" rad/s, less than wmin = "<<wmin*wn<<" rad/s."<<endl
                            <<"Initial turbine speed will be set as wmin = "<<wmin*wn<<" rad/s";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    set_initial_turbine_speed_in_rad_per_s(wmin*wn);
                    initialize_pitch_angle();
                    return;
                }
            }
            else
            {
                set_initial_pitch_angle_in_deg(pitch_low);
                set_initial_turbine_speed_in_rad_per_s(w_mppt_low);
            }
        }
        else
        {
            osstream<<"Error. Mechanical power with Cpmax at 0 pitch angle is less than required mechanical power."<<endl
                    <<"Initialization of "<<get_model_name()<<" of "<<get_device_name()<<" failed.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT_AERODYNAMIC_MODEL::initialize_pitch_angle()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        double D = get_damping_in_pu();
        double mbase = get_mbase_in_MVA();
        complex<double> zsource = get_source_impedance_in_pu_based_on_mbase();

        complex<double> selec = gen->get_complex_generation_in_MVA();
        complex<double> vterm = get_terminal_complex_voltage_in_pu();

        selec /= mbase;
        double iterm = steps_fast_complex_abs(selec)/ steps_fast_complex_abs(vterm);
        selec += (iterm*iterm*zsource);

        selec *= mbase;

        double pelec = selec.real();
        size_t n = get_number_of_lumped_wt_generators();
        pelec /= n;

        double pn = get_rated_power_per_wt_generator_in_MW();

        if(pelec<=pn)
        {
            double wn = get_nominal_turbine_speed_in_rad_per_s();
            double w = get_initial_turbine_speed_in_rad_per_s();
            double dspeed = (w-wn)/wn;
            double pdamp = D*dspeed*(mbase/wn)*w/n;
            double eta = get_gear_efficiency();
            double pmech = (pelec+pdamp)/eta;

            double pitch_low = 0.0, pitch_high = 0.0;
            double pitch_step = 2.0;

            set_initial_pitch_angle_in_deg(pitch_low);
            double pwind = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);

            size_t iter_count = 0, iter_max = 100;
            while(true)
            {
                pitch_high = pitch_low + pitch_step;

                set_initial_pitch_angle_in_deg(pitch_high);
                pwind = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);

                if(pwind>pelec)
                    pitch_low = pitch_high;
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

            set_initial_pitch_angle_in_deg(pitch_low);
            double pwind_low = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
            set_initial_pitch_angle_in_deg(pitch_high);
            double pwind_high = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);

            osstream<<"Desired pmech = "<<pmech<<" MW (per wind turbine)"<<endl
                    <<"Initial pitch angle: low pitch = "<<pitch_low<<" deg, pmech = "<<pwind_low<<" MW"<<endl
                    <<"                     high pitch = "<<pitch_high<<" deg, pmech = "<<pwind_high<<" MW"<<endl;

            iter_count = 0;
            while(true)
            {
                double pitch_new = 0.5*(pitch_low+pitch_high);
                set_initial_pitch_angle_in_deg(pitch_new);
                double pnew = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
                osstream<<"Iteration "<<iter_count<<": pitch = "<<pitch_new<<" deg, pmech = "<<pnew<<" MW"<<endl;

                if(fabs(pnew-pmech)>FLOAT_EPSILON)
                {
                    if(pnew>pmech)
                        pitch_low = pitch_new;
                    if(pnew<pmech)
                        pitch_high = pitch_new;

                    ++iter_count;
                    if(iter_count<=iter_max)
                        ;
                    else
                    {
                        string iteration_info = osstream.str();
                        osstream.str("");
                        osstream<<"Warning. Failed to get initial pitch angle in "<<iter_max<<" iterations."<<endl
                                <<"Pitch angle is initialized as "<<pitch_new<<" deg."<<endl
                                <<"Check "<<get_model_name()<<" model of "<<get_device_name()<<endl
                                <<"Below are iteration:"<<endl
                                <<iteration_info;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        break;
                    }
                }
                else
                {
                    osstream<<__FUNCTION__<<"() takes "<<iter_count<<" iterations.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    break;
                }
            }
        }
        else
        {
            osstream<<"Error when getting initial turbine speed of "<<get_model_name()<<" model of "<<get_device_name()<<". Initial power exceeds WT nominal power.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }
    }
}

void WT_AERODYNAMIC_MODEL::initialize_turbine_speed()
{
    double w = get_turbine_reference_speed_in_rad_per_s_without_speed_limit();
    set_initial_turbine_speed_in_rad_per_s(w);
}

void WT_AERODYNAMIC_MODEL::run(DYNAMIC_MODE mode)
{
    //ostringstream osstream;
    //osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    //show_information_with_leading_time_stamp(osstream);
}

double WT_AERODYNAMIC_MODEL::get_maximum_available_mechanical_power_per_wt_generator_in_MW(double vwind)
{
    double cpmax = get_cpmax_at_zero_pitch();
    double pmax = get_total_wind_power_per_wt_generator_in_MW(vwind);
    return pmax*cpmax;
}

double WT_AERODYNAMIC_MODEL::get_turbine_mechanical_power_per_wt_generator_in_MW() const
{
    double w = get_turbine_speed_in_rad_per_s();
    double pmech = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
    double eta = get_gear_efficiency();
    return pmech*eta;
}

double WT_AERODYNAMIC_MODEL::get_turbine_mechanical_power_in_MW() const
{
    return get_turbine_mechanical_power_per_wt_generator_in_MW()*get_number_of_lumped_wt_generators();
}

double WT_AERODYNAMIC_MODEL::get_turbine_reference_speed_in_rad_per_s()
{
    ostringstream osstream;

    double wn = get_nominal_turbine_speed_in_rad_per_s();
    double w = get_turbine_reference_speed_in_rad_per_s_without_speed_limit();

    //osstream<<__LINE__<<"@"<<__FILE__<<endl
    //        <<"wref = "<<w<<", wn = "<<wn<<endl;
    double wmax = get_max_steady_state_turbine_speed_in_pu()*wn;
    double wmin = get_min_steady_state_turbine_speed_in_pu()*wn;
    if(w>wmax)
        w = wmax;

    if(w<wmin)
        w = wmin;
    //osstream<<"w to return is "<<w;
    //show_information_with_leading_time_stamp(osstream);
    return w;
}

double WT_AERODYNAMIC_MODEL::get_turbine_reference_speed_in_pu()
{
    return get_turbine_reference_speed_in_rad_per_s()/get_nominal_turbine_speed_in_rad_per_s();
}

double WT_AERODYNAMIC_MODEL::get_turbine_reference_speed_in_rad_per_s_without_speed_limit()
{
    ostringstream osstream;

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
    {
        WT_GENERATOR_MODEL* wtgenmodel = gen->get_wt_generator_model();
        if(wtgenmodel!=NULL)
        {
            complex<double> selec = wtgenmodel->get_terminal_complex_power_in_MVA();
            double iterm = wtgenmodel->get_terminal_current_in_pu_based_on_mbase();
            complex<double> zsource = get_source_impedance_in_pu_based_on_mbase();
            double mbase = get_mbase_in_MVA();
            selec += (iterm*iterm*zsource)*mbase;

            size_t n = get_number_of_lumped_wt_generators();
            selec /= n;

            double pelec = selec.real();

            double pitch = get_pitch_angle_in_deg();
            if(fabs(get_current_pelec_including_loss_per_turbine_in_MW()-pelec)<FLOAT_EPSILON and
               fabs(get_current_pitch_angle_in_deg()-pitch)<FLOAT_EPSILON)
                return get_current_turbine_speed_reference_without_limit_in_rad_per_s();
            else
            {
                set_current_pitch_angle_in_deg(pitch);
                set_current_pelec_including_loss_per_turbine_in_MW(pelec);
                update_current_turbine_speed_reference_without_limit();
                return get_current_turbine_speed_reference_without_limit_in_rad_per_s();
            }
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double WT_AERODYNAMIC_MODEL::get_mppt_speed_in_rad_per_s(double pitch_in_deg)
{
    double lambda = get_lambda_at_Cpmax(pitch_in_deg);
    double vwind = get_wind_speed_in_mps();
    double r = get_turbine_blade_radius_in_m();
    double w_mppt = lambda*vwind/r;
    return w_mppt;
}


void WT_AERODYNAMIC_MODEL::set_current_pitch_angle_in_deg(double pitch)
{
    if(fabs(get_current_pitch_angle_in_deg()-pitch)<FLOAT_EPSILON)
        return;

    current_pitch_angle_in_deg = pitch;

    update_current_lambda_at_cpmax_with_current_pitch_angle();
}

void WT_AERODYNAMIC_MODEL::update_current_lambda_at_cpmax_with_current_pitch_angle()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    double pitch = get_current_pitch_angle_in_deg();

    double lambda_low = 3.0;
    double lambda_high = 3.0;
    double lambda_step =  1.0;

    size_t iter_count = 0, iter_max = 100;
    while(true)
    {
        double der = get_derivative_of_Cp_over_lambda(lambda_low, pitch);
        if(der<=0.0)
        {
            if(der==0.0)
                lambda_low += lambda_step;
            else
                lambda_low -= lambda_step;
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
    iter_count=0;
    while(true)
    {
        double der = get_derivative_of_Cp_over_lambda(lambda_high, pitch);
        if(der>=0.0)
        {
            if(der==0.0)
                lambda_high -= lambda_step;
            else
                lambda_high += lambda_step;
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

    double lambda_new = 0.0;
    iter_count=0;
    while(true)
    {
        lambda_new = 0.5*(lambda_low+lambda_high);
        double der_new =  get_derivative_of_Cp_over_lambda(lambda_new, pitch);
        if(fabs(der_new)>FLOAT_EPSILON)
        {
            if(der_new>0.0)
                lambda_low = lambda_new;
            else
                lambda_high = lambda_new;

            ++iter_count;
            if(iter_count<=iter_max)
                ;
            else
                break;
        }
        else
            break;
    }
    current_lambda_at_cpmax = lambda_new;
}


void WT_AERODYNAMIC_MODEL::set_current_pelec_including_loss_per_turbine_in_MW(double pelec)
{
    if(fabs(get_current_pelec_including_loss_per_turbine_in_MW()-pelec)>FLOAT_EPSILON)
        current_pelec_including_loss_per_turbine = pelec;
}

void WT_AERODYNAMIC_MODEL::update_current_turbine_speed_reference_without_limit()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    double pitch = get_current_pitch_angle_in_deg();

    double w_mppt = get_mppt_speed_in_rad_per_s(pitch);

    if(get_turbine_speed_mode()==WT_MPPT_MODE)
    {
        current_turbine_speed_reference_without_limit_in_rad_per_s = w_mppt;
        return;
    }

    double pelec = get_current_pelec_including_loss_per_turbine_in_MW();

    double pmech = pelec;
    double cpmax = get_Cpmax(pitch);

    double vwind = get_wind_speed_in_mps();
    double pmax = cpmax*get_total_wind_power_per_wt_generator_in_MW(vwind);
    if(pmax>=pmech)
    {
        // pmax>pmech

        double damping = get_damping_in_pu();
        double wn = get_nominal_turbine_speed_in_rad_per_s();
        double eta = get_gear_efficiency();

        double mbase = get_mbase_in_MVA();
        size_t n = get_number_of_lumped_wt_generators();

        double w_low, w_high;
        double w_step = 0.5;
        double p_low, p_high;

        size_t iter_count =0, iter_max = 100;
        if(get_turbine_speed_mode()==WT_UNDERSPEED_MODE)
        {
            w_high = w_mppt;
            iter_count = 0;
            while(true)
            {
                w_low = w_high - w_step;
                p_low = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w_low);
                double telec = pelec/w_low;
                double tdamp = damping*(w_low/wn-1.0)*(mbase/wn)/n;
                double tmech = telec+tdamp;
                pmech = tmech*w_low;
                pmech /= eta;
                if(p_low>=pmech)
                    w_high = w_low;
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
        }
        else
        {
            w_low = w_mppt;
            iter_count = 0;
            while(true)
            {
                w_high = w_low + w_step;
                p_high = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w_high);
                double telec = pelec/w_high;
                double tdamp = damping*(w_high/wn-1.0)*(mbase/wn)/n;
                double tmech = telec+tdamp;
                pmech = tmech*w_high;
                pmech /= eta;
                if(p_high>=pmech)
                    w_low = w_high;
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
        }

        //double plow = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(wlow);
        //double phigh = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(whigh);


        double w = 0.0;
        iter_count = 0;
        while(true)
        {
            double w_new = 0.5*(w_low+w_high);
            double p_new = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w_new);
            //cout<<"when w = "<<w_new<<" rad/s ("<<w_new/wn<<" pu), extracted mechanical power = "<<pnew<<"MW"<<endl;
            double telec = pelec/w_new;
            double tdamp = damping*(w_new/wn-1.0)*(mbase/wn)/n;
            double tmech = telec+tdamp;
            //cout<<"telec = "<<telec<<", tdamp = "<<tdamp<<", tmech = "<<tmech<<endl;
            //cout<<"pelec = "<<telec*w_new<<", pdamp = "<<tdamp*w_new<<", pmech = "<<tmech*w_new<<endl;
            pmech = tmech*w_new;
            pmech /= eta;
            if(fabs(p_new-pmech)>FLOAT_EPSILON)
            {
                if(get_turbine_speed_mode()==WT_UNDERSPEED_MODE)
                {
                    if(p_new>pmech)
                        w_high = w_new;
                    else
                        w_low = w_new;
                }
                else
                {
                    if(p_new>pmech)
                        w_low = w_new;
                    else
                        w_high = w_new;
                }
                iter_count++;
                if(iter_count<=iter_max)
                    ;
                else
                {
                    w = w_new;
                    /*osstream<<"Warning. Failed to get reference wt turbine speed in "<<iter_max<<" iterations."<<endl
                           <<"Current electrical power generation = "<<pmech*n<<"MW. Maximum available wind power = "<<pmax*n<<" MW at 0.0 pitch ange"<<endl
                           <<"Maximum available wind power = "<<get_total_wind_power_per_wt_generator_in_MW(vwind)*cpmax*n<<" MW at current pitch ange"<<endl
                           <<"Current wind speed = "<<vwind<<" m/s, pitch angle = "<<pitch<<" deg, Cpmax = "<<cpmax<<" at w_mpppt = "<<w_mppt<<" rad/s"<<endl
                           <<"Reference turbine speed is returned as "<<w<<" rad/s."<<endl
                           <<"Check "<<get_model_name()<<" model of "<<get_device_name();
                    toolkit.show_information_with_leading_time_stamp(osstream);*/
                    break;
                }
            }
            else
            {
                w = w_new;
                //osstream<<__FUNCTION__<<"() takes "<<iter_count<<" iterations.";
                //show_information_with_leading_time_stamp(osstream);
                break;
            }
        }

        current_turbine_speed_reference_without_limit_in_rad_per_s = w;
    }
    else
    {
        /*osstream<<"Warning. Current electrical power generation of "<<get_device_name()<<" exceeds the maximum available wind power:"<<endl
               <<"Current electrical power generation = "<<pmech*n<<"MW. Maximum available wind power = "<<pmax*n<<" MW"<<endl
               <<"Current wind speed = "<<vwind<<" m, pitch angle = "<<pitch<<" deg, Cpmax = "<<cpmax<<" at w_mpppt = "<<w_mppt<<" rad/s"<<endl
               <<"MPPT speed will be returned as speed reference: "<<w_mppt<<" rad/s.";
        toolkit.show_information_with_leading_time_stamp(osstream);*/

        current_turbine_speed_reference_without_limit_in_rad_per_s = w_mppt;
        return;
    }
}

double WT_AERODYNAMIC_MODEL::get_current_pitch_angle_in_deg() const
{
    return current_pitch_angle_in_deg;
}

double WT_AERODYNAMIC_MODEL::get_current_lambda_at_cpmax() const
{
    return current_lambda_at_cpmax;
}

double WT_AERODYNAMIC_MODEL::get_current_pelec_including_loss_per_turbine_in_MW() const
{
    return current_pelec_including_loss_per_turbine;
}

double WT_AERODYNAMIC_MODEL::get_current_turbine_speed_reference_without_limit_in_rad_per_s() const
{
    return current_turbine_speed_reference_without_limit_in_rad_per_s;
}

double WT_AERODYNAMIC_MODEL::get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(double speed_rad_per_s) const
{
    double w = speed_rad_per_s;
    double r = get_turbine_blade_radius_in_m();
    double v = get_wind_speed_in_mps();
    double lambda = w*r/v;
    double beta = get_pitch_angle_in_deg();
    double cp = get_Cp(lambda, beta);

    double pmax = get_total_wind_power_per_wt_generator_in_MW(v);

    return pmax*cp;
}
double WT_AERODYNAMIC_MODEL::get_Cpmax(double pitch_deg)
{
    double lambda = get_lambda_at_Cpmax(pitch_deg);
    return get_Cp(lambda, pitch_deg);
}

double WT_AERODYNAMIC_MODEL::get_lambda_at_Cpmax(double pitch_deg)
{
    /*ostringstream osstream;
    osstream<<"Searching lambda_max of pitch "<<pitch_deg<<endl
            <<"Current [pitch, lambda_max] pair is ["<<get_current_pitch_angle_in_deg()
            <<", "<<get_current_lambda_at_cpmax()<<"]";
    toolkit.show_information_with_leading_time_stamp(osstream);*/

    if(fabs(get_current_pitch_angle_in_deg()-pitch_deg)<FLOAT_EPSILON)
        return get_current_lambda_at_cpmax();

    set_current_pitch_angle_in_deg(pitch_deg);

    //osstream<<"New [pitch, lambda_max] pair is ["<<get_current_pitch_angle_in_deg()
    //        <<", "<<get_current_lambda_at_cpmax()<<"]";
    //show_information_with_leading_time_stamp(osstream);

    return get_current_lambda_at_cpmax();
}
