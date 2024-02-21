#include "header/model/wtg_models/wt_voltage_ride_through_model/wt_voltage_ride_through_model.h"
#include "header/basic/utility.h"
#include "header/device/wt_generator.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

WT_VOLTAGE_RIDE_THROUGH_MODEL::WT_VOLTAGE_RIDE_THROUGH_MODEL(STEPS& toolkit) : WTG_MODEL(toolkit)
{
    ;
}

WT_VOLTAGE_RIDE_THROUGH_MODEL::~WT_VOLTAGE_RIDE_THROUGH_MODEL()
{
    ;
}

string WT_VOLTAGE_RIDE_THROUGH_MODEL::get_model_type() const
{
    return "WT VRT";
}

complex<double> WT_VOLTAGE_RIDE_THROUGH_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double WT_VOLTAGE_RIDE_THROUGH_MODEL::get_terminal_bus_voltage_in_pu() const
{
	complex<double> V = get_terminal_bus_complex_voltage_in_pu();
	double x = V.real(), y = V.imag();
	return steps_sqrt(x*x + y * y);
}

complex<double> WT_VOLTAGE_RIDE_THROUGH_MODEL::get_wt_generator_terminal_generation_in_MVA() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* genmodel = gen->get_wt_generator_model();
    if(genmodel!=NULL)
    {
        if(not genmodel->is_model_initialized())
            genmodel->initialize();

        return genmodel->get_terminal_complex_power_in_MVA();
    }
    else
        return 0.0;
}

complex<double> WT_VOLTAGE_RIDE_THROUGH_MODEL::get_wt_generator_terminal_generation_in_pu_based_on_mbase() const
{
    return get_wt_generator_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> WT_VOLTAGE_RIDE_THROUGH_MODEL::get_wt_generator_terminal_complex_current_in_pu() const
{
    WT_GENERATOR* gen = get_wt_generator_pointer();
    WT_GENERATOR_MODEL* model = gen->get_wt_generator_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

double WT_VOLTAGE_RIDE_THROUGH_MODEL::get_wt_generator_terminal_current_in_pu() const
{
	complex<double> I =get_wt_generator_terminal_complex_current_in_pu();
	double x = I.real(), y = I.imag();
	return steps_sqrt(x*x+y*y);
}

