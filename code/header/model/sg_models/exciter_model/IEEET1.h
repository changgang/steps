#ifndef IEEET1_H
#define IEEET1_H

#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"
class IEEET1: public EXCITER_MODEL
{
    public:
        IEEET1();
        IEEET1(const IEEET1& model);
        virtual ~IEEET1();
        virtual IEEET1& operator=(const IEEET1& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_TR_in_s(double T);
        void set_KA(double K);
        void set_TA_in_s(double T);
        void set_VRmax_in_pu(double vr);
        void set_VRmin_in_pu(double vr);
        void set_KE(double K);
        void set_TE_in_s(double T);
        void set_KF(double K);
        void set_TF_in_s(double T);
        void set_E1_in_pu(double E);
        void set_SE1_in_pu(double S);
        void set_E2_in_pu(double E);
        void set_SE2_in_pu(double S);

        double get_TR_in_s() const;
        double get_KA() const;
        double get_TA_in_s() const;
        double get_VRmax_in_pu() const;
        double get_VRmin_in_pu() const;
        double get_KE() const;
        double get_TE_in_s() const;
        double get_KF() const;
        double get_TF_in_s() const;
        double get_E1_in_pu() const;
        double get_SE1_in_pu() const;
        double get_E2_in_pu() const;
        double get_SE2_in_pu() const;
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
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const IEEET1& model);

        FIRST_ORDER_BLOCK sensor;
        FIRST_ORDER_BLOCK regulator;
        DIFFERENTIAL_BLOCK feedbacker;
        INTEGRAL_BLOCK exciter;
        SATURATION_BLOCK saturation_block;
        double KE;
};

#endif // EXCITER_MODEL_H
