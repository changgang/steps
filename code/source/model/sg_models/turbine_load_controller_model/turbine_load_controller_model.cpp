#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model.h"
#include <cstdio>
#include "header/basic/utility.h"
TURBINE_LOAD_CONTROLLER_MODEL::TURBINE_LOAD_CONTROLLER_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
}

TURBINE_LOAD_CONTROLLER_MODEL::~TURBINE_LOAD_CONTROLLER_MODEL()
{
}

string TURBINE_LOAD_CONTROLLER_MODEL::get_model_type() const
{
    return "TURBINE LOAD CONTROLLER";
}

double TURBINE_LOAD_CONTROLLER_MODEL::get_initial_mechanical_power_reference_in_pu_based_on_mbase_from_turbine_governor_model() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        TURBINE_GOVERNOR_MODEL* turbine = generator->get_turbine_governor_model();
        if(turbine!=NULL)
        {
            if(not turbine->is_model_initialized())
                turbine->initialize();
            return turbine->get_initial_mechanical_power_reference_in_pu_based_on_mbase();
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double TURBINE_LOAD_CONTROLLER_MODEL::get_terminal_active_power_in_pu_based_on_mbase_from_generator_model() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
        if(sg!=NULL)
        {
            if(not sg->is_model_initialized())
                sg->initialize();
            return sg->get_terminal_active_power_in_pu_based_on_mbase();
        }
    }
    return 0.0;
}

double TURBINE_LOAD_CONTROLLER_MODEL::get_rotor_speed_deviation_in_pu_from_generator_model() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
        if(sg!=NULL)
        {
            if(not sg->is_model_initialized())
                sg->initialize();
            return sg->get_rotor_speed_deviation_in_pu();
        }
    }
    return 0.0;
}

