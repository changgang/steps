#ifndef SG_MODEL_H
#define SG_MODEL_H

#include "header/model/model.h"
#include <complex>

class GENERATOR;

class SG_MODEL : public MODEL
{
public:
        SG_MODEL();
        virtual ~SG_MODEL();

        GENERATOR* get_generator_pointer() const;
        double get_mbase_in_MVA() const;
        double get_bus_base_frequency_in_Hz() const;
        complex<double> get_terminal_complex_voltage_in_pu() const;
    public: // specific model level
        virtual string get_model_type() const = 0;
        virtual string get_model_name() const = 0;

        virtual double get_model_data_with_index(size_t index) const = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_index(size_t index, double value) = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

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

        virtual void prepare_model_variable_table() = 0;
        virtual double get_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
};
#endif // SG_MODEL_H
