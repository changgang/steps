#ifndef IEEEG1_H
#define IEEEG1_H

#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"

class IEEEG1 : public TURBINE_GOVERNOR_MODEL
{
    public:
        IEEEG1();
        IEEEG1(const IEEEG1&model);
        virtual ~IEEEG1();
        virtual IEEEG1& operator=(const IEEEG1& model);
    public: // specific model level
        virtual string get_model_name() const;
        virtual double get_double_data_with_index(size_t index) const;
        virtual double get_double_data_with_name(string par_name) const;
        virtual void set_double_data_with_index(size_t index, double value);
        virtual void set_double_data_with_name(string par_name, double value);

        void set_K(double K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_Uo_in_pu(double U);
        void set_Uc_in_pu(double U);
        void set_Pmax_in_pu(double P);
        void set_Pmin_in_pu(double P);
        void set_T4_in_s(double T);
        void set_T5_in_s(double T);
        void set_T6_in_s(double T);
        void set_T7_in_s(double T);
        void set_K1(double K);
        void set_K2(double K);
        void set_K3(double K);
        void set_K4(double K);
        void set_K5(double K);
        void set_K6(double K);
        void set_K7(double K);
        void set_K8(double K);

        double get_K() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_Uo_in_pu() const;
        double get_Uc_in_pu() const;
        double get_Pmax_in_pu() const;
        double get_Pmin_in_pu() const;
        double get_T4_in_s() const;
        double get_T5_in_s() const;
        double get_T6_in_s() const;
        double get_T7_in_s() const;
        double get_K1() const;
        double get_K2() const;
        double get_K3() const;
        double get_K4() const;
        double get_K5() const;
        double get_K6() const;
        double get_K7() const;
        double get_K8() const;

    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_mechanical_power_in_pu_based_on_mbase() const;
        virtual double get_mechanical_power_upper_limit_in_pu_based_on_mbase() const;
        virtual double get_mechanical_power_lower_limit_in_pu_based_on_mbase() const;
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
        void copy_from_const_model(const IEEEG1& model);
        LEAD_LAG_BLOCK droop;
        double Tservo;
        double Uo, Uc;
        INTEGRAL_BLOCK servo_motor;
        FIRST_ORDER_BLOCK delayer1;
        double K1, K2;
        FIRST_ORDER_BLOCK delayer2;
        double K3, K4;
        FIRST_ORDER_BLOCK delayer3;
        double K5, K6;
        FIRST_ORDER_BLOCK delayer4;
        double K7, K8;
};

#endif // IEEEG1_H
