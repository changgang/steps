#ifndef WT_TURBINE_MODEL_H
#define WT_TURBINE_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
class WT_TURBINE_MODEL : public WTG_MODEL
{
    /*
    wind turbine mechanical model:
    inputs:
        vwind: wind speed
        pitch: pitch angle
        pelec: electrical power generation
    output:
        pmech: mechanical power
        speed: wind turbine generator speed
        speedRef: speed reference
        angle: wind turbine generator rotor angle
    */
    public:
        WT_TURBINE_MODEL(STEPS& toolkit);
        virtual ~WT_TURBINE_MODEL();

        virtual string get_model_type() const;

        void set_damping_in_pu(double D);
        double get_damping_in_pu() const;


        // inputs
        double get_wt_generator_active_power_generation_in_MW() const;
        double get_initial_wind_turbine_speed_in_pu_from_wt_areodynamic_model() const;
        double get_mechanical_power_in_pu_from_wt_aerodynamic_model() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_turbine_speed_in_pu() const = 0;
        virtual double get_generator_speed_in_pu() const = 0;
        virtual double get_rotor_angle_in_deg() const = 0;
        virtual double get_rotor_angle_in_rad() const = 0;

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

        double damping;
};

#endif // WT_TURBINE_MODEL_H
