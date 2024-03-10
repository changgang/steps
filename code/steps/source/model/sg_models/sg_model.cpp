#include "header/model/sg_models/sg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
SG_MODEL::SG_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_GENERATOR);
    Aptr = NULL;
    Bptr = NULL;
    Cptr = NULL;
    Dptr = NULL;
}

SG_MODEL::~SG_MODEL()
{
    if(Aptr != NULL)
    {
        delete Aptr;
        Aptr = NULL;
    }
    if(Bptr != NULL)
    {
        delete Bptr;
        Bptr = NULL;
    }
    if(Cptr != NULL)
    {
        delete Cptr;
        Cptr = NULL;
    }
    if(Dptr != NULL)
    {
        delete Dptr;
        Dptr = NULL;
    }
}

GENERATOR* SG_MODEL::get_generator_pointer() const
{
    return (GENERATOR*) get_device_pointer();
}

double SG_MODEL::get_mbase_in_MVA() const
{
    GENERATOR* gen = get_generator_pointer();
    return gen->get_mbase_in_MVA();
}

double SG_MODEL::get_one_over_mbase_in_one_over_MVA() const
{
    GENERATOR* gen = get_generator_pointer();
    return gen->get_one_over_mbase_in_one_over_MVA();
}

double SG_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

complex<double> SG_MODEL::get_terminal_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double SG_MODEL::get_terminal_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

void SG_MODEL::initialize_ABCD_matrix_for_linearization()
{
    if(Aptr == NULL)
        Aptr = new STEPS_SPARSE_MATRIX;
    if(Bptr == NULL)
        Bptr = new STEPS_SPARSE_MATRIX;
    if(Cptr == NULL)
        Cptr = new STEPS_SPARSE_MATRIX;
    if(Dptr == NULL)
        Dptr = new STEPS_SPARSE_MATRIX;
}

void SG_MODEL::build_linearized_matrix_ABCD_with_basic_ABCD_and_EFGH(vector<STEPS_SPARSE_MATRIX*> matrix)
{
    if(matrix.size()!=8)
    {
        ostringstream osstream;
        osstream<<"ERROR. Vector matrix in SG_MODEL::"<<__FUNCTION__<<"() has "<<matrix.size()<<"individual matrix which is NOT 8";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    STEPS_SPARSE_MATRIX& Abasic = *(matrix[0]);
    STEPS_SPARSE_MATRIX& Bbasic = *(matrix[1]);
    STEPS_SPARSE_MATRIX& Cbasic = *(matrix[2]);
    STEPS_SPARSE_MATRIX& Dbasic = *(matrix[3]);
    STEPS_SPARSE_MATRIX& E = *(matrix[4]);
    STEPS_SPARSE_MATRIX& F = *(matrix[5]);
    STEPS_SPARSE_MATRIX& G = *(matrix[6]);
    STEPS_SPARSE_MATRIX& H = *(matrix[7]);

    STEPS_SPARSE_MATRIX ED = E*Dbasic;
    STEPS_SPARSE_MATRIX DE = Dbasic*E;
    STEPS_SPARSE_MATRIX I1 = build_identity_matrix(ED);
    STEPS_SPARSE_MATRIX I2 = build_identity_matrix(DE);

    STEPS_SPARSE_MATRIX DIFF1 = I1-ED;
    STEPS_SPARSE_MATRIX DIFF2 = I2-DE;
    STEPS_SPARSE_MATRIX INV1 = inv(DIFF1);
    STEPS_SPARSE_MATRIX INV2 = inv(DIFF2);

    STEPS_SPARSE_MATRIX B_INV1 = Bbasic*INV1;
    STEPS_SPARSE_MATRIX B_INV1_E = B_INV1*E;
    STEPS_SPARSE_MATRIX B_INV1_F = B_INV1*F;
    STEPS_SPARSE_MATRIX B_INV1_E_C = B_INV1_E*Cbasic;
    STEPS_SPARSE_MATRIX G_INV2 = G*INV2;
    STEPS_SPARSE_MATRIX G_INV2_C = G_INV2*Cbasic;
    STEPS_SPARSE_MATRIX G_INV2_D = G_INV2*Dbasic;
    STEPS_SPARSE_MATRIX G_INV2_D_F = G_INV2_D*F;

    STEPS_SPARSE_MATRIX Atemp = Abasic+B_INV1_E_C;
    STEPS_SPARSE_MATRIX Dtemp = G_INV2_D_F+H;

    *Aptr = Atemp;
    *Bptr = B_INV1_F;
    *Cptr = G_INV2_C;
    *Dptr = Dtemp;
}

STEPS_SPARSE_MATRIX SG_MODEL::get_linearized_matrix_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_matrix_A();
        case 'B':
            return get_linearized_matrix_B();
        case 'C':
            return get_linearized_matrix_C();
        case 'D':
            return get_linearized_matrix_D();
        default:
            STEPS_SPARSE_MATRIX matrix;
            return matrix;
    }
}

STEPS_SPARSE_MATRIX SG_MODEL::get_linearized_matrix_A() const
{
    if(Aptr!=NULL) return *Aptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to A matrix exists in SG_MODEL. Cannot return linearized A matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX SG_MODEL::get_linearized_matrix_B() const
{
    if(Bptr!=NULL) return *Bptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to B matrix exists in SG_MODEL. Cannot return linearized B matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX SG_MODEL::get_linearized_matrix_C() const
{
    if(Cptr!=NULL) return *Cptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to C matrix exists in SG_MODEL. Cannot return linearized C matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX SG_MODEL::get_linearized_matrix_D() const
{
    if(Dptr!=NULL) return *Dptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to D matrix exists in SG_MODEL. Cannot return linearized D matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX* SG_MODEL::get_linearized_matrix_pointer_A()
{
    return Aptr;
}

STEPS_SPARSE_MATRIX* SG_MODEL::get_linearized_matrix_pointer_B()
{
    return Bptr;
}

STEPS_SPARSE_MATRIX* SG_MODEL::get_linearized_matrix_pointer_C()
{
    return Cptr;
}

STEPS_SPARSE_MATRIX* SG_MODEL::get_linearized_matrix_pointer_D()
{
    return Dptr;
}

