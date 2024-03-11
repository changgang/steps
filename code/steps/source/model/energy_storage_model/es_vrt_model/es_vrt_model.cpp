#include "header/model/energy_storage_model/es_vrt_model/es_vrt_model.h"
#include "header/basic/utility.h"
#include "header/device/energy_storage.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

ES_VRT_MODEL::ES_VRT_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{
    ;
}

ES_VRT_MODEL::~ES_VRT_MODEL()
{
    ;
}

string ES_VRT_MODEL::get_model_type() const
{
    return "ES VRT";
}

complex<double> ES_VRT_MODEL::get_terminal_bus_complex_voltage_in_pu() const
{
    BUS* bus = get_bus_pointer();
    return bus->get_positive_sequence_complex_voltage_in_pu();
}

double ES_VRT_MODEL::get_terminal_bus_voltage_in_pu() const
{
	complex<double> V = get_terminal_bus_complex_voltage_in_pu();
	double x = V.real(), y = V.imag();
	return steps_sqrt(x*x + y * y);
}

complex<double> ES_VRT_MODEL::get_energy_storage_terminal_generation_in_MVA() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* model = energy_storage->get_es_converter_model();
    if(model!=NULL)
    {
        if(not model->is_model_initialized())
            model->initialize();

        return model->get_terminal_complex_power_in_MVA();
    }
    else
        return 0.0;
}

complex<double> ES_VRT_MODEL::get_energy_storage_terminal_generation_in_pu_based_on_mbase() const
{
    return get_energy_storage_terminal_generation_in_MVA()/get_mbase_in_MVA();
}

complex<double> ES_VRT_MODEL::get_energy_storage_terminal_complex_current_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* model = energy_storage->get_es_converter_model();
    if(model!=NULL and model->is_model_initialized())
        return model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    else
        return 0.0;
}

double ES_VRT_MODEL::get_energy_storage_terminal_current_in_pu() const
{
	complex<double> I =get_energy_storage_terminal_complex_current_in_pu();
	double x = I.real(), y = I.imag();
	return steps_sqrt(x*x+y*y);
}
