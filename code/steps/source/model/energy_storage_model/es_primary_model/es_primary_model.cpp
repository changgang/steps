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
    return 0;
}

double ES_PRIMARY_MODEL::get_rotor_speed_in_pu() const
{
    return 0;
}

double ES_PRIMARY_MODEL::get_active_power_reference_in_pu() const
{
    return 0;
}

double ES_PRIMARY_MODEL::get_valve_reference_in_pu() const
{
    return 0;
}
