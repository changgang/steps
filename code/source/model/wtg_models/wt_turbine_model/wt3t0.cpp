#include "header/model/wtg_models/wt_turbine_model/wt3t0.h"
#include "header/basic/utility.h"

static vector<string> MODEL_VARIABLE_TABLE{ "GENERATOR MECHANICAL POWER IN PU", //0
                                            "GENERATOR MECHANICAL POWER IN MW",      //1
                                            "MECHANICAL POWER REFERENCE IN PU",      //2
                                            "GENERATOR ROTOR SPEED DEVIATION IN PU",     //3
                                            "STATE@GOVERNOR",     //4
                                            "STATE@TURBINE",       //5
                                            };//8

WT3T0::WT3T0()
{
    clear();
}

WT3T0::~WT3T0()
{
    clear();
}

void WT3T0::clear()
{
    ;
}

void WT3T0::copy_from_const_model(const WT3T0& model)
{
    clear();

    set_damping_in_pu(model.get_damping_in_pu());
    set_Hturbine_in_s(model.get_Hturbine_in_s());
    set_Hgenerator_in_s(model.get_Hgenerator_in_s());
    set_Kshaft_in_pu(model.get_Kshaft_in_pu());
    set_Dshaft_in_pu(model.get_Dshaft_in_pu());
}

WT3T0::WT3T0(const WT3T0& model) : WT_TURBINE_MODEL()
{
    copy_from_const_model(model);
}

WT3T0& WT3T0::operator=(const WT3T0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string WT3T0::get_model_name() const
{
    return "WT3T0";
}


void WT3T0::set_Hturbine_in_s(double H)
{
    turbine_inertia_block.set_T_in_s(2.0*H);
}

void WT3T0::set_Hgenerator_in_s(double H)
{
    generator_inertia_block.set_T_in_s(2.0*H);
}

void WT3T0::set_Kshaft_in_pu(double K)
{
    shaft_twist_block.set_T_in_s(1.0/K);
}

void WT3T0::set_Dshaft_in_pu(double D)
{
    Dshaft = D;
}

double WT3T0::get_Hturbine_in_s() const
{
    return 2.0*turbine_inertia_block.get_T_in_s();
}

double WT3T0::get_Hgenerator_in_s() const
{
    return 2.0*generator_inertia_block.get_T_in_s();
}

double WT3T0::get_Kshaft_in_pu() const
{
    return 1.0/shaft_twist_block.get_T_in_s();
}

double WT3T0::get_Dshaft_in_pu() const
{
    return Dshaft;
}

double WT3T0::get_double_data_with_index(size_t index) const
{
    return 0.0;
}

double WT3T0::get_double_data_with_name(string par_name) const
{
    return 0.0;
}

void WT3T0::set_double_data_with_index(size_t index, double value)
{
    return;
}

void WT3T0::set_double_data_with_name(string par_name, double value)
{
    return;
}

bool WT3T0::setup_model_with_steps_string(string data)
{
    return false;
}

bool WT3T0::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<8)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    double ht, hg, damp, kshaft, dshaft;

    size_t i=3;
    ht = get_double_data(dyrdata[i],"0.0"); i++;
    hg = get_double_data(dyrdata[i],"0.0"); i++;
    damp = get_double_data(dyrdata[i],"0.0"); i++;
    kshaft = get_double_data(dyrdata[i],"0.0"); i++;
    dshaft = get_double_data(dyrdata[i],"0.0");

    set_Hturbine_in_s(ht);
    set_Hgenerator_in_s(hg);
    set_damping_in_pu(damp);
    set_Kshaft_in_pu(kshaft);
    set_Dshaft_in_pu(dshaft);

    is_successful = true;

    return is_successful;
}

bool WT3T0::setup_model_with_bpa_string(string data)
{
    return false;
}


void WT3T0::initialize()
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
        return;

    WT_AERODYNAMIC_MODEL* aero_model = gen->get_wt_aerodynamic_model();
    if(aero_model==NULL)
        return;

    if(not aero_model->is_model_initialized())
        aero_model->initialize();

    //double speed = aero_model->get_initial_speed_in_pu();


    size_t bus = gen->get_generator_bus();

    double wbase = 2.0*PI*psdb->get_system_base_frequency_in_Hz();

    generator_rotor_angle_block.set_T_in_s(1.0/wbase);
    generator_rotor_angle_block.set_output(psdb->get_bus_angle_in_rad(bus));
    generator_rotor_angle_block.initialize();

    double speed = aero_model->get_turbine_reference_speed_in_rad_per_s();
    double slip = speed-1.0;

    generator_inertia_block.set_output(slip);
    generator_inertia_block.initialize();

    turbine_inertia_block.set_output(slip);
    double hturbine = get_Hturbine_in_s();
    if(fabs(hturbine)>FLOAT_EPSILON)
        turbine_inertia_block.initialize();

    WT_GENERATOR* generator = get_wt_generator_pointer();
    double pelec = generator->get_p_generation_in_MW()/get_mbase_in_MVA();
    double telec = pelec/speed;
    double tmech = get_damping_in_pu()*slip+telec;

    shaft_twist_block.set_output(tmech);
    shaft_twist_block.initialize();

    set_flag_model_initialized_as_true();
}

void WT3T0::run(DYNAMIC_MODE mode)
{
    ;

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double WT3T0::get_wind_turbine_generator_speed_in_pu() const
{
    return generator_inertia_block.get_output();
}

double WT3T0::get_wind_turbine_generator_rotor_angle_in_deg() const
{
    return rad2deg(get_wind_turbine_generator_rotor_angle_in_rad());
}

double WT3T0::get_wind_turbine_generator_rotor_angle_in_rad() const
{
    return generator_rotor_angle_block.get_output();
}

void WT3T0::check()
{
    ;
}

void WT3T0::report()
{
    ostringstream sstream;
    sstream<<get_standard_model_string();
    show_information_with_leading_time_stamp(sstream);
}

void WT3T0::save()
{
    ;
}
string WT3T0::get_standard_model_string() const
{
    return "";

    /*ostringstream sstream;

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
    */
}

size_t WT3T0::get_variable_index_from_variable_name(string var_name)
{
    return MODEL::get_variable_index_from_variable_name(var_name, MODEL_VARIABLE_TABLE);
}
string WT3T0::get_variable_name_from_variable_index(size_t var_index)
{
    return MODEL::get_variable_name_from_variable_index(var_index, MODEL_VARIABLE_TABLE);
}
double WT3T0::get_variable_with_index(size_t var_index)
{
    string var_name = get_variable_name_from_variable_index(var_index);
    return get_variable_with_name(var_name);
}
double WT3T0::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);

    if(var_name == "GENERATOR MECHANICAL POWER IN PU")
        return 0.0;

    return 0.0;
}

string WT3T0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string WT3T0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string WT3T0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
