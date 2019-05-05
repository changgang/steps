#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"
#include "header/basic/utility.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

WT_ELECTRICAL_MODEL::WT_ELECTRICAL_MODEL()
{
}

WT_ELECTRICAL_MODEL::~WT_ELECTRICAL_MODEL()
{
    ;
}

string WT_ELECTRICAL_MODEL::get_model_type() const
{
    return "WT ELECTRICAL";
}

complex<double> WT_ELECTRICAL_MODEL::get_wt_generator_terminal_generation_in_MVA() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;
    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel==NULL)
        return 0.0;
    if(not genmodel->is_model_initialized())
        genmodel->initialize();

    return genmodel->get_terminal_complex_power_in_MVA();
}

complex<double> WT_ELECTRICAL_MODEL::get_wt_generator_terminal_generation_in_pu_based_on_mbase() const
{
    return get_wt_generator_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> WT_ELECTRICAL_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t bus = gen->get_generator_bus();
    return psdb.get_bus_complex_voltage_in_pu(bus);
}

double WT_ELECTRICAL_MODEL::get_terminal_bus_voltage_in_pu() const
{
	complex<double> V = get_terminal_bus_complex_voltage_in_pu();
	double x = V.real(), y = V.imag();
	return sqrt(x*x + y * y);
}

double WT_ELECTRICAL_MODEL::get_terminal_bus_frequency_in_pu() const
{
    return get_terminal_bus_frequency_deviation_in_pu()+1.0;
}

double WT_ELECTRICAL_MODEL::get_terminal_bus_frequency_deviation_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen==NULL)
        return 0.0;

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t bus = gen->get_generator_bus();
    return psdb.get_bus_frequency_deviation_in_pu(bus);
}

complex<double> WT_ELECTRICAL_MODEL::get_wt_generator_terminal_complex_current_in_pu() const
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

double WT_ELECTRICAL_MODEL::get_wt_generator_terminal_current_in_pu() const
{
	complex<double> I =get_wt_generator_terminal_complex_current_in_pu();
	double x = I.real(), y = I.imag();
	return sqrt(x*x+y*y);
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

    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t bus = get_bus_to_regulate();
    if(bus==0)
        bus = source->get_source_bus();

    return set_voltage_reference_in_pu(psdb.get_bus_voltage_in_pu(bus));
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

double WT_ELECTRICAL_MODEL::get_wt_generator_speed_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen == NULL)
        return 0.0;

    WT_TURBINE_MODEL* turbinemodel = gen->get_wt_turbine_model();
    if(turbinemodel != NULL and turbinemodel->is_model_initialized())
        return turbinemodel->get_turbine_speed_in_pu();
    else
        return 1.0;
}

double WT_ELECTRICAL_MODEL::get_wt_generator_speed_referance_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    if(gen == NULL)  return 0.0;


	WT_PITCH_MODEL* pitch_model = gen->get_wt_pitch_model();
	if (pitch_model != NULL)
	{
		if (not pitch_model->is_model_initialized())
			pitch_model->initialize();
		if (pitch_model->get_hold_wtg_speed_flag() == true)
			return pitch_model->get_const_wtg_speed_reference_in_pu();
		else
			return pitch_model->get_wt_generator_reference_speed_in_pu();
	}
	else
	{
		WT_AERODYNAMIC_MODEL* aero_model = gen->get_wt_aerodynamic_model();
		if (aero_model != NULL)
		{
			if (not aero_model->is_model_initialized())
				aero_model->initialize();
			return aero_model->get_turbine_reference_speed_in_pu();
		}
		else
			return 0.0;
	}
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
