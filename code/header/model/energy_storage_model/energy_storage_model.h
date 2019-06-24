#ifndef ENERGY_STORAGE_MODEL_H
#define ENERGY_STORAGE_MODEL_H

#include "header/model/model.h"
#include <complex>

class ENERGY_STORAGE;

class ENERGY_STORAGE_MODEL : public MODEL
{
public:
        ENERGY_STORAGE_MODEL();
        virtual ~ENERGY_STORAGE_MODEL();

        ENERGY_STORAGE* get_energy_storage_pointer() const;
        double get_mbase_in_MVA() const;
        complex<double> get_terminal_bus_complex_voltage_in_pu() const;
        double get_terminal_bus_voltage_in_pu() const;
        double get_terminal_bus_angle_in_rad() const;
        double get_terminal_bus_frequency_deviation_in_pu() const;

        void set_Pmax_in_pu(double p);
        void set_E0_in_pu(double E);
        void set_En_in_MWh(double E);
        void set_Pref_in_pu(double p);
        void set_Vref_in_pu(double v);

        double get_Pmax_in_pu() const;
        double get_E0_in_pu() const;
        double get_En_in_MWh() const;
        double get_Pref_in_pu() const;
        double get_Vref_in_pu() const;

    public: // specific model level
        virtual string get_model_type() const;
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual complex<double> get_terminal_complex_power_in_pu_based_on_mbase() const = 0;
        virtual complex<double> get_terminal_complex_power_in_MVA() const = 0 ;
        virtual double get_terminal_active_power_in_pu_based_on_mbase() const = 0;
        virtual double get_terminal_active_power_in_MW() const = 0;
        virtual double get_terminal_reactive_power_in_pu_based_on_mbase() const = 0;
        virtual double get_terminal_reactive_power_in_MVar() const = 0;
        virtual double get_terminal_current_in_kA() const = 0;
        virtual complex<double> get_terminal_complex_current_in_kA() const = 0;

        virtual complex<double> get_terminal_complex_current_in_pu_based_on_mbase() const = 0 ;
        virtual complex<double> get_terminal_complex_current_in_pu_based_on_sbase() const = 0;
        virtual double get_terminal_current_in_pu_based_on_mbase() const = 0;
        virtual double get_terminal_current_in_pu_based_on_sbase() const = 0;

        virtual double get_energy_state_in_pu() const = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        double initial_energy_in_pu;
        double nominal_energy_in_MWh;
        double max_power_in_pu;

        double Pref_in_pu, Vref_in_pu;
};
#endif // ENERGY_STORAGE_MODEL_H
