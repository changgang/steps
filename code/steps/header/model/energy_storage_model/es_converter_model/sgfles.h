#ifndef SGFLES_H
#define SGFLES_H

#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/csconv3.h"
#include "header/model/common_model/elec_model/frequency_controller.h"
#include "header/block/first_order_block.h"
#include "header/block/pi_block.h"
#include "header/block/integral_block.h"

class SGFLES : public ES_CONVERTER_MODEL
{
    public:
        SGFLES(STEPS& toolkit);
        SGFLES(const SGFLES& model);
        virtual ~SGFLES();
        virtual SGFLES& operator=(const SGFLES&);

        void set_KPLL(double K);
        void set_KIPLL(double K);
        void set_PLLmax(double pmax);
        void set_Imax_in_pu_based_on_mbase(double Imax);
        void set_Kvi(double K);
        void set_Tvi_in_s(double T);
        void set_Kdroop(double K);
        void set_Tdroop_in_s(double T);
        void set_Tpcmd(double T);
        void set_Tv(double Tv);
        void set_Tqcmd(double Tqcmd);
        void set_lvrt_voltage_threshold(double v);
        void set_k_Ip_lvrt_during(double k);
        void set_slope_Ip_lvrt_recover(double slope);
        void set_k_Iq_lvrt_during(double k);
        void set_slope_Iq_lvrt_recover(double slope);
        void set_hvrt_voltage_threshold(double v);
        void set_k_Iq_hvrt_during(double k);
        void set_slope_Iq_hvrt_recover(double slope);

        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;
        double get_Imax_in_pu_based_on_mbase() const;
        double get_Kvi() const;
        double get_Tvi_in_s() const;
        double get_Kdroop() const;
        double get_Tdroop_in_s() const;
        double get_Tpcmd() const;
        double get_Tv() const;
        double get_Tqcmd() const;
        double get_lvrt_voltage_threshold() const;
        double get_k_Ip_lvrt_during() const;
        double get_slope_Ip_lvrt_recover() const;
        double get_k_Iq_lvrt_during() const;
        double get_slope_Iq_lvrt_recover() const;
        double get_hvrt_voltage_threshold() const;
        double get_k_Iq_hvrt_during() const;
        double get_slope_Iq_hvrt_recover() const;

        double get_Ipcmd_in_pu_based_on_mbase();
        double get_Iqcmd_in_pu_based_on_mbase();

        double get_bus_voltage_in_pu() const;
        void set_voltage_reference_in_pu(double Vref);
        double get_voltage_reference_in_pu() const;

        bool is_P_elec_control_bypassed() const;
        bool is_Q_elec_control_bypassed() const;

        void check_lvrt_status();
        void set_lvrt_status(VRT_STATUS status);
        VRT_STATUS get_lvrt_status() const;
        void check_hvrt_status();
        void set_hvrt_status(VRT_STATUS status);
        VRT_STATUS get_hvrt_status() const;
        bool is_active_power_recovered() const;
        bool is_reactive_power_recovered() const;
        double get_lvrt_IP_command();
        double get_lvrt_IQ_command();
        double get_hvrt_IQ_command();

        void update_IPQ0_in_lvrt_during_status(double Ip0, double Iq0);
        void update_IPQ0_in_lvrt_recover_status(double Ip0, double Iq0);
        void set_time_when_lvrt_enter_recover_status(double t);
        void set_Ip0_lvrt_activated_in_pu(double I);
        void set_Iq0_lvrt_activated_in_pu(double I);
        void set_Ip0_lvrt_recover_activated_in_pu(double I);
        void set_Iq0_lvrt_recover_activated_in_pu(double I);
        double get_time_when_lvrt_enter_recover_status() const;
        double get_Ip0_lvrt_activated_in_pu() const;
        double get_Iq0_lvrt_activated_in_pu() const;
        double get_Ip0_lvrt_recover_activated_in_pu() const;
        double get_Iq0_lvrt_recover_activated_in_pu() const;

        void update_IPQ0_in_hvrt_during_status(double Ip0, double Iq0);
        void update_IPQ0_in_hvrt_recover_status(double Ip0, double Iq0);
        void set_time_when_hvrt_enter_recover_status(double t);
        void set_Ip0_hvrt_activated_in_pu(double I);
        void set_Iq0_hvrt_activated_in_pu(double I);
        void set_Ip0_hvrt_recover_activated_in_pu(double I);
        void set_Iq0_hvrt_recover_activated_in_pu(double I);
        double get_time_when_hvrt_enter_recover_status() const;
        double get_Ip0_hvrt_activated_in_pu() const;
        double get_Iq0_hvrt_activated_in_pu() const;
        double get_Ip0_hvrt_recover_activated_in_pu() const;
        double get_Iq0_hvrt_recover_activated_in_pu() const;

        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void check_limits_in_initialization();
        virtual void run(DYNAMIC_MODE mode);

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual double get_initial_active_current_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_current_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_voltage_command_in_pu() const;
        virtual double get_initial_active_power_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_power_command_in_pu_based_on_mbase() const;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();

        double get_pll_angle_in_rad();
        double get_pll_angle_in_deg();
        double get_pll_frequency_deviation_in_pu();
        double get_pll_frequency_deviation_in_Hz();
        double get_pll_frequency_in_pu();
        double get_pll_frequency_in_Hz();

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

    private:
        void copy_from_const_model(const SGFLES& model);

        CSCONV3 csconv3;
        double Imax;
        FREQUENCY_CONTROLLER frequency_controller;
        FIRST_ORDER_BLOCK active_power_regulator_filter;

        FIRST_ORDER_BLOCK reactive_current_regulator_filter;
        INTEGRAL_BLOCK voltage_regulator_integrator;

        double lvrt_voltage_threshold;
        double k_Ip_lvrt_during, slope_Ip_lvrt_recover;
        double k_Iq_lvrt_during, slope_Iq_lvrt_recover;

        double hvrt_voltage_threshold;
        double k_Iq_hvrt_during, slope_Iq_hvrt_recover;

        double Vref;
        VRT_STATUS lvrt_status, hvrt_status;
        bool P_recovered, Q_recovered;
        bool P_elec_control_bypassed, Q_elec_control_bypassed;
        double Ip0_lvrt_activated, Ip0_lvrt_recover_activated;
        double Iq0_lvrt_activated, Iq0_lvrt_recover_activated;
        double t0_lvrt_recover_activated;
        double Ip0_hvrt_activated, Ip0_hvrt_recover_activated;
        double Iq0_hvrt_activated, Iq0_hvrt_recover_activated;
        double t0_hvrt_recover_activated;

};

#endif // SGFLES
