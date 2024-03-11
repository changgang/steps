#ifndef ES_MODEL_H
#define ES_MODEL_H

#include "header/model/model.h"
#include <complex>

class ENERGY_STORAGE;

class ES_MODEL : public MODEL
{
public:
        ES_MODEL(STEPS& toolkit);
        virtual ~ES_MODEL();

        ENERGY_STORAGE* get_energy_storage_pointer() const;
        double get_mbase_in_MVA() const;
        double get_one_over_mbase_in_one_over_MVA() const;

        double get_terminal_voltage_in_pu() const;
        complex<double> get_terminal_complex_voltage_in_pu() const;
        double get_terminal_voltage_angle_in_rad() const;
        double get_bus_base_frequency_in_Hz() const;
        complex<double> get_source_impedance_in_pu_based_on_mbase() const;

    public: // specific model level
        virtual string get_model_type() const = 0;
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

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
};
#endif // ES_MODEL_H
