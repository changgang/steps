#ifndef PSASPS3_H
#define PSASPS3_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class PSASPS3 : public STABILIZER_MODEL
{
    public:
        PSASPS3();
        PSASPS3(const PSASPS3& model);
        virtual ~PSASPS3();
        virtual PSASPS3& operator=(const PSASPS3& model);

        virtual string get_model_name() const;
    public:
        void set_Macc(double M);
        void set_Iacc(double I);
        void set_Nacc(double N);
        void set_dedc_1_flag(bool flag);
        void set_Tacc_in_s(double T);
        void set_TD_in_s(double T);
        void set_Ti1_in_s(double T);
        void set_Ti2_in_s(double T);
        void set_Ti3_in_s(double T);
        void set_Ti4_in_s(double T);
        void set_K0(double K);
        void set_K1(double K);
        void set_K2(double K);
        void set_K3(double K);
        void set_K4(double K);
        void set_KP(double K);
        void set_T6_in_s(double T);
        void set_Vsmax(double vmax);
        void set_Vsmin(double vmin);

        double get_Macc() const;
        double get_Iacc() const;
        double get_Nacc() const;
        bool get_dedc_1_flag() const;
        double get_Tacc_in_s() const;
        double get_TD_in_s() const;
        double get_Ti1_in_s() const;
        double get_Ti2_in_s() const;
        double get_Ti3_in_s() const;
        double get_Ti4_in_s() const;
        double get_K0() const;
        double get_K1() const;
        double get_K2() const;
        double get_K3() const;
        double get_K4() const;
        double get_KP() const;
        double get_Vsmax() const;
        double get_Vsmin() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_stabilizing_signal_in_pu() const;

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
        void copy_from_const_model(const PSASPS3& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        double Macc, Iacc, Nacc;
        bool dedc_block_1_flag;
        DIFFERENTIAL_BLOCK dedc_block_1, dedc_block_2;
        INTEGRAL_BLOCK phase_tuner_1, phase_tuner_2, phase_tuner_3, phase_tuner_4;
        double K0, K1, K2, K3, K4, KP;
        double Vsmax, Vsmin;

        double speed_deviation_ref_pu, Pe_ref_pu, Pmech_ref_pu;
};

#endif // PSASPS3_H
