#ifndef VSC_HVDC_H
#define VSC_HVDC_H

#include "header/device/nonbus_device.h"
#include "header/device/vsc_hvdc_var.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_model.h"
#include "header/basic/sparse_matrix_define.h"
#include "header/basic/inphno.h"

#include <complex>
#include <string>


using namespace std;

class BUS;
class VSC_HVDC_MODEL;

class VSC_HVDC : public NONBUS_DEVICE
{
    public:
        VSC_HVDC(STEPS& toolkit);
        VSC_HVDC(const VSC_HVDC& vsc);
        virtual ~VSC_HVDC();

        void set_identifier(const string id);
        void set_name(const string name);
        void set_converter_count(const unsigned int n);
        void set_dc_bus_count(const unsigned int n);
        void set_dc_line_count(const unsigned int n);
        void set_status(const bool status);
        void set_ac_converter_bus_with_dc_voltage_control(const unsigned int bus);

        void set_converter_bus(unsigned int index, const unsigned int bus);
        void set_converter_dc_operation_mode(unsigned int index, const VSC_HVDC_DC_CONTROL_MODE mode);
        void set_converter_ac_operation_mode(unsigned int index, const VSC_HVDC_AC_CONTROL_MODE mode);

        void set_converter_nominal_ac_power_command_in_MW(unsigned int index, const double P);
        void set_converter_nominal_dc_voltage_command_in_kV(unsigned int index, const double V);

        void set_converter_nominal_ac_voltage_command_in_pu(unsigned int index, const double V);
        void set_converter_nominal_ac_power_command_in_Mvar(unsigned int index, const double Q);

        void set_converter_loss_factor_A_in_kW(unsigned int index, const double A);
        void set_converter_loss_factor_B_in_kW_per_amp(unsigned int index, const double B);
        void set_converter_minimum_loss_in_kW(unsigned int index, const double P);

        void set_converter_rated_capacity_in_MVA(unsigned int index, const double S);
        void set_converter_current_rating_in_amp(unsigned int index, const double I);

        void set_converter_power_weighting_factor(unsigned int index, const double pwf);
        void set_converter_Qmax_in_MVar(unsigned int index, const double Q);
        void set_converter_Qmin_in_MVar(unsigned int index, const double Q);

        void set_converter_remote_bus_to_regulate(unsigned int index, const unsigned int bus);
        void set_converter_remote_regulation_percent(unsigned int index, const double rmpct);

        void set_dc_bus_number(unsigned int index, const unsigned int bus);
        void set_dc_bus_name (unsigned int index, const string name);
        void set_dc_bus_area(unsigned int index, const unsigned int area);
        void set_dc_bus_zone(unsigned int index, const unsigned int zone);
        void set_ac_bus_number_of_dc_bus(unsigned int index, const unsigned int bus);
        void set_owner_number(unsigned int index, const unsigned int owner);
        void set_ground_resistance_in_ohm(unsigned int index, const double R);
        void set_dc_bus_generation_power_in_MW(unsigned int index, const double P);
        void set_dc_bus_load_power_in_MW(unsigned int index, const double P);

        void set_dc_line_sending_side_bus(unsigned int index, const unsigned int bus);
        void set_dc_line_receiving_side_bus(unsigned int index, const unsigned int bus);
        void set_dc_line_identifier(unsigned int index, const string identifier);
        void set_dc_line_meter_end_bus(unsigned int index, const unsigned int meter_bus);
        void set_dc_line_resistance_in_ohm(unsigned int index, const double R);
        void set_dc_line_inductance_in_mH(unsigned int index, const double L);

        void set_convergence_flag(bool flag);

        virtual void clear();

        string get_identifier() const;
        unsigned int get_identifier_index() const;
        string get_name() const;
        unsigned int get_name_index() const;
        unsigned int get_converter_count() const;
        unsigned int get_dc_bus_count() const;
        unsigned int get_dc_line_count() const;
        bool get_status() const;
        unsigned int get_ac_converter_bus_with_dc_voltage_control() const;

        unsigned int get_converter_bus(unsigned int index) const;
        VSC_HVDC_DC_CONTROL_MODE get_converter_dc_operation_mode(unsigned int index) const;
        VSC_HVDC_AC_CONTROL_MODE get_converter_ac_operation_mode(unsigned int index) const;

        double get_converter_nominal_ac_power_command_in_MW(unsigned int index) const;
        double get_converter_nominal_active_power_command_in_pu(unsigned int index) const;
        double get_converter_nominal_dc_voltage_command_in_kV(unsigned int index) const;

        double get_converter_nominal_ac_voltage_command_in_kV(unsigned int index) const;
        double get_converter_nominal_ac_voltage_command_in_pu(unsigned int index) const;
        double get_converter_nominal_ac_power_command_in_Mvar(unsigned int index) const;
        double get_converter_nominal_reactive_power_command_in_pu(unsigned int index) const;

        double get_converter_loss_factor_A_in_kW(unsigned int index) const;
        double get_converter_loss_factor_B_in_kW_per_amp(unsigned int index) const;
        double get_converter_minimum_loss_in_kW(unsigned int index) const;

        double get_converter_rated_capacity_in_MVA(unsigned int index) const;
        double get_converter_current_rating_in_amp(unsigned int index) const;

        double get_converter_power_weighting_factor(unsigned int index) const;
        double get_converter_Qmax_in_MVar(unsigned int index) const;
        double get_converter_Qmin_in_MVar(unsigned int index) const;

        unsigned int get_converter_remote_bus_to_regulate(unsigned int index) const;
        double get_converter_remote_regulation_percent(unsigned int index) const;

        unsigned int get_dc_bus_number(unsigned int index) const;
        unsigned int get_ac_bus_number_of_dc_bus(unsigned int index) const;
        string get_dc_bus_name (unsigned int index) const;
        unsigned int get_dc_bus_area(unsigned int index) const;
        unsigned int get_dc_bus_zone(unsigned int index) const;
        unsigned int get_owner_number(unsigned int index) const;
        double get_ground_resistance_in_ohm(unsigned int index) const;
        double get_dc_bus_generation_power_in_MW(unsigned int index) const;
        double get_dc_bus_load_power_in_MW(unsigned int index) const;

        unsigned int get_dc_line_sending_side_bus(unsigned int index) const;
        unsigned int get_dc_line_receiving_side_bus(unsigned int index) const;
        string get_dc_line_identifier(unsigned int index) const;
        unsigned int get_dc_line_meter_end_bus(unsigned int index) const;
        double get_dc_line_resistance_in_ohm(unsigned int index) const;
        double get_dc_line_inductance_in_mH(unsigned int index) const;

        void set_max_iteration(unsigned int iteration);
        double get_max_iteration();
        void set_allowed_max_P_mismatch_in_MW(double max_mismatch);
        double get_allowed_max_P_mistmach_in_MW();
        void set_dc_base_voltage_in_KV(double dc_base_voltage);
        double get_dc_base_voltage_in_KV();

        VSC_HVDC& operator=(const VSC_HVDC& vsc);

        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual bool is_valid() const;
        virtual void check();
        virtual void report() const;
        virtual void save() const;
        virtual DEVICE_ID get_device_id() const;
        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        void set_vsc_hvdc_model(VSC_HVDC_MODEL* model);

        VSC_HVDC_MODEL* get_vsc_hvdc_model() const;

        void build_dc_network_matrix();
        void export_dc_network_matrix(string filename);
        void show_dc_network_matrix();
        void build_jacobian();
        void show_jacobian();

        void solve_steady_state();
        void initialize_Udc_vector();
        void initialize_Pdc_loss_vector();
        void update_Pdc_command_with_converter_capacity_limit();

        double calculate_dc_power_mismatch();
        double get_allowed_max_active_power_imbalance_in_MW();
        double get_maximum_active_power_mismatch_in_MW();
        void set_allowed_max_active_power_in_MW(double P);
        unsigned int get_iteration_count();
        void update_dc_bus_voltage();
        void update_Pdc_loss();
        double calculate_dc_active_power_of_slack_bus();
        bool check_slack_limit(double P);

        void build_delta_p_vector();
        void get_Pdc_command_vector();
        void calculate_raw_dc_current_into_dc_network();
        void update_raw_dc_current_into_dc_network();
        void calculate_raw_dc_power_into_dc_network();
        void add_Pdc_command_to_delta_p_vector();
        void add_supply_power_to_delta_p_vector();
        void add_consumed_power_to_delta_p_vector();
        void add_raw_dc_power_to_delta_p_vector();
        void add_Pdc_loss_to_delta_p_vector();
        void save_dc_bus_powerflow_result_to_file(const string& filename) const;

        void build_inphno();
        void build_initial_zero_matrix();
        void add_dc_lines_to_dc_network();
        double get_dc_voltage_of_dc_bus_number(unsigned int bus);
        unsigned int get_index_of_dc_bus_number(unsigned int bus);
        bool is_dc_network_matrix_set();

        vector<double> generate_active_power();
        vector<double> generate_reactive_power();
        vector<double> generate_ac_side_voltage();

        double get_converter_ac_bus_current(unsigned int index);
        double get_converter_ac_bus_power_factor(unsigned int index);
        double get_converter_dc_bus_current(unsigned int index);
        double get_converter_dc_voltage(unsigned int index);
        double get_converter_dc_power(unsigned int index);
        double get_converter_ac_active_power(unsigned int index);
        double get_converter_ac_reactive_power(unsigned int index);
        double get_converter_ac_bus_voltage(unsigned int index);
        double get_converter_ac_bus_angle(unsigned int index);
        double get_converter_ac_bus_complex_voltage(unsigned int index);
    private:
        void copy_from_const_vsc(const VSC_HVDC& vsc);

        unsigned int vsc_hvdc_name_index;
        unsigned int identifier_index;
        bool status;
        unsigned int ac_converter_bus_with_dc_voltage_control;

        unsigned int n_converter, n_dc_bus, n_dc_line;

        vector<VSC_HVDC_CONVERTER_VAR> converters;
        vector<VSC_HVDC_DC_BUS_VAR>  dc_buses;
        vector<VSC_HVDC_DC_LINE_VAR> dc_lines;

        STEPS_SPARSE_MATRIX dc_network_matrix;
        STEPS_SPARSE_MATRIX jacobian;
        INPHNO inphno;

        vector<double> Pdc_command;
        vector<double> Pdc_loss;
        vector<double> bus_power;
        vector<double> delta_Udc;
        vector<double> Udc;
        vector<double> bus_current;
        vector<double> delta_power;
        vector<double> Qdc_command;
        vector<double> supply_power;
        vector<double> consumed_power;
        vector<double> active_power;
        vector<double> reactive_power;
        vector<double> Us;
        vector<BUS*> internal_bus_pointers;

        unsigned int max_iteration;
        unsigned int iteration_count;
        double max_P_mismatch_in_MW;
        double P_threshold_in_MW;
        double dc_base_voltage_in_KV;

        double P_slack_ac_side;
        double P_slack_dc_side;
        bool converged;

        VSC_HVDC_MODEL* vsc_hvdc_model;

};
#endif // VSC_HVDC_H
