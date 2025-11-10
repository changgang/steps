#ifndef SGFLSVS0_H
#define SGFLSVS0_H

#include "header/model/svs_model/svs_control_model/svs_control_model.h"
#include "header/block/pi_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"
#include <complex>

enum SVS_TYPE
{
    SVC = 1,
    SVG = 2
};

class SGFLSVS0 : public SVS_CONTROL_MODEL
{
    public:
        SGFLSVS0(STEPS& toolkit);
        SGFLSVS0(const SGFLSVS0& model);
        virtual ~SGFLSVS0();
        virtual SGFLSVS0& operator=(const SGFLSVS0& model);

        void set_svs_type(SVS_TYPE type);
        void set_Tc1_in_s(double Tc1);
        void set_Tb1_in_s(double Tb1);
        void set_Tc2_in_s(double Tc2);
        void set_Tb2_in_s(double Tb2);
        void set_Kp(double Kp);
        void set_Ki(double Ki);
        void set_T0_in_s(double T0);
        void set_Xc(double Xc);
        void set_Vemax(double Vemax);
        void set_Vemin(double Vemin);
        void set_Qnmax(double Qnmax);
        void set_Qnmin(double Qnmin);

        SVS_TYPE get_svs_type() const;
        double get_Tc1_in_s() const;
        double get_Tb1_in_s() const;
        double get_Tc2_in_s() const;
        double get_Tb2_in_s() const;
        double get_Kp() const;
        double get_Ki() const;
        double get_T0_in_s() const;
        double get_Xc() const;
        double get_Vemax() const;
        double get_Vemin() const;
        double get_Qnmax() const;
        double get_Qnmin() const;

        virtual string get_model_name() const;

    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void check_limits_in_initialization();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        double get_Q_nominal_in_pu_base_on_sbase();
        virtual double get_terminal_current_in_pu_based_on_sbase();

        void set_Vref_in_pu(double Vref);
        double get_Vref_in_pu();

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

    private:
        void copy_from_const_model(const SGFLSVS0& model);

        SVS_TYPE svs_type;
        LEAD_LAG_BLOCK voltage_sensor_block;
        LEAD_LAG_BLOCK phase_tuner_block;
        PI_BLOCK voltage_regulate_block;
        FIRST_ORDER_BLOCK firing_delay_block;
        double Xc, Vemax, Vemin;

        double Vref;
};
#endif // SGFLSVS0_H
