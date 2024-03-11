#include "header/model/energy_storage_model/es_electrical_model/es_electrical_model.h"
#include "header/basic/utility.h"
#include "header/device/energy_storage.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

ES_ELECTRICAL_MODEL::ES_ELECTRICAL_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{
}

ES_ELECTRICAL_MODEL::~ES_ELECTRICAL_MODEL()
{
    ;
}

void ES_ELECTRICAL_MODEL::unbypass_model()
{
    flag_model_bypassed = false;
}

void ES_ELECTRICAL_MODEL::bypass_model()
{
    flag_model_bypassed = true;
}

bool ES_ELECTRICAL_MODEL::is_model_bypassed() const
{
    return flag_model_bypassed;
}

string ES_ELECTRICAL_MODEL::get_model_type() const
{
    return "ES ELECTRICAL";
}

complex<double> ES_ELECTRICAL_MODEL::get_energy_storage_terminal_generation_in_MVA() const
{
    ENERGY_STORAGE* enerage_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* model = enerage_storage->get_es_converter_model();
    if(model!=NULL)
    {
        if(not model->is_model_initialized())
            model->initialize();

        return model->get_terminal_complex_power_in_MVA();
    }
    else
        return 0.0;
}

complex<double> ES_ELECTRICAL_MODEL::get_energy_storage_terminal_generation_in_pu_based_on_mbase() const
{
    return get_energy_storage_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> ES_ELECTRICAL_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double ES_ELECTRICAL_MODEL::get_terminal_bus_voltage_in_pu() const
{
	complex<double> V = get_terminal_bus_complex_voltage_in_pu();
	double x = V.real(), y = V.imag();
	return steps_sqrt(x*x + y * y);
}

double ES_ELECTRICAL_MODEL::get_terminal_bus_frequency_in_pu() const
{
    return get_terminal_bus_frequency_deviation_in_pu()+1.0;
}

double ES_ELECTRICAL_MODEL::get_terminal_bus_frequency_deviation_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_frequency_deviation_in_pu();
}

complex<double> ES_ELECTRICAL_MODEL::get_energy_storage_terminal_complex_current_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* model = energy_storage->get_es_converter_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

double ES_ELECTRICAL_MODEL::get_energy_storage_terminal_current_in_pu() const
{
	complex<double> I =get_energy_storage_terminal_complex_current_in_pu();
	double x = I.real(), y = I.imag();
	return steps_sqrt(x*x+y*y);
}

void ES_ELECTRICAL_MODEL::set_bus_to_regulate(unsigned int bus)
{
    bus_to_regulate = bus;
}

unsigned int ES_ELECTRICAL_MODEL::get_bus_to_regulate()  const
{
    return bus_to_regulate;
}

void ES_ELECTRICAL_MODEL::set_voltage_reference_in_pu(double vref)
{
    voltage_reference_in_pu = vref;
}

void ES_ELECTRICAL_MODEL::set_voltage_reference_in_pu_with_bus_to_regulate()
{
    ENERGY_STORAGE* source = get_energy_storage_pointer();
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int bus = get_bus_to_regulate();
    if(bus==0)
        bus = source->get_source_bus();

    return set_voltage_reference_in_pu(psdb.get_bus_positive_sequence_voltage_in_pu(bus));
}

double ES_ELECTRICAL_MODEL::get_voltage_reference_in_pu() const
{
    return voltage_reference_in_pu;
}

void ES_ELECTRICAL_MODEL::set_frequency_reference_in_pu(double fref)
{
    frequency_reference_in_pu = fref;
}

double ES_ELECTRICAL_MODEL::get_frequency_reference_in_pu() const
{
    return frequency_reference_in_pu;
}

void ES_ELECTRICAL_MODEL::set_active_power_reference_in_pu(double pref)
{
    active_power_reference_in_pu = pref;
}

double ES_ELECTRICAL_MODEL::get_active_power_reference_in_pu() const
{
    return active_power_reference_in_pu;
}

void ES_ELECTRICAL_MODEL::set_reactive_power_reference_in_pu(double qref)
{
    reactive_power_reference_in_pu = qref;
}

double ES_ELECTRICAL_MODEL::get_reactive_power_reference_in_pu() const
{
    return reactive_power_reference_in_pu;
}

void ES_ELECTRICAL_MODEL::set_power_factor_reference_in_pu(double pfref)
{
    power_factor_reference_in_pu = pfref;
}

double ES_ELECTRICAL_MODEL::get_power_factor_reference_in_pu() const
{
    return power_factor_reference_in_pu;
}

void ES_ELECTRICAL_MODEL::set_var_control_mode(PE_VAR_CONTROL_MODE mode)
{
    pe_var_control_mode = mode;
}

PE_VAR_CONTROL_MODE ES_ELECTRICAL_MODEL::get_var_control_mode() const
{
    return pe_var_control_mode;
}
