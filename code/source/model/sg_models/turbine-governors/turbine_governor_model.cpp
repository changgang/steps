#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/model/sg_models/sync_generator_model/sync_generator_model.h"
#include "header/device/generator.h"

#include "header/basic/utility.h"
#include <cstdio>

TURBINE_GOVERNOR_MODEL::TURBINE_GOVERNOR_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
}

TURBINE_GOVERNOR_MODEL::~TURBINE_GOVERNOR_MODEL()
{
    ;
}

string TURBINE_GOVERNOR_MODEL::get_model_type() const
{
    return "TURBINE GOVERNOR";
}

double TURBINE_GOVERNOR_MODEL::get_rotor_speed_deviation_in_pu_from_sync_generator_model() const
{
    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator==NULL)
        return 0.0;

    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model==NULL)
        return 0.0;
    else
        return gen_model->get_variable_with_name("GENERATOR ROTOR SPEED DEVIATION IN PU");


}
double TURBINE_GOVERNOR_MODEL::get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model() const
{
    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator==NULL)
        return 0.0;
    else
    {
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model==NULL)
            return 0.0;
        else
            return gen_model->get_initial_mechanical_power_in_pu_based_on_mbase();
    }
}

void TURBINE_GOVERNOR_MODEL::set_mechanical_power_reference_in_pu_based_on_mbase(double P)
{
    mechanical_power_reference_in_pu = P;
}

double TURBINE_GOVERNOR_MODEL::get_mechanical_power_reference_in_pu_based_on_mbase() const
{
    return mechanical_power_reference_in_pu;
}
