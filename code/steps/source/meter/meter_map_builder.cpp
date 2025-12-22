#include "header/meter/meter_map_builder.h"

#include "header/steps_namespace.h"

#include "header/meter/bus_meter_getter.h"
#include "header/meter/sg_meter_getter.h"
#include "header/meter/wtg_meter_getter.h"
#include "header/meter/pvu_meter_getter.h"
#include "header/meter/es_meter_getter.h"
#include "header/meter/sv_source_meter_getter.h"
#include "header/meter/load_meter_getter.h"
#include "header/meter/ac_line_meter_getter.h"
#include "header/meter/transformer_meter_getter.h"
#include "header/meter/lcc_hvdc2t_meter_getter.h"
#include "header/meter/vsc_hvdc_meter_getter.h"
#include "header/meter/mt_hvdc_meter_getter.h"
#include "header/meter/equivalent_device_meter_getter.h"

void build_bus_meter_map()
{
    using namespace bus_meter_getter;
    bus_meter_map.insert({"VOLTAGE IN PU",                     get_voltage_in_pu});
    bus_meter_map.insert({"VOLTAGE IN KV",                     get_voltage_in_kV});
    bus_meter_map.insert({"ANGLE IN DEG",                      get_angle_in_deg});
    bus_meter_map.insert({"FREQUENCY DEVIATION IN PU",         get_frequency_deviation_in_pu});
    bus_meter_map.insert({"FREQUENCY DEVIATION IN HZ",         get_frequency_deviation_in_Hz});
    bus_meter_map.insert({"FREQUENCY IN PU",                   get_frequency_in_pu});
    bus_meter_map.insert({"FREQUENCY IN HZ",                   get_frequency_in_Hz});
    bus_meter_map.insert({"ROCOF IN HZ/S",                     get_rocof_in_Hz_per_second});
}

void build_sg_meter_map()
{
    using namespace sg_meter_getter;
    sg_meter_map.insert({"ROTOR ANGLE IN DEG",                              get_rotor_angle_in_deg});
    sg_meter_map.insert({"ROTOR ANGLE IN RAD",                              get_rotor_angle_in_rad});
    sg_meter_map.insert({"ROTOR SPEED IN PU",                               get_rotor_speed_in_pu});
    sg_meter_map.insert({"ROTOR SPEED IN HZ",                               get_rotor_speed_in_Hz});
    sg_meter_map.insert({"ROTOR SPEED DEVIATION IN PU",                     get_rotor_speed_deviation_in_pu});
    sg_meter_map.insert({"ROTOR SPEED DEVIATION IN HZ",                     get_rotor_speed_deviation_in_Hz});
    sg_meter_map.insert({"TERMINAL VOLTAGE IN PU",                          get_terminal_voltage_in_pu});
    sg_meter_map.insert({"INTERNAL VOLTAGE IN PU",                          get_internal_voltage_in_pu});
    sg_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",                 get_terminal_current_in_pu_mbase});
    sg_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",                 get_terminal_current_in_pu_sbase});
    sg_meter_map.insert({"TERMINAL CURRENT IN KA",                          get_terminal_current_in_kA});
    sg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",            get_terminal_active_power_in_pu_mbase});
    sg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",            get_terminal_active_power_in_pu_sbase});
    sg_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",                     get_terminal_active_power_in_MW});
    sg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",          get_terminal_reactive_power_in_pu_mbase});
    sg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",          get_terminal_reactive_power_in_pu_sbase});
    sg_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",                 get_terminal_reactive_power_in_MVar});
    sg_meter_map.insert({"TERMINAL APPRRENT POWER IN PU ON MBASE",          get_terminal_apparent_power_in_pu_mbase});
    sg_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",          get_terminal_apparent_power_in_pu_sbase});
    sg_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",                  get_terminal_apparent_power_in_MVA});
    sg_meter_map.insert({"AIRGAP POWER IN PU ON MBASE",                     get_airgap_power_in_pu_mbase});
    sg_meter_map.insert({"AIRGAP POWER IN PU ON SBASE",                     get_airgap_power_in_pu_sbase});
    sg_meter_map.insert({"AIRGAP POWER IN MW",                              get_airgap_power_in_MW});
    sg_meter_map.insert({"ACCELERATING POWER IN PU ON MBASE",               get_accelerating_power_in_pu_mbase});
    sg_meter_map.insert({"ACCELERATING POWER IN PU ON SBASE",               get_accelerating_power_in_pu_sbase});
    sg_meter_map.insert({"ACCELERATING POWER IN MW",                        get_accelerating_power_in_MW});
    sg_meter_map.insert({"MECHANICAL POWER IN PU ON MBASE",                 get_mechanical_power_in_pu_mbase});
    sg_meter_map.insert({"MECHANICAL POWER IN PU ON SBASE",                 get_mechanical_power_in_pu_sbase});
    sg_meter_map.insert({"MECHANICAL POWER IN MW",                          get_mechanical_power_in_MW});
    sg_meter_map.insert({"MECHANICAL POWER REFERENCE IN PU ON MBASE",       get_mechanical_power_reference_in_pu_mbase});
    sg_meter_map.insert({"MECHANICAL POWER REFERENCE IN PU ON SBASE",       get_mechanical_power_reference_in_pu_sbase});
    sg_meter_map.insert({"MECHANICAL POWER REFERENCE IN MW",                get_mechanical_power_reference_in_MW});
    sg_meter_map.insert({"COMPENSATED VOLTAGE IN PU",                       get_compensated_voltage_in_pu});
    sg_meter_map.insert({"VOLTAGE REFERENCE IN PU",                         get_voltage_reference_in_pu});
    sg_meter_map.insert({"STABILIZING SIGNAL IN PU",                        get_stabilizing_signal_in_pu});
    sg_meter_map.insert({"EXCITATION VOLTAGE IN PU",                        get_excitation_voltage_in_pu});
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
    wtg_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",               get_terminal_current_in_pu_mbase});
    wtg_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",               get_terminal_current_in_pu_sbase});
    wtg_meter_map.insert({"TERMINAL CURRENT IN KA",                        get_terminal_current_in_kA});
    wtg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",          get_terminal_active_power_in_pu_mbase});
    wtg_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",          get_terminal_active_power_in_pu_sbase});
    wtg_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",                   get_terminal_active_power_in_MW});
    wtg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",        get_terminal_reactive_power_in_pu_mbase});
    wtg_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",        get_terminal_reactive_power_in_pu_sbase});
    wtg_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",               get_terminal_reactive_power_in_MVar});
    wtg_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON MBASE",        get_terminal_apparent_power_in_pu_mbase});
    wtg_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",        get_terminal_apparent_power_in_pu_sbase});
    wtg_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",                get_terminal_apparent_power_in_MVA});
    wtg_meter_map.insert({"MECHANICAL POWER IN PU ON MBASE",               get_mechanical_power_in_pu_mbase});
    wtg_meter_map.insert({"MECHANICAL POWER IN PU ON SBASE",               get_mechanical_power_in_pu_sbase});
    wtg_meter_map.insert({"MECHANICAL POWER IN MW",                        get_mechanical_power_in_MW});
    wtg_meter_map.insert({"MAX AVAILABLE MECHANICAL POWER IN PU ON MBASE", get_max_available_mechanical_power_in_pu_mbase});
    wtg_meter_map.insert({"MAX AVAILABLE MECHANICAL POWER IN PU ON SBASE", get_max_available_mechanical_power_in_pu_sbase});
    wtg_meter_map.insert({"MAX AVAILABLE MECHANICAL POWER IN MW",          get_max_available_mechanical_power_in_MW});
    wtg_meter_map.insert({"SPEED REFERENCE IN PU",                         get_speed_reference_in_pu});
    wtg_meter_map.insert({"SPEED REFERENCE IN RAD/S",                      get_speed_reference_in_rad_s});
    wtg_meter_map.insert({"TURBINE SPEED DEVIATION IN PU",                 get_turbine_speed_deviation_in_pu});
    wtg_meter_map.insert({"TURBINE SPEED DEVIATION IN HZ",                 get_turbine_speed_deviation_in_Hz});
    wtg_meter_map.insert({"TURBINE SPEED IN PU",                           get_turbine_speed_in_pu});
    wtg_meter_map.insert({"TURBINE SPEED IN HZ",                           get_turbine_speed_in_Hz});
    wtg_meter_map.insert({"ROTOR SPEED DEVIATION IN PU",                   get_rotor_speed_deviation_in_pu});
    wtg_meter_map.insert({"ROTOR SPEED DEVIATION IN HZ",                   get_rotor_speed_deviation_in_Hz});
    wtg_meter_map.insert({"ROTOR SPEED IN PU",                             get_rotor_speed_in_pu});
    wtg_meter_map.insert({"ROTOR SPEED IN HZ",                             get_rotor_speed_in_Hz});
    wtg_meter_map.insert({"ROTOR ANGLE IN DEG",                            get_rotor_angle_in_deg});
    wtg_meter_map.insert({"ROTOR ANGLE IN RAD",                            get_rotor_angle_in_rad});
    wtg_meter_map.insert({"ACTIVE CURRENT COMMAND IN PU",                  get_active_current_command_in_pu});
    wtg_meter_map.insert({"REACTIVE CURRENT COMMAND IN PU",                get_reactive_current_command_in_pu});
    wtg_meter_map.insert({"ACTIVE POWER COMMAND IN PU",                    get_active_power_command_in_pu});
    wtg_meter_map.insert({"REACTIVE POWER COMMAND IN PU",                  get_reactive_power_command_in_pu});
    wtg_meter_map.insert({"REACTIVE VOLTAGE COMMAND IN PU",                get_reactive_voltage_command_in_pu});
    wtg_meter_map.insert({"PITCH ANGLE IN DEG",                            get_pitch_angle_in_deg});
    wtg_meter_map.insert({"WIND SPEED IN PU",                              get_wind_speed_in_pu});
    wtg_meter_map.insert({"WIND SPEED IN MPS",                             get_wind_speed_mps});
    wtg_meter_map.insert({"BRAKING RESISTOR POWER IN PU ON MBASE",         get_braking_resistor_power_in_pu});
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
    pvu_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",                        get_terminal_current_in_pu_mbase});
    pvu_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",                        get_terminal_current_in_pu_sbase});
    pvu_meter_map.insert({"TERMINAL CURRENT IN KA",                                 get_terminal_current_in_kA});

    pvu_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",                   get_terminal_active_power_in_pu_mbase});
    pvu_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",                   get_terminal_active_power_in_pu_sbase});
    pvu_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",                            get_terminal_active_power_in_MW});

    pvu_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",                 get_terminal_reactive_power_in_pu_mbase});
    pvu_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",                 get_terminal_reactive_power_in_pu_sbase});
    pvu_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",                        get_terminal_reactive_power_in_MVar});

    pvu_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON MBASE",                 get_terminal_apparent_power_in_pu_mbase});
    pvu_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",                 get_terminal_apparent_power_in_pu_sbase});
    pvu_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",                         get_terminal_apparent_power_in_MVA});

    pvu_meter_map.insert({"ACTIVE CURRENT COMMAND IN PU",                           get_active_current_command_in_pu});
    pvu_meter_map.insert({"REACTIVE CURRENT COMMAND IN PU",                         get_reactive_current_command_in_pu});
    pvu_meter_map.insert({"ACTIVE POWER COMMAND IN PU",                             get_active_power_command_in_pu});
    pvu_meter_map.insert({"REACTIVE POWER COMMAND IN PU",                           get_reactive_power_command_in_pu});
    pvu_meter_map.insert({"REACTIVE VOLTAGE COMMAND IN PU",                         get_reactive_voltage_command_in_pu});

    pvu_meter_map.insert({"SOLAR IRRADIANCE IN PU ON S0",                           get_solar_irradiance_pu_S0});
    pvu_meter_map.insert({"SOLAR IRRADIANCE IN PU ON SREF",                         get_solar_irradiance_pu_Sref});
    pvu_meter_map.insert({"SOLAR IRRADIANCE IN W/M2",                               get_solar_irradiance_wpm2});

    pvu_meter_map.insert({"TEMPERATURE IN C DEG",                                   get_temperature_c_deg});

    pvu_meter_map.insert({"MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN PU ON MBASE", get_max_available_power_irradiance_pu_mbase});
    pvu_meter_map.insert({"MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN PU ON SBASE", get_max_available_power_irradiance_pu_sbase});
    pvu_meter_map.insert({"MAXIMUM AVAILABLE POWER FROM IRRADIANCE IN MW",          get_max_available_power_irradiance_MW});

    pvu_meter_map.insert({"PANEL POWER IN PU ON MBASE",                             get_panel_power_in_pu_mbase});
    pvu_meter_map.insert({"PANEL POWER IN MW",                                      get_panel_power_in_MW});

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
    es_meter_map.insert({"TERMINAL CURRENT IN PU ON MBASE",           get_terminal_current_in_pu_mbase});
    es_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",           get_terminal_current_in_pu_sbase});
    es_meter_map.insert({"TERMINAL CURRENT IN KA",                    get_terminal_current_in_kA});
    es_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON MBASE",      get_terminal_active_power_in_pu_mbase});
    es_meter_map.insert({"TERMINAL ACTIVE POWER IN PU ON SBASE",      get_terminal_active_power_in_pu_sbase});
    es_meter_map.insert({"TERMINAL ACTIVE POWER IN MW",               get_terminal_active_power_in_MW});
    es_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON MBASE",    get_terminal_reactive_power_in_pu_mbase});
    es_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE",    get_terminal_reactive_power_in_pu_sbase});
    es_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",           get_terminal_reactive_power_in_MVar});
    es_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON MBASE",    get_terminal_apparent_power_in_pu_mbase});
    es_meter_map.insert({"TERMINAL APPARENT POWER IN PU ON SBASE",    get_terminal_apparent_power_in_pu_sbase});
    es_meter_map.insert({"TERMINAL APPARENT POWER IN MVA",            get_terminal_apparent_power_in_MVA});
    es_meter_map.insert({"ACTIVE CURRENT COMMAND IN PU",              get_active_current_command_in_pu});
    es_meter_map.insert({"REACTIVE CURRENT COMMAND IN PU",            get_reactive_current_command_in_pu});
    es_meter_map.insert({"ACTIVE POWER COMMAND IN PU",                get_active_power_command_in_pu});
    es_meter_map.insert({"REACTIVE POWER COMMAND IN PU",              get_reactive_power_command_in_pu});
    es_meter_map.insert({"REACTIVE VOLTAGE COMMAND IN PU",            get_reactive_voltage_command_in_pu});
    es_meter_map.insert({"STATE OF CHARGE IN PU",                     get_state_of_charge_in_pu});
    es_meter_map.insert({"SOC POWER IN PU ON MBASE",                  get_soc_power_in_pu_mbase});
    es_meter_map.insert({"SOC POWER IN PU ON SBASE",                  get_soc_power_in_pu_sbase});
    es_meter_map.insert({"SOC POWER IN MW",                           get_soc_power_in_MW});
    es_meter_map.insert({"MECHANICAL POWER IN PU ON MBASE",           get_mechanical_power_in_pu_mbase});
    es_meter_map.insert({"ROTOR SPEED IN PU",                         get_rotor_speed_in_pu});
    es_meter_map.insert({"ROTOR SPEED REFERENCE IN PU",               get_rotor_speed_reference_in_pu});
    es_meter_map.insert({"VALVE REFERENCE IN PU",                     get_valve_reference_in_pu});
    es_meter_map.insert({"VALVE IN PU",                               get_valve_in_pu});
    es_meter_map.insert({"ACTIVE POWER REFERENCE IN PU ON MBASE",     get_active_power_reference_in_pu_mbase});
    es_meter_map.insert({"FREQUENCY REGULATION POWER IN PU ON MBASE", get_frequency_regulation_power_in_pu_mbase});
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
    using namespace sv_source_meter_getter;
    sv_source_meter_map.insert({"TERMINAL CURRENT IN PU ON SBASE",        get_terminal_current_in_pu_sbase});
    sv_source_meter_map.insert({"TERMINAL REACTIVE POWER IN PU ON SBASE", get_terminal_reactive_power_in_pu_sbase});
    sv_source_meter_map.insert({"TERMINAL REACTIVE POWER IN MVAR",        get_terminal_reactive_power_in_MVar});
    sv_source_meter_map.insert({"SVS CONTROL MODEL INTERNAL VARIABLE",    get_svs_control_model_internal_variable});
}

void build_load_meter_map()
{
    using namespace load_meter_getter;
    load_meter_map.insert({"ACTIVE POWER IN MW",                      get_active_power_in_MW});
    load_meter_map.insert({"ACTIVE POWER IN PU",                      get_active_power_in_pu});
    load_meter_map.insert({"REACTIVE POWER IN MVAR",                  get_reactive_power_in_MVar});
    load_meter_map.insert({"REACTIVE POWER IN PU",                    get_reactive_power_in_pu});
    load_meter_map.insert({"CURRENT IN KA",                           get_current_in_kA});
    load_meter_map.insert({"CURRENT IN PU",                           get_current_in_pu});
    load_meter_map.insert({"TOTAL SCALE IN PU",                       get_total_scale_in_pu});
    load_meter_map.insert({"MANUALLY SCALE IN PU",                    get_manually_scale_in_pu});
    load_meter_map.insert({"RELAY SHED SCALE IN PU",                  get_relay_shed_scale_in_pu});
    load_meter_map.insert({"LOAD MODEL INTERNAL VARIABLE",            get_load_model_internal_variable});
    load_meter_map.insert({"FREQUENCY RELAY MODEL INTERNAL VARIABLE", get_frequency_relay_model_internal_variable});
    load_meter_map.insert({"VOLTAGE RELAY MODEL INTERNAL VARIABLE",   get_voltage_relay_model_internal_variable});
}

void build_fixed_shunt_meter_map()
{
}

void build_ac_line_meter_map()
{
    using namespace ac_line_meter_getter;
    ac_line_meter_map.insert({"CURRENT IN KA",                                     get_current_in_kA});
    ac_line_meter_map.insert({"CURRENT IN PU",                                     get_current_in_pu});
    ac_line_meter_map.insert({"ACTIVE POWER IN MW",                                get_active_power_in_MW});
    ac_line_meter_map.insert({"ACTIVE POWER IN PU",                                get_active_power_in_pu});
    ac_line_meter_map.insert({"REACTIVE POWER IN MVAR",                            get_reactive_power_in_MVar});
    ac_line_meter_map.insert({"REACTIVE POWER IN PU",                              get_reactive_power_in_pu});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE IN OHM",                         get_apparent_impedance_in_ohm});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE IN PU",                          get_apparent_impedance_in_pu});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE ANGLE IN DEG",                   get_apparent_impedance_angle_in_deg});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE ANGLE IN RAD",                   get_apparent_impedance_angle_in_rad});
    ac_line_meter_map.insert({"CURRENT AT SENDING SIDE IN KA",                     get_current_at_sending_side_in_kA});
    ac_line_meter_map.insert({"ACTIVE POWER AT SENDING SIDE IN MW",                get_active_power_at_sending_side_in_MW});
    ac_line_meter_map.insert({"REACTIVE POWER AT SENDING SIDE IN MVAR",            get_reactive_power_at_sending_side_in_MVar});
    ac_line_meter_map.insert({"CURRENT AT RECEIVING SIDE IN KA",                   get_current_at_receiving_side_in_kA});
    ac_line_meter_map.insert({"ACTIVE POWER AT RECEIVING SIDE IN MW",              get_active_power_at_receiving_side_in_MW});
    ac_line_meter_map.insert({"REACTIVE POWER AT RECEIVING SIDE IN MVAR",          get_reactive_power_at_receiving_side_in_MVar});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE AT SENDING SIDE IN OHM",         get_apparent_impedance_at_sending_side_in_ohm});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE ANGLE AT SENDING SIDE IN DEG",   get_apparent_impedance_angle_at_sending_side_in_deg});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE AT RECEIVING SIDE IN OHM",       get_apparent_impedance_at_receiving_side_in_ohm});
    ac_line_meter_map.insert({"APPARENT IMPEDANCE ANGLE AT RECEIVING SIDE IN DEG", get_apparent_impedance_angle_at_receiving_side_in_deg});
}

void build_transformer_meter_map()
{
    using namespace transformer_meter_getter;
    transformer_meter_map.insert({"CURRENT IN KA",                               get_winding_current_in_kA});
    transformer_meter_map.insert({"CURRENT IN PU",                               get_winding_current_in_pu});
    transformer_meter_map.insert({"ACTIVE POWER IN MW",                          get_winding_active_power_in_MW});
    transformer_meter_map.insert({"ACTIVE POWER IN PU",                          get_winding_active_power_in_pu});
    transformer_meter_map.insert({"REACTIVE POWER IN MVAR",                      get_winding_reactive_power_in_MVar});
    transformer_meter_map.insert({"REACTIVE POWER IN PU",                        get_winding_reactive_power_in_pu});

    transformer_meter_map.insert({"CURRENT AT PRIMARY WINDING IN KA",            get_primary_side_current_in_kA});
    transformer_meter_map.insert({"CURRENT AT SECONDARY WINDING IN KA",          get_secondary_side_current_in_kA});
    transformer_meter_map.insert({"CURRENT AT TERTIARY WINDING IN KA",           get_tertiary_side_current_in_kA});
    transformer_meter_map.insert({"CURRENT AT PRIMARY WINDING IN PU",            get_primary_side_current_in_pu});
    transformer_meter_map.insert({"CURRENT AT SECONDARY WINDING IN PU",          get_secondary_side_current_in_pu});
    transformer_meter_map.insert({"CURRENT AT TERTIARY WINDING IN PU",           get_tertiary_side_current_in_pu});

    transformer_meter_map.insert({"ACTIVE POWER AT PRIMARY WINDING IN MW",       get_primary_side_active_power_in_MW});
    transformer_meter_map.insert({"ACTIVE POWER AT SECONDARY WINDING IN MW",     get_secondary_side_active_power_in_MW});
    transformer_meter_map.insert({"ACTIVE POWER AT TERTIARY WINDING IN MW",      get_tertiary_side_active_power_in_MW});
    transformer_meter_map.insert({"ACTIVE POWER AT PRIMARY WINDING IN PU",       get_primary_side_active_power_in_pu});
    transformer_meter_map.insert({"ACTIVE POWER AT SECONDARY WINDING IN PU",     get_secondary_side_active_power_in_pu});
    transformer_meter_map.insert({"ACTIVE POWER AT TERTIARY WINDING IN PU",      get_tertiary_side_active_power_in_pu});

    transformer_meter_map.insert({"REACTIVE POWER AT PRIMARY WINDING IN MVAR",   get_primary_side_reactive_power_in_MVar});
    transformer_meter_map.insert({"REACTIVE POWER AT SECONDARY WINDING IN MVAR", get_secondary_side_reactive_power_in_MVar});
    transformer_meter_map.insert({"REACTIVE POWER AT TERTIARY WINDING IN MVAR",  get_tertiary_side_reactive_power_in_MVar});
    transformer_meter_map.insert({"REACTIVE POWER AT PRIMARY WINDING IN PU",     get_primary_side_reactive_power_in_pu});
    transformer_meter_map.insert({"REACTIVE POWER AT SECONDARY WINDING IN PU",   get_secondary_side_reactive_power_in_pu});
    transformer_meter_map.insert({"REACTIVE POWER AT TERTIARY WINDING IN PU",    get_tertiary_side_reactive_power_in_pu});
}

void build_lcc_hvdc2t_meter_map()
{
    using namespace lcc_hvdc2t_meter_getter;
    lcc_hvdc2t_meter_map.insert({"DC CURRENT IN KA",                    get_dc_current_in_kA});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER DC CURRENT IN KA",          get_rectifier_dc_current_in_kA});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER AC CURRENT IN KA",          get_rectifier_ac_current_in_kA});
    lcc_hvdc2t_meter_map.insert({"INVERTER DC CURRENT IN KA",           get_inverter_dc_current_in_kA});
    lcc_hvdc2t_meter_map.insert({"INVERTER AC CURRENT IN KA",           get_inverter_ac_current_in_kA});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER ALPHA IN DEG",              get_rectifier_alpha_in_deg});
    lcc_hvdc2t_meter_map.insert({"INVERTER GAMMA IN DEG",               get_inverter_gamma_in_deg});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER MU IN DEG",                 get_rectifier_mu_in_deg});
    lcc_hvdc2t_meter_map.insert({"INVERTER MU IN DEG",                  get_inverter_mu_in_deg});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER DC VOLTAGE IN KV",          get_rectifier_dc_voltage_in_kV});
    lcc_hvdc2t_meter_map.insert({"INVERTER DC VOLTAGE IN KV",           get_inverter_dc_voltage_in_kV});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER AC VOLTAGE IN PU",          get_rectifier_ac_voltage_in_pu});
    lcc_hvdc2t_meter_map.insert({"INVERTER AC VOLTAGE IN PU",           get_inverter_ac_voltage_in_pu});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER DC POWER IN MW",            get_rectifier_dc_power_in_MW});
    lcc_hvdc2t_meter_map.insert({"INVERTER DC POWER IN MW",             get_inverter_dc_power_in_MW});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER AC ACTIVE POWER IN MW",     get_rectifier_ac_active_power_in_MW});
    lcc_hvdc2t_meter_map.insert({"INVERTER AC ACTIVE POWER IN MW",      get_inverter_ac_active_power_in_MW});
    lcc_hvdc2t_meter_map.insert({"RECTIFIER AC REACTIVE POWER IN MVAR", get_rectifier_ac_reactive_power_in_MVar});
    lcc_hvdc2t_meter_map.insert({"INVERTER AC REACTIVE POWER IN MVAR",  get_inverter_ac_reactive_power_in_MVar});
    lcc_hvdc2t_meter_map.insert({"2T LCC HVDC MODEL INTERNAL VARIABLE", get_lcc_hvdc2t_model_internal_variable});
}

void build_vsc_hvdc_meter_map()
{
    using namespace vsc_hvdc_meter_getter;
    vsc_hvdc_meter_map.insert({"CONVERTER AC VOLTAGE IN PU",                       get_converter_ac_voltage_in_pu});
    vsc_hvdc_meter_map.insert({"CONVERTER AC VOLTAGE IN KV",                       get_converter_ac_voltage_in_kV});
    vsc_hvdc_meter_map.insert({"CONVERTER AC CURRENT IN KA",                       get_converter_ac_current_in_kA});
    vsc_hvdc_meter_map.insert({"CONVERTER DC VOLTAGE IN KV",                       get_converter_dc_voltage_in_kV});
    vsc_hvdc_meter_map.insert({"CONVERTER DC CURRENT IN KA",                       get_converter_dc_current_in_kA});
    vsc_hvdc_meter_map.insert({"CONVERTER DC POWER IN MW",                         get_converter_dc_power_in_MW});
    vsc_hvdc_meter_map.insert({"CONVERTER AC ACTIVE POWER IN MW",                  get_converter_ac_active_power_in_MW});
    vsc_hvdc_meter_map.insert({"CONVERTER AC REACTIVE POWER IN MVAR",              get_converter_ac_reactive_power_in_MVar});
    vsc_hvdc_meter_map.insert({"DC BUS VOLTAGE IN KV",                             get_dc_bus_voltage_in_kV});
    vsc_hvdc_meter_map.insert({"DC LINE CURRENT IN KA",                            get_dc_line_current_in_kA});
    vsc_hvdc_meter_map.insert({"DC LINE POWER IN MW",                              get_dc_line_power_in_MW});
    vsc_hvdc_meter_map.insert({"VSC LCC_HVDC2T NETWORK MODEL INTERNAL VARIABLE",   get_vsc_hvdc_network_model_internal_variable});
    vsc_hvdc_meter_map.insert({"VSC LCC_HVDC2T CONVERTER MODEL INTERNAL VARIABLE", get_vsc_hvdc_converter_model_internal_variable});
}

void build_mt_hvdc_meter_map()
{
    using namespace mt_hvdc_meter_getter;

    mt_hvdc_meter_map.insert({"LCC CONVERTER DC CURRENT IN KA",             get_lcc_converter_dc_current_in_kA});
    mt_hvdc_meter_map.insert({"LCC CONVERTER DC VOLTAGE IN KV",             get_lcc_converter_dc_voltage_in_kV});
    mt_hvdc_meter_map.insert({"LCC CONVERTER DC POWER IN MW",               get_lcc_converter_dc_power_in_MW});
    mt_hvdc_meter_map.insert({"LCC CONVERTER AC CURRENT IN KA",             get_lcc_converter_ac_current_in_kA});
    mt_hvdc_meter_map.insert({"LCC CONVERTER FIRING ANGLE IN DEG",          get_lcc_converter_firing_angle_in_deg});
    mt_hvdc_meter_map.insert({"LCC CONVERTER ACTIVE POWER IN MW",           get_lcc_converter_active_power_in_MW});
    mt_hvdc_meter_map.insert({"LCC CONVERTER REACTIVE POWER IN MVAR",       get_lcc_converter_reactive_power_in_MVar});
    mt_hvdc_meter_map.insert({"LCC CONVERTER REFERENCE ANGLE OF CC IN DEG", get_lcc_converter_reference_angle_of_cc_in_deg});
    mt_hvdc_meter_map.insert({"LCC CONVERTER REFERENCE ANGLE OF CEA IN DEG",get_lcc_converter_reference_angle_of_cea_in_deg});
    mt_hvdc_meter_map.insert({"LCC CONVERTER REFERENCE ANGLE OF CV IN DEG", get_lcc_converter_reference_angle_of_cv_in_deg});
    mt_hvdc_meter_map.insert({"LCC CONVERTER IDC REF IN KA",                get_lcc_converter_idc_ref_in_kA});

    mt_hvdc_meter_map.insert({"VSC CONVERTER DC CURRENT IN KA",         get_vsc_converter_dc_current_in_kA});
    mt_hvdc_meter_map.insert({"VSC CONVERTER DC VOLTAGE IN KV",         get_vsc_converter_dc_voltage_in_kV});
    mt_hvdc_meter_map.insert({"VSC CONVERTER DC POWER IN MW",           get_vsc_converter_dc_power_in_MW});
    mt_hvdc_meter_map.insert({"VSC CONVERTER AC CURRENT IN KA",         get_vsc_converter_ac_current_in_kA});
    mt_hvdc_meter_map.insert({"VSC CONVERTER ACTIVE POWER IN MW",       get_vsc_converter_active_power_in_MW});
    mt_hvdc_meter_map.insert({"VSC CONVERTER REACTIVE POWER IN MVAR",   get_vsc_converter_reactive_power_in_MVar});
    mt_hvdc_meter_map.insert({"VSC CONVERTER ID IN KA",                 get_vsc_converter_id_in_kA});
    mt_hvdc_meter_map.insert({"VSC CONVERTER IQ IN KA",                 get_vsc_converter_iq_in_kA});

    mt_hvdc_meter_map.insert({"DC BUS VOLTAGE IN KV",                   get_dc_bus_voltage_in_kV});

    mt_hvdc_meter_map.insert({"DC LINE CURRENT ON SENDING SIDE IN KA",  get_dc_line_current_on_sending_side_in_kA});
    mt_hvdc_meter_map.insert({"DC LINE CURRENT ON RECEIVING SIDE IN KA",get_dc_line_current_on_receiving_side_in_kA});

    mt_hvdc_meter_map.insert({"HLCC CONVERTER MODEL INTERNAL VARIABLE", get_hlcc_converter_model_internal_variable});
    mt_hvdc_meter_map.insert({"HVSC CONVERTER MODEL INTERNAL VARIABLE", get_hvsc_converter_model_internal_variable});
    mt_hvdc_meter_map.insert({"DC NETWORK MODEL INTERNAL VARIABLE",     get_dc_network_model_internal_variable});
}

void build_equivalent_device_meter_map()
{
    using namespace equivalent_device_meter_getter;
    equivalent_device_meter_map.insert({"VOLTAGE SOURCE VOLTAGE IN PU",             get_voltage_source_voltage_in_pu});
    equivalent_device_meter_map.insert({"VOLTAGE SOURCE VOLTAGE ANGLE IN DEG",      get_voltage_source_voltage_angle_in_deg});
    equivalent_device_meter_map.insert({"VOLTAGE SOURCE RESISTANCE IN PU",          get_voltage_source_resistance_in_pu});
    equivalent_device_meter_map.insert({"VOLTAGE SOURCE REACTANCE IN PU",           get_voltage_source_reactance_in_pu});

    equivalent_device_meter_map.insert({"ACTIVE CONSTANT POWER LOAD IN MW",         get_active_constant_power_load_in_MW});
    equivalent_device_meter_map.insert({"REACTIVE CONSTANT POWER LOAD IN MVAR",     get_reactive_constant_power_load_in_MVar});
    equivalent_device_meter_map.insert({"ACTIVE CONSTANT CURRENT LOAD IN MW",       get_active_constant_current_load_in_MW});
    equivalent_device_meter_map.insert({"REACTIVE CONSTANT CURRENT LOAD IN MVAR",   get_reactive_constant_current_load_in_MVar});
    equivalent_device_meter_map.insert({"ACTIVE CONSTANT IMPEDANCE LOAD IN MW",     get_active_constant_impedance_load_in_MW});
    equivalent_device_meter_map.insert({"REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR", get_reactive_constant_impedance_load_in_MVar});

    equivalent_device_meter_map.insert({"ACTIVE POWER GENERATION IN MW",            get_active_power_generation_in_MW});
    equivalent_device_meter_map.insert({"REACTIVE POWER GENERATION IN MVAR",        get_reactive_power_generation_in_MVar});
    equivalent_device_meter_map.insert({"ACTIVE POWER LOAD IN MW",                  get_active_power_load_in_MW});
    equivalent_device_meter_map.insert({"REACTIVE POWER LOAD IN MVAR",              get_reactive_power_load_in_MVar});
    equivalent_device_meter_map.insert({"ACTIVE POWER NET LOAD IN MW",              get_active_power_net_load_in_MW});
    equivalent_device_meter_map.insert({"REACTIVE POWER NET LOAD IN MVAR",          get_reactive_power_net_load_in_MVar});

    equivalent_device_meter_map.insert({"ACTIVE CONSTANT POWER LOAD IN PU",         get_active_constant_power_load_in_pu});
    equivalent_device_meter_map.insert({"REACTIVE CONSTANT POWER LOAD IN PU",       get_reactive_constant_power_load_in_pu});
    equivalent_device_meter_map.insert({"ACTIVE CONSTANT CURRENT LOAD IN PU",       get_active_constant_current_load_in_pu});
    equivalent_device_meter_map.insert({"REACTIVE CONSTANT CURRENT LOAD IN PU",     get_reactive_constant_current_load_in_pu});
    equivalent_device_meter_map.insert({"ACTIVE CONSTANT IMPEDANCE LOAD IN PU",     get_active_constant_impedance_load_in_pu});
    equivalent_device_meter_map.insert({"REACTIVE CONSTANT IMPEDANCE LOAD IN PU",   get_reactive_constant_impedance_load_in_pu});

    equivalent_device_meter_map.insert({"ACTIVE POWER GENERATION IN PU",            get_active_power_generation_in_pu});
    equivalent_device_meter_map.insert({"REACTIVE POWER GENERATION IN PU",          get_reactive_power_generation_in_pu});
    equivalent_device_meter_map.insert({"ACTIVE POWER LOAD IN PU",                  get_active_power_load_in_pu});
    equivalent_device_meter_map.insert({"REACTIVE POWER LOAD IN PU",                get_reactive_power_load_in_pu});
    equivalent_device_meter_map.insert({"ACTIVE POWER NET LOAD IN PU",              get_active_power_net_load_in_pu});
    equivalent_device_meter_map.insert({"REACTIVE POWER NET LOAD IN PU",            get_reactive_power_net_load_in_pu});
}
