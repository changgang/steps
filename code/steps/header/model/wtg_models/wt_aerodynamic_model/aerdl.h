#ifndef AERDL_H
#define AERDL_H

#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include "header/basic/multi_point_line.h"
#include <cstdlib>

class AERDL : public WT_AERODYNAMIC_MODEL
{
    public:
        AERDL(STEPS& toolkit);
        AERDL(const AERDL& model);
        virtual ~AERDL();
        virtual AERDL& operator=(const AERDL& model);
    public:
        virtual double get_Cp(double lambda, double pitch_deg) const;
        virtual double get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const;
    public:
        virtual string get_model_name() const;

        virtual void initialize();
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

        void set_Ka(double value);
        void set_a(double value);
        void set_b(double value);
        void set_c(double value);
        void set_initial_turbine_mechanical_power_in_pu(double value);

        double get_Ka() const;
        double get_a() const;
        double get_b() const;
        double get_c() const;

        double get_initial_turbine_mechanical_power_in_pu() const;

        void initialize_turbine_mechanical_power_in_pu();
        void initialize_turbine_speed_in_pu();

        void set_initial_turbine_speed_in_pu(double value);
        double get_turbine_reference_speed_in_pu_without_speed_limit(double pelec);

        virtual double get_initial_turbine_speed_in_pu();//called by turbine model and electrical model
        virtual double get_maximum_available_mechanical_power_per_wt_generator_in_MW_considering_gear_efficiency(double vwind);//called by meter
        virtual double get_turbine_mechanical_power_in_MW();//called by turbine model
        virtual double get_turbine_reference_speed_in_pu();//called by wt electrical model and pitch model
        virtual double get_pitch_angle_in_deg() const;//called by meter
    private:
        void copy_from_const_model(const AERDL& model);
        double Ka;
        double a, b, c;
        double initial_turbine_mechanical_power_in_pu;
        double initial_turbine_speed_in_pu;

};

#endif // AERDL_H
