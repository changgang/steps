#include "header/model/load_relay_model/PUFLS.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include<iostream>

using namespace std;

PUFLS::PUFLS()
{
    clear();
}

PUFLS::~PUFLS()
{
    clear();
}

void PUFLS::clear()
{
    set_frequency_sensor_time_in_s(1.0);
    set_continuous_frequency_threshold_in_Hz(0.0);
    set_time_delay_in_s(0.0);
    set_scale_K_in_pu_per_Hz(0.0);
    set_maximum_continuous_shed_scale_in_pu(0.0);

    set_additional_stage_trigger_signal(REALTIME_FREQUENCY);
    set_additional_stage_frequency_threshold_in_Hz(0.0);
    set_additional_stage_shed_scale_in_pu(0.0);
    set_additional_stage_time_delay_in_s(0.0);

    flag_additional_stage_is_tripped = false;

    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
    {
        discrete_stage_shed_scale_in_pu[stage] = 0.0;
        discrete_stage_timer[stage].reset();
        flag_discrete_stage_is_tripped[stage] = false;
    }

    current_continuous_shed_command_in_pu = 0.0;
}

void PUFLS::copy_from_const_model(const PUFLS& model)
{
    clear();
    set_frequency_sensor_time_in_s(model.get_frequency_sensor_time_in_s());
    set_continuous_frequency_threshold_in_Hz(model.get_continuous_frequency_threshold_in_Hz());
    set_time_delay_in_s(model.get_time_delay_in_s());
    set_scale_K_in_pu_per_Hz(model.get_scale_K_in_pu_per_Hz());
    set_maximum_continuous_shed_scale_in_pu(model.get_maximum_continuous_shed_scale_in_pu());

    set_additional_stage_trigger_signal(model.get_additional_stage_trigger_signal());
    set_additional_stage_frequency_threshold_in_Hz(model.get_additional_stage_frequency_threshold_in_Hz());
    set_additional_stage_shed_scale_in_pu(model.get_additional_stage_shed_scale_in_pu());
    set_additional_stage_time_delay_in_s(model.get_additional_stage_time_delay_in_s());

    set_discrete_stage_time_delay_in_s(model.get_discrete_stage_time_delay_in_s());
    for(size_t stage =0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
        set_discrete_stage_shed_scale_in_pu(stage, model.get_discrete_stage_shed_scale_in_pu(stage));
 }

PUFLS::PUFLS(const PUFLS& model) : LOAD_FREQUENCY_RELAY_MODEL()
{
    copy_from_const_model(model);
}

PUFLS& PUFLS::operator=(const PUFLS& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return *this;
}

string PUFLS::get_model_name() const
{
    return "PUFLS";
}

double PUFLS::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double PUFLS::get_double_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="")
        return 0.0;

    return 0.0;
}

void PUFLS::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void PUFLS::set_double_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void PUFLS::set_frequency_sensor_time_in_s(double t)
{
    frequency_sensor.set_T_in_s(t);
}

void PUFLS::set_continuous_frequency_threshold_in_Hz(double f)
{
    continuous_frequency_threshold_in_Hz = f;
}

void PUFLS::set_scale_K_in_pu_per_Hz(double K)
{
    this->K = K;
}

void PUFLS::set_time_delay_in_s(double t)
{
    t_delay = t;
}

void PUFLS::set_maximum_continuous_shed_scale_in_pu(double scale)
{
    max_continuous_shed_scale = scale;
}

void PUFLS::set_additional_stage_trigger_signal(UFLS_TRIGGER_SIGNAL signal)
{
    trigger_signal = signal;
}

void PUFLS::set_additional_stage_frequency_threshold_in_Hz(double f)
{
    additional_stage_frequency_threshold_in_Hz = f;
}

void PUFLS::set_additional_stage_time_delay_in_s(double t)
{
    additional_stage_timer.set_timer_interval_in_s(t);
}

void PUFLS::set_additional_stage_shed_scale_in_pu(double scale)
{
    additional_shed_scale = scale;
}

void PUFLS::set_discrete_stage_time_delay_in_s(double t)
{
    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
    {
        discrete_stage_timer[stage].set_timer_interval_in_s(t);
    }
}

void PUFLS::set_discrete_stage_shed_scale_in_pu(size_t stage, double scale)
{
    if(stage<MAX_LOAD_RELAY_STAGE)
        discrete_stage_shed_scale_in_pu[stage] = scale;
}

double PUFLS::get_frequency_sensor_time_in_s() const
{
    return frequency_sensor.get_T_in_s();
}

double PUFLS::get_continuous_frequency_threshold_in_Hz() const
{
    return continuous_frequency_threshold_in_Hz;
}

double PUFLS::get_scale_K_in_pu_per_Hz() const
{
    return K;
}

double PUFLS::get_time_delay_in_s() const
{
    return t_delay;
}

double PUFLS::get_maximum_continuous_shed_scale_in_pu() const
{
    return max_continuous_shed_scale;
}

UFLS_TRIGGER_SIGNAL PUFLS::get_additional_stage_trigger_signal() const
{
    return trigger_signal;
}

double PUFLS::get_additional_stage_frequency_threshold_in_Hz() const
{
    return additional_stage_frequency_threshold_in_Hz;
}

double PUFLS::get_additional_stage_time_delay_in_s() const
{
    return additional_stage_timer.get_timer_interval_in_s();
}

double PUFLS::get_additional_stage_shed_scale_in_pu() const
{
    return additional_shed_scale;
}

double PUFLS::get_discrete_stage_time_delay_in_s() const
{
    return discrete_stage_timer[0].get_timer_interval_in_s();
}

double PUFLS::get_discrete_stage_shed_scale_in_pu(size_t stage) const
{
    if(stage<MAX_LOAD_RELAY_STAGE)
        return discrete_stage_shed_scale_in_pu[stage];
    else
        return 0.0;
}

bool PUFLS::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

bool PUFLS::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");

    size_t n = dyrdata.size();

    if(n<12)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!="PUFLSAL" and model_name!="PUFLSAR" and model_name!="PUFLSZN" and model_name!="PUFLSBL")
        return is_successful;

    double t_freq, fth_continuous, tdelay, K_scale, max_scale;
    size_t signal_flag;
    double fth_additional, additional_scale, additional_time_delay;

    size_t i=3;
    signal_flag = get_integer_data(dyrdata[i], "0"); i++;
    t_freq = get_double_data(dyrdata[i],"0.0"); i++;
    fth_continuous = get_double_data(dyrdata[i],"0.0"); i++;
    K_scale = get_double_data(dyrdata[i],"0.0"); i++;
    tdelay = get_double_data(dyrdata[i],"0.0"); i++;
    max_scale = get_double_data(dyrdata[i],"0.0"); i++;

    set_frequency_sensor_time_in_s(t_freq);
    set_continuous_frequency_threshold_in_Hz(fth_continuous);
    set_time_delay_in_s(tdelay);
    set_scale_K_in_pu_per_Hz(K_scale);
    set_maximum_continuous_shed_scale_in_pu(max_scale);

    fth_additional = get_double_data(dyrdata[i],"0.0"); i++;
    additional_time_delay = get_double_data(dyrdata[i],"0.0"); i++;
    additional_scale = get_double_data(dyrdata[i],"0.0"); i++;

    if(signal_flag==0)
        set_additional_stage_trigger_signal(REALTIME_FREQUENCY);
    else
        set_additional_stage_trigger_signal(MINIMUM_FREQUENCY);
    set_additional_stage_frequency_threshold_in_Hz(fth_additional);
    set_additional_stage_time_delay_in_s(additional_time_delay);
    set_additional_stage_shed_scale_in_pu(additional_scale);

    if(n>=12)
    {
        tdelay = get_double_data(dyrdata[i],"0.0"); i++;
        set_discrete_stage_time_delay_in_s(tdelay);

        for(size_t stage=0; i!=n and stage!=MAX_LOAD_RELAY_STAGE; ++i, ++stage)
        {
            double scale = get_double_data(dyrdata[i],"0.0");
            set_discrete_stage_shed_scale_in_pu(stage, scale);
        }
    }


    if(model_name=="PUFLSAL")
        set_subsystem_type(ALL_SYSTEM_TYPE);
    else
    {
        if(model_name=="PUFLSAR")
            set_subsystem_type(AREA_SUBSYSTEM_TYPE);
        else
        {
            if(model_name=="PUFLSZN")
                set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
            else
            {
                //PUFLSBL
                set_subsystem_type(BUS_SUBSYSTEM_TYPE);
            }
        }
    }
    is_successful = true;

    return is_successful;
}

bool PUFLS::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return false;
}

void PUFLS::initialize()
{
    LOAD* load = get_load_pointer();
    if(load==NULL)
        return;

    POWER_SYSTEM_DATABASE* psdb = load->get_power_system_database();
    double fbase = psdb->get_system_base_frequency_in_Hz();

    additional_stage_timer.set_attached_device(load);
    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
        discrete_stage_timer[stage].set_attached_device(load);

    double t_delay = get_time_delay_in_s();
    double delt = get_dynamic_simulation_time_step_in_s();

    frequency_sensor.set_output(fbase);
    frequency_sensor.initialize();

    double current_time = get_dynamic_simulation_time_in_s();

    history_minimum_frequency_buffer.set_buffer_size(2*(size_t)(t_delay/delt)+2);
    history_minimum_frequency_buffer.initialize_buffer(current_time, fbase);

    additional_stage_timer.reset();
    flag_additional_stage_is_tripped = false;

    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
    {
        discrete_stage_timer[stage].reset();
        flag_discrete_stage_is_tripped[stage] = false;
    }

    current_continuous_shed_command_in_pu = 0.0;
}

void PUFLS::run(DYNAMIC_MODE mode)
{
    ostringstream osstream;

    double TIME = get_dynamic_simulation_time_in_s();

    double freq = get_bus_frequency_in_Hz();

    frequency_sensor.set_input(freq);
    frequency_sensor.run(mode);

    append_new_minimum_frequency();

    if(mode==UPDATE_MODE or mode==INTEGRATE_MODE)
    {

        size_t N = get_number_of_discrete_stage_to_meet_total_continuous_shed_scale();
        //cout<<"N="<<N<<endl;
        // try to shed lines
        for(size_t stage=0; stage!=N; ++stage)
        {
            if(is_discrete_stage_tipped(stage))
                continue;

            if(is_discrete_stage_timer_started(stage))
            {
                if(is_discrete_stage_timer_timed_out(stage) and is_minimum_frequency_declining())
                {
                    osstream<<"PUFLS discrete stage "<<stage<<" timer of "<<get_device_name()<<" is timed out at time "<<TIME<<" s."<<endl
                      <<get_discrete_stage_shed_scale_in_pu(stage)*100.0<<"% loads are tripped.";
                    show_information_with_leading_time_stamp(osstream);
                    trip_discrete_stage(stage);
                    continue;
                }
            }
            else
            {
                osstream<<"PUFLS discrete stage "<<stage<<" timer of "<<get_device_name()<<" is started at time "<<TIME<<" s."<<endl
                  <<get_discrete_stage_shed_scale_in_pu(stage)*100.0<<"% loads will be tripped.";
                show_information_with_leading_time_stamp(osstream);
                start_discrete_stage_timer_of_stage(stage);
                continue;
            }
        }
    }
    if(mode==UPDATE_MODE)
    {
        update_continuous_shed_command();
        try_to_shed_additional_stage();
    }
}

void PUFLS::append_new_minimum_frequency()
{
    ostringstream osstream;

    double current_time = get_dynamic_simulation_time_in_s();
    double delt = get_dynamic_simulation_time_step_in_s();

    double current_freq = frequency_sensor.get_output();
    //double previous_minimum_freq = history_minimum_frequency_buffer.get_buffer_value_at_head();
    double previous_minimum_freq = history_minimum_frequency_buffer.get_buffer_value_at_time(current_time-delt);
    //cout<<"at time "<<STEPS::TIME<<": freq = "<<current_freq<<", previous minimum = "<<previous_minimum_freq<<endl;

    if(current_freq<previous_minimum_freq)
        history_minimum_frequency_buffer.append_data(current_time, current_freq);
    else
        history_minimum_frequency_buffer.append_data(current_time, previous_minimum_freq);
/*
    size_t bus = ((LOAD*) get_device_pointer())->get_load_bus();
    if(bus==3)
    {
        osstream<<setw(10)<<setprecision(6)
                <<"At time "<<current_time<<"s, "<<get_device_name()<<endl
                <<"Current bus frequency is: "<<get_bus_frequency_in_Hz()<<"Hz"<<endl
                <<"Current sensed frequency is: "<<frequency_sensor.get_output()<<"Hz"<<endl
                <<"Current minimum frequency is: "<<history_minimum_frequency_buffer.get_buffer_value_at_head()<<"Hz";
        show_information_with_leading_time_stamp(osstream);
        history_minimum_frequency_buffer.show_buffer();
    }
*/
}

void PUFLS::update_continuous_shed_command()
{
    ostringstream osstream;

    double current_time = get_dynamic_simulation_time_in_s();

    double f_th = get_continuous_frequency_threshold_in_Hz();
    double delay = get_time_delay_in_s();
    double K = get_scale_K_in_pu_per_Hz();

    if(is_minimum_frequency_declining())
    {
        double delayed_minimum_freq = history_minimum_frequency_buffer.get_buffer_value_at_time(current_time-delay);
        if(delayed_minimum_freq<f_th)
        {
            double delta_frequency = f_th-delayed_minimum_freq;
            current_continuous_shed_command_in_pu = K*delta_frequency;
            /*size_t bus = ((LOAD*) get_device_pointer())->get_load_bus();
            if(bus==3)
            {
                osstream<<setw(10)<<setprecision(6)
                        <<"At time "<<current_time<<"s, "<<get_device_name()<<" minimum frequency is declining."<<endl
                        <<"Current bus frequency is: "<<get_bus_frequency_in_Hz()<<"Hz"<<endl
                        <<"Current sensed frequency is: "<<frequency_sensor.get_output()<<"Hz"<<endl
                        <<"Current minimum frequency is: "<<history_minimum_frequency_buffer.get_buffer_value_at_head()<<"Hz"<<endl
                        <<"Delayed minimum frequency is: "<<delayed_minimum_freq<<"Hz"<<endl
                        <<"Current shed command is: "<<current_continuous_shed_command_in_pu;
                show_information_with_leading_time_stamp(osstream);
                history_minimum_frequency_buffer.show_buffer();
            }*/
        }
    }
}

double PUFLS::get_continuous_shed_command_in_pu() const
{
    return current_continuous_shed_command_in_pu;
}

double PUFLS::get_total_discrete_shed_scale_in_pu() const
{
    double scale = 0.0;
    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
    {
        if(is_discrete_stage_tipped(stage))
            scale += discrete_stage_shed_scale_in_pu[stage];
    }
    return scale;
}

size_t PUFLS::get_number_of_discrete_stage_to_meet_total_continuous_shed_scale() const
{
    double total_shed_command = get_continuous_shed_command_in_pu();
    double discrete_shed_command = 0.0;

    double c_max = get_maximum_continuous_shed_scale_in_pu();
    if(total_shed_command>c_max)
        discrete_shed_command = total_shed_command - c_max;
    else
        return 0;

    size_t N=0;
    double total_to_shed = 0.0;
    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
    {
        double shed_scale_of_stage = get_discrete_stage_shed_scale_in_pu(stage);

        if((discrete_shed_command-(total_to_shed+shed_scale_of_stage))<=FLOAT_EPSILON)
            break;
        total_to_shed += shed_scale_of_stage;
        N++;
    }
    return N;
}

double PUFLS::get_sum_of_shed_scale_of_first_n_discrete_stages_in_pu(size_t n) const
{
    double total = 0.0;
    for(size_t stage=0; stage!=n and stage!=MAX_LOAD_RELAY_STAGE; ++stage)
    {
        total += get_discrete_stage_shed_scale_in_pu(stage);
    }
    return total;
}

double PUFLS::get_total_shed_scale_of_all_discrete_stages_in_pu() const
{
    return get_sum_of_shed_scale_of_first_n_discrete_stages_in_pu(MAX_LOAD_RELAY_STAGE);
}

bool PUFLS::is_discrete_stage_tipped(size_t stage) const
{
    if(stage<MAX_LOAD_RELAY_STAGE)
        return flag_discrete_stage_is_tripped[stage];
    else
        return false;
}

bool PUFLS::is_discrete_stage_timer_timed_out(size_t stage)
{
    if(stage<MAX_LOAD_RELAY_STAGE)
        return discrete_stage_timer[stage].is_timed_out();
    else
        return false;
}

void PUFLS::trip_discrete_stage(size_t stage)
{
    if(stage<MAX_LOAD_RELAY_STAGE)
    {
        if(not is_discrete_stage_tipped(stage))
        {
            flag_discrete_stage_is_tripped[stage] = true;
            discrete_stage_timer[stage].reset();
        }
    }
}

bool PUFLS::is_discrete_stage_timer_started(size_t stage) const
{
    if(stage<MAX_LOAD_RELAY_STAGE)
        return discrete_stage_timer[stage].is_started();
    else
        return false;
}

void PUFLS::start_discrete_stage_timer_of_stage(size_t stage)
{
    if(stage<MAX_LOAD_RELAY_STAGE)
    {
        if(not is_discrete_stage_timer_started(stage))
            discrete_stage_timer[stage].start();
    }
}

void PUFLS::reset_discrete_stage_timer_of_stage(size_t stage)
{
    if(stage<MAX_LOAD_RELAY_STAGE)
    {
        if(is_discrete_stage_timer_started(stage))
            discrete_stage_timer[stage].reset();
    }
}

void PUFLS::try_to_shed_additional_stage()
{
    if(is_additional_stage_set()) // additional stage is set
    {
        if(not is_additional_stage_tripped())
        {
            if(is_additional_stage_timer_started())
            {
                if(is_additional_stage_timer_timed_out())
                    trip_additional_stage();
                else
                    try_to_reset_additional_stage_timer();
            }
            else // additional stage is not started
                try_to_start_additional_stage_timer();
        }
    }
}

bool PUFLS::is_additional_stage_set() const
{
    return fabs(get_additional_stage_shed_scale_in_pu())>FLOAT_EPSILON;
}


bool PUFLS::is_additional_stage_tripped() const
{
    return flag_additional_stage_is_tripped;
}

bool PUFLS::is_additional_stage_timer_timed_out() const
{
    return additional_stage_timer.is_timed_out();
}

bool PUFLS::is_additional_stage_timer_started() const
{
    return additional_stage_timer.is_started();
}

void PUFLS::trip_additional_stage()
{
    if(is_additional_stage_tripped())
        return;

    double current_time = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    osstream<<"PUFLS additional stage of "<<get_device_name()<<" is timed out at time "
      <<setprecision(3)<<fixed<<current_time<<" s. "<<endl
      <<setprecision(4)<<fixed<<get_additional_stage_shed_scale_in_pu()*100.0<<"% loads is tripped.";
    show_information_with_leading_time_stamp(osstream);
    flag_additional_stage_is_tripped = true;
    additional_stage_timer.reset();
}

void PUFLS::try_to_start_additional_stage_timer()
{
    if(is_additional_stage_timer_started())
        return;

    double current_time = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    double current_freq = frequency_sensor.get_output();
    double f_th = get_additional_stage_frequency_threshold_in_Hz();
    switch(get_additional_stage_trigger_signal())
    {
        case REALTIME_FREQUENCY:
        {
            if(current_freq<f_th)
            {
                osstream<<"PUFLS additional stage timer of "<<get_device_name()<<" is started at time "<<current_time
                  <<" s due to drop of frequency."<<endl
                  <<"Current frequency is "<<current_freq<<" Hz, and stage frequency threshold is "<<f_th<<" Hz.",
                show_information_with_leading_time_stamp(osstream);
                start_additional_stage_timer();
            }
            break;
        }
        default: // MINIMUM_FREQUENCY
        {
            if(current_freq<f_th)
            {
                if(is_frequency_recovering_beyond_current_minimum_frequency()) // frequency is recovering
                {
                    osstream<<"PUFLS additional stage timer of "<<get_device_name()<<" is started at time "
                      <<current_time<<" s due to recovery of frequency."<<endl
                      <<"Current frequency is "<<current_freq<<" Hz.";
                    show_information_with_leading_time_stamp(osstream);
                    start_additional_stage_timer();
                }
                else
                {
                    if(is_minimum_frequency_not_changing())
                    {
                        osstream<<"PUFLS additional stage timer of "<<get_device_name()<<" is started at time "
                          <<current_time<<" s since minimum frequency is not changing."<<endl
                          <<"Current frequency is "<<current_freq<<" Hz.";
                        show_information_with_leading_time_stamp(osstream);
                        start_additional_stage_timer();
                    }
                }
            }
            break;
        }
    }
}
void PUFLS::start_additional_stage_timer()
{
    if(is_additional_stage_timer_started())
        return;

    additional_stage_timer.start();
}

void PUFLS::try_to_reset_additional_stage_timer()
{
    if(not is_additional_stage_timer_started())
        return;

    double current_time = get_dynamic_simulation_time_in_s();

    ostringstream osstream;
    double current_freq = frequency_sensor.get_output();
    double f_th = get_additional_stage_frequency_threshold_in_Hz();
    switch(get_additional_stage_trigger_signal())
    {
        case REALTIME_FREQUENCY:
        {
            if(current_freq>f_th)
            {
                osstream<<"PUFLS additional stage timer of "<<get_device_name()<<" is reset at time "<<current_time
                  <<" s due to recovery of frequency."<<endl
                  <<"Current frequency is "<<current_freq<<" Hz, and stage frequency threshold is "<<f_th<<" Hz.";
                show_information_with_leading_time_stamp(osstream);
                reset_additional_stage_timer();
            }
            break;
        }
        default: // MINIMUM_FREQUENCY
        {
            if(is_minimum_frequency_declining())
            {
                osstream<<"PUFLS additional stage timer of "<<get_device_name()<<" is reset at time "<<current_time
                  <<" s due to declining of minimum frequency."<<endl
                  <<"Current frequency is "<<current_freq<<" Hz.";
                show_information_with_leading_time_stamp(osstream);
                reset_additional_stage_timer();
            }
            break;
        }
    }
}

void PUFLS::reset_additional_stage_timer()
{
    if(not is_additional_stage_timer_started())
        return;
    additional_stage_timer.reset();
}

bool PUFLS::is_frequency_recovering_beyond_current_minimum_frequency() const
{
    double current_freq = frequency_sensor.get_output();
    double current_minimum_freq = history_minimum_frequency_buffer.get_buffer_value_at_head();

    return (current_freq-current_minimum_freq)>FLOAT_EPSILON;
}

bool PUFLS::is_minimum_frequency_declining() const
{
    double current_minimum_freq = history_minimum_frequency_buffer.get_buffer_value_at_head();
    double previous_minimum_freq = history_minimum_frequency_buffer.get_buffer_value_at_delay_index(1);

    return (current_minimum_freq-previous_minimum_freq)<-FLOAT_EPSILON;
}

bool PUFLS::is_minimum_frequency_not_changing() const
{
    return not is_minimum_frequency_declining();
}

double PUFLS::get_total_shed_scale_factor_in_pu() const
{
    double total_shed_scale = 0.0;

    double total_continous_shed = get_continuous_shed_command_in_pu();
    double c_max = get_maximum_continuous_shed_scale_in_pu();

    if(total_continous_shed<=c_max)
        total_shed_scale += total_continous_shed;
    else
    {
        total_shed_scale += c_max;
        total_shed_scale += get_total_discrete_shed_scale_in_pu();
    }

    if(flag_additional_stage_is_tripped)
        total_shed_scale += get_additional_stage_shed_scale_in_pu();
    //cout<<"total shed scale = "<<total_shed_scale<<endl;

    return total_shed_scale;
}

void PUFLS::check()
{
    ;
}

void PUFLS::report()
{
    ;
}

void PUFLS::save()
{
    ;
}

string PUFLS::get_standard_model_string() const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    size_t bus = load->get_load_bus();
    string identifier = load->get_identifier();
    osstream<<bus<<", "
      <<"'"<<get_detailed_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setprecision(1)<<fixed<<(get_additional_stage_trigger_signal()==REALTIME_FREQUENCY ? 0 : 1)<<", "
      <<setprecision(4)<<fixed<<get_frequency_sensor_time_in_s()<<", "
      <<setprecision(4)<<fixed<<get_continuous_frequency_threshold_in_Hz()<<", "
      <<setprecision(4)<<fixed<<get_scale_K_in_pu_per_Hz()<<", "
      <<setprecision(4)<<fixed<<get_time_delay_in_s()<<", "
      <<setprecision(4)<<fixed<<get_additional_stage_frequency_threshold_in_Hz()<<", "
      <<setprecision(4)<<fixed<<get_additional_stage_time_delay_in_s()<<", "
      <<setprecision(4)<<fixed<<get_additional_stage_shed_scale_in_pu()<<", "
      <<setprecision(4)<<fixed<<get_discrete_stage_time_delay_in_s();
    for(size_t stage=0; stage!=MAX_LOAD_RELAY_STAGE; ++stage)
        osstream<<", "
          <<setprecision(4)<<fixed<<get_discrete_stage_shed_scale_in_pu(stage);

    osstream<<"  /";
    return osstream.str();
}

size_t PUFLS::get_variable_index_from_variable_name(string var_name)
{
    if(var_name=="SCALE")
        return 0;

    return INDEX_NOT_EXIST;
}


string PUFLS::get_variable_name_from_variable_index(size_t var_index)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_index is provided: "<<var_index;
    show_information_with_leading_time_stamp(osstream);
    return "";
}

double PUFLS::get_variable_with_index(size_t var_index)
{
    switch(var_index)
    {
        case 0:
            return get_total_shed_scale_factor_in_pu();
        default:
            return 0.0;
    }
}
double PUFLS::get_variable_with_name(string var_name)
{
    size_t index = get_variable_index_from_variable_name(var_name);
    return get_variable_with_index(index);
}

string PUFLS::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PUFLS::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PUFLS::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
