#ifndef WT_DBR_MODEL_H
#define WT_DBR_MODEL_H

#include "header/model/wtg_models/wtg_model.h"
class WT_DBR_MODEL : public WTG_MODEL
{
    /*
    dynamic braking resistor model:
    inputs:
        pcmd: acitve power command
        pelec: electrical power generation
    output:
        pdbr: braking resisitor power
    */
    public:
        WT_DBR_MODEL(STEPS& toolkit);
        virtual ~WT_DBR_MODEL();

        virtual string get_model_type() const;

        void set_initial_braking_resistor_energy_in_pu(double E);
        void set_braking_resistor_energy_threshold_in_pu(double E);

        double get_initial_braking_resistor_energy_in_pu() const;
        double get_braking_resistor_energy_threshold_in_pu() const;

        // inputs
        double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() const;
        double get_active_power_command_in_pu_based_on_mbase() const;

        double get_braking_resistor_power_in_MW();


    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_braking_resistor_power_in_pu_based_on_mbase() = 0;

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

        double Edbr,Ebst;
};

#endif // WT_DBR_MODEL_H
