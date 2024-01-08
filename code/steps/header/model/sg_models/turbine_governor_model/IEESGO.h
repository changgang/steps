#ifndef IEESGO_H
#define IEESGO_H

#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/block/proportional_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"

class IEESGO : public TURBINE_GOVERNOR_MODEL
{
    public:
        IEESGO(STEPS& toolkit);
        IEESGO(const IEESGO&model);
        virtual ~IEESGO();
        virtual IEESGO& operator=(const IEESGO& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_K1(double K);
        void set_K2(double K);
        void set_K3(double K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_T4_in_s(double T);
        void set_T5_in_s(double T);
        void set_T6_in_s(double T);
        void set_Pmax_in_pu(double P);
        void set_Pmin_in_pu(double P);

        double get_K1() const;
        double get_K2() const;
        double get_K3() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_T4_in_s() const;
        double get_T5_in_s() const;
        double get_T6_in_s() const;
        double get_Pmax_in_pu() const;
        double get_Pmin_in_pu() const;

    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_mechanical_power_in_pu_based_on_mbase() const;
        virtual double get_mechanical_power_upper_limit_in_pu_based_on_mbase() const;
        virtual double get_mechanical_power_lower_limit_in_pu_based_on_mbase() const;
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

        virtual void linearize();
        virtual STEPS_SPARSE_MATRIX get_linearized_system_A() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_B() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_C() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_D() const;
        virtual void get_linearized_system_ABCD(STEPS_SPARSE_MATRIX* A,
                                                STEPS_SPARSE_MATRIX* B,
                                                STEPS_SPARSE_MATRIX* C,
                                                STEPS_SPARSE_MATRIX* D) const;
    private:
        void copy_from_const_model(const IEESGO& model);

        LEAD_LAG_BLOCK governor_tuner;
        FIRST_ORDER_BLOCK governor;
        FIRST_ORDER_BLOCK high_pressure_turbine;
        FIRST_ORDER_BLOCK medium_pressure_turbine;
        FIRST_ORDER_BLOCK low_pressure_turbine;
        double pmax, pmin;
};

#endif // IEESGO_H
