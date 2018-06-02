#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"
#include "header/basic/utility.h"
#include <iostream>

using namespace std;

AERD0::AERD0()
{
    set_C1(0.22);
    set_C2(116.0);
    set_C3(0.4);
    set_C4(5.0);
    set_C5(-12.5);
    set_C6(0.0);
}

AERD0::AERD0(const AERD0& model):WT_AERODYNAMIC_MODEL()
{
    copy_from_const_model(model);
}

AERD0::~AERD0()
{

}

AERD0& AERD0::operator=(const AERD0& model)
{
    if(this==(&model))
        return *this;

    copy_from_const_model(model);
    return *this;
}

void AERD0::copy_from_const_model(const AERD0& model)
{
    WT_AERODYNAMIC_MODEL::copy_from_const_model(model);

    set_C1(model.get_C1());
    set_C2(model.get_C2());
    set_C3(model.get_C3());
    set_C4(model.get_C4());
    set_C5(model.get_C5());
    set_C6(model.get_C6());
}


void AERD0::set_C1(double c)
{
    Cp_Coefficients[0] = c;
}

void AERD0::set_C2(double c)
{
    Cp_Coefficients[1] = c;
}

void AERD0::set_C3(double c)
{
    Cp_Coefficients[2] = c;
}

void AERD0::set_C4(double c)
{
    Cp_Coefficients[3] = c;
}

void AERD0::set_C5(double c)
{
    Cp_Coefficients[4] = c;
}

void AERD0::set_C6(double c)
{
    Cp_Coefficients[5] = c;
}

double AERD0::get_C1() const
{
    return Cp_Coefficients[0];
}

double AERD0::get_C2() const
{
    return Cp_Coefficients[1];
}

double AERD0::get_C3() const
{
    return Cp_Coefficients[2];
}

double AERD0::get_C4() const
{
    return Cp_Coefficients[3];
}

double AERD0::get_C5() const
{
    return Cp_Coefficients[4];
}

double AERD0::get_C6() const
{
    return Cp_Coefficients[5];
}

double AERD0::get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(double speed_rad_per_s) const
{
    double w = speed_rad_per_s;

    double r = get_turbine_blade_radius_in_m();
    double v = get_wind_speed_in_mps();
    double lambda = w*r/v;

    double beta = get_pitch_angle_in_deg();

    double Cp = get_Cp(lambda, beta);

    double rou = get_air_density_in_kgpm3();

    double r2 = r*r;
    double v3 = v*v*v;

    //size_t n = get_number_of_lumped_wt_generators();

    return 0.5*rou*PI*r2*v3*Cp*1e-6;
}

double AERD0::get_Cp(double lambda, double pitch_deg) const
{
    //double pitch_angle = deg2rad(pitch_deg);
    double pitch_angle = pitch_deg;
    double lambdai = 1.0/(lambda+0.08*pitch_angle)-0.035/(pitch_angle*pitch_angle*pitch_angle+1.0);
    double L = 1.0/lambdai;

    double C1 = get_C1();
    double C2 = get_C2();
    double C3 = get_C3();
    double C4 = get_C4();
    double C5 = get_C5();
    double C6 = get_C6();

    return C1*(C2/L-C3*pitch_angle-C4)*exp(C5/L)+C6*lambda;
}

double AERD0::get_Cpmax(double pitch_deg) const
{
    double lambda = get_lambda_at_Cpmax(pitch_deg);
    return get_Cp(lambda, pitch_deg);
}

double AERD0::get_lambda_at_Cpmax(double pitch_deg) const
{
    ostringstream sstream;

    double wn = get_nominal_turbine_speed_in_rad_per_s();
    double r = get_turbine_blade_radius_in_m();
    double v = get_nominal_wind_speed_in_mps();

    double lambdalow = 0.5*wn*r/v;
    double lambdahigh = 2.0*wn*r/v;
    while(true)
    {
        double der = get_derivative_of_Cp_over_lambda(lambdalow, pitch_deg);
        if(der>0.0)
            break;
        else
            lambdalow *= 0.5;
    }
    while(true)
    {
        double der = get_derivative_of_Cp_over_lambda(lambdahigh, pitch_deg);
        if(der<0.0)
            break;
        else
            lambdahigh *= 2.0;
    }

    double derlow =  get_derivative_of_Cp_over_lambda(lambdalow, pitch_deg);
    double derhigh =  get_derivative_of_Cp_over_lambda(lambdahigh, pitch_deg);
    double newlambda = 0.0;
    size_t iter_count = 0, iter_max = 50;
    while(true)
    {
        newlambda = lambdalow*(derhigh/(derhigh-derlow))-lambdahigh*(derlow/(derhigh-derlow));
        double dernew =  get_derivative_of_Cp_over_lambda(newlambda, pitch_deg);

        if(fabs(dernew)<1e-6)
        {
            lambdalow = newlambda;
            lambdahigh = newlambda;
            break;
        }

        if(dernew>0.0)
        {
            lambdalow = newlambda;
            derlow = dernew;
        }
        else
        {
            lambdahigh = newlambda;
            derhigh = dernew;
        }
        if(fabs(lambdalow-lambdahigh)<1e-6)
            break;
        iter_count++;
        if(iter_count>iter_max)
        {
            newlambda = 0.5*(lambdalow+lambdahigh);
            sstream<<"Warning. Failed to get lambda at Cpmax in "<<iter_max<<" iterations."<<endl
                   <<"Lambda is returned as "<<newlambda<<"."<<endl
                   <<"Check "<<get_model_name()<<" model of "<<get_device_name();
            show_information_with_leading_time_stamp(sstream);
            break;
        }
    }

    newlambda = 0.5*(lambdalow+lambdahigh);

    return newlambda;
}

double AERD0::get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const
{
    double Cp0 = get_Cp(lambda, pitch_deg);
    double Cp1 = get_Cp(lambda+0.001, pitch_deg);

    return (Cp1-Cp0)*1e3;
}


string AERD0::get_model_name() const
{
    return "AERD0";
}

double AERD0::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double AERD0::get_double_data_with_name(string par_name) const
{
    return 0.0;
}

void AERD0::set_double_data_with_index(size_t index, double value)
{
    return;
}

void AERD0::set_double_data_with_name(string par_name, double value)
{
    return;
}

bool AERD0::setup_model_with_steps_string(string data)
{
    return true;
}

bool AERD0::setup_model_with_psse_string(string data)
{
    return true;
}

bool AERD0::setup_model_with_bpa_string(string data)
{
    return true;
}

void AERD0::initialize()
{
    ostringstream sstream;

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return;

    initialize_wind_turbine_blade_radius_and_gear_ratio();

    double pmax = get_total_wind_power_per_wt_generator_in_MW(get_wind_speed_in_mps());
    double cp_max = get_Cpmax(0.0);
    double pmech = gen->get_p_generation_in_MW()/gen->get_number_of_lumped_wt_generators()/get_gear_efficiency();
    if(pmax*cp_max<pmech)
    {
        sstream<<"Initialization error. Wind speed "<<get_wind_speed_in_mps()<<" m/s is not enough to generate power for "<<get_device_name()
               <<endl
               <<"Maximum power in wind is "<<pmax<<" MW, Cpmax is "<<cp_max<<" with best lambda "<<get_lambda_at_Cpmax(0.0)<<" at 0.0 pitch angle.";
        show_information_with_leading_time_stamp(sstream);
    }

    initialize_pitch_angle_and_turbine_speed();

    set_flag_model_initialized_as_true();
}

void AERD0::initialize_wind_turbine_blade_radius_and_gear_ratio()
{
    initialize_turbine_blade_radius_with_nominal_parameters();
    initialize_generator_to_turbine_gear_ratio();
}

void AERD0::initialize_turbine_blade_radius_with_nominal_parameters()
{
    ostringstream sstream;
    if(get_rated_power_per_wt_generator_in_MW()<=0.0)
    {
        sstream<<"Error. Wind turbine nominal power is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    if(get_power_system_base_frequency_in_Hz()<=0.0)
    {
        sstream<<"Error. Wind turbine nominal frequency is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    if(get_number_of_pole_pairs()==0)
    {
        sstream<<"Error. Wind turbine number of pole pairs is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    if(get_generator_to_turbine_gear_ratio()<=0.0)
    {
        sstream<<"Error. Wind turbine gear ratio is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    if(get_nominal_air_density_in_kgpm3()<=0.0)
    {
        sstream<<"Error. Wind turbine nominal air density is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    if(get_nominal_wind_speed_in_mps()<=0.0)
    {
        sstream<<"Error. Wind turbine nominal wind speed is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    if(get_gear_efficiency()<=0.0)
    {
        sstream<<"Error. Gear efficiency is not properly set before setup turbine blade radius.";
        show_information_with_leading_time_stamp(sstream);
    }

    double pn = get_rated_power_per_wt_generator_in_MW()*1e6;
    double eta = get_gear_efficiency();
    pn /= eta;

    double v = get_nominal_wind_speed_in_mps();
    double v3 =  v*v*v;
    double rou = get_nominal_air_density_in_kgpm3();
    double lambda_mppt = get_lambda_at_Cpmax(0.0);
    double cp_max = get_Cp(lambda_mppt, 0.0);
    double blade_area = 2.0*pn/(cp_max*rou*v3);
    double blade_radius = sqrt(blade_area/PI);
    set_turbine_blade_radius_in_m(blade_radius);
    return;
}

void AERD0::initialize_generator_to_turbine_gear_ratio()
{
    double lambda_max = get_lambda_at_Cpmax(0.0);
    double vwind = get_nominal_wind_speed_in_mps();
    double radius = get_turbine_blade_radius_in_m();

    double wt = lambda_max*vwind/radius;

    size_t n = get_number_of_pole_pairs();
    double fbase = get_power_system_base_frequency_in_Hz();
    double wg = 2.0*PI*fbase/n;
    double turnratio = wg/wt;
    set_generator_to_turbine_gear_ratio(turnratio);
    return;
}


void AERD0::initialize_pitch_angle_and_turbine_speed()
{
    ostringstream sstream;

    set_initial_pitch_angle_in_deg(0.0);
    initialize_turbine_speed();

    double wn = get_nominal_turbine_speed_in_rad_per_s();
    double w = get_initial_turbine_speed_in_rad_per_s();
    cout<<"initialized with 0.0 pitch, w = "<<w<<" rad/s, wn = "<<wn<<" rad/s"<<endl;

    double wmax = get_max_steady_state_turbine_speed_in_pu()*wn;
    double wmin = get_min_steady_state_turbine_speed_in_pu()*wn;
    if(w >= wmin and w <= wmax)
        return;

    if(w>wmax)
        set_initial_turbine_speed_in_rad_per_s(wmax);
    else
        set_initial_turbine_speed_in_rad_per_s(wmin);

    initialize_pitch_angle();
}

void AERD0::initialize_pitch_angle()
{
    ostringstream sstream;

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return;

    double D = get_damping_in_pu();
    double mbase = get_mbase_in_MVA();
    complex<double> zsource = get_source_impedance_in_pu_based_on_mbase();

    complex<double> selec = gen->get_complex_generation_in_MVA();
    complex<double> vterm = get_terminal_complex_voltage_in_pu();

    selec /= mbase;
    double iterm = abs(selec)/abs(vterm);
    selec += (iterm*iterm*zsource);

    selec *= mbase;

    double pelec = selec.real();
    size_t n = get_number_of_lumped_wt_generators();
    pelec /= n;

    double pn = get_rated_power_per_wt_generator_in_MW();

    if(pelec>pn)
    {
        sstream<<"Error when getting initial turbine speed of "<<get_model_name()<<" model of "<<get_device_name()<<". Initial power exceeds WT nominal power.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    double v = get_wind_speed_in_mps();
    double r = get_turbine_blade_radius_in_m();
    double wn = get_nominal_turbine_speed_in_rad_per_s();
    double w = get_initial_turbine_speed_in_rad_per_s();
    double dspeed = (w-wn)/wn;
    double pdamp = D*dspeed*mbase/n;
    double eta = get_gear_efficiency();
    double pmech = (pelec+pdamp)/eta;

    double pitchlow, pitchhigh;
    pitchlow = 0.0;
    pitchhigh = 10.0;

    double pwind;
    while(true)
    {
        set_initial_pitch_angle_in_deg(pitchhigh);
        pwind = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
        if(pwind<=pmech)
            break;
        else
        {
            pitchhigh *=2.0;
            if(pitchhigh>90.0)
                pitchhigh = 90.0;
        }
    }

    set_initial_pitch_angle_in_deg(pitchlow);
    double pwindlow = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
    set_initial_pitch_angle_in_deg(pitchhigh);
    double pwindhigh = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);

    //cout<<"pmech = "<<pmech<<" MW, rotor speed = "<<w<<" rad/s, wn = "<<wn<<" rad/s"<<endl;
    //cout<<pitchlow<<" deg, "<<pwindlow<<" MW"<<endl;
    //cout<<pitchhigh<<" deg, "<<pwindhigh<<" MW"<<endl;
    size_t iter_count = 0, iter_max = 50;

    while(true)
    {
        double pitchnew = 0.5*(pitchlow+pitchhigh);
        set_initial_pitch_angle_in_deg(pitchnew);
        double pnew = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
        //cout<<pitchnew<<" deg, "<<pnew<<" MW"<<endl;
        if(fabs(pnew-pmech)<FLOAT_EPSILON)
            break;
        if(pnew>pmech)
            pitchlow = pitchnew;
        if(pnew<pmech)
            pitchhigh = pitchnew;

        iter_count++;
        if(iter_count>iter_max)
        {
            sstream<<"Warning. Failed to get initial pitch angle in "<<iter_max<<" iterations."<<endl
                   <<"Pitch angle is initialized as "<<pnew<<" deg."<<endl
                   <<"Check "<<get_model_name()<<" model of "<<get_device_name();
            show_information_with_leading_time_stamp(sstream);
            break;
        }
    }
}

void AERD0::initialize_turbine_speed()
{
    ostringstream sstream;

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return;

    double D = get_damping_in_pu();
    double mbase = get_mbase_in_MVA();
    complex<double> zsource = get_source_impedance_in_pu_based_on_mbase();

    complex<double> selec = gen->get_complex_generation_in_MVA();
    complex<double> vterm = get_terminal_complex_voltage_in_pu();

    selec /= mbase;
    double iterm = abs(selec)/abs(vterm);
    selec += (iterm*iterm*zsource);

    selec *= mbase;

    double pelec = selec.real();
    size_t n = get_number_of_lumped_wt_generators();
    pelec /= n;

    double pn = get_rated_power_per_wt_generator_in_MW();

    if(pelec>pn)
    {
        sstream<<"Error when getting initial turbine speed of "<<get_model_name()<<" model of "<<get_device_name()<<". Initial power exceeds WT nominal power.";
        show_information_with_leading_time_stamp(sstream);
        return;
    }

    double eta = get_gear_efficiency();
    double pmech = pelec/eta;
    double pdamp = 0.0;

    double lambdamax = get_lambda_at_Cpmax(get_pitch_angle_in_deg());

    double v = get_wind_speed_in_mps();
    double r = get_turbine_blade_radius_in_m();
    double wn = get_nominal_turbine_speed_in_rad_per_s();

    double wmax = lambdamax*v/r;

    double wlow, whigh;
    if(get_overspeed_mode_flag()==false)
    {
        wlow = wmax*0.5;
        whigh = wmax;
        while(true)
        {
            if(get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(wlow)>pmech)
                wlow *=0.5;
            else
                break;
        }
    }
    else
    {
        wlow = wmax;
        whigh = wmax*2.0;
        while(true)
        {
            if(get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(whigh)>pmech)
                whigh *=2.0;
            else
                break;
        }
    }

    double plow = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(wlow);
    double phigh = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(whigh);
    cout<<"wlow = "<<wlow<<" rad/s, plow = "<<plow<<" MW"<<endl;
    cout<<"whigh = "<<whigh<<" rad/s, phigh = "<<phigh<<" MW"<<endl;

    double w = 0.0;
    size_t iter_count = 0, iter_max = 200;
    while(true)
    {
        double wnew = 0.5*(wlow+whigh);
        double pnew = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(wnew);
        cout<<"wnew = "<<wnew<<" rad/s, pnew = "<<pnew<<" MW, pmech = "<<pmech<<endl;
        double dspeed = (wnew-wn)/wn;
        pdamp = D*dspeed*mbase/n;
        cout<<"pdamp = "<<pdamp<<" MW"<<endl;
        pmech = (pelec+pdamp)/eta;
        if(fabs(pnew-pmech)<1e-6)
        {
            w = wnew;
            break;
        }
        if(get_overspeed_mode_flag()==false)
        {
            if(pnew>pmech)
                whigh = wnew;
            else
                wlow = wnew;
        }
        else
        {
            if(pnew>pmech)
                wlow = wnew;
            else
                whigh = wnew;
        }
        iter_count++;
        if(iter_count>iter_max)
        {
            w = wnew;
            sstream<<"Warning. Failed to get initial wt turbine speed in "<<iter_max<<" iterations."<<endl
                   <<"Turbine speed is initialized as "<<w<<" rad/s."<<endl
                   <<"Check "<<get_model_name()<<" model of "<<get_device_name();
            show_information_with_leading_time_stamp(sstream);
            break;
        }
    }
    set_initial_turbine_speed_in_rad_per_s(w);
}

void AERD0::run(DYNAMIC_MODE mode)
{
    ;
}

double AERD0::get_turbine_mechanical_power_per_wt_generator_in_MW() const
{
    double w = get_turbine_speed_in_rad_per_s();
    double pmech = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(w);
    double eta = get_gear_efficiency();
    return pmech*eta;
}

double AERD0::get_turbine_mechanical_power_in_MW() const
{
    return get_turbine_mechanical_power_per_wt_generator_in_MW()*get_number_of_lumped_wt_generators();
}

double AERD0::get_turbine_reference_speed_in_rad_per_s() const
{
    ostringstream sstream;

    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_GENERATOR_MODEL* wtgenmodel = gen->get_wt_generator_model();
    if(wtgenmodel==NULL)
        return 0.0;

    complex<double> selec = wtgenmodel->get_terminal_complex_power_in_MVA();
    double iterm = wtgenmodel->get_terminal_current_in_pu_based_on_mbase();
    complex<double> zsource = get_source_impedance_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();
    selec += (iterm*iterm*zsource)*mbase;

    size_t n = get_number_of_lumped_wt_generators();
    selec /= n;

    double pelec = selec.real();

    double pitch = get_pitch_angle_in_deg();
    double lambda = get_lambda_at_Cpmax(pitch);
    double vwind = get_wind_speed_in_mps();
    double r = get_turbine_blade_radius_in_m();
    double w_mppt = lambda*vwind/r;

    double cpmax = get_Cpmax(pitch);
    double pwind = get_total_wind_power_per_wt_generator_in_MW(vwind);
    if(pelec>pwind*cpmax)
    {
        sstream<<"Warning. Current electrical power generation of "<<get_device_name()<<" exceeds the maximum available wind power:"<<endl
               <<"Current electrical power generation = "<<pelec*n<<"MW. Maximum available wind power = "<<pwind*cpmax*n<<" MW"<<endl
               <<"Current wind speed = "<<vwind<<" m, pitch angle = "<<pitch<<" deg, Cpmax = "<<cpmax<<" at w_mpppt = "<<w_mppt<<" rad/s"<<endl
               <<"MPPT speed will be returned as speed reference: "<<w_mppt<<" rad/s.";
        show_information_with_leading_time_stamp(sstream);
        return w_mppt;
    }

    double wlow, whigh;
    if(get_overspeed_mode_flag()==false)
    {
        wlow = w_mppt*0.5;
        whigh = w_mppt;
    }
    else
    {
        wlow = w_mppt;
        whigh = w_mppt*2.0;
    }

    double plow = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(wlow);
    double phigh = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(whigh);

    double w = 0.0;
    size_t iter_count = 0, iter_max = 100;
    while(true)
    {
        double wnew = 0.5*(wlow+whigh);
        double pnew = get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(wnew);

        if(fabs(pnew-pelec)<1e-6)
        {
            w = wnew;
            break;
        }
        if(get_overspeed_mode_flag()==false)
        {
            if(pnew>pelec)
                whigh = wnew;
            else
                wlow = wnew;
        }
        else
        {
            if(pnew>pelec)
                wlow = wnew;
            else
                whigh = wnew;
        }
        iter_count++;
        if(iter_count>iter_max)
        {
            w = wnew;
            sstream<<"Warning. Failed to get reference wt turbine speed in "<<iter_max<<" iterations."<<endl
                   <<"Current electrical power generation = "<<pelec*n<<"MW. Maximum available wind power = "<<pwind*cpmax*n<<" MW"<<endl
                   <<"Current wind speed = "<<vwind<<" m, pitch angle = "<<pitch<<" deg, Cpmax = "<<cpmax<<" at w_mpppt = "<<w_mppt<<" rad/s"<<endl
                   <<"Reference turbine speed is returned as "<<w<<" rad/s."<<endl
                   <<"Check "<<get_model_name()<<" model of "<<get_device_name();
            show_information_with_leading_time_stamp(sstream);
            break;
        }
    }
    return w;
}

void AERD0::check()
{
    ;
}

void AERD0::clear()
{
    ;
}

void AERD0::report()
{
    ;
}

void AERD0::save()
{
    ;
}

string AERD0::get_standard_model_string() const
{
    return "";
}

size_t AERD0::get_variable_index_from_variable_name(string var_name)
{
    return 0;
}

string AERD0::get_variable_name_from_variable_index(size_t var_index)
{
    return "";
}

double AERD0::get_variable_with_index(size_t var_index)
{
    return 0.0;
}

double AERD0::get_variable_with_name(string var_name)
{
    return 0.0;
}

string AERD0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string AERD0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string AERD0::get_dynamic_data_in_steps_format() const
{
    return "";
}