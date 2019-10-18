#include "header/model/wtg_models/wt_relay_model/wt_relay_model.h"

#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>

WT_RELAY_MODEL::WT_RELAY_MODEL()
{
    common_constructor();
}

WT_RELAY_MODEL::~WT_RELAY_MODEL()
{
    ;
}

void WT_RELAY_MODEL::common_constructor()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");
}

string WT_RELAY_MODEL::get_model_type() const
{
    return "WT RELAY";
}

double WT_RELAY_MODEL::get_wind_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen != NULL)
    {
        WIND_SPEED_MODEL* windmodel = gen->get_wind_speed_model();
        if(windmodel != NULL)
        {
            if(not windmodel->is_model_initialized())
                windmodel->initialize();

            return windmodel->get_wind_speed_in_pu();
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double WT_RELAY_MODEL::get_wt_generator_rotor_speed_in_pu() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator!=NULL)
    {
        WT_TURBINE_MODEL* turbine_model = generator->get_wt_turbine_model();
        if(turbine_model!=NULL)
        {
            if(not turbine_model->is_model_initialized())
                turbine_model->initialize();

            return turbine_model->get_generator_speed_in_pu();
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double WT_RELAY_MODEL::get_bus_frequency_in_pu() const
{
    WT_GENERATOR* generator = get_wt_generator_pointer();
    if(generator!=NULL)
    {
        size_t bus = generator->get_source_bus();
        if(bus!=0)
        {
            STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            return psdb.get_bus_frequency_in_pu(bus);
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}
