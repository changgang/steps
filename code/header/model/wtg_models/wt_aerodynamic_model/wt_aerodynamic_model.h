#ifndef WT_AERODYNAMIC_MODEL_H
#define WT_AERODYNAMIC_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
#include <cstdlib>

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
        WT_AERODYNAMIC_MODEL& operator=(const WT_AERODYNAMIC_MODEL& model);
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

        void set_initial_wind_speed_in_mps(double v);
        void set_initial_pitch_angle_in_deg(double p);
        void set_initial_turbine_speed_in_rad_per_s(double w);

        double get_initial_pitch_angle_in_deg() const;

        void set_air_density_in_kgpm3(double rou);
        void set_overspeed_mode_flag(bool flag);

        double get_wind_speed_in_mps() const;
        double get_air_density_in_kgpm3() const;
        double get_pitch_angle_in_deg() const;
        double get_turbine_frequency_in_Hz() const;
        double get_turbine_speed_in_rad_per_s() const;
        bool get_overspeed_mode_flag() const;
    public:
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
        virtual double get_turbine_mechanical_power_in_MW() const = 0;
        virtual double get_turbine_reference_speed_in_rad_per_s() const = 0;

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
        bool overspeed_mode_flag;
};

#endif // WT_AERODYNAMIC_MODEL_H
