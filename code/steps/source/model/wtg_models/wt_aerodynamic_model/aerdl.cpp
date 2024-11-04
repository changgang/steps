#include "header/model/wtg_models/wt_aerodynamic_model/aerdl.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

AERDL::AERDL(STEPS& toolkit) : WT_AERODYNAMIC_MODEL(toolkit)
{
    clear();
}

AERDL::AERDL(const AERDL& model):WT_AERODYNAMIC_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

AERDL::~AERDL()
{
    ;
}

AERDL& AERDL::operator=(const AERDL& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void AERDL::clear()
{
    set_model_float_parameter_count(7);
}

void AERDL::copy_from_const_model(const AERDL& model)
{
    set_toolkit(model.get_toolkit());

    clear();

    set_Ka(model.get_Ka());
    set_initial_pitch_angle_in_deg(model.get_initial_pitch_angle_in_deg());
    set_a(model.get_a());
    set_b(model.get_b());
    set_c(model.get_c());
    set_max_steady_state_turbine_speed_in_pu(model.get_max_steady_state_turbine_speed_in_pu());
    set_min_steady_state_turbine_speed_in_pu(model.get_min_steady_state_turbine_speed_in_pu());


}

string AERDL::get_model_name() const
{
    return "AERDL";
}

void AERDL::set_Ka(double value)
{
    if(value<0)
        value = -value;
    this->Ka = value;
}

void AERDL::set_a(double value)
{
    if(value>0)
        value = -value;
    this->a = value;
}

void AERDL::set_b(double value)
{
    if(value<0)
        value = -value;
    this->b = value;
}

void AERDL::set_c(double value)
{
    if(value<0)
        value = -value;
    this->c = value;
}

void AERDL::set_initial_turbine_mechanical_power_in_pu(double value)
{
    this->initial_turbine_mechanical_power_in_pu = value;
}

double AERDL::get_Ka() const
{
    return this->Ka;
}

double AERDL::get_a() const
{
    return this->a;
}

double AERDL::get_b() const
{
    return this->b;
}

double AERDL::get_c() const
{
    return this->c;
}

double AERDL::get_initial_turbine_mechanical_power_in_pu() const
{
    return this->initial_turbine_mechanical_power_in_pu;
}

void AERDL::initialize_turbine_mechanical_power_in_pu()
{
    double Pmech0 = get_active_power_generation_including_stator_loss_and_braking_resisitor_power_in_MW();
    double mbase = get_mbase_in_MVA();
    Pmech0 /= mbase;
    set_initial_turbine_mechanical_power_in_pu(Pmech0);
}

void AERDL::initialize_turbine_speed_in_pu()
{
    double speed = get_turbine_reference_speed_in_pu();
    set_initial_turbine_speed_in_pu(speed);
}

double AERDL::get_maximum_available_mechanical_power_per_wt_generator_in_MW_considering_gear_efficiency(double vwind)//called by meter
{
    double power = 0.25*get_Ka()*get_initial_pitch_angle_in_deg()*get_initial_pitch_angle_in_deg()-get_initial_turbine_mechanical_power_in_pu();
    return power*get_mbase_in_MVA()/get_number_of_lumped_wt_generators();
}

double AERDL::get_turbine_mechanical_power_in_MW()//called by turbine model
{
    double mbase = get_mbase_in_MVA();
    if(fabs(get_pitch_angle_in_deg()-get_initial_pitch_angle_in_deg())<DOUBLE_EPSILON)
        return get_initial_turbine_mechanical_power_in_pu()*mbase;

    double Pmech = get_initial_turbine_mechanical_power_in_pu()-get_Ka()*get_pitch_angle_in_deg()*(get_pitch_angle_in_deg()-get_initial_pitch_angle_in_deg());

    return Pmech*mbase;
}

double AERDL::get_turbine_reference_speed_in_pu_without_speed_limit(double pelec)
{
    double speed_ref = get_a()*pelec*pelec+get_b()*pelec+get_c();

    return speed_ref;
}

double AERDL::get_turbine_reference_speed_in_pu()//called by wt electrical model and pitch model
{
    double pelec = get_active_power_generation_including_stator_loss_and_braking_resisitor_power_in_MW();
    pelec /= get_mbase_in_MVA();
    double speed = get_turbine_reference_speed_in_pu_without_speed_limit(pelec);
    double maxspeed = get_max_steady_state_turbine_speed_in_pu();
    double minspeed = get_min_steady_state_turbine_speed_in_pu();
    if(speed>=maxspeed)
        speed = maxspeed;

    if(speed<=minspeed)
        speed = minspeed;
    return speed;
}

double AERDL::get_pitch_angle_in_deg() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_PITCH_MODEL* model = gen->get_wt_pitch_model();
    double pitch_angle = 0.0;
    if(model!=NULL and model->is_model_initialized())
        pitch_angle = model->get_pitch_angle_in_deg();
    else
        pitch_angle = get_initial_pitch_angle_in_deg();

    double min_pitch_angle = 0.5*get_initial_pitch_angle_in_deg();

    if(pitch_angle<=min_pitch_angle)
        pitch_angle = min_pitch_angle;

    return pitch_angle;
}

void AERDL::set_initial_turbine_speed_in_pu(double value)
{
    this->initial_turbine_speed_in_pu = value;
}

double AERDL::get_initial_turbine_speed_in_pu()//called by turbine model and electrical model
{
    return this->initial_turbine_speed_in_pu;
}

void AERDL::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        initialize_turbine_mechanical_power_in_pu();
        initialize_turbine_speed_in_pu();
        set_flag_model_initialized_as_true();
        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                <<"(1) Initial turbine mechanical power in pu = "<<get_initial_turbine_mechanical_power_in_pu()<<"."<<endl
                <<"(2) Initial pitch angle in deg = "<<get_initial_pitch_angle_in_deg()<<"."<<endl
                <<"(3) Initial turbine speed in pu = "<<get_initial_turbine_speed_in_pu()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

bool AERDL::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;
    if(data.size()>=10)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {

            unsigned int ibus;
            string id;

            double ka, pitch0, co_a, co_b, co_c, minspeed, maxspeed;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            ka = get_double_data(data[i],"0.0"); i++;
            pitch0 = get_double_data(data[i],"0.0"); i++;
            co_a = get_double_data(data[i],"0.0"); i++;
            co_b = get_double_data(data[i],"0.0"); i++;
            co_c = get_double_data(data[i],"0.0"); i++;
            minspeed = get_double_data(data[i],"0.0"); i++;
            maxspeed = get_double_data(data[i],"0.0"); i++;

            DEVICE_ID did = get_wt_generator_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();


            set_Ka(ka);


            if(pitch0<0)
            {
                osstream<<"Warning. The parameter initial pitch angle in deg ( = "<<pitch0<<" deg ) of "<<did.get_compound_device_name()
                <<" is negative of "<<get_model_name()<<" model."<<endl
                <<"The parameter initial pitch angle in deg will be updated as "<<-pitch0<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
                pitch0 = -pitch0;
            }
            set_initial_pitch_angle_in_deg(pitch0);

            if(co_a>0)
            {
                osstream<<"Warning. The parameter coefficient a ( = "<<co_a<<" ) of "<<did.get_compound_device_name()
                <<" is positive of "<<get_model_name()<<" model."<<endl
                <<"The parameter coefficient a will be updated as "<<-co_a<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
                co_a = -co_a;
            }
            set_a(co_a);

            if(co_b<0)
            {
                osstream<<"Warning. The parameter coefficient b ( = "<<co_b<<" ) of "<<did.get_compound_device_name()
                <<" is negative of "<<get_model_name()<<" model."<<endl
                <<"The parameter coefficient b will be updated as "<<-co_b<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
                co_b = -co_b;
            }
            set_b(co_b);

            if(co_c<0)
            {
                osstream<<"Warning. The parameter coefficient c ( = "<<co_c<<" ) of "<<did.get_compound_device_name()
                <<" is negative of "<<get_model_name()<<" model."<<endl
                <<"The parameter coefficient c will be updated as "<<-co_c<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
                co_c = -co_c;
            }
            set_c(co_c);

            if(maxspeed<0.0)
            {
                osstream<<"Error. The parameter max steady state turbine speed in pu ( = "<<maxspeed<<" ) of "<<did.get_compound_device_name()<<" is lower than Zero.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(minspeed<0.0)
            {
                osstream<<"Error. The parameter min steady state turbine speed in pu ( = "<<minspeed<<" ) of "<<did.get_compound_device_name()<<" is lower than Zero.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            set_min_steady_state_turbine_speed_in_pu(minspeed);
            set_max_steady_state_turbine_speed_in_pu(maxspeed);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool AERDL::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool AERDL::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}


void AERDL::check()
{
    ostringstream osstream;
}

void AERDL::report()
{
    ;
}

void AERDL::save()
{
    ;
}

string AERDL::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    WT_GENERATOR* gen = get_wt_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(4)<<get_Ka()<<", "
            <<setw(8)<<setprecision(4)<<get_initial_pitch_angle_in_deg()<<", "
            <<setw(8)<<setprecision(4)<<get_a()<<", "
            <<setw(8)<<setprecision(4)<<get_b()<<", "
            <<setw(8)<<setprecision(4)<<get_c()<<", "
            <<setw(8)<<setprecision(4)<<get_min_steady_state_turbine_speed_in_pu()<<", "
            <<setw(8)<<setprecision(4)<<get_max_steady_state_turbine_speed_in_pu()<<" /";

    return osstream.str();
}

void AERDL::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("KA", i); i++;
    add_model_data_name_and_index_pair("INITIAL PITCH ANGLE IN DEG", i); i++;
    add_model_data_name_and_index_pair("COEFFICIENT A", i); i++;
    add_model_data_name_and_index_pair("COEFFICIENT B", i); i++;
    add_model_data_name_and_index_pair("COEFFICIENT C", i); i++;
    add_model_data_name_and_index_pair("MIN TURBINE SPEED IN PU", i); i++;
    add_model_data_name_and_index_pair("MAX TURBINE SPEED IN PU", i); i++;
}

double AERDL::get_model_data_with_name(string par_name) const
{

    par_name = string2upper(par_name);

    if(par_name=="KA")
        return get_Ka();

    if(par_name=="INITIAL PITCH ANGLE IN DEG")
        return get_initial_pitch_angle_in_deg();

    if(par_name=="COEFFICIENT A")
        return get_a();

    if(par_name=="COEFFICIENT B")
        return get_b();

    if(par_name=="COEFFICIENT C")
        return get_c();

    if(par_name=="MIN TURBINE SPEED IN PU")
        return get_min_steady_state_turbine_speed_in_pu();

    if(par_name=="MAX TURBINE SPEED IN PU")
        return get_max_steady_state_turbine_speed_in_pu();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);

    return 0.0;
}

void AERDL::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KA")
        return set_Ka(value);

    if(par_name=="INITIAL PITCH ANGLE IN DEG")
        return set_initial_pitch_angle_in_deg(value);

    if(par_name=="COEFFICIENT A")
        return set_a(value);

    if(par_name=="COEFFICIENT B")
        return set_b(value);

    if(par_name=="COEFFICIENT C")
        return set_c(value);

    if(par_name=="MIN TURBINE SPEED IN PU")
        return set_min_steady_state_turbine_speed_in_pu(value);

    if(par_name=="MAX TURBINE SPEED IN PU")
        return set_max_steady_state_turbine_speed_in_pu(value);

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double AERDL::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void AERDL::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("INITIAL TURBINE SPEED IN PU", i); i++;
}

double AERDL::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="INITIAL TURBINE SPEED IN PU")
        return get_initial_turbine_speed_in_pu();
    return 0.0;
}

string AERDL::get_dynamic_data_in_psse_format() const
{
    return "";
}

string AERDL::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string AERDL::get_dynamic_data_in_steps_format() const
{
    return "";
}

double AERDL::get_Cp(double lambda, double pitch_deg) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}

double AERDL::get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    osstream<<"WARNING. "<<get_model_name()<<"::"<<__FUNCTION__<<"() is called without implementation. ZERO is returned with no meaning.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return 0;
}
