#ifndef WT_AERODYNAMIC_MODEL_H
#define WT_AERODYNAMIC_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
#include <cstdlib>

enum WTG_TURBINE_SPEED_MODE
{
    WT_UNDERSPEED_MODE = -1,
    WT_MPPT_MODE = 0,
    WT_OVERSPEED_MODE = 1
};


class WT_AERODYNAMIC_MODEL : public WTG_MODEL
{
    /*  aero dynamic model for WTG
        inputs:
            vwind: wind speed, in pu or in mps
            beta: pitch angle, in deg
            speed: wt rotor speed, in pu, or rad/s. not speed deviation
    */
    public:
        WT_AERODYNAMIC_MODEL();
        WT_AERODYNAMIC_MODEL(const WT_AERODYNAMIC_MODEL& model);
        virtual ~WT_AERODYNAMIC_MODEL();
        virtual WT_AERODYNAMIC_MODEL& operator=(const WT_AERODYNAMIC_MODEL& model);
        void copy_from_const_model(const WT_AERODYNAMIC_MODEL& model);
    public:
        virtual string get_model_type() const;

        double get_damping_in_pu() const;

        void set_number_of_pole_pairs(size_t n);
        void set_generator_to_turbine_gear_ratio(double r);
        void set_gear_efficiency(double eta);
        void set_turbine_blade_radius_in_m(double r);
        void set_nominal_wind_speed_in_mps(double v);
        void set_nominal_air_density_in_kgpm3(double rou);

        size_t get_number_of_pole_pairs() const;
        double get_generator_to_turbine_gear_ratio() const;
        double get_gear_efficiency() const;
        double get_turbine_blade_radius_in_m() const;
        double get_nominal_wind_speed_in_mps() const;
        double get_nominal_air_density_in_kgpm3() const;
        double get_nominal_turbine_speed_in_rad_per_s() const;

        void set_air_density_in_kgpm3(double rou);
        void set_turbine_speed_mode(WTG_TURBINE_SPEED_MODE mode);
        void set_max_steady_state_turbine_speed_in_pu(double w);
        void set_min_steady_state_turbine_speed_in_pu(double w);

        double get_air_density_in_kgpm3() const;
        WTG_TURBINE_SPEED_MODE get_turbine_speed_mode() const;
        double get_max_steady_state_turbine_speed_in_pu() const;
        double get_min_steady_state_turbine_speed_in_pu() const;

        double get_wind_speed_in_mps() const;
        double get_pitch_angle_in_deg() const;
        double get_turbine_speed_in_rad_per_s() const;
        double get_turbine_frequency_in_Hz() const;

        void set_initial_pitch_angle_in_deg(double pitch);
        void set_initial_turbine_speed_in_rad_per_s(double w);
        double get_initial_pitch_angle_in_deg() const;
        double get_initial_turbine_speed_in_rad_per_s() const;
        double get_initial_turbine_speed_in_pu() const;
        double get_total_wind_power_per_wt_generator_in_MW(double vwind) const;


        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        double get_maximum_available_mechanical_power_per_wt_generator_in_MW(double vwind);
        double get_turbine_mechanical_power_per_wt_generator_in_MW() const;
        double get_turbine_mechanical_power_in_MW() const;
        double get_turbine_reference_speed_in_rad_per_s();
        double get_turbine_reference_speed_in_pu();

        double get_extracted_power_from_wind_per_wt_generator_in_MW_with_turbine_speed_in_rad_per_s(double speed_rad_per_s) const;
        double get_Cpmax(double pitch_deg);
        double get_lambda_at_Cpmax(double pitch_deg);
    public:
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual double get_Cp(double lambda, double pitch_deg) const = 0;
        virtual double get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        void set_cpmax_at_zero_pitch();
        double get_cpmax_at_zero_pitch() const;
        void initialize_wind_turbine_blade_radius_and_gear_ratio();
        void initialize_turbine_blade_radius_with_nominal_parameters();
        void initialize_generator_to_turbine_gear_ratio();
        void initialize_pitch_angle_and_turbine_speed();
        void initialize_pitch_angle_and_turbine_speed_with_mppt_mode();
        void initialize_pitch_angle_and_turbine_speed_with_underspeed_or_overspeed_mode();
        void initialize_pitch_angle();
        void initialize_turbine_speed();
        double get_turbine_reference_speed_in_rad_per_s_without_speed_limit();
        double get_mppt_speed_in_rad_per_s(double pitch_in_deg);

        void set_current_pitch_angle_in_deg(double pitch);
        void update_current_lambda_at_cpmax_with_current_pitch_angle();
        void set_current_pelec_including_loss_per_turbine_in_MW(double pelec);
        void update_current_turbine_speed_reference_without_limit();
        double get_current_pitch_angle_in_deg() const;
        double get_current_lambda_at_cpmax() const;
        double get_current_pelec_including_loss_per_turbine_in_MW() const;
        double get_current_turbine_speed_reference_without_limit_in_rad_per_s() const;
        // turbine nominals
        double nominal_wind_speed_in_mps;
        double turbine_blade_radius_in_m;
        double nominal_air_density_in_kgpm3;

        size_t number_of_pole_pairs;
        double generator_to_turbine_gear_ratio;
        double gear_efficiency;

        // operation parameter
        double initial_wind_speed_in_mps;
        double initial_pitch_angle_in_deg;
        double initial_turbine_speed_in_rad_per_s;

        double wind_speed_in_mps;
        double air_density_in_kgpm3;
        double pitch_angle_in_deg;
        double turbine_speed_in_rad_per_s;
        WTG_TURBINE_SPEED_MODE speed_mode;

        double max_steady_state_turbine_speed_in_pu, min_steady_state_turbine_speed_in_pu;

        double cpmax_at_zero_pitch;
        double current_pitch_angle_in_deg, current_lambda_at_cpmax;
        double current_pelec_including_loss_per_turbine, current_turbine_speed_reference_without_limit_in_rad_per_s;
};

#endif // WT_AERODYNAMIC_MODEL_H
