#ifndef ES_RELAY_MODEL_H
#define ES_RELAY_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
class ES_RELAY_MODEL : public ES_MODEL
{
    /*
    es relay model:
    inputs:
        frequency: bus frequency
        voltage: bus voltage
    output:
        trip es if necessary
    */
    public:
        ES_RELAY_MODEL(STEPS& toolkit);
        virtual ~ES_RELAY_MODEL();

        virtual string get_model_type() const;
        // inputs
        double get_bus_frequency_in_pu() const;
        //double get_bus_positive_sequence_voltage_in_pu() const;
    public: // specific model level
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
    private:
        void common_constructor();
};

#endif // ES_RELAY_MODEL_H
