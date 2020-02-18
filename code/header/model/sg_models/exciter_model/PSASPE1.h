#ifndef PSASPE1_H
#define PSASPE1_H

#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"
class PSASPE1: public EXCITER_MODEL
{
    public:
        PSASPE1(STEPS& toolkit);
        PSASPE1(const PSASPE1& model);
        virtual ~PSASPE1();
        virtual PSASPE1& operator=(const PSASPE1& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_KR(double K);
        void set_TR_in_s(double T);
        void set_KA(double K);
        void set_TA_in_s(double T);
        void set_TE_in_s(double T);
        void set_Efdmax_in_pu(double emax);
        void set_Efdmin_in_pu(double emin);
        void set_KF(double K);
        void set_TF_in_s(double T);

        double get_KR() const;
        double get_TR_in_s() const;
        double get_KA() const;
        double get_TA_in_s() const;
        double get_TE_in_s() const;
        double get_Efdmax_in_pu() const;
        double get_Efdmin_in_pu() const;
        double get_KF() const;
        double get_TF_in_s() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_excitation_voltage_in_pu();
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
        void copy_from_const_model(const PSASPE1& model);

        void set_VAmax_in_pu(double vmax);
        void set_VAmin_in_pu(double vmin);
        double get_VAmax_in_pu() const;
        double get_VAmin_in_pu() const;

        FIRST_ORDER_BLOCK sensor;
        FIRST_ORDER_BLOCK regulator;
        DIFFERENTIAL_BLOCK feedbacker;
        FIRST_ORDER_BLOCK exciter;
        double VAmax, VAmin;
        double Efd0;
};

#endif // EXCITER_MODEL_H
