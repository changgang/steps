#include "header/model/wtg_models/wt_turbine_model/wt3t1.h"
#include "header/basic/utility.h"

static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@GOVERNOR",     //4
                                            "STATE@TURBINE",       //5
                                            };//8

WT3T1::WT3T1()
{
    clear();
}

WT3T1::~WT3T1()
{
    clear();
}

void WT3T1::clear()
{
}

void WT3T1::copy_from_const_model(const WT3T1& model)
{
    clear();

    //this->set_power_system_database(model.get_power_system_database());
    //this->set_device_id(model.get_device_id());

}

WT3T1::WT3T1(const WT3T1&model) : WT_TURBINE_MODEL()
{
    copy_from_const_model(model);
}

WT3T1& WT3T1::operator=(const WT3T1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string WT3T1::get_model_name() const
{
    return "WT3T1";
}

double WT3T1::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double WT3T1::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="VMAX")
        return 0.0;

    return 0.0;
}

void WT3T1::set_double_data_with_index(size_t index, double value)
{
    if(index==0)
        return;
}

void WT3T1::set_double_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="VMAX")
        return;
}

void WT3T1::set_Htotal_in_s(double H)
{
    Htotal = H;
}

void WT3T1::set_Damp_in_pu(double D)
{
    Damp = D;
}

void WT3T1::set_Kaero(double K)
{
    Kaero = K;
}

void WT3T1::set_Theta2_in_deg(double theta)
{
    Theta2 = theta;
}

void WT3T1::set_Htfrac_in_pu(double frac)
{
    Htfrac = frac;
}

void WT3T1::set_Freq1_in_Hz(double f)
{
    Freq1 = f;
}

void WT3T1::set_Dshaft_in_pu(double D)
{
    Dshaft = D;
}

double WT3T1::get_Htotal_in_s() const
{
    return Htotal;
}

double WT3T1::get_Damp_in_pu() const
{
    return Damp;
}

double WT3T1::get_Kaero() const
{
    return Kaero;
}

double WT3T1::get_Theta2_in_deg() const
{
    return Theta2;
}

double WT3T1::get_Htfrac_in_pu() const
{
    return Htfrac;
}

double WT3T1::get_Freq1_in_Hz() const
{
    return Freq1;
}

double WT3T1::get_Dshaft_in_pu() const
{
    return Dshaft;
}

bool WT3T1::setup_model_with_steps_string(string data)
{
    return false;
}

bool WT3T1::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<11)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double vw, h, damp, kaero, theta2, htfrac, freq1, dshaft;

    size_t i=3;
    vw = get_double_data(dyrdata[i],"0.0"); i++;
    h = get_double_data(dyrdata[i],"0.0"); i++;
    damp = get_double_data(dyrdata[i],"0.0"); i++;
    kaero = get_double_data(dyrdata[i],"0.0"); i++;
    theta2 = get_double_data(dyrdata[i],"0.0"); i++;
    htfrac = get_double_data(dyrdata[i],"0.0"); i++;
    freq1 = get_double_data(dyrdata[i],"0.0"); i++;
    dshaft = get_double_data(dyrdata[i],"0.0");

    set_initial_wind_speed_in_pu(vw);
    set_Htotal_in_s(h);
    set_Damp_in_pu(damp);
    set_Kaero(kaero);
    set_Theta2_in_deg(theta2);
    set_Htfrac_in_pu(htfrac);
    set_Freq1_in_Hz(freq1);
    set_Dshaft_in_pu(dshaft);

    is_successful = true;

    return is_successful;
}

bool WT3T1::setup_model_with_bpa_string(string data)
{
    return false;
}


void WT3T1::initialize()
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return;

    size_t bus = gen->get_source_bus();

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
        return;

    double wbase = 2.0*PI*psdb->get_system_base_frequency_in_Hz();

    generator_rotor_angle.set_T_in_s(1.0/wbase);
    generator_rotor_angle.set_output(psdb->get_bus_angle_in_rad(bus));
    generator_rotor_angle.initialize();

    double speed = get_initial_wind_turbine_speed_in_pu();

    double htotal = get_Htotal_in_s();
    double htfrac = get_Htfrac_in_pu();
    if(htfrac<=0.0)
    {
        generator_inertia.set_T_in_s(2.0*htotal);

    }
    else
    {
        double hg = (1.0-htfrac)*htotal;
        double ht = htfrac*htotal;

        generator_inertia.set_T_in_s(2.0*hg);
        turbine_inertia.set_T_in_s(2.0*ht);

        double f = 2.0*PI*get_Freq1_in_Hz();

        double kshaft =(2.0*ht*hg*f*f)/(htotal*wbase);

        shaft_twist.set_T_in_s(1.0/kshaft);
    }


    set_flag_model_initialized_as_true();
}

void WT3T1::run(DYNAMIC_MODE mode)
{
    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = Pref-speed;
    governor.set_input(input);
    governor.run(mode);

    input = governor.get_output();
    turbine.set_input(input);
    turbine.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}
double WT3T1::get_mechanical_power_in_pu_based_on_mbase() const
{
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();
    double D = get_D();

    return turbine.get_output()-D*speed;
}

double WT3T1::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Valvemax_in_pu();
}

double WT3T1::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Valvemin_in_pu();
}

void WT3T1::check()
{
    ;
}

void WT3T1::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}
void WT3T1::save()
{
    ;
}
string WT3T1::get_standard_model_string() const
{
    ostringstream sstream;

    double R = get_R();
    double T1 = get_T1_in_s();
    double Vmax = get_Valvemax_in_pu();
    double Vmin = get_Valvemin_in_pu();
    double T2 = get_T2_in_s();
    double T3 = get_T3_in_s();
    double D = get_D();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    sstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<R<<", "
      <<setw(8)<<setprecision(6)<<T1<<", "
      <<setw(8)<<setprecision(6)<<Vmax<<", "
      <<setw(8)<<setprecision(6)<<Vmin<<", "
      <<setw(8)<<setprecision(6)<<T2<<", "
      <<setw(8)<<setprecision(6)<<T3<<", "
      <<setw(8)<<setprecision(6)<<D<<"  /";
    return sstream.str();
}

size_t WT3T1::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string WT3T1::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double WT3T1::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double WT3T1::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "GENERATOR MECHANICAL POWER IN PU")
        return get_mechanical_power_in_pu_based_on_mbase();

    if(var_name == "GENERATOR MECHANICAL POWER IN MW")
    {
        WT_GENERATOR* generator = get_wt_generator_pointer();
        return get_mechanical_power_in_pu_based_on_mbase()*generator->get_mbase_in_MVA();
    }

    if(var_name == "MECHANICAL POWER REFERENCE IN PU")
        return get_mechanical_power_reference_in_pu_based_on_mbase();

    if(var_name == "GENERATOR ROTOR SPEED DEVIATION IN PU")
        return get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    if(var_name == "STATE@GOVERNOR")
        return governor.get_state();

    if(var_name == "STATE@TURBINE")
        return turbine.get_state();

    return 0.0;
}

string WT3T1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3T1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3T1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
