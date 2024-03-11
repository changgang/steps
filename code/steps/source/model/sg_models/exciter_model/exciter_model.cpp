#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/device/generator.h"
#include "header/steps_namespace.h"
#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

EXCITER_MODEL::EXCITER_MODEL(STEPS& toolkit) : SG_MODEL(toolkit)
{
}

EXCITER_MODEL::~EXCITER_MODEL()
{
    ;
}

string EXCITER_MODEL::get_model_type() const
{
    return "EXCITER";
}

double EXCITER_MODEL::get_initial_excitation_voltage_in_pu_from_sync_generator_model() const
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model!=NULL)
        return gen_model->get_initial_excitation_voltage_in_pu();
    else
        return 0.0;
}

double EXCITER_MODEL::get_compensated_voltage_in_pu()
{
    GENERATOR* generator = get_generator_pointer();
    COMPENSATOR_MODEL* comp_model = generator->get_compensator_model();
    if(comp_model!=NULL)
        return comp_model->get_compensated_voltage_in_pu();
    else
    {
        //unsigned int bus = generator->get_generator_bus();
        //return psdb.get_bus_positive_sequence_voltage_in_pu(bus);
        return get_terminal_voltage_in_pu();
    }
}

double EXCITER_MODEL::get_stabilizing_signal_in_pu()
{
    GENERATOR* generator = get_generator_pointer();
    STABILIZER_MODEL* pss = generator->get_stabilizer_model();
    if(pss!=NULL)
    {
        if(pss->is_model_initialized())
            return pss->get_stabilizing_signal_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;
}


double EXCITER_MODEL::get_field_current_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model!=NULL)
        return gen_model->get_field_current_in_pu_based_on_mbase();
    else
        return 0.0;
}

void EXCITER_MODEL::set_voltage_reference_in_pu(double vref)
{
    voltage_reference_in_pu = vref;
}
double EXCITER_MODEL::get_voltage_reference_in_pu() const
{
    return voltage_reference_in_pu;
}

STEPS_SPARSE_MATRIX* EXCITER_MODEL::get_linearized_matrix(string matrix_type)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="AVR" and avr_matrix!=NULL) return avr_matrix;
    if(matrix_type=="AVR-GEN" and avr_gen_matrix!=NULL) return avr_gen_matrix;
    if(matrix_type=="AVR-PSS" and avr_pss_matrix!=NULL) return avr_pss_matrix;
    if(matrix_type=="AVR-COMP" and avr_comp_matrix!=NULL) return avr_comp_matrix;
    return NULL;
}

void EXCITER_MODEL::set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="AVR")
    {
        if(avr_matrix==NULL) avr_matrix = matrix;
        else
        {
            if(avr_matrix!=matrix)
            {
                delete avr_matrix;
                avr_matrix = matrix;
            }
        }
    }
    if(matrix_type=="AVR-GEN")
    {
        if(avr_gen_matrix==NULL) avr_gen_matrix = matrix;
        else
        {
            if(avr_gen_matrix!=matrix)
            {
                delete avr_gen_matrix;
                avr_gen_matrix = matrix;
            }
        }
    }
    if(matrix_type=="AVR-PSS")
    {
        if(avr_pss_matrix==NULL) avr_pss_matrix = matrix;
        else
        {
            if(avr_pss_matrix!=matrix)
            {
                delete avr_pss_matrix;
                avr_pss_matrix = matrix;
            }
        }
    }
    if(matrix_type=="AVR-COMP")
    {
        if(avr_comp_matrix==NULL) avr_comp_matrix = matrix;
        else
        {
            if(avr_comp_matrix!=matrix)
            {
                delete avr_comp_matrix;
                avr_comp_matrix = matrix;
            }
        }
    }
}
