#ifndef CSEET2_H
#define CSEET2_H

#include "header/model/exciter_model/CSEET1.h"

class CSEET2: public EXCITER_MODEL
{
    // PSASP 11-12
    public:
        CSEET2();
        CSEET2(const CSEET2& model);
        virtual ~CSEET2();
        virtual CSEET2& operator=(const CSEET2& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_excitation_source(AVR_EXCITATION_SOURCE source);
        void set_stabilizer_slot(AVR_FEEDBACK_SLOT slot);
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
        void set_KF(double K);
        void set_TF_in_s(double T);
        void set_VRmax_in_pu(double vmax);
        void set_VRmin_in_pu(double vmin);
        void set_KC(double K);

        AVR_EXCITATION_SOURCE get_excitation_source() const;
        AVR_FEEDBACK_SLOT get_stabilizer_slot() const;
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
        double get_KF() const;
        double get_TF_in_s() const;
        double get_VRmax_in_pu() const;
        double get_VRmin_in_pu() const;
        double get_KC() const;
    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_excitation_voltage_in_pu() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual size_t get_variable_index_from_variable_name(string var_name);
        virtual string get_variable_name_from_variable_index(size_t var_index);
        virtual double get_variable_with_index(size_t var_index);
        virtual double get_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const CSEET2& model);
    private:
        AVR_FEEDBACK_SLOT stabilizer_slot;
        AVR_EXCITATION_SOURCE excitation_source;
        AVR_TUNER_TYPE tuner_type;

        FIRST_ORDER_BLOCK sensor;
        // serial
        bool KV;
        LEAD_LAG_BLOCK serial_tuner1_lead_lag;
        PI_BLOCK serial_tuner1_pi;
        LEAD_LAG_BLOCK serial_tuner2;
        // parallel
        double KP;
        INTEGRAL_BLOCK parallel_integral;
        DIFFERENTIAL_BLOCK parallel_differential;
        // common
        FIRST_ORDER_BLOCK regulator;
        DIFFERENTIAL_BLOCK feedbacker;
        double VRmax, VRmin;
        double KC;

};

#endif // EXCITER_MODEL_H
