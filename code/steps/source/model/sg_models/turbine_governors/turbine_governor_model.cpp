#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/model/sg_models/sync_generator_model/sync_generator_model.h"
#include "header/device/generator.h"

#include "header/basic/utility.h"
#include <cstdio>
#include <iostream>

TURBINE_GOVERNOR_MODEL::TURBINE_GOVERNOR_MODEL(STEPS& toolkit) : SG_MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_GENERATOR);
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
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model!=NULL)
        return gen_model->get_rotor_speed_deviation_in_pu();
    else
        return 0.0;
}

double TURBINE_GOVERNOR_MODEL::get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model() const
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model!=NULL)
        return gen_model->get_initial_mechanical_power_in_pu_based_on_mbase();
    else
        return 0.0;
}

void TURBINE_GOVERNOR_MODEL::set_initial_mechanical_power_reference_in_pu_based_on_mbase(double P)
{
    initial_mechanical_power_reference_in_pu = P;
}

double TURBINE_GOVERNOR_MODEL::get_initial_mechanical_power_reference_in_pu_based_on_mbase() const
{
    return initial_mechanical_power_reference_in_pu;
}

double TURBINE_GOVERNOR_MODEL::get_mechanical_power_reference_in_pu_based_on_mbase() const
{
    GENERATOR* generator = get_generator_pointer();
    TURBINE_LOAD_CONTROLLER_MODEL* model =generator->get_turbine_load_controller_model();
    if(model!=NULL)
        return model->get_mechanical_power_reference_in_pu_based_on_mbase();
    else
        return get_initial_mechanical_power_reference_in_pu_based_on_mbase();
}

STEPS_SPARSE_MATRIX* TURBINE_GOVERNOR_MODEL::get_linearized_matrix(string matrix_type)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="GOV" and gov_matrix!=NULL) return gov_matrix;
    if(matrix_type=="GOV-GEN" and gov_gen_matrix!=NULL) return gov_gen_matrix;
    if(matrix_type=="GOV-TLC" and gov_tlc_matrix!=NULL) return gov_tlc_matrix;
    return NULL;
}

void TURBINE_GOVERNOR_MODEL::set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="GOV")
    {
        if(gov_matrix==NULL) gov_matrix = matrix;
        else
        {
            if(gov_matrix!=matrix)
            {
                delete gov_matrix;
                gov_matrix = matrix;
            }
        }
    }
    if(matrix_type=="GOV-GEN")
    {
        if(gov_gen_matrix==NULL) gov_gen_matrix = matrix;
        else
        {
            if(gov_gen_matrix!=matrix)
            {
                delete gov_gen_matrix;
                gov_gen_matrix = matrix;
            }
        }
    }
    if(matrix_type=="GOV-TLC")
    {
        if(gov_tlc_matrix==NULL) gov_tlc_matrix = matrix;
        else
        {
            if(gov_tlc_matrix!=matrix)
            {
                delete gov_tlc_matrix;
                gov_tlc_matrix = matrix;
            }
        }
    }
}
