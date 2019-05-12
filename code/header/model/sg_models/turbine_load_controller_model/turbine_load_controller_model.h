#ifndef TURBINE_LOAD_CONTROLLER_MODEL_H
#define TURBINE_LOAD_CONTROLLER_MODEL_H

#include "header/model/sg_models/sg_model.h"
#include "header/block/integral_block.h"
#include "header/basic/constants.h"
#include "header/meter/meter.h"

typedef METER SIGNAL;

class TURBINE_LOAD_CONTROLLER_MODEL : public SG_MODEL
{
    /*
    Model of turbine load controller
    Input:
        generator speed from sync generator model
        generator electrical power from sync generator model
    Output:
        load control signal to turbine governor model
    */
    public:
        TURBINE_LOAD_CONTROLLER_MODEL();
        virtual ~TURBINE_LOAD_CONTROLLER_MODEL();

    public: // stabilizer common
        virtual string get_model_type() const;
        double get_initial_mechanical_power_reference_in_pu_based_on_mbase_from_turbine_governor_model() const;
        double get_terminal_active_power_in_pu_based_on_mbase_from_generator_model() const;
        double get_rotor_speed_deviation_in_pu_from_generator_model() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void set_block_toolkit() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_mechanical_power_reference_in_pu_based_on_mbase() const = 0;

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
};

#endif // TURBINE_LOAD_CONTROLLER_MODEL_H
