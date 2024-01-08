#ifndef CSEET1_H
#define CSEET1_H

#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/pi_block.h"
#include "header/block/saturation_block.h"



class CSEET1: public EXCITER_MODEL
{
    // PSASP 3-10
    public:
        CSEET1(STEPS& toolkit);
        CSEET1(const CSEET1& model);
        virtual ~CSEET1();
        virtual CSEET1& operator=(const CSEET1& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_excitation_source(AVR_EXCITATION_SOURCE source);
        void set_exciter_brush(AVR_EXCITER_BRUSH brush);
        void set_feedback_slot(AVR_FEEDBACK_SLOT slot);
        void set_tuner_type(AVR_TUNER_TYPE type);

        void set_TR_in_s(double T);
        // serial
        void set_serial_tuner_KV(bool K);
        void set_serial_tuner_K(double K);
        void set_serial_tuner_T1_in_s(double T);
        void set_serial_tuner_T2_in_s(double T);
        void set_serial_tuner_VA1max_in_pu(double vmax);
        void set_serial_tuner_VA1min_in_pu(double vmin);
        void set_serial_tuner_T3_in_s(double T);
        void set_serial_tuner_T4_in_s(double T);
        // parallel
        void set_parallel_tuner_KP(double K);
        void set_parallel_tuner_KI(double K);
        void set_parallel_tuner_VImax_in_pu(double vmax);
        void set_parallel_tuner_VImin_in_pu(double vmin);
        void set_parallel_tuner_KD(double K);
        void set_parallel_tuner_TD_in_s(double T);
        void set_parallel_tuner_VDmax_in_pu(double vmax);
        void set_parallel_tuner_VDmin_in_pu(double vmin);
        // common
        void set_KA(double K);
        void set_TA_in_s(double T);
        void set_VAmax_in_pu(double vmax);
        void set_VAmin_in_pu(double vmin);
        void set_KH(double K);
        void set_KF(double K);
        void set_TF_in_s(double T);
        void set_KB(double K);
        void set_T5_in_s(double T);
        void set_VRmax_in_pu(double vmax);
        void set_VRmin_in_pu(double vmin);
        void set_KE(double K);
        void set_TE_in_s(double T);
        void set_Vemax_in_pu(double vmax);
        void set_SE75_in_pu(double S);
        void set_SE100_in_pu(double S);
        void set_KC(double K);
        void set_KD(double K);
        void set_Efdmax_in_pu(double vmax);

        AVR_EXCITATION_SOURCE get_excitation_source() const;
        AVR_EXCITER_BRUSH get_exciter_brush() const;
        AVR_FEEDBACK_SLOT get_feedback_slot() const;
        AVR_TUNER_TYPE get_tuner_type() const;

        double get_TR_in_s() const;
        // serial
        bool get_serial_tuner_KV() const;
        double get_serial_tuner_K() const;
        double get_serial_tuner_T1_in_s() const;
        double get_serial_tuner_T2_in_s() const;
        double get_serial_tuner_VA1max_in_pu() const;
        double get_serial_tuner_VA1min_in_pu() const;
        double get_serial_tuner_T3_in_s() const;
        double get_serial_tuner_T4_in_s() const;
        // parallel tuner
        double get_parallel_tuner_KP() const;
        double get_parallel_tuner_KI() const;
        double get_parallel_tuner_VImax_in_pu() const;
        double get_parallel_tuner_VImin_in_pu() const;
        double get_parallel_tuner_KD() const;
        double get_parallel_tuner_TD_in_s() const;
        double get_parallel_tuner_VDmax_in_pu() const;
        double get_parallel_tuner_VDmin_in_pu() const;
        // common
        double get_KA() const;
        double get_TA_in_s() const;
        double get_VAmax_in_pu() const;
        double get_VAmin_in_pu() const;
        double get_KH() const;
        double get_KF() const;
        double get_TF_in_s() const;
        double get_KB() const;
        double get_T5_in_s() const;
        double get_VRmax_in_pu() const;
        double get_VRmin_in_pu() const;
        double get_KE() const;
        double get_TE_in_s() const;
        double get_Vemax_in_pu() const;
        double get_SE75_in_pu() const;
        double get_SE100_in_pu() const;
        double get_KC() const;
        double get_KD() const;
        double get_Efdmax_in_pu() const;
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
        virtual STEPS_SPARSE_MATRIX get_linearized_system_A() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_B() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_C() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_D() const;
        virtual void get_linearized_system_ABCD(STEPS_SPARSE_MATRIX* A,
                                                STEPS_SPARSE_MATRIX* B,
                                                STEPS_SPARSE_MATRIX* C,
                                                STEPS_SPARSE_MATRIX* D) const;
    private:
        void copy_from_const_model(const CSEET1& model);
        double get_initial_Ve_with_Fex_function() const;
        double get_Fex(double Ve, double Ifd) const;

        AVR_FEEDBACK_SLOT feedback_slot;
        AVR_EXCITATION_SOURCE excitation_source;
        AVR_EXCITER_BRUSH exciter_brush;
        AVR_TUNER_TYPE tuner_type;

        FIRST_ORDER_BLOCK sensor;
        // serial tuner
        bool KV;
        LEAD_LAG_BLOCK serial_tuner1_lead_lag;
        PI_BLOCK serial_tuner1_pi;
        LEAD_LAG_BLOCK serial_tuner2;
        // parallel tuner
        double KP;
        INTEGRAL_BLOCK parallel_tuner_integral;
        DIFFERENTIAL_BLOCK parallel_tuner_differential;
        // common
        FIRST_ORDER_BLOCK regulator1, regulator2;
        double Vrmax, Vrmin;
        DIFFERENTIAL_BLOCK feedbacker;
        double KH;
        INTEGRAL_BLOCK exciter;
        double KE;
        SATURATION_BLOCK saturation_block;
        double KC, KD;
        double Efdmax;
};

#endif // EXCITER_MODEL_H
