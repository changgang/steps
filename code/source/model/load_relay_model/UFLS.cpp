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
    clear();
}

void UFLS::clear()
{
    frequency_sensor.set_limiter_type(NO_LIMITER);
    frequency_sensor.set_K(1.0);

    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
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
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
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

double UFLS::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double UFLS::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void UFLS::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void UFLS::set_double_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void UFLS::set_frequency_sensor_time_in_s(double t)
{
    frequency_sensor.set_T_in_s(t);
}

void UFLS::set_frequency_threshold_in_Hz_of_stage(size_t i, double f)
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return;
    frequency_threshold_in_Hz[i] = f;
}

void UFLS::set_time_delay_in_s_of_stage(size_t i, double t)
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return;
    stage_timer[i].set_timer_interval_in_s(t);
}

void UFLS::set_scale_in_pu_of_stage(size_t i, double s)
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return;
    scale_in_pu[i] = s;
}

void UFLS::set_breaker_time_in_s(double t)
{
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
        breaker_timer[i].set_timer_interval_in_s(t);
}

double UFLS::get_frequency_sensor_time_in_s() const
{
    return frequency_sensor.get_T_in_s();
}

double UFLS::get_frequency_threshold_in_Hz_of_stage(size_t i) const
{
    if(i<MAX_LOAD_RELAY_STAGE)
        return frequency_threshold_in_Hz[i];
    else
        return 0.0;
}

double UFLS::get_time_delay_in_s_of_stage(size_t i) const
{
    if(i<MAX_LOAD_RELAY_STAGE)
        return stage_timer[i].get_timer_interval_in_s();
    else
        return 0.0;
}

double UFLS::get_scale_in_pu_of_stage(size_t i) const
{
    if(i<MAX_LOAD_RELAY_STAGE)
        return scale_in_pu[i];
    else
        return 0.0;
}

double UFLS::get_breaker_time_in_s() const
{
    return breaker_timer[0].get_timer_interval_in_s();
}

bool UFLS::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool UFLS::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<20)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!="UFLSAL" and model_name!="UFLSAR" and model_name!="UFLSZN" and model_name!="UFLSBL")
        return is_successful;

    double t_sensor, tbreak, fth, tdelay, scale;

    size_t i=3;

    t_sensor = get_double_data(dyrdata[i],"0.0"); ++i;
    set_frequency_sensor_time_in_s(t_sensor);

    tbreak = get_double_data(dyrdata[i],"0.0"); ++i;
    set_breaker_time_in_s(tbreak);

    size_t stage = 0;

    size_t n = dyrdata.size()-2;
    for(i=5; i<n; i=i+3)
    {
        fth = get_double_data(dyrdata[i],"0.0");
        tdelay = get_double_data(dyrdata[i+1],"0.0");
        scale = get_double_data(dyrdata[i+2],"0.0");

        set_frequency_threshold_in_Hz_of_stage(stage, fth);
        set_time_delay_in_s_of_stage(stage, tdelay);
        set_scale_in_pu_of_stage(stage, scale);

        ++stage;
    }

    if(model_name=="UFLSAL")
        set_subsystem_type(ALL_SYSTEM_TYPE);
    else
    {
        if(model_name=="UFLSAR")
            set_subsystem_type(AREA_SUBSYSTEM_TYPE);
        else
        {
            if(model_name=="UFLSZN")
                set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
            else
            {
                //UFLSBL
                set_subsystem_type(BUS_SUBSYSTEM_TYPE);
            }
        }
    }
    is_successful = true;

    return is_successful;
}

bool UFLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void UFLS::initialize()
{
    LOAD* load = get_load_pointer();
    POWER_SYSTEM_DATABASE* psdb = load->get_power_system_database();
    double fbase = psdb->get_system_base_frequency_in_Hz();

    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].set_attached_device(load);
        breaker_timer[i].set_attached_device(load);
    }

    frequency_sensor.set_output(fbase);
    frequency_sensor.initialize();

    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].reset();
        breaker_timer[i].reset();
        flag_stage_is_tripped[i]=false;
    }
}

void UFLS::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    double current_time = get_dynamic_simulation_time_in_s();

    double freq = get_bus_frequency_in_Hz();

    frequency_sensor.set_input(freq);
    frequency_sensor.run(mode);

    if(mode==UPDATE_MODE)
    {
        double f = frequency_sensor.get_output();

        for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
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
                    show_information_with_leading_time_stamp(osstream);

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
                    show_information_with_leading_time_stamp(osstream);
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
                        show_information_with_leading_time_stamp(osstream);
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
                    show_information_with_leading_time_stamp(osstream);
                    start_stage_delayer_timer(i);
                }
            }
        }
    }
}


double UFLS::get_total_shed_scale_factor_in_pu() const
{
    double total_scale = 0.0;
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        if(is_stage_tripped(i))
            total_scale += get_scale_in_pu_of_stage(i);

    }
    return total_scale;
}

bool UFLS::is_stage_delayer_timer_started(size_t i) const
{
    return stage_timer[i].is_started();
}

bool UFLS::is_stage_breaker_timer_started(size_t i) const
{
    return breaker_timer[i].is_started();
}

void UFLS::start_stage_delayer_timer(size_t i)
{
    if(not stage_timer[i].is_started())
        stage_timer[i].start();
}

void UFLS::start_stage_breaker_timer(size_t i)
{
    if(not breaker_timer[i].is_started())
    {
        breaker_timer[i].start();
        reset_stage_delayer_timer(i);
    }
}

void UFLS::reset_stage_delayer_timer(size_t i)
{
    if(stage_timer[i].is_started())
        stage_timer[i].reset();
}

void UFLS::reset_stage_breaker_timer(size_t i)
{
    if(breaker_timer[i].is_started())
        breaker_timer[i].reset();
}

bool UFLS::is_stage_delayer_timer_timed_out(size_t i) const
{
    if(stage_timer[i].is_started())
        return stage_timer[i].is_timed_out();
    else
        return false;
}

bool UFLS::is_stage_breaker_timer_timed_out(size_t i) const
{
    if(breaker_timer[i].is_started())
        return breaker_timer[i].is_timed_out();
    else
        return false;
}

void UFLS::trip_stage(size_t i)
{
    ostringstream osstream;
    if(i<MAX_LOAD_RELAY_STAGE)
    {
        if(not is_stage_tripped(i))
        {
            flag_stage_is_tripped[i] = true;
            reset_stage_breaker_timer(i);
        }
    }
}

bool UFLS::is_stage_tripped(size_t i) const
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return false;

    return flag_stage_is_tripped[i];
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

string UFLS::get_standard_model_string() const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    size_t bus = load->get_load_bus();
    string identifier = load->get_identifier();
    osstream<<bus<<", "
      <<"'"<<get_detailed_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setprecision(4)<<fixed<<get_frequency_sensor_time_in_s()<<", "
      <<setprecision(4)<<fixed<<get_breaker_time_in_s();

    double fth, tdelay, scale;
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        fth = get_frequency_threshold_in_Hz_of_stage(i);
        tdelay = get_time_delay_in_s_of_stage(i);
        scale = get_scale_in_pu_of_stage(i);
        if(fabs(fth)<FLOAT_EPSILON)
            break;
        osstream<<", ";
        osstream<<setprecision(3)<<fixed<<fth<<", "
          <<setprecision(3)<<fixed<<tdelay<<", "
          <<setprecision(3)<<fixed<<scale;
    }
    osstream<<"  /";
    return osstream.str();
}

size_t UFLS::get_variable_index_from_variable_name(string var_name)
{
    if(var_name=="SCALE")
        return 0;

    return INDEX_NOT_EXIST;
}


string UFLS::get_variable_name_from_variable_index(size_t var_index)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_index is provided: "<<var_index;
    show_information_with_leading_time_stamp(osstream);
    return "";
}

double UFLS::get_variable_with_index(size_t var_index)
{
    switch(var_index)
    {
        case 0:
            return get_total_shed_scale_factor_in_pu();
        default:
            return 0.0;
    }
}
double UFLS::get_variable_with_name(string var_name)
{
    size_t index = get_variable_index_from_variable_name(var_name);
    return get_variable_with_index(index);
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
