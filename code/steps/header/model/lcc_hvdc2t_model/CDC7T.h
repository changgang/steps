#ifndef CDC7T_H
#define CDC7T_H

#include "header/model/lcc_hvdc2t_model/lcc_hvdc2t_model.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"
#include "header/basic/timer.h"

class CDC7T: public LCC_HVDC2T_MODEL
{
    public:
        CDC7T(STEPS& toolkit);
        CDC7T(const CDC7T& model);
        virtual ~CDC7T();
        virtual CDC7T& operator=(const CDC7T& model);
    public: // specific exciter

        void set_inverter_dc_voltage_sensor_T_in_s(double t);
        void set_rectifier_dc_voltage_sensor_T_in_s(double t);
        void set_dc_line_dynamic_block_T_in_s(double t);
        void set_dc_line_dynamic_block_k(double k);

        void set_rectifier_dc_current_controller_kp(double kp);
        void set_rectifier_dc_current_controller_ki(double ki);
        void set_inverter_dc_current_margin_controller_kp(double kp);
        void set_inverter_dc_current_margin_controller_ki(double ki);
        void set_inverter_gamma_controller_kp(double kp);
        void set_inverter_gamma_controller_ki(double ki);

        double get_inverter_dc_voltage_sensor_T_in_s() const;
        double get_rectifier_dc_voltage_sensor_T_in_s() const;
        double get_dc_line_dynamic_block_T_in_s() const;
        double get_dc_line_dynamic_block_k() const;

        double get_rectifier_dc_current_controller_kp() const;
        double get_rectifier_dc_current_controller_ki() const;
        double get_inverter_dc_current_margin_controller_kp() const;
        double get_inverter_dc_current_margin_controller_ki() const;
        double get_inverter_gamma_controller_kp() const;
        double get_inverter_gamma_controller_ki() const;

        void set_converter_dynamic_gamma_reference_value_in_deg(double gamma_ref);
        void set_inverter_firing_angle_beta_of_current_control_in_deg(double beta_of_current_control);
        void set_inverter_firing_angle_beta_of_gamma_control_in_deg(double beta_of_gamma_control);

        double get_converter_dynamic_gamma_reference_value_in_deg() const;
        double get_inverter_firing_angle_beta_of_current_control_in_deg() const;
        double get_inverter_firing_angle_beta_of_gamma_control_in_deg() const;
        double get_rectifier_firing_angle_alpha_command_value_in_deg() const;
        double get_inverter_firing_angle_beta_command_value_in_deg() const;

        void set_line_L_in_H(double L_in_H);
        double get_line_L_in_H() const;
        void set_inverter_min_unbypass_gamma_value_in_deg(double unbypass_gamma);
        double get_inverter_min_unbypass_gamma_value_in_deg() const;
        void set_rectifier_min_unblock_alpha_value_in_deg(double unblock_alpha);
        double get_rectifier_min_unblock_alpha_value_in_deg() const;


        double solve_inverter_gamma_with_beta_in_deg(double beta_in_deg);

        //CDC6T FUNCTIONS
        void set_rectifier_ac_instantaneous_blocking_voltage_in_pu(double v);
        void set_rectifier_ac_delayed_blocking_voltage_in_pu(double v);
        void set_rectifier_ac_delayed_blocking_time_in_s(double t);
        void set_inverter_ac_instantaneous_blocking_voltage_in_pu(double v);
        void set_communication_delay_between_converters_in_s(double t);
        void set_rectifier_ac_delayed_unblocking_voltage_in_pu(double v);
        void set_rectifier_ac_delayed_unblocking_time_in_s(double t);
        void set_inverter_ac_delayed_unblocking_time_in_s(double t);

        void set_inverter_dc_instantaneous_bypassing_voltage_in_kV(double v);
        void set_inverter_ac_delayed_bypassing_voltage_in_pu(double v);
        void set_inverter_ac_delayed_bypassing_time_in_s(double t);
        void set_inverter_ac_delayed_unbypassing_voltage_in_pu(double v);
        void set_inverter_ac_delayed_unbypassing_time_in_s(double t);


        double get_rectifier_ac_instantaneous_blocking_voltage_in_pu() const;
        double get_rectifier_ac_delayed_blocking_voltage_in_pu() const;
        double get_rectifier_ac_delayed_blocking_time_in_s() const;
        double get_inverter_ac_instantaneous_blocking_voltage_in_pu() const;
        double get_communication_delay_between_converters_in_s() const;
        double get_rectifier_ac_delayed_unblocking_voltage_in_pu() const;
        double get_rectifier_ac_delayed_unblocking_time_in_s() const;
        double get_inverter_ac_delayed_unblocking_time_in_s() const;

        double get_inverter_dc_instantaneous_bypassing_voltage_in_kV() const;
        double get_inverter_ac_delayed_bypassing_voltage_in_pu() const;
        double get_inverter_ac_delayed_bypassing_time_in_s() const;
        double get_inverter_ac_delayed_unbypassing_voltage_in_pu() const;
        double get_inverter_ac_delayed_unbypassing_time_in_s() const;


    public:
        virtual string get_model_name() const;
        virtual bool is_pi_dynamics_considered() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void check_limits_in_initialization();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check_blocking_logic();
        virtual void check_bypassing_logic();
        virtual void check_mode_switching_logic();
        virtual void solve_2t_lcc_hvdc_model_without_integration();
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

        virtual double get_internal_idc_command_in_kA() const ;
        virtual double get_internal_alpha_command_in_deg() const ;
        virtual double get_internal_beta_command_in_deg_of_current_control() const ;
        virtual double get_internal_beta_command_in_deg_of_gamma_control() const ;

    private:
        void copy_from_const_model(const CDC7T& model);

        FIRST_ORDER_BLOCK inverter_dc_voltage_sensor, rectifier_dc_voltage_sensor;
        FIRST_ORDER_BLOCK dc_line_dynamic_block;
        PI_BLOCK rectifier_dc_current_controller;
        PI_BLOCK inverter_dc_current_margin_controller;
        PI_BLOCK inverter_gamma_controller;

        double dc_current_reference_value_in_kA;
        double inverter_gamma_reference_value_in_deg;
        double inverter_firing_angle_beta_of_current_control_in_deg;
        double inverter_firing_angle_beta_of_gamma_control_in_deg;
        //double inverter_firing_angle_beta_command_value;

        double inverter_min_unbypass_gamma_value_in_deg;
        double rectifier_min_unblock_alpha_value_in_deg;
        double line_L_in_H;

        bool is_pi_controller_dynamics_considered;

        //CDC6T VARIABLES
        double rectifier_ac_instantaneous_blocking_voltage_in_pu;
        double rectifier_ac_delayed_blocking_voltage_in_pu, rectifier_ac_delayed_blocking_time_in_s;
        double inverter_ac_instantaneous_blocking_voltage_in_pu;
        double minimum_blocking_time_in_s;
        double rectifier_ac_delayed_unblocking_voltage_in_pu, rectifier_ac_delayed_unblocking_time_in_s;
        double inverter_ac_delayed_unblocking_time_in_s;

        double communication_delay_between_converters_in_s;

        double inverter_dc_instantaneous_bypassing_voltage_in_kV;
        double inverter_ac_delayed_bypassing_voltage_in_pu, inverter_ac_delayed_bypassing_time_in_s;
        double minimum_bypassing_time_in_s;
        double inverter_ac_delayed_unbypassing_voltage_in_pu, inverter_ac_delayed_unbypassing_time_in_s;

        double minimum_time_in_switched_mode;
        //END OF CDC6T

    private:
        // blocking related timers
        TIMER rec_ac_blocking_timer,
              inv_ac_blocking_signal_transmitting_timer,
              rec_ac_unblocking_timer,
              inv_ac_unblocking_timer,
              inv_ac_unblocking_signal_transmitting_timer;
        // bypassing related timers
        TIMER inv_ac_bypassing_timer,
              inv_ac_unbypassing_timer;
};

#endif // EXCITER_MODEL_H
