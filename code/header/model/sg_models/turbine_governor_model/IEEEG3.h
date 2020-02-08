#ifndef IEEEG3_H
#define IEEEG3_H

#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/differential_block.h"

class IEEEG3 : public TURBINE_GOVERNOR_MODEL
{
    public:
        IEEEG3();
        IEEEG3(const IEEEG3&model);
        virtual ~IEEEG3();
        virtual IEEEG3& operator=(const IEEEG3& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_TG_in_s(double TG);
        void set_TP_in_s(double TP);
        void set_Uo_in_pu(double U);
        void set_Uc_in_pu(double U);
        void set_Pmax_in_pu(double P);
        void set_Pmin_in_pu(double P);
        void set_sigma(double sigma);
        void set_delta(double delta);
        void set_TR_in_s(double T);
        void set_TW_in_s(double T);
        void set_a11(double a);
        void set_a13(double a);
        void set_a21(double a);
        void set_a23(double a);

        double get_TG_in_s() const;
        double get_TP_in_s() const;
        double get_Uo_in_pu() const;
        double get_Uc_in_pu() const;
        double get_Pmax_in_pu() const;
        double get_Pmin_in_pu() const;
        double get_sigma() const;
        double get_delta() const;
        double get_TR_in_s() const;
        double get_TW_in_s() const;
        double get_a11() const;
        double get_a13() const;
        double get_a21() const;
        double get_a23() const;

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
        void copy_from_const_model(const IEEEG3& model);

        FIRST_ORDER_BLOCK governor;
        INTEGRAL_BLOCK servo_motor;
        float delta, sigma;
        DIFFERENTIAL_BLOCK feedbacker;
        LEAD_LAG_BLOCK water_hammer;
        float TW;
        float a11, a13, a21, a23;
};

#endif // IEEEG3_H
