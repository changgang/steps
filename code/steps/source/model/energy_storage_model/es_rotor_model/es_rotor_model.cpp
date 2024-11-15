#include "header/model/energy_storage_model/es_rotor_model/es_rotor_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ES_ROTOR_MODEL::ES_ROTOR_MODEL(STEPS& toolkit) : ES_MODEL(toolkit),
                                                 rotor_speed_block(toolkit)
{

}

ES_ROTOR_MODEL::~ES_ROTOR_MODEL()
{
    ;
}

string ES_ROTOR_MODEL::get_model_type() const
{
    return "ES ROTOR";
}

void ES_ROTOR_MODEL::set_initial_mechanical_power_in_pu_on_mbase(double p)
{
    this->initial_Pmech_in_pu_on_mbase = p;
}

double ES_ROTOR_MODEL::get_initial_mechanical_power_in_pu_on_mbase() const
{
    return this->initial_Pmech_in_pu_on_mbase;
}

void ES_ROTOR_MODEL::set_initial_rotor_speed_reference_in_pu(double s)
{
    this->initial_rotor_speed_reference_in_pu = s;
}

double ES_ROTOR_MODEL::get_initial_rotor_speed_reference_in_pu() const
{
    return this->initial_rotor_speed_reference_in_pu;
}

void ES_ROTOR_MODEL::set_H_in_s(double H)
{
    rotor_speed_block.set_T_in_s(2*H);
}

double ES_ROTOR_MODEL::get_H_in_s() const
{
    return rotor_speed_block.get_T_in_s()/2.0;
}

void ES_ROTOR_MODEL::lock_rotor()
{
    rotor_lock_flag = true;
}

void ES_ROTOR_MODEL::unlock_rotor()
{
    rotor_lock_flag = false;
}

bool ES_ROTOR_MODEL::is_rotor_locked() const
{
    return rotor_lock_flag;
}

void ES_ROTOR_MODEL::activate_rotor_braking()
{
    this->rotor_braking_flag = true;
}

void ES_ROTOR_MODEL::deactivate_rotor_braking()
{
    this->rotor_braking_flag = false;
}

bool ES_ROTOR_MODEL::is_rotor_braking_activated() const
{
    return this->rotor_braking_flag;
}

INTEGRAL_BLOCK* ES_ROTOR_MODEL::get_rotor_speed_block()
{
    return &rotor_speed_block;
}

double ES_ROTOR_MODEL::get_mechanical_power_in_pu_on_mbase() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_PRIMARY_MODEL* primary_model = energy_storage->get_es_primary_model();
    if(primary_model!=NULL)
    {
        if(not primary_model->is_model_initialized())
            primary_model->initialize();

        return primary_model->get_mechanical_power_in_pu_on_mbase();
    }
    else
    {
        return get_initial_mechanical_power_in_pu_on_mbase();
    }
}

double ES_ROTOR_MODEL::get_active_power_including_stator_loss_in_pu_on_mbase() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* converter = energy_storage->get_es_converter_model();
    return converter->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
}

double ES_ROTOR_MODEL::get_rotor_speed_reference_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_AUXILIARY_MODEL* aux_model = energy_storage->get_es_auxiliary_model();
    if(aux_model!=NULL)
    {
        return aux_model->get_rotor_speed_reference_in_pu();
    }
    else
    {
        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
            return 0.0;
        if(mode==ES_CHARGE_MODE)
            return -1.0;
        if(mode==ES_DISCHARGE_MODE)
            return 1.0;

        return 0.0;
    }
}

