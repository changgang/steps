#ifndef MTHVDCVSC1_H
#define MTHVDCVSC1_H


#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_vsc_model/hvsc_model.h"
#include "header/block/pid_block.h"
#include "header/block/first_order_block.h"


class MTHVDCVSC1 : public HVSC_MODEL
{
    public:
        MTHVDCVSC1(STEPS& toolkit);
        ~MTHVDCVSC1();
        MTHVDCVSC1(const MTHVDCVSC1& model);
        virtual MTHVDCVSC1& operator=(const MTHVDCVSC1& model);
        void copy_from_const_model(const MTHVDCVSC1& model);

    public:
        virtual string get_model_name() const;


        void set_active_power_block_Kp(double kp);
        void set_active_power_block_Ki(double ki);
        void set_active_power_block_Kd(double kd);
        void set_active_power_block_Td_in_s(double td);
        void set_active_power_block_pmax(double pmax);
        void set_active_power_block_pmin(double pmin);

        void set_reactive_power_block_Kp(double kp);
        void set_reactive_power_block_Ki(double ki);
        void set_reactive_power_block_Kd(double kd);
        void set_reactive_power_block_Td_in_s(double td);
        void set_reactive_power_block_qmax(double qmax);
        void set_reactive_power_block_qmin(double qmin);
        void set_dc_voltage_block_ceq_in_uF(const double ceq);

        double get_active_power_block_Kp() const;
        double get_active_power_block_Ki() const;
        double get_active_power_block_Kd() const;
        double get_active_power_block_Td_in_s() const;
        double get_active_power_block_pmax() const;
        double get_active_power_block_pmin() const;
        double get_reactive_power_block_Kp() const;
        double get_reactive_power_block_Ki() const;
        double get_reactive_power_block_Kd() const;
        double get_reactive_power_block_Td_in_s() const;
        double get_reactive_power_block_qmax() const;
        double get_reactive_power_block_qmin() const;
        double get_dc_voltage_block_ceq_in_uF() const ;

        void set_active_power_block_Pref(double pref);
        void set_active_power_block_Udcref(double udcref);

        void set_reactive_power_block_Qref(double qref);
        void set_reactive_power_block_Uacref(double uacref);

        double get_active_power_block_Pref() const;
        double get_active_power_block_Udcref() const;

        double get_reactive_power_block_Qref() const;
        double get_reactive_power_block_Uacref() const;


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
        virtual void update_converters();

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;


    public://input for initialize

    public://output
        virtual double get_vsc_converter_ueq_in_kV() const;
        virtual double get_vsc_converter_req_ohm() const;

        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_dq_axis_in_pu_on_converter_base() const;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_converter_base() const;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_pu_on_system_base() const;
        virtual complex<double> get_converter_dynamic_current_from_ac_bus_to_converter_in_xy_axis_in_kA() const;
        virtual complex<double> get_converter_dynamic_ac_complex_power() const;
        virtual double get_converter_dynamic_active_power_in_MW() const;
        virtual double get_converter_dynamic_reactive_power_in_Mvar() const;
        virtual double get_converter_dynamic_current_from_converter_to_ceq_in_kA() const;

        virtual double get_converter_dynamic_power_of_converter_to_ceq() const;

    private:
        //BLOCK
        PID_BLOCK p_block;
        PID_BLOCK q_block;
        FIRST_ORDER_BLOCK p_sensor, q_sensor;
        INTEGRAL_BLOCK udc_block;

        double p_ref, q_ref, udc_ref, uac_ref;
        double ceq_in_uF;

};

#endif // HLCC_MODEL_H
