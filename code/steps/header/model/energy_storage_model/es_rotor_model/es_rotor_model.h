#ifndef ES_ROTOR_MODEL_H
#define ES_ROTOR_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
#include "header/block/integral_block.h"
#include <cstdlib>

class ES_ROTOR_MODEL : public ES_MODEL
{
    public:
        ES_ROTOR_MODEL(STEPS& toolkit);
        virtual ~ES_ROTOR_MODEL();
    public:
        virtual string get_model_type() const;
        virtual string get_model_name() const = 0;

        void set_initial_mechanical_power_in_pu_on_mbase(double p);
        double get_initial_mechanical_power_in_pu_on_mbase() const;
        void set_initial_rotor_speed_reference_in_pu(double s);
        double get_initial_rotor_speed_reference_in_pu() const;

        void set_H_in_s(double H);
        double get_H_in_s() const;

        INTEGRAL_BLOCK* get_rotor_speed_block();

        void lock_rotor();
        void unlock_rotor();
        bool is_rotor_locked() const;

        void activate_rotor_braking();
        void deactivate_rotor_braking();
        bool is_rotor_braking_activated() const;

        double get_mechanical_power_in_pu_on_mbase() const;
        double get_active_power_including_stator_loss_in_pu_on_mbase() const;
        double get_rotor_speed_reference_in_pu() const;

        virtual void check_rotor_lock_logic() = 0;
        virtual void check_rotor_braking_logic() = 0;
        virtual double get_rotor_speed_in_pu() = 0;
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
        double initial_Pmech_in_pu_on_mbase;
        double initial_rotor_speed_reference_in_pu;
        INTEGRAL_BLOCK rotor_speed_block;
        bool rotor_lock_flag;
        bool rotor_braking_flag;

};

#endif // ES_ROTOR_MODEL_H
