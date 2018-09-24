#ifndef METER_SETTER_H
#define METER_SETTER_H

#include "header/meter/meter.h"

class POWER_SYSTEM_DATABASE;

class METER_SETTER
{
    public:
        METER_SETTER();
        ~METER_SETTER();

        void set_power_system_database(POWER_SYSTEM_DATABASE* db);
        POWER_SYSTEM_DATABASE* get_power_system_database() const;

        METER prepare_bus_voltage_in_pu_meter(size_t bus);
        METER prepare_bus_voltage_in_kV_meter(size_t bus);
        METER prepare_bus_angle_in_deg_meter(size_t bus);
        METER prepare_bus_angle_in_rad_meter(size_t bus);
        METER prepare_bus_frequency_in_pu_meter(size_t bus);
        METER prepare_bus_frequency_in_Hz_meter(size_t bus);
        METER prepare_bus_frequency_deviation_in_pu_meter(size_t bus);
        METER prepare_bus_frequency_deviation_in_Hz_meter(size_t bus);
        METER prepare_bus_ROCOV_in_pu_per_s_meter(size_t bus);
        METER prepare_bus_ROCOV_in_kV_per_s_meter(size_t bus);
        METER prepare_bus_ROCOF_in_pu_per_s_meter(size_t bus);
        METER prepare_bus_ROCOF_in_Hz_per_s_meter(size_t bus);

        METER prepare_line_current_in_kA_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_active_power_in_MW_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_reactive_power_in_MVar_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_apparent_impedance_in_Ohm_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_apparent_impedance_angle_in_deg_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_current_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_active_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_reactive_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_apparent_impedance_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_line_apparent_impedance_angle_in_rad_meter(const DEVICE_ID& device_id, size_t side_bus);

        METER prepare_transformer_current_in_kA_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_transformer_active_power_in_MW_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_transformer_reactive_power_in_MVar_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_transformer_current_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_transformer_active_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);
        METER prepare_transformer_reactive_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus);

        METER prepare_load_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_load_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_load_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_load_current_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_load_active_power_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_load_reactive_power_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_load_load_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_load_frequency_relay_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_load_voltage_relay_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_load_total_scale_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_load_manually_scale_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_load_relay_shed_scale_in_pu_meter(const DEVICE_ID& device_id);

        METER prepare_generator_rotor_angle_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_generator_rotor_speed_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_generator_rotor_speed_in_Hz_meter(const DEVICE_ID& device_id);
        METER prepare_generator_rotor_speed_deviation_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_generator_rotor_speed_deviation_in_Hz_meter(const DEVICE_ID& device_id);
        METER prepare_generator_internal_voltage_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_current_in_pu_on_mbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_current_in_pu_on_sbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_active_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_active_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_reactive_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_terminal_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_generator_airgap_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_airgap_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_airgap_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_generator_accelerating_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_accelerating_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_accelerating_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_generator_mechanical_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_mechanical_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id);
        METER prepare_generator_mechanical_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_generator_compensated_voltage_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_generator_excitation_voltage_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_generator_stabilizing_signal_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_generator_sync_generator_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_generator_compensator_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_generator_stabilizer_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_generator_exciter_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_generator_turbine_governor_model_internal_variable_meter(const DEVICE_ID& device_id, string name);

        METER prepare_wt_generator_terminal_current_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_terminal_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_terminal_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_terminal_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_mechanical_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_max_available_mechanical_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_speed_reference_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_speed_reference_in_rad_per_s_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_turbine_speed_deviation_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_turbine_speed_deviation_in_Hz_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_turbine_speed_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_turbine_speed_in_Hz_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_rotor_speed_deviation_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_rotor_speed_deviation_in_Hz_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_rotor_speed_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_rotor_speed_in_Hz_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_rotor_angle_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_rotor_angle_in_rad_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_active_current_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_reactive_current_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_active_power_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_reactive_power_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_reactive_voltage_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_pitch_angle_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_wind_speed_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_wind_speed_in_mps_meter(const DEVICE_ID& device_id);
        METER prepare_wt_generator_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_wt_aerodynamic_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_wt_turbine_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_wt_electrical_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_wt_pitch_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_wind_speed_model_internal_variable_meter(const DEVICE_ID& device_id, string name);

        METER prepare_pv_unit_terminal_current_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_terminal_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_terminal_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_terminal_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_active_current_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_reactive_current_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_active_power_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_reactive_power_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_pv_unit_reactive_voltage_command_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_pv_panel_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_pv_converter_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_pv_electrical_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
        METER prepare_pv_irradiance_model_internal_variable_meter(const DEVICE_ID& device_id, string name);

        METER prepare_hvdc_dc_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_dc_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_dc_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_dc_voltage_in_kV_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_ac_voltage_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_alpha_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_mu_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_dc_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_ac_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_ac_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_rectifier_ac_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_dc_voltage_in_kV_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_ac_voltage_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_gamma_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_mu_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_dc_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_ac_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_ac_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_inverter_ac_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_hvdc_model_internal_variable_meter(const DEVICE_ID& device_id, string name);

        METER prepare_equivalent_device_voltage_source_voltage_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_voltage_source_voltage_angle_in_deg_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_voltage_source_resistance_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_voltage_source_reactance_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_active_constant_current_load_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_reactive_constant_current_load_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_active_constant_impedance_load_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_active_power_generation_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_reactive_power_generation_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_active_power_load_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_reactive_power_load_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_active_power_net_load_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_active_constant_power_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_reactive_constant_power_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_active_constant_current_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_reactive_constant_current_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_active_constant_impedance_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_active_power_generation_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_reactive_power_generation_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_active_power_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_reactive_power_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_active_power_net_load_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_equivalent_device_reactive_power_net_load_in_pu_meter(const DEVICE_ID& device_id);

        METER prepare_energy_storage_state_of_energy_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_active_power_in_MW_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_active_power_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_reactive_power_in_MVar_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_reactive_power_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_terminal_current_in_kA_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_terminal_current_in_pu_meter(const DEVICE_ID& device_id);
        METER prepare_energy_storage_model_internal_variable_meter(const DEVICE_ID& device_id, string name);
    private:
        bool prepare_bus_meter(METER& meter, const size_t bus);
        bool prepare_line_meter(METER& meter, const DEVICE_ID& device_id, size_t side_bus);
        bool prepare_transformer_meter(METER& meter, const DEVICE_ID& device_id, size_t side_bus);
        bool prepare_load_meter(METER& meter, const DEVICE_ID& device_id);
        bool prepare_generator_meter(METER& meter, const DEVICE_ID& device_id);
        bool prepare_wt_generator_meter(METER& meter, const DEVICE_ID& device_id);
        bool prepare_pv_unit_meter(METER& meter, const DEVICE_ID& device_id);
        bool prepare_energy_storage_meter(METER& meter, const DEVICE_ID& device_id);
        bool prepare_hvdc_meter(METER& meter, const DEVICE_ID& device_id);
        bool prepare_equivalent_device_meter(METER& meter, const DEVICE_ID& device_id);

        POWER_SYSTEM_DATABASE* db;
};
#endif // METER_SETTER_H
