#include "header/model/energy_storage_model/es_battery_model/es_battery_model.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>

using namespace std;

ES_BATTERY_MODEL::ES_BATTERY_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{

}

ES_BATTERY_MODEL::~ES_BATTERY_MODEL()
{

}

string ES_BATTERY_MODEL::get_model_type() const
{
    return "ES BATTERY";
}
