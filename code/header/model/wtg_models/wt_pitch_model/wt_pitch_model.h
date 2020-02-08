#ifndef WT_PITCH_MODEL_H
#define WT_PITCH_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
class WT_PITCH_MODEL : public WTG_MODEL
{
    /*
    wind turbine mechanical model:
    inputs:
        speed: wt generator speed
        freq: terminal bus frequency
        pelec: wt active power command
    output:
        pitch: pitch angle in deg
    */
    public:
        WT_PITCH_MODEL();
        virtual ~WT_PITCH_MODEL();

        virtual string get_model_type() const;
        // inputs
        double get_wt_generator_speed_in_pu() const;
        double get_wt_generator_reference_speed_in_pu() const;
        double get_bus_frequency_in_pu() const;
        double get_bus_frequency_deviation_in_pu() const;
        double get_initial_pitch_angle_in_deg_from_wt_aerodynamic_model() const;

        void set_frequency_upper_deadband_in_pu(double freq);
        void set_frequency_lower_deadband_in_pu(double freq);
        double get_frequency_upper_deadband_in_pu() const;
        double get_frequency_lower_deadband_in_pu() const;

        void set_hold_wtg_speed_flag(bool flag);
        void set_const_wtg_speed_reference_in_pu(double w);
        bool get_hold_wtg_speed_flag() const;
        double get_const_wtg_speed_reference_in_pu() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_pitch_angle_in_deg() const = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string() const = 0;

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
        void common_constructor();

        float frequency_deadband_upper_in_pu, frequency_deadband_lower_in_pu;
        bool hold_wtg_speed_flag;
        float const_wtg_speed_reference;
};

#endif // WT_PITCH_MODEL_H
