#ifndef VSC_HVDC1_H_INCLUDED
#define VSC_HVDC1_H_INCLUDED

#include "header/model/vsc_hvdc_model/VSC_HVDC_MODEL.h"
#include "header/model/vsc_hvdc_model/vsc_station.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"
#include "header/basic/timer.h"

class VSC_HVDC1: public VSC_HVDC_MODEL
{
    public:
        VSC_HVDC1(STEPS& toolkit);
        VSC_HVDC1(const VSC_HVDC1& model);
        virtual ~VSC_HVDC1();
        virtual VSC_HVDC1& operator=(const VSC_HVDC1& model);
        void set_vsc_stations_count(unsigned int n);

    public: // specific exciter
        virtual string get_model_name() const;
        virtual void clear();
        void set_converter_bus(unsigned int index, unsigned int bus);
        void set_converter_active_control_mode(unsigned int index, unsigned int active_control_mode);
        void set_converter_reactive_control_mode(unsigned int index, unsigned int reactive_control_mode);
        void set_converter_active_power_control_T_in_s(unsigned int index, double t);
        void set_converter_active_power_Pmax_in_pu(unsigned int index, double Pmax);
        void set_converter_active_power_Pmin_in_pu(unsigned int index, double Pmin);
        void set_converter_active_power_kpp(unsigned int index, double k);
        void set_converter_reactive_power_control_T_in_s(unsigned int index, double t);
        void set_converter_reactive_power_Qmax_in_pu(unsigned int index, double Qmax);
        void set_converter_reactive_power_Qmin_in_pu(unsigned int index, double Qmin);
        void set_converter_reactive_power_kpq(unsigned int index, double k);
        void set_converter_dc_voltage_control_T_in_s(unsigned int index, double t);
        void set_converter_dc_voltage_Umax(unsigned int index, double Umax);
        void set_converter_dc_voltage_Umin(unsigned int index, double Umin);
        void set_converter_dc_voltage_kpud(unsigned int index, double k);
        void set_converter_ac_voltage_control_T_in_s(unsigned int index, double t);
        void set_converter_ac_voltage_Umax(unsigned int index, double Umax);
        void set_converter_ac_voltage_Umin(unsigned int index, double Umin);
        void set_converter_dc_voltage_kpuc(unsigned int index, double k);
        void set_dc_voltage_ceq(unsigned int index, double ceq);

        unsigned int get_vsc_stations_count() const;
        unsigned int get_converter_bus(unsigned int index) const;
        unsigned int get_converter_active_control_mode(unsigned int index) const;
        unsigned int get_converter_reactive_control_mode(unsigned int index) const;
        double get_converter_active_power_control_T_in_s(unsigned int index) const;
        double get_converter_active_power_Pmax_in_pu(unsigned int index) const;
        double get_converter_active_power_Pmin_in_pu(unsigned int index) const;
        double get_converter_active_power_kpp(unsigned int index) const;
        double get_converter_reactive_power_control_T_in_s(unsigned int index) const;
        double get_converter_reactive_power_Qmax_in_pu(unsigned int index) const;
        double get_converter_reactive_power_Qmin_in_pu(unsigned int index) const;
        double get_converter_reactive_power_kpq(unsigned int index) const;
        double get_converter_dc_voltage_control_T_in_s(unsigned int index) const;
        double get_converter_dc_voltage_Umax(unsigned int index) const;
        double get_converter_dc_voltage_Umin(unsigned int index) const;
        double get_converter_dc_voltage_kpud(unsigned int index) const;
        double get_converter_ac_voltage_control_T_in_s(unsigned int index) const;
        double get_converter_ac_voltage_Umax(unsigned int index) const;
        double get_converter_ac_voltage_Umin(unsigned int index) const;
        double get_converter_dc_voltage_kpuc(unsigned int index)const ;
        double get_dc_voltage_ceq(unsigned int index) const;


    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check_blocking_logic();
        virtual void check_bypassing_logic();
        virtual void check_mode_switching_logic();
        virtual void solve_hvdc_model_without_integration();
        virtual void check();
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

    private:
        void copy_from_const_model(const VSC_HVDC1& model);

    private:
        // vsc converter
        vector<VSC_MODEL_VAR> vsc_stations;
        unsigned int n_converter;
        // blocking related timers

        /*double time_when_rectifier_ac_voltage_below_delayed_blocking_voltage;
        double time_when_rectifier_ac_voltage_above_delayed_unblocking_voltage;
        double time_when_inverter_ac_voltage_below_instataneous_blocking_voltage;
        double time_when_inverter_ac_voltage_above_instataneous_unblocking_voltage;
        double time_when_inverter_ac_unblocking_signal_is_sent;

        double time_when_inverter_ac_voltage_below_delayed_bypassing_voltage;
        double time_when_inverter_ac_voltage_above_delayed_unbypassing_voltage;*/
};

#endif // VSC_HVDC1_H_INCLUDED
