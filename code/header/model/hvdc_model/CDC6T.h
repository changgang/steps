#ifndef CDC6T_H
#define CDC6T_H

#include "header/model/hvdc_model/hvdc_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"
#include "header/basic/timer.h"

class CDC6T: public HVDC_MODEL
{
    public:
        CDC6T();
        CDC6T(const CDC6T& model);
        virtual ~CDC6T();
        virtual CDC6T& operator=(const CDC6T& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_inverter_dc_voltage_sensor_T_in_s(double t);
        void set_rectifier_dc_voltage_sensor_T_in_s(double t);
        void set_dc_current_sensor_T_in_s(double t);

        void set_rectifier_ac_instantaneous_blocking_voltage_in_pu(double v);
        void set_rectifier_ac_delayed_blocking_voltage_in_pu(double v);
        void set_rectifier_ac_delayed_blocking_time_in_s(double t);
        void set_inverter_ac_instantenous_blocking_voltage_in_pu(double v);
        void set_communication_delay_between_converters_in_s(double t);
        void set_rectifier_ac_delayed_unblocking_voltage_in_pu(double v);
        void set_rectifier_ac_delayed_unblocking_time_in_s(double t);
        void set_inverter_ac_delayed_unblocking_time_in_s(double t);

        void set_inverter_dc_instantaneous_bypassing_voltage_in_kV(double v);
        void set_inverter_ac_delayed_bypassing_voltage_in_pu(double v);
        void set_inverter_ac_delayed_bypassing_time_in_s(double t);
        void set_inverter_ac_delayed_unbypassing_voltage_in_pu(double v);
        void set_inverter_ac_delayed_unbypassing_time_in_s(double t);

        double get_inverter_dc_voltage_sensor_T_in_s() const;
        double get_rectifier_dc_voltage_sensor_T_in_s() const;
        double get_dc_current_sensor_T_in_s() const;

        double get_rectifier_ac_instantaneous_blocking_voltage_in_pu() const;
        double get_rectifier_ac_delayed_blocking_voltage_in_pu() const;
        double get_rectifier_ac_delayed_blocking_time_in_s() const;
        double get_inverter_ac_instantenous_blocking_voltage_in_pu() const;
        double get_communication_delay_between_converters_in_s() const;
        double get_rectifier_ac_delayed_unblocking_voltage_in_pu() const;
        double get_rectifier_ac_delayed_unblocking_time_in_s() const;
        double get_inverter_ac_delayed_unblocking_time_in_s() const;

        double get_inverter_dc_instantenous_bypassing_voltage_in_kV() const;
        double get_inverter_ac_delayed_bypassing_voltage_in_pu() const;
        double get_inverter_ac_delayed_bypassing_time_in_s() const;
        double get_inverter_ac_delayed_unbypassing_voltage_in_pu() const;
        double get_inverter_ac_delayed_unbypassing_time_in_s() const;
    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check_blocking_logic();
        virtual void check_bypassing_logic();
        virtual void check_mode_switching_logic();
        virtual void solve_hvdc_model_without_integration();
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
        void copy_from_const_model(const CDC6T& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        FIRST_ORDER_BLOCK inverter_dc_voltage_sensor, dc_current_sensor, rectifier_dc_voltage_sensor;

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

        /*double time_when_rectifier_ac_voltage_below_delayed_blocking_voltage;
        double time_when_rectifier_ac_voltage_above_delayed_unblocking_voltage;
        double time_when_inverter_ac_voltage_below_instataneous_blocking_voltage;
        double time_when_inverter_ac_voltage_above_instataneous_unblocking_voltage;
        double time_when_inverter_ac_unblocking_signal_is_sent;

        double time_when_inverter_ac_voltage_below_delayed_bypassing_voltage;
        double time_when_inverter_ac_voltage_above_delayed_unbypassing_voltage;*/
};

#endif // EXCITER_MODEL_H
