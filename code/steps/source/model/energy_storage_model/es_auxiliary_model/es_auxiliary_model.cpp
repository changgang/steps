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

