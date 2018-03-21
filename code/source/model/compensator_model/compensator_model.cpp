#include "header/model/compensator_model/compensator_model.h"
#include "header/device/generator.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
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

complex<double> COMPENSATOR_MODEL::get_generator_terminal_voltage_in_pu() const
{
    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator==NULL)
        return 0.0;
    else
    {
        size_t bus = generator->get_generator_bus();
        POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
        return psdb->get_bus_complex_voltage_in_pu(bus);
    }
}

complex<double> COMPENSATOR_MODEL::get_generator_terminal_current_in_pu() const
{
    GENERATOR* generator = (GENERATOR*) get_device_pointer();
    if(generator==NULL)
        return 0.0;
    else
    {
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model==NULL)
            return 0.0;
        if(not gen_model->is_model_initialized())
            gen_model->initialize();
        complex<double> I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        return I;
    }
}
