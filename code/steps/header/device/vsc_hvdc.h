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
        void set_converter_count(unsigned int n);
        void set_dc_bus_count(unsigned int n);
        void set_dc_line_count(unsigned int n);
        void set_status(const bool status);
        void set_dc_network_base_voltage_in_kV(const double base_voltage);

        void set_converter_ac_bus(const unsigned int index, const unsigned int bus);

        void set_converter_dc_operation_mode(const unsigned int index, const VSC_HVDC_DC_CONTROL_MODE mode);
        void set_converter_ac_operation_mode(const unsigned int index, const VSC_HVDC_AC_CONTROL_MODE mode);

        void set_converter_nominal_ac_active_power_command_in_MW(const unsigned int index, const double P);
        void set_converter_nominal_dc_voltage_command_in_kV(const unsigned int index, const double V);
        void set_converter_initial_dc_voltage_reference_in_kV(const unsigned int index, const double V);
        void set_converter_initial_dc_active_power_reference_in_MW(const unsigned int index, const double P);
        void set_converter_initial_power_voltage_droop_coefficient(const unsigned int index, const double droop);
        void set_converter_initial_dc_current_reference_in_kA(const unsigned int index, const double I);
        void set_converter_initial_current_voltage_droop_coefficient(const unsigned int index, const double droop);


        void set_converter_nominal_ac_voltage_command_in_pu(const unsigned int index, const double V);
        void set_converter_nominal_ac_reactive_power_command_in_Mvar(const unsigned int index, const double Q);

        void set_converter_loss_factor_A_in_kW(const unsigned int index, const double A);
        void set_converter_loss_factor_B_in_kW_per_amp(const unsigned int index, const double B);
        void set_converter_loss_factor_C_in_KW_per_amp_squared(const unsigned int index, const double C);
        void set_converter_minimum_loss_in_kW(const unsigned int index, const double P);

        void set_converter_rated_capacity_in_MVA(const unsigned int index, const double S);
        void set_converter_rated_current_in_A(const unsigned int index, const double I);

        void set_converter_power_weighting_factor(const unsigned int index, const double pwf);
        void set_converter_Qmax_in_MVar(const unsigned int index, const double Q);
        void set_converter_Qmin_in_MVar(const unsigned int index, const double Q);
        void set_converter_Udmax_in_kV(const unsigned int index, const double Udmax);
        void set_converter_Udmin_in_kV(const unsigned int index, const double Udmin);

        void set_converter_remote_bus_to_regulate(const unsigned int index, const unsigned int bus);
        void set_converter_remote_regulation_percent(const unsigned int index, const double rmpct);

        void set_dc_bus_number(const unsigned int index, const unsigned int bus);
        void set_dc_bus_name(const unsigned int index, const string name);
        void set_dc_bus_ac_bus_number(const unsigned int index, const unsigned int bus);
        void set_dc_bus_area(const unsigned int index, const unsigned int area);
        void set_dc_bus_zone(const unsigned int index, const unsigned int zone);
        void set_dc_bus_owner_number(const unsigned int index, const unsigned int owner);
        void set_dc_bus_ground_resistance_in_ohm(const unsigned int index, const double R);
        void set_dc_bus_generation_power_in_MW(const unsigned int index, const double P);
        void set_dc_bus_load_power_in_MW(const unsigned int index, const double P);

        void set_dc_line_sending_side_bus(const unsigned int index, const unsigned int bus);
        void set_dc_line_receiving_side_bus(const unsigned int index, const unsigned int bus);
        void set_dc_line_identifier(const unsigned int index, const string identifier);
        void set_dc_line_meter_end_bus(const unsigned int index, const unsigned int meter_bus);
        void set_dc_line_resistance_in_ohm(const unsigned int index, const double R);
        void set_dc_line_inductance_in_mH(const unsigned int index, const double L);


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
        double get_dc_network_base_voltage_in_kV() const;

        unsigned int get_converter_ac_bus(unsigned int index) const;
        VSC_HVDC_DC_CONTROL_MODE get_converter_dc_operation_mode(unsigned int index) const;
        VSC_HVDC_AC_CONTROL_MODE get_converter_ac_operation_mode(unsigned int index) const;

        double get_converter_nominal_ac_active_power_command_in_MW(unsigned int index) const;
        double get_converter_nominal_ac_active_power_command_in_pu(unsigned int index) const;
        double get_converter_nominal_dc_voltage_command_in_kV(unsigned int index) const;
        double get_converter_nominal_dc_voltage_command_in_pu(unsigned int index) const;

        double get_converter_nominal_ac_voltage_command_in_kV(unsigned int index) const;
        double get_converter_nominal_ac_voltage_command_in_pu(unsigned int index) const;
        double get_converter_initial_dc_voltage_reference_in_kV(const unsigned int index)const;
        double get_converter_initial_dc_active_power_reference_in_MW(const unsigned int index)const;
        double get_converter_initial_power_voltage_droop_coefficient(const unsigned int index)const;
        double get_converter_initial_dc_current_reference_in_kA(const unsigned int index)const;
        double get_converter_initial_current_voltage_droop_coefficient(const unsigned int index)const;

        int get_converter_alpha(unsigned int index);
        int get_converter_beta(unsigned int index);

        double get_converter_nominal_ac_reactive_power_command_in_Mvar(unsigned int index) const;
        double get_converter_nominal_reactive_power_command_in_pu(unsigned int index) const;

        double get_converter_loss_factor_A_in_kW(unsigned int index) const;
        double get_converter_loss_factor_B_in_kW_per_amp(unsigned int index) const;
        double get_converter_loss_factor_C_in_kW_per_amp_squard(unsigned int index) const;
        double get_converter_minimum_loss_in_kW(unsigned int index) const;

        double get_converter_rated_capacity_in_MVA(unsigned int index) const;
        double get_converter_current_rating_in_amp(unsigned int index) const;

        double get_converter_power_weighting_factor(unsigned int index) const;
        double get_converter_Qmax_in_MVar(unsigned int index) const;
        double get_converter_Qmin_in_MVar(unsigned int index) const;
        double get_converter_Udmax_in_kV(const unsigned int index) const;
        double get_converter_Udmin_in_kV(const unsigned int index) const;

        unsigned int get_converter_remote_bus_to_regulate(unsigned int index) const;
        double get_converter_remote_regulation_percent(unsigned int index) const;

        unsigned int get_dc_bus_number(unsigned int index) const;
        string get_dc_bus_name (unsigned int index) const;
        unsigned int get_dc_bus_ac_bus_number(unsigned int index) const;
        unsigned int get_dc_bus_area(unsigned int index) const;
        unsigned int get_dc_bus_zone(unsigned int index) const;
        unsigned int get_dc_bus_owner_number(unsigned int index) const;
        double get_dc_bus_ground_resistance_in_ohm(unsigned int index) const;
        double get_dc_bus_generation_power_in_MW(unsigned int index) const;
        double get_dc_bus_load_power_in_MW(unsigned int index) const;

        unsigned int get_dc_line_sending_side_bus(unsigned int index) const;
        unsigned int get_dc_line_receiving_side_bus(unsigned int index) const;
        string get_dc_line_identifier(unsigned int index) const;
        unsigned int get_dc_line_meter_end_bus(unsigned int index) const;
        double get_dc_line_resistance_in_ohm(unsigned int index) const;
        double get_dc_line_inductance_in_mH(unsigned int index) const;

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


        void solve_steady_state();
        void build_dc_network_matrix();
        void build_inphno();
        void build_initial_zero_matrix();
        void add_dc_lines_to_dc_network();
        void build_dc_bus_power_mismatch_vector();
        void build_Pdc_command_vector();
        void build_jacobian();

        void set_max_iteration(unsigned int iteration);
        void set_allowed_max_P_mismatch_in_MW(double max_mismatch);

        double get_maximum_active_power_mismatch_in_MW();
        double get_allowed_max_active_power_imbalance_in_MW();
        unsigned int get_iteration_count();
        double get_max_iteration();
        unsigned int get_index_of_dc_bus_number(unsigned int bus);
        double get_dc_voltage_of_dc_bus_number(unsigned int bus);

        double get_dc_current_reference_of_dc_bus_number(unsigned int bus);
        double get_dc_voltage_reference_of_dc_bus_number(unsigned int bus);
        double get_current_voltage_dropp_coefficient_of_dc_bus_number(unsigned int bus);
        double get_power_voltage_droop_coefficient_of_dc_bus_number(unsigned int bus);
        int get_alpha_of_dc_bus_number(unsigned int bus);
        int get_beta_of_dc_bus_number(unsigned int bus);

        void initialize_Udc_vector();
        void initialize_P_converter_loss_vector();
        void initialize_alpha_vector();
        void initialize_beta_vector();
        void initialize_Kpi_Kdi_coefficient_vector();
        void initialize_Udi_Idi_reference_vector();

        void update_dc_bus_voltage();
        void update_Pdc_loss();
        void update_raw_dc_current_into_dc_network();
        void calculate_raw_dc_power_into_dc_network();
        void add_Pdc_command_to_P_mismatch_vector();
        void add_generation_power_to_P_mismatch_vector();
        void add_load_power_to_P_mismatch_vector();
        void add_raw_dc_power_to_P_mismatch_vector();
        void add_P_converter_loss_to_P_mismatch_vector();

        void set_convergence_flag(bool flag);
        void save_dc_bus_powerflow_result_to_file(const string& filename) const;
        void calculate_dc_active_power_of_slack_bus();
        bool check_slack_limit(double P);
        bool is_dc_network_matrix_set();
        bool is_jacobian_matrix_set();

        void export_dc_network_matrix(string filename);
        void show_dc_network_matrix();
        void show_jacobian_matrix();
        double get_dc_network_matrix_entry_between_dc_bus(unsigned int ibus, unsigned int jbus);
        double get_jacobian_matrix_entry_between_dc_bus(unsigned int ibus, unsigned int jbus);

    private:
        void copy_from_const_vsc(const VSC_HVDC& vsc);

        void set_ac_converter_bus_with_dc_voltage_control(const unsigned int bus);
        unsigned int dc_bus_no2index(unsigned int bus) const;
        unsigned int dc_bus_index2no(unsigned int index) const;
        unsigned int get_dc_bus_converter_index_with_dc_bus_index(unsigned int index) const;
        unsigned int get_dc_bus_converter_index_with_dc_bus_number(unsigned int bus) const;

        bool converter_index_is_out_of_range_in_function(const unsigned int index, const string& func);
        bool dc_bus_index_is_out_of_range_in_function(const unsigned int index, const string& func);
        bool dc_line_index_is_out_of_range_in_function(const unsigned int index, const string& func);

        unsigned int vsc_hvdc_name_index;
        unsigned int identifier_index;
        bool status;
        unsigned int ac_converter_bus_with_dc_voltage_control;
        unsigned int dc_base_voltage_in_kV;

        vector<VSC_HVDC_CONVERTER_STRUCT> converters;
        vector<VSC_HVDC_DC_BUS_STRUCT>  dc_buses;
        vector<VSC_HVDC_DC_LINE_STRUCT> dc_lines;

        STEPS_SPARSE_MATRIX dc_network_matrix;
        STEPS_SPARSE_MATRIX jacobian;
        INPHNO inphno;

        vector<double> active_power, reactive_power;
        vector<double> Pdc_command, P_converter_loss, P_mismatch, Udc, Udc_mismatch;
        vector<double> generation_power, load_power;
        vector<double> bus_power, bus_current;
        vector<int> alpha, beta, Kdp_droop, Kdi_droop, Ud_reference, Id_reference;

        unsigned int iteration_count, max_iteration;
        double P_threshold_in_MW;
        double max_P_mismatch_in_MW;
        double P_slack_dc_side;
        double P_slack_ac_side;
        bool converged;

        VSC_HVDC_MODEL* vsc_hvdc_model;
};
#endif // VSC_HVDC_H
