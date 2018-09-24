#include "header/model/load_relay_model/UVLS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
using namespace std;

UVLS::UVLS()
{
    clear();
}

UVLS::~UVLS()
{
    clear();
}

void UVLS::clear()
{
    prepare_model_variable_table();

    voltage_sensor.set_limiter_type(NO_LIMITER);
    voltage_sensor.set_K(1.0);

    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        set_voltage_threshold_in_pu_of_stage(i, 0.0);
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
void UVLS::copy_from_const_model(const UVLS& model)
{
    clear();
    set_voltage_sensor_time_in_s(model.get_voltage_sensor_time_in_s());
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        this->set_voltage_threshold_in_pu_of_stage(i, model.get_voltage_threshold_in_pu_of_stage(i));
        this->set_time_delay_in_s_of_stage(i, model.get_time_delay_in_s_of_stage(i));
        this->set_scale_in_pu_of_stage(i, model.get_scale_in_pu_of_stage(i));
        //this->set_delayer_timer_in_s_of_stage(i, model.get_delayer_timer_in_s_of_stage(i));
        //this->set_breaker_timer_in_s_of_stage(i, model.get_breaker_timer_in_s_of_stage(i));
    }
    this->set_breaker_time_in_s(model.get_breaker_time_in_s());
}

UVLS::UVLS(const UVLS& model) : LOAD_VOLTAGE_RELAY_MODEL()
{
    copy_from_const_model(model);
}

UVLS& UVLS::operator=(const UVLS& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return *this;
}

string UVLS::get_model_name() const
{
    return "UVLS";
}

double UVLS::get_model_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double UVLS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void UVLS::set_model_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void UVLS::set_model_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void UVLS::set_voltage_sensor_time_in_s(double t)
{
    voltage_sensor.set_T_in_s(t);
}

void UVLS::set_voltage_threshold_in_pu_of_stage(size_t i, double f)
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return;
    voltage_threshold_in_pu[i] = f;
}

void UVLS::set_time_delay_in_s_of_stage(size_t i, double t)
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return;
    stage_timer[i].set_timer_interval_in_s(t);
}

void UVLS::set_scale_in_pu_of_stage(size_t i, double s)
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return;
    scale_in_pu[i] = s;
}

void UVLS::set_breaker_time_in_s(double t)
{
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
        breaker_timer[i].set_timer_interval_in_s(t);
}

double UVLS::get_voltage_sensor_time_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double UVLS::get_voltage_threshold_in_pu_of_stage(size_t i) const
{
    if(i<MAX_LOAD_RELAY_STAGE)
        return voltage_threshold_in_pu[i];
    else
        return 0.0;
}

double UVLS::get_time_delay_in_s_of_stage(size_t i) const
{
    if(i<MAX_LOAD_RELAY_STAGE)
        return stage_timer[i].get_timer_interval_in_s();
    else
        return 0.0;
}

double UVLS::get_scale_in_pu_of_stage(size_t i) const
{
    if(i<MAX_LOAD_RELAY_STAGE)
        return scale_in_pu[i];
    else
        return 0.0;
}

double UVLS::get_breaker_time_in_s() const
{
    return breaker_timer[0].get_timer_interval_in_s();
}

bool UVLS::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool UVLS::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    if(dyrdata.size()<20)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!="UVLSAL" and model_name!="UVLSAR" and model_name!="UVLSZN" and model_name!="UVLSBL")
        return is_successful;

    double t_sensor, tbreak, vth, tdelay, scale;

    size_t i=3;

    t_sensor = get_double_data(dyrdata[i],"0.0"); ++i;
    set_voltage_sensor_time_in_s(t_sensor);

    tbreak = get_double_data(dyrdata[i],"0.0"); ++i;
    set_breaker_time_in_s(tbreak);

    size_t stage = 0;

    size_t n = dyrdata.size()-2;
    for(i=5; i<n; i=i+3)
    {
        vth = get_double_data(dyrdata[i],"0.0");
        tdelay = get_double_data(dyrdata[i+1],"0.0");
        scale = get_double_data(dyrdata[i+2],"0.0");

        set_voltage_threshold_in_pu_of_stage(stage, vth);
        set_time_delay_in_s_of_stage(stage, tdelay);
        set_scale_in_pu_of_stage(stage, scale);

        ++stage;
    }

    if(model_name=="UVLSAL")
        set_subsystem_type(ALL_SYSTEM_TYPE);
    else
    {
        if(model_name=="UVLSAR")
            set_subsystem_type(AREA_SUBSYSTEM_TYPE);
        else
        {
            if(model_name=="UVLSZN")
                set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
            else
            {
                //UVLSBL
                set_subsystem_type(BUS_SUBSYSTEM_TYPE);
            }
        }
    }
    is_successful = true;

    return is_successful;
}

bool UVLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void UVLS::initialize()
{
    LOAD* load = get_load_pointer();
    POWER_SYSTEM_DATABASE* psdb = load->get_power_system_database();
    double volt = psdb->get_bus_voltage_in_pu(load->get_load_bus());

    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].set_attached_device(load);
        breaker_timer[i].set_attached_device(load);
    }

    voltage_sensor.set_output(volt);
    voltage_sensor.initialize();

    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].reset();
        breaker_timer[i].reset();
        flag_stage_is_tripped[i]=false;
    }
}

void UVLS::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    double current_time = get_dynamic_simulation_time_in_s();

    double volt = get_bus_voltage_in_pu();

    voltage_sensor.set_input(volt);
    voltage_sensor.run(mode);

    if(mode==UPDATE_MODE)
    {
        double v = voltage_sensor.get_output();

        for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
        {
            if(is_stage_tripped(i)) // already tripped
                continue;

            // haven't been tripped
            if(is_stage_breaker_timer_started(i))
            {
                if(is_stage_breaker_timer_timed_out(i)) // breaker timed out
                {
                    osstream<<"UVLS stage "<<i<<" timer of "<<get_device_name()<<" is timed out at time "<<current_time<<" s."<<endl
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
                    osstream<<"UVLS stage "<<i<<" timer of "<<get_device_name()<<" is sending tripping signal to breaker at time "<<current_time<<" s since stage delayer timer is timed out."<<endl
                      <<"Current voltage is "<<v<<" pu, and stage voltage threshold is "<<get_voltage_threshold_in_pu_of_stage(i)<<" pu.";
                    show_information_with_leading_time_stamp(osstream);
                    start_stage_breaker_timer(i); // start breaker;
                }
                else// delayer not timed out
                {
                    if(v<=get_voltage_threshold_in_pu_of_stage(i))
                        continue; //do nothing
                    else
                    {
                        // need to reset
                        osstream<<"UVLS stage "<<i<<" timer of "<<get_device_name()<<" is reset at time "<<current_time<<" s due to recovery of voltage."<<endl
                          <<"Current voltage is "<<v<<" pu, and stage voltage threshold is "<<get_voltage_threshold_in_pu_of_stage(i)<<" pu.";
                        show_information_with_leading_time_stamp(osstream);
                        reset_stage_delayer_timer(i);
                    }
                }
                continue;
            }
            else// delayer not started. check if need to start
            {
                if(v<get_voltage_threshold_in_pu_of_stage(i))
                {
                    osstream<<"UVLS stage "<<i<<" timer of "<<get_device_name()<<" is started at time "<<current_time<<" s due to drop of voltage."<<endl
                      <<"Current voltage is "<<v<<" pu, and stage voltage threshold is "<<get_voltage_threshold_in_pu_of_stage(i)<<" pu.";
                    show_information_with_leading_time_stamp(osstream);
                    start_stage_delayer_timer(i);
                }
            }
        }
    }
}


double UVLS::get_total_shed_scale_factor_in_pu() const
{
    double total_scale = 0.0;
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        if(is_stage_tripped(i))
            total_scale += get_scale_in_pu_of_stage(i);

    }
    return total_scale;
}

bool UVLS::is_stage_delayer_timer_started(size_t i) const
{
    return stage_timer[i].is_started();
}

bool UVLS::is_stage_breaker_timer_started(size_t i) const
{
    return breaker_timer[i].is_started();
}

void UVLS::start_stage_delayer_timer(size_t i)
{
    if(not stage_timer[i].is_started())
        stage_timer[i].start();
}

void UVLS::start_stage_breaker_timer(size_t i)
{
    if(not breaker_timer[i].is_started())
    {
        breaker_timer[i].start();
        reset_stage_delayer_timer(i);
    }
}

void UVLS::reset_stage_delayer_timer(size_t i)
{
    if(stage_timer[i].is_started())
        stage_timer[i].reset();
}

void UVLS::reset_stage_breaker_timer(size_t i)
{
    if(breaker_timer[i].is_started())
        breaker_timer[i].reset();
}

bool UVLS::is_stage_delayer_timer_timed_out(size_t i) const
{
    if(stage_timer[i].is_started())
        return stage_timer[i].is_timed_out();
    else
        return false;
}

bool UVLS::is_stage_breaker_timer_timed_out(size_t i) const
{
    if(breaker_timer[i].is_started())
        return breaker_timer[i].is_timed_out();
    else
        return false;
}

void UVLS::trip_stage(size_t i)
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

bool UVLS::is_stage_tripped(size_t i) const
{
    if(i>=MAX_LOAD_RELAY_STAGE)
        return false;

    return flag_stage_is_tripped[i];
}

void UVLS::check()
{
    ;
}

void UVLS::report()
{
    ;
}

void UVLS::save()
{
    ;
}

string UVLS::get_standard_model_string() const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    size_t bus = load->get_load_bus();
    string identifier = load->get_identifier();
    osstream<<bus<<", "
      <<"'"<<get_detailed_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setprecision(4)<<fixed<<get_voltage_sensor_time_in_s()<<", "
      <<setprecision(4)<<fixed<<get_breaker_time_in_s();

    double vth, tdelay, scale;
    for(size_t i=0; i!=MAX_LOAD_RELAY_STAGE; ++i)
    {
        vth = get_voltage_threshold_in_pu_of_stage(i);
        tdelay = get_time_delay_in_s_of_stage(i);
        scale = get_scale_in_pu_of_stage(i);
        if(fabs(vth)<FLOAT_EPSILON)
            break;
        osstream<<", ";
        osstream<<setprecision(3)<<fixed<<vth<<", "
          <<setprecision(3)<<fixed<<tdelay<<", "
          <<setprecision(3)<<fixed<<scale;
    }
    osstream<<"  /";
    return osstream.str();
}

void UVLS::prepare_model_variable_table()
{
    size_t i=0;
    add_model_variable_name_and_index_pair("SHED SCALE IN PU", i); i++;
}

double UVLS::get_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name=="SHED SCALE IN PU")
        return get_total_shed_scale_factor_in_pu();
    return 0.0;
}

string UVLS::get_dynamic_data_in_psse_format() const
{
    return "";
}

string UVLS::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string UVLS::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
