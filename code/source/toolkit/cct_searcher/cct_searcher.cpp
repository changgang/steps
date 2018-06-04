#include "header/toolkit/cct_searcher/cct_searcher.h"
#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <iostream>
#include <ctime>

using namespace std;

CCT_SEARCHER::CCT_SEARCHER()
{
    set_power_system_database_maximum_bus_number(10000);
    psdb.set_system_base_power_in_MVA(100.0);
    psdb.set_system_base_frequency_in_Hz(50.0);

    set_dynamic_simulation_time_step_in_s(0.01);

    set_search_title("CCT_SEARCH");

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

    set_simulator_max_iteration(200);
    set_simulator_allowed_max_power_imbalance_in_MVA(0.0001);
    set_simulator_iteration_accelerator(1.0);
}

CCT_SEARCHER::~CCT_SEARCHER()
{
    ;
}

void CCT_SEARCHER::set_power_system_database_maximum_bus_number(size_t number)
{
    psdb.set_allowed_max_bus_number(number);
}

void CCT_SEARCHER::set_search_title(string title)
{
    search_title = title;
}

void CCT_SEARCHER::set_powerflow_data_filename(string filename)
{
    powerflow_data_filename = filename;
}

void CCT_SEARCHER::set_dynamic_data_filename(string filename)
{
    dynamic_data_filename = filename;
}

void CCT_SEARCHER::set_fault_device(DEVICE_ID did)
{
    if(did.is_valid() and did.get_device_type()=="LINE")
        fault_device = did;
}

void CCT_SEARCHER::set_fault_side_bus(size_t bus)
{
    fault_side_bus = bus;
}

void CCT_SEARCHER::set_fault_location_to_fault_side_bus_in_pu(double location)
{
    fault_location = location;
}

void CCT_SEARCHER::set_fault_shunt_in_pu(complex<double> shunt)
{
    if(abs(shunt)>FLOAT_EPSILON)
        fault_shunt_in_pu = shunt;
}

void CCT_SEARCHER::set_fault_time_in_s(double time)
{
    fault_time_in_s = time;
}

void CCT_SEARCHER::set_minimum_clearing_time_in_s(double time)
{
    minimum_clearing_time = time;
}

void CCT_SEARCHER::set_maximum_clearing_time_in_s(double time)
{
    maximum_clearing_time = time;
}

void CCT_SEARCHER::set_flag_trip_line_after_clearing_fault(bool flag)
{
    flag_trip_line_after_clearing_fault = flag;
}

void CCT_SEARCHER::set_simulation_time_span_in_s(double time)
{
    simulation_time_span_in_s = time;
}

void CCT_SEARCHER::set_angle_difference_threshold_in_deg(double angle)
{
    angle_difference_threshold_in_deg = angle;
}

void CCT_SEARCHER::set_simulator_max_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_iteration = iteration;
}

void CCT_SEARCHER::set_simulator_allowed_max_power_imbalance_in_MVA(double tol)
{
    if(tol>0.0)
    {
        P_threshold_in_MW = tol;
        Q_threshold_in_MVar = tol;
    }
}

void CCT_SEARCHER::set_simulator_iteration_accelerator(double iter_alpha)
{
    if(iter_alpha>0.0)
        this->alpha = iter_alpha;
}

size_t CCT_SEARCHER::get_power_system_database_maximum_bus_number() const
{
    return psdb.get_allowed_max_bus_number();
}

string CCT_SEARCHER::get_search_title() const
{
    return search_title;
}

string CCT_SEARCHER::get_powerflow_data_filename() const
{
    return powerflow_data_filename;
}

string CCT_SEARCHER::get_dynamic_data_filename() const
{
    return dynamic_data_filename;
}


DEVICE_ID CCT_SEARCHER::get_fault_device() const
{
    return fault_device;
}

size_t CCT_SEARCHER::get_fault_side_bus() const
{
    return fault_side_bus;
}

double CCT_SEARCHER::get_fault_location_to_fault_side_bus_in_pu() const
{
    return fault_location;
}

complex<double> CCT_SEARCHER::get_fault_shunt_in_pu() const
{
    return fault_shunt_in_pu;
}

double CCT_SEARCHER::get_fault_time_in_s() const
{
    return fault_time_in_s;
}

double CCT_SEARCHER::get_minimum_clearing_time_in_s() const
{
    return minimum_clearing_time;
}

double CCT_SEARCHER::get_maximum_clearing_time_in_s() const
{
    return maximum_clearing_time;
}

bool CCT_SEARCHER::get_flag_trip_line_after_clearing_fault() const
{
    return flag_trip_line_after_clearing_fault;
}

double CCT_SEARCHER::get_simulation_time_span_in_s() const
{
    return simulation_time_span_in_s;
}

double CCT_SEARCHER::get_angle_difference_threshold_in_deg() const
{
    return angle_difference_threshold_in_deg;
}

size_t CCT_SEARCHER::get_simulator_max_iteration() const
{
    return max_iteration;
}

double CCT_SEARCHER::get_simulator_allowed_max_power_imbalance_in_MVA() const
{
    return P_threshold_in_MW;
}

double CCT_SEARCHER::get_simulator_iteration_accelerator() const
{
    return alpha;
}

double CCT_SEARCHER::search_cct()
{
    ostringstream sstream;
    if(not is_searcher_is_properly_set())
    {
        sstream<<"CCT searcher is not properly set. No CCT will be searched."<<endl
          <<"0.0 will be returned.";
        show_information_with_leading_time_stamp(sstream);
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
            sstream<<"System is stable at clearing time "<<clearing_time_min<<" s and "
              <<clearing_time_max<<" s"<<endl;
            double delt = clearing_time_max-clearing_time_min;
            clearing_time_min  = clearing_time_max;
            is_clearing_time_min_stable = true;

            clearing_time_max += delt;
            sstream<<"Now go on checking clearing time "<<clearing_time_max<<" s.";
            show_information_with_leading_time_stamp(sstream);

            is_clearing_time_max_stable = perform_simulation_with_clearing_time(clearing_time_max);
        }
        else
        {
            if(is_clearing_time_min_stable==true and is_clearing_time_max_stable==false)
            {
                sstream<<"System is stable at clearing time "<<clearing_time_min<<" s, but unstable at clearing time "
                  <<clearing_time_max<<" s"<<endl;
                double delt = clearing_time_max-clearing_time_min;
                clearing_time_min  = clearing_time_min;
                is_clearing_time_min_stable = true;

                clearing_time_max = clearing_time_min+0.5*delt;
                sstream<<"Now go on checking clearing time "<<clearing_time_max<<" s.";
                show_information_with_leading_time_stamp(sstream);

                is_clearing_time_max_stable = perform_simulation_with_clearing_time(clearing_time_max);
            }
            else
            {
                if(is_clearing_time_min_stable==false and is_clearing_time_max_stable==false)
                {
                    sstream<<"System is unstable at clearing time "<<clearing_time_min<<" s and "
                      <<clearing_time_max<<" s"<<endl;

                    clearing_time_max  = clearing_time_min;
                    is_clearing_time_max_stable = false;

                    clearing_time_min *=0.5;
                    sstream<<"Now go on checking clearing time "<<clearing_time_min<<" s.";
                    show_information_with_leading_time_stamp(sstream);

                    is_clearing_time_min_stable = perform_simulation_with_clearing_time(clearing_time_min);
                }
                else
                {
                    // invalid condition
                    // is_clearing_time_min_stable==false and is_clearing_time_max_stable==true
                    sstream<<"Warning. System is unstable when fault clearing time is "<<clearing_time_min
                      <<", but stable when fault clearing time is "<<clearing_time_max<<endl
                      <<"This is impossible for power system operation."<<endl
                      <<"If you observe this message, CONGRATULATIONS, you find something interesting. Go on working on it.";
                    show_information_with_leading_time_stamp(sstream);

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

bool CCT_SEARCHER::is_searcher_is_properly_set() const
{
    ostringstream sstream;

    bool is_properly_set = true;
    sstream<<"CCT searcher is not properly set due to:"<<endl;
    if(get_powerflow_data_filename()=="")
    {
        is_properly_set = false;
        sstream<<"Powerflow data file is not set."<<endl;
    }
    if(get_dynamic_data_filename()=="")
    {
        is_properly_set = false;
        sstream<<"Dynamic data file is not set."<<endl;
    }
    if(not get_fault_device().is_valid())
    {
        is_properly_set = false;
        sstream<<"Fault device is not set."<<endl;
    }
    if(get_fault_side_bus()==0)
    {
        is_properly_set = false;
        sstream<<"Fault side bus is not set."<<endl;
    }
    if(abs(get_fault_shunt_in_pu())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        sstream<<"Fault shunt is not set."<<endl;
    }
    if(fabs(get_minimum_clearing_time_in_s()-get_maximum_clearing_time_in_s())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        sstream<<"Minimum clearing time and maximum clearing time are identical."<<endl;
    }
    if(fabs(get_simulation_time_span_in_s())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        sstream<<"Simulation time span is not set."<<endl;
    }
    if(fabs(get_angle_difference_threshold_in_deg())<FLOAT_EPSILON)
    {
        is_properly_set = false;
        sstream<<"Angle difference threshold is not set."<<endl;
    }

    if(not is_properly_set)
        show_information_with_leading_time_stamp(sstream);
    return is_properly_set;
}


bool CCT_SEARCHER::perform_simulation_with_clearing_time(double clearing_time)
{
    psdb.clear_database();

    PSSE_IMEXPORTER importer;
    importer.set_power_system_database(&psdb);

    importer.load_powerflow_data(get_powerflow_data_filename());
    importer.load_dynamic_data(get_dynamic_data_filename());

    POWERFLOW_SOLVER solver;
    solver.set_power_system_database(&psdb);
    solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    solver.set_flat_start_logic(false);
    solver.solve_with_full_Newton_Raphson_solution();

    DYNAMICS_SIMULATOR simulator(&psdb);
    simulator.set_allowed_max_power_imbalance_in_MVA(get_simulator_allowed_max_power_imbalance_in_MVA());
    simulator.set_max_DAE_iteration(10);
    simulator.set_max_network_iteration(get_simulator_max_iteration());
    simulator.set_iteration_accelerator(get_simulator_iteration_accelerator());

    prepare_generators_in_islands(simulator);

    simulator.prepare_meters();

    string filename = get_search_title();
    if(filename != "")
    {
        filename += "_fault_at_bus_"+num2str(get_fault_side_bus())+"_of_"+get_fault_device().get_device_name()+"_cleared_after_"+num2str(clearing_time)+"s";
        if(get_flag_trip_line_after_clearing_fault())
            filename += "_with_tripping_line";
        else
            filename += "_without_tripping_line";
    }
    simulator.set_output_file(filename);

    simulator.start();
    double fault_time = get_fault_time_in_s();
    simulator.run_to(fault_time);

    apply_fault(simulator);
    simulator.run_to(fault_time+clearing_time);

    clear_fault(simulator);

    double tend = get_simulation_time_span_in_s();
    double delt = get_dynamic_simulation_time_step_in_s();

    simulator.run_to(tend-1.0);

    bool is_stable = true;
    double TIME = get_dynamic_simulation_time_in_s();
    while(TIME<tend)
    {
        simulator.run_to(TIME+delt);
        cout<<__FUNCTION__<<", line "<<__LINE__<<endl;
        is_stable = check_if_system_is_stable();
        cout<<__FUNCTION__<<", line "<<__LINE__<<endl;
        if(not is_stable)
            break;
        TIME = get_dynamic_simulation_time_in_s();
    }

    simulator.clear();
    psdb.clear_database();

    return is_stable;
}


void CCT_SEARCHER::prepare_generators_in_islands(DYNAMICS_SIMULATOR& simulator)
{
    generators_in_islands.clear();

    NETWORK_DATABASE* network_db = simulator.get_network_database();

    POWER_SYSTEM_DATABASE* psdb = simulator.get_power_system_database();

    vector< vector<size_t> > islands = network_db->get_islands_with_physical_bus_number();
    size_t nislands = islands.size();
    for(size_t i=0; i!=nislands; ++i)
    {
        vector<size_t> island = islands[i];

        vector<GENERATOR*> generators_in_island;
        vector<GENERATOR*> generators_at_bus;

        size_t nisland = island.size();
        for(size_t j=0; j!=nisland; ++j)
        {
            size_t bus = island[j];
            generators_at_bus = psdb->get_generators_connecting_to_bus(bus);
            size_t n = generators_at_bus.size();
            for(size_t k=0; k!=n; ++k)
            {
                GENERATOR* generator = generators_at_bus[k];
                if(generator->get_status()==true)
                    generators_in_island.push_back(generator);
            }
        }

        generators_in_islands.push_back(generators_in_island);
    }
}

void CCT_SEARCHER::apply_fault(DYNAMICS_SIMULATOR& simulator)
{
    DEVICE_ID did = get_fault_device();
    size_t bus = get_fault_side_bus();
    double location = get_fault_location_to_fault_side_bus_in_pu();
    complex<double> shunt = get_fault_shunt_in_pu();
    simulator.set_line_fault(did, bus, location, shunt);
}

void CCT_SEARCHER::clear_fault(DYNAMICS_SIMULATOR& simulator)
{
    DEVICE_ID did = get_fault_device();
    size_t bus = get_fault_side_bus();
    double location = get_fault_location_to_fault_side_bus_in_pu();
    simulator.clear_line_fault(did, bus, location);

    double flag_trip = get_flag_trip_line_after_clearing_fault();
    if(flag_trip)
        simulator.trip_line(did);
}

//bool CCT_SEARCHER::check_if_system_is_stable(DYNAMICS_SIMULATOR& simulator) const
bool CCT_SEARCHER::check_if_system_is_stable() const
{
    cout<<__FUNCTION__<<", line "<<__LINE__<<endl;
    ostringstream sstream;
    double TIME = get_dynamic_simulation_time_in_s();
    bool system_is_stable = true;
    size_t n = generators_in_islands.size();
    cout<<__FUNCTION__<<", line "<<__LINE__<<endl;
    for(size_t island=0; island!=n; island++)
    {
        vector<GENERATOR*> generators_in_island = generators_in_islands[island];
        vector<double> angles;
        size_t n = generators_in_island.size();
        for(size_t i=0; i!=n; ++i)
        {
            GENERATOR* generator = generators_in_island[i];
            if(generator->get_status()==true)
            {
                SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
                angles.push_back(genmodel->get_rotor_angle_in_deg());
            }
        }
        cout<<__FUNCTION__<<", line "<<__LINE__<<endl;
        double angle_max=0.0, angle_min = 0.0;
        size_t nangle = angles.size();
        if(nangle>0)
        {
            angle_max = angles[0];
            angle_min = angles[0];
            for(size_t i=1; i!=nangle; ++i)
            {
                if(angles[i]>angle_max)
                    angle_max = angles[i];
                if(angles[i]<angle_min)
                    angle_min = angles[i];
            }
        }
        double angle_difference = angle_max - angle_min;
        double scaled_angle_difference = angle_difference;
        while(scaled_angle_difference>180.0)
            scaled_angle_difference -= 360.0;
        while(scaled_angle_difference<-180.0)
            scaled_angle_difference += 360.0;
        if(fabs(scaled_angle_difference)>get_angle_difference_threshold_in_deg())
        {
            system_is_stable = false;
            sstream<<"The following island is detected to be unstable at time "<<TIME<<" s. Maximum angle difference is :"<<fabs(angle_difference)<<" deg (a.k.a. "<<fabs(scaled_angle_difference)<<" deg)"<<endl
                   <<"Generator          Rotor angle in deg"<<endl;

            size_t n = generators_in_island.size();
            for(size_t i=0; i!=n; ++i)
            {
                GENERATOR* generator = generators_in_island[i];
                SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
                sstream<<generator->get_device_name()<<"  "<<genmodel->get_rotor_angle_in_deg()<<endl;
            }
            show_information_with_leading_time_stamp(sstream);
            break;
        }
    }
    return system_is_stable;

    /*
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
    while(angle_difference>180.0)
        angle_difference -= 360.0;
    while(angle_difference<-180.0)
        angle_difference += 360.0;
    if(fabs(angle_difference)>get_angle_difference_threshold_in_deg())
        return false;
    else
        return true;
    */
}

void CCT_SEARCHER::run_case_with_clearing_time(double time)
{
    string temp_search_title = get_search_title();
    set_search_title("CCT_Searched");

    double temp_angle_difference = get_angle_difference_threshold_in_deg();
    set_angle_difference_threshold_in_deg(360.0);

    perform_simulation_with_clearing_time(time);

    set_search_title(temp_search_title);
    set_angle_difference_threshold_in_deg(temp_angle_difference);
}


bool CCT_SEARCHER::is_valid() const
{
    return true; // disabled
}
void CCT_SEARCHER::check()
{
    ;// disabled
}
void CCT_SEARCHER::clear()
{
    ;//disabled
}
