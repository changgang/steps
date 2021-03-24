#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model.h"
#include <cstdio>
#include "header/basic/utility.h"
TURBINE_LOAD_CONTROLLER_MODEL::TURBINE_LOAD_CONTROLLER_MODEL(STEPS& toolkit) : SG_MODEL(toolkit)
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

double TURBINE_LOAD_CONTROLLER_MODEL::get_terminal_active_power_in_pu_based_on_mbase_from_generator_model() const
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
    if(sg!=NULL)
    {
        if(not sg->is_model_initialized())
            sg->initialize();
        return sg->get_terminal_active_power_in_pu_based_on_mbase();
    }
    else
        return 0.0;
}

double TURBINE_LOAD_CONTROLLER_MODEL::get_rotor_speed_deviation_in_pu_from_generator_model() const
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
    if(sg!=NULL)
    {
        if(not sg->is_model_initialized())
            sg->initialize();
        return sg->get_rotor_speed_deviation_in_pu();
    }
    else
        return 0.0;
}


STEPS_SPARSE_MATRIX* TURBINE_LOAD_CONTROLLER_MODEL::get_linearized_matrix(string matrix_type)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="TLC" and tlc_matrix!=NULL) return tlc_matrix;
    if(matrix_type=="TLC-GOV" and tlc_gov_matrix!=NULL) return tlc_gov_matrix;
    return NULL;
}

void TURBINE_LOAD_CONTROLLER_MODEL::set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="TLC")
    {
        if(tlc_matrix==NULL) tlc_matrix = matrix;
        else
        {
            if(tlc_matrix!=matrix)
            {
                delete tlc_matrix;
                tlc_matrix = matrix;
            }
        }
    }
    if(matrix_type=="TLC-GOV")
    {
        if(tlc_gov_matrix==NULL) tlc_gov_matrix = matrix;
        else
        {
            if(tlc_gov_matrix!=matrix)
            {
                delete tlc_gov_matrix;
                tlc_gov_matrix = matrix;
            }
        }
    }
}
