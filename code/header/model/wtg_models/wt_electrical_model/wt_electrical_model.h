#ifndef WT_ELECTRICAL_MODEL_H
#define WT_ELECTRICAL_MODEL_H

#include "header/model/wtg_models/wt_electrical_model/wind_turbine_power_speed_lookup_table.h"
#include "header/model/wtg_models/wtg_model.h"
#include <complex>
class WT_ELECTRICAL_MODEL : public WTG_MODEL
{
    /*
    PE Electrical control model
    General inputs:
        vterm: wt generator terminal or remote AC voltage
        iterm: wt generator terminal current
        freq: terminal AC frequency
        pelec: wt generator active power generation
        qelec: wt generator reactive power generation
        speed: wt generator speed in pu
        speedref: wt generator speed reference in pu
    General output:
        ipcmd: active current command in pu
        iqcmd: reactive current command in pu
    */
    public:
        WT_ELECTRICAL_MODEL();
        virtual ~WT_ELECTRICAL_MODEL();
    public: // pe elctricla control common
        virtual string get_model_type() const;
        // get input
        complex<double> get_source_generation_in_MVA() const;
        double get_source_bus_voltage_in_pu() const;
        double get_source_bus_frequency_in_pu() const;
        double get_source_bus_frequency_deviation_in_pu() const;
        // reference
        void set_bus_to_regulate(size_t bus);
        size_t get_bus_to_regulate()  const;

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
        void set_wind_turbine_reference_speed_in_pu(double speed);


        complex<double> get_complex_voltage_in_pu() const;
        complex<double> get_complex_terminal_current_in_pu() const;
        complex<double> get_complex_generation_in_pu() const;

        double get_wind_turbine_generator_speed_in_pu() const;
        double get_wind_turbine_generator_speed_referance_in_pu() const;

        void set_wind_turbine_power_speed_lookup_table(WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE table);
        WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE get_wind_turbine_power_speed_lookup_table() const;

        double get_wind_turbine_reference_speed_with_power_in_pu(double power);
        double get_wind_turbine_reference_power_with_speed_in_pu(double speed);
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual double get_double_data_with_index(size_t index) const = 0;
        virtual double get_double_data_with_name(string par_name) const = 0;
        virtual void set_double_data_with_index(size_t index, double value) = 0;
        virtual void set_double_data_with_name(string par_name, double value) = 0;

        virtual bool setup_model_with_steps_string(string data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_active_current_command_in_pu_based_on_mbase() const = 0;
        virtual double get_active_power_command_in_pu_based_on_mbase() const = 0;
        virtual double get_reactive_current_command_in_pu_based_on_mbase() const = 0;
        virtual double get_reactive_power_command_in_pu_based_on_mbase() const = 0;
        virtual double get_reactive_voltage_command_in_pu_based_on_mbase() const = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;


        virtual size_t get_variable_index_from_variable_name(string var_name)= 0;
        virtual string get_variable_name_from_variable_index(size_t var_index)= 0;
        virtual double get_variable_with_index(size_t var_index)= 0;
        virtual double get_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        size_t bus_to_regulate;
        double voltage_reference_in_pu;
        double frequency_reference_in_pu;
        double active_power_reference_in_pu;
        double reactive_power_reference_in_pu;
        double power_factor_reference_in_pu;
        double turbine_speed_reference_in_pu;
        PE_VAR_CONTROL_MODE pe_var_control_mode;
        WIND_TURBINE_POWER_SPEED_LOOKUP_TABLE power_speed_table;
};

#endif // WT_ELECTRICAL_MODEL_H
