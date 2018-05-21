#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/device/generator.h"
#include "header/steps_namespace.h"
#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <iostream>

using namespace std;

EXCITER_MODEL::EXCITER_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("GENERATOR");
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
    if(generator!=NULL)
    {
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
            return gen_model->get_initial_excitation_voltage_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;

}

double EXCITER_MODEL::get_compensated_voltage_in_pu() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    GENERATOR* generator = get_generator_pointer();
    if(generator==NULL)
        return 0.0;
    COMPENSATOR_MODEL* comp_model = generator->get_compensator_model();
    if(comp_model==NULL)
    {
        size_t bus = generator->get_generator_bus();
        return psdb->get_bus_voltage_in_pu(bus);
    }
    else
    {
        return comp_model->get_compensated_voltage_in_pu();
    }
}

double EXCITER_MODEL::get_stabilizing_signal_in_pu() const
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
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
