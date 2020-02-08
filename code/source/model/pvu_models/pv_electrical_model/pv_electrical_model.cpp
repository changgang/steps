#include "header/model/pvu_models/pv_electrical_model/pv_electrical_model.h"
#include "header/basic/utility.h"
#include "header/device/pv_unit.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

PV_ELECTRICAL_MODEL::PV_ELECTRICAL_MODEL()
{
}

PV_ELECTRICAL_MODEL::~PV_ELECTRICAL_MODEL()
{
    ;
}

string PV_ELECTRICAL_MODEL::get_model_type() const
{
    return "PV ELECTRICAL";
}

complex<double> PV_ELECTRICAL_MODEL::get_pv_unit_terminal_generation_in_MVA() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        PV_CONVERTER_MODEL* pv_unitmodel = pv_unit->get_pv_converter_model();
        if(pv_unitmodel!=NULL)
        {
            if(not pv_unitmodel->is_model_initialized())
                pv_unitmodel->initialize();

            return pv_unitmodel->get_terminal_complex_power_in_MVA();
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

complex<double> PV_ELECTRICAL_MODEL::get_pv_unit_terminal_generation_in_pu_based_on_mbase() const
{
    return get_pv_unit_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> PV_ELECTRICAL_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int bus = pv_unit->get_unit_bus();
        return psdb.get_bus_positive_sequence_complex_voltage_in_pu(bus);
    }
    else
        return 0.0;
}

double PV_ELECTRICAL_MODEL::get_terminal_bus_voltage_in_pu() const
{
	complex<double> V = get_terminal_bus_complex_voltage_in_pu();
	double x = V.real(), y = V.imag();
	return steps_sqrt(x*x + y * y);
}

double PV_ELECTRICAL_MODEL::get_terminal_bus_frequency_in_pu() const
{
    return get_terminal_bus_frequency_deviation_in_pu()+1.0;
}

double PV_ELECTRICAL_MODEL::get_terminal_bus_frequency_deviation_in_pu() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int bus = pv_unit->get_unit_bus();
        return psdb.get_bus_frequency_deviation_in_pu(bus);
    }
    else
        return 0.0;
}

complex<double> PV_ELECTRICAL_MODEL::get_pv_unit_terminal_complex_current_in_pu() const
{
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    if(pv_unit!=NULL)
    {
        PV_CONVERTER_MODEL* model = pv_unit->get_pv_converter_model();
        if(model!=NULL and model->is_model_initialized())
            return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double PV_ELECTRICAL_MODEL::get_pv_unit_terminal_current_in_pu() const
{
	complex<double> I =get_pv_unit_terminal_complex_current_in_pu();
	double x = I.real(), y = I.imag();
	return steps_sqrt(x*x+y*y);
}
// reference

void PV_ELECTRICAL_MODEL::set_bus_to_regulate(unsigned int bus)
{
    bus_to_regulate = bus;
}

unsigned int PV_ELECTRICAL_MODEL::get_bus_to_regulate()  const
{
    return bus_to_regulate;
}

void PV_ELECTRICAL_MODEL::set_voltage_reference_in_pu(double vref)
{
    voltage_reference_in_pu = vref;
}

void PV_ELECTRICAL_MODEL::set_voltage_reference_in_pu_with_bus_to_regulate()
{
    PV_UNIT* source = get_pv_unit_pointer();
    if(source!=NULL)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int bus = get_bus_to_regulate();
        if(bus==0)
            bus = source->get_source_bus();

        return set_voltage_reference_in_pu(psdb.get_bus_positive_sequence_voltage_in_pu(bus));
    }
}

double PV_ELECTRICAL_MODEL::get_voltage_reference_in_pu() const
{
    return voltage_reference_in_pu;
}

void PV_ELECTRICAL_MODEL::set_frequency_reference_in_pu(double fref)
{
    frequency_reference_in_pu = fref;
}

double PV_ELECTRICAL_MODEL::get_frequency_reference_in_pu() const
{
    return frequency_reference_in_pu;
}

void PV_ELECTRICAL_MODEL::set_active_power_reference_in_pu(double pref)
{
    active_power_reference_in_pu = pref;
}

double PV_ELECTRICAL_MODEL::get_active_power_reference_in_pu() const
{
    return active_power_reference_in_pu;
}

void PV_ELECTRICAL_MODEL::set_reactive_power_reference_in_pu(double qref)
{
    reactive_power_reference_in_pu = qref;
}

double PV_ELECTRICAL_MODEL::get_reactive_power_reference_in_pu() const
{
    return reactive_power_reference_in_pu;
}

void PV_ELECTRICAL_MODEL::set_power_factor_reference_in_pu(double pfref)
{
    power_factor_reference_in_pu = pfref;
}

double PV_ELECTRICAL_MODEL::get_power_factor_reference_in_pu() const
{
    return power_factor_reference_in_pu;
}

void PV_ELECTRICAL_MODEL::set_var_control_mode(PE_VAR_CONTROL_MODE mode)
{
    pe_var_control_mode = mode;
}

PE_VAR_CONTROL_MODE PV_ELECTRICAL_MODEL::get_var_control_mode() const
{
    return pe_var_control_mode;
}
