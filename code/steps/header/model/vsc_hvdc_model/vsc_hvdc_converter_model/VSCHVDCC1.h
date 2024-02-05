#ifndef VSCHVDCC1_H_INCLUDED
#define VSCHVDCC1_H_INCLUDED

#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"
#include "header/block/integral_block.h"
#include "header/block/pid_block.h"
#include "header/block/pi_block.h"
#include "header/block/lead_lag_block.h"
#include "header/block/first_order_block.h"

class VSCHVDCC1: public VSC_HVDC_CONVERTER_MODEL
{
    public:
        VSCHVDCC1(STEPS& toolkit);
        VSCHVDCC1(const VSCHVDCC1& model);
        virtual ~VSCHVDCC1();
        virtual VSCHVDCC1& operator=(const VSCHVDCC1& model);
    public:
        virtual string get_model_name() const;

        void set_dc_voltage_block_ceq_in_uF(double c);

        void set_active_power_sensor_T_in_s(double t);
        void set_reactive_power_sensor_T_in_s(double t);
        void set_frequency_sensor_T_in_s(double t);
        void set_ac_voltage_sensor_T_in_s(double t);

        void set_frequency_tuner_T1_in_s(double t);
        void set_frequency_tuner_T2_in_s(double t);
        void set_frequency_responser_Kp(double k);
        void set_frequency_responser_Ki(double k);
        void set_omega_block_Tj_in_s(double t);
        void set_omega_block_D_in_pu(double d);
        void set_active_current_reducer_T_in_s(double t);

        void set_voltage_tuner_T1_in_s(double t);
        void set_voltage_tuner_T2_in_s(double t);
        void set_voltage_responser_Kp(double k);
        void set_voltage_responser_Ki(double k);
        void set_voltage_block_Tv_in_s(double t);
        void set_voltage_block_D_in_pu(double t);
        void set_reactive_current_reducer_T_in_s(double t);
        void set_dc_voltage_response_block_Kp(double k);
        void set_dc_voltage_response_block_Ki(double k);

        double get_dc_voltage_block_ceq_in_uF() const;

        double get_active_power_sensor_T_in_s() const;
        double get_reactive_power_sensor_T_in_s() const;
        double get_frequency_sensor_T_in_s() const;
        double get_ac_voltage_sensor_T_in_s() const;

        double get_frequency_tuner_T1_in_s() const;
        double get_frequency_tuner_T2_in_s() const;
        double get_frequency_responser_Kp() const;
        double get_frequency_responser_Ki() const;
        double get_omega_block_Tj_in_s() const;
        double get_omega_block_D_in_pu() const;
        double get_active_current_reducer_T_in_s() const;

        double get_voltage_tuner_T1_in_s() const;
        double get_voltage_tuner_T2_in_s() const;
        double get_voltage_responser_Kp() const;
        double get_voltage_responser_Ki() const;
        double get_voltage_block_Tv_in_s() const;
        double get_voltage_block_D_in_pu() const;
        double get_reactive_current_reducer_T_in_s() const;

        double get_dc_voltage_response_block_Kp() const;
        double get_dc_voltage_response_block_Ki() const;

        void set_Pref_in_pu(double pref);
        void set_Qref_in_pu(double qref);
        void set_Uacref_in_pu(double uacref);

        double get_Pref_in_pu() const;
        double get_Qref_in_pu() const;
        double get_Uacref_in_pu() const;

        double get_Porder_in_pu() const;
        double get_Qorder_in_pu() const;

        double get_dynamic_dc_voltage_in_kV() const;
        double get_dynamic_dc_voltage_in_pu() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:
        virtual double get_initial_angle_at_pll_in_rad() const;
        virtual double get_dynamic_angle_at_pll_in_rad() const;
        virtual complex<double> get_converter_voltage_in_xy_axis_in_pu_as_voltage_source() const ;
        virtual complex<double> get_converter_dynamic_current_from_converter_to_ac_bus_in_dq_axis_in_pu_on_converter_base_as_current_source() const ;
    private:
        void copy_from_const_model(const VSCHVDCC1& model);
        double get_converter_dc_power_from_converter_to_Ceq_in_MW();
    private:
        INTEGRAL_BLOCK udc_block;

        FIRST_ORDER_BLOCK p_sensor, f_sensor;
        INTEGRAL_BLOCK omega_block, angle_block;
        PI_BLOCK dc_voltage_response_block;
        PI_BLOCK frequency_response_block;
        LEAD_LAG_BLOCK frequency_tuner;
        INTEGRAL_BLOCK active_current_reducer;
        double D_omega;

        double p_ref;
        double vdc_ref;

        FIRST_ORDER_BLOCK q_sensor, uac_sensor;
        INTEGRAL_BLOCK voltage_block;
        PI_BLOCK voltage_response_block;
        LEAD_LAG_BLOCK voltage_tuner;
        INTEGRAL_BLOCK reactive_current_reducer;
        double D_voltage;

        double q_ref, uac_ref;
};
#endif // VSCHVDCC1_H_INCLUDED
