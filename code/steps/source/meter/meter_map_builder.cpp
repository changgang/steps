#include "header/meter/meter_map_builder.h"
#include "header/meter/bus_meter_getter.h"
#include "header/meter/sg_meter_getter.h"
#include "header/meter/wtg_meter_getter.h"
#include "header/meter/pvu_meter_getter.h"
#include "header/meter/es_meter_getter.h"
#include "header/steps_namespace.h"

void build_bus_meter_map()
{
    using namespace bus_meter_getter;
    bus_meter_map.insert({"VOLTAGE IN PU",                     get_voltage_pu});
    bus_meter_map.insert({"VOLTAGE IN KV",                     get_voltage_kV});
    bus_meter_map.insert({"ANGLE IN DEG",                      get_angle_deg});
    bus_meter_map.insert({"FREQUENCY DEVIATION IN PU",         get_frequency_deviation_pu});
    bus_meter_map.insert({"FREQUENCY DEVIATION IN HZ",         get_frequency_deviation_Hz});
    bus_meter_map.insert({"FREQUENCY IN PU",                   get_frequency_pu});
    bus_meter_map.insert({"FREQUENCY IN HZ",                   get_frequency_Hz});
    bus_meter_map.insert({"ROCOF IN HZ/S",                     get_rocof_Hz_per_second});
}

void build_sg_meter_map()
{
    using namespace sg_meter_getter;
    sg_meter_map.insert({"ROTOR ANGLE IN DEG",                              get_rotor_angle_deg});
    sg_meter_map.insert({"ROTOR ANGLE IN RAD",                              get_rotor_angle_rad});
    sg_meter_map.insert({"ROTOR SPEED IN PU",                               get_rotor_speed_pu});
    sg_meter_map.insert({"ROTOR SPEED IN HZ",                               get_rotor_speed_hz});
    sg_meter_map.insert({"ROTOR SPEED DEVIATION IN PU",                     get_rotor_speed_deviation_pu});
    sg_meter_map.insert({"ROTOR SPEED DEVIATION IN HZ",                     get_rotor_speed_deviation_hz});
    sg_meter_map.insert({"TERMINAL VOLTAGE IN PU",                          get_terminal_voltage_pu});
    sg_meter_map.insert({"INTERNAL VOLTAGE IN PU",                          get_internal_voltage_pu});
    sg_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",                 get_terminal_current_pu_mbase});
    sg_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",                 get_terminal_current_pu_sbase});
    sg_meter_map.insert({"TERMINAL CURRENT IN KA",                          get_terminal_current_ka});
    sg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",            get_terminal_active_power_pu_mbase});
    sg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",            get_terminal_active_power_pu_sbase});
    sg_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",                     get_terminal_active_power_mw});
    sg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",          get_terminal_reactive_power_pu_mbase});
    sg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",          get_terminal_reactive_power_pu_sbase});
    sg_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",                 get_terminal_reactive_power_mvar});
    sg_meter_map.insert({"TERMINAL APPRRENT POWER IN PU ON MBASE",          get_terminal_apparent_power_pu_mbase});
    sg_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",          get_terminal_apparent_power_pu_sbase});
    sg_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",                  get_terminal_apparent_power_mva});
    sg_meter_map.insert({"AIRGAP POWER IN PU ON MBASE",                     get_airgap_power_pu_mbase});
    sg_meter_map.insert({"AIRGAP POWER IN PU ON SBASE",                     get_airgap_power_pu_sbase});
    sg_meter_map.insert({"AIRGAP POWER IN MW",                              get_airgap_power_mw});
    sg_meter_map.insert({"ACCELERATING POWER IN PU ON MBASE",               get_accelerating_power_pu_mbase});
    sg_meter_map.insert({"ACCELERATING POWER IN PU ON SBASE",               get_accelerating_power_pu_sbase});
    sg_meter_map.insert({"ACCELERATING POWER IN MW",                        get_accelerating_power_mw});
    sg_meter_map.insert({"MECHANICAL POWER IN PU ON MBASE",                 get_mechanical_power_pu_mbase});
    sg_meter_map.insert({"MECHANICAL POWER IN PU ON SBASE",                 get_mechanical_power_pu_sbase});
    sg_meter_map.insert({"MECHANICAL POWER IN MW",                          get_mechanical_power_mw});
    sg_meter_map.insert({"MECHANICAL POWER REFERENCE IN PU ON MBASE",       get_mechanical_power_reference_pu_mbase});
    sg_meter_map.insert({"MECHANICAL POWER REFERENCE IN PU ON SBASE",       get_mechanical_power_reference_pu_sbase});
    sg_meter_map.insert({"MECHANICAL POWER REFERENCE IN MW",                get_mechanical_power_reference_mw});
    sg_meter_map.insert({"COMPENSATED VOLTAGE IN PU",                       get_compensated_voltage_pu});
    sg_meter_map.insert({"VOLTAGE REFERENCE IN PU",                         get_voltage_reference_pu});
    sg_meter_map.insert({"STABILIZING SIGNAL IN PU",                        get_stabilizing_signal_pu});
    sg_meter_map.insert({"EXCITATION VOLTAGE IN PU",                        get_excitation_voltage_pu});
    sg_meter_map.insert({"SYNC GENERATOR MODEL INTERNAL VARIABLE",          get_sync_generator_model_internal_variable});
    sg_meter_map.insert({"COMPENSATOR MODEL INTERNAL VARIABLE",             get_compensator_model_internal_variable});
    sg_meter_map.insert({"STABILIZER MODEL INTERNAL VARIABLE",              get_stabilizer_model_internal_variable});
    sg_meter_map.insert({"EXCITER MODEL INTERNAL VARIABLE",                 get_exciter_model_internal_variable});
    sg_meter_map.insert({"TURBINE GOVERNOR MODEL INTERNAL VARIABLE",        get_turbine_governor_model_internal_variable});
    sg_meter_map.insert({"TURBINE LOAD CONTROLLER MODEL INTERNAL VARIABLE", get_turbine_load_controller_model_internal_variable});
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
    using namespace pvu_meter_getter;
    pvu_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",                        get_terminal_current_pu_mbase});
    pvu_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",                        get_terminal_current_pu_sbase});
    pvu_meter_map.insert({"TERMINAL CURRENT IN KA",                                 get_terminal_current_ka});

    pvu_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",                   get_terminal_active_power_pu_mbase});
    pvu_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",                   get_terminal_active_power_pu_sbase});
    pvu_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",                            get_terminal_active_power_mw});

    pvu_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",                 get_terminal_reactive_power_pu_mbase});
    pvu_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",                 get_terminal_reactive_power_pu_sbase});
    pvu_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",                        get_terminal_reactive_power_mvar});

    pvu_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON MBASE",                 get_terminal_apparent_power_pu_mbase});
    pvu_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",                 get_terminal_apparent_power_pu_sbase});
    pvu_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",                         get_terminal_apparent_power_mva});

    pvu_meter_map.insert({"ACTIVE CURRENT COMMAND IN PU",                           get_active_current_command_pu});
    pvu_meter_map.insert({"REACTIVE CURRENT COMMAND IN PU",                         get_reactive_current_command_pu});
    pvu_meter_map.insert({"ACTIVE POWER COMMAND IN PU",                             get_active_power_command_pu});
    pvu_meter_map.insert({"REACTIVE POWER COMMAND IN PU",                           get_reactive_power_command_pu});
    pvu_meter_map.insert({"REACTIVE VOLTAGE COMMAND IN PU",                         get_reactive_voltage_command_pu});

    pvu_meter_map.insert({"SOLAR IRRADIANCE IN PU ON S0",                           get_solar_irradiance_pu_S0});
    pvu_meter_map.insert({"SOLAR IRRADIANCE IN PU ON SREF",                         get_solar_irradiance_pu_Sref});
    pvu_meter_map.insert({"SOLAR IRRADIANCE IN W/M2",                               get_solar_irradiance_wpm2});

    pvu_meter_map.insert({"TEMPERATURE IN C DEG",                                   get_temperature_c_deg});

    pvu_meter_map.insert({"MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN PU ON MBASE", get_max_available_power_irradiance_pu_mbase});
    pvu_meter_map.insert({"MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN PU ON SBASE", get_max_available_power_irradiance_pu_sbase});
    pvu_meter_map.insert({"MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN MW",          get_max_available_power_irradiance_mw});

    pvu_meter_map.insert({"PANEL POWER IN PU ON MBASE",                             get_panel_power_pu_mbase});
    pvu_meter_map.insert({"PANEL POWER IN MW",                                      get_panel_power_mw});

    pvu_meter_map.insert({"PV CONVERTER MODEL INTERNAL VARIABLE",                   get_converter_model_internal_variable});
    pvu_meter_map.insert({"PV PANEL MODEL INTERNAL VARIABLE",                       get_panel_model_internal_variable});
    pvu_meter_map.insert({"PV ELECTRICAL MODEL INTERNAL VARIABLE",                  get_electrical_model_internal_variable});
    pvu_meter_map.insert({"PV IRRADIANCE MODEL INTERNAL VARIABLE",                  get_irradiance_model_internal_variable});
    pvu_meter_map.insert({"PV TEMPERATURE MODEL INTERNAL VARIABLE",                 get_temperature_model_internal_variable});
    pvu_meter_map.insert({"PV VRT MODEL INTERNAL VARIABLE",                         get_vrt_model_internal_variable});
    pvu_meter_map.insert({"PV RELAY MODEL INTERNAL VARIABLE",                       get_relay_model_internal_variable});
}

void build_es_meter_map()
{
    using namespace es_meter_getter;
    es_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",           get_terminal_current_pu_mbase});
    es_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",           get_terminal_current_pu_sbase});
    es_meter_map.insert({"TERMINAL CURRENT IN KA",                    get_terminal_current_ka});
    es_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",      get_terminal_active_power_pu_mbase});
    es_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",      get_terminal_active_power_pu_sbase});
    es_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",               get_terminal_active_power_mw});
    es_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",    get_terminal_reactive_power_pu_mbase});
    es_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",    get_terminal_reactive_power_pu_sbase});
    es_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",           get_terminal_reactive_power_mvar});
    es_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON MBASE",    get_terminal_apparent_power_pu_mbase});
    es_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",    get_terminal_apparent_power_pu_sbase});
    es_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",            get_terminal_apparent_power_mva});
    es_meter_map.insert({"ACTIVE CURRENT COMMAND IN PU",              get_active_current_command_pu});
    es_meter_map.insert({"REACTIVE CURRENT COMMAND IN PU",            get_reactive_current_command_pu});
    es_meter_map.insert({"ACTIVE POWER COMMAND IN PU",                get_active_power_command_pu});
    es_meter_map.insert({"REACTIVE POWER COMMAND IN PU",              get_reactive_power_command_pu});
    es_meter_map.insert({"REACTIVE VOLTAGE COMMAND IN PU",            get_reactive_voltage_command_pu});
    es_meter_map.insert({"STATE OF CHARGE IN PU",                     get_state_of_charge_pu});
    es_meter_map.insert({"SOC POWER IN PU ON MBASE",                  get_soc_power_pu_mbase});
    es_meter_map.insert({"SOC POWER IN PU ON SBASE",                  get_soc_power_pu_sbase});
    es_meter_map.insert({"SOC POWER IN MW",                           get_soc_power_mw});
    es_meter_map.insert({"MECHANICAL POWER IN PU ON MBASE",           get_mechanical_power_pu_mbase});
    es_meter_map.insert({"ROTOR SPEED IN PU",                         get_rotor_speed_pu});
    es_meter_map.insert({"ROTOR SPEED REFERENCE IN PU",               get_rotor_speed_reference_pu});
    es_meter_map.insert({"VALVE REFERENCE IN PU",                     get_valve_reference_pu});
    es_meter_map.insert({"VALVE IN PU",                               get_valve_pu});
    es_meter_map.insert({"ACTIVE POWER REFERENCE IN PU ON MBASE",     get_active_power_reference_pu_mbase});
    es_meter_map.insert({"FREQUENCY REGULATION POWER IN PU ON MBASE", get_frequency_regulation_power_pu_mbase});
    es_meter_map.insert({"ES CONVERTER MODEL INTERNAL VARIABLE",      get_converter_model_internal_variable});
    es_meter_map.insert({"ES SOC MODEL INTERNAL VARIABLE",            get_soc_model_internal_variable});
    es_meter_map.insert({"ES ELECTRICAL MODEL INTERNAL VARIABLE",     get_electrical_model_internal_variable});
    es_meter_map.insert({"ES VRT MODEL INTERNAL VARIABLE",            get_vrt_model_internal_variable});
    es_meter_map.insert({"ES RELAY MODEL INTERNAL VARIABLE",          get_relay_model_internal_variable});
    es_meter_map.insert({"ES PRIMARY MODEL INTERNAL VARIABLE",        get_primary_model_internal_variable});
    es_meter_map.insert({"ES ROTOR MODEL INTERNAL VARIABLE",          get_rotor_model_internal_variable});
    es_meter_map.insert({"ES AUXILIARY MODEL INTERNAL VARIABLE",      get_auxiliary_model_internal_variable});
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
