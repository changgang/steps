#ifndef IEEEG2_H
#define IEEEG2_H

#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"

class IEEEG2 : public TURBINE_GOVERNOR_MODEL
{
    public:
        IEEEG2(STEPS& toolkit);
        IEEEG2(const IEEEG2&model);
        virtual ~IEEEG2();
        virtual IEEEG2& operator=(const IEEEG2& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_K(double K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_Pmax_in_pu(double P);
        void set_Pmin_in_pu(double P);
        void set_T4_in_s(double T);

        double get_K() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_Pmax_in_pu() const;
        double get_Pmin_in_pu() const;
        double get_T4_in_s() const;

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
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const IEEEG2& model);

        FIRST_ORDER_BLOCK droop;
        LEAD_LAG_BLOCK tuner;
        LEAD_LAG_BLOCK water_hammer;
        double Pmax, Pmin;
        double K3, K4;
};

#endif // IEEEG2_H
