#ifndef CDC4T_H
#define CDC4T_H

#include "header/model/hvdc_model/CDC4T.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"
#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"
class CDC4T: public EXCITER_MODEL
{
    public:
        CDC4T();
        CDC4T(const CDC4T& model);
        virtual ~CDC4T();
        virtual CDC4T& operator=(const CDC4T& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_dynamic_alpha_min_in_deg(double angle);
        void set_dynamic_gamma_min_in_deg(double angle);
        void set_dc_voltage_sensor_T_in_s(double t);
        void set_dc_current_sensor_T_in_s(double t);
        void set_rectifier_ac_blocking_voltage_in_pu(double v);
        void set_rectifier_ac_unblocking_voltage_in_pu(double v);
        void set_min_blocking_time_in_s(double t);
        void set_inverter_dc_bypassing_voltage_in_kV(double v);
        void set_inverter_ac_unbypassing_voltage_in_pu(double v);
        void set_min_bypassing_time_in_s(double t);
        void set_min_dc_voltage_following_block_in_kV(double v);
        void set_min_dc_current_following_block_in_A(double I);
        void set_voltage_recovery_rate_in_pu_per_s(double rate);
        void set_current_recovery_rate_in_pu_per_s(double rate);
        void set_VDCOL(const VDCOL& vdcol);
        void set_minimum_time_in_switched_mode_in_s(double t);

        double get_dynamic_alpha_min_in_deg() const;
        double get_dynamic_gamma_min_in_deg() const;
        double get_dc_voltage_sensor_T_in_s() const;
        double get_dc_current_sensor_T_in_s() const;
        double get_rectifier_ac_blocking_voltage_in_pu() const;
        double get_rectifier_ac_unblocking_voltage_in_pu() const;
        double get_min_blocking_time_in_s() const;
        double get_inverter_dc_bypassing_voltage_in_kV() const;
        double get_inverter_ac_unbypassing_voltage_in_pu() const;
        double get_min_bypassing_time_in_s() const;
        double get_min_dc_voltage_following_block_in_kV() const;
        double get_min_dc_current_following_block_in_A() const;
        double get_voltage_recovery_rate_in_pu_per_s() const;
        double get_current_recovery_rate_in_pu_per_s() const;
        VDCOL get_VDCOL() const;
        double get_minimum_time_in_switched_mode_in_s() const;



    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_excitation_voltage_in_pu() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual size_t get_variable_index_from_variable_name(string var_name);
        virtual string get_variable_name_from_variable_index(size_t var_index);
        virtual double get_variable_with_index(size_t var_index);
        virtual double get_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const CDC4T& model);
        FIRST_ORDER_BLOCK dc_voltage_sensor, dc_current_sensor;
        double dynamic_alpha_min_in_deg, dynamic_gamma_min_in_deg;
        double rectifier_ac_blocking_voltage_in_pu, rectifier_ac_unblocking_voltage_in_pu, minimum_blocking_time_in_s;
        double inverter_dc_bypassing_voltage_in_kV, inverter_ac_unbypassing_voltage_in_pu, minimum_bypassing_time_in_s;
        double minimum_dc_voltage_following_block_in_kV, minimum_dc_current_following_block_in_A;
        double voltage_recovery_rate_in_pu_per_s, current_recovery_rate_in_pu_per_s;
        VDCOL vdcol_limiter;
        double minimum_time_in_switched_mode;

};

#endif // EXCITER_MODEL_H
