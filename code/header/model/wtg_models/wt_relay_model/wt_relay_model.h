#ifndef WT_RELAY_MODEL_H
#define WT_RELAY_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
class WT_RELAY_MODEL : public WTG_MODEL
{
    /*
    wind relay model:
    inputs:
        vwind: wind speed
        speed: rotor speed
        frequency: bus frequency
        voltage: bus voltage
    output:
        trip wt generator if necessary
    */
    public:
        WT_RELAY_MODEL();
        virtual ~WT_RELAY_MODEL();

        virtual string get_model_type() const;
        // inputs
        double get_wind_speed_in_pu() const;
        double get_wt_generator_rotor_speed_in_pu() const;
        double get_bus_frequency_in_pu() const;
        double get_bus_voltage_in_pu() const;
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
        virtual string get_standard_psse_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        void common_constructor();
};

#endif // WT_RELAY_MODEL_H
