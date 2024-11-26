#ifndef ESAU0_H
#define ESAU0_H

#include "header/model/energy_storage_model/es_auxiliary_model/es_auxiliary_model.h"
#include "header/basic/multi_point_line.h"
#include <cstdlib>

class ESAU0 : public ES_AUXILIARY_MODEL
{
    public:
        ESAU0(STEPS& toolkit);
        ESAU0(const ESAU0& model);
        virtual ~ESAU0();
        virtual ESAU0& operator=(const ESAU0&);
    public:
        virtual string get_model_name() const;

        void set_start_charge_mode_power_threshold(double p);
        double get_start_charge_mode_power_threshold() const;

        void set_start_discharge_mode_power_threshold(double p);
        double get_start_discharge_mode_power_threshold() const;

        void set_power_slope_in_start_charge_mode(double k);
        double get_power_slope_in_start_charge_mode() const;

        void set_power_slope_in_stop_charge_mode(double k);
        double get_power_slope_in_stop_charge_mode() const;

        void set_power_slope_in_start_discharge_mode(double k);
        double get_power_slope_in_start_discharge_mode() const;

        void set_power_slope_in_stop_discharge_mode(double k);
        double get_power_slope_in_stop_discharge_mode() const;

        void set_speed_slope_in_start_charge_mode(double k);
        double get_speed_slope_in_start_charge_mode() const;

        void set_speed_slope_in_stop_charge_mode(double k);
        double get_speed_slope_in_stop_charge_mode() const;

        void set_speed_slope_in_start_discharge_mode(double k);
        double get_speed_slope_in_start_discharge_mode() const;

        void set_speed_slope_in_stop_discharge_mode(double k);
        double get_speed_slope_in_stop_discharge_mode() const;

        void set_valve_slope_in_start_charge_mode(double k);
        double get_valve_slope_in_start_charge_mode() const;

        void set_valve_slope_in_stop_charge_mode(double k);
        double get_valve_slope_in_stop_charge_mode() const;

        void set_valve_slope_in_start_discharge_mode(double k);
        double get_valve_slope_in_start_discharge_mode() const;

        void set_valve_slope_in_stop_discharge_mode(double k);
        double get_valve_slope_in_stop_discharge_mode() const;

        void add_power_speed_pair_in_charge(double power, double speed);
        void add_power_speed_pair_in_discharge(double power, double speed);
        void add_power_valve_pair_in_charge(double power, double valve);
        void add_power_valve_pair_in_discharge(double power, double valve);

        double get_target_power_reference_in_pu() const;
        double get_target_speed_reference_in_pu() const;
        double get_target_valve_reference_in_pu() const;

        virtual void check_energy_storage_mode();
        virtual void check_when_energy_storage_in_out_service_mode();
        virtual void check_when_energy_storage_in_start_discharge_mode();
        virtual void check_when_energy_storage_in_stop_discharge_mode();
        virtual void check_when_energy_storage_in_discharge_mode();
        virtual void check_when_energy_storage_in_start_charge_mode();
        virtual void check_when_energy_storage_in_stop_charge_mode();
        virtual void check_when_energy_storage_in_charge_mode();

        virtual double get_active_power_reference_in_pu();
        virtual double get_rotor_speed_reference_in_pu();
        virtual double get_valve_reference_in_pu();

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const ESAU0& model);

        double start_charge_mode_power_threshold;
        double start_discharge_mode_power_threshold;

        double power_slope_in_start_charge_mode;
        double power_slope_in_stop_charge_mode;
        double power_slope_in_start_discharge_mode;
        double power_slope_in_stop_discharge_mode;

        double speed_slope_in_start_charge_mode;
        double speed_slope_in_stop_charge_mode;
        double speed_slope_in_start_discharge_mode;
        double speed_slope_in_stop_discharge_mode;

        double valve_slope_in_start_charge_mode;
        double valve_slope_in_stop_charge_mode;
        double valve_slope_in_start_discharge_mode;
        double valve_slope_in_stop_discharge_mode;

        bool speed_reach_target_flag;
        bool power_reach_target_flag;
        bool valve_reach_target_flag;

        MULTI_POINT_LINE power_speed_table_in_charge;
        MULTI_POINT_LINE power_speed_table_in_discharge;
        MULTI_POINT_LINE power_valve_table_in_charge;;
        MULTI_POINT_LINE power_valve_table_in_discharge;;

};

#endif // ESAU0_H
