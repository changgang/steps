#include "header/model/load_model/load_model.h"
#include "header/power_system_database.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

LOAD_MODEL::LOAD_MODEL(STEPS& toolkit) : MODEL(toolkit)
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_LOAD);
    Aptr = NULL;
    Bptr = NULL;
    Cptr = NULL;
    Dptr = NULL;
}

LOAD_MODEL::~LOAD_MODEL()
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

LOAD* LOAD_MODEL::get_load_pointer() const
{
    return (LOAD*) get_device_pointer();
}


void LOAD_MODEL::set_voltage_source_flag(bool flag)
{
    voltage_source_flag = flag;
}

bool LOAD_MODEL::get_voltage_source_flag() const
{
    return voltage_source_flag;
}

bool LOAD_MODEL::is_voltage_source() const
{
    return voltage_source_flag==true;
}

string LOAD_MODEL::get_model_type() const
{
    return "LOAD CHARACTERISTICS";
}


void LOAD_MODEL::synchronize_bus_voltage_and_frequency()
{
    BUS* bus = get_bus_pointer();
    voltage_pu = bus->get_positive_sequence_voltage_in_pu();
    complex_voltage_pu = bus->get_positive_sequence_complex_voltage_in_pu();
    frequency_deviation_pu = bus->get_frequency_deviation_in_pu();
}

double LOAD_MODEL::get_bus_positive_sequence_voltage_in_pu() const
{
    return voltage_pu;

    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_pu();
}

double LOAD_MODEL::get_bus_positive_sequence_voltage_in_kV() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_voltage_in_kV();
}

complex<double> LOAD_MODEL::get_bus_positive_sequence_complex_voltage_in_pu() const
{
    return complex_voltage_pu;

    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double LOAD_MODEL::get_bus_frequency_deviation_in_pu() const
{
    return frequency_deviation_pu;

    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

double LOAD_MODEL::get_bus_base_frequency_in_Hz() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_base_frequency_in_Hz();
}

void LOAD_MODEL::set_subsystem_type(SUBSYSTEM_TYPE subtype)
{
    subsystem_type = subtype;
}

SUBSYSTEM_TYPE LOAD_MODEL::get_subsystem_type() const
{
    return subsystem_type;
}

string LOAD_MODEL::get_detailed_model_name() const
{
    string model_name = get_model_name();
    switch(get_subsystem_type())
    {
        case BUS_SUBSYSTEM_TYPE:
        {
            model_name += "BL";
            break;
        }
        case AREA_SUBSYSTEM_TYPE:
        {
            model_name += "AR";
            break;
        }
        case ZONE_SUBSYSTEM_TYPE:
        {
            model_name += "ZN";
            break;
        }
        case OWNER_SUBSYSTEM_TYPE:
        {
            model_name += "OW";
            break;
        }
        default:
        {
            model_name += "AL";
            break;
        }
    }
    return model_name;
}


void LOAD_MODEL::initialize_ABCD_matrix_for_linearization()
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

STEPS_SPARSE_MATRIX LOAD_MODEL::get_linearized_system_variable(char var) const
{
    var = toupper(var);
    switch(var)
    {
        case 'A':
            return get_linearized_system_A();
        case 'B':
            return get_linearized_system_B();
        case 'C':
            return get_linearized_system_C();
        case 'D':
            return get_linearized_system_D();
        default:
            STEPS_SPARSE_MATRIX matrix;
            return matrix;
    }
}

STEPS_SPARSE_MATRIX LOAD_MODEL::get_linearized_system_A() const
{
    if(Aptr!=NULL) return *Aptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to A matrix exists in LOAD_MODEL. Cannot return linearized A matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX LOAD_MODEL::get_linearized_system_B() const
{
    if(Bptr!=NULL) return *Bptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to B matrix exists in LOAD_MODEL. Cannot return linearized B matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX LOAD_MODEL::get_linearized_system_C() const
{
    if(Cptr!=NULL) return *Cptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to C matrix exists in LOAD_MODEL. Cannot return linearized C matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}

STEPS_SPARSE_MATRIX LOAD_MODEL::get_linearized_system_D() const
{
    if(Dptr!=NULL) return *Dptr;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. No pointer to D matrix exists in LOAD_MODEL. Cannot return linearized D matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        STEPS_SPARSE_MATRIX temp;
        return temp;
    }
}
