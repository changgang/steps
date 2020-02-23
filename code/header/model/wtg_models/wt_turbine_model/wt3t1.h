#ifndef WT3T1_H
#define WT3T1_H

#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"
#include "header/block/integral_block.h"

class WT3T1 : public WT_TURBINE_MODEL
{
    public:
        WT3T1(STEPS& toolkit);
        WT3T1(const WT3T1&model);
        virtual ~WT3T1();
        virtual WT3T1& operator=(const WT3T1& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_Htotal_in_s(double H);
        void set_Damp_in_pu(double D);
        void set_Kaero(double K);
        void set_Theta2_in_deg(double theta);
        void set_Htfrac_in_pu(double frac);
        void set_Freq1_in_Hz(double f);
        void set_Dshaft_in_pu(double D);

        double get_Htotal_in_s() const;
        double get_Damp_in_pu() const;
        double get_Kaero() const;
        double get_Theta2_in_deg() const;
        double get_Htfrac_in_pu() const;
        double get_Freq1_in_Hz() const;
        double get_Dshaft_in_pu() const;

    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_wind_turbine_mechanical_power_in_pu_based_on_mbase() const;
        virtual double get_wind_turbine_generator_speed_reference_in_pu() const;
        virtual double get_wind_turbine_generator_speed_in_pu() const;
        virtual double get_wind_turbine_generator_rotor_angle_in_deg() const;
        virtual double get_wind_turbine_generator_rotor_angle_in_rad() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WT3T1& model);

        INTEGRAL_BLOCK shaft_twist;
        INTEGRAL_BLOCK turbine_inertia;
        INTEGRAL_BLOCK generator_inertia;
        INTEGRAL_BLOCK generator_rotor_angle;

        float Htotal;
        float Damp;
        float Kaero;
        float Theta2;
        float Htfrac;
        float Freq1;
        float Dshaft;
};

#endif // WT3T1_H
