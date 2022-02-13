#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"
#include "header/device/vsc_hvdc.h"
#include "header/basic/utility.h"


VSC_HVDC_CONVERTER_MODEL::VSC_HVDC_CONVERTER_MODEL(STEPS& toolkit):VSC_HVDC_MODEL(toolkit)
{
    converter_index = INDEX_NOT_EXIST;
    converter_name_index = INDEX_NOT_EXIST;
    set_as_current_source();
}

VSC_HVDC_CONVERTER_MODEL::~VSC_HVDC_CONVERTER_MODEL()
{
    ;
}

string VSC_HVDC_CONVERTER_MODEL::get_model_type() const
{
    return "VSC HVDC CONVERTER";
}

void VSC_HVDC_CONVERTER_MODEL::set_converter_name(string name)
{
    VSC_HVDC* vsc = get_vsc_hvdc_pointer();
    if(vsc!=NULL)
    {
        unsigned int n = vsc->get_converter_count();
        for(unsigned int i=0; i!=n; ++i)
        {
            if(vsc->get_converter_name(i)==name)
            {
                converter_index = i;
                add_string_to_str_int_map(name);
                converter_name_index = get_index_of_string(name);
                break;
            }
        }
    }
}

string VSC_HVDC_CONVERTER_MODEL::get_converter_name() const
{
    return get_string_of_index(get_converter_name_index());
}

unsigned int VSC_HVDC_CONVERTER_MODEL::get_converter_name_index() const
{
    return converter_name_index;
}

unsigned int VSC_HVDC_CONVERTER_MODEL::get_converter_index() const
{
    return converter_index;
}

unsigned int VSC_HVDC_CONVERTER_MODEL::get_converter_bus() const
{
    return get_vsc_hvdc_pointer()->get_converter_ac_bus(get_converter_index());
}

void VSC_HVDC_CONVERTER_MODEL::set_as_voltage_source()
{
    voltage_source_flag = true;
}

void VSC_HVDC_CONVERTER_MODEL::set_as_current_source()
{
    voltage_source_flag = false;
}

bool VSC_HVDC_CONVERTER_MODEL::is_voltage_source() const
{
    return voltage_source_flag;
}


void VSC_HVDC_CONVERTER_MODEL::set_converter_active_control_mode(VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE active_control_mode)
{
    active_power_control_mode = active_control_mode;
}

void VSC_HVDC_CONVERTER_MODEL::set_converter_reactive_control_mode(VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE reactive_control_mode)
{
    reactive_power_control_mode = reactive_control_mode;
}


VSC_HVDC_CONVERTER_ACTIVE_POWER_DYNAMIC_CONTROL_MODE VSC_HVDC_CONVERTER_MODEL::get_converter_active_control_mode() const
{
    return active_power_control_mode;
}

VSC_HVDC_CONVERTER_REACTIVE_POWER_DYNAMIC_CONTROL_MODE VSC_HVDC_CONVERTER_MODEL::get_converter_reactive_control_mode() const
{
    return reactive_power_control_mode;
}

/*
VSC_MODEL_VAR::VSC_MODEL_VAR(STEPS& toolkit):
               active_power_control_block(toolkit),
               reactive_power_control_block(toolkit),
               ac_voltage_control_block(toolkit),
               ud_voltage_control_block(toolkit),
               dc_voltage_block(toolkit)
{
    ;

}
VSC_MODEL_VAR::~VSC_MODEL_VAR()
{

}

VSC_MODEL_VAR::VSC_MODEL_VAR(const VSC_MODEL_VAR& var):
               active_power_control_block(var.active_power_control_block.get_toolkit()),
               reactive_power_control_block(var.reactive_power_control_block.get_toolkit()),
               ac_voltage_control_block(var.ac_voltage_control_block.get_toolkit()),
               ud_voltage_control_block(var.ud_voltage_control_block.get_toolkit()),
               dc_voltage_block(var.dc_voltage_block.get_toolkit())
{
    converter_bus = var.converter_bus;
    converter_busptr = var.converter_busptr;
    active_power_control_mode = var.active_power_control_mode;
    reactive_power_control_mode = var.reactive_power_control_mode;

}
*/
