#ifndef URCSCT_H
#define URCSCT_H

#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/pi_block.h"
#include "header/basic/continuous_buffer.h"

class URCSCT : public TURBINE_GOVERNOR_MODEL
{
    public:
        URCSCT(STEPS& toolkit);
        URCSCT(const URCSCT&model);
        virtual ~URCSCT();
        virtual URCSCT& operator=(const URCSCT& model);
    public: // specific model level
        virtual string get_model_name() const;

        // GAST2A
        void set_gas_W(double W);
        void set_gas_X_in_s(double T);
        void set_gas_Y_in_s(double T);
        void set_gas_Z(unsigned int Z);
        void set_gas_ETD_in_s(double T);
        void set_gas_TCD_in_s(double T);
        void set_gas_Prate_in_MW(double P);
        void set_gas_T_in_s(double T);
        void set_gas_max_in_pu(double U);
        void set_gas_min_in_pu(double U);
        void set_gas_ECR_in_s(double T);
        void set_gas_K3(double K);
        void set_gas_a(double a);
        void set_gas_b_in_s(double T);
        void set_gas_c(double K);
        void set_gas_Tf_in_s(double T);
        void set_gas_Kf(double K);
        void set_gas_K5(double K);
        void set_gas_K4(double K);
        void set_gas_T3_in_s(double T);
        void set_gas_T4_in_s(double T);
        void set_gas_Tt_in_s(double T);
        void set_gas_T5_in_s(double T);
        void set_gas_af1(double K);
        void set_gas_bf1(double K);
        void set_gas_af2(double K);
        void set_gas_bf2(double K);
        void set_gas_cf2(double K);
        void set_gas_TR_in_deg(double T);
        void set_gas_K6(double K);
        void set_gas_TC_in_deg(double T);
        // IEEEG1
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
        //URCSCT
        void set_ST_in_MW(double S);
        void set_PoutA_in_MW(double P);
        void set_SToutA_in_MW(double S);
        void set_PoutB_in_MW(double P);
        void set_SToutB_in_MW(double S);
        void set_PoutC_in_MW(double P);
        void set_SToutC_in_MW(double S);

        // GAST2A
        double get_gas_W() const;
        double get_gas_X_in_s() const;
        double get_gas_Y_in_s() const;
        unsigned int get_gas_Z() const;
        double get_gas_ETD_in_s() const;
        double get_gas_TCD_in_s() const;
        double get_gas_Prate_in_MW() const;
        double get_gas_T_in_s() const;
        double get_gas_max_in_pu() const;
        double get_gas_min_in_pu() const;
        double get_gas_ECR_in_s() const;
        double get_gas_K3() const;
        double get_gas_a() const;
        double get_gas_b_in_s() const;
        double get_gas_c() const;
        double get_gas_Tf_in_s() const;
        double get_gas_Kf() const;
        double get_gas_K5() const;
        double get_gas_K4() const;
        double get_gas_T3_in_s() const;
        double get_gas_T4_in_s() const;
        double get_gas_Tt_in_s() const;
        double get_gas_T5_in_s() const;
        double get_gas_af1() const;
        double get_gas_bf1() const;
        double get_gas_af2() const;
        double get_gas_bf2() const;
        double get_gas_cf2() const;
        double get_gas_TR_in_deg() const;
        double get_gas_K6() const;
        double get_gas_TC_in_deg() const;
        // IEEEG1
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
        //URCSCT
        double get_ST_in_MW() const;
        double get_PoutA_in_MW() const;
        double get_SToutA_in_MW() const;
        double get_PoutB_in_MW() const;
        double get_SToutB_in_MW() const;
        double get_PoutC_in_MW() const;
        double get_SToutC_in_MW() const;
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
        void copy_from_const_model(const URCSCT& model);

        // GAST2A
        double gas_governor_W, gas_governor_X, gas_governor_Y;
        unsigned int gas_governor_Z;
        double gas_governor_MAX, gas_governor_MIN;
        LEAD_LAG_BLOCK gas_governor_droop;
        PI_BLOCK gas_governor_iso;

        double gas_K3;
        double gas_T;
        CONTINUOUS_BUFFER gas_fuel_control;

        double gas_K6, gas_Kf;

        double gas_a, gas_b, gas_c;
        FIRST_ORDER_BLOCK gas_valve_positioner;

        FIRST_ORDER_BLOCK gas_fuel_system;

        double gas_ECR;
        CONTINUOUS_BUFFER gas_combustor;

        double gas_ETD;
        CONTINUOUS_BUFFER gas_turbine_exhaust;
        double gas_TR, gas_af1, gas_bf1;

        double gas_K4;
        FIRST_ORDER_BLOCK gas_radiation_shield;

        FIRST_ORDER_BLOCK gas_thermocouple;

        double gas_TC;
        double gas_T5, gas_Tt;
        PI_BLOCK gas_temperature_control;

        FIRST_ORDER_BLOCK gas_turbine_dynamic;

        double gas_af2, gas_bf2, gas_cf2;
        double gas_turbine_rate_MW;

        // IEEEG1
        LEAD_LAG_BLOCK droop;
        INTEGRAL_BLOCK servo_motor;
        FIRST_ORDER_BLOCK delayer1;
        FIRST_ORDER_BLOCK delayer2;
        FIRST_ORDER_BLOCK delayer3;
        FIRST_ORDER_BLOCK delayer4;
        double Tservo;
        double Uo, Uc;
        double K1, K2;
        double K3, K4;
        double K5, K6;
        double K7, K8;

        // URCSCT
        double ST;
        double PoutA, SToutA, PoutB, SToutB, PoutC, SToutC;

        double steam_pmech_ref;
};

#endif // URCSCT_H
