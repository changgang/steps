#ifndef PSASPS1_H
#define PSASPS1_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class PSASPS1 : public STABILIZER_MODEL
{
    public:
        PSASPS1(STEPS& toolkit);
        PSASPS1(const PSASPS1& model);
        virtual ~PSASPS1();
        virtual PSASPS1& operator=(const PSASPS1& model);

        virtual string get_model_name() const;
    public:
        void set_Kq1(double K);
        void set_Kq2(double K);
        void set_Kq3(double K);
        void set_K(unsigned int K);
        void set_Tq_in_s(double T);
        void set_T1e_in_s(double T);
        void set_T2e_in_s(double T);
        void set_T3e_in_s(double T);
        void set_T4e_in_s(double T);
        void set_Vsmax(double vmax);
        void set_Vsmin(double vmin);

        double get_Kq1() const;
        double get_Kq2() const;
        double get_Kq3() const;
        unsigned int get_K() const;
        double get_Tq_in_s() const;
        double get_T1e_in_s() const;
        double get_T2e_in_s() const;
        double get_T3e_in_s() const;
        double get_T4e_in_s() const;
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
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

        virtual void linearize();
    private:
        void copy_from_const_model(const PSASPS1& model);

        double Kq1, Kq2, Kq3;
        unsigned int K;
        DIFFERENTIAL_BLOCK dedc_block;
        LEAD_LAG_BLOCK phase_tuner_1, phase_tuner_2, phase_tuner_3;
        double Vsmax, Vsmin;

        double speed_deviation_ref_pu, Pe_ref_pu, Vterminal_ref_pu;
};

#endif // PSASPS1_H
