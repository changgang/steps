#include "header/toolkit/contingency_screener/contingency_screener.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <istream>
#include <iostream>
#include <ctime>

using namespace std;

CONTINGENCY_SCREENER::CONTINGENCY_SCREENER()
{
    psdb.set_allowed_max_bus_number(10000);

    set_powerflow_data_filename("");
    set_dynamic_data_filename("");

    set_fault_side_bus(0);
    set_fault_location_to_fault_side_bus_in_pu(0.0);
    set_fault_shunt_in_pu(0.0);
    set_fault_time_in_s(0.0); // default value
    set_minimum_clearing_time_in_s(0.1); // default value
    set_maximum_clearing_time_in_s(1.5); // default value
    set_flag_trip_line_after_clearing_fault(false);
    set_simulation_time_span_in_s(5.0); // default value
    set_angle_difference_threshold_in_deg(170.0); // default value

    set_simulator_max_iteration(100);
    set_simulator_allowed_max_power_imbalance_in_MVA(0.0001);
    set_simulator_iteration_accelerator(1.0);
}

CONTINGENCY_SCREENER::~CONTINGENCY_SCREENER()
{
    ;
}


void CONTINGENCY_SCREENER::set_power_system_database_maximum_bus_number(size_t number)
{
    psdb.set_allowed_max_bus_number(number);
}

size_t CONTINGENCY_SCREENER::get_power_system_database_maximum_bus_number() const
{
    return psdb.get_allowed_max_bus_number();
}

void CONTINGENCY_SCREENER::set_powerflow_data_filename(string filename)
{
    powerflow_data_filename = filename;
}

void CONTINGENCY_SCREENER::set_dynamic_data_filename(string filename)
{
    dynamic_data_filename = filename;
}

void CONTINGENCY_SCREENER::set_fault_device(DEVICE_ID did)
{
    if(did.is_valid() and did.get_device_type()=="LINE")
        fault_device = did;
}

void CONTINGENCY_SCREENER::set_fault_side_bus(size_t bus)
{
    fault_side_bus = bus;
}

void CONTINGENCY_SCREENER::set_fault_location_to_fault_side_bus_in_pu(double location)
{
    fault_location = location;
}

void CONTINGENCY_SCREENER::set_fault_shunt_in_pu(complex<double> shunt)
{
    if(steps_fast_complex_abs(shunt)>FLOAT_EPSILON)
        fault_shunt_in_pu = shunt;
}

void CONTINGENCY_SCREENER::set_fault_time_in_s(double time)
{
    fault_time_in_s = time;
}

void CONTINGENCY_SCREENER::set_minimum_clearing_time_in_s(double time)
{
    minimum_clearing_time = time;
}

void CONTINGENCY_SCREENER::set_maximum_clearing_time_in_s(double time)
{
    maximum_clearing_time = time;
}

void CONTINGENCY_SCREENER::set_flag_trip_line_after_clearing_fault(bool flag)
{
    flag_trip_line_after_clearing_fault = flag;
}

void CONTINGENCY_SCREENER::set_simulation_time_span_in_s(double time)
{
    simulation_time_span_in_s = time;
}

void CONTINGENCY_SCREENER::set_angle_difference_threshold_in_deg(double angle)
{
    angle_difference_threshold_in_deg = angle;
}

void CONTINGENCY_SCREENER::set_simulator_max_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_iteration = iteration;
}

void CONTINGENCY_SCREENER::set_simulator_allowed_max_power_imbalance_in_MVA(double tol)
{
    if(tol>0.0)
    {
        P_threshold_in_MW = tol;
        Q_threshold_in_MVar = tol;
    }
}

void CONTINGENCY_SCREENER::set_simulator_iteration_accelerator(double iter_alpha)
{
    if(iter_alpha>0.0)
        this->alpha = iter_alpha;
}


string CONTINGENCY_SCREENER::get_powerflow_data_filename() const
{
    return powerflow_data_filename;
}

string CONTINGENCY_SCREENER::get_dynamic_data_filename() const
{
    return dynamic_data_filename;
}


DEVICE_ID CONTINGENCY_SCREENER::get_fault_device() const
{
    return fault_device;
}

size_t CONTINGENCY_SCREENER::get_fault_side_bus() const
{
    return fault_side_bus;
}

double CONTINGENCY_SCREENER::get_fault_location_to_fault_side_bus_in_pu() const
{
    return fault_location;
}

complex<double> CONTINGENCY_SCREENER::get_fault_shunt_in_pu() const
{
    return fault_shunt_in_pu;
}

double CONTINGENCY_SCREENER::get_fault_time_in_s() const
{
    return fault_time_in_s;
}

double CONTINGENCY_SCREENER::get_minimum_clearing_time_in_s() const
{
    return minimum_clearing_time;
}

double CONTINGENCY_SCREENER::get_maximum_clearing_time_in_s() const
{
    return maximum_clearing_time;
}

bool CONTINGENCY_SCREENER::get_flag_trip_line_after_clearing_fault() const
{
    return flag_trip_line_after_clearing_fault;
}

double CONTINGENCY_SCREENER::get_simulation_time_span_in_s() const
{
    return simulation_time_span_in_s;
}

double CONTINGENCY_SCREENER::get_angle_difference_threshold_in_deg() const
{
    return angle_difference_threshold_in_deg;
}

size_t CONTINGENCY_SCREENER::get_simulator_max_iteration() const
{
    return max_iteration;
}

double CONTINGENCY_SCREENER::get_simulator_allowed_max_power_imbalance_in_MVA() const
{
    return P_threshold_in_MW;
}

double CONTINGENCY_SCREENER::get_simulator_iteration_accelerator() const
{
    return alpha;
}

void CONTINGENCY_SCREENER::append_generator_to_monitor(const DEVICE_ID& did)
{
    if(did.is_valid() and did.get_device_type()=="GENERATOR")
        monitored_generators.push_back(did);
}

double CONTINGENCY_SCREENER::search_cct()
{
    STEPS& toolkit = get_default_toolkit();
    ostringstream osstream;
    if(not is_searcher_is_properly_set())
    {
        osstream<<"CCT searcher is not properly set. No CCT will be searched."<<endl
          <<"0.0 will be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0.0;
    }

    double t_clearing_min = get_minimum_clearing_time_in_s();
    double t_clearing_max = get_maximum_clearing_time_in_s();

    double clearing_time_min, clearing_time_max;

    clearing_time_min = t_clearing_min;
    bool is_clearing_time_min_stable = perform_simulation_with_clearing_time(clearing_time_min);

    clearing_time_max = t_clearing_max;
    bool is_clearing_time_max_stable = perform_simulation_with_clearing_time(clearing_time_max);

    while(true)
    {
        if(is_clearing_time_min_stable==true and is_clearing_time_max_stable==true)
        {
            osstream<<"System is stable at clearing time "<<clearing_time_min<<" s and "
              <<clearing_time_max<<" s"<<endl;
            double delt = clearing_time_max-clearing_time_min;
            clearing_time_min  = clearing_time_max;
            is_clearing_time_min_stable = true;

            clearing_time_max += delt;
            osstream<<"Now go on checking clearing time "<<clearing_time_max<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            is_clearing_time_max_stable = perform_simulation_with_clearing_time(clearing_time_max);
        }
        else
        {
            if(is_clearing_time_min_stable==true and is_clearing_time_max_stable==false)
            {
                osstream<<"System is stable at clearing time "<<clearing_time_min<<" s, but unstable at clearing time "
                  <<clearing_time_max<<" s"<<endl;
                double delt = clearing_time_max-clearing_time_min;
                clearing_time_min  = clearing_time_min;
                is_clearing_time_min_stable = true;

                clearing_time_max = clearing_time_min+0.5*delt;
                osstream<<"Now go on checking clearing time "<<clearing_time_max<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                is_clearing_time_max_stable = perform_simulation_with_clearing_time(clearing_time_max);
            }
            else
            {
                if(is_clearing_time_min_stable==false and is_clearing_time_max_stable==false)
                {
                    osstream<<"System is unstable at clearing time "<<clearing_time_min<<" s and "
                      <<clearing_time_max<<" s"<<endl;

                    clearing_time_max  = clearing_time_min;
                    is_clearing_time_max_stable = false;

                    clearing_time_min *=0.5;
                    osstream<<"Now go on checking clearing time "<<clearing_time_min<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    is_clearing_time_min_stable = perform_simulation_with_clearing_time(clearing_time_min);
                }
                else
                {
                    // invalid condition
                    // is_clearing_time_min_stable==false and is_clearing_time_max_stable==true
                    osstream<<"Warning. System is unstable when fault clearing time is "<<clearing_time_min
                      <<", but stable when fault clearing time is "<<clearing_time_max<<endl
                      <<"This is impossible for power system operation."<<endl
                      <<"If you observe this message, CONGRATULATIONS, you find something interesting. Go on working on it.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    break;
                }
            }
        }
        if(fabs(clearing_time_min-clearing_time_max)<0.001)
            break;
        if(clearing_time_min>1.5)
            break;
    }

    if(clearing_time_min<1.5)
        return clearing_time_min;
    else
        return 0.0;
}

bool CONTINGENCY_SCREENER::is_searcher_is_properly_set() const
{
    ostringstream osstream;
    STEPS& toolkit = get_default_toolkit();

    bool is_properly_set = true;
    osstream<<"CCT searcher is not properly set due to:"<<endl;
    if(get_powerflow_data_filename()=="")
    {
        is_properly_set = false;
        osstream<<"Powerflow data file is not set."<<endl;
    }
    if(get_dynamic_data_filename()=="")
    {
        is_properly_set = false;
        osstream<<"Dynamic data file is not set."<<endl;
    }
    if(not get_fault_device().is_valid())
    {
        is_properly_set = false;
        osstream<<"Fault device is not set."<<endl;
    }
    if(get_fault_side_bus()==0)
    {
        is_properly_set = false;
        osstream<<"Fault side bus is not set."<<endl;
    }
    if(steps_fast_complex_abs(get_fault_shunt_in_pu())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        osstream<<"Fault shunt is not set."<<endl;
    }
    if(fabs(get_minimum_clearing_time_in_s()-get_maximum_clearing_time_in_s())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        osstream<<"Minimum clearing time and maximum clearing time are identical."<<endl;
    }
    if(fabs(get_simulation_time_span_in_s())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        osstream<<"Simulation time span is not set."<<endl;
    }
    if(fabs(get_angle_difference_threshold_in_deg())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        osstream<<"Angle difference threshold is not set."<<endl;
    }
    if(get_monitored_generator_count()<2)
    {
        is_properly_set = false;
        osstream<<"Less than 2 generators are monitored."<<endl;
    }
    if(not is_properly_set)
        toolkit.show_information_with_leading_time_stamp(osstream);
    return is_properly_set;
}

bool CONTINGENCY_SCREENER::perform_simulation_with_clearing_time(double clearing_time)
{
    STEPS& toolkit = get_default_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    psdb.clear();

    PSSE_IMEXPORTER importer;

    importer.load_powerflow_data(get_powerflow_data_filename());
    importer.load_dynamic_data(get_dynamic_data_filename());

    POWERFLOW_SOLVER solver;
    solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    solver.set_flat_start_logic(false);
    solver.solve_with_full_Newton_Raphson_solution();

    DYNAMICS_SIMULATOR simulator;
    simulator.set_allowed_max_power_imbalance_in_MVA(get_simulator_allowed_max_power_imbalance_in_MVA());
    simulator.set_max_network_iteration(get_simulator_max_iteration());
    simulator.set_iteration_accelerator(get_simulator_iteration_accelerator());

    set_meters(simulator);

    string filename = "cs_cleared_after_"+num2str(clearing_time)+"s";
    simulator.set_output_file(filename);

    simulator.start();
    double fault_time = get_fault_time_in_s();
    simulator.run_to(fault_time);

    apply_fault(simulator);
    simulator.run_to(fault_time+clearing_time);

    clear_fault(simulator);

    double tend = get_simulation_time_span_in_s();
    //double delt = toolkit.get_dynamic_simulation_time_step_in_s();

    simulator.run_to(tend-1.0);

    bool is_stable = true;
    double TIME = toolkit.get_dynamic_simulation_time_in_s();
    while(TIME<tend)
    {
        simulator.run_a_step();
        TIME = toolkit.get_dynamic_simulation_time_in_s();
        is_stable = check_if_system_is_stable(simulator);
        if(is_stable)
            continue;
        else
            break;
    }

    return is_stable;
}

size_t CONTINGENCY_SCREENER::get_monitored_generator_count() const
{
    return monitored_generators.size();
}

DEVICE_ID CONTINGENCY_SCREENER::get_monitored_generator(size_t i) const
{
    if(i<get_monitored_generator_count())
        return monitored_generators[i];
    else
    {
        DEVICE_ID did;
        return did;
    }
}

void CONTINGENCY_SCREENER::set_meters(DYNAMICS_SIMULATOR& simulator)
{
    STEPS& toolkit = simulator.get_toolkit(__PRETTY_FUNCTION__);

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    simulator.clear_meters();

    size_t n = get_monitored_generator_count();
    for(size_t i=0; i!=n; ++i)
    {
        DEVICE_ID did = get_monitored_generator(i);
        if(did.is_valid() and did.get_device_type()=="GENERATOR")
        {
            METER meter = setter.prepare_generator_rotor_angle_in_deg_meter(did);
            simulator.append_meter(meter);
        }
    }
}

void CONTINGENCY_SCREENER::apply_fault(DYNAMICS_SIMULATOR& simulator)
{
    DEVICE_ID did = get_fault_device();
    size_t bus = get_fault_side_bus();
    double location = get_fault_location_to_fault_side_bus_in_pu();
    complex<double> shunt = get_fault_shunt_in_pu();
    simulator.set_line_fault(did, bus, location, shunt);
}

void CONTINGENCY_SCREENER::clear_fault(DYNAMICS_SIMULATOR& simulator)
{
    DEVICE_ID did = get_fault_device();
    size_t bus = get_fault_side_bus();
    double location = get_fault_location_to_fault_side_bus_in_pu();
    simulator.clear_line_fault(did, bus, location);

    double flag_trip = get_flag_trip_line_after_clearing_fault();
    if(flag_trip)
        simulator.trip_line(did);
}

bool CONTINGENCY_SCREENER::check_if_system_is_stable(DYNAMICS_SIMULATOR& simulator) const
{
    vector<double> angles = simulator.get_all_meters_value();
    double angle_max=0.0, angle_min = 0.0;
    size_t n = angles.size();
    if(n>0)
    {
        angle_max = angles[0];
        angle_min = angles[0];
        for(size_t i=1; i!=n; ++i)
        {
            if(angles[i]>angle_max)
                angle_max = angles[i];
            if(angles[i]<angle_min)
                angle_min = angles[i];
        }
    }
    double angle_difference = angle_max - angle_min;
    while(angle_difference>360.0)
        angle_difference -= 360.0;
    /*while(angle_difference>180.0)
        angle_difference -= 360.0;
    while(angle_difference<-180.0)
        angle_difference += 360.0;*/
    if(fabs(angle_difference)>get_angle_difference_threshold_in_deg())
        return false;
    else
        return true;
}



bool CONTINGENCY_SCREENER::is_valid() const
{
    return true; // disabled
}
void CONTINGENCY_SCREENER::check()
{
    ;// disabled
}
void CONTINGENCY_SCREENER::clear()
{
    ;//disabled
}
