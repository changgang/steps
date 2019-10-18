#ifndef PSASPS8_H
#define PSASPS8_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class PSASPS8 : public STABILIZER_MODEL
{
    public:
        PSASPS8();
        PSASPS8(const PSASPS8& model);
        virtual ~PSASPS8();
        virtual PSASPS8& operator=(const PSASPS8& model);

        virtual string get_model_name() const;
    public:
        void set_Kqv(double K);
        void set_Tqv_in_s(double T);
        void set_Tq1_in_s(double T);
        void set_Tq1p_in_s(double T);
        void set_Tq2_in_s(double T);
        void set_Tq2p_in_s(double T);
        void set_Tq3_in_s(double T);
        void set_Tq3p_in_s(double T);
        void set_Vsmax(double vmax);
        void set_Vsmin(double vmin);

        double get_Kqv() const;
        double get_Tqv_in_s() const;
        double get_Tq1_in_s() const;
        double get_Tq1p_in_s() const;
        double get_Tq2_in_s() const;
        double get_Tq2p_in_s() const;
        double get_Tq3_in_s() const;
        double get_Tq3p_in_s() const;
        double get_Vsmax() const;
        double get_Vsmin() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

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
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const PSASPS8& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        FIRST_ORDER_BLOCK sensor;
        LEAD_LAG_BLOCK phase_tuner_1, phase_tuner_2, phase_tuner_3;
        double Vsmax, Vsmin;

        double V_ref_pu;
};

#endif // PSASPS8_H
