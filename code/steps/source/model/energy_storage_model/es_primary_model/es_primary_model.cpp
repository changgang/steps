#include "header/model/energy_storage_model/es_primary_model/es_primary_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ES_PRIMARY_MODEL::ES_PRIMARY_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{

}

ES_PRIMARY_MODEL::~ES_PRIMARY_MODEL()
{
    ;
}

string ES_PRIMARY_MODEL::get_model_type() const
{
    return "ES PRIMARY";
}

double ES_PRIMARY_MODEL::get_rotor_speed_reference_in_pu() const
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

double ES_PRIMARY_MODEL::get_rotor_speed_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ROTOR_MODEL* rotor_model = energy_storage->get_es_rotor_model();
    if(rotor_model!=NULL)
    {
        return rotor_model->get_rotor_speed_in_pu();
    }
    else
    {
        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
            return 0.0;
        if(mode==ES_CHARGE_MODE or mode==ES_START_CHARGE_MODE or mode == ES_STOP_CHARGE_MODE)
            return -1.0;
        if(mode==ES_DISCHARGE_MODE or mode==ES_START_DISCHARGE_MODE or mode==ES_STOP_DISCHARGE_MODE)
            return 1.0;

        return 0.0;
    }
}

double ES_PRIMARY_MODEL::get_active_power_reference_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ELECTRICAL_MODEL* elec_model = energy_storage->get_es_electrical_model();
    if(elec_model!=NULL)
        return elec_model->get_frequency_regulation_power_in_pu();
    else
    {
        ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();
        return converter_model->get_initial_active_power_command_in_pu_based_on_mbase();
    }
}

double ES_PRIMARY_MODEL::get_valve_reference_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_AUXILIARY_MODEL* aux_model = energy_storage->get_es_auxiliary_model();
    if(aux_model!=NULL)
    {
        return aux_model->get_valve_reference_in_pu();
    }
    else
    {
        ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();
        return converter_model->get_initial_active_power_command_in_pu_based_on_mbase();
    }
}

bool ES_PRIMARY_MODEL::is_primary_model_frozen() const
{
    return primary_model_freeze_flag;
}

void ES_PRIMARY_MODEL::freeze_primary_model()
{
    primary_model_freeze_flag = true;
}

void ES_PRIMARY_MODEL::unfreeze_primary_model()
{
    primary_model_freeze_flag = false;
}

bool ES_PRIMARY_MODEL::is_rotor_locked() const
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
