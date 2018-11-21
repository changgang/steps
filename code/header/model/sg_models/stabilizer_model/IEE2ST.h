#ifndef IEE2ST_H
#define IEE2ST_H

#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/lead_lag_block.h"

class IEE2ST : public STABILIZER_MODEL
{
    public:
        IEE2ST();
        IEE2ST(const IEE2ST& model);
        virtual ~IEE2ST();
        virtual IEE2ST& operator=(const IEE2ST& model);

        virtual string get_model_name() const;
    public:
        void set_K1(double K);
        void set_T1_in_s(double T);
        void set_K2(double K);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_T4_in_s(double T);
        void set_T5_in_s(double T);
        void set_T6_in_s(double T);
        void set_T7_in_s(double T);
        void set_T8_in_s(double T);
        void set_T9_in_s(double T);
        void set_T10_in_s(double T);
        void set_Vssmax(double vmax);
        void set_Vssmin(double vmin);
        void set_Vcmax(double vmax);
        void set_Vcmin(double vmin);

        double get_K1() const;
        double get_K2() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_T4_in_s() const;
        double get_T5_in_s() const;
        double get_T6_in_s() const;
        double get_T7_in_s() const;
        double get_T8_in_s() const;
        double get_T9_in_s() const;
        double get_T10_in_s() const;
        double get_Vssmax() const;
        double get_Vssmin() const;
        double get_Vcmax() const;
        double get_Vcmin() const;

    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_stabilizing_signal_in_pu() const;

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const IEE2ST& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        FIRST_ORDER_BLOCK sensor_1, sensor_2;
        DIFFERENTIAL_BLOCK filter; FIRST_ORDER_BLOCK alternative_filter;
        LEAD_LAG_BLOCK phase_tuner_1, phase_tuner_2, phase_tuner_3;
        double Vssmax, Vssmin;
        double Vcmax, Vcmin;




};

#endif // IEE2ST_H
