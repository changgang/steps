#ifndef ESTR0_H
#define ESTR0_H

#include "header/model/energy_storage_model/energy_storage_model.h"
#include "header/block/pid_block.h"
#include "header/block/integral_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/first_order_block.h"


class ESTR0 : public ENERGY_STORAGE_MODEL
{
    public:
        ESTR0();
        ESTR0(const ESTR0& model);
        virtual ~ESTR0();
        virtual ESTR0& operator=(const ESTR0& model);

        virtual string get_model_name() const;
    public:
        void set_Tp1_in_s(double t);
        void set_Tp2_in_s(double t);
        void set_Tp3_in_s(double t);
        void set_Tp4_in_s(double t);
        void set_Kpp(double k);
        void set_Kip(double k);
        void set_Kdp(double k);
        void set_Tdp_in_s(double t);
        void set_Tr_in_s(double t);
        void set_Iacmax_in_pu(double imax);
        void set_Kin_in_pu(double k);
        void set_Kout_in_pu(double k);

        void set_Tq1_in_s(double t);
        void set_Tq2_in_s(double t);
        void set_Tq3_in_s(double t);
        void set_Tq4_in_s(double t);
        void set_Kq(double k);
        void set_Dq(double d);

        double get_Tp1_in_s() const;
        double get_Tp2_in_s() const;
        double get_Tp3_in_s() const;
        double get_Tp4_in_s() const;
        double get_Kpp() const;
        double get_Kip() const;
        double get_Kdp() const;
        double get_Tdp_in_s() const;
        double get_Tr_in_s() const;
        double get_Iacmax_in_pu() const;
        double get_Kin_in_pu() const;
        double get_Kout_in_pu() const;


        double get_Tq1_in_s() const;
        double get_Tq2_in_s() const;
        double get_Tq3_in_s() const;
        double get_Tq4_in_s() const;
        double get_Kq() const;
        double get_Dq() const;

    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual complex<double> get_terminal_complex_power_in_pu_based_on_mbase() const;
        virtual complex<double> get_terminal_complex_power_in_MVA() const;
        virtual double get_terminal_active_power_in_pu_based_on_mbase() const;
        virtual double get_terminal_active_power_in_MW() const;
        virtual double get_terminal_reactive_power_in_pu_based_on_mbase() const;
        virtual double get_terminal_reactive_power_in_MVar() const;
        virtual double get_terminal_current_in_kA() const;
        virtual complex<double> get_terminal_complex_current_in_kA() const;

        virtual complex<double> get_terminal_complex_current_in_pu_based_on_mbase() const;
        virtual complex<double> get_terminal_complex_current_in_pu_based_on_sbase() const;
        virtual double get_terminal_current_in_pu_based_on_mbase() const;
        virtual double get_terminal_current_in_pu_based_on_sbase() const;

        virtual double get_energy_state_in_pu() const;

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const ESTR0& model);

        LEAD_LAG_BLOCK active_lead_lag_1, active_lead_lag_2;
        PID_BLOCK active_pid_block;
        float Pmax;
        FIRST_ORDER_BLOCK active_power_filter;
        INTEGRAL_BLOCK energy_state_block;
        double E0;
        float Enominal;

        float Effin, Effout;
        LEAD_LAG_BLOCK reactive_lead_lag_1, reactive_lead_lag_2;
        INTEGRAL_BLOCK reactive_integral_block;
        float Dq;


};


#endif // GENERATOR_MODEL_H
