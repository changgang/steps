#ifndef ES_AUXILIARY_MODEL_H
#define ES_AUXILIARY_MODEL_H

#include "header/model/energy_storage_model/es_model.h"
#include <cstdlib>

class ES_AUXILIARY_MODEL : public ES_MODEL
{
    public:
        ES_AUXILIARY_MODEL(STEPS& toolkit);
        virtual ~ES_AUXILIARY_MODEL();
    public:

        virtual string get_model_type() const;
        virtual string get_model_name() const = 0;

        bool is_primary_model_frozen() const;
        void freeze_primary_model();
        void unfreeze_primary_model();

        bool is_electrical_active_power_control_frozen() const;
        void freeze_electrical_active_power_control();
        void unfreeze_electrical_active_power_control();

        bool is_manual_control() const;
        void activate_manual_control();
        void deactivate_manual_control();

        void activate_rotor_braking();
        void deactivate_rotor_braking();
        bool is_rotor_braking_activated() const;

        void lock_rotor();
        void unlock_rotor();
        bool is_rotor_locked() const;

        double get_rotor_speed_in_pu() const;
        double get_mechanical_power_in_pu_on_mbase() const;
        double get_active_power_including_stator_loss_in_pu_on_mbase() const;

        void initialize_primary_model_as_out_service_mode();
        void initialize_electrical_model_as_out_service_mode();

        void set_speed_reference_when_into_start_discharge_mode(double s);
        double get_speed_reference_when_into_start_discharge_mode() const;

        void set_speed_reference_when_into_stop_discharge_mode(double s);
        double get_speed_reference_when_into_stop_discharge_mode() const;

        void set_speed_reference_when_into_start_charge_mode(double s);
        double get_speed_reference_when_into_start_charge_mode() const;

        void set_speed_reference_when_into_stop_charge_mode(double s);
        double get_speed_reference_when_into_stop_charge_mode() const;

        void set_power_reference_when_into_start_discharge_mode(double p);
        double get_power_reference_when_into_start_discharge_mode() const;

        void set_power_reference_when_into_stop_discharge_mode(double p);
        double get_power_reference_when_into_stop_discharge_mode() const;

        void set_power_reference_when_into_start_charge_mode(double p);
        double get_power_reference_when_into_start_charge_mode() const;

        void set_power_reference_when_into_stop_charge_mode(double p);
        double get_power_reference_when_into_stop_charge_mode() const;

        void set_valve_reference_when_into_start_discharge_mode(double p);
        double get_valve_reference_when_into_start_discharge_mode() const;

        void set_valve_reference_when_into_stop_discharge_mode(double p);
        double get_valve_reference_when_into_stop_discharge_mode() const;

        void set_valve_reference_when_into_start_charge_mode(double p);
        double get_valve_reference_when_into_start_charge_mode() const;

        void set_valve_reference_when_into_stop_charge_mode(double p);
        double get_valve_reference_when_into_stop_charge_mode() const;

        void set_time_when_into_start_discharge_mode(double t);
        double get_time_when_into_start_discharge_mode() const;

        void set_time_when_into_stop_discharge_mode(double t);
        double get_time_when_into_stop_discharge_mode() const;

        void set_time_when_into_start_charge_mode(double t);
        double get_time_when_into_start_charge_mode() const;

        void set_time_when_into_stop_charge_mode(double t);
        double get_time_when_into_stop_charge_mode() const;

        void set_manual_power_in_pu(double p);
        double get_manual_power_in_pu() const;

        double get_rotor_braking_Damp() const;
        void set_rotor_braking_Damp(double D);

        void set_current_Pref(double p);
        double get_current_Pref() const;

        void set_current_speedref(double s);
        double get_current_speedref() const;

        void set_current_Yref(double y);
        double get_current_Yref() const;

        virtual void check_energy_storage_mode() = 0;
        virtual void check_when_energy_storage_in_out_service_mode() = 0;
        virtual void check_when_energy_storage_in_start_discharge_mode() = 0;
        virtual void check_when_energy_storage_in_stop_discharge_mode() = 0;
        virtual void check_when_energy_storage_in_discharge_mode() = 0;
        virtual void check_when_energy_storage_in_start_charge_mode() = 0;
        virtual void check_when_energy_storage_in_stop_charge_mode() = 0;
        virtual void check_when_energy_storage_in_charge_mode() = 0;

        virtual double get_active_power_reference_in_pu() = 0;
        virtual double get_rotor_speed_reference_in_pu() = 0;
        virtual double get_valve_reference_in_pu() = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
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

        double speed_reference_when_into_start_discharge_mode;
        double speed_reference_when_into_stop_discharge_mode;

        double speed_reference_when_into_start_charge_mode;
        double speed_reference_when_into_stop_charge_mode;

        double power_reference_when_into_start_discharge_mode;
        double power_reference_when_into_stop_discharge_mode;

        double power_reference_when_into_start_charge_mode;
        double power_reference_when_into_stop_charge_mode;

        double valve_reference_when_into_start_discharge_mode;
        double valve_reference_when_into_stop_discharge_mode;

        double valve_reference_when_into_start_charge_mode;
        double valve_reference_when_into_stop_charge_mode;

        double time_when_into_start_discharge_mode;
        double time_when_into_stop_discharge_mode;

        double time_when_into_start_charge_mode;
        double time_when_into_stop_charge_mode;

        bool manual_control_flag;
        bool rotor_braking_flag;
        bool rotor_lock_flag;

        double rotor_braking_Damp;

        double current_Pref;
        double current_speedref;
        double current_Yref;
};

#endif // ES_AUXILIARY_MODEL_H
