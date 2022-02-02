
#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"


VSC_HVDC_CONVERTER_MODEL::VSC_HVDC_CONVERTER_MODEL(STEPS& toolkit):VSC_HVDC_MODEL(toolkit)
{
    ;
}

VSC_HVDC_CONVERTER_MODEL::~VSC_HVDC_CONVERTER_MODEL()
{
    ;
}

string VSC_HVDC_CONVERTER_MODEL::get_model_type() const
{
    return "VSC HVDC CONVERTER";
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
