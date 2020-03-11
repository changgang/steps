#ifndef CCT_SEARCHER_H
#define CCT_SEARCHER_H

#include "header/meter/meter.h"
#include "header/power_system_database.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include <fstream>

class CCT_SEARCHER
{
    public:
        CCT_SEARCHER();
        virtual ~CCT_SEARCHER();

        void set_power_system_database_maximum_bus_number(unsigned int number);
        void set_search_title(string title);

        void set_powerflow_data_filename(string filename);
        void set_dynamic_data_filename(string filename);

        void set_fault_device(DEVICE_ID did);
        void set_fault_side_bus(unsigned int bus);
        void set_fault_location_to_fault_side_bus_in_pu(double location);
        void set_fault_shunt_in_pu(complex<double> shunt);
        void set_flag_trip_line_after_clearing_fault(bool flag);

        void set_simulator_max_iteration(unsigned int iteration);
        void set_simulator_allowed_max_power_imbalance_in_MVA(double tol);
        void set_simulator_iteration_accelerator(double alpha);

        unsigned int get_power_system_database_maximum_bus_number() const;
        string get_search_title() const;

        string get_powerflow_data_filename() const;
        string get_dynamic_data_filename() const;

        DEVICE_ID get_fault_device() const;
        unsigned int get_fault_side_bus() const;
        double get_fault_location_to_fault_side_bus_in_pu() const;
        complex<double> get_fault_shunt_in_pu() const;
        bool get_flag_trip_line_after_clearing_fault() const;

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

        double search_cct();

        void run_case_with_clearing_time(double time);

    private:
        bool is_searcher_is_properly_set() const;

        void prepare_generators_in_islands(DYNAMICS_SIMULATOR& simulator);

        bool perform_simulation_with_clearing_time(double clearing_time);

        void apply_fault(DYNAMICS_SIMULATOR& simulator);
        void clear_fault(DYNAMICS_SIMULATOR& simulator);
        bool check_if_system_is_stable() const;
        //bool check_if_system_is_stable(DYNAMICS_SIMULATOR& simulator) const;

        string search_title;
        string powerflow_data_filename, dynamic_data_filename;

        DEVICE_ID fault_device;
        unsigned int fault_side_bus;
        double fault_location;
        complex<double> fault_shunt_in_pu;
        double fault_time_in_s;
        double minimum_clearing_time, maximum_clearing_time;
        bool flag_trip_line_after_clearing_fault;
        double simulation_time_span_in_s;
        double angle_difference_threshold_in_deg;

        double P_threshold_in_MW, Q_threshold_in_MVar, alpha;
        unsigned int max_iteration;
        double delt;

        vector< vector<GENERATOR*> > generators_in_islands;
    private:
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
};
#endif // CCT_SEARCHER_H
