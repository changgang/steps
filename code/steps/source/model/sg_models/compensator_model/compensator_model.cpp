#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/device/generator.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

COMPENSATOR_MODEL::COMPENSATOR_MODEL(STEPS& toolkit) : SG_MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
}

COMPENSATOR_MODEL::~COMPENSATOR_MODEL()
{
    ;
}

string COMPENSATOR_MODEL::get_model_type() const
{
    return "COMPENSATOR";
}

complex<double> COMPENSATOR_MODEL::get_generator_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

complex<double> COMPENSATOR_MODEL::get_generator_terminal_complex_current_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
    if(gen_model!=NULL)
    {
        if(not gen_model->is_model_initialized())
            gen_model->initialize();
        complex<double> I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        return I;
    }
    else
        return 0.0;
}

double COMPENSATOR_MODEL::get_generator_terminal_voltage_in_pu()
{
    return get_terminal_voltage_in_pu();
}

double COMPENSATOR_MODEL::get_generator_terminal_current_in_pu() const
{
    return abs(get_generator_terminal_complex_current_in_pu());
}

STEPS_SPARSE_MATRIX* COMPENSATOR_MODEL::get_linearized_matrix(string matrix_type)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="COMP" and comp_matrix!=NULL) return comp_matrix;
    if(matrix_type=="COMP-AVR" and comp_avr_matrix!=NULL) return comp_avr_matrix;
    return NULL;
}

void COMPENSATOR_MODEL::set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix)
{
    matrix_type = string2upper(matrix_type);
    if(matrix_type=="COMP")
    {
        if(comp_matrix==NULL) comp_matrix = matrix;
        else
        {
            if(comp_matrix!=matrix)
            {
                delete comp_matrix;
                comp_matrix = matrix;
            }
        }
    }
    if(matrix_type=="COMP-AVR")
    {
        if(comp_avr_matrix==NULL) comp_avr_matrix = matrix;
        else
        {
            if(comp_avr_matrix!=matrix)
            {
                delete comp_avr_matrix;
                comp_avr_matrix = matrix;
            }
        }
    }
}
