#ifndef WT3E0_H
#define WT3E0_H

#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"

class WT3E0: public WT_ELECTRICAL_MODEL
{
    public:
        WT3E0(STEPS& toolkit);
        WT3E0(const WT3E0& model);
        virtual ~WT3E0();
        virtual WT3E0& operator=(const WT3E0& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_Xcomp_in_pu(double Xc);
        void set_TRV_in_s(double T);
        void set_Fn(double Fn);
        void set_KIV(double K);
        void set_Qmax_in_pu(double q);
        void set_Qmin_in_pu(double q);
        void set_KPV(double K);
        void set_TV_in_s(double T);
        void set_TFV_in_s(double T);
        void set_TP_in_s(double T);
        void set_KQI(double K);
        void set_Vmax_in_pu(double v);
        void set_Vmin_in_pu(double v);
        void set_voltage_flag(unsigned int flag);
        void set_KQV(double K);
        void set_EQmax_in_pu(double I);
        void set_EQmin_in_pu(double I);

        void set_Tspeed_in_s(double T);
        void set_KPP(double K);
        void set_KIP(double K);
        void set_Kvi(double K);
        void set_Tvi_in_s(double T);
        void set_Kdroop(double K);
        void set_Tdroop_in_s(double T);
        void set_frequency_deviation_upper_deadband_in_pu(double f);
        void set_frequency_deviation_lower_deadband_in_pu(double f);
        void set_Kfint(double K);
        void set_rPmax_in_pu(double r);
        void set_rPmin_in_pu(double r);
        void set_TFP_in_s(double T);
        void set_Pmax_in_pu(double p);
        void set_Pmin_in_pu(double p);
        void set_IPmax_in_pu(double I);

        double get_Xcomp_in_pu() const;
        double get_TRV_in_s() const;
        double get_Fn() const;
        double get_KIV() const;
        double get_Qmax_in_pu() const;
        double get_Qmin_in_pu() const;
        double get_KPV() const;
        double get_TV_in_s() const;
        double get_TFV_in_s() const;
        double get_TP_in_s() const;
        double get_KQI() const;
        double get_Vmax_in_pu() const;
        double get_Vmin_in_pu() const;
        unsigned int get_voltage_flag() const;
        double get_KQV() const;
        double get_EQmax_in_pu() const;
        double get_EQmin_in_pu() const;

        double get_Tspeed_in_s() const;
        double get_KPP() const;
        double get_KIP() const;
        double get_Kvi() const;
        double get_Tvi_in_s() const;
        double get_Kdroop() const;
        double get_Tdroop_in_s() const;
        double get_frequency_deviation_upper_deadband_in_pu() const;
        double get_frequency_deviation_lower_deadband_in_pu() const;
        double get_Kfint() const;
        double get_TFP_in_s() const;
        double get_rPmax_in_pu() const;
        double get_rPmin_in_pu() const;
        double get_Pmax_in_pu() const;
        double get_Pmin_in_pu() const;
        double get_IPmax_in_pu() const;

        bool is_frequency_regulation_enabled() const;

        void set_speed_reference_bias_in_pu(double bias);
        double get_speed_reference_bias_in_pu() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_active_current_command_in_pu_based_on_mbase();
        virtual double get_active_power_command_in_pu_based_on_mbase() const;
        virtual double get_reactive_current_command_in_pu_based_on_mbase();
        virtual double get_reactive_power_command_in_pu_based_on_mbase();
        virtual double get_reactive_voltage_command_in_pu() const;


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
        void copy_from_const_model(const WT3E0& model);

        void trip_frequency_regulation();

        double Xcomp;
        FIRST_ORDER_BLOCK voltage_sensor;
        double Fn;
        FIRST_ORDER_BLOCK voltage_regulator_first_order_block;
        INTEGRAL_BLOCK voltage_regulator_integrator;
        FIRST_ORDER_BLOCK voltage_regulator_filter;
        FIRST_ORDER_BLOCK active_power_sensor;
        INTEGRAL_BLOCK Q_error_integrator;
        unsigned int Voltage_Flag;
        INTEGRAL_BLOCK V_error_integrator;
        double EQmax, EQmin;

        FIRST_ORDER_BLOCK wind_turbine_speed_reference_sensor;
        PI_BLOCK torque_PI_regulator;
        DIFFERENTIAL_BLOCK virtual_inertia_emulator;
        FIRST_ORDER_BLOCK frequency_droop_controller;

        double f_upper_pu, f_lower_pu;
        INTEGRAL_BLOCK frequency_integral_controller;
        double max_torque_rate, min_torque_rate;
        INTEGRAL_BLOCK power_order_integrator;
        double IPmax;

        bool frequency_regulation_enabled;

        double speedref_bias;
};

#endif // EXCITER_MODEL_H
