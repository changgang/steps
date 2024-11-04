#ifndef ELEC2_H
#define ELEC2_H

#include "header/model/common_model/elec_model/elec_model.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"

class ELEC2: public ELEC_MODEL
{
    public:
        ELEC2(STEPS& toolkit);
        ELEC2(const ELEC2& model);
        virtual ~ELEC2();
        virtual ELEC2& operator=(const ELEC2& model);
        virtual void clear();
        void copy_from_const_model(const ELEC2& model);
        string get_model_name() const;
    public:
        void set_var_control_mode(REACTIVE_POWER_CONTROL_MODE mode);
        REACTIVE_POWER_CONTROL_MODE get_var_control_mode() const;

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

        void set_Kvi(double K);
        void set_Tvi_in_s(double T);
        void set_Kdroop(double K);
        void set_Tdroop_in_s(double T);
        void set_frequency_deviation_upper_deadband_in_pu(double f);
        void set_frequency_deviation_lower_deadband_in_pu(double f);
        void set_Kfint(double K);
        void set_KIF(double K);
        void set_KPF(double K);
        void set_Ipmax_in_pu(double I);
        void set_Ipmin_in_pu(double I);

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

        double get_Kvi() const;
        double get_Tvi_in_s() const;
        double get_Kdroop() const;
        double get_Tdroop_in_s() const;
        double get_frequency_deviation_upper_deadband_in_pu() const;
        double get_frequency_deviation_lower_deadband_in_pu() const;
        double get_Kfint() const;
        double get_KIF() const;
        double get_KPF() const;
        double get_Ipmax_in_pu() const;
        double get_Ipmin_in_pu() const;

        double get_voltage_sensor_state() const;
        double get_voltage_regulator_first_order_block_state() const;
        double get_voltage_regulator_integrator_state() const;
        double get_voltage_regulator_filter_state() const;
        double get_active_power_sensor_state() const;
        double get_Q_error_integrator_state() const;
        double get_V_error_integrator_state() const;
        double get_virtual_inertia_emulator_state() const;
        double get_frequency_droop_controller_state() const;
        double get_frequency_integral_controller_state() const;
        double get_Ipcmd_controller_state() const;
    public:

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual double get_active_current_command_in_pu_based_on_mbase();
        virtual double get_active_power_command_in_pu_based_on_mbase() const;
        virtual double get_reactive_current_command_in_pu_based_on_mbase();
        virtual double get_reactive_power_command_in_pu_based_on_mbase();
        virtual double get_reactive_voltage_command_in_pu() const;

    private:

        double Xcomp;
        FIRST_ORDER_BLOCK voltage_sensor;
        double Fn;
        FIRST_ORDER_BLOCK voltage_regulator_first_order_block;
        INTEGRAL_BLOCK voltage_regulator_integrator;
        FIRST_ORDER_BLOCK voltage_regulator_filter;
        FIRST_ORDER_BLOCK active_power_sensor;
        INTEGRAL_BLOCK Q_error_integrator;
        REACTIVE_POWER_CONTROL_MODE pe_var_control_mode;
        unsigned int Voltage_Flag;
        INTEGRAL_BLOCK V_error_integrator;
        double EQmax, EQmin;

        DIFFERENTIAL_BLOCK virtual_inertia_emulator;
        FIRST_ORDER_BLOCK frequency_droop_controller;

        double f_upper_pu, f_lower_pu;
        INTEGRAL_BLOCK frequency_integral_controller;
        PI_BLOCK Ipcmd_controller;
        double Ipmax, Ipmin;

        //bool frequency_regulation_enabled;
};

#endif // ELEC2_H

