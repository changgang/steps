#ifndef ES_PRIMARY_MODEL_H
#define ES_PRIMARY_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
#include <cstdlib>

class ES_PRIMARY_MODEL : public ES_MODEL
{
    public:
        ES_PRIMARY_MODEL(STEPS& toolkit);
        virtual ~ES_PRIMARY_MODEL();
    public:
        virtual string get_model_type() const;
    public:
        virtual string get_model_name() const = 0;

        bool is_rotor_locked() const;
        double get_rotor_speed_reference_in_pu() const;
        double get_rotor_speed_in_pu() const;
        double get_active_power_reference_in_pu() const;
        double get_valve_reference_in_pu() const;

        void set_initial_rotor_speed_in_pu(double s);
        void set_initial_active_power_reference_in_pu(double p);
        void set_initial_valve_in_pu(double v);
        double get_initial_rotor_speed_in_pu() const;
        double get_initial_active_power_reference_in_pu() const;
        double get_initial_valve_in_pu() const;

        bool is_primary_model_frozen() const;
        void freeze_primary_model();
        void unfreeze_primary_model();

        virtual double get_valve_in_pu() const = 0;
        virtual double get_delta_Y_in_pu() const = 0;
        virtual double get_delta_Yref_in_pu() const = 0;
        virtual double get_mechanical_power_in_pu_on_mbase() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void initialize_Pmech_as_zero() = 0;

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
        double initial_Pref;
        double initial_speed;
        double initial_valve;

        bool primary_model_freeze_flag;

};

#endif // ES_PRIMARY_MODEL_H
