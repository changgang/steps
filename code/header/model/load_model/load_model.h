#ifndef LOAD_MODEL_H
#define LOAD_MODEL_H

#include "header/model/model.h"
#include "header/basic/steps_enum.h"
#include <complex>

class LOAD;

class LOAD_MODEL : public MODEL
{
    public:
        LOAD_MODEL();
        virtual ~LOAD_MODEL();
        LOAD* get_load_pointer() const;
        // common load model
        virtual string get_model_type() const;
        // common inputs
        double get_bus_voltage_in_pu() const;
        double get_bus_frequency_deviation_in_pu() const;
        // common scale
        void set_subsystem_type(SUBSYSTEM_TYPE subtype);
        SUBSYSTEM_TYPE get_subsystem_type() const;
        string get_detailed_model_name() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void set_block_toolkit() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual complex<double> get_load_power_in_MVA() const = 0;
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
        SUBSYSTEM_TYPE subsystem_type;

};
#endif // LOAD_MODEL_H
