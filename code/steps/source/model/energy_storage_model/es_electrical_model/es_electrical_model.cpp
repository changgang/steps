#include "header/model/energy_storage_model/es_electrical_model/es_electrical_model.h"
#include "header/basic/utility.h"
#include "header/device/energy_storage.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"

ES_ELECTRICAL_MODEL::ES_ELECTRICAL_MODEL(STEPS& toolkit) : ES_MODEL(toolkit)
{
    unbypass_model();
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
	return steps_fast_complex_abs(get_energy_storage_terminal_complex_current_in_pu());
}

