#ifndef ES_ELECTRICAL_MODEL_H
#define ES_ELECTRICAL_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
#include <complex>
class ES_ELECTRICAL_MODEL : public ES_MODEL
{
    public:
        ES_ELECTRICAL_MODEL(STEPS& toolkit);
        virtual ~ES_ELECTRICAL_MODEL();

        void unbypass_model();
        void bypass_model();
        bool is_model_bypassed() const;

        void activate_active_power_control();
        void deactivate_active_power_control();
        bool is_active_power_control_activated() const;

        void set_active_power_control_mode_in_discharge(unsigned int mode);
        void set_active_power_control_mode_in_charge(unsigned int mode);

        unsigned int get_active_power_control_mode_in_discharge() const;
        unsigned int get_active_power_control_mode_in_charge() const;

    public:
        virtual string get_model_type() const;

        complex<double> get_energy_storage_terminal_generation_in_MVA() const;
        complex<double> get_energy_storage_terminal_generation_in_pu_based_on_mbase() const;
        complex<double> get_energy_storage_terminal_complex_current_in_pu() const;
        double get_energy_storage_terminal_current_in_pu() const;
        double get_rotor_speed_in_pu() const;
        double get_rotor_speed_reference_in_pu() const;

    public:
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void initialize_active_power_control_as_zero() = 0;
        virtual double get_frequency_regulation_power_in_pu() const = 0;
        virtual double get_active_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_active_power_command_in_pu_based_on_mbase() const = 0;
        virtual double get_reactive_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_power_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_voltage_command_in_pu() const = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        bool flag_model_bypassed;
        bool flag_active_model_activated;

        unsigned int active_power_control_mode_in_discharge, active_power_control_mode_in_charge;
};


#endif // ES_ELECTRICAL_MODEL_H
