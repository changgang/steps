#include "header/model/load_relay_model/UFLS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
using namespace std;

UFLS::UFLS()
{
    clear();
}

UFLS::~UFLS()
{
}

void UFLS::clear()
{
    set_model_float_parameter_count(52);

    frequency_sensor.set_limiter_type(NO_LIMITER);
    frequency_sensor.set_K(1.0);

    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        set_frequency_threshold_in_Hz_of_stage(i, 0.0);
        set_time_delay_in_s_of_stage(i, 0.0);
        set_scale_in_pu_of_stage(i, 0.0);
        stage_timer[i].set_timer_interval_in_s(INFINITE_THRESHOLD);
        stage_timer[i].reset();
        breaker_timer[i].set_timer_interval_in_s(INFINITE_THRESHOLD);
        breaker_timer[i].reset();
        flag_stage_is_tripped[i] = false;

        //set_delayer_timer_in_s_of_stage(i, 1e10);
        //set_breaker_timer_in_s_of_stage(i, 1e10);
    }
    set_breaker_time_in_s(0.0);
}
void UFLS::copy_from_const_model(const UFLS& model)
{
    clear();
    set_frequency_sensor_time_in_s(model.get_frequency_sensor_time_in_s());
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        this->set_frequency_threshold_in_Hz_of_stage(i, model.get_frequency_threshold_in_Hz_of_stage(i));
        this->set_time_delay_in_s_of_stage(i, model.get_time_delay_in_s_of_stage(i));
        this->set_scale_in_pu_of_stage(i, model.get_scale_in_pu_of_stage(i));
        //this->set_delayer_timer_in_s_of_stage(i, model.get_delayer_timer_in_s_of_stage(i));
        //this->set_breaker_timer_in_s_of_stage(i, model.get_breaker_timer_in_s_of_stage(i));
    }
    this->set_breaker_time_in_s(model.get_breaker_time_in_s());
}

UFLS::UFLS(const UFLS& model) : LOAD_FREQUENCY_RELAY_MODEL()
{
    copy_from_const_model(model);
}

UFLS& UFLS::operator=(const UFLS& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return *this;
}

string UFLS::get_model_name() const
{
    return "UFLS";
}

void UFLS::set_frequency_sensor_time_in_s(double t)
{
    frequency_sensor.set_T_in_s(t);
}

void UFLS::set_frequency_threshold_in_Hz_of_stage(unsigned int i, double f)
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        frequency_threshold_in_Hz[i] = f;
}

void UFLS::set_time_delay_in_s_of_stage(unsigned int i, double t)
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        stage_timer[i].set_timer_interval_in_s(t);
}

void UFLS::set_scale_in_pu_of_stage(unsigned int i, double s)
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        scale_in_pu[i] = s;
}

void UFLS::set_breaker_time_in_s(double t)
{
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        breaker_timer[i].set_timer_interval_in_s(t);
}

double UFLS::get_frequency_sensor_time_in_s() const
{
    return frequency_sensor.get_T_in_s();
}

double UFLS::get_frequency_threshold_in_Hz_of_stage(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return frequency_threshold_in_Hz[i];
    else
        return 0.0;
}

double UFLS::get_time_delay_in_s_of_stage(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return stage_timer[i].get_timer_interval_in_s();
    else
        return 0.0;
}

double UFLS::get_scale_in_pu_of_stage(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return scale_in_pu[i];
    else
        return 0.0;
}

double UFLS::get_breaker_time_in_s() const
{
    return breaker_timer[0].get_timer_interval_in_s();
}

bool UFLS::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=20)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name=="UFLSAL" or model_name=="UFLSBL" or model_name=="UFLSAR" or model_name=="UFLSZN")
        {
            double t_sensor, tbreak, fth, tdelay, scale;

            unsigned int i=3;

            t_sensor = get_double_data(data[i],"0.0"); ++i;
            set_frequency_sensor_time_in_s(t_sensor);

            tbreak = get_double_data(data[i],"0.0"); ++i;
            set_breaker_time_in_s(tbreak);

            unsigned int stage = 0;

            unsigned int n = data.size()-2;
            for(i=5; i<n; i=i+3)
            {
                fth = get_double_data(data[i],"0.0");
                tdelay = get_double_data(data[i+1],"0.0");
                scale = get_double_data(data[i+2],"0.0");

                set_frequency_threshold_in_Hz_of_stage(stage, fth);
                set_time_delay_in_s_of_stage(stage, tdelay);
                set_scale_in_pu_of_stage(stage, scale);

                ++stage;
            }

            if(model_name=="UFLSAL")
                set_subsystem_type(ALL_SYSTEM_TYPE);
            else
            {
                if(model_name=="UFLSBL")
                    set_subsystem_type(BUS_SUBSYSTEM_TYPE);
                else
                {
                    if(model_name=="UFLSAR")
                        set_subsystem_type(AREA_SUBSYSTEM_TYPE);
                    else
                    {
                        //UFLSZN
                        set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
                    }
                }
            }
            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool UFLS::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool UFLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void UFLS::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    frequency_sensor.set_toolkit(toolkit);
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].set_toolkit(toolkit);
        breaker_timer[i].set_toolkit(toolkit);
    }
}

void UFLS::initialize()
{
    LOAD* load = get_load_pointer();
    if(load!=NULL)
    {
        setup_block_toolkit_and_parameters();

        double fbase = get_bus_base_frequency_in_Hz();

        for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        {
            stage_timer[i].set_attached_device(load);
            breaker_timer[i].set_attached_device(load);
        }

        frequency_sensor.set_output(fbase);
        frequency_sensor.initialize();

        for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        {
            stage_timer[i].reset();
            breaker_timer[i].reset();
            flag_stage_is_tripped[i]=false;
        }
    }
}

void UFLS::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double current_time = toolkit.get_dynamic_simulation_time_in_s();

    double freq = get_bus_frequency_in_Hz();

    frequency_sensor.set_input(freq);
    frequency_sensor.run(mode);

    if(mode==RELAY_MODE)
    {
        double f = frequency_sensor.get_output();

        for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        {
            if(is_stage_tripped(i)) // already tripped
                continue;

            // haven't been tripped
            if(is_stage_breaker_timer_started(i))
            {
                if(is_stage_breaker_timer_timed_out(i)) // breaker timed out
                {
                    osstream<<"UFLS stage "<<i<<" timer of "<<get_device_name()<<" is timed out at time "<<current_time<<" s."<<endl
                      <<get_scale_in_pu_of_stage(i)*100.0<<"% loads are tripped.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    trip_stage(i); // trip it
                }
                continue;
            }
            // breaker not started
            if(is_stage_delayer_timer_started(i)) // delayer started
            {
                if(is_stage_delayer_timer_timed_out(i)) // delayer timed out
                {
                    osstream<<"UFLS stage "<<i<<" timer of "<<get_device_name()<<" is sending tripping signal to breaker at time "<<current_time<<" s since stage delayer timer is timed out."<<endl
                      <<"Current frequency is "<<f<<" Hz, and stage frequency threshold is "<<get_frequency_threshold_in_Hz_of_stage(i)<<" Hz.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    start_stage_breaker_timer(i); // start breaker;
                }
                else// delayer not timed out
                {
                    if(f<=get_frequency_threshold_in_Hz_of_stage(i))
                        continue; //do nothing
                    else
                    {
                        // need to reset
                        osstream<<"UFLS stage "<<i<<" timer of "<<get_device_name()<<" is reset at time "<<current_time<<" s due to recovery of frequency."<<endl
                          <<"Current frequency is "<<f<<" Hz, and stage frequency threshold is "<<get_frequency_threshold_in_Hz_of_stage(i)<<" Hz.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        reset_stage_delayer_timer(i);
                    }
                }
                continue;
            }
            else// delayer not started. check if need to start
            {
                if(f<get_frequency_threshold_in_Hz_of_stage(i))
                {
                    osstream<<"UFLS stage "<<i<<" timer of "<<get_device_name()<<" is started at time "<<current_time<<" s due to drop of frequency."<<endl
                      <<"Current frequency is "<<f<<" Hz, and stage frequency threshold is "<<get_frequency_threshold_in_Hz_of_stage(i)<<" Hz.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    start_stage_delayer_timer(i);
                }
            }
        }
    }
}


double UFLS::get_total_shed_scale_factor_in_pu() const
{
    double total_scale = 0.0;
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        total_scale += (is_stage_tripped(i)*get_scale_in_pu_of_stage(i));
    return total_scale;
}

bool UFLS::is_stage_delayer_timer_started(unsigned int i) const
{
    return stage_timer[i].is_started();
}

bool UFLS::is_stage_breaker_timer_started(unsigned int i) const
{
    return breaker_timer[i].is_started();
}

void UFLS::start_stage_delayer_timer(unsigned int i)
{
    if(not stage_timer[i].is_started())
        stage_timer[i].start();
}

void UFLS::start_stage_breaker_timer(unsigned int i)
{
    if(not breaker_timer[i].is_started())
    {
        breaker_timer[i].start();
        reset_stage_delayer_timer(i);
    }
}

void UFLS::reset_stage_delayer_timer(unsigned int i)
{
    if(stage_timer[i].is_started())
        stage_timer[i].reset();
}

void UFLS::reset_stage_breaker_timer(unsigned int i)
{
    if(breaker_timer[i].is_started())
        breaker_timer[i].reset();
}

bool UFLS::is_stage_delayer_timer_timed_out(unsigned int i) const
{
    if(stage_timer[i].is_started())
        return stage_timer[i].is_timed_out();
    else
        return false;
}

bool UFLS::is_stage_breaker_timer_timed_out(unsigned int i) const
{
    if(breaker_timer[i].is_started())
        return breaker_timer[i].is_timed_out();
    else
        return false;
}

void UFLS::trip_stage(unsigned int i)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
    {
        if(not is_stage_tripped(i))
        {
            flag_stage_is_tripped[i] = true;
            reset_stage_breaker_timer(i);
            DYNAMICS_SIMULATOR& sim = toolkit.get_dynamic_simulator();
            sim.enable_relay_action_flag();
        }
    }
}

bool UFLS::is_stage_tripped(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return flag_stage_is_tripped[i];
    else
        return false;
}

void UFLS::check()
{
    ;
}

void UFLS::report()
{
    ;
}

void UFLS::save()
{
    ;
}

string UFLS::get_standard_psse_string() const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    unsigned int bus = load->get_load_bus();
    string identifier = "'"+load->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(4)<<fixed<<get_frequency_sensor_time_in_s()<<", "
            <<setw(8)<<setprecision(4)<<fixed<<get_breaker_time_in_s()<<", ";

    unsigned int n_content = 5;
    double fth, tdelay, scale;
    unsigned int i=0;
    for(i=0; i!=(STEPS_MAX_LOAD_RELAY_STAGE-1); ++i)
    {
        fth = get_frequency_threshold_in_Hz_of_stage(i);
        tdelay = get_time_delay_in_s_of_stage(i);
        scale = get_scale_in_pu_of_stage(i);
        if(fabs(fth)<FLOAT_EPSILON)
            break;
        osstream<<setw(8)<<setprecision(4)<<fixed<<fth<<", ";
        n_content++;
        if(n_content==10)
        {
            osstream<<"\n"
                    <<setw(10)<<"";
            n_content = 1;
        }
        osstream<<setw(8)<<setprecision(4)<<fixed<<tdelay<<", ";
        n_content++;
        if(n_content==10)
        {
            osstream<<"\n"
                    <<setw(10)<<"";
            n_content = 1;
        }
        osstream<<setw(8)<<setprecision(4)<<fixed<<scale<<", ";
        n_content++;
        if(n_content==10)
        {
            osstream<<"\n"
                    <<setw(10)<<"";
            n_content = 1;
        }
    }
    i = STEPS_MAX_LOAD_RELAY_STAGE - 1;
    fth = get_frequency_threshold_in_Hz_of_stage(i);
    tdelay = get_time_delay_in_s_of_stage(i);
    scale = get_scale_in_pu_of_stage(i);
    if(fabs(fth)>=FLOAT_EPSILON)
    {
        osstream<<setw(8)<<setprecision(4)<<fixed<<fth<<", ";
        n_content++;
        if(n_content==10)
        {
            osstream<<"\n"
                    <<setw(10)<<"";
            n_content = 1;
        }
        osstream<<setw(8)<<setprecision(4)<<fixed<<tdelay<<", ";
        n_content++;
        if(n_content==10)
        {
            osstream<<"\n"
                    <<setw(10)<<"";
            n_content = 1;
        }
        osstream<<setw(8)<<setprecision(4)<<fixed<<scale<<", ";
    }
    string model_string = osstream.str();
    osstream.str("");

    model_string = trim_string(model_string, ", ")+" /";

    return model_string;
}

void UFLS::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TF", i); i++;
    add_model_data_name_and_index_pair("TB", i); i++;
    for(unsigned int stage=0; stage<STEPS_MAX_LOAD_RELAY_STAGE; ++stage)
    {
        string name = "FTH "+num2str(stage);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "TD "+num2str(stage);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "P "+num2str(stage);
        add_model_data_name_and_index_pair(name, i); i++;
    }
}

double UFLS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="TF")
        return get_frequency_sensor_time_in_s();
    if(par_name=="TB")
        return get_breaker_time_in_s();
    for(unsigned int stage=0; stage<STEPS_MAX_LOAD_RELAY_STAGE; ++stage)
    {
        string name = "FTH "+num2str(stage);
        if(par_name==name)
            return get_frequency_threshold_in_Hz_of_stage(stage);
        name = "TD "+num2str(stage);
        if(par_name==name)
            return get_time_delay_in_s_of_stage(stage);
        name = "P "+num2str(stage);
        if(par_name==name)
            return get_scale_in_pu_of_stage(stage);
    }

    return 0.0;
}

void UFLS::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="TF")
        return set_frequency_sensor_time_in_s(value);
    if(par_name=="TB")
        return set_breaker_time_in_s(value);
    for(unsigned int stage=0; stage<STEPS_MAX_LOAD_RELAY_STAGE; ++stage)
    {
        string name = "FTH "+num2str(stage);
        if(par_name==name)
            return set_frequency_threshold_in_Hz_of_stage(stage, value);
        name = "TD "+num2str(stage);
        if(par_name==name)
            return set_time_delay_in_s_of_stage(stage, value);
        name = "P "+num2str(stage);
        if(par_name==name)
            return set_scale_in_pu_of_stage(stage, value);
    }

    return;
}

double UFLS::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void UFLS::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_inernal_variable_name_and_index_pair("SHED SCALE IN PU", i); i++;
}

double UFLS::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="SHED SCALE IN PU")
        return get_total_shed_scale_factor_in_pu();
    return 0.0;
}

string UFLS::get_dynamic_data_in_psse_format() const
{
    return "";
}

string UFLS::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string UFLS::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
