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
        WT_TURBINE_MODEL();
        virtual ~WT_TURBINE_MODEL();

        virtual string get_model_type() const;

        void set_damping_in_pu(double D);
        double get_damping_in_pu() const;


        // inputs
        double get_wt_generator_active_power_generation_in_MW() const;
        double get_pitch_angle_in_deg() const;
        double get_wind_speed_in_mps() const;
        double get_wind_speed_in_pu() const;
        //reference
        void set_mechanical_power_reference_in_pu_based_on_mbase(double P);
        double get_mechanical_power_reference_in_pu_based_on_mbase() const;

        void set_initial_pitch_angle_in_deg(double pitch);
        void set_initial_wind_speed_in_mps(double vwind);
        void set_initial_wind_speed_in_pu(double vwind);
        void set_initial_wind_turbine_speed_in_pu(double speed);

        double get_initial_pitch_angle_in_deg() const;
        double get_initial_wind_speed_in_mps() const;
        double get_initial_wind_speed_in_pu() const;
        double get_initial_wind_turbine_speed_in_pu() const;
        //
        void set_wt_turbine_nominal_wind_speed_in_mps(double vw);
        double get_wt_turbine_nominal_wind_speed_in_mps() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual double get_double_data_with_index(size_t index) const = 0;
        virtual double get_double_data_with_name(string par_name) const = 0;
        virtual void set_double_data_with_index(size_t index, double value) = 0;
        virtual void set_double_data_with_name(string par_name, double value) = 0;

        virtual bool setup_model_with_steps_string(string data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_wind_turbine_mechanical_power_in_pu_based_on_mbase() const = 0;
        virtual double get_wind_turbine_generator_speed_reference_in_pu() const = 0;
        virtual double get_wind_turbine_generator_speed_in_pu() const = 0;
        virtual double get_wind_turbine_generator_rotor_angle_in_deg() const = 0;
        virtual double get_wind_turbine_generator_rotor_angle_in_rad() const = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual size_t get_variable_index_from_variable_name(string var_name)= 0;
        virtual string get_variable_name_from_variable_index(size_t var_index)= 0;
        virtual double get_variable_with_index(size_t var_index)= 0;
        virtual double get_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        void common_constructor();


        double mechanical_power_reference_in_pu;

        double initial_pitch_angle_in_deg;
        double initial_wind_speed_in_mps;
        double initial_wind_speed_in_pu;
        double initial_wind_turbine_speed_in_pu;

        double damping;
};

#endif // WT_TURBINE_MODEL_H
