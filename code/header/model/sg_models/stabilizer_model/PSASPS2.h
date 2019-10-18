#ifndef PSASPS2_H
#define PSASPS2_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class PSASPS2 : public STABILIZER_MODEL
{
    public:
        PSASPS2();
        PSASPS2(const PSASPS2& model);
        virtual ~PSASPS2();
        virtual PSASPS2& operator=(const PSASPS2& model);

        virtual string get_model_name() const;
    public:
        void set_Kw(double K);
        void set_Kp(double K);
        void set_Kt(double K);
        void set_Tr_in_s(double T);
        void set_Tw1_in_s(double T);
        void set_Tw2_in_s(double T);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_T4_in_s(double T);
        void set_T5_in_s(double T);
        void set_T6_in_s(double T);
        void set_Vsmax(double vmax);
        void set_Vsmin(double vmin);

        double get_Kw() const;
        double get_Kp() const;
        double get_Kt() const;
        double get_Tr_in_s() const;
        double get_Tw1_in_s() const;
        double get_Tw2_in_s() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_T4_in_s() const;
        double get_T5_in_s() const;
        double get_T6_in_s() const;
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
        void copy_from_const_model(const PSASPS2& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        double Kw, Kp, Kt;
        FIRST_ORDER_BLOCK sensor_w, sensor_p, sensor_t;
        DIFFERENTIAL_BLOCK dedc_block_1, dedc_block_2;
        LEAD_LAG_BLOCK phase_tuner_1, phase_tuner_2, phase_tuner_3;
        double Vsmax, Vsmin;

        double speed_deviation_ref_pu, Pe_ref_pu, Pmech_ref_pu;
};

#endif // PSASPS2_H
