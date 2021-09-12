#include "header/model/load_relay_model/UVLS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
using namespace std;

UVLS::UVLS(STEPS& toolkit) : LOAD_VOLTAGE_RELAY_MODEL(toolkit),
                             voltage_sensor(toolkit)
{
    clear();
}

UVLS::~UVLS()
{
}

void UVLS::clear()
{
    set_model_float_parameter_count(52);

    voltage_sensor.set_limiter_type(NO_LIMITER);
    voltage_sensor.set_K(1.0);

    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
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
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    voltage_sensor.set_toolkit(toolkit);

    clear();
    set_voltage_sensor_time_in_s(model.get_voltage_sensor_time_in_s());
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        this->set_voltage_threshold_in_pu_of_stage(i, model.get_voltage_threshold_in_pu_of_stage(i));
        this->set_time_delay_in_s_of_stage(i, model.get_time_delay_in_s_of_stage(i));
        this->set_scale_in_pu_of_stage(i, model.get_scale_in_pu_of_stage(i));
        //this->set_delayer_timer_in_s_of_stage(i, model.get_delayer_timer_in_s_of_stage(i));
        //this->set_breaker_timer_in_s_of_stage(i, model.get_breaker_timer_in_s_of_stage(i));
    }
    this->set_breaker_time_in_s(model.get_breaker_time_in_s());
}

UVLS::UVLS(const UVLS& model) : LOAD_VOLTAGE_RELAY_MODEL(model.get_toolkit()),
                                voltage_sensor(model.get_toolkit())
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

void UVLS::set_voltage_sensor_time_in_s(double t)
{
    voltage_sensor.set_T_in_s(t);
}

void UVLS::set_voltage_threshold_in_pu_of_stage(unsigned int i, double f)
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        voltage_threshold_in_pu[i] = f;
}

void UVLS::set_time_delay_in_s_of_stage(unsigned int i, double t)
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        stage_timer[i].set_timer_interval_in_s(t);
}

void UVLS::set_scale_in_pu_of_stage(unsigned int i, double s)
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        scale_in_pu[i] = s;
}

void UVLS::set_breaker_time_in_s(double t)
{
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        breaker_timer[i].set_timer_interval_in_s(t);
}

double UVLS::get_voltage_sensor_time_in_s() const
{
    return voltage_sensor.get_T_in_s();
}

double UVLS::get_voltage_threshold_in_pu_of_stage(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return voltage_threshold_in_pu[i];
    else
        return 0.0;
}

double UVLS::get_time_delay_in_s_of_stage(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return stage_timer[i].get_timer_interval_in_s();
    else
        return 0.0;
}

double UVLS::get_scale_in_pu_of_stage(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return scale_in_pu[i];
    else
        return 0.0;
}

double UVLS::get_breaker_time_in_s() const
{
    return breaker_timer[0].get_timer_interval_in_s();
}

bool UVLS::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=20)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name=="UVLSAL" or model_name!="UVLSBL" or model_name!="UVLSAR" or model_name!="UVLSZN")
        {
            double t_sensor, tbreak, vth, tdelay, scale;

            unsigned int i=3;

            t_sensor = get_double_data(data[i],"0.0"); ++i;
            set_voltage_sensor_time_in_s(t_sensor);

            tbreak = get_double_data(data[i],"0.0"); ++i;
            set_breaker_time_in_s(tbreak);

            unsigned int stage = 0;

            unsigned int n = data.size()-2;
            for(i=5; i<n; i=i+3)
            {
                vth = get_double_data(data[i],"0.0");
                tdelay = get_double_data(data[i+1],"0.0");
                scale = get_double_data(data[i+2],"0.0");

                set_voltage_threshold_in_pu_of_stage(stage, vth);
                set_time_delay_in_s_of_stage(stage, tdelay);
                set_scale_in_pu_of_stage(stage, scale);

                ++stage;
            }

            if(model_name=="UVLSAL")
                set_subsystem_type(ALL_SYSTEM_TYPE);
            else
            {
                if(model_name=="UVLSBL")
                    set_subsystem_type(BUS_SUBSYSTEM_TYPE);
                else
                {
                    if(model_name=="UVLSAR")
                        set_subsystem_type(AREA_SUBSYSTEM_TYPE);
                    else
                    {//UVLSZN
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

bool UVLS::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool UVLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void UVLS::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit();
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].set_toolkit(toolkit);
        breaker_timer[i].set_toolkit(toolkit);
    }
}

void UVLS::initialize()
{
    LOAD* load = get_load_pointer();
    setup_block_toolkit_and_parameters();

    //double volt = psdb.get_bus_positive_sequence_voltage_in_pu(load->get_load_bus());
    double volt = get_bus_positive_sequence_voltage_in_pu();

    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].set_attached_device(load);
        breaker_timer[i].set_attached_device(load);
    }

    voltage_sensor.set_output(volt);
    voltage_sensor.initialize();

    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
    {
        stage_timer[i].reset();
        breaker_timer[i].reset();
        flag_stage_is_tripped[i]=false;
    }
}

void UVLS::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit();
    double current_time = toolkit.get_dynamic_simulation_time_in_s();

    double volt = get_bus_positive_sequence_voltage_in_pu();

    voltage_sensor.set_input(volt);
    voltage_sensor.run(mode);

    if(mode==RELAY_MODE)
    {
        double v = voltage_sensor.get_output();

        for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        {
            if(is_stage_tripped(i)) // already tripped
                continue;

            // haven't been tripped
            if(is_stage_breaker_timer_started(i))
            {
                if(is_stage_breaker_timer_timed_out(i)) // breaker timed out
                {
                    osstream<<"UVLS stage "<<i<<" timer of "<<get_compound_device_name()<<" is timed out at time "<<current_time<<" s."<<endl
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
                    osstream<<"UVLS stage "<<i<<" timer of "<<get_compound_device_name()<<" is sending tripping signal to breaker at time "<<current_time<<" s since stage delayer timer is timed out."<<endl
                      <<"Current voltage is "<<v<<" pu, and stage voltage threshold is "<<get_voltage_threshold_in_pu_of_stage(i)<<" pu.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    start_stage_breaker_timer(i); // start breaker;
                }
                else// delayer not timed out
                {
                    if(v<=get_voltage_threshold_in_pu_of_stage(i))
                        continue; //do nothing
                    else
                    {
                        // need to reset
                        osstream<<"UVLS stage "<<i<<" timer of "<<get_compound_device_name()<<" is reset at time "<<current_time<<" s due to recovery of voltage."<<endl
                          <<"Current voltage is "<<v<<" pu, and stage voltage threshold is "<<get_voltage_threshold_in_pu_of_stage(i)<<" pu.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        reset_stage_delayer_timer(i);
                    }
                }
                continue;
            }
            else// delayer not started. check if need to start
            {
                if(v<get_voltage_threshold_in_pu_of_stage(i))
                {
                    osstream<<"UVLS stage "<<i<<" timer of "<<get_compound_device_name()<<" is started at time "<<current_time<<" s due to drop of voltage."<<endl
                      <<"Current voltage is "<<v<<" pu, and stage voltage threshold is "<<get_voltage_threshold_in_pu_of_stage(i)<<" pu.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    start_stage_delayer_timer(i);
                }
            }
        }
    }
}


double UVLS::get_total_shed_scale_factor_in_pu() const
{
    double total_scale = 0.0;
    for(unsigned int i=0; i!=STEPS_MAX_LOAD_RELAY_STAGE; ++i)
        total_scale += (is_stage_tripped(i)*get_scale_in_pu_of_stage(i));
    return total_scale;
}

bool UVLS::is_stage_delayer_timer_started(unsigned int i) const
{
    return stage_timer[i].is_started();
}

bool UVLS::is_stage_breaker_timer_started(unsigned int i) const
{
    return breaker_timer[i].is_started();
}

void UVLS::start_stage_delayer_timer(unsigned int i)
{
    if(not stage_timer[i].is_started())
        stage_timer[i].start();
}

void UVLS::start_stage_breaker_timer(unsigned int i)
{
    if(not breaker_timer[i].is_started())
    {
        breaker_timer[i].start();
        reset_stage_delayer_timer(i);
    }
}

void UVLS::reset_stage_delayer_timer(unsigned int i)
{
    if(stage_timer[i].is_started())
        stage_timer[i].reset();
}

void UVLS::reset_stage_breaker_timer(unsigned int i)
{
    if(breaker_timer[i].is_started())
        breaker_timer[i].reset();
}

bool UVLS::is_stage_delayer_timer_timed_out(unsigned int i) const
{
    if(stage_timer[i].is_started())
        return stage_timer[i].is_timed_out();
    else
        return false;
}

bool UVLS::is_stage_breaker_timer_timed_out(unsigned int i) const
{
    if(breaker_timer[i].is_started())
        return breaker_timer[i].is_timed_out();
    else
        return false;
}

void UVLS::trip_stage(unsigned int i)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
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

bool UVLS::is_stage_tripped(unsigned int i) const
{
    if(i<STEPS_MAX_LOAD_RELAY_STAGE)
        return flag_stage_is_tripped[i];
    else
        return false;

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

string UVLS::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    unsigned int bus = load->get_load_bus();
    string identifier = "'"+load->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"BL'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(4)<<fixed<<get_voltage_sensor_time_in_s()<<", "
            <<setw(8)<<setprecision(4)<<fixed<<get_breaker_time_in_s();

    unsigned int n_content = 5;
    double vth, tdelay, scale;
    unsigned int i=0;
    for(i=0; i!=(STEPS_MAX_LOAD_RELAY_STAGE-1); ++i)
    {
        vth = get_voltage_threshold_in_pu_of_stage(i);
        tdelay = get_time_delay_in_s_of_stage(i);
        scale = get_scale_in_pu_of_stage(i);
        if(fabs(vth)<DOUBLE_EPSILON)
            break;
        osstream<<", "
                <<setw(8)<<setprecision(4)<<fixed<<vth<<", ";
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
        osstream<<setw(8)<<setprecision(4)<<fixed<<scale;
        n_content++;
        if(n_content==10)
        {
            osstream<<"\n"
                    <<setw(10)<<"";
            n_content = 1;
        }
    }
    i = STEPS_MAX_LOAD_RELAY_STAGE - 1;
    vth = get_voltage_threshold_in_pu_of_stage(i);
    tdelay = get_time_delay_in_s_of_stage(i);
    scale = get_scale_in_pu_of_stage(i);
    if(fabs(vth)>=DOUBLE_EPSILON)
    {
        osstream<<", "
                <<setw(8)<<setprecision(4)<<fixed<<vth<<", ";
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
        osstream<<setw(8)<<setprecision(4)<<fixed<<scale;
    }
    osstream<<" /";

    return osstream.str();
}

void UVLS::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TV", i); i++;
    add_model_data_name_and_index_pair("TB", i); i++;
    for(unsigned int stage=0; stage<STEPS_MAX_LOAD_RELAY_STAGE; ++stage)
    {
        string name = "VTH "+num2str(stage);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "TD "+num2str(stage);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "P "+num2str(stage);
        add_model_data_name_and_index_pair(name, i); i++;
    }
}

double UVLS::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="TV")
        return get_voltage_sensor_time_in_s();
    if(par_name=="TB")
        return get_breaker_time_in_s();
    for(unsigned int stage=0; stage<STEPS_MAX_LOAD_RELAY_STAGE; ++stage)
    {
        string name = "VTH "+num2str(stage);
        if(par_name==name)
            return get_voltage_threshold_in_pu_of_stage(stage);
        name = "TD "+num2str(stage);
        if(par_name==name)
            return get_time_delay_in_s_of_stage(stage);
        name = "P "+num2str(stage);
        if(par_name==name)
            return get_scale_in_pu_of_stage(stage);
    }
    return 0.0;
}

void UVLS::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="TV")
        return set_voltage_sensor_time_in_s(value);
    if(par_name=="TB")
        return set_breaker_time_in_s(value);
    for(unsigned int stage=0; stage<STEPS_MAX_LOAD_RELAY_STAGE; ++stage)
    {
        string name = "VTH "+num2str(stage);
        if(par_name==name)
            return set_voltage_threshold_in_pu_of_stage(stage, value);
        name = "TD "+num2str(stage);
        if(par_name==name)
            return set_time_delay_in_s_of_stage(stage, value);
        name = "P "+num2str(stage);
        if(par_name==name)
            return set_scale_in_pu_of_stage(stage, value);
    }

    return;
}

double UVLS::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void UVLS::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("SHED SCALE IN PU", i); i++;
}

double UVLS::get_model_internal_variable_with_name(string var_name)
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
