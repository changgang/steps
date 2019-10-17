#include "header/model/sg_models/turbine_load_controller_model/lcfb1.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <cstdio>
#include "header/basic/utility.h"
#include <vector>
LCFB1::LCFB1()
{
    clear();
}

LCFB1::~LCFB1()
{
    //clear();
}

void LCFB1::clear()
{
    set_model_float_parameter_count(9);
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_frequency_regulation_flag(false);
    set_power_regulation_flag(false);

    Fb = 0.0;
    db = 0.0;
    Emax = INFINITE_THRESHOLD;

    Pelec_sensor.set_limiter_type(NO_LIMITER);
    error_integrator.set_limiter_type(NON_WINDUP_LIMITER);
    error_integrator.set_upper_limit(INFINITE_THRESHOLD);
    error_integrator.set_lower_limit(-INFINITE_THRESHOLD);

    set_Pref0(0.0);
    set_Pelec0(0.0);

}

void LCFB1::copy_from_const_model(const LCFB1& model)
{
    clear();

    this->set_frequency_regulation_flag(model.get_frequency_regulation_flag());
    this->set_power_regulation_flag(model.get_power_regulation_flag());
    this->set_Fb(model.get_Fb());
    this->set_Tpelec_in_s(model.get_Tpelec_in_s());
    this->set_db(model.get_db());
    this->set_Emax(model.get_Emax());
    this->set_Kp(model.get_Kp());
    this->set_Ki(model.get_Ki());
    this->set_Irmax(model.get_Irmax());
}

LCFB1::LCFB1(const LCFB1& model) : TURBINE_LOAD_CONTROLLER_MODEL()
{
    copy_from_const_model(model);
}

LCFB1& LCFB1::operator=(const LCFB1& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}


string LCFB1::get_model_name() const
{
    return "LCFB1";
}

void LCFB1::set_frequency_regulation_flag(bool flag)
{
    frequency_regulation_flag = flag;
}
void LCFB1::set_power_regulation_flag(bool flag)
{
    power_regulation_flag = flag;
}

void LCFB1::set_Fb(double f)
{
    Fb = f;
}

void LCFB1::set_Tpelec_in_s(double t)
{
    Pelec_sensor.set_T_in_s(t);
}

void LCFB1::set_db(double db)
{
    if(db<0.0)
        db = -db;

    this->db = db;
}

void LCFB1::set_Emax(double e)
{
    if(e<0.0)
        e = -e;
    this->Emax = e;
}

void LCFB1::set_Kp(double k)
{
    this->Kp = k;
}

void LCFB1::set_Ki(double k)
{
    error_integrator.set_T_in_s(1.0/k);
}

void LCFB1::set_Irmax(double i)
{
    error_integrator.set_upper_limit(i);
    error_integrator.set_lower_limit(-i);
}

bool LCFB1::get_frequency_regulation_flag() const
{
    return frequency_regulation_flag;
}

bool LCFB1::get_power_regulation_flag() const
{
    return power_regulation_flag;
}

double LCFB1::get_Fb() const
{
    return Fb;
}

double LCFB1::get_Tpelec_in_s() const
{
    return Pelec_sensor.get_T_in_s();
}
double LCFB1::get_db() const
{
    return db;
}
double LCFB1::get_Emax() const
{
    return Emax;
}
double LCFB1::get_Kp() const
{
    return Kp;
}
double LCFB1::get_Ki() const
{
    return 1.0/error_integrator.get_T_in_s();
}
double LCFB1::get_Irmax() const
{
    return error_integrator.get_upper_limit();
}


void LCFB1::set_Pref0(double p)
{
    Pref0 = p;
}

void LCFB1::set_Pelec0(double p)
{
    Pelec0 = p;
}

double LCFB1::get_Pref0() const
{
    return Pref0;
}

double LCFB1::get_Pelec0() const
{
    return Pelec0;
}

bool LCFB1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=12)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            size_t frequency_flag, power_flag;
            double fb, tpelec, db, emax, kp, ki, irmax;

            size_t i=3;
            frequency_flag = get_integer_data(data[i],"0"); i++;
            power_flag = get_integer_data(data[i],"0"); i++;

            fb = get_double_data(data[i],"0.0"); i++;
            tpelec = get_double_data(data[i],"0.0"); i++;
            db = get_double_data(data[i],"0.0"); i++;
            emax = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            ki = get_double_data(data[i],"0.0"); i++;
            irmax = get_double_data(data[i],"0.0");

            set_frequency_regulation_flag((frequency_flag==0?false:true));
            set_power_regulation_flag((power_flag==0?false:true));
            set_Fb(fb);
            set_Tpelec_in_s(tpelec);
            set_db(db);
            set_Emax(emax);
            set_Kp(kp);
            set_Ki(ki);
            set_Irmax(irmax);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool LCFB1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool LCFB1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void LCFB1::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    Pelec_sensor.set_toolkit(toolkit);
    error_integrator.set_toolkit(toolkit);
}

void LCFB1::initialize()
{
    ostringstream osstream;

    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        setup_block_toolkit_and_parameters();

        double pref = get_initial_mechanical_power_reference_in_pu_based_on_mbase_from_turbine_governor_model();
        set_Pref0(pref);

        error_integrator.set_output(0.0);
        error_integrator.initialize();

        double pelec = get_terminal_active_power_in_pu_based_on_mbase_from_generator_model();
        Pelec_sensor.set_output(pelec);
        Pelec_sensor.initialize();

        set_Pelec0(pelec);

        set_flag_model_initialized_as_true();
    }
    else
    {
        deactivate_model();
        set_flag_model_initialized_as_true();
    }
}

void LCFB1::run(DYNAMIC_MODE mode)
{
    if(not is_model_active())
        return;

    double pelec = get_terminal_active_power_in_pu_based_on_mbase_from_generator_model();
    Pelec_sensor.set_input(pelec);
    Pelec_sensor.run(mode);

    double speed = get_rotor_speed_deviation_in_pu_from_generator_model();

    double error = 0.0;
    if(get_power_regulation_flag()==true)
        error = get_Pelec0() - Pelec_sensor.get_output();

    if(get_frequency_regulation_flag()==true)
        error -= (speed*get_Fb());

    double db = get_db();
    double emax = get_Emax();

    if(error>=-db and error<=db)
        error = 0.0;
    else
    {
        if(error>db)
        {
            error -= db;
            if(error>emax) error = emax;
        }
        else
        {
            error -= (-db);
            if(error<-emax) error = -emax;
        }
    }

    error_integrator.set_input(error);
    error_integrator.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double LCFB1::get_mechanical_power_reference_in_pu_based_on_mbase() const
{
    double speed = get_rotor_speed_deviation_in_pu_from_generator_model();

    double error = 0.0;
    if(get_power_regulation_flag()==true)
        error = get_Pelec0() - Pelec_sensor.get_output();

    if(get_frequency_regulation_flag()==true)
        error -= (speed*get_Fb());

    double db = get_db();
    double emax = get_Emax();

    if(error>=-db or error<=db)
        error = 0.0;
    else
    {
        if(error>db)
        {
            error -= db;
            if(error>emax) error = emax;
        }
        else
        {
            error -= (-db);
            if(error<-emax) error = -emax;
        }
    }

    double preg = get_Kp()*error+error_integrator.get_output();
    double irmax = get_Irmax();
    if(preg>irmax) preg = irmax;
    else
        if(preg<-irmax) preg = -irmax;

    return preg+get_Pref0();
}

void LCFB1::check()
{
    ;
}

void LCFB1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void LCFB1::save()
{
    string model = get_standard_psse_string();
    return;
}

string LCFB1::get_standard_psse_string() const
{
    ostringstream osstream;

    bool fflag = get_frequency_regulation_flag();
    bool pflag = get_power_regulation_flag();
    double fb = get_Fb();
    double tpelec = get_Tpelec_in_s();
    double db = get_db();
    double emax = get_Emax();
    double kp = get_Kp();
    double ki = get_Ki();
    double irmax = get_Irmax();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<(fflag==false?0:1)<<", "
            <<setw(8)<<(pflag==false?0:1)<<", "
            <<setw(8)<<setprecision(6)<<fb<<", "
            <<setw(8)<<setprecision(6)<<tpelec<<", "
            <<setw(8)<<setprecision(6)<<db<<", "
            <<setw(8)<<setprecision(6)<<emax<<", "
            <<setw(8)<<setprecision(6)<<kp<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<ki<<", "
            <<setw(8)<<setprecision(6)<<irmax<<" /";

    return osstream.str();
}

void LCFB1::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("F_FLAG", i); i++;
    add_model_data_name_and_index_pair("P_FLAG", i); i++;
    add_model_data_name_and_index_pair("FB", i); i++;
    add_model_data_name_and_index_pair("TPELEC", i); i++;
    add_model_data_name_and_index_pair("DB", i); i++;
    add_model_data_name_and_index_pair("EMAX", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KI", i); i++;
    add_model_data_name_and_index_pair("IRMAX", i); i++;
}

double LCFB1::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name == "F_FLAG")
        return get_frequency_regulation_flag();
    if(par_name == "P_FLAG")
        return get_power_regulation_flag();
    if(par_name=="FB")
        return get_Fb();
    if(par_name=="TPELEC")
        return get_Tpelec_in_s();
    if(par_name=="DB")
        return get_db();
    if(par_name=="EMAX")
        return get_Emax();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="KI")
        return get_Ki();
    if(par_name=="IRMAX")
        return get_Irmax();

    return 0.0;
}

void LCFB1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name == "F_FLAG")
        return set_frequency_regulation_flag(bool(value));
    if(par_name == "P_FLAG")
        return set_power_regulation_flag(bool(value));
    if(par_name=="FB")
        return set_Fb(value);
    if(par_name=="TPELEC")
        return set_Tpelec_in_s(value);
    if(par_name=="DB")
        return set_db(value);
    if(par_name=="EMAX")
        return set_Emax(value);
    if(par_name=="KP")
        return set_Kp(value);
    if(par_name=="KI")
        return set_Ki(value);
    if(par_name=="IRMAX")
        return set_Irmax(value);

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

void LCFB1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("PREF0", i); i++;
    add_model_inernal_variable_name_and_index_pair("PELEC0", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@POWER SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@ERROR INTEGRATOR", i);
}

double LCFB1::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "PREF0")
        return get_Pref0();

    if(var_name == "PELEC0")
        return get_Pelec0();

    if(var_name == "STATE@POWER SENSOR")
        return Pelec_sensor.get_state();

    if(var_name == "STATE@ERROR INTEGRATOR")
        return error_integrator.get_state();

    return 0.0;
}



string LCFB1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string LCFB1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string LCFB1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
