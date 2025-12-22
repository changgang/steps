#include "header/meter/sv_source_meter_getter.h"
#include "header/device/sv_source.h"
#include "header/model/svs_model/svs_control_model/svs_control_model.h"
#include "header/steps_namespace.h"

double sv_source_meter_getter::get_terminal_current_in_pu_sbase(const METER_PARAMETER& para)
{
    SV_SOURCE* svs = (SV_SOURCE*) para.device_pointer;
    SVS_CONTROL_MODEL* model = svs->get_svs_control_model();
    return model ? model->get_terminal_current_in_pu_based_on_sbase() : 0.0;
}

double sv_source_meter_getter::get_terminal_reactive_power_in_pu_sbase(const METER_PARAMETER& para)
{
    SV_SOURCE* svs = (SV_SOURCE*) para.device_pointer;
    SVS_CONTROL_MODEL* model = svs->get_svs_control_model();
    return model ? model->get_terminal_reactive_power_in_pu_based_on_sbase() : 0.0;
}

double sv_source_meter_getter::get_terminal_reactive_power_in_MVar(const METER_PARAMETER& para)
{
    SV_SOURCE* svs = (SV_SOURCE*) para.device_pointer;
    SVS_CONTROL_MODEL* model = svs->get_svs_control_model();
    return model ? model->get_terminal_reactive_power_in_MVar() : 0.0;
}

double sv_source_meter_getter::get_svs_control_model_internal_variable(const METER_PARAMETER& para)
{
    SV_SOURCE* svs = (SV_SOURCE*) para.device_pointer;
    string internal_var_name = para.internal_var_name;
    SVS_CONTROL_MODEL* model = svs->get_svs_control_model();
    return model ? model->get_model_internal_variable_with_name(internal_var_name) : 0.0;
}
