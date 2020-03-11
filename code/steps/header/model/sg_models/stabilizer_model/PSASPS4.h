#ifndef PSASPS4_H
#define PSASPS4_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class PSASPS4 : public STABILIZER_MODEL
{
    public:
        PSASPS4(STEPS& toolkit);
        PSASPS4(const PSASPS4& model);
        virtual ~PSASPS4();
        virtual PSASPS4& operator=(const PSASPS4& model);

        virtual string get_model_name() const;
    public:
        void set_Kw(double K);
        void set_Trw_in_s(double T);
        void set_T5_in_s(double T);
        void set_T6_in_s(double T);
        void set_T7_in_s(double T);
        void set_Kr(double K);
        void set_Trp_in_s(double T);
        void set_Tw_in_s(double T);
        void set_Tw1_in_s(double T);
        void set_Tw2_in_s(double T);
        void set_Ks(double K);
        void set_T9_in_s(double T);
        void set_T10_in_s(double T);
        void set_T12_in_s(double T);
        void set_Kp(double K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T13_in_s(double T);
        void set_T14_in_s(double T);
        void set_T3_in_s(double T);
        void set_T4_in_s(double T);
        void set_Vsmax(double vmax);
        void set_Vsmin(double vmin);

        double get_Kw() const;
        double get_Trw_in_s() const;
        double get_T5_in_s() const;
        double get_T6_in_s() const;
        double get_T7_in_s() const;
        double get_Kr() const;
        double get_Trp_in_s() const;
        double get_Tw_in_s() const;
        double get_Tw1_in_s() const;
        double get_Tw2_in_s() const;
        double get_Ks() const;
        double get_T9_in_s() const;
        double get_T10_in_s() const;
        double get_T12_in_s() const;
        double get_Kp() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T13_in_s() const;
        double get_T14_in_s() const;
        double get_T3_in_s() const;
        double get_T4_in_s() const;
        double get_Vsmax() const;
        double get_Vsmin() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_stabilizing_signal_in_pu();

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
        void copy_from_const_model(const PSASPS4& model);

        FIRST_ORDER_BLOCK speed_sensor, pelec_sensor;
        DIFFERENTIAL_BLOCK speed_dedc_block_1, speed_dedc_block_2, pelec_dedc_block_1, pelec_dedc_block_2;
        //FIRST_ORDER_BLOCK pelec_dedc_block_1;
        double Ks;
        LEAD_LAG_BLOCK notch_filter_phase_tuner;
        FIRST_ORDER_BLOCK notch_filter_first_order_block_1, notch_filter_first_order_block_2, notch_filter_first_order_block_3, notch_filter_first_order_block_4;
        LEAD_LAG_BLOCK phase_tuner_1, phase_tuner_2, phase_tuner_3;
        double Vsmax, Vsmin;

        double speed_deviation_ref_pu, Pe_ref_pu;
};

#endif // PSASPS4_H
