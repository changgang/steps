#ifndef WT_GENERATOR_MODEL_H
#define WT_GENERATOR_MODEL_H

#include "header/model/wtg_models/wtg_model.h"

class WT_GENERATOR_MODEL : public WTG_MODEL
{
    public:
        WT_GENERATOR_MODEL(STEPS& toolkit);
        virtual ~WT_GENERATOR_MODEL();

    public:
        virtual string get_model_type() const;
    public:
        void set_current_source_flag(bool flag);
        inline bool get_current_source_flag() const{return current_source_flag;}
        inline bool is_current_source() const{return current_source_flag;}
        inline bool is_voltage_source() const{return not current_source_flag;}

        void set_simplified_model_flag(bool flag);
        inline bool is_simplified_model() const{return simplified_model_flag;}

        // get inputs for dynamics run
        double get_active_current_command_in_pu_based_on_mbase();
        double get_reactive_current_command_in_pu_based_on_mbase();
        double get_reactive_voltage_command_in_pu();
        double get_active_power_command_in_pu_based_on_mbase();
        double get_reactive_power_command_in_pu_based_on_mbase();

        complex<double> get_terminal_complex_power_in_pu_based_on_mbase();
        complex<double> get_terminal_complex_power_in_MVA();
        double get_terminal_active_power_in_MW();
        double get_terminal_reactive_power_in_MVar();
        double get_terminal_active_power_in_pu_based_on_mbase();
        double get_terminal_reactive_power_in_pu_based_on_mbase();

        complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
        double get_terminal_current_in_pu_based_on_sbase();
        double get_terminal_current_in_pu_based_on_mbase();
        double get_active_power_generation_including_stator_loss_in_MW();
        complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();


    public:
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

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

        virtual double get_initial_active_current_command_in_pu_based_on_mbase() const = 0;
        virtual double get_initial_reactive_current_command_in_pu_based_on_mbase() const = 0;
        virtual double get_initial_reactive_voltage_command_in_pu() const = 0;
        virtual double get_initial_active_power_command_in_pu_based_on_mbase() const = 0;
        virtual double get_initial_reactive_power_command_in_pu_based_on_mbase() const = 0;

        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() = 0;
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() = 0;
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase() = 0;
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis() = 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;

    private:
        bool current_source_flag;
        bool simplified_model_flag;
};

#endif // WT_GENERATOR_MODEL_H
