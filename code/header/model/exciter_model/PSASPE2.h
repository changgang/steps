#ifndef PSASPE2_H
#define PSASPE2_H

#include "header/model/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/pi_block.h"
class PSASPE2: public EXCITER_MODEL
{
    public:
        PSASPE2();
        PSASPE2(const PSASPE2& model);
        virtual ~PSASPE2();
        virtual PSASPE2& operator=(const PSASPE2& model);
    public: // specific exciter
        virtual string get_model_name() const;
        virtual double get_double_data_with_index(size_t index) const;
        virtual double get_double_data_with_name(string par_name) const;
        virtual void set_double_data_with_index(size_t index, double value);
        virtual void set_double_data_with_name(string par_name, double value);

        void set_KR(double K);
        void set_TR_in_s(double T);
        void set_K2(bool K);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_T4_in_s(double T);
        void set_KA(double K);
        void set_TA_in_s(double T);
        void set_Efdmax_in_pu(double emax);
        void set_Efdmin_in_pu(double emin);
        void set_Vta_in_pu(double V);
        void set_Vtb_in_pu(double V);
        void set_Kpt(double K);
        void set_Kit(double K);
        void set_Ke(double K);

        double get_KR() const;
        double get_TR_in_s() const;
        bool get_K2() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_T4_in_s() const;
        double get_KA() const;
        double get_TA_in_s() const;
        double get_Efdmax_in_pu() const;
        double get_Efdmin_in_pu() const;
        double get_Vta_in_pu() const;
        double get_Vtb_in_pu() const;
        double get_Kpt() const;
        double get_Kit() const;
        double get_Ke() const;
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
        void copy_from_const_model(const PSASPE2& model);
        FIRST_ORDER_BLOCK sensor;
        bool K2;
        LEAD_LAG_BLOCK tuner1_lead_lag;
        PI_BLOCK tuner1_pi;
        LEAD_LAG_BLOCK tuner2;
        FIRST_ORDER_BLOCK regulator;
        double Efdmax, Efdmin;
        double Vta, Vtb;
        double Kpt, Kit, Ke;

        double Efd0;
        double Vt0;
};

#endif // EXCITER_MODEL_H
