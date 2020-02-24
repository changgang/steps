#ifndef HVDC_MODEL_H
#define HVDC_MODEL_H

#include "header/basic/timer.h"
#include "header/model/hvdc_model/vdcol.h"
#include "header/basic/steps_enum.h"
#include "header/model/model.h"
#include "header/block/integral_block.h"
#include <complex>

class HVDC;
class BUS;

class HVDC_MODEL : public MODEL
{
    public:
        HVDC_MODEL(STEPS& toolkit);
        virtual ~HVDC_MODEL();
        HVDC* get_hvdc_pointer() const;
    public: // hvdc common
        virtual string get_model_type() const;
        // get input for initialization
        double get_initial_alpha_in_deg() const;
        double get_initial_gamma_in_deg() const;
        double get_auxiliary_signal_in_MW() const;

        // common setter and getter
        void set_converter_dynamic_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, double angle);
        void set_converter_dynamic_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter, double angle);

        void set_dc_voltage_command_recovery_rate_in_pu_per_second(double rate);
        void set_dc_current_command_recovery_rate_in_pu_per_second(double rate);

        void set_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing(double V);
        void set_minimum_dc_current_in_kA_following_unblocking(double I);

        void set_maximum_count_of_bypassing_before_blocked(unsigned int n);

        void set_mininum_blocking_time_in_s(double t);
        void set_mininum_bypassing_time_in_s(double t);
        void set_minimum_time_in_switched_mode_in_s(double t);

        double get_converter_dynamic_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_dynamic_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const;

        double get_dc_voltage_command_recovery_rate_in_pu_per_second() const;
        double get_dc_current_command_recovery_rate_in_pu_per_second() const;

        double get_minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing() const;
        double get_minimum_dc_current_in_kA_following_unblocking() const;

        void set_minimum_dc_current_command_in_kA(double I);
        double get_minimum_dc_current_command_in_kA() const;

        double get_maximum_count_of_bypassing_before_blocked() const;

        double get_mininum_blocking_time_in_s() const;
        double get_mininum_bypassing_time_in_s() const;
        double get_minimum_time_in_switched_mode_in_s() const;

        void set_VDCOL(const VDCOL& limiter);
        VDCOL get_VDCOL() const;

        void set_attached_device_of_common_meters();

        double get_inverter_dc_voltage_command_in_kV();
        double get_rectifier_dc_current_command_in_kA(double Vdci_measured, double Idc_measured);

        // block and bypass logic
        void block_hvdc();
        void unblock_hvdc();
        void manual_block_hvdc();
        void manual_unblock_hvdc();
        bool is_blocked() const;
        bool is_unblocking() const;
        bool is_manual_blocked() const;
        double get_blocked_time() const;
        double get_unblocking_time() const;
        void clear_unblocking_time(HVDC_CONVERTER_SIDE converter);
        bool is_block_timer_timed_out() const;

        void bypass_hvdc();
        void unbypass_hvdc();
        void manual_bypass_hvdc();
        void manual_unbypass_hvdc();
        bool is_manual_bypassed() const;
        bool is_bypassed() const;
        bool is_unbypassing() const;
        double get_bypassed_time() const;
        double get_unbypassing_time() const;
        void clear_unbypassing_time();
        bool is_bypass_timer_timed_out() const;

        void switch_hvdc_mode();
        void switch_hvdc_mode_back();
        bool is_mode_switched() const;
        double get_mode_switched_time() const;
        bool is_mode_switch_timer_timed_out() const;

        // common solver
        void solve_hvdc_model_without_line_dynamics(double Iset_kA, double Vset_kV);
        void solve_hvdc_model_with_line_dynamics(double Iset_kA, double Vset_kV);
        //

        double get_converter_dc_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_dc_current_in_kA(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_dc_power_in_MW(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_commutation_overlap_angle_in_deg(HVDC_CONVERTER_SIDE converter) const;
        complex<double> get_converter_ac_complex_power_in_MVA(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_ac_power_factor_angle_in_deg(HVDC_CONVERTER_SIDE converter) const;
        complex<double> get_converter_ac_current_in_pu(HVDC_CONVERTER_SIDE converter) const;
        complex<double> get_converter_ac_current_in_kA(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_ac_voltage_in_pu(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_ac_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const;
        complex<double> get_converter_ac_complex_voltage_in_pu(HVDC_CONVERTER_SIDE converter) const;

        double get_time_duration_to_the_last_bypass_in_s() const;
        double get_time_of_the_last_bypass_in_s() const;
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;
        void set_common_timer_toolkit();

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void check_blocking_logic() = 0;
        virtual void check_bypassing_logic() = 0;
        virtual void check_mode_switching_logic() = 0;
        virtual void solve_hvdc_model_without_integration() = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        void solve_hvdc_as_bypassed(double Iset_kA);
        void clear_record_of_bytime_time();
        void append_bypass_record(double time);
        unsigned int get_bypass_record_count() const;

        double firing_angle_max[2], firing_angle_min[2];
        TIMER block_timer, bypass_timer, mode_switch_timer;
        bool bypassed, mode_switched, manual_blocked, manual_bypassed;
        double time_when_unblocking, time_when_unbypassing, time_when_mode_unswitching;
        bool unblocking, unbypassing;
        bool dc_current_recovered_after_unblocking, dc_voltage_recovered_after_unblocking;

        double dc_voltage_command_recovery_rate_in_pu_per_second;
        double dc_current_command_recovery_rate_in_pu_per_second;

        double minimum_dc_voltage_in_kV_following_unblocking_and_unbypassing;
        double minimum_dc_current_in_kA_following_unblocking;

        double minimum_dc_current_command_in_kA;
        VDCOL vdcol_limiter;

        double record_of_bypass_time[STEPS_MAX_HVDC_BYPASS_RECORD_SIZE];
        unsigned int max_count_of_bypass_before_blocked;
};

#endif // HVDC_MODEL_H
