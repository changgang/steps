#ifndef AERD0_H
#define AERD0_H

#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include <cstdlib>

class AERD0 : public WT_AERODYNAMIC_MODEL
{
    public:
        AERD0();
        AERD0(const AERD0& model);
        virtual ~AERD0();
        AERD0& operator=(const AERD0& model);
    public:
        void set_C1(double c);
        void set_C2(double c);
        void set_C3(double c);
        void set_C4(double c);
        void set_C5(double c);
        void set_C6(double c);

        double get_C1() const;
        double get_C2() const;
        double get_C3() const;
        double get_C4() const;
        double get_C5() const;
        double get_C6() const;
    public:
        void setup_as_typical_wt_generator();
        void setup_turbine_blade_radius_with_nominal_parameters();
        void setup_generator_to_turbine_gear_ratio();
        void initialize_pitch_angle();
        void initialize_turbine_speed();
        double get_extracted_power_from_wind_per_wt_generatorin_MW() const;
        double get_Cp(double lambda, double pitch_deg) const;
        double get_Cpmax(double pitch_deg) const;
        double get_lambda_at_Cpmax(double pitch_deg) const;
        double get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const;
    public:
        virtual string get_model_name() const;

        virtual double get_double_data_with_index(size_t index) const;
        virtual double get_double_data_with_name(string par_name) const;
        virtual void set_double_data_with_index(size_t index, double value);
        virtual void set_double_data_with_name(string par_name, double value);

        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_turbine_mechanical_power_in_MW() const;
        virtual double get_turbine_reference_speed_in_rad_per_s() const;

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual size_t get_variable_index_from_variable_name(string var_name);
        virtual string get_variable_name_from_variable_index(size_t var_index);
        virtual double get_variable_with_index(size_t var_index);
        virtual double get_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const AERD0& model);
        // Cp function parameters
        double Cp_Coefficients[6];
};

#endif // AERD0_H
