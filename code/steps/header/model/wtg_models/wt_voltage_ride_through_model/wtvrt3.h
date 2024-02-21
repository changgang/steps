#ifndef WTVRT3_H
#define WTVRT3_H

#include "header/model/wtg_models/wt_voltage_ride_through_model/wt_voltage_ride_through_model.h"
#include "header/model/converter_common_models/vrt_model/lvrt_control.h"
#include "header/model/converter_common_models/vrt_model/hvrt_control.h"

class WTVRT3: public WT_VOLTAGE_RIDE_THROUGH_MODEL
{
    public:
        WTVRT3(STEPS& toolkit);
        WTVRT3(const WTVRT3& model);
        virtual ~WTVRT3();
        virtual WTVRT3& operator=(const WTVRT3& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_vrt_voltage_threshold(char HL_flag, double v);
        void set_vrt_prepare2recover_delay_time_in_s(char HL_flag, double t);

        void set_P_vrt_during_mode(char HL_flag, VRT_DURING_CONTROL_MODE m);
        void set_kP(char HL_flag, double k);
        void set_P_set_in_pu(char HL_flag, double p);
        void set_k1_Ip(char HL_flag, double k);
        void set_k2_Ip(char HL_flag, double k);
        void set_Ip_set_in_pu(char HL_flag, double I);
        void set_LVPL_low_voltage_in_pu(double v);
        void set_LVPL_high_voltage_in_pu(double v);
        void set_LVPL_gain_at_high_voltage(double g);

        void set_P_vrt_prepare2recover_mode(char HL_flag, VRT_PREPARE2RECOVER_CONTROL_MODE m);
        void set_k_Ip_prepare2recover(char HL_flag, double k);
        void set_Ip_set_prepare2recover(char HL_flag, double I);

        void set_P_vrt_recover_mode(char HL_flag, VRT_RECOVER_CONTROL_MODE m);
        void set_P_recover_slope_in_pu_per_s(char HL_flag, double s);
        void set_P_recover_inertia_in_s(char HL_flag, double T);

        void set_Q_vrt_during_mode(char HL_flag, VRT_DURING_CONTROL_MODE m);
        void set_kQ(char HL_flag, double k);
        void set_Q_set_in_pu(char HL_flag, double q);
        void set_k1_Iq(char HL_flag, double k);
        void set_k2_Iq(char HL_flag, double k);
        void set_Iq_set_in_pu(char HL_flag, double I);

        void set_Q_vrt_prepare2recover_mode(char HL_flag, VRT_PREPARE2RECOVER_CONTROL_MODE m);
        void set_k_Iq_prepare2recover(char HL_flag, double k);
        void set_Iq_set_prepare2recover(char HL_flag, double I);

        void set_Q_vrt_recover_mode(char HL_flag, VRT_RECOVER_CONTROL_MODE m);
        void set_Q_recover_slope_in_pu_per_s(char HL_flag, double s);
        void set_Q_recover_inertia_in_s(char HL_flag, double T);

        double get_vrt_voltage_threshold(char HL_flag) const;
        double get_vrt_prepare2recover_delay_time_in_s(char HL_flag) const;

        VRT_DURING_CONTROL_MODE get_P_vrt_during_mode(char HL_flag) const;
        double get_kP(char HL_flag) const;
        double get_P_set_in_pu(char HL_flag) const;
        double get_k1_Ip(char HL_flag) const;
        double get_k2_Ip(char HL_flag) const;
        double get_Ip_set_in_pu(char HL_flag) const;
        double get_LVPL_low_voltage_in_pu() const;
        double get_LVPL_high_voltage_in_pu() const;
        double get_LVPL_gain_at_high_voltage() const;

        VRT_PREPARE2RECOVER_CONTROL_MODE get_P_vrt_prepare2recover_mode(char HL_flag) const;
        double get_k_Ip_prepare2recover(char HL_flag) const;
        double get_Ip_set_prepare2recover(char HL_flag) const;

        VRT_RECOVER_CONTROL_MODE get_P_vrt_recover_mode(char HL_flag) const;
        double get_P_recover_slope_in_pu_per_s(char HL_flag) const;
        double get_P_recover_inertia_in_s(char HL_flag) const;

        VRT_DURING_CONTROL_MODE get_Q_vrt_during_mode(char HL_flag) const;
        double get_kQ(char HL_flag) const;
        double get_Q_set_in_pu(char HL_flag) const;
        double get_k1_Iq(char HL_flag) const;
        double get_k2_Iq(char HL_flag) const;
        double get_Iq_set_in_pu(char HL_flag) const;

        VRT_PREPARE2RECOVER_CONTROL_MODE get_Q_vrt_prepare2recover_mode(char HL_flag) const;
        double get_k_Iq_prepare2recover(char HL_flag) const;
        double get_Iq_set_prepare2recover(char HL_flag) const;

        VRT_RECOVER_CONTROL_MODE get_Q_vrt_recover_mode(char HL_flag) const;
        double get_Q_recover_slope_in_pu_per_s(char HL_flag) const;
        double get_Q_recover_inertia_in_s(char HL_flag) const;

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
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WTVRT3& model);

        LVRT_CONTROL lvrt;
        HVRT_CONTROL hvrt;
};

#endif // EXCITER_MODEL_H
