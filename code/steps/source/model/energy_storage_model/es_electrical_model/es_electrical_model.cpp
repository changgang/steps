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

void ES_ELECTRICAL_MODEL::activate_active_power_control()
{
    flag_active_model_activated = true;
}

void ES_ELECTRICAL_MODEL::deactivate_active_power_control()
{
    flag_active_model_activated =false;
}

bool ES_ELECTRICAL_MODEL::is_active_power_control_activated() const
{
    return flag_active_model_activated;
}

void ES_ELECTRICAL_MODEL::set_active_power_control_mode_in_discharge(unsigned int mode)
{
    this->active_power_control_mode_in_discharge = mode;
}

void ES_ELECTRICAL_MODEL::set_active_power_control_mode_in_charge(unsigned int mode)
{
    this->active_power_control_mode_in_charge = mode;
}

unsigned int ES_ELECTRICAL_MODEL::get_active_power_control_mode_in_discharge() const
{
    return this->active_power_control_mode_in_discharge;
}

unsigned int ES_ELECTRICAL_MODEL::get_active_power_control_mode_in_charge() const
{
    return this->active_power_control_mode_in_charge;
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

double ES_ELECTRICAL_MODEL::get_rotor_speed_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_ROTOR_MODEL* rotor_model = energy_storage->get_es_rotor_model();
    if(rotor_model!=NULL)
    {
        return rotor_model->get_rotor_speed_in_pu();
    }
    else
    {
        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
            return 0.0;
        if(mode==ES_CHARGE_MODE or mode==ES_START_CHARGE_MODE or mode == ES_STOP_CHARGE_MODE)
            return -1.0;
        if(mode==ES_DISCHARGE_MODE or mode==ES_START_DISCHARGE_MODE or mode==ES_STOP_DISCHARGE_MODE)
            return 1.0;

        return 0.0;
    }
}

double ES_ELECTRICAL_MODEL::get_rotor_speed_reference_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_AUXILIARY_MODEL* aux_model = energy_storage->get_es_auxiliary_model();
    if(aux_model!=NULL)
    {
        return aux_model->get_rotor_speed_reference_in_pu();
    }
    else
    {
        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_OUT_SERVICE_MODE)
            return 0.0;
        if(mode==ES_CHARGE_MODE)
            return -1.0;
        if(mode==ES_DISCHARGE_MODE)
            return 1.0;

        return 0.0;
    }
}

double ES_ELECTRICAL_MODEL::get_active_power_reference_in_pu() const
{
    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_AUXILIARY_MODEL* aux_model = energy_storage->get_es_auxiliary_model();
    if(aux_model!=NULL)
        return aux_model->get_active_power_reference_in_pu();
    else
    {
        ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();
        return converter_model->get_initial_active_power_command_in_pu_based_on_mbase();
    }

}
