#include "header/model/energy_storage_model/es_rotor_model/es_rotor_model.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ES_ROTOR_MODEL::ES_ROTOR_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
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

double ES_ROTOR_MODEL::get_mechanical_power_in_pu() const
{
    return 0;
}

double ES_ROTOR_MODEL::get_terminal_active_power_including_stator_loss_in_pu() const
{
    return 0;
}
