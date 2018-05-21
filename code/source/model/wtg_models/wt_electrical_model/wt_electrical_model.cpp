#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"

#include "header/device/wt_generator.h"
#include "header/power_system_database.h"

WT_ELECTRICAL_MODEL::WT_ELECTRICAL_MODEL()
{
    set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR("WT GENERATOR");
}

WT_ELECTRICAL_MODEL::~WT_ELECTRICAL_MODEL()
{
    ;
}

string WT_ELECTRICAL_MODEL::get_model_type() const
{
    return "WIND ELECTRICAL CONTROL";
}

complex<double> WT_ELECTRICAL_MODEL::get_source_generation_in_MVA() const
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source==NULL)
        return 0.0;

    return source->get_complex_generation_in_MVA();
}

double WT_ELECTRICAL_MODEL::get_source_bus_voltage_in_pu() const
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = source->get_power_system_database();
    if(psdb==NULL)
        return 0.0;

    size_t bus = source->get_source_bus();
    return psdb->get_bus_voltage_in_pu(bus);
}

double WT_ELECTRICAL_MODEL::get_source_bus_frequency_in_pu() const
{
    return get_source_bus_frequency_deviation_in_pu()+1.0;
}

double WT_ELECTRICAL_MODEL::get_source_bus_frequency_deviation_in_pu() const
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = source->get_power_system_database();
    if(psdb==NULL)
        return 0.0;

    size_t bus = source->get_source_bus();
    return psdb->get_bus_frequency_deviation_in_pu(bus);
}

// reference

void WT_ELECTRICAL_MODEL::set_bus_to_regulate(size_t bus)
{
    bus_to_regulate = bus;
}

size_t WT_ELECTRICAL_MODEL::get_bus_to_regulate()  const
{
    return bus_to_regulate;
}

void WT_ELECTRICAL_MODEL::set_voltage_reference_in_pu(double vref)
{
    voltage_reference_in_pu = vref;
}

void WT_ELECTRICAL_MODEL::set_voltage_reference_in_pu_with_bus_to_regulate()
{
    WT_GENERATOR* source = get_wt_generator_pointer();
    if(source==NULL)
        return;

    POWER_SYSTEM_DATABASE* psdb = source->get_power_system_database();
    if(psdb==NULL)
        return;

    size_t bus = get_bus_to_regulate();
    if(bus==0)
        bus = source->get_source_bus();

    return set_voltage_reference_in_pu(psdb->get_bus_voltage_in_pu(bus));
}

double WT_ELECTRICAL_MODEL::get_voltage_reference_in_pu() const
{
    return voltage_reference_in_pu;
}

void WT_ELECTRICAL_MODEL::set_frequency_reference_in_pu(double fref)
{
    frequency_reference_in_pu = fref;
}

double WT_ELECTRICAL_MODEL::get_frequency_reference_in_pu() const
{
    return frequency_reference_in_pu;
}

void WT_ELECTRICAL_MODEL::set_active_power_reference_in_pu(double pref)
{
    active_power_reference_in_pu = pref;
}

double WT_ELECTRICAL_MODEL::get_active_power_reference_in_pu() const
{
    return active_power_reference_in_pu;
}

void WT_ELECTRICAL_MODEL::set_reactive_power_reference_in_pu(double qref)
{
    reactive_power_reference_in_pu = qref;
}

double WT_ELECTRICAL_MODEL::get_reactive_power_reference_in_pu() const
{
    return reactive_power_reference_in_pu;
}

void WT_ELECTRICAL_MODEL::set_power_factor_reference_in_pu(double pfref)
{
    power_factor_reference_in_pu = pfref;
}

double WT_ELECTRICAL_MODEL::get_power_factor_reference_in_pu() const
{
    return power_factor_reference_in_pu;
}

void WT_ELECTRICAL_MODEL::set_var_control_mode(PE_VAR_CONTROL_MODE mode)
{
    pe_var_control_mode = mode;
}

PE_VAR_CONTROL_MODE WT_ELECTRICAL_MODEL::get_var_control_mode() const
{
    return pe_var_control_mode;
}

void WT_ELECTRICAL_MODEL::set_wind_turbine_reference_speed_in_pu(double speed)
{
    turbine_speed_reference_in_pu = speed;
}


complex<double> WT_ELECTRICAL_MODEL::get_complex_voltage_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return 0.0;

    size_t bus = gen->get_generator_bus();
    return psdb->get_bus_complex_voltage_in_pu(bus);
}

complex<double> WT_ELECTRICAL_MODEL::get_complex_terminal_current_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

complex<double> WT_ELECTRICAL_MODEL::get_complex_generation_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    POWER_SYSTEM_DATABASE* psdb = gen->get_power_system_database();
    if(psdb==NULL)
        return 0.0;

    size_t bus = gen->get_generator_bus();
    return psdb->get_bus_complex_voltage_in_pu(bus);
}



double WT_ELECTRICAL_MODEL::get_wind_turbine_generator_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen == NULL)
        return 0.0;

    WT_TURBINE_MODEL* turbinemodel = gen->get_wt_turbine_model();
    if(turbinemodel != NULL and turbinemodel->is_model_initialized())
        return turbinemodel->get_wind_turbine_generator_speed_in_pu();
    else
        return 1.0;
}

double WT_ELECTRICAL_MODEL::get_wind_turbine_generator_speed_referance_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen == NULL)
        return 0.0;

    WT_AERODYNAMIC_MODEL* aero_model = gen->get_wt_aerodynamic_model();
    if(aero_model != NULL)
    {
        if(not aero_model->is_model_initialized())
            aero_model->initialize();
        return aero_model->get_turbine_reference_speed_in_rad_per_s();
    }
    else
        return 0.0;

}

void WT_ELECTRICAL_MODEL::set_wind_turbine_power_speed_lookup_table(WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE table)
{
    power_speed_table = table;
}

WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE WT_ELECTRICAL_MODEL::get_wind_turbine_power_speed_lookup_table() const
{
    return power_speed_table;
}

double WT_ELECTRICAL_MODEL::get_wind_turbine_reference_speed_with_power_in_pu(double power)
{
    return power_speed_table.get_reference_speed_with_power_in_pu(power);
}

double WT_ELECTRICAL_MODEL::get_wind_turbine_reference_power_with_speed_in_pu(double speed)
{
    return power_speed_table.get_reference_power_with_speed_in_pu(speed);
}
