#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/device/generator.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

COMPENSATOR_MODEL::COMPENSATOR_MODEL()
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        size_t bus = generator->get_generator_bus();
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        return psdb.get_bus_positive_sequence_complex_voltage_in_pu(bus);
    }
    else
        return 0.0;
}

complex<double> COMPENSATOR_MODEL::get_generator_terminal_complex_current_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
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
