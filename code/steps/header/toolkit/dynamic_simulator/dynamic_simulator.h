#ifndef DYNAMICS_SIMULATOR_H
#define DYNAMICS_SIMULATOR_H

#include "header/basic/power_mismatch_struct.h"
#include "header/meter/meter.h"
#include "header/network/network_matrix.h"
#include "header/basic/sparse_matrix_define.h"
#include <fstream>

class POWER_SYSTEM_DATABASE;

class DYNAMICS_SIMULATOR
{
    public:
        DYNAMICS_SIMULATOR(STEPS& toolkit);
        ~DYNAMICS_SIMULATOR();
        STEPS& get_toolkit() const;

        void clear();

        void set_dynamic_simulation_time_step_in_s(double delt);
        double get_dynamic_simulation_time_step_in_s() const;
        void set_dynamic_simulation_time_in_s(double time);
        double get_dynamic_simulation_time_in_s() const;

        NETWORK_MATRIX& get_network_matrix();

        void set_csv_file_export_enable_flag(bool flag);
        void set_json_file_export_enable_flag(bool flag);
        void set_bin_file_export_enable_flag(bool flag);
        bool is_csv_file_export_enabled() const;
        bool is_json_file_export_enabled() const;
        bool is_bin_file_export_enabled() const;

        //void set_current_simulation_time_in_s(double time);
        //double get_current_simulation_time_in_s() const;

        void set_max_DAE_iteration(unsigned int iteration);
        void set_min_DAE_iteration(unsigned int iteration);
        void set_max_network_iteration(unsigned int iteration);
        void set_max_update_iteration(unsigned int iteration);
        void set_max_event_update_iteration(unsigned int iteration);
        void set_max_network_solution_divergent_threshold(unsigned int div_th);
        void set_allowed_max_power_imbalance_in_MVA(double tol);
        void set_iteration_accelerator(double alpha);
        void set_non_divergent_solution_logic(bool logic);
        void set_automatic_iteration_accelerator_tune_logic(bool logic);
        void set_rotor_angle_stability_surveillance_flag(bool flag);
        void set_rotor_angle_stability_threshold_in_deg(double angle_th);

        unsigned int get_max_DAE_iteration() const;
        unsigned int get_min_DAE_iteration() const;
        unsigned int get_max_network_iteration() const;
        unsigned int get_max_update_iteration() const;
        unsigned int get_max_event_update_iteration() const;
        unsigned int get_max_network_solution_divergent_threshold() const;
        double get_allowed_max_power_imbalance_in_MVA() const;
        double get_iteration_accelerator() const;
        bool get_non_divergent_solution_logic() const;
        bool get_automatic_iteration_accelerator_tune_logic() const;
        bool get_rotor_angle_stability_surveillance_flag() const;
        double get_rotor_angle_stability_threshold_in_deg() const;

        void show_dynamic_simulator_configuration() const;

        unsigned int get_memory_usage_in_bytes();

        void set_network_matrix_update_as_unrequired();
        void set_network_matrix_update_as_required();
        bool is_network_matrix_update_required() const;

        void prepare_meters();
        void prepare_bus_related_meters();
        void prepare_generator_related_meters();
        void prepare_wt_generator_related_meters();
        void prepare_pv_unit_related_meters();
        void prepare_energy_storage_related_meters();
        void prepare_load_related_meters();
        void prepare_line_related_meters();
        void prepare_transformer_related_meters();
        void prepare_hvdc_related_meters();
        void prepare_equivalent_device_related_meters();

        void prepare_bus_related_meter(unsigned int bus, string meter_type);
        void prepare_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        void prepare_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        void prepare_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        void prepare_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        void prepare_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");
        void prepare_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name="");
        void prepare_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name="");
        void prepare_hvdc_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name="");
        void prepare_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name="");

        void append_meter(const METER& meter);
        unsigned int get_meter_count() const;
        METER get_meter(unsigned int i);
        void update_all_meters_value();
        vector<double> get_all_meters_value();
        void clear_meters();

        void set_output_file(string filename);
        string get_output_file() const;

        void start();
        void stop();
        void run_to(double time);
        void run_a_step();
        void update_with_event();
        void run_all_models(DYNAMIC_MODE mode);
        void update_bus_frequency_blocks();
        void update_equivalent_devices_buffer();
        void update_equivalent_devices_output();
    public:
        void enable_relay_action_flag();
    private:
        void disable_relay_action_flag();
        bool get_relay_actiion_flag() const;
    public: // events
        void change_dynamic_simulator_time_step(double newDELT);

        void set_bus_fault(unsigned int bus, const complex<double>& fault_shunt);
        void clear_bus_fault(unsigned int bus);
        void trip_bus(unsigned int bus);
        void trip_buses(const vector<unsigned int>& buses);

        void set_line_fault(const DEVICE_ID& line_id, unsigned int side_bus, double location, const complex<double>& fault_shunt);
        void clear_line_fault(const DEVICE_ID& line_id, unsigned int side_bus, double location);
        void trip_line(const DEVICE_ID& line_id);
        void trip_line_breaker(const DEVICE_ID& line_id, unsigned int side_bus);
        void close_line(const DEVICE_ID& line_id);
        void close_line_breaker(const DEVICE_ID& line_id, unsigned int side_bus);

        void trip_transformer(const DEVICE_ID& trans_id);
        void trip_transformer_breaker(const DEVICE_ID& trans_id, unsigned int side_bus);
        void close_transformer(const DEVICE_ID& trans_id);
        void close_transformer_breaker(const DEVICE_ID& trans_id, unsigned int side_bus);

        void trip_generator(const DEVICE_ID& gen_id);
        void shed_generator(const DEVICE_ID& gen_id, double percent);

        void trip_wt_generator(const DEVICE_ID& gen_id, unsigned int n);
        void shed_wt_generator(const DEVICE_ID& gen_id, double percent);

        void trip_load(const DEVICE_ID& load_id);
        void close_load(const DEVICE_ID& load_id);
        void scale_load(const DEVICE_ID& load_id, double percent);
        void scale_all_load(double percent);

        void trip_fixed_shunt(const DEVICE_ID& shunt_id);
        void close_fixed_shunt(const DEVICE_ID& shunt_id);


        void manual_bypass_hvdc(const DEVICE_ID& hvdc_id);
        void manual_block_hvdc(const DEVICE_ID& hvdc_id);
        void manual_unbypass_hvdc(const DEVICE_ID& hvdc_id);
        void manual_unblock_hvdc(const DEVICE_ID& hvdc_id);

        double get_generator_voltage_reference_in_pu(const DEVICE_ID& gen_id);
        double get_generator_mechanical_power_reference_in_pu_based_on_mbase(const DEVICE_ID& gen_id);
        double get_generator_mechanical_power_reference_in_MW(const DEVICE_ID& gen_id);

        double get_generator_excitation_voltage_in_pu(const DEVICE_ID& gen_id);
        double get_generator_mechanical_power_in_pu_based_on_mbase(const DEVICE_ID& gen_id);
        double get_generator_mechanical_power_in_MW(const DEVICE_ID& gen_id);

        void change_generator_voltage_reference_in_pu(const DEVICE_ID& gen_id, double vref);
        void change_generator_mechanical_power_reference_in_pu_based_on_mbase(const DEVICE_ID& gen_id, double Pref);
        void change_generator_mechanical_power_reference_in_MW(const DEVICE_ID& gen_id, double Pref);

        void change_generator_excitation_voltage_in_pu(const DEVICE_ID& gen_id, double efd);
        void change_generator_mechanical_power_in_pu_based_on_mbase(const DEVICE_ID& gen_id, double pmech);
        void change_generator_mechanical_power_in_MW(const DEVICE_ID& gen_id, double pmech);

        double get_hvdc_power_order_in_MW(const DEVICE_ID& hvdc_id);
        void change_hvdc_power_order_in_MW(const DEVICE_ID& hvdc_id, double porder);

        void switch_on_equivalent_device();
    private:
        void optimize_network_ordering();
        void set_internal_bus_pointer();
        void set_internal_bus_complex_voltage_vector();

        void prepare_devices_for_run();

        void integrate();
        void update();
        void update_relay_models();
        double get_system_max_angle_difference_in_deg();

        void open_meter_output_files();
        void close_meter_output_files();
        void save_meter_information();
        void save_meter_values();

        bool solve_network();
        void initialize_internal_bus_voltage_vector();

        void solve_hvdcs_without_integration();

        void get_bus_current_mismatch();
        void get_bus_currnet_into_network();
        void add_generators_to_bus_current_mismatch();
        void add_wt_generators_to_bus_current_mismatch();
        void add_pv_units_to_bus_current_mismatch();
        void add_loads_to_bus_current_mismatch();
        void add_hvdcs_to_bus_current_mismatch();
        void add_equivalent_devices_to_bus_current_mismatch();

        complex<double> get_bus_complex_voltage_in_pu_with_internal_bus_number(unsigned int internal_bus) const;

        bool is_converged();
        void calculate_bus_power_mismatch_in_MVA();
        GREATEST_POWER_CURRENT_MISMATCH_STRUCT get_max_power_mismatch_struct();
        GREATEST_POWER_CURRENT_MISMATCH_STRUCT get_max_current_mismatch_struct();

        void build_bus_current_mismatch_vector();

        void build_jacobian();

        void check_convergence() const;

        double get_max_active_power_imbalance_in_MW() const;
        double get_max_reactive_power_imbalance_in_MVar() const;

        void tune_iteration_accelerator_based_on_maximum_current_mismatch(double imax);

        void build_bus_delta_current_vector();

        void solve_bus_voltage_update();

        void update_bus_voltage();

        void update_network_dynamic_matrix();

        void guess_bus_voltage_with_bus_fault_set(unsigned int bus, const FAULT& fault);
        void guess_bus_voltage_with_bus_fault_cleared(unsigned int bus, const FAULT& fault);

        void guess_bus_voltage_with_line_fault_set(const DEVICE_ID& did, unsigned int side_bus, double location, const FAULT& fault);
        void guess_bus_voltage_with_line_fault_cleared(const DEVICE_ID& did, unsigned int side_bus, double location, const FAULT& fault);

        void update_generators_in_islands();
        bool is_system_angular_stable() const;

        STEPS* toolkit;

        double DELT;
        double TIME;

        unsigned int ITER_DAE, ITER_NET;
        size_t microseconds_elapse_of_differential_equations_in_a_step, microseconds_elapse_of_network_solution_in_a_step;
        double time_elapse_in_a_step, time_elapse_of_differential_equations_in_a_step, time_elapse_of_network_solution_in_a_step;
        double P_threshold_in_MW, Q_threshold_in_MVar;
        unsigned int network_iteration_count, DAE_iteration_count;
        unsigned int max_network_iteration, max_DAE_iteration, max_update_iteration, max_event_update_iteration, min_DAE_iteration, max_network_solution_divergent_threshold;
        unsigned int current_max_network_iteration;
        double alpha;
        bool non_divergent_solution_enabled;
        bool automatic_iteration_accelerator_tune_enabled;

        double max_current_mismatch_pu, max_power_mismatch_MVA;
        unsigned int max_mismatch_bus;

        vector<BUS*> in_service_buses;
        vector<GENERATOR*> generators;
        vector<WT_GENERATOR*> wt_generators;
        vector<PV_UNIT*> pv_units;
        vector<ENERGY_STORAGE*> e_storages;
        vector<LOAD*> loads;
        vector<LINE*> lines;
        vector<HVDC*> hvdcs;
        vector<EQUIVALENT_DEVICE*> e_devices;

        vector<BUS*> internal_bus_pointers;
        vector<double> internal_bus_voltage_in_pu;
        vector<complex<double> > internal_bus_complex_voltage_in_pu;

        vector< complex<double> > I_mismatch, S_mismatch;
        vector<double> I_vec;

        //vector<double> delta_V;
        //STEPS_SPARSE_MATRIX jacobian;
        vector<complex<double> > delta_V;
        STEPS_COMPLEX_SPARSE_MATRIX jacobian;

        vector<METER> meters;
        vector<double> meter_values;

        bool flag_rotor_angle_stability_surveillance;
        double rotor_angle_stability_threshold_in_deg;
        vector< vector<GENERATOR*> > generators_in_islands;

        string output_filename;
        ofstream csv_output_file, json_output_file, bin_output_file;

        bool csv_file_export_enabled, json_file_export_enabled, bin_file_export_enabled;

        bool relay_action_flag;

        bool detailed_log_enabled;

        bool network_matrix_update_required;
};

#endif // DYNAMICS_SIMULATOR_H
