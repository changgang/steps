#ifndef CDC4T_H
#define CDC4T_H

#include "header/model/lcc_hvdc2t_model/lcc_hvdc2t_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"

class CDC4T: public LCC_HVDC2T_MODEL
{
    public:
        CDC4T(STEPS& toolkit);
        CDC4T(const CDC4T& model);
        virtual ~CDC4T();
        virtual CDC4T& operator=(const CDC4T& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_inverter_dc_voltage_sensor_T_in_s(double t);
        void set_dc_current_sensor_T_in_s(double t);
        void set_rectifier_ac_instantaneous_blocking_voltage_in_pu(double v);
        void set_rectifier_ac_instantaneous_unblocking_voltage_in_pu(double v);

        void set_inverter_dc_instantaneous_bypassing_voltage_in_kV(double v);
        void set_inverter_ac_instantaneous_unbypassing_voltage_in_pu(double v);

        double get_inverter_dc_voltage_sensor_T_in_s() const;
        double get_dc_current_sensor_T_in_s() const;
        double get_rectifier_ac_instantaneous_blocking_voltage_in_pu() const;
        double get_rectifier_ac_instantaneous_unblocking_voltage_in_pu() const;
        double get_inverter_dc_instantaneous_bypassing_voltage_in_kV() const;
        double get_inverter_ac_instantaneous_unbypassing_voltage_in_pu() const;

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
    private:
        void copy_from_const_model(const CDC4T& model);

        FIRST_ORDER_BLOCK inverter_dc_voltage_sensor, dc_current_sensor;
        double rectifier_ac_instantaneous_blocking_voltage_in_pu, rectifier_ac_instantaneous_unblocking_voltage_in_pu;
        double inverter_dc_instantaneous_bypassing_voltage_in_kV, inverter_ac_instantaneous_unbypassing_voltage_in_pu;
};

#endif // EXCITER_MODEL_H
