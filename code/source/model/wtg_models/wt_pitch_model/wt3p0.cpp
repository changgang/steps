#include "header/model/wtg_models/wt_pitch_model/wt3p0.h"
#include "header/basic/utility.h"

static vector<string> MODEL_VARIABLE_TABLE{ "PITCH ANGLE IN DEG", //0
                                            "SPEED REFERENCE IN PU",      //1
                                            "POWER REFERENCE IN PU",     //2
                                            "FREQUENCY REFERENCE IN PU",      //3
                                            "STATE@SPEED CONTROLLER",     //4
                                            "STATE@POWER CONTROLLER",     //5
                                            "STATE@FREQUENCY CONTROLLER",     //6
                                            "STATE@PITCH INTEGRATOR",       //7
                                            };//8

WT3P0::WT3P0()
{
    clear();
}

WT3P0::~WT3P0()
{
    clear();
}

void WT3P0::clear()
{
    speed_controller.set_limiter_type(NO_LIMITER);

    power_controller.set_limiter_type(NO_LIMITER);

    frequency_controller.set_limiter_type(NO_LIMITER);

    pitch_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    set_ratePitchmax_in_deg_per_s(0.0);
}

void WT3P0::copy_from_const_model(const WT3P0& model)
{
    clear();

    //this->set_power_system_database(model.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    set_Tp_in_s(model.get_Tp_in_s());
    set_Kp_speed_controller(model.get_Kp_speed_controller());
    set_Ki_speed_controller(model.get_Ki_speed_controller());
    set_Kp_power_controller(model.get_Kp_power_controller());
    set_Ki_power_controller(model.get_Ki_power_controller());
    set_Kp_frequency_controller(model.get_Kp_frequency_controller());
    set_Ki_frequency_controller(model.get_Ki_frequency_controller());
    set_Pitchmax_in_deg(model.get_Pitchmax_in_deg());
    set_Pitchmin_in_deg(model.get_Pitchmin_in_deg());
    set_ratePitchmax_in_deg_per_s(model.get_ratePitchmax_in_deg_per_s());

    set_power_reference_in_pu(model.get_power_reference_in_pu());
}

WT3P0::WT3P0(const WT3P0&model) : WT_PITCH_MODEL()
{
    copy_from_const_model(model);
}

WT3P0& WT3P0::operator=(const WT3P0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string WT3P0::get_model_name() const
{
    return "WT3P0";
}

double WT3P0::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double WT3P0::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PITCHMAX")
        return get_Pitchmax_in_deg();
    if(par_name=="PITCHMIN")
        return get_Pitchmin_in_deg();

    return 0.0;
}

void WT3P0::set_double_data_with_index(size_t index, double value)
{
    if(index==0)
        return;
}

void WT3P0::set_double_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PITCHMAX")
        return set_Pitchmax_in_deg(value);
    if(par_name=="PITCHMIN")
        return set_Pitchmin_in_deg(value);
}

void WT3P0::set_Tp_in_s(double T)
{
    pitch_integrator.set_T_in_s(T);
}

void WT3P0::set_Kp_speed_controller(double K)
{
    speed_controller.set_Kp(K);
}

void WT3P0::set_Ki_speed_controller(double K)
{
    speed_controller.set_Ki(K);
}

void WT3P0::set_Kp_power_controller(double K)
{
    power_controller.set_Kp(K);
}

void WT3P0::set_Ki_power_controller(double K)
{
    power_controller.set_Ki(K);
}

void WT3P0::set_Kp_frequency_controller(double K)
{
    frequency_controller.set_Kp(K);
}

void WT3P0::set_Ki_frequency_controller(double K)
{
    frequency_controller.set_Ki(K);
}

void WT3P0::set_Pitchmax_in_deg(double P)
{
    pitch_integrator.set_upper_limit(P);
}

void WT3P0::set_Pitchmin_in_deg(double P)
{
    pitch_integrator.set_lower_limit(P);
}

void WT3P0::set_ratePitchmax_in_deg_per_s(double rP)
{
    ratePitchmax = rP;
}

double WT3P0::get_Tp_in_s() const
{
    return pitch_integrator.get_T_in_s();
}

double WT3P0::get_Kp_speed_controller() const
{
    return speed_controller.get_Kp();
}

double WT3P0::get_Ki_speed_controller() const
{
    return speed_controller.get_Ki();
}

double WT3P0::get_Kp_power_controller() const
{
    return power_controller.get_Kp();
}

double WT3P0::get_Ki_power_controller() const
{
    return power_controller.get_Ki();
}

double WT3P0::get_Kp_frequency_controller() const
{
    return frequency_controller.get_Kp();
}

double WT3P0::get_Ki_frequency_controller() const
{
    return frequency_controller.get_Ki();
}

double WT3P0::get_Pitchmax_in_deg() const
{
    return pitch_integrator.get_upper_limit();
}

double WT3P0::get_Pitchmin_in_deg() const
{
    return pitch_integrator.get_lower_limit();
}

double WT3P0::get_ratePitchmax_in_deg_per_s() const
{
    return ratePitchmax;
}


bool WT3P0::setup_model_with_steps_string(string data)
{
    return false;
}

bool WT3P0::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<14)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double tp, kps, kis, kpp, kip, kpf, kif, pmax, pmin, rpmax, pref;

    size_t i=3;
    tp = get_double_data(dyrdata[i],"0.0"); i++;
    kps = get_double_data(dyrdata[i],"0.0"); i++;
    kis = get_double_data(dyrdata[i],"0.0"); i++;
    kpp = get_double_data(dyrdata[i],"0.0"); i++;
    kip = get_double_data(dyrdata[i],"0.0"); i++;
    kpf = get_double_data(dyrdata[i],"0.0"); i++;
    kif = get_double_data(dyrdata[i],"0.0"); i++;
    pmax = get_double_data(dyrdata[i],"0.0"); i++;
    pmin = get_double_data(dyrdata[i],"0.0"); i++;
    rpmax = get_double_data(dyrdata[i],"0.0"); i++;
    pref = get_double_data(dyrdata[i],"0.0");

    set_Tp_in_s(tp);
    set_Kp_speed_controller(kps);
    set_Ki_speed_controller(kis);
    set_Kp_power_controller(kpp);
    set_Ki_power_controller(kip);
    set_Kp_frequency_controller(kpf);
    set_Ki_frequency_controller(kif);
    set_Pitchmax_in_deg(pmax);
    set_Pitchmin_in_deg(pmin);
    set_ratePitchmax_in_deg_per_s(rpmax);
    set_power_reference_in_pu(pref);

    is_successful = true;

    return is_successful;
}

bool WT3P0::setup_model_with_bpa_string(string data)
{
    return false;
}


void WT3P0::initialize()
{
    ostringstream sstream;
    double pitch0 = get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model();

    pitch_integrator.set_output(pitch0);
    pitch_integrator.initialize();

    double pmax = get_Pitchmax_in_deg();
    double pmin = get_Pitchmin_in_deg();
    if(pitch0>pmax)
    {
        sstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
          <<"Pitch angle is "<<pitch0<<" deg, and Pitchmax is "<<pmax<<" deg.";
        show_information_with_leading_time_stamp(sstream);
    }
    if(pitch0<pmin)
    {
        sstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
          <<"Pitch angle is "<<pitch0<<" deg, and Pitchmin is "<<pmin<<" deg.";
        show_information_with_leading_time_stamp(sstream);
    }

    speed_controller.set_output(pitch0);
    speed_controller.initialize();

    power_controller.set_output(0.0);
    power_controller.initialize();

    frequency_controller.set_output(0.0);
    frequency_controller.initialize();

    double speed = get_wt_generator_speed_in_pu();
    set_speed_reference_in_pu(speed);

    set_frequency_reference_in_pu(1.0);

    // power reference was set when loading data

    set_flag_model_initialized_as_true();
}

void WT3P0::run(DYNAMIC_MODE mode)
{
    double speed = get_wt_generator_speed_in_pu();
    double speedref = get_speed_reference_in_pu();

    speed_controller.set_input(speed-speedref);
    speed_controller.run(mode);

    double freq = get_bus_frequency_in_pu();
    double fref = get_frequency_reference_in_pu();

    frequency_controller.set_input(freq-fref);
    frequency_controller.run(mode);

    double ipcmd = get_wt_active_power_command_in_pu();
    double pref = get_power_reference_in_pu();
    double perror = ipcmd-pref;
    if(perror<0.0)
        perror = 0.0;

    power_controller.set_input(perror);
    power_controller.run(mode);

    double rate = speed_controller.get_output()+power_controller.get_output()+frequency_controller.get_output();
    rate -= pitch_integrator.get_output();

    double ratemax = get_ratePitchmax_in_deg_per_s();
    if(rate>ratemax)
        rate = ratemax;
    if(rate<-ratemax)
        rate = -ratemax;

    pitch_integrator.set_input(rate);
    pitch_integrator.run(mode);


    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double WT3P0::get_pitch_angle_in_deg() const
{
    return pitch_integrator.get_output();
}

void WT3P0::check()
{
    ;
}

void WT3P0::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}
void WT3P0::save()
{
    ;
}
string WT3P0::get_standard_model_string() const
{
    ostringstream sstream;

    double tp = get_Tp_in_s();
    double kps = get_Kp_speed_controller();
    double kis = get_Ki_speed_controller();
    double kpp = get_Kp_power_controller();
    double kip = get_Ki_power_controller();
    double kpf = get_Kp_frequency_controller();
    double kif = get_Ki_frequency_controller();
    double pmax = get_Pitchmax_in_deg();
    double pmin = get_Pitchmin_in_deg();
    double rpmax = get_ratePitchmax_in_deg_per_s();
    double pref = get_power_reference_in_pu();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    sstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<tp<<", "
      <<setw(8)<<setprecision(6)<<kps<<", "
      <<setw(8)<<setprecision(6)<<kis<<", "
      <<setw(8)<<setprecision(6)<<kpp<<", "
      <<setw(8)<<setprecision(6)<<kip<<", "
      <<setw(8)<<setprecision(6)<<kpf<<", "
      <<setw(8)<<setprecision(6)<<kif<<", "
      <<setw(8)<<setprecision(6)<<pmax<<", "
      <<setw(8)<<setprecision(6)<<pmin<<", "
      <<setw(8)<<setprecision(6)<<rpmax<<", "
      <<setw(8)<<setprecision(6)<<pref<<" /";
    return sstream.str();
}

size_t WT3P0::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string WT3P0::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double WT3P0::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double WT3P0::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "PITCH ANGLE IN DEG")
        return get_pitch_angle_in_deg();

    if(var_name == "SPEED REFERENCE IN PU")
        return get_speed_reference_in_pu();

    if(var_name == "POWER REFERENCE IN PU")
        return get_power_reference_in_pu();

    if(var_name == "FREQUENCY REFERENCE IN PU")
        return get_frequency_reference_in_pu();

    if(var_name == "STATE@SPEED CONTROLLER")
        return speed_controller.get_state();

    if(var_name == "STATE@POWER CONTROLLER")
        return power_controller.get_state();

    if(var_name == "STATE@FREQUENCY CONTROLLER")
        return frequency_controller.get_state();

    if(var_name == "STATE@PITCH INTEGRATOR")
        return pitch_integrator.get_state();

    return 0.0;
}

string WT3P0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3P0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3P0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
