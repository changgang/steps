#ifndef PSASPS5_H
#define PSASPS5_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class PSASPS5 : public STABILIZER_MODEL
{
    public:
        PSASPS5(STEPS& toolkit);
        PSASPS5(const PSASPS5& model);
        virtual ~PSASPS5();
        virtual PSASPS5& operator=(const PSASPS5& model);

        virtual string get_model_name() const;
    public:
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_T4_in_s(double T);
        void set_T5_in_s(double T);
        void set_K1(double K);
        void set_T6_in_s(double T);
        void set_K2(double K);
        void set_a(double a);
        void set_p(double p);
        void set_K(double K);
        void set_Vsmax(double vmax);
        void set_Vsmin(double vmin);

        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_T4_in_s() const;
        double get_T5_in_s() const;
        double get_K1() const;
        double get_T6_in_s() const;
        double get_K2() const;
        double get_a() const;
        double get_p() const;
        double get_K() const;
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
        virtual STEPS_SPARSE_MATRIX get_linearized_system_A() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_B() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_C() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_D() const;
        virtual void get_linearized_system_ABCD(STEPS_SPARSE_MATRIX* A,
                                                STEPS_SPARSE_MATRIX* B,
                                                STEPS_SPARSE_MATRIX* C,
                                                STEPS_SPARSE_MATRIX* D) const;
    private:
        void copy_from_const_model(const PSASPS5& model);

        FIRST_ORDER_BLOCK sensor_1, sensor_2;
        DIFFERENTIAL_BLOCK dedc_block_1, dedc_block_2, dedc_block_3;
        FIRST_ORDER_BLOCK speed_amplifier;
        double K2;
        double a, p;
        double K;
        double Vsmax, Vsmin;
};

#endif // PSASPS5_H
