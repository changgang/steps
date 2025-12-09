#include "header/meter/meter_map_builder.h"
#include "header/meter/wtg_meter_getter.h"
#include "header/steps_namespace.h"

void build_bus_meter_map()
{
}

void build_sg_meter_map()
{
}

void build_wtg_meter_map()
{
    using namespace wtg_meter_getter;
    wtg_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",               get_terminal_current_pu_mbase});
    wtg_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",               get_terminal_current_pu_sbase});
    wtg_meter_map.insert({"TERMINAL CURRENT IN KA",                        get_terminal_current_ka});
    wtg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",          get_terminal_active_power_pu_mbase});
    wtg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",          get_terminal_active_power_pu_sbase});
    wtg_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",                   get_terminal_active_power_mw});
    wtg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",        get_terminal_reactive_power_pu_mbase});
    wtg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",        get_terminal_reactive_power_pu_sbase});
    wtg_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",               get_terminal_reactive_power_mvar});
    wtg_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON MBASE",        get_terminal_apparent_power_pu_mbase});
    wtg_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",        get_terminal_apparent_power_pu_sbase});
    wtg_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",                get_terminal_apparent_power_mva});
    wtg_meter_map.insert({"MECHANICAL POWER IN PU ON MBASE",               get_mechanical_power_pu_mbase});
    wtg_meter_map.insert({"MECHANICAL POWER IN PU ON SBASE",               get_mechanical_power_pu_sbase});
    wtg_meter_map.insert({"MECHANICAL POWER IN MW",                        get_mechanical_power_mw});
    wtg_meter_map.insert({"MAX AVAILABLE MECHANICAL POWER IN PU ON MBASE", get_max_available_mechanical_power_pu_mbase});
    wtg_meter_map.insert({"MAX AVAILABLE MECHANICAL POWER IN PU ON SBASE", get_max_available_mechanical_power_pu_sbase});
    wtg_meter_map.insert({"MAX AVAILABLE MECHANICAL POWER IN MW",          get_max_available_mechanical_power_mw});
    wtg_meter_map.insert({"SPEED REFERENCE IN PU",                         get_speed_reference_pu});
    wtg_meter_map.insert({"SPEED REFERENCE IN RAD/S",                      get_speed_reference_rad_s});
    wtg_meter_map.insert({"TURBINE SPEED DEVIATION IN PU",                 get_turbine_speed_deviation_pu});
    wtg_meter_map.insert({"TURBINE SPEED DEVIATION IN HZ",                 get_turbine_speed_deviation_hz});
    wtg_meter_map.insert({"TURBINE SPEED IN PU",                           get_turbine_speed_pu});
    wtg_meter_map.insert({"TURBINE SPEED IN HZ",                           get_turbine_speed_hz});
    wtg_meter_map.insert({"ROTOR SPEED DEVIATION IN PU",                   get_rotor_speed_deviation_pu});
    wtg_meter_map.insert({"ROTOR SPEED DEVIATION IN HZ",                   get_rotor_speed_deviation_hz});
    wtg_meter_map.insert({"ROTOR SPEED IN PU",                             get_rotor_speed_pu});
    wtg_meter_map.insert({"ROTOR SPEED IN HZ",                             get_rotor_speed_hz});
    wtg_meter_map.insert({"ROTOR ANGLE IN DEG",                            get_rotor_angle_deg});
    wtg_meter_map.insert({"ROTOR ANGLE IN RAD",                            get_rotor_angle_rad});
    wtg_meter_map.insert({"ACTIVE CURRENT COMMAND IN PU",                  get_active_current_command_pu});
    wtg_meter_map.insert({"REACTIVE CURRENT COMMAND IN PU",                get_reactive_current_command_pu});
    wtg_meter_map.insert({"ACTIVE POWER COMMAND IN PU",                    get_active_power_command_pu});
    wtg_meter_map.insert({"REACTIVE POWER COMMAND IN PU",                  get_reactive_power_command_pu});
    wtg_meter_map.insert({"REACTIVE VOLTAGE COMMAND IN PU",                get_reactive_voltage_command_pu});
    wtg_meter_map.insert({"PITCH ANGLE IN DEG",                            get_pitch_angle_deg});
    wtg_meter_map.insert({"WIND SPEED IN PU",                              get_wind_speed_pu});
    wtg_meter_map.insert({"WIND SPEED IN MPS",                             get_wind_speed_mps});
    wtg_meter_map.insert({"BRAKING RESISTOR POWER IN PU ON MBASE",         get_braking_resistor_power_pu});
    wtg_meter_map.insert({"WT GENERATOR MODEL INTERNAL VARIABLE",          get_generator_model_internal_variable});
    wtg_meter_map.insert({"WT AERODYNAMIC MODEL INTERNAL VARIABLE",        get_aerodynamic_model_internal_variable});
    wtg_meter_map.insert({"WT TURBINE MODEL INTERNAL VARIABLE",            get_turbine_model_internal_variable});
    wtg_meter_map.insert({"WT ELECTRICAL MODEL INTERNAL VARIABLE",         get_electrical_model_internal_variable});
    wtg_meter_map.insert({"WT PITCH MODEL INTERNAL VARIABLE",              get_pitch_model_internal_variable});
    wtg_meter_map.insert({"WT VRT MODEL INTERNAL VARIABLE",                get_vrt_model_internal_variable});
    wtg_meter_map.insert({"WT RELAY MODEL INTERNAL VARIABLE",              get_relay_model_internal_variable});
    wtg_meter_map.insert({"WIND SPEED MODEL INTERNAL VARIABLE",            get_windspeed_model_internal_variable});
    wtg_meter_map.insert({"WT DBR MODEL INTERNAL VARIABLE",                get_dbr_model_internal_variable});
}

void build_pvu_meter_map()
{
}

void build_es_meter_map()
{
}

void build_sv_source_meter_map()
{
}

void build_load_meter_map()
{
}

void build_fixed_shunt_meter_map()
{
}

void build_ac_line_meter_map()
{
}

void build_transformer_meter_map()
{
}

void build_lcc_hvdc2t_meter_map()
{
}

void build_vsc_hvdc_meter_map()
{
}

void build_mt_hvdc_meter_map()
{
}

void build_equivalent_device_meter_map()
{
}
