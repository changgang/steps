#ifndef PV_ELECTRICAL_MODEL_H
#define PV_ELECTRICAL_MODEL_H

#include "header/model/pvu_models/pvu_model.h"
#include <complex>
class PV_ELECTRICAL_MODEL : public PVU_MODEL
{
    /*
    PV electrical control model
    General inputs:
        vterm: pv unit terminal or remote AC voltage
        iterm: pv unit terminal current
        freq: terminal AC frequency
        pelec: pv unit active power generation
        qelec: pv unit reactive power generation
        speed: pv unit speed in pu
        speedref: pv unit speed reference in pu
    General output:
        ipcmd: active current command in pu
        iqcmd: reactive current command in pu
    */
    public:
        PV_ELECTRICAL_MODEL(STEPS& toolkit);
        virtual ~PV_ELECTRICAL_MODEL();

        void unbypass_model();
        void bypass_model();
        bool is_model_bypassed() const;
    public: // pe elctricla control common
        virtual string get_model_type() const;
        // get input
        complex<double> get_pv_unit_terminal_generation_in_MVA() const;
        complex<double> get_pv_unit_terminal_generation_in_pu_based_on_mbase() const;
        complex<double> get_terminal_bus_complex_voltage_in_pu() const;
        double get_terminal_bus_voltage_in_pu() const;
        complex<double> get_pv_unit_terminal_complex_current_in_pu() const;
        double get_pv_unit_terminal_current_in_pu() const;

        double get_terminal_bus_frequency_in_pu() const;
        double get_terminal_bus_frequency_deviation_in_pu() const;

        double get_active_power_reference_in_pu_from_panel_model() const;
        // reference
        void set_bus_to_regulate(unsigned int bus);
        unsigned int get_bus_to_regulate()  const;

        void set_voltage_reference_in_pu(double vref);
        void set_voltage_reference_in_pu_with_bus_to_regulate();
        double get_voltage_reference_in_pu() const;
        void set_frequency_reference_in_pu(double fref);
        double get_frequency_reference_in_pu() const;
        void set_active_power_reference_in_pu(double pref);
        double get_active_power_reference_in_pu() const;
        void set_reactive_power_reference_in_pu(double qref);
        double get_reactive_power_reference_in_pu() const;
        void set_power_factor_reference_in_pu(double pfref);
        double get_power_factor_reference_in_pu() const;
        void set_var_control_mode(PE_VAR_CONTROL_MODE mode);
        PE_VAR_CONTROL_MODE get_var_control_mode() const;
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_active_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_active_power_command_in_pu_based_on_mbase() const = 0;
        virtual double get_reactive_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_power_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_voltage_command_in_pu() const = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

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
        unsigned int bus_to_regulate;
        double voltage_reference_in_pu;
        double frequency_reference_in_pu;
        double active_power_reference_in_pu;
        double reactive_power_reference_in_pu;
        double power_factor_reference_in_pu;
        PE_VAR_CONTROL_MODE pe_var_control_mode;

        bool flag_model_bypassed;
};

#endif // PV_ELECTRICAL_MODEL_H
