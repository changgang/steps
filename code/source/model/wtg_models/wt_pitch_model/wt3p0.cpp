#include "header/model/wtg_models/wt_pitch_model/wt3p0.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;
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
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_hold_wtg_speed_flag(false);

    speed_reference_sensor.set_limiter_type(NO_LIMITER);

    speed_controller.set_limiter_type(NO_LIMITER);

    frequency_sensor.set_limiter_type(NO_LIMITER);
    frequency_sensor.set_K(1.0);

    frequency_controller.set_limiter_type(NO_LIMITER);
    set_Kd_frequency_controller(0.0);
    set_Td_frequency_controller_in_s(0.5);

    pitch_integrator.set_limiter_type(NON_WINDUP_LIMITER);

    set_ratePitchmax_in_deg_per_s(0.0);
}

void WT3P0::copy_from_const_model(const WT3P0& model)
{
    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    set_Tspeed_in_s(model.get_Tspeed_in_s());
    set_Kp_speed_controller(model.get_Kp_speed_controller());
    set_Ki_speed_controller(model.get_Ki_speed_controller());
    set_Tfrequency_in_s(model.get_Tfrequency_in_s());
    set_frequency_upper_deadband_in_pu(model.get_frequency_upper_deadband_in_pu());
    set_frequency_lower_deadband_in_pu(model.get_frequency_lower_deadband_in_pu());
    set_Kp_frequency_controller(model.get_Kp_frequency_controller());
    set_Ki_frequency_controller(model.get_Ki_frequency_controller());
    set_Kd_frequency_controller(model.get_Kd_frequency_controller());
    set_Td_frequency_controller_in_s(model.get_Td_frequency_controller_in_s());
    set_Pitchmax_in_deg(model.get_Pitchmax_in_deg());
    set_Pitchmin_in_deg(model.get_Pitchmin_in_deg());
    set_ratePitchmax_in_deg_per_s(model.get_ratePitchmax_in_deg_per_s());
    set_Tp_in_s(model.get_Tp_in_s());
    set_hold_wtg_speed_flag(model.get_hold_wtg_speed_flag());
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

void WT3P0::set_Tspeed_in_s(double T)
{
    speed_reference_sensor.set_T_in_s(T);
}

void WT3P0::set_Kp_speed_controller(double K)
{
    speed_controller.set_Kp(K);
}

void WT3P0::set_Ki_speed_controller(double K)
{
    speed_controller.set_Ki(K);
}

void WT3P0::set_Tfrequency_in_s(double T)
{
    frequency_sensor.set_T_in_s(T);
}

void WT3P0::set_Kp_frequency_controller(double K)
{
    frequency_controller.set_Kp(K);
}

void WT3P0::set_Ki_frequency_controller(double K)
{
    frequency_controller.set_Ki(K);
}

void WT3P0::set_Kd_frequency_controller(double K)
{
    frequency_controller.set_Kd(K);
}

void WT3P0::set_Td_frequency_controller_in_s(double T)
{
    frequency_controller.set_Td_in_s(T);
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

void WT3P0::set_Tp_in_s(double T)
{
    pitch_integrator.set_T_in_s(T);
}

double WT3P0::get_Tspeed_in_s() const
{
    return speed_reference_sensor.get_T_in_s();
}

double WT3P0::get_Kp_speed_controller() const
{
    return speed_controller.get_Kp();
}

double WT3P0::get_Ki_speed_controller() const
{
    return speed_controller.get_Ki();
}

double WT3P0::get_Tfrequency_in_s() const
{
    return frequency_sensor.get_T_in_s();
}

double WT3P0::get_Kp_frequency_controller() const
{
    return frequency_controller.get_Kp();
}

double WT3P0::get_Ki_frequency_controller() const
{
    return frequency_controller.get_Ki();
}

double WT3P0::get_Kd_frequency_controller() const
{
    return frequency_controller.get_Kd();
}

double WT3P0::get_Td_frequency_controller_in_s() const
{
    return frequency_controller.get_Td_in_s();
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

double WT3P0::get_Tp_in_s() const
{
    return pitch_integrator.get_T_in_s();
}

bool WT3P0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=18)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            int hold_speed_flag = 0;
            double tp, kps, kis, tf, fup, flow, kpf, kif, kdf, tdf, rpmax, pmax, pmin, tspeed;

            size_t i=3;
            hold_speed_flag = get_integer_data(data[i],"0"); i++;
            tspeed = get_double_data(data[i],"0.0"); i++;
            kps = get_double_data(data[i],"0.0"); i++;
            kis = get_double_data(data[i],"0.0"); i++;
            tf = get_double_data(data[i],"0.0"); i++;
            flow = get_double_data(data[i],"1.0"); i++;
            fup = get_double_data(data[i],"1.0"); i++;
            kpf = get_double_data(data[i],"0.0"); i++;
            kif = get_double_data(data[i],"0.0"); i++;
            kdf = get_double_data(data[i],"0.0"); i++;
            tdf = get_double_data(data[i],"0.0"); i++;
            rpmax = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            tp = get_double_data(data[i],"0.0");

            if(hold_speed_flag==0)
                set_hold_wtg_speed_flag(false);
            else
                set_hold_wtg_speed_flag(true);

            set_Tspeed_in_s(tspeed);
            set_Kp_speed_controller(kps);
            set_Ki_speed_controller(kis);
            set_Tfrequency_in_s(tf);
            set_frequency_upper_deadband_in_pu(fup);
            set_frequency_lower_deadband_in_pu(flow);
            set_Kp_frequency_controller(kpf);
            set_Ki_frequency_controller(kif);
            set_Kd_frequency_controller(kdf);
            set_Td_frequency_controller_in_s(tdf);
            set_ratePitchmax_in_deg_per_s(rpmax);
            set_Pitchmin_in_deg(pmin);
            set_Pitchmax_in_deg(pmax);
            set_Tp_in_s(tp);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool WT3P0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool WT3P0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void WT3P0::set_block_toolkit()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    speed_reference_sensor.set_toolkit(toolkit);
    speed_controller.set_toolkit(toolkit);
    frequency_sensor.set_toolkit(toolkit);
    frequency_controller.set_toolkit(toolkit);
    pitch_integrator.set_toolkit(toolkit);
}

void WT3P0::initialize()
{
    ostringstream osstream;
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator!=NULL)
    {
        WT_AERODYNAMIC_MODEL* aerdmodel = wt_generator->get_wt_aerodynamic_model();
        if(aerdmodel!=NULL)
        {
            if(not aerdmodel->is_model_initialized())
                aerdmodel->initialize();

            set_block_toolkit();

            STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

            double pitch0 = get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model();

            pitch_integrator.set_output(pitch0);
            pitch_integrator.initialize();

            double pmax = get_Pitchmax_in_deg();
            double pmin = get_Pitchmin_in_deg();
            if(pitch0>pmax)
            {
                osstream<<"Initialization error. Pitch of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                  <<"Pitch angle is "<<pitch0<<" deg, and Pitchmax is "<<pmax<<" deg.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(pitch0<pmin)
            {
                osstream<<"Initialization error. Pitch of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                  <<"Pitch angle is "<<pitch0<<" deg, and Pitchmin is "<<pmin<<" deg.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            speed_controller.set_output(pitch0);
            speed_controller.initialize();

            double speed_ref = get_wt_generator_reference_speed_in_pu();
            speed_reference_sensor.set_output(speed_ref);
            speed_reference_sensor.initialize();
            if(get_hold_wtg_speed_flag()==true)
                set_const_wtg_speed_reference_in_pu(speed_ref);

            frequency_sensor.set_output(0.0);
            frequency_sensor.initialize();

            frequency_controller.set_output(0.0);
            frequency_controller.initialize();

            set_flag_model_initialized_as_true();

            osstream<<get_model_name()<<" model of "<<get_device_name()<<" is initialized."<<endl
                    <<"(1) speed regulator state: "<<speed_controller.get_state()<<endl
                    <<"(2) frequency regulator state: "<<frequency_controller.get_state()<<endl
                    <<"(3) pitch integrator state: "<<pitch_integrator.get_state()<<endl
                    <<"(4) pitch angle is "<<get_pitch_angle_in_deg()<<" deg";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void WT3P0::run(DYNAMIC_MODE mode)
{
    double speed = get_wt_generator_speed_in_pu();
    double speedref = 0.0;
    if(get_hold_wtg_speed_flag()==true)
        speedref = get_const_wtg_speed_reference_in_pu();
    else
        speedref = get_wt_generator_reference_speed_in_pu();

    speed_reference_sensor.set_input(speedref);
    speed_reference_sensor.run(mode);

    speedref = speed_reference_sensor.get_output();

    speed_controller.set_input(speed-speedref);
    speed_controller.run(mode);

    double freq = get_bus_frequency_deviation_in_pu();
    frequency_sensor.set_input(freq);
    frequency_sensor.run(mode);
    freq = frequency_sensor.get_output();

    double fup = get_frequency_upper_deadband_in_pu();
    double flow = get_frequency_lower_deadband_in_pu();
    if(freq>=flow and freq<=fup)
        freq = 0.0;
    else
    {
        if(freq>fup)
            freq -= fup;
        else
            freq -= flow;
    }

    frequency_controller.set_input(freq);
    frequency_controller.run(mode);

    double rate = speed_controller.get_output() + frequency_controller.get_output();
    rate -= pitch_integrator.get_output();

    double ratemax = get_ratePitchmax_in_deg_per_s();
    if(rate<=ratemax and rate>-ratemax)
        ;
    else
    {
        if(rate>ratemax)
            rate = ratemax;
        else
            rate = -ratemax;
    }

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
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void WT3P0::save()
{
    ;
}
string WT3P0::get_standard_model_string() const
{
    ostringstream osstream;

    bool flag = get_hold_wtg_speed_flag();
    int hold_speed_flag = 0;
    if(flag==true)
        hold_speed_flag = 1;

    double tspeed = get_Tspeed_in_s();
    double kps = get_Kp_speed_controller();
    double kis = get_Ki_speed_controller();
    double tf = get_Tfrequency_in_s();
    double fup = get_frequency_upper_deadband_in_pu();
    double flow = get_frequency_lower_deadband_in_pu();
    double kpf = get_Kp_frequency_controller();
    double kif = get_Ki_frequency_controller();
    double kdf = get_Kd_frequency_controller();
    double tdf = get_Td_frequency_controller_in_s();
    double rpmax = get_ratePitchmax_in_deg_per_s();
    double pmax = get_Pitchmax_in_deg();
    double pmin = get_Pitchmin_in_deg();
    double tp = get_Tp_in_s();

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<hold_speed_flag<<", "
      <<setw(8)<<setprecision(6)<<tspeed<<", "
      <<setw(8)<<setprecision(6)<<kps<<", "
      <<setw(8)<<setprecision(6)<<kis<<", "
      <<setw(8)<<setprecision(6)<<tf<<", "
      <<setw(8)<<setprecision(6)<<flow<<", "
      <<setw(8)<<setprecision(6)<<fup<<", "
      <<setw(8)<<setprecision(6)<<kpf<<", "
      <<setw(8)<<setprecision(6)<<kif<<", "
      <<setw(8)<<setprecision(6)<<kdf<<", "
      <<setw(8)<<setprecision(6)<<tdf<<", "
      <<setw(8)<<setprecision(6)<<rpmax<<", "
      <<setw(8)<<setprecision(6)<<pmin<<", "
      <<setw(8)<<setprecision(6)<<pmax<<", "
      <<setw(8)<<setprecision(6)<<tp<<" /";
    return osstream.str();
}

void WT3P0::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("HOLD SPEED FLAG", i); i++; /*1*/
    add_model_data_name_and_index_pair("T SPEED IN S", i); i++; /*2*/
    add_model_data_name_and_index_pair("KP SPEED", i); i++; /*3*/
    add_model_data_name_and_index_pair("KI SPEED", i); i++; /*4*/
    add_model_data_name_and_index_pair("T FREQUENCY IN S", i); i++; /*5*/
    add_model_data_name_and_index_pair("F LOWER IN PU", i); i++; /*6*/
    add_model_data_name_and_index_pair("F UPPER IN PU", i); i++; /*7*/
    add_model_data_name_and_index_pair("KP FREQUENCY", i); i++; /*8*/
    add_model_data_name_and_index_pair("KI FREQUENCY", i); i++; /*9*/
    add_model_data_name_and_index_pair("KD FREQUENCY", i); i++; /*10*/
    add_model_data_name_and_index_pair("TD FREQUENCY IN S", i); i++; /*11*/
    add_model_data_name_and_index_pair("MAX PITCH RATE IN DEG/S", i); i++; /*12*/
    add_model_data_name_and_index_pair("PITCH MIN IN DEG", i); i++; /*13*/
    add_model_data_name_and_index_pair("PITCH MAX IN DEG", i); i++; /*14*/
    add_model_data_name_and_index_pair("T PITCH IN S", i); i++; /*15*/
}

double WT3P0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name == "HOLD SPEED FLAG") return get_hold_wtg_speed_flag();
    if(par_name == "T SPEED IN S")    return get_Tspeed_in_s();
    if(par_name == "KP SPEED")        return get_Kp_speed_controller();
    if(par_name == "KI SPEED")        return get_Ki_speed_controller();
    if(par_name == "T FREQUENCY IN S") return get_Tfrequency_in_s();
    if(par_name == "F LOWER IN PU")    return get_frequency_lower_deadband_in_pu();
    if(par_name == "F UPPER IN PU")    return get_frequency_upper_deadband_in_pu();
    if(par_name == "KP FREQUENCY")     return get_Kp_frequency_controller();
    if(par_name == "KI FREQUENCY")     return get_Ki_frequency_controller();
    if(par_name == "KD FREQUENCY")     return get_Kd_frequency_controller();
    if(par_name == "TD FREQUENCY IN S") return get_Td_frequency_controller_in_s();
    if(par_name == "MAX PITCH RATE IN DEG/S") return get_ratePitchmax_in_deg_per_s();
    if(par_name == "PITCH MIN IN DEG")        return get_Pitchmin_in_deg();
    if(par_name == "PITCH MAX IN DEG")        return get_Pitchmax_in_deg();
    if(par_name == "T PITCH IN S")            return get_Tp_in_s();
    return 0.0;
}



void WT3P0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name == "HOLD SPEED FLAG")
    {
        bool flag = false;
        if (value != 0.0)
            flag = true;
        return set_hold_wtg_speed_flag(flag);
    }
    if(par_name == "T SPEED IN S") return set_Tspeed_in_s(value);
    if(par_name == "KP SPEED")     return set_Kp_speed_controller(value);
    if(par_name == "KI SPEED")     return set_Ki_speed_controller(value);
    if(par_name == "T FREQUENCY IN S") return set_Tfrequency_in_s(value);
    if(par_name == "F LOWER IN PU")    return set_frequency_lower_deadband_in_pu(value);
    if(par_name == "F UPPER IN PU")    return set_frequency_upper_deadband_in_pu(value);
    if(par_name == "KP FREQUENCY")     return set_Kp_frequency_controller(value);
    if(par_name == "KI FREQUENCY")     return set_Ki_frequency_controller(value);
    if(par_name == "KD FREQUENCY")     return set_Kd_frequency_controller(value);
    if(par_name == "TD FREQUENCY IN S") return set_Td_frequency_controller_in_s(value);
    if(par_name == "MAX PITCH RATE IN DEG/S") return set_ratePitchmax_in_deg_per_s(value);
    if(par_name == "PITCH MIN IN DEG")        return set_Pitchmin_in_deg(value);
    if(par_name == "PITCH MAX IN DEG")        return set_Pitchmax_in_deg(value);
    if(par_name == "T PITCH IN S")            return set_Tp_in_s(value);

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}


void WT3P0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED REFERENCE SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SPEED REGULATOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@FREQUENCY SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@FREQUENCY PID CONTROLLER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@PITCH INTEGRATOR", i); i++;
}

double WT3P0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@SPEED REFERENCE SENSOR")
        return speed_reference_sensor.get_state();
    if(var_name == "STATE@SPEED REGULATOR")
        return speed_controller.get_state();
    if(var_name == "STATE@FREQUENCY SENSOR")
        return frequency_sensor.get_state();
    if(var_name == "STATE@FREQUENCY PID CONTROLLER")
        return frequency_controller.get_state();
    if(var_name == "STATE@PITCH INTEGRATOR")
        return pitch_integrator.get_state();

    if(var_name == "STATE@SPEED CONTROLLER")
        return speed_controller.get_state();

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
