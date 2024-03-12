#ifndef CONTINGENCY_SCREENER_H
#define CONTINGENCY_SCREENER_H

#include "header/meter/meter.h"
#include "header/power_system_database.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include <fstream>

class CONTINGENCY_SCREENER
{
    public:
        CONTINGENCY_SCREENER();
        virtual ~CONTINGENCY_SCREENER();

        void set_power_system_database_maximum_bus_number(unsigned int number);
        unsigned int get_power_system_database_maximum_bus_number() const;


        void scan_N_k_ac_ine_fault(unsigned int k);
        void scan_N_k_ac_ine_trip(unsigned int k);
        void scan_N_k_generator_trip(unsigned int k);
        void scan_N_k_lcc_hvdc2t_block(unsigned int k);


        void scan_ac_line_fault(FAULT fault, double flocation, double clearing_time, bool trip_after_clear);
        void scan_N_k_ac_ine_trip(vector<DEVICE_ID> lines);
        void scan_N_1_generator_trip();
        void scan_N_2_generator_trip();
        void scan_N_1_2t_lcc_hvdc_bypass(double recover_time);
        void scan_N_1_2t_lcc_hvdc_block(double unblock_time);


        void set_powerflow_data_filename(string filename);
        void set_dynamic_data_filename(string filename);

        void set_fault_device(DEVICE_ID did);
        void set_fault_side_bus(unsigned int bus);
        void set_fault_location_to_fault_side_bus_in_pu(double location);
        void set_fault_shunt_in_pu(complex<double> shunt);
        void set_flag_trip_ac_line_after_clearing_fault(bool flag);

        void set_simulator_max_iteration(unsigned int iteration);
        void set_simulator_allowed_max_power_imbalance_in_MVA(double tol);
        void set_simulator_iteration_accelerator(double alpha);

        string get_powerflow_data_filename() const;
        string get_dynamic_data_filename() const;

        DEVICE_ID get_fault_device() const;
        unsigned int get_fault_side_bus() const;
        double get_fault_location_to_fault_side_bus_in_pu() const;
        complex<double> get_fault_shunt_in_pu() const;
        bool get_flag_trip_ac_line_after_clearing_fault() const;

        unsigned int get_simulator_max_iteration() const;
        double get_simulator_allowed_max_power_imbalance_in_MVA() const;
        double get_simulator_iteration_accelerator() const;


        // the following 5 function are not supposed to changed by user. USE DEFAULT
        void set_fault_time_in_s(double time);
        void set_minimum_clearing_time_in_s(double time);
        void set_maximum_clearing_time_in_s(double time);
        void set_simulation_time_span_in_s(double time);
        void set_angle_difference_threshold_in_deg(double angle);

        double get_fault_time_in_s() const;
        double get_minimum_clearing_time_in_s() const;
        double get_maximum_clearing_time_in_s() const;
        double get_simulation_time_span_in_s() const;
        double get_angle_difference_threshold_in_deg() const;

        void append_generator_to_monitor(const DEVICE_ID& did);

        double search_cct();
    private:
        bool is_searcher_is_properly_set() const;
        bool perform_simulation_with_clearing_time(double clearing_time);
        unsigned int get_monitored_generator_count() const;
        DEVICE_ID get_monitored_generator(unsigned int i) const;
        void set_meters(DYNAMICS_SIMULATOR& simulator);
        void apply_fault(DYNAMICS_SIMULATOR& simulator);
        void clear_fault(DYNAMICS_SIMULATOR& simulator);
        bool check_if_system_is_stable(DYNAMICS_SIMULATOR& simulator) const;

        string search_title;
        string powerflow_data_filename, dynamic_data_filename;

        DEVICE_ID fault_device;
        unsigned int fault_side_bus;
        double fault_location;
        complex<double> fault_shunt_in_pu;
        double fault_time_in_s;
        double minimum_clearing_time, maximum_clearing_time;
        bool flag_trip_ac_line_after_clearing_fault;
        double simulation_time_span_in_s;
        double angle_difference_threshold_in_deg;

        double P_threshold_in_MW, Q_threshold_in_MVar, alpha;
        unsigned int max_iteration;

        vector<DEVICE_ID> monitored_generators;
    private:
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
};
#endif // CONTINGENCY_SCREENER_H
