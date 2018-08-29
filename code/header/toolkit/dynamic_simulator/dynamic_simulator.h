#ifndef DYNAMICS_SIMULATOR_H
#define DYNAMICS_SIMULATOR_H

#include "header/basic/base.h"
#include "header/meter/meter.h"
#include "header/basic/sparse_matrix.h"
//#include "header/power_system_database.h"
#include "header/network_database.h"
#include <fstream>

class POWER_SYSTEM_DATABASE;

class DYNAMICS_SIMULATOR: public BASE
{
    public:
        DYNAMICS_SIMULATOR(POWER_SYSTEM_DATABASE* psdb);
        ~DYNAMICS_SIMULATOR();
        virtual void clear();

        bool is_power_system_database_set() const;

        NETWORK_DATABASE* get_network_database() const;

        void set_csv_file_export_enable_flag(bool flag);
        void set_json_file_export_enable_flag(bool flag);
        bool is_csv_file_export_enabled() const;
        bool is_json_file_export_enabled() const;

        //void set_current_simulation_time_in_s(double time);
        //double get_current_simulation_time_in_s() const;

        void set_max_DAE_iteration(size_t iteration);
        void set_max_network_iteration(size_t iteration);
        void set_max_update_event_iteration(size_t iteration);
        void set_allowed_max_power_imbalance_in_MVA(double tol);
        void set_iteration_accelerator(double alpha);
        void set_rotor_angle_stability_survilliance_flag(bool flag);
        void set_rotor_angle_stability_threshold_in_deg(double angle_th);

        size_t get_max_DAE_iteration() const;
        size_t get_max_network_iteration() const;
        size_t get_max_update_event_iteration() const;
        double get_allowed_max_power_imbalance_in_MVA() const;
        double get_iteration_accelerator() const;
        bool get_rotor_angle_stability_survilliance_flag() const;
        double get_rotor_angle_stability_threshold_in_deg() const;

        void prepare_meters();
        void prepare_bus_related_meters();
        void prepare_generator_related_meters();
        void prepare_wt_generator_related_meters();
        void prepare_load_related_meters();
        void prepare_line_related_meters();
        void prepare_hvdc_related_meters();
        void prepare_equivalent_device_related_meters();

        void prepare_bus_related_meter(size_t bus, string meter_type);
        void prepare_generator_related_meter(DEVICE_ID did, string meter_type);
        void prepare_wt_generator_related_meter(DEVICE_ID did, string meter_type);
        void prepare_load_related_meter(DEVICE_ID did, string meter_type);
        void prepare_line_related_meter(DEVICE_ID did, string meter_type, string side);
        void prepare_hvdc_related_meter(DEVICE_ID did, string meter_type, string side);
        void prepare_equivalent_device_related_meter(DEVICE_ID did, string meter_type);

        void append_meter(const METER& meter);
        size_t get_meter_count() const;
        METER get_meter(size_t i);
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


    public: // events
        void change_dynamic_simulator_time_step(double newDELT);

        void set_bus_fault(size_t bus, complex<double> fault_shunt);
        void clear_bus_fault(size_t bus);
        void trip_bus(size_t bus);
        void trip_buses(const vector<size_t> buses);

        void set_line_fault(DEVICE_ID line_id, size_t side_bus, double location, complex<double> fault_shunt);
        void clear_line_fault(DEVICE_ID line_id, size_t side_bus, double location);
        void trip_line(DEVICE_ID line_id);
        void trip_line_breaker(DEVICE_ID line_id, size_t side_bus);
        void close_line(DEVICE_ID line_id);
        void close_line_breaker(DEVICE_ID line_id, size_t side_bus);

        void trip_transformer(DEVICE_ID trans_id);
        void trip_transformer_breaker(DEVICE_ID trans_id, size_t side_bus);
        void close_transformer(DEVICE_ID trans_id);
        void close_transformer_breaker(DEVICE_ID trans_id, size_t side_bus);

        void trip_generator(DEVICE_ID gen_id);
        void shed_generator(DEVICE_ID gen_id,double percent);

        void trip_load(DEVICE_ID load_id);
        void close_load(DEVICE_ID load_id);
        void scale_load(DEVICE_ID load_id, double percent);
        void scale_all_load(double percent);

        void trip_fixed_shunt(DEVICE_ID shunt_id);
        void close_fixed_shunt(DEVICE_ID shunt_id);


        void manual_bypass_hvdc(DEVICE_ID hvdc_id);
        void manual_block_hvdc(DEVICE_ID hvdc_id);
        void manual_unbypass_hvdc(DEVICE_ID hvdc_id);
        void manual_unblock_hvdc(DEVICE_ID hvdc_id);

        void change_generator_voltage_reference_in_pu(DEVICE_ID gen_id, double vref);
        void change_generator_power_reference_in_MW(DEVICE_ID gen_id, double Pref);

        void switch_on_equivalent_device();

    private:
        void integrate();
        void update();
        double get_system_max_angle_difference_in_deg();

        void open_meter_output_files();
        void close_meter_output_files();
        void save_meter_information();
        void save_meter_values();

        bool solve_network();
        void solve_hvdcs_without_integration();

        void get_bus_current_mismatch();
        void get_bus_currnet_into_network();
        void add_generators_to_bus_current_mismatch();
        void add_wt_generators_to_bus_current_mismatch();
        void add_loads_to_bus_current_mismatch();
        void add_hvdcs_to_bus_current_mismatch();
        void add_equivalent_devices_to_bus_current_mismatch();

        bool is_converged();
        void get_bus_power_mismatch_in_MVA();
        double get_max_power_mismatch_in_MVA();

        void build_bus_current_mismatch_vector();

        void build_jacobian();

        void check_convergence() const;


        double get_max_active_power_imbalance_in_MW() const;
        double get_max_reactive_power_imbalance_in_MVar() const;

        void build_bus_delta_current_vector();

        void solve_bus_voltage_update();

        void update_bus_voltage();

        void update_network_dynamic_matrix();

        void guess_bus_voltage_with_bus_fault_set(size_t bus, FAULT fault);
        void guess_bus_voltage_with_bus_fault_cleared(size_t bus, FAULT fault);

        void guess_bus_voltage_with_line_fault_set(DEVICE_ID did, size_t side_bus, double location, FAULT fault);
        void guess_bus_voltage_with_line_fault_cleared(DEVICE_ID did, size_t side_bus, double location, FAULT fault);

        void update_generators_in_islands();
        bool is_system_angular_stable() const;

        //double TIME;
        size_t ITER_DAE, ITER_NET;
        double P_threshold_in_MW, Q_threshold_in_MVar;
        size_t network_iteration_count, DAE_iteration_count;
        size_t max_network_iteration, max_DAE_iteration, max_update_event_iteration;
        double alpha;

        vector< complex<double> > I_mismatch, S_mismatch;
        vector<double> I_vec;

        vector<double> delta_V;

        SPARSE_MATRIX jacobian;

        NETWORK_DATABASE* network_db;

        vector<METER> meters;
        vector<double> meter_values;

        bool flag_rotor_angle_stability_survilliance;
        double rotor_angle_stability_threshold_in_deg;
        vector< vector<GENERATOR*> > generators_in_islands;

        string output_filename;
        ofstream csv_output_file, json_output_file;

        bool csv_file_export_enabled, json_file_export_enabled;
    private:
        virtual bool is_valid() const;
        virtual void check();
};
#endif // DYNAMICS_SIMULATOR_H
