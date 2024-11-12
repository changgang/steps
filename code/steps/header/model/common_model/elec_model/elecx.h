#ifndef ELECX_H
#define ELECX_H

#include "header/model/common_model/elec_model/elec_model.h"
#include "header/model/common_model/elec_model/frequency_controller.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"

class ELECX: public ELEC_MODEL
{
    public:
        ELECX(STEPS& toolkit);
        ELECX(const ELECX& model);
        virtual ~ELECX();
        virtual ELECX& operator=(const ELECX& model);
        virtual void clear();
        void copy_from_const_model(const ELECX& model);
        string get_model_name() const;
    public:
        void set_var_control_mode(ELEC_REACTIVE_POWER_CONTROL_MODE mode);
        ELEC_REACTIVE_POWER_CONTROL_MODE get_var_control_mode() const;

        void set_active_power_control_mode(ELEC_ACTIVE_POWER_CONTROL_MODE mode);
        ELEC_ACTIVE_POWER_CONTROL_MODE get_active_power_control_mode() const;

        void enable_active_power_closed_loop_control();
        void enable_active_power_open_loop_control();
        bool get_active_power_closed_control_flag() const;

        // reactive power control loop
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

        // frequency controller
        void set_Kvi(double K);
        void set_Tvi_in_s(double T);
        void set_Kdroop(double K);
        void set_Tdroop_in_s(double T);
        void set_frequency_deviation_upper_deadband_in_pu(double f);
        void set_frequency_deviation_lower_deadband_in_pu(double f);
        void set_Kfint(double K);

        double get_Kvi() const;
        double get_Tvi_in_s() const;
        double get_Kdroop() const;
        double get_Tdroop_in_s() const;
        double get_frequency_deviation_upper_deadband_in_pu() const;
        double get_frequency_deviation_lower_deadband_in_pu() const;
        double get_Kfint() const;

        //active power control loop
        //speed control with torque
        void set_Tspeed_in_s(double T);
        void set_KPP(double K);
        void set_KIP(double K);
        double get_Tspeed_in_s() const;
        double get_KPP() const;
        double get_KIP() const;

        //speed control
        void set_Kpspeed(double K);
        void set_Kispeed(double K);
        double get_Kpspeed() const;
        double get_Kispeed() const;

        //Vdc control
        void set_Kpdc(double K);
        void set_Kidc(double K);
        double get_Kpdc() const;
        double get_Kidc() const;

        //other active power parameters
        void set_rPmax_in_pu(double r);
        void set_rPmin_in_pu(double r);
        void set_TFP_in_s(double T);
        void set_Pmax_in_pu(double p);
        void set_Pmin_in_pu(double p);
        void set_IPmax_in_pu(double I);
        void set_Kpip(double K);
        void set_Kiip(double K);


        double get_rPmax_in_pu() const;
        double get_rPmin_in_pu() const;
        double get_TFP_in_s() const;
        double get_Pmax_in_pu() const;
        double get_Pmin_in_pu() const;
        double get_IPmax_in_pu() const;
        double get_Kpip() const;
        double get_Kiip() const;

        //state of block
        double get_voltage_sensor_state() const;
        double get_voltage_regulator_first_order_block_state() const;
        double get_voltage_regulator_integrator_state() const;
        double get_voltage_regulator_filter_state() const;
        double get_active_power_sensor_state() const;
        double get_Q_error_integrator_state() const;
        double get_V_error_integrator_state() const;
        double get_virtual_inertia_controller_state() const;
        double get_frequency_droop_controller_state() const;
        double get_frequency_integral_controller_state() const;
        double get_power_order_integrator_state() const;
        double get_speed_reference_sensor_state() const;
        double get_torque_controller_state() const;
    public:

        void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual double get_active_current_command_in_pu_based_on_mbase();
        virtual double get_active_power_command_in_pu_based_on_mbase() const;
        virtual double get_reactive_current_command_in_pu_based_on_mbase();
        virtual double get_reactive_power_command_in_pu_based_on_mbase();
        virtual double get_reactive_voltage_command_in_pu() const;
    private:

        ELEC_ACTIVE_POWER_CONTROL_MODE pe_control_mode;
        ELEC_REACTIVE_POWER_CONTROL_MODE pe_var_control_mode;
        bool active_power_closed_control_flag;

        //reactive power control block
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

        //active power control block
        FREQUENCY_CONTROLLER frequency_controller;
        FIRST_ORDER_BLOCK speed_reference_sensor;
        PI_BLOCK speed_controller;
        PI_BLOCK torque_controller;
        PI_BLOCK Vdc_controller;
        PI_BLOCK power_closed_loop_controller;

        double max_Pcmd_rate, min_Pcmd_rate;
        INTEGRAL_BLOCK power_order_integrator;
        double IPmax;

};

#endif // ELECX_H

