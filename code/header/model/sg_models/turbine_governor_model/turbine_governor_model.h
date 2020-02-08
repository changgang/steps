#ifndef TURBINE_GOVERNOR_MODEL_H
#define TURBINE_GOVERNOR_MODEL_H

#include "header/model/sg_models/sg_model.h"
class TURBINE_GOVERNOR_MODEL : public SG_MODEL
{
    public:
        TURBINE_GOVERNOR_MODEL();
        virtual ~TURBINE_GOVERNOR_MODEL();

        virtual string get_model_type() const;
        // inputs
        double get_rotor_speed_deviation_in_pu_from_sync_generator_model() const;
        double get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model() const;

        //reference
        void set_initial_mechanical_power_reference_in_pu_based_on_mbase(double P);
        double get_initial_mechanical_power_reference_in_pu_based_on_mbase() const;

        double get_mechanical_power_reference_in_pu_based_on_mbase() const;


    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_mechanical_power_in_pu_based_on_mbase() const = 0;
        virtual double get_mechanical_power_upper_limit_in_pu_based_on_mbase() const = 0;
        virtual double get_mechanical_power_lower_limit_in_pu_based_on_mbase() const = 0;

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
        double initial_mechanical_power_reference_in_pu;
};

#endif // TURBINE_GOVERNOR_MODEL_H
