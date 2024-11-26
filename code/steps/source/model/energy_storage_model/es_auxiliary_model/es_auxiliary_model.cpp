#include "header/model/energy_storage_model/es_auxiliary_model/es_auxiliary_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ES_AUXILIARY_MODEL::ES_AUXILIARY_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{

}

ES_AUXILIARY_MODEL::~ES_AUXILIARY_MODEL()
{
    ;
}

string ES_AUXILIARY_MODEL::get_model_type() const
{
    return "ES AUXILIARY";
}

bool ES_AUXILIARY_MODEL::is_primary_model_frozen() const
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_PRIMARY_MODEL* pmy = es->get_es_primary_model();
    if(pmy!=NULL)
        return pmy->is_primary_model_frozen();
    else
    {
        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
            return true;
        else
            return false;
    }
}

void ES_AUXILIARY_MODEL::freeze_primary_model()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_PRIMARY_MODEL* pmy = es->get_es_primary_model();
    if(pmy!=NULL)
        return pmy->freeze_primary_model();
}

void ES_AUXILIARY_MODEL::unfreeze_primary_model()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_PRIMARY_MODEL* pmy = es->get_es_primary_model();
    if(pmy!=NULL)
        return pmy->unfreeze_primary_model();
}

bool ES_AUXILIARY_MODEL::is_electrical_active_power_control_frozen() const
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec = es->get_es_electrical_model();
    if(elec!=NULL)
        return not elec->is_active_power_control_activated();
    else
    {
        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
            return true;
        else
            return false;
    }
}

void ES_AUXILIARY_MODEL::freeze_electrical_active_power_control()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec = es->get_es_electrical_model();
    if(elec!=NULL)
        return elec->deactivate_active_power_control();
}

void ES_AUXILIARY_MODEL::unfreeze_electrical_active_power_control()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec = es->get_es_electrical_model();
    if(elec!=NULL)
        return elec->activate_active_power_control();
}

bool ES_AUXILIARY_MODEL::is_manual_control() const
{
    return manual_control_flag;
}

void ES_AUXILIARY_MODEL::activate_manual_control()
{
    manual_control_flag = true;
}

void ES_AUXILIARY_MODEL::deactivate_manual_control()
{
    manual_control_flag = false;
}

void ES_AUXILIARY_MODEL::set_speed_reference_when_into_start_discharge_mode(double s)
{
    speed_reference_when_into_start_discharge_mode = s;
}

double ES_AUXILIARY_MODEL::get_speed_reference_when_into_start_discharge_mode() const
{
    return speed_reference_when_into_start_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_speed_reference_when_into_stop_discharge_mode(double s)
{
    speed_reference_when_into_stop_discharge_mode = s;
}

double ES_AUXILIARY_MODEL::get_speed_reference_when_into_stop_discharge_mode() const
{
    return speed_reference_when_into_stop_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_speed_reference_when_into_start_charge_mode(double s)
{
    speed_reference_when_into_start_charge_mode = s;
}

double ES_AUXILIARY_MODEL::get_speed_reference_when_into_start_charge_mode() const
{
    return speed_reference_when_into_start_charge_mode;
}

void ES_AUXILIARY_MODEL::set_speed_reference_when_into_stop_charge_mode(double s)
{
    speed_reference_when_into_stop_charge_mode = s;
}

double ES_AUXILIARY_MODEL::get_speed_reference_when_into_stop_charge_mode() const
{
    return speed_reference_when_into_stop_charge_mode;
}

void ES_AUXILIARY_MODEL::set_power_reference_when_into_start_discharge_mode(double p)
{
    power_reference_when_into_start_discharge_mode = p;
}

double ES_AUXILIARY_MODEL::get_power_reference_when_into_start_discharge_mode() const
{
    return power_reference_when_into_start_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_power_reference_when_into_stop_discharge_mode(double p)
{
    power_reference_when_into_stop_discharge_mode = p;
}

double ES_AUXILIARY_MODEL::get_power_reference_when_into_stop_discharge_mode() const
{
    return power_reference_when_into_stop_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_power_reference_when_into_start_charge_mode(double p)
{
    power_reference_when_into_start_charge_mode = p;
}

double ES_AUXILIARY_MODEL::get_power_reference_when_into_start_charge_mode() const
{
    return power_reference_when_into_start_charge_mode;
}

void ES_AUXILIARY_MODEL::set_power_reference_when_into_stop_charge_mode(double p)
{
    power_reference_when_into_stop_charge_mode = p;
}

double ES_AUXILIARY_MODEL::get_power_reference_when_into_stop_charge_mode() const
{
    return power_reference_when_into_stop_charge_mode;
}

void ES_AUXILIARY_MODEL::set_valve_reference_when_into_start_discharge_mode(double p)
{
    valve_reference_when_into_start_discharge_mode = p;
}

double ES_AUXILIARY_MODEL::get_valve_reference_when_into_start_discharge_mode() const
{
    return valve_reference_when_into_start_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_valve_reference_when_into_stop_discharge_mode(double p)
{
    valve_reference_when_into_stop_discharge_mode = p;
}

double ES_AUXILIARY_MODEL::get_valve_reference_when_into_stop_discharge_mode() const
{
    return valve_reference_when_into_stop_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_valve_reference_when_into_start_charge_mode(double p)
{
    valve_reference_when_into_start_charge_mode = p;
}

double ES_AUXILIARY_MODEL::get_valve_reference_when_into_start_charge_mode() const
{
    return valve_reference_when_into_start_charge_mode;
}

void ES_AUXILIARY_MODEL::set_valve_reference_when_into_stop_charge_mode(double p)
{
    valve_reference_when_into_stop_charge_mode = p;
}

double ES_AUXILIARY_MODEL::get_valve_reference_when_into_stop_charge_mode() const
{
    return valve_reference_when_into_stop_charge_mode;
}

void ES_AUXILIARY_MODEL::set_time_when_into_start_discharge_mode(double t)
{
    time_when_into_start_discharge_mode = t;
}

double ES_AUXILIARY_MODEL::get_time_when_into_start_discharge_mode() const
{
    return time_when_into_start_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_time_when_into_stop_discharge_mode(double t)
{
    time_when_into_stop_discharge_mode = t;
}

double ES_AUXILIARY_MODEL::get_time_when_into_stop_discharge_mode() const
{
    return time_when_into_stop_discharge_mode;
}

void ES_AUXILIARY_MODEL::set_time_when_into_start_charge_mode(double t)
{
    time_when_into_start_charge_mode = t;
}

double ES_AUXILIARY_MODEL::get_time_when_into_start_charge_mode() const
{
    return time_when_into_start_charge_mode;
}

void ES_AUXILIARY_MODEL::set_time_when_into_stop_charge_mode(double t)
{
    time_when_into_stop_charge_mode = t;
}

double ES_AUXILIARY_MODEL::get_time_when_into_stop_charge_mode() const
{
    return time_when_into_stop_charge_mode;
}

void ES_AUXILIARY_MODEL::set_manual_power_in_pu(double p)
{
    this->manual_power = p;
}

double ES_AUXILIARY_MODEL::get_manual_power_in_pu() const
{
    return this->manual_power;
}

void ES_AUXILIARY_MODEL::activate_rotor_braking()
{
    this->rotor_braking_flag = true;
}

void ES_AUXILIARY_MODEL::deactivate_rotor_braking()
{
    this->rotor_braking_flag = false;
}

bool ES_AUXILIARY_MODEL::is_rotor_braking_activated() const
{
    return this->rotor_braking_flag;
}

void ES_AUXILIARY_MODEL::lock_rotor()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ROTOR_MODEL* rotor = es->get_es_rotor_model();
    if(rotor!=NULL)
    {
        rotor->lock_rotor();
    }
}

void ES_AUXILIARY_MODEL::unlock_rotor()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ROTOR_MODEL* rotor = es->get_es_rotor_model();
    if(rotor!=NULL)
    {
        rotor->unlock_rotor();
    }
}

bool ES_AUXILIARY_MODEL::is_rotor_locked() const
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ROTOR_MODEL* rotor = es->get_es_rotor_model();
    if(rotor!=NULL)
    {
        return rotor->is_rotor_locked();
    }
    else
        return true;
}

double ES_AUXILIARY_MODEL::get_rotor_speed_in_pu() const
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ROTOR_MODEL* rotor = es->get_es_rotor_model();
    if(rotor!=NULL)
    {
        return rotor->get_rotor_speed_in_pu();
    }
    else
    {
        return 0.0;
    }
}

double ES_AUXILIARY_MODEL::get_mechanical_power_in_pu_on_mbase() const
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_PRIMARY_MODEL* pmy = es->get_es_primary_model();
    if(pmy!=NULL)
    {
        return pmy->get_mechanical_power_in_pu_on_mbase();
    }
    else
    {
        return 0.0;
    }
}

double ES_AUXILIARY_MODEL::get_active_power_including_stator_loss_in_pu_on_mbase() const
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* con = es->get_es_converter_model();
    if(con!=NULL)
    {
        return con->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
    }
    else
    {
        return 0.0;
    }
}

void ES_AUXILIARY_MODEL::initialize_primary_model_as_out_service_mode()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_PRIMARY_MODEL* pmy = es->get_es_primary_model();
    if(pmy!=NULL)
    {
        pmy->initialize_Pmech_as_zero();
    }
    else
        return;
}

void ES_AUXILIARY_MODEL::initialize_electrical_model_as_out_service_mode()
{
    ENERGY_STORAGE* es = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec = es->get_es_electrical_model();
    if(elec!=NULL)
    {
        elec->initialize_active_power_control_as_zero();
    }
    else
        return;
}

double ES_AUXILIARY_MODEL::get_rotor_braking_Damp() const
{
    return this->rotor_braking_Damp;
}

void ES_AUXILIARY_MODEL::set_rotor_braking_Damp(double D)
{
    this->rotor_braking_Damp = D;
}

void ES_AUXILIARY_MODEL::set_current_Pref(double p)
{
    this->current_Pref = p;
}

double ES_AUXILIARY_MODEL::get_current_Pref() const
{
    return this->current_Pref;
}

void ES_AUXILIARY_MODEL::set_current_speedref(double s)
{
    this->current_speedref = s;
}

double ES_AUXILIARY_MODEL::get_current_speedref() const
{
    return this->current_speedref;
}

void ES_AUXILIARY_MODEL::set_current_Yref(double y)
{
    this->current_Yref = y;
}

double ES_AUXILIARY_MODEL::get_current_Yref() const
{
    return this->current_Yref;
}
