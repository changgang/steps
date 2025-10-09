#include "header/model/energy_storage_model/es_auxiliary_model/esau0.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ESAU0::ESAU0(STEPS& toolkit) : ES_AUXILIARY_MODEL(toolkit)
{
    clear();
}

ESAU0::~ESAU0()
{
    ;
}

void ESAU0::clear()
{
    set_model_float_parameter_count(70);

    speed_reach_target_flag = false;
    power_reach_target_flag = false;
    valve_reach_target_flag = false;

    deactivate_rotor_braking();
}

void ESAU0::copy_from_const_model(const ESAU0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

ESAU0::ESAU0(const ESAU0& model):ES_AUXILIARY_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESAU0& ESAU0::operator=(const ESAU0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESAU0::get_model_name() const
{
    return "ESAU0";
}

void ESAU0::set_start_charge_mode_power_threshold(double p)
{
    start_charge_mode_power_threshold = p;
}

double ESAU0::get_start_charge_mode_power_threshold() const
{
    return start_charge_mode_power_threshold;
}

void ESAU0::set_start_discharge_mode_power_threshold(double p)
{
    start_discharge_mode_power_threshold = p;
}

double ESAU0::get_start_discharge_mode_power_threshold() const
{
    return start_discharge_mode_power_threshold;
}

void ESAU0::set_power_slope_in_start_charge_mode(double k)
{
    power_slope_in_start_charge_mode = k;
}

double ESAU0::get_power_slope_in_start_charge_mode() const
{
    return power_slope_in_start_charge_mode;
}

void ESAU0::set_power_slope_in_stop_charge_mode(double k)
{
    power_slope_in_stop_charge_mode = k;
}

double ESAU0::get_power_slope_in_stop_charge_mode() const
{
    return power_slope_in_stop_charge_mode;
}

void ESAU0::set_power_slope_in_start_discharge_mode(double k)
{
    power_slope_in_start_discharge_mode = k;
}

double ESAU0::get_power_slope_in_start_discharge_mode() const
{
    return power_slope_in_start_discharge_mode;
}

void ESAU0::set_power_slope_in_stop_discharge_mode(double k)
{
    power_slope_in_stop_discharge_mode = k;
}

double ESAU0::get_power_slope_in_stop_discharge_mode() const
{
    return power_slope_in_stop_discharge_mode;
}

void ESAU0::set_speed_slope_in_start_charge_mode(double k)
{
    speed_slope_in_start_charge_mode = k;
}

double ESAU0::get_speed_slope_in_start_charge_mode() const
{
    return speed_slope_in_start_charge_mode;
}

void ESAU0::set_speed_slope_in_stop_charge_mode(double k)
{
    speed_slope_in_stop_charge_mode = k;
}

double ESAU0::get_speed_slope_in_stop_charge_mode() const
{
    return speed_slope_in_stop_charge_mode;
}

void ESAU0::set_speed_slope_in_start_discharge_mode(double k)
{
    speed_slope_in_start_discharge_mode = k;
}

double ESAU0::get_speed_slope_in_start_discharge_mode() const
{
    return speed_slope_in_start_discharge_mode;
}

void ESAU0::set_speed_slope_in_stop_discharge_mode(double k)
{
    speed_slope_in_stop_discharge_mode = k;
}

double ESAU0::get_speed_slope_in_stop_discharge_mode() const
{
    return speed_slope_in_stop_discharge_mode;
}

void ESAU0::set_valve_slope_in_start_charge_mode(double k)
{
    valve_slope_in_start_charge_mode = k;
}

double ESAU0::get_valve_slope_in_start_charge_mode() const
{
    return valve_slope_in_start_charge_mode;
}

void ESAU0::set_valve_slope_in_stop_charge_mode(double k)
{
    valve_slope_in_stop_charge_mode = k;
}

double ESAU0::get_valve_slope_in_stop_charge_mode() const
{
    return valve_slope_in_stop_charge_mode;
}

void ESAU0::set_valve_slope_in_start_discharge_mode(double k)
{
    valve_slope_in_start_discharge_mode = k;
}

double ESAU0::get_valve_slope_in_start_discharge_mode() const
{
    return valve_slope_in_start_discharge_mode;
}

void ESAU0::set_valve_slope_in_stop_discharge_mode(double k)
{
    valve_slope_in_stop_discharge_mode = k;
}

double ESAU0::get_valve_slope_in_stop_discharge_mode() const
{
    return valve_slope_in_stop_discharge_mode;
}

void ESAU0::add_power_speed_pair_in_charge(double power, double speed)
{
    power_speed_table_in_charge.add_x_and_y_pair(power, speed);
}

void ESAU0::add_power_speed_pair_in_discharge(double power, double speed)
{
    power_speed_table_in_discharge.add_x_and_y_pair(power, speed);
}

void ESAU0::add_power_valve_pair_in_charge(double power, double valve)
{
    power_valve_table_in_charge.add_x_and_y_pair(power,valve);
}

void ESAU0::add_power_valve_pair_in_discharge(double power, double valve)
{
    power_valve_table_in_discharge.add_x_and_y_pair(power, valve);
}

double ESAU0::get_target_power_reference_in_pu() const
{
    double p = 0.0;
    if(not is_manual_control())
    {
        ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
        ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
        ES_CONVERTER_MODEL* con_model = energy_storage->get_es_converter_model();
        if(elec_model!=NULL)
            p = elec_model->get_frequency_regulation_power_in_pu();
        else
            p = con_model->get_initial_active_power_command_in_pu_based_on_mbase();
    }
    else
    {
        p = get_manual_power_in_pu();
    }

    ES_MODE mode = get_energy_storage_mode();
    if((mode==ES_STOP_DISCHARGE_MODE and p<0) or (mode==ES_STOP_CHARGE_MODE and p>0))
        p = 0.0;
    return p;

}

double ESAU0::get_target_speed_reference_in_pu() const
{
    double p = get_target_power_reference_in_pu();
    double speed_ref;

    if(p>0.0)
    {
        speed_ref = power_speed_table_in_discharge.get_y_at_x(p);

        if(p<(get_start_discharge_mode_power_threshold()-FLOAT_EPSILON))
            speed_ref = power_speed_table_in_discharge.get_min_y();
    }
    else
    {
        if(p<0.0)
        {
            speed_ref = power_speed_table_in_charge.get_y_at_x(p);
            if(p>(get_start_charge_mode_power_threshold()+FLOAT_EPSILON))
                speed_ref = power_speed_table_in_charge.get_max_y();
        }
        else
            speed_ref = 0.0;
    }

    return speed_ref;

}

double ESAU0::get_target_valve_reference_in_pu() const
{
    double p = get_target_power_reference_in_pu();
    double yref;

    if(p>0.0)
    {
        yref = power_valve_table_in_discharge.get_y_at_x(p);
        if(p<(get_start_discharge_mode_power_threshold()-FLOAT_EPSILON))
            yref = power_valve_table_in_discharge.get_min_y();
    }
    else
    {
        if(p<0.0)
        {
            yref = power_valve_table_in_charge.get_y_at_x(p);
            if(p>(get_start_charge_mode_power_threshold()+FLOAT_EPSILON))
                yref = power_valve_table_in_charge.get_max_y();
        }
        else
            yref = 0.0;
    }

    return yref;
}

void ESAU0::set_energy_storage_in_out_service_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    freeze_primary_model();
    initialize_primary_model_as_out_service_mode();

    freeze_electrical_active_power_control();
    initialize_electrical_model_as_out_service_mode();

    lock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_OUT_SERVICE_MODE);
}

void ESAU0::set_energy_storage_in_start_discharge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    double p0 = get_current_Pref();
    double speed0 = get_current_speedref();
    double yref0 = get_current_Yref();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    set_power_reference_when_into_start_discharge_mode(p0);
    set_speed_reference_when_into_start_discharge_mode(speed0);
    set_valve_reference_when_into_start_discharge_mode(yref0);

    set_time_when_into_start_discharge_mode(time);

    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    unfreeze_primary_model();
    unfreeze_electrical_active_power_control();

    unlock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_START_DISCHARGE_MODE);
}

void ESAU0::set_energy_storage_in_stop_discharge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    double p0 = get_current_Pref();
    double speed0 = get_current_speedref();
    double yref0 = get_current_Yref();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    set_power_reference_when_into_stop_discharge_mode(p0);
    set_speed_reference_when_into_stop_discharge_mode(speed0);
    set_valve_reference_when_into_stop_discharge_mode(yref0);
    set_time_when_into_stop_discharge_mode(time);

    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    unfreeze_primary_model();
    unfreeze_electrical_active_power_control();

    unlock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_STOP_DISCHARGE_MODE);

}

void ESAU0::set_energy_storage_in_discharge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    unfreeze_primary_model();
    unfreeze_electrical_active_power_control();

    unlock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_DISCHARGE_MODE);
}

void ESAU0::set_energy_storage_in_start_charge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    double p0 = get_current_Pref();
    double speed0 = get_current_speedref();
    double yref0 = get_current_Yref();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    set_power_reference_when_into_start_charge_mode(p0);
    set_speed_reference_when_into_start_charge_mode(speed0);
    set_valve_reference_when_into_start_charge_mode(yref0);
    set_time_when_into_start_charge_mode(time);

    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    unfreeze_primary_model();
    unfreeze_electrical_active_power_control();

    unlock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_START_CHARGE_MODE);
}

void ESAU0::set_energy_storage_in_stop_charge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    double p0 = get_current_Pref();
    double speed0 = get_current_speedref();
    double yref0 = get_current_Yref();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();

    set_power_reference_when_into_stop_charge_mode(p0);
    set_speed_reference_when_into_stop_charge_mode(speed0);
    set_valve_reference_when_into_stop_charge_mode(yref0);
    set_time_when_into_stop_charge_mode(time);

    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    unfreeze_primary_model();
    unfreeze_electrical_active_power_control();

    unlock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_STOP_CHARGE_MODE);

}

void ESAU0::set_energy_storage_in_charge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    power_reach_target_flag = false;
    speed_reach_target_flag = false;
    valve_reach_target_flag = false;

    unfreeze_primary_model();
    unfreeze_electrical_active_power_control();

    unlock_rotor();
    deactivate_rotor_braking();
    energy_storage->set_energy_storage_mode(ES_CHARGE_MODE);

}

void ESAU0::check_energy_storage_mode()
{
    ES_MODE mode = get_energy_storage_mode();
    switch(mode)
    {
        case ES_OUT_SERVICE_MODE:
        {
            check_when_energy_storage_in_out_service_mode();
            break;
        }
        case ES_START_DISCHARGE_MODE:
        {
            check_when_energy_storage_in_start_discharge_mode();
            break;
        }
        case ES_STOP_DISCHARGE_MODE:
        {
            check_when_energy_storage_in_stop_discharge_mode();
            break;
        }
        case ES_DISCHARGE_MODE:
        {
            check_when_energy_storage_in_discharge_mode();
            break;
        }
        case ES_START_CHARGE_MODE:
        {
            check_when_energy_storage_in_start_charge_mode();
            break;
        }
        case ES_STOP_CHARGE_MODE:
        {
            check_when_energy_storage_in_stop_charge_mode();
            break;
        }
        case ES_CHARGE_MODE:
        {
            check_when_energy_storage_in_charge_mode();
            break;
        }
        default:
        {
            break;
        }
    }

}

void ESAU0::check_when_energy_storage_in_out_service_mode()
{
    double Ptarget = get_target_power_reference_in_pu();
    double Pd = get_start_discharge_mode_power_threshold();
    double Pc = get_start_charge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget>Pd)
        {
            set_energy_storage_in_start_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_OUT_SERVICE_MODE to ES_START_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(>"<<Pd<<")."<<endl
                    <<"The rotor is unlocked at time "<<time<<" s."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            if(Ptarget<Pc)
            {
                set_energy_storage_in_start_charge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_OUT_SERVICE_MODE to ES_START_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(<"<<Pc<<")."<<endl
                        <<"The rotor is unlocked at time "<<time<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }

    }
    else//manual control
    {
        if(Ptarget>0.0)
        {
            set_energy_storage_in_start_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_OUT_SERVICE_MODE to ES_START_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually."<<endl
                    <<"The rotor is unlocked at time "<<time<<" s."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        else
        {
            if(Ptarget<0.0)
            {
                set_energy_storage_in_start_charge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_OUT_SERVICE_MODE to ES_START_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually."<<endl
                        <<"The rotor is unlocked at time "<<time<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

            }
        }
    }

}

void ESAU0::check_when_energy_storage_in_start_discharge_mode()
{
    double Ptarget = get_target_power_reference_in_pu();
    double Pd = get_start_discharge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget<=Pd)
        {
            set_energy_storage_in_stop_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_START_DISCHARGE_MODE to ES_STOP_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(<"<<Pd<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            if(power_reach_target_flag==true and speed_reach_target_flag==true and valve_reach_target_flag==true)
            {
                set_energy_storage_in_discharge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_START_DISCHARGE_MODE to ES_DISCHARGE_MODE at time "<<time<<" s since both Pref, wref and Yref reach target.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
    else//manual control
    {
        if(Ptarget<get_current_Pref())
        {
            set_energy_storage_in_stop_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_START_DISCHARGE_MODE to ES_STOP_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        else
        {
            if(power_reach_target_flag==true and speed_reach_target_flag==true and valve_reach_target_flag==true)
            {
                set_energy_storage_in_discharge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_START_DISCHARGE_MODE to ES_DISCHARGE_MODE at time "<<time<<" s since both Pref, wref and Yref reach target manually.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

        }
    }
}

void ESAU0::check_when_energy_storage_in_stop_discharge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    double Ptarget = get_target_power_reference_in_pu();
    double Pd = get_start_discharge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget>Pd)
        {
            set_energy_storage_in_start_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_STOP_DISCHARGE_MODE to ES_START_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(>"<<Pd<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            ES_PRIMARY_MODEL* primary_model = energy_storage->get_es_primary_model();
            ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
            double Pmech = primary_model->get_mechanical_power_in_pu_on_mbase();
            double Pcmd = elec_model->get_active_power_command_in_pu_based_on_mbase();
            double speed = get_rotor_speed_in_pu();

            if(speed<=MINIMUM_ROTOR_SPEED_THRESHOLD and not is_rotor_locked())
            {
                lock_rotor();
                osstream<<"The rotor of "<<get_compound_device_name()<<" is locked at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pmech<=FLOAT_EPSILON or is_rotor_locked()) and not is_primary_model_frozen())
            {
                freeze_primary_model();
                initialize_primary_model_as_out_service_mode();
                osstream<<"The primary model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pcmd<=FLOAT_EPSILON or is_rotor_locked()) and not is_electrical_active_power_control_frozen())
            {
                freeze_electrical_active_power_control();
                initialize_electrical_model_as_out_service_mode();
                osstream<<"The active power control in Electrical model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(is_primary_model_frozen() and is_electrical_active_power_control_frozen())
            {

                if(not is_rotor_locked())
                {
                    if(not is_rotor_braking_activated())
                    {
                        activate_rotor_braking();
                        osstream<<"The rotor braking of "<<get_compound_device_name()<<" is activated at time "<<time<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    set_energy_storage_in_out_service_mode();
                    osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                            <<"ES_STOP_DISCHARGE_MODE to ES_OUT_SERVICE_MODE at time "<<time<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }
    else//manual control
    {
        if(Ptarget<=0.0)
        {
            ES_PRIMARY_MODEL* primary_model = energy_storage->get_es_primary_model();
            ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
            double Pmech = primary_model->get_mechanical_power_in_pu_on_mbase();
            double Pcmd = elec_model->get_active_power_command_in_pu_based_on_mbase();
            double speed = get_rotor_speed_in_pu();

            if(speed<=MINIMUM_ROTOR_SPEED_THRESHOLD and not is_rotor_locked())
            {
                lock_rotor();
                osstream<<"The rotor of "<<get_compound_device_name()<<" is locked at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pmech<=FLOAT_EPSILON or is_rotor_locked()) and not is_primary_model_frozen())
            {
                freeze_primary_model();
                initialize_primary_model_as_out_service_mode();
                osstream<<"The primary model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pcmd<=FLOAT_EPSILON or is_rotor_locked()) and not is_electrical_active_power_control_frozen())
            {
                freeze_electrical_active_power_control();
                initialize_electrical_model_as_out_service_mode();
                osstream<<"The active power control in electrical model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(is_primary_model_frozen() and is_electrical_active_power_control_frozen())
            {
                if(not is_rotor_locked())
                {
                    if(not is_rotor_braking_activated())
                    {
                        activate_rotor_braking();
                        osstream<<"The rotor braking of "<<get_compound_device_name()<<" is activated at time "<<time<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    set_energy_storage_in_out_service_mode();
                    osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                            <<"ES_STOP_DISCHARGE_MODE to ES_OUT_SERVICE_MODE at time "<<time<<" s manually.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
        else
        {
            if(Ptarget>get_current_Pref())
            {
                set_energy_storage_in_start_discharge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_STOP_DISCHARGE_MODE to ES_STAT_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                if(power_reach_target_flag==true and speed_reach_target_flag==true and valve_reach_target_flag==true)
                {
                    set_energy_storage_in_discharge_mode();
                    osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                            <<"ES_STOP_DISCHARGE_MODE to ES_DISCHARGE_MODE at time "<<time<<" s since both Pref, wref and Yref reach target manually.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                }
            }

        }
    }

}

void ESAU0::check_when_energy_storage_in_discharge_mode()
{
    double Ptarget = get_target_power_reference_in_pu();
    double Pd = get_start_discharge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget<=Pd)
        {
            set_energy_storage_in_stop_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_DISCHARGE_MODE to ES_STOP_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(<"<<Pd<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else//manual
    {
        if(Ptarget>get_current_Pref())
        {
            set_energy_storage_in_start_discharge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_STOP_DISCHARGE_MODE to ES_STAT_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        else
        {
            if(Ptarget<get_current_Pref())
            {
                set_energy_storage_in_stop_discharge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_DISCHARGE_MODE to ES_STOP_DISCHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
                toolkit.show_information_with_leading_time_stamp(osstream);

            }
        }
    }

}

void ESAU0::check_when_energy_storage_in_start_charge_mode()
{
    double Ptarget = get_target_power_reference_in_pu();
    double Pc = get_start_charge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget>Pc)
        {
            set_energy_storage_in_stop_charge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_START_CHARGE_MODE to ES_STOP_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(>"<<Pc<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            if(power_reach_target_flag==true and speed_reach_target_flag==true and valve_reach_target_flag==true)
            {
                set_energy_storage_in_charge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_START_CHARGE_MODE to ES_CHARGE_MODE at time "<<time<<" s since both Pref, wref and Yref reach target.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
    else //manually
    {

        if(Ptarget>get_current_Pref())
        {
            set_energy_storage_in_stop_charge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_START_CHARGE_MODE to ES_STOP_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        else
        {
            if(power_reach_target_flag==true and speed_reach_target_flag==true and valve_reach_target_flag==true)
            {
                set_energy_storage_in_charge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_START_CHARGE_MODE to ES_CHARGE_MODE at time "<<time<<" s since both Pref, wref and Yref reach target manually.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

        }
    }

}

void ESAU0::check_when_energy_storage_in_stop_charge_mode()
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    double Ptarget = get_target_power_reference_in_pu();
    double Pc = get_start_charge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget<Pc)
        {
            set_energy_storage_in_start_charge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_STOP_CHARGE_MODE to ES_START_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(<"<<Pc<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            ES_PRIMARY_MODEL* primary_model = energy_storage->get_es_primary_model();
            ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
            double Pmech = primary_model->get_mechanical_power_in_pu_on_mbase();
            double Pcmd = elec_model->get_active_power_command_in_pu_based_on_mbase();
            double speed = get_rotor_speed_in_pu();

            if(speed>=-MINIMUM_ROTOR_SPEED_THRESHOLD and not is_rotor_locked())
            {
                lock_rotor();
                osstream<<"The rotor of "<<get_compound_device_name()<<" is locked at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pmech>=-FLOAT_EPSILON or is_rotor_locked()) and not is_primary_model_frozen())
            {
                freeze_primary_model();
                initialize_primary_model_as_out_service_mode();
                osstream<<"The primary model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }


            if((Pcmd>=-FLOAT_EPSILON or is_rotor_locked()) and not is_electrical_active_power_control_frozen())
            {
                freeze_electrical_active_power_control();
                initialize_electrical_model_as_out_service_mode();
                osstream<<"The active power control in electrical model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(is_primary_model_frozen() and is_electrical_active_power_control_frozen())
            {
                if(not is_rotor_locked())
                {
                    if(not is_rotor_braking_activated())
                    {
                        activate_rotor_braking();
                        osstream<<"The rotor braking of "<<get_compound_device_name()<<" is activated at time "<<time<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    set_energy_storage_in_out_service_mode();
                    osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                            <<"ES_STOP_CHARGE_MODE to ES_OUT_SERVICE_MODE at time "<<time<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

            }
        }
    }
    else//manual control
    {
        if(Ptarget>=0.0)
        {
            ES_PRIMARY_MODEL* primary_model = energy_storage->get_es_primary_model();
            ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
            double Pmech = primary_model->get_mechanical_power_in_pu_on_mbase();
            double Pcmd = elec_model->get_active_power_command_in_pu_based_on_mbase();
            double speed = get_rotor_speed_in_pu();

            if(speed>=-MINIMUM_ROTOR_SPEED_THRESHOLD and not is_rotor_locked())
            {
                lock_rotor();
                osstream<<"The rotor of "<<get_compound_device_name()<<" is locked at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pmech>=-FLOAT_EPSILON or is_rotor_locked()) and not is_primary_model_frozen())
            {
                freeze_primary_model();
                initialize_primary_model_as_out_service_mode();
                osstream<<"The primary model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if((Pcmd>=-FLOAT_EPSILON or is_rotor_locked()) and not is_electrical_active_power_control_frozen())
            {
                freeze_electrical_active_power_control();
                initialize_electrical_model_as_out_service_mode();
                osstream<<"The active power control in electrical model of "<<get_compound_device_name()<<" is frozen at time "<<time<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(is_primary_model_frozen() and is_electrical_active_power_control_frozen())
            {
                if(not is_rotor_locked())
                {
                    if(not is_rotor_braking_activated())
                    {
                        activate_rotor_braking();
                        osstream<<"The rotor braking of "<<get_compound_device_name()<<" is activated at time "<<time<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    set_energy_storage_in_out_service_mode();
                    osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                            <<"ES_STOP_CHARGE_MODE to ES_OUT_SERVICE_MODE at time "<<time<<" s manually.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }

        }
        else
        {
            if(Ptarget<get_current_Pref())
            {
                set_energy_storage_in_start_charge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_STOP_CHARGE_MODE to ES_STAT_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                if(speed_reach_target_flag==true and power_reach_target_flag==true and valve_reach_target_flag==true)
                {
                    set_energy_storage_in_charge_mode();
                    osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_STOP_CHARGE_MODE to ES_CHARGE_MODE at time "<<time<<" s since both Pref, wref and Yref reach target manually.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }

}

void ESAU0::check_when_energy_storage_in_charge_mode()
{
    double Ptarget = get_target_power_reference_in_pu();
    double Pc = get_start_charge_mode_power_threshold();
    ostringstream osstream;
    STEPS & toolkit = get_toolkit();
    double time = toolkit.get_dynamic_simulation_time_in_s();
    if(not is_manual_control())
    {
        if(Ptarget>=Pc)
        {
            set_energy_storage_in_stop_charge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_CHARGE_MODE to ES_STOP_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<"(>"<<Pc<<").";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else//manually
    {
        if(Ptarget<get_current_Pref())
        {
            set_energy_storage_in_start_charge_mode();
            osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                    <<"ES_STOP_CHARGE_MODE to ES_STAT_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
            toolkit.show_information_with_leading_time_stamp(osstream);

        }
        else
        {
            if(Ptarget>get_current_Pref())
            {
                set_energy_storage_in_stop_charge_mode();
                osstream<<"MODE of "<<get_compound_device_name()<<" is changed from "
                        <<"ES_CHARGE_MODE to ES_STOP_CHARGE_MODE at time "<<time<<" s since Ptarget is "<<Ptarget<<" manually.";
                toolkit.show_information_with_leading_time_stamp(osstream);

            }
        }
    }

}

bool ESAU0::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=73)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            unsigned int mode;

            double p_d, p_c;

            double pcmd_d_1, speed_d_1, valve_d_1, pcmd_d_2, speed_d_2, valve_d_2, pcmd_d_3, speed_d_3, valve_d_3;
            double pcmd_d_4, speed_d_4, valve_d_4, pcmd_d_5, speed_d_5, valve_d_5, pcmd_d_6, speed_d_6, valve_d_6;
            double pcmd_d_7, speed_d_7, valve_d_7, pcmd_d_8, speed_d_8, valve_d_8, pcmd_d_9, speed_d_9, valve_d_9;

            double pcmd_c_1, speed_c_1, valve_c_1, pcmd_c_2, speed_c_2, valve_c_2, pcmd_c_3, speed_c_3, valve_c_3;
            double pcmd_c_4, speed_c_4, valve_c_4, pcmd_c_5, speed_c_5, valve_c_5, pcmd_c_6, speed_c_6, valve_c_6;
            double pcmd_c_7, speed_c_7, valve_c_7, pcmd_c_8, speed_c_8, valve_c_8, pcmd_c_9, speed_c_9, valve_c_9;

            double kp_start_c, kp_stop_c, kp_start_d, kp_stop_d;
            double ks_start_c, ks_stop_c, ks_start_d, ks_stop_d;
            double kv_start_c, kv_stop_c, kv_start_d, kv_stop_d;

            double D;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            mode = (unsigned int)(get_integer_data(data[i],"0"));i++;

            p_d = get_double_data(data[i],"0.0"); i++;
            p_c = get_double_data(data[i],"0.0"); i++;
            pcmd_d_1 = get_double_data(data[i],"0.0"); i++;
            speed_d_1 = get_double_data(data[i],"0.0"); i++;
            valve_d_1 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_2 = get_double_data(data[i],"0.0"); i++;
            speed_d_2 = get_double_data(data[i],"0.0"); i++;
            valve_d_2 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_3 = get_double_data(data[i],"0.0"); i++;
            speed_d_3 = get_double_data(data[i],"0.0"); i++;
            valve_d_3 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_4 = get_double_data(data[i],"0.0"); i++;
            speed_d_4 = get_double_data(data[i],"0.0"); i++;
            valve_d_4 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_5 = get_double_data(data[i],"0.0"); i++;
            speed_d_5 = get_double_data(data[i],"0.0"); i++;
            valve_d_5 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_6 = get_double_data(data[i],"0.0"); i++;
            speed_d_6 = get_double_data(data[i],"0.0"); i++;
            valve_d_6 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_7 = get_double_data(data[i],"0.0"); i++;
            speed_d_7 = get_double_data(data[i],"0.0"); i++;
            valve_d_7 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_8 = get_double_data(data[i],"0.0"); i++;
            speed_d_8 = get_double_data(data[i],"0.0"); i++;
            valve_d_8 = get_double_data(data[i],"0.0"); i++;
            pcmd_d_9 = get_double_data(data[i],"0.0"); i++;
            speed_d_9 = get_double_data(data[i],"0.0"); i++;
            valve_d_9 = get_double_data(data[i],"0.0"); i++;

            pcmd_c_1 = get_double_data(data[i],"0.0"); i++;
            speed_c_1 = get_double_data(data[i],"0.0"); i++;
            valve_c_1 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_2 = get_double_data(data[i],"0.0"); i++;
            speed_c_2 = get_double_data(data[i],"0.0"); i++;
            valve_c_2 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_3 = get_double_data(data[i],"0.0"); i++;
            speed_c_3 = get_double_data(data[i],"0.0"); i++;
            valve_c_3 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_4 = get_double_data(data[i],"0.0"); i++;
            speed_c_4 = get_double_data(data[i],"0.0"); i++;
            valve_c_4 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_5 = get_double_data(data[i],"0.0"); i++;
            speed_c_5 = get_double_data(data[i],"0.0"); i++;
            valve_c_5 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_6 = get_double_data(data[i],"0.0"); i++;
            speed_c_6 = get_double_data(data[i],"0.0"); i++;
            valve_c_6 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_7 = get_double_data(data[i],"0.0"); i++;
            speed_c_7 = get_double_data(data[i],"0.0"); i++;
            valve_c_7 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_8 = get_double_data(data[i],"0.0"); i++;
            speed_c_8 = get_double_data(data[i],"0.0"); i++;
            valve_c_8 = get_double_data(data[i],"0.0"); i++;
            pcmd_c_9 = get_double_data(data[i],"0.0"); i++;
            speed_c_9 = get_double_data(data[i],"0.0"); i++;
            valve_c_9 = get_double_data(data[i],"0.0"); i++;

            kp_start_c = get_double_data(data[i],"0.0"); i++;
            kp_stop_c = get_double_data(data[i],"0.0"); i++;
            kp_start_d = get_double_data(data[i],"0.0"); i++;
            kp_stop_d = get_double_data(data[i],"0.0"); i++;

            ks_start_c = get_double_data(data[i],"0.0"); i++;
            ks_stop_c = get_double_data(data[i],"0.0"); i++;
            ks_start_d = get_double_data(data[i],"0.0"); i++;
            ks_stop_d = get_double_data(data[i],"0.0"); i++;

            kv_start_c = get_double_data(data[i],"0.0"); i++;
            kv_stop_c = get_double_data(data[i],"0.0"); i++;
            kv_start_d = get_double_data(data[i],"0.0"); i++;
            kv_stop_d = get_double_data(data[i],"0.0"); i++;

            D = get_double_data(data[i],"0.0"); i++;

            DEVICE_ID did = get_energy_storage_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            ENERGY_STORAGE* es = psdb.get_energy_storage(did);
            if(es!=NULL)
            {
                if(mode==0)
                    deactivate_manual_control();
                else
                    activate_manual_control();

                set_start_discharge_mode_power_threshold(p_d);
                set_start_charge_mode_power_threshold(p_c);

                add_power_speed_pair_in_discharge(pcmd_d_1, speed_d_1);
                add_power_valve_pair_in_discharge(pcmd_d_1, valve_d_1);
                add_power_speed_pair_in_discharge(pcmd_d_2, speed_d_2);
                add_power_valve_pair_in_discharge(pcmd_d_2, valve_d_2);
                add_power_speed_pair_in_discharge(pcmd_d_3, speed_d_3);
                add_power_valve_pair_in_discharge(pcmd_d_3, valve_d_3);
                add_power_speed_pair_in_discharge(pcmd_d_4, speed_d_4);
                add_power_valve_pair_in_discharge(pcmd_d_4, valve_d_4);
                add_power_speed_pair_in_discharge(pcmd_d_5, speed_d_5);
                add_power_valve_pair_in_discharge(pcmd_d_5, valve_d_5);
                add_power_speed_pair_in_discharge(pcmd_d_6, speed_d_6);
                add_power_valve_pair_in_discharge(pcmd_d_6, valve_d_6);
                add_power_speed_pair_in_discharge(pcmd_d_7, speed_d_7);
                add_power_valve_pair_in_discharge(pcmd_d_7, valve_d_7);
                add_power_speed_pair_in_discharge(pcmd_d_8, speed_d_8);
                add_power_valve_pair_in_discharge(pcmd_d_8, valve_d_8);
                add_power_speed_pair_in_discharge(pcmd_d_9, speed_d_9);
                add_power_valve_pair_in_discharge(pcmd_d_9, valve_d_9);

                add_power_speed_pair_in_charge(pcmd_c_1, speed_c_1);
                add_power_valve_pair_in_charge(pcmd_c_1, valve_c_1);
                add_power_speed_pair_in_charge(pcmd_c_2, speed_c_2);
                add_power_valve_pair_in_charge(pcmd_c_2, valve_c_2);
                add_power_speed_pair_in_charge(pcmd_c_3, speed_c_3);
                add_power_valve_pair_in_charge(pcmd_c_3, valve_c_3);
                add_power_speed_pair_in_charge(pcmd_c_4, speed_c_4);
                add_power_valve_pair_in_charge(pcmd_c_4, valve_c_4);
                add_power_speed_pair_in_charge(pcmd_c_5, speed_c_5);
                add_power_valve_pair_in_charge(pcmd_c_5, valve_c_5);
                add_power_speed_pair_in_charge(pcmd_c_6, speed_c_6);
                add_power_valve_pair_in_charge(pcmd_c_6, valve_c_6);
                add_power_speed_pair_in_charge(pcmd_c_7, speed_c_7);
                add_power_valve_pair_in_charge(pcmd_c_7, valve_c_7);
                add_power_speed_pair_in_charge(pcmd_c_8, speed_c_8);
                add_power_valve_pair_in_charge(pcmd_c_8, valve_c_8);
                add_power_speed_pair_in_charge(pcmd_c_9, speed_c_9);
                add_power_valve_pair_in_charge(pcmd_c_9, valve_c_9);

                set_power_slope_in_start_charge_mode(kp_start_c);
                set_power_slope_in_stop_charge_mode(kp_stop_c);
                set_power_slope_in_start_discharge_mode(kp_start_d);
                set_power_slope_in_stop_discharge_mode(kp_stop_d);

                set_speed_slope_in_start_charge_mode(ks_start_c);
                set_speed_slope_in_stop_charge_mode(ks_stop_c);
                set_speed_slope_in_start_discharge_mode(ks_start_d);
                set_speed_slope_in_stop_discharge_mode(ks_stop_d);

                set_valve_slope_in_start_charge_mode(kv_start_c);
                set_valve_slope_in_stop_charge_mode(kv_stop_c);
                set_valve_slope_in_start_discharge_mode(kv_start_d);
                set_valve_slope_in_stop_discharge_mode(kv_stop_d);

                set_rotor_braking_Damp(D);

                is_successful = true;

                return is_successful;
            }
            else
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such wt generator exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
        }
        else
        {
            return is_successful;
        }
    }
    else
    {
        return is_successful;
    }
}

bool ESAU0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESAU0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESAU0::setup_block_toolkit_and_parameters()
{

}

void ESAU0::initialize()
{
    if(not is_model_initialized())
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;

        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
        {
            freeze_electrical_active_power_control();
            freeze_primary_model();
            lock_rotor();

        }
        else
        {
            unfreeze_electrical_active_power_control();
            unfreeze_primary_model();
            unlock_rotor();
        }

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESAU0::check_limits_in_initialization()
{
    return;
}

void ESAU0::run(DYNAMIC_MODE mode)
{
    if(mode==DYNAMIC_UPDATE_MODE)
    {
        check_energy_storage_mode();

        double Pref = get_active_power_reference_in_pu();
        double speed = get_rotor_speed_reference_in_pu();
        double Y = get_valve_reference_in_pu();
        set_current_Pref(Pref);
        set_current_speedref(speed);
        set_current_Yref(Y);

        set_flag_model_updated_as_true();
    }
}

double ESAU0::get_active_power_reference_in_pu()
{
    STEPS& toolkit = get_toolkit();
    ES_MODE mode = get_energy_storage_mode();
    double order = 0.0, order0 = 0.0, target = 0.0;
    double time = 0.0, t0 = 0.0;
    switch(mode)
    {
        case ES_OUT_SERVICE_MODE:
        {
            return 0.0;
        }
        case ES_START_DISCHARGE_MODE:
        {
            order0 = get_power_reference_when_into_start_discharge_mode();
            t0 = get_time_when_into_start_discharge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 + get_power_slope_in_start_discharge_mode()*(time-t0);
            target = get_target_power_reference_in_pu();

            if(order>target)
            {
                power_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_STOP_DISCHARGE_MODE:
        {
            order0 = get_power_reference_when_into_stop_discharge_mode();
            t0 = get_time_when_into_stop_discharge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 - get_power_slope_in_stop_discharge_mode()*(time-t0);
            target = get_target_power_reference_in_pu();

            if(order<target)
            {
                power_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_START_CHARGE_MODE:
        {
            order0 = get_power_reference_when_into_start_charge_mode();
            t0 = get_time_when_into_start_charge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 - get_power_slope_in_start_charge_mode()*(time-t0);
            target = get_target_power_reference_in_pu();

            if(order<target)
            {
                power_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_STOP_CHARGE_MODE:
        {
            order0 = get_power_reference_when_into_stop_charge_mode();
            t0 = get_time_when_into_stop_charge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 + get_power_slope_in_stop_charge_mode()*(time-t0);
            target = get_target_power_reference_in_pu();

            if(order>target)
            {
                power_reach_target_flag=true;
                order = target;
            }
            return order;
        }
        case ES_DISCHARGE_MODE:
        {
            return get_target_power_reference_in_pu();
        }
        case ES_CHARGE_MODE:
        {
            return get_target_power_reference_in_pu();
        }
        default:
            return 0.0;
    }
}

double ESAU0::get_rotor_speed_reference_in_pu()
{
    STEPS& toolkit = get_toolkit();
    ES_MODE mode = get_energy_storage_mode();
    double order = 0.0, order0 = 0.0, target = 0.0;
    double time = 0.0, t0 = 0.0;
    switch(mode)
    {
        case ES_OUT_SERVICE_MODE:
        {
            return 0.0;
        }
        case ES_START_DISCHARGE_MODE:
        {
            order0 = get_speed_reference_when_into_start_discharge_mode();
            t0 = get_time_when_into_start_discharge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 + get_speed_slope_in_start_discharge_mode()*(time-t0);
            target = get_target_speed_reference_in_pu();

            if(order>target)
            {
                speed_reach_target_flag = true;
                order = target;

            }
            return order;
        }
        case ES_STOP_DISCHARGE_MODE:
        {
            order0 = get_speed_reference_when_into_stop_discharge_mode();
            t0 = get_time_when_into_stop_discharge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 - get_speed_slope_in_stop_discharge_mode()*(time-t0);
            target = get_target_speed_reference_in_pu();

            if(order<target)
            {
                speed_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_START_CHARGE_MODE:
        {
            order0 = get_speed_reference_when_into_start_charge_mode();
            t0 = get_time_when_into_start_charge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 - get_speed_slope_in_start_charge_mode()*(time-t0);
            target = get_target_speed_reference_in_pu();

            if(order<target)
            {
                speed_reach_target_flag = true;
                order = target;
            }
            return order;
        }
        case ES_STOP_CHARGE_MODE:
        {
            order0 = get_speed_reference_when_into_stop_charge_mode();
            t0 = get_time_when_into_stop_charge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 + get_speed_slope_in_stop_charge_mode()*(time-t0);
            target = get_target_speed_reference_in_pu();

            if(order>target)
            {
                speed_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_DISCHARGE_MODE:
        {
            return get_target_speed_reference_in_pu();
        }
        case ES_CHARGE_MODE:
        {
            return get_target_speed_reference_in_pu();
        }
        default:
            return 0.0;
    }
}

double ESAU0::get_valve_reference_in_pu()
{

    STEPS& toolkit = get_toolkit();
    ES_MODE mode = get_energy_storage_mode();
    double order = 0.0, order0 = 0.0, target = 0.0;
    double time = 0.0, t0 = 0.0;
    switch(mode)
    {
        case ES_OUT_SERVICE_MODE:
        {
            return 0.0;
        }
        case ES_START_DISCHARGE_MODE:
        {
            order0 = get_valve_reference_when_into_start_discharge_mode();
            t0 = get_time_when_into_start_discharge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 + get_valve_slope_in_start_discharge_mode()*(time-t0);
            target = get_target_valve_reference_in_pu();

            if(order>target)
            {
                valve_reach_target_flag = true;
                order = target;
            }
            return order;
        }
        case ES_STOP_DISCHARGE_MODE:
        {
            order0 = get_valve_reference_when_into_stop_discharge_mode();
            t0 = get_time_when_into_stop_discharge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 - get_valve_slope_in_stop_discharge_mode()*(time-t0);
            target = get_target_valve_reference_in_pu();

            if(order<target)
            {
                valve_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_START_CHARGE_MODE:
        {
            order0 = get_valve_reference_when_into_start_charge_mode();
            t0 = get_time_when_into_start_charge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 - get_valve_slope_in_start_charge_mode()*(time-t0);
            target = get_target_valve_reference_in_pu();

            if(order<target)
            {
                valve_reach_target_flag = true;
                order = target;
            }
            return order;
        }
        case ES_STOP_CHARGE_MODE:
        {
            order0 = get_valve_reference_when_into_stop_charge_mode();
            t0 = get_time_when_into_stop_charge_mode();
            time = toolkit.get_dynamic_simulation_time_in_s();
            order = order0 + get_valve_slope_in_stop_charge_mode()*(time-t0);
            target = get_target_valve_reference_in_pu();

            if(order>target)
            {
                valve_reach_target_flag = true;
                order = target;
            }

            return order;
        }
        case ES_DISCHARGE_MODE:
        {
            return get_target_valve_reference_in_pu();
        }
        case ES_CHARGE_MODE:
        {
            return get_target_valve_reference_in_pu();
        }
        default:
            return 0.0;
    }

}

void ESAU0::check()
{
    ;
}

void ESAU0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESAU0::save()
{
    ;
}

string ESAU0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";


    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    int mode = (is_manual_control()==true)? 1 : 0;
    double p_d = get_start_discharge_mode_power_threshold();
    double p_c = get_start_charge_mode_power_threshold();

    double pcmd_d_1 = power_speed_table_in_discharge.get_point_x_with_index(0);
    double speed_d_1 = power_speed_table_in_discharge.get_point_y_with_index(0);
    double valve_d_1 = power_valve_table_in_discharge.get_point_y_with_index(0);
    double pcmd_d_2 = power_speed_table_in_discharge.get_point_x_with_index(1);
    double speed_d_2 = power_speed_table_in_discharge.get_point_y_with_index(1);
    double valve_d_2 = power_valve_table_in_discharge.get_point_y_with_index(1);
    double pcmd_d_3 = power_speed_table_in_discharge.get_point_x_with_index(2);
    double speed_d_3 = power_speed_table_in_discharge.get_point_y_with_index(2);
    double valve_d_3 = power_valve_table_in_discharge.get_point_y_with_index(2);
    double pcmd_d_4 = power_speed_table_in_discharge.get_point_x_with_index(3);
    double speed_d_4 = power_speed_table_in_discharge.get_point_y_with_index(3);
    double valve_d_4 = power_valve_table_in_discharge.get_point_y_with_index(3);
    double pcmd_d_5 = power_speed_table_in_discharge.get_point_x_with_index(4);
    double speed_d_5 = power_speed_table_in_discharge.get_point_y_with_index(4);
    double valve_d_5 = power_valve_table_in_discharge.get_point_y_with_index(4);
    double pcmd_d_6 = power_speed_table_in_discharge.get_point_x_with_index(5);
    double speed_d_6 = power_speed_table_in_discharge.get_point_y_with_index(5);
    double valve_d_6 = power_valve_table_in_discharge.get_point_y_with_index(5);
    double pcmd_d_7 = power_speed_table_in_discharge.get_point_x_with_index(6);
    double speed_d_7 = power_speed_table_in_discharge.get_point_y_with_index(6);
    double valve_d_7 = power_valve_table_in_discharge.get_point_y_with_index(6);
    double pcmd_d_8 = power_speed_table_in_discharge.get_point_x_with_index(7);
    double speed_d_8 = power_speed_table_in_discharge.get_point_y_with_index(7);
    double valve_d_8 = power_valve_table_in_discharge.get_point_y_with_index(7);
    double pcmd_d_9 = power_speed_table_in_discharge.get_point_x_with_index(8);
    double speed_d_9 = power_speed_table_in_discharge.get_point_y_with_index(8);
    double valve_d_9 = power_valve_table_in_discharge.get_point_y_with_index(8);

    double pcmd_c_1 = power_speed_table_in_charge.get_point_x_with_index(8);
    double speed_c_1 = power_speed_table_in_charge.get_point_y_with_index(8);
    double valve_c_1 = power_valve_table_in_charge.get_point_y_with_index(8);
    double pcmd_c_2 = power_speed_table_in_charge.get_point_x_with_index(7);
    double speed_c_2 = power_speed_table_in_charge.get_point_y_with_index(7);
    double valve_c_2 = power_valve_table_in_charge.get_point_y_with_index(7);
    double pcmd_c_3 = power_speed_table_in_charge.get_point_x_with_index(6);
    double speed_c_3 = power_speed_table_in_charge.get_point_y_with_index(6);
    double valve_c_3 = power_valve_table_in_charge.get_point_y_with_index(6);
    double pcmd_c_4 = power_speed_table_in_charge.get_point_x_with_index(5);
    double speed_c_4 = power_speed_table_in_charge.get_point_y_with_index(5);
    double valve_c_4 = power_valve_table_in_charge.get_point_y_with_index(5);
    double pcmd_c_5 = power_speed_table_in_charge.get_point_x_with_index(4);
    double speed_c_5 = power_speed_table_in_charge.get_point_y_with_index(4);
    double valve_c_5 = power_valve_table_in_charge.get_point_y_with_index(4);
    double pcmd_c_6 = power_speed_table_in_charge.get_point_x_with_index(3);
    double speed_c_6 = power_speed_table_in_charge.get_point_y_with_index(3);
    double valve_c_6 = power_valve_table_in_charge.get_point_y_with_index(3);
    double pcmd_c_7 = power_speed_table_in_charge.get_point_x_with_index(2);
    double speed_c_7 = power_speed_table_in_charge.get_point_y_with_index(2);
    double valve_c_7 = power_valve_table_in_charge.get_point_y_with_index(2);
    double pcmd_c_8 = power_speed_table_in_charge.get_point_x_with_index(1);
    double speed_c_8 = power_speed_table_in_charge.get_point_y_with_index(1);
    double valve_c_8 = power_valve_table_in_charge.get_point_y_with_index(1);
    double pcmd_c_9 = power_speed_table_in_charge.get_point_x_with_index(0);
    double speed_c_9 = power_speed_table_in_charge.get_point_y_with_index(0);
    double valve_c_9 = power_valve_table_in_charge.get_point_y_with_index(0);

    double kp_start_c = get_power_slope_in_start_charge_mode();
    double kp_stop_c = get_power_slope_in_stop_charge_mode();
    double kp_start_d = get_power_slope_in_start_discharge_mode();
    double kp_stop_d = get_power_slope_in_stop_discharge_mode();

    double ks_start_c = get_speed_slope_in_start_charge_mode();
    double ks_stop_c = get_speed_slope_in_stop_charge_mode();
    double ks_start_d = get_speed_slope_in_start_discharge_mode();
    double ks_stop_d = get_speed_slope_in_stop_discharge_mode();

    double kv_start_c = get_valve_slope_in_start_charge_mode();
    double kv_stop_c = get_valve_slope_in_stop_charge_mode();
    double kv_start_d = get_valve_slope_in_start_discharge_mode();
    double kv_stop_d = get_valve_slope_in_stop_discharge_mode();

    double D = get_rotor_braking_Damp();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<mode<<", "
            <<setw(8)<<setprecision(4)<<p_d<<", "
            <<setw(8)<<setprecision(4)<<p_c<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_1<<", "
            <<setw(8)<<setprecision(4)<<speed_d_1<<", "
            <<setw(8)<<setprecision(4)<<valve_d_1<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_2<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<speed_d_2<<", "
            <<setw(8)<<setprecision(4)<<valve_d_2<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_3<<", "
            <<setw(8)<<setprecision(4)<<speed_d_3<<", "
            <<setw(8)<<setprecision(4)<<valve_d_3<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_4<<", "
            <<setw(8)<<setprecision(4)<<speed_d_4<<", "
            <<setw(8)<<setprecision(4)<<valve_d_4<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_5<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<speed_d_5<<", "
            <<setw(8)<<setprecision(4)<<valve_d_5<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_6<<", "
            <<setw(8)<<setprecision(4)<<speed_d_6<<", "
            <<setw(8)<<setprecision(4)<<valve_d_6<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_7<<", "
            <<setw(8)<<setprecision(4)<<speed_d_7<<", "
            <<setw(8)<<setprecision(4)<<valve_d_7<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_8<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<speed_d_8<<", "
            <<setw(8)<<setprecision(4)<<valve_d_8<<", "
            <<setw(8)<<setprecision(4)<<pcmd_d_9<<", "
            <<setw(8)<<setprecision(4)<<speed_d_9<<", "
            <<setw(8)<<setprecision(4)<<valve_d_9<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_1<<", "
            <<setw(8)<<setprecision(4)<<speed_c_1<<", "
            <<setw(8)<<setprecision(4)<<valve_c_1<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_2<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<speed_c_2<<", "
            <<setw(8)<<setprecision(4)<<valve_c_2<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_3<<", "
            <<setw(8)<<setprecision(4)<<speed_c_3<<", "
            <<setw(8)<<setprecision(4)<<valve_c_3<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_4<<", "
            <<setw(8)<<setprecision(4)<<speed_c_4<<", "
            <<setw(8)<<setprecision(4)<<valve_c_4<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_5<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<speed_c_5<<", "
            <<setw(8)<<setprecision(4)<<valve_c_5<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_6<<", "
            <<setw(8)<<setprecision(4)<<speed_c_6<<", "
            <<setw(8)<<setprecision(4)<<valve_c_6<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_7<<", "
            <<setw(8)<<setprecision(4)<<speed_c_7<<", "
            <<setw(8)<<setprecision(4)<<valve_c_7<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_8<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<speed_c_8<<", "
            <<setw(8)<<setprecision(4)<<valve_c_8<<", "
            <<setw(8)<<setprecision(4)<<pcmd_c_9<<", "
            <<setw(8)<<setprecision(4)<<speed_c_9<<", "
            <<setw(8)<<setprecision(4)<<valve_c_9<<", "
            <<setw(8)<<setprecision(4)<<kp_start_c<<", "
            <<setw(8)<<setprecision(4)<<kp_stop_c<<", "
            <<setw(8)<<setprecision(4)<<kp_start_d<<", "
            <<setw(8)<<setprecision(4)<<kp_stop_d<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<ks_start_c<<", "
            <<setw(8)<<setprecision(4)<<ks_stop_c<<", "
            <<setw(8)<<setprecision(4)<<ks_start_d<<", "
            <<setw(8)<<setprecision(4)<<ks_stop_d<<", "
            <<setw(8)<<setprecision(4)<<kv_start_c<<", "
            <<setw(8)<<setprecision(4)<<kv_stop_c<<", "
            <<setw(8)<<setprecision(4)<<kv_start_d<<", "
            <<setw(8)<<setprecision(4)<<kv_stop_d<<", "
            <<setw(8)<<setprecision(4)<<D<<" /";

    return osstream.str();
}

void ESAU0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double ESAU0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0;

    return 0.0;
}

void ESAU0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return ;

    return;
}

double ESAU0::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    return mint;
}

void ESAU0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("AA", i); i++;
}

double ESAU0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "A")
        return 0.0;

    return 0.0;
}

string ESAU0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESAU0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESAU0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}


