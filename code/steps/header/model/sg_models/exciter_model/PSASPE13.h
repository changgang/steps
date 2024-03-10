#ifndef PSASPE13_H
#define PSASPE13_H

#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"
class PSASPE13: public EXCITER_MODEL
{
    public:
        PSASPE13(STEPS& toolkit);
        PSASPE13(const PSASPE13& model);
        virtual ~PSASPE13();
        virtual PSASPE13& operator=(const PSASPE13& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_TR_in_s(double T);
        void set_VImax_in_pu(double vmax);
        void set_VImin_in_pu(double vmin);
        void set_TC_in_s(double T);
        void set_TB_in_s(double T);
        void set_KA(double K);
        void set_TA_in_s(double T);
        void set_VRmax_in_pu(double vmax);
        void set_VRmin_in_pu(double vmin);
        void set_KF(double K);
        void set_TF_in_s(double T);
        void set_Efdmax_in_pu(double emax);
        void set_Efdmin_in_pu(double emin);
        void set_KC(double K);

        double get_TR_in_s() const;
        double get_VImax_in_pu() const;
        double get_VImin_in_pu() const;
        double get_TC_in_s() const;
        double get_TB_in_s() const;
        double get_KA() const;
        double get_TA_in_s() const;
        double get_VRmax_in_pu() const;
        double get_VRmin_in_pu() const;
        double get_KF() const;
        double get_TF_in_s() const;
        double get_Efdmax_in_pu() const;
        double get_Efdmin_in_pu() const;
        double get_KC() const;
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
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

        virtual void linearize();
        virtual void build_linearized_matrix_ABCD();
    private:
        void copy_from_const_model(const PSASPE13& model);

        FIRST_ORDER_BLOCK sensor;
        double VImax, VImin;
        LEAD_LAG_BLOCK tuner;
        FIRST_ORDER_BLOCK regulator;
        DIFFERENTIAL_BLOCK feedbacker;
        double Efdmax, Efdmin;
        double KC;
};

#endif // EXCITER_MODEL_H
