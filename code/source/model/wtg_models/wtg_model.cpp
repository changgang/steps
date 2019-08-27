#include "header/model/wtg_models/wtg_model.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
WTG_MODEL::WTG_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");
    busptr = NULL;
}

WTG_MODEL::~WTG_MODEL()
{
    ;
}

WT_GENERATOR* WTG_MODEL::get_wt_generator_pointer() const
{
    return (WT_GENERATOR*) get_device_pointer();
}

void WTG_MODEL::set_bus_pointer()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    WT_GENERATOR* generator = get_wt_generator_pointer();
    size_t bus = generator->get_generator_bus();

    busptr = psdb.get_bus(bus);
    if(busptr==NULL)
    {
        ostringstream osstream;
        osstream<<"Warning. No bus pointer is set for "<<get_model_name()<<" model of "<<generator->get_device_name()<<"\n"
                <<"Check model data.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

BUS* WTG_MODEL::get_bus_pointer() const
{
    return busptr;
}

double WTG_MODEL::get_mbase_in_MVA() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_mbase_in_MVA();
    else
        return 0.0;
}

complex<double> WTG_MODEL::get_terminal_complex_voltage_in_pu()
{
    BUS* busptr =  get_bus_pointer();
    if(busptr!=NULL)
        return busptr->get_complex_voltage_in_pu();
    else
    {
        set_bus_pointer();
        if(get_bus_pointer()!=NULL)
            return get_terminal_complex_voltage_in_pu();
        else
            return 0.0;
    }
}

size_t WTG_MODEL::get_number_of_lumped_wt_generators() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_number_of_lumped_wt_generators();
    else
        return 0;
}

double WTG_MODEL::get_rated_power_per_wt_generator_in_MW() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen!=NULL)
        return gen->get_rated_power_per_wt_generator_in_MW();
    else
        return 0.0;
}

double WTG_MODEL::get_bus_base_frequency_in_Hz()
{
    BUS* busptr =  get_bus_pointer();
    if(busptr!=NULL)
        return busptr->get_base_frequency_in_Hz();
    else
    {
        set_bus_pointer();
        if(get_bus_pointer()!=NULL)
            return get_bus_base_frequency_in_Hz();
        else
            return 0.0;
    }
}

complex<double> WTG_MODEL::get_source_impedance_in_pu_based_on_mbase() const
{
    WT_GENERATOR* wt_generator = get_wt_generator_pointer();
    if(wt_generator!=NULL)
        return wt_generator->get_source_impedance_in_pu();
    else
        return 0.0;
}
