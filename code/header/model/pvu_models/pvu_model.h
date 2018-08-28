#ifndef PVU_MODEL_H
#define PVU_MODEL_H

#include "header/model/model.h"
#include <complex>

class PV_UNIT;

class PVU_MODEL : public MODEL
{
public:
        PVU_MODEL();
        virtual ~PVU_MODEL();

        PV_UNIT* get_pv_unit_pointer() const;
        double get_mbase_in_MVA() const;
        complex<double> get_terminal_complex_voltage_in_pu() const;

        size_t get_number_of_lumped_pv_units() const;
        double get_rated_power_per_pv_unit_in_MW() const;
        double get_bus_base_frequency_in_Hz() const;
        complex<double> get_source_impedance_in_pu_based_on_mbase() const;

    public: // specific model level
        virtual string get_model_type() const = 0;
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
};
#endif // PVU_MODEL_H