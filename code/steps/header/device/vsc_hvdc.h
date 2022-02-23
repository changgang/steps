#ifndef VSC_HVDC_H
#define VSC_HVDC_H

#include "header/device/nonbus_device.h"
#include "header/device/vsc_hvdc_var.h"
#include "header/basic/device_id.h"
#include "header/basic/dc_device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/vsc_hvdc_network_model.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_model.h"
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
        void set_converter_name(const unsigned int index, string name);
        void set_converter_status(const unsigned int index, bool status);

        void set_converter_active_power_operation_mode(const unsigned int index, const VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE mode);
        void set_converter_dc_voltage_control_priority(const unsigned int index, const unsigned int p);
        void set_converter_reactive_power_operation_mode(const unsigned int index, const VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE mode);

        void set_converter_nominal_ac_active_power_command_in_MW(const unsigned int index, const double P);
        void set_converter_nominal_dc_voltage_command_in_kV(const unsigned int index, const double V);
        void set_converter_initial_dc_voltage_reference_in_kV(const unsigned int index, const double V);
        void set_converter_initial_dc_active_power_reference_in_MW(const unsigned int index, const double P);
        void set_converter_initial_dc_current_reference_in_kA(const unsigned int index, const double I);
        void set_converter_initial_droop_coefficient_for_droop_control(const unsigned int index, const double droop);

        void set_converter_nominal_ac_voltage_command_in_pu(const unsigned int index, const double V);
        void set_converter_nominal_ac_reactive_power_command_in_Mvar(const unsigned int index, const double Q);

        void set_converter_loss_factor_A_in_kW(const unsigned int index, const double A);
        void set_converter_loss_factor_B_in_kW_per_amp(const unsigned int index, const double B);
        void set_converter_loss_factor_C_in_KW_per_amp_squared(const unsigned int index, const double C);
        void set_converter_minimum_loss_in_kW(const unsigned int index, const double P);

        void set_converter_rated_capacity_in_MVA(const unsigned int index, const double S);
        void set_converter_rated_current_in_A(const unsigned int index, const double I);

        void set_converter_commutating_impedance_in_ohm(unsigned int index, const complex<double> z);
        void set_converter_commutating_impedance_in_pu(unsigned int index, const complex<double> z_in_pu);
        void set_converter_P_to_AC_bus_in_MW(unsigned int index, double P);
        void set_converter_Q_to_AC_bus_in_MVar(unsigned int index, double Q);
        void set_converter_Pmax_in_MW(const unsigned int index, const double P);
        void set_converter_Pmin_in_MW(const unsigned int index, const double P);
        void set_converter_Qmax_in_MVar(const unsigned int index, const double Q);
        void set_converter_Qmin_in_MVar(const unsigned int index, const double Q);
        void set_converter_Udmax_in_kV(const unsigned int index, const double Udmax);
        void set_converter_Udmin_in_kV(const unsigned int index, const double Udmin);

        void set_converter_remote_bus_to_regulate(const unsigned int index, const unsigned int bus);
        void set_converter_remote_regulation_percent(const unsigned int index, const double rmpct);

        void set_dc_bus_number(const unsigned int index, const unsigned int bus);
        void set_dc_bus_name(const unsigned int index, const string name);
        void set_dc_bus_converter_index_with_ac_bus_number(const unsigned int index, const unsigned int bus);
        void set_dc_bus_Vdc_in_kV(const unsigned int index, const double Udc);
        void set_dc_bus_area(const unsigned int index, const unsigned int area);
        void set_dc_bus_zone(const unsigned int index, const unsigned int zone);
        void set_dc_bus_owner_number(const unsigned int index, const unsigned int owner);
        void set_dc_bus_ground_resistance_in_ohm(const unsigned int index, const double R);
        void set_dc_bus_generation_power_in_MW(const unsigned int index, const double P);
        void set_dc_bus_load_power_in_MW(const unsigned int index, const double P);

        void set_dc_line_sending_side_bus(const unsigned int index, const unsigned int bus);
        void set_dc_line_receiving_side_bus(const unsigned int index, const unsigned int bus);
        void set_dc_line_identifier(const unsigned int index, const string identifier);
        void set_dc_line_status(const unsigned int index, bool status);
        void set_dc_line_meter_end(const unsigned int index, const unsigned int meter_end);
        void set_dc_line_resistance_in_ohm(const unsigned int index, const double R);
        void set_dc_line_inductance_in_mH(const unsigned int index, const double L);
        void set_dc_line_fault_location(const unsigned int index, const unsigned int bus, double location);
        void set_dc_line_fault_r_in_ohm(const unsigned int index, const double r_ohm);
        void clear_dc_line_fault(const unsigned int index);

        virtual void clear();

        string get_identifier() const;
        unsigned int get_identifier_index() const;
        string get_name() const;
        unsigned int get_name_index() const;
        unsigned int get_converter_count() const;
        unsigned int get_dc_bus_count() const;
        unsigned int get_dc_line_count() const;
        bool get_status() const;
        double get_dc_network_base_voltage_in_kV() const;

        VSC_HVDC_CONVERTER_STRUCT* get_converter(unsigned int index);
        unsigned int get_converter_ac_bus(unsigned int index) const;
        unsigned int get_converter_index_with_converter_name(string name) const;
        string get_converter_name(unsigned int index) const;
        unsigned int get_converter_name_index(unsigned int index) const;
        BUS* get_converter_ac_bus_pointer(unsigned int index) const;
        unsigned int get_converter_index_with_ac_bus(unsigned int bus) const;
        unsigned int get_converter_index_with_dc_bus(unsigned int bus) const;
        bool get_converter_status(unsigned int index) const;
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE get_converter_active_power_operation_mode(unsigned int index) const;
        unsigned int get_converter_dc_voltage_control_priority(const unsigned int index) const;
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE get_converter_reactive_power_operation_mode(unsigned int index) const;

        double get_converter_nominal_ac_active_power_command_in_MW(unsigned int index) const;
        double get_converter_nominal_ac_active_power_command_in_pu(unsigned int index) const;
        double get_converter_nominal_dc_voltage_command_in_kV(unsigned int index) const;
        double get_converter_nominal_dc_voltage_command_in_pu(unsigned int index) const;

        double get_converter_nominal_ac_voltage_command_in_kV(unsigned int index) const;
        double get_converter_nominal_ac_voltage_command_in_pu(unsigned int index) const;
        double get_converter_initial_dc_voltage_reference_in_kV(const unsigned int index)const;
        double get_converter_initial_dc_current_reference_in_kA(const unsigned int index)const;
        double get_converter_initial_dc_active_power_reference_in_MW(const unsigned int index)const;
        double get_converter_initial_droop_coefficient_for_droop_control(const unsigned int index)const;

        double get_converter_nominal_ac_reactive_power_command_in_Mvar(unsigned int index) const;
        double get_converter_nominal_reactive_power_command_in_pu(unsigned int index) const;

        double get_converter_loss_factor_A_in_kW(unsigned int index) const;
        double get_converter_loss_factor_B_in_kW_per_amp(unsigned int index) const;
        double get_converter_loss_factor_C_in_kW_per_amp_squard(unsigned int index) const;
        double get_converter_minimum_loss_in_kW(unsigned int index) const;

        double get_converter_rated_capacity_in_MVA(unsigned int index) const;
        double get_converter_rated_current_in_A(unsigned int index) const;

        complex<double> get_converter_commutating_impedance_in_ohm(unsigned int index) const;
        complex<double> get_converter_commutating_impedance_in_pu_on_converter_base(unsigned int index) const;
        complex<double> get_converter_commutating_impedance_in_pu_on_system_base(unsigned int index) const;

        double get_converter_P_to_AC_bus_in_MW(unsigned int index) const;
        double get_converter_Q_to_AC_bus_in_MVar(unsigned int index) const;
        double get_converter_Pmax_in_MW(const unsigned int index) const;
        double get_converter_Pmin_in_MW(const unsigned int index) const;
        double get_converter_Qmax_in_MVar(unsigned int index) const;
        double get_converter_Qmin_in_MVar(unsigned int index) const;
        double get_converter_Udmax_in_kV(const unsigned int index) const;
        double get_converter_Udmin_in_kV(const unsigned int index) const;

        unsigned int get_converter_remote_bus_to_regulate(unsigned int index) const;
        double get_converter_remote_regulation_percent(unsigned int index) const;

        unsigned int get_dc_bus_number(unsigned int index) const;
        string get_dc_bus_name (unsigned int index) const;
        unsigned int get_converter_ac_bus_number_with_dc_bus_index(unsigned int index) const;
        double get_dc_bus_Vdc_in_kV(unsigned int index) const;
        double get_dc_bus_Vdc_in_kV_with_dc_bus_number(unsigned int bus) const;
        unsigned int get_dc_bus_area(unsigned int index) const;
        unsigned int get_dc_bus_zone(unsigned int index) const;
        unsigned int get_dc_bus_owner_number(unsigned int index) const;
        double get_dc_bus_ground_resistance_in_ohm(unsigned int index) const;
        double get_dc_bus_generation_power_in_MW(unsigned int index) const;
        double get_dc_bus_load_power_in_MW(unsigned int index) const;

        unsigned int get_dc_line_sending_side_bus(unsigned int index) const;
        unsigned int get_dc_line_receiving_side_bus(unsigned int index) const;
        string get_dc_line_identifier(unsigned int index) const;
        bool get_dc_line_status(unsigned int index) const;
        DC_DEVICE_ID get_dc_line_device_id(unsigned int index) const;
        unsigned int get_dc_line_meter_end(unsigned int index) const;
        unsigned int get_dc_line_meter_end_bus(unsigned int index) const;
        double get_dc_line_resistance_in_ohm(unsigned int index) const;
        double get_dc_line_inductance_in_mH(unsigned int index) const;
        double get_dc_line_fault_location(const unsigned int index) const;
        double get_dc_line_fault_r_in_ohm(const unsigned int index) const;

        unsigned int get_dc_line_index(DC_DEVICE_ID& did) const;

        double get_dc_line_current_in_kA(unsigned int index, LINE_SIDE meter_side) const;
        double get_dc_line_power_in_MW(unsigned int index, LINE_SIDE meter_side) const;
        double get_dc_line_current_in_kA(unsigned int index, unsigned int dc_bus) const;
        double get_dc_line_power_in_MW(unsigned int index, unsigned int dc_bus) const;
        double get_dc_line_current_in_kA(DC_DEVICE_ID dc_did, unsigned int meter_side) const;
        double get_dc_line_power_in_MW(DC_DEVICE_ID dc_did, unsigned int meter_side) const;
        double get_dc_line_loss_in_MW(unsigned int index) const;

        VSC_HVDC& operator=(const VSC_HVDC& vsc);

        bool is_connected_to_dc_bus(unsigned int bus) const;

        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual bool is_valid() const;
        virtual void check();
        virtual void report() const;
        virtual void save() const;
        virtual DEVICE_ID get_device_id() const;
        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        void set_vsc_hvdc_network_model(VSC_HVDC_NETWORK_MODEL* model);
        void set_vsc_hvdc_converter_model(VSC_HVDC_CONVERTER_MODEL* model);

        VSC_HVDC_NETWORK_MODEL* get_vsc_hvdc_network_model() const;
        VSC_HVDC_CONVERTER_MODEL* get_vsc_hvdc_converter_model(unsigned int index) const;
        vector<VSC_HVDC_CONVERTER_MODEL*> get_vsc_hvdc_converter_models() const;

        bool is_converter_a_dynamic_voltage_source(unsigned int index) const;

        complex<double> get_converter_Norton_admittance_as_voltage_source(unsigned int index) const;

        virtual void run(DYNAMIC_MODE mode);

        void solve_steady_state();

        void initialize_steady_state_solver();
        void initialize_dc_bus_voltage();

        void optimize_network_ordering();
        void initialize_physical_internal_bus_pair();
        void reorder_physical_internal_bus_pair();
        void set_internal_bus_pointer();
        INPHNO get_network_internal_physical_number_inphno() const;

        void update_P_equation_internal_buses();

        void build_dc_network_matrix();
        void build_initial_zero_matrix();
        void add_dc_lines_to_dc_network();

        void calculate_raw_bus_power_mismatch();
        void build_dc_bus_power_mismatch_vector();
        void build_jacobian();
        double calculate_jacobian_matrix_entry(unsigned int k);
        void update_current_dc_slack_bus();
        unsigned int get_dc_slack_bus_number() const;

        unsigned int get_converter_index_of_dc_voltage_control_priority(unsigned int p) const;

        void update_converters_P_and_Q_to_AC_bus();
        void update_converter_P_and_Q_to_AC_bus(unsigned int index);
        void update_converters_P_to_DC_network();
        void update_converter_P_to_DC_network(unsigned int index);

        void set_max_iteration(unsigned int iteration);
        void set_allowed_max_P_mismatch_in_MW(double max_mismatch);

        double get_maximum_active_power_mismatch_in_MW();
        double get_allowed_max_active_power_imbalance_in_MW();
        unsigned int get_iteration_count();
        double get_max_iteration();
        unsigned int get_index_of_dc_bus_number(unsigned int bus);
        double get_dc_voltage_of_dc_bus_number(unsigned int bus);
        unsigned int get_dc_bus_index_with_ac_bus_number(unsigned int bus);

        int get_alpha_of_dc_bus_number(unsigned int bus);
        int get_beta_of_dc_bus_number(unsigned int bus);
        int get_converter_alpha(unsigned int index) const;
        int get_converter_beta(unsigned int index) const;

        void initialize_alpha_vector();
        void initialize_beta_vector();
        void initialize_active_and_reactive_power_control_mode_vector();

        void update_dc_bus_voltage();
        void calculate_raw_dc_current_into_dc_network();
        void calculate_raw_dc_power_into_dc_network();
        void add_generation_power_to_raw_bus_power_mismatch();
        void add_load_power_to_raw_bus_power_mismatch();
        void add_converter_dc_power_command_to_raw_bus_power_mismatch();
        double get_converter_dc_power_command_at_dc_bus(unsigned int bus);
        double get_converter_dc_power_command(unsigned int converter_index);

        double solve_Pdc_with_active_power_control_and_reactive_power_control(unsigned int converter_index) const;
        double solve_Pdc_with_active_power_control_and_ac_voltage_control(unsigned int converter_index) const;
        double solve_Pdc_with_voltage_angle_and_reactive_power_control(unsigned int converter_index) const;
        double solve_Pdc_with_voltage_angle_and_voltage_control(unsigned int converter_index) const;
        double solve_Pdc_with_dc_active_power_voltage_droop_control(unsigned int converter_index) const;
        double solve_Pdc_with_dc_current_voltage_droop_control(unsigned int converter_index) const;

        double get_converter_dc_power_from_converter_to_dc_network_with_P_and_Q_to_AC_bus_in_MVA(unsigned int converter_index, double P, double Q) const;
        double get_converter_dc_power_from_dc_network_to_converter_with_P_and_Q_to_AC_bus_in_MVA(unsigned int converter_index, double P, double Q) const;

        double solve_converter_Pac_with_Pdc(unsigned int converter_index);

        void set_converter_Pdc_command_to_dc_network_in_MW(const unsigned int converter_index, const double P);
        double get_converter_Pdc_command_to_dc_network_in_MW(unsigned int converter_index) const;

        void set_converter_Pdc_from_Ceq_to_DC_network_in_MW(const unsigned int converter_index, const double P);
        double get_converter_Pdc_from_Ceq_to_DC_network_in_MW(unsigned int converter_index) const;

        void set_convergence_flag(bool flag);
        void save_dc_bus_powerflow_result_to_file(const string& filename) const;
        void calculate_dc_active_power_of_slack_bus();
        bool check_dc_slack_converter_constraint();
        bool is_dc_network_matrix_set();
        bool is_jacobian_matrix_set();

        bool get_convergence_flag() const;
        void export_dc_network_matrix(string filename);
        void show_inphno_bus_number();
        void show_dc_network_matrix();
        void export_dc_bus_voltage_with_network_ordering();
        void show_jacobian_matrix();
        double get_dc_network_matrix_entry_between_dc_bus(unsigned int ibus, unsigned int jbus);
        double get_jacobian_matrix_entry_between_dc_bus(unsigned int ibus, unsigned int jbus);

        complex<double> get_converter_ac_voltage_in_pu_with_ac_bus_number(unsigned int bus);
        complex<double> get_converter_ac_voltage_in_kV_with_ac_bus_number(unsigned int bus);
        complex<double> get_converter_ac_current_in_kA_with_ac_bus_number(unsigned int bus);
        double get_converter_dc_voltage_in_kV_with_ac_bus_number(unsigned int bus);
        double get_converter_dc_current_in_kA_with_ac_bus_number(unsigned int bus);
        double get_converter_dc_power_in_MW_with_ac_bus_number(unsigned int bus);
        double get_converter_dynamic_ac_active_power_in_MW_with_ac_bus_number(unsigned int bus);
        double get_converter_dynamic_ac_reactive_power_in_MVar_with_ac_bus_number(unsigned int bus);

        unsigned int get_dc_bus_with_converter_index(unsigned int converter_index) const;
        unsigned int get_dc_bus_index_with_converter_index(unsigned int converter_index) const;
        complex<double> get_converter_dynamic_current_to_ac_bus_in_pu_on_system_base_as_current_source(unsigned int converter_index);
        complex<double> get_converter_dynamic_Norton_current_to_ac_bus_in_pu_on_system_base_as_voltage_source(unsigned int converter_index);


        double get_converter_ac_bus_base_voltage_in_kV(unsigned int index) const;
        complex<double> get_converter_ac_bus_positive_sequency_complex_voltage_in_pu(unsigned int index) const;
        complex<double> get_converter_ac_bus_positive_sequency_complex_voltage_in_kV(unsigned int index) const;

    public:
        unsigned int get_dc_bus_converter_index_with_dc_bus_index(unsigned int index) const;
        unsigned int dc_bus_no2index(unsigned int bus) const;
        unsigned int dc_bus_index2no(unsigned int index) const;
        unsigned int dc_line_device_id2index(DC_DEVICE_ID dc_did) const;
        DC_DEVICE_ID dc_line_index2device_id(unsigned int index) const;

        unsigned int get_dc_bus_converter_index_with_dc_bus_number(unsigned int bus) const;
    private:
        void copy_from_const_vsc(const VSC_HVDC& vsc);

        bool converter_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool dc_bus_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool dc_line_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;

        void delete_vsc_hvdc_network_model();
        void delete_vsc_hvdc_converter_models();
        void delete_vsc_hvdc_converter_model(unsigned int index);
    private:
        unsigned int vsc_hvdc_name_index;
        unsigned int identifier_index;
        bool status;
        unsigned int dc_base_voltage_in_kV;

        vector<VSC_HVDC_CONVERTER_STRUCT> converters;
        vector<VSC_HVDC_DC_BUS_STRUCT>  dc_buses;
        vector<VSC_HVDC_DC_LINE_STRUCT> dc_lines;

        STEPS_SPARSE_MATRIX dc_network_matrix;
        STEPS_SPARSE_MATRIX jacobian;
        INPHNO inphno;

        vector<unsigned int> internal_P_equation_buses;
        vector<VSC_HVDC_DC_BUS_STRUCT*> internal_bus_pointers;;

        vector<VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE> current_active_power_control_mode;
        vector<VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE> current_reactive_power_control_mode;
        vector<double> P_mismatch, Udc_mismatch;
        vector<double> bus_power, bus_current;
        vector<int> alpha, beta, Kdp_droop, Kdi_droop, Ud_reference, Id_reference;

        unsigned int iteration_count, max_iteration;
        double P_threshold_in_MW;
        double max_P_mismatch_in_MW;
        double P_slack_ac_side;
        unsigned int current_dc_slack_bus;
        bool converged;

        VSC_HVDC_NETWORK_MODEL* vsc_hvdc_network_model;
        vector<VSC_HVDC_CONVERTER_MODEL*> vsc_hvdc_converter_models;

};
#endif // VSC_HVDC_H
