#ifndef ES_CONVERTER_MODEL_H
#define ES_CONVERTER_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"

// POWER ELECTRONICS SOURCES
class ES_CONVERTER_MODEL : public ES_MODEL
{
    /*
    Model of es converter
    Inputs:
        IPcmd: active current command in pu, from wt_electrical_model
        IQcmd: reactive current command in pu, from wt_electrical_model
        Pcmd: active power command in pu, from wt_electrical_model
        Qcmd: reactive power command in pu, from wt_electrical_model
        Eqcmd: reactive voltage command in pu, from wt_electrical_model
    Output:
        Isource: current injection to network
    */

    public:
        ES_CONVERTER_MODEL(STEPS& toolkit);
        virtual ~ES_CONVERTER_MODEL();

    public:
        virtual string get_model_type() const;
    public:
        void set_current_source_flag(bool flag);
        bool get_current_source_flag() const;
        bool is_current_source() const;
        bool is_voltage_source() const;
        // set initial active and reactive current commands
        void set_initial_active_current_command_in_pu_based_on_mbase(double ip_command);
        double get_initial_active_current_command_in_pu_based_on_mbase() const;

        void set_initial_reactive_current_command_in_pu_based_on_mbase(double iq_command);
        double get_initial_reactive_current_command_in_pu_based_on_mbase() const;

        void set_initial_reactive_voltage_command_in_pu(double eq_command);
        double get_initial_reactive_voltage_command_in_pu() const;

        void set_initial_active_power_command_in_pu_based_on_mbase(double P_command);
        double get_initial_active_power_command_in_pu_based_on_mbase() const;

        void set_initial_reactive_power_command_in_pu_based_on_mbase(double Q_command);
        double get_initial_reactive_power_command_in_pu_based_on_mbase() const;
        // get inputs for dynamics run
        double get_active_current_command_in_pu_based_on_mbase();
        double get_reactive_current_command_in_pu_based_on_mbase();
        double get_reactive_voltage_command_in_pu();
        double get_active_power_command_in_pu_based_on_mbase();
        double get_reactive_power_command_in_pu_based_on_mbase();
    public:
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase() = 0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() = 0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase() = 0;
        virtual double get_terminal_current_in_pu_based_on_mbase() = 0;
        virtual double get_terminal_current_in_pu_based_on_sbase() = 0;

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

        virtual complex<double> get_terminal_complex_power_in_pu_based_on_mbase() = 0;
        virtual complex<double> get_terminal_complex_power_in_MVA() = 0;
        virtual double get_terminal_active_power_in_pu_based_on_mbase() = 0;
        virtual double get_terminal_active_power_in_MW() = 0;
        virtual double get_terminal_reactive_power_in_pu_based_on_mbase() = 0;
        virtual double get_terminal_reactive_power_in_MVar() = 0;
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() = 0;
        virtual double get_active_power_generation_including_stator_loss_in_MW() = 0;

        virtual double get_pll_angle_in_rad() = 0;
        virtual double get_pll_angle_in_deg() = 0;
        virtual double get_pll_frequency_deviation_in_pu() = 0;
        virtual double get_pll_frequency_deviation_in_Hz() = 0;
        virtual double get_pll_frequency_in_pu() = 0;
        virtual double get_pll_frequency_in_Hz() = 0;
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis() = 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;

    private:
        bool current_source_flag;
        double IP_command0, IQ_command0, EQ_command0, P_command0, Q_command0;
};

#endif // ES_CONVERTER_MODEL_H
