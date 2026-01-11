#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

class STEPS;

#define ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR

#define USE_DYNAMIC_CURRENT_MISMATCH_CONTROL

DYNAMICS_SIMULATOR::DYNAMICS_SIMULATOR(STEPS& toolkit)
{
    this->toolkit = (&toolkit);

    DELT = 0.01;
    clear();
    detailed_log_enabled = false;
}

DYNAMICS_SIMULATOR::~DYNAMICS_SIMULATOR()
{
    stop();
    toolkit = NULL;

    clear();
}

STEPS& DYNAMICS_SIMULATOR::get_toolkit() const
{
    return *toolkit;
}

void DYNAMICS_SIMULATOR::clear()
{
    clear_meters();

    set_output_file("");

    close_meter_output_files();

    set_bin_file_export_enable_flag(false);
    set_csv_file_export_enable_flag(true);
    set_json_file_export_enable_flag(false);

    set_dynamic_simulation_time_in_s(0.0);

    set_max_DAE_iteration(100);
    set_min_DAE_iteration(3);
    set_max_network_iteration(50);
    set_max_update_iteration(3);
    set_max_event_update_iteration(10);
    set_max_network_solution_divergent_threshold(3);
    set_allowed_max_power_imbalance_in_MVA(0.001);
    set_voltage_change_threshold_when_solving_network_in_pu(INFINITE_THRESHOLD);
    set_iteration_accelerator(1.0);
    set_non_divergent_solution_logic(false);
    set_automatic_iteration_accelerator_tune_logic(false);
    set_rotor_angle_stability_surveillance_flag(false);
    set_rotor_angle_stability_threshold_in_deg(360.0);
    generators_in_islands.clear();
    flag_rotor_angle_stable = true;
    set_time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled_in_s(0.1);
    set_k_for_leading_part_of_bus_frequency_model(3.0);

    set_change_time_step_adaptively_logic(false);
    buses4change_detection.clear();
    generators4change_detection.clear();
    set_system_change_detection_time_range_in_s(1.0);
    set_system_change_detection_bus_voltage_threshold_in_pu(0.01);
    set_system_change_detection_rotor_speed_threshold_in_pu(0.001);

    unsigned int n = buffer4buses_voltage_change_detection.size();
    if(n>0)
    {
        for(unsigned int i=0; i<n; ++i)
            delete buffer4buses_voltage_change_detection[i];
    }
    n = buffer4generators_rotor_speed_change_detection.size();
    if(n>0)
    {
        for(unsigned int i=0; i<n; ++i)
            delete buffer4generators_rotor_speed_change_detection[i];
    }
}

void DYNAMICS_SIMULATOR::set_max_dynamic_simulation_time_step_in_s(double delt)
{
    DELT_MAX = delt;
}

void DYNAMICS_SIMULATOR::set_min_dynamic_simulation_time_step_in_s(double delt)
{
    DELT_MIN = delt;
}

void DYNAMICS_SIMULATOR::set_dynamic_simulation_time_step_in_s(double delt)
{
    STEPS& toolkit = get_toolkit();
    DELT = delt;
    ostringstream osstream;
    osstream<<"System dynamic simulation time step is set as :"<<DELT<<" s.";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

NETWORK_MATRIX& DYNAMICS_SIMULATOR::get_network_matrix()
{
    STEPS& toolkit = get_toolkit();
    return toolkit.get_network_matrix();
}

void DYNAMICS_SIMULATOR::set_csv_file_export_enable_flag(bool flag)
{
    csv_file_export_enabled = flag;
}

void DYNAMICS_SIMULATOR::set_json_file_export_enable_flag(bool flag)
{
    json_file_export_enabled = flag;
}

void DYNAMICS_SIMULATOR::set_bin_file_export_enable_flag(bool flag)
{
    bin_file_export_enabled = flag;
}

bool DYNAMICS_SIMULATOR::is_csv_file_export_enabled() const
{
    return csv_file_export_enabled;
}

bool DYNAMICS_SIMULATOR::is_json_file_export_enabled() const
{
    return json_file_export_enabled;
}

bool DYNAMICS_SIMULATOR::is_bin_file_export_enabled() const
{
    return bin_file_export_enabled;
}

void DYNAMICS_SIMULATOR::set_max_DAE_iteration(unsigned int iteration)
{
    if(iteration>0)
        max_DAE_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_min_DAE_iteration(unsigned int iteration)
{
    if(iteration>0)
    {
        if(iteration<2) iteration = 2;

        min_DAE_iteration = iteration;
    }
}

void DYNAMICS_SIMULATOR::set_max_network_iteration(unsigned int iteration)
{
    if(iteration>0)
       max_network_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_max_update_iteration(unsigned int iteration)
{
    if(iteration>0)
       max_update_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_max_event_update_iteration(unsigned int iteration)
{
    if(iteration>0)
        max_event_update_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_max_network_solution_divergent_threshold(unsigned int div_th)
{
    if(div_th>0)
       max_network_solution_divergent_threshold = div_th;
}

void DYNAMICS_SIMULATOR::set_allowed_max_power_imbalance_in_MVA(double tol)
{
    if(tol>0.0)
    {
        P_threshold_in_MW = tol;
        Q_threshold_in_MVar = tol;
    }
}

void DYNAMICS_SIMULATOR::set_voltage_change_threshold_when_solving_network_in_pu(double v)
{
    v_change_threshold_pu = v;
}

void DYNAMICS_SIMULATOR::set_iteration_accelerator(double iter_alpha)
{
    if(iter_alpha>0.0)
       alpha = iter_alpha;
}

void DYNAMICS_SIMULATOR::set_non_divergent_solution_logic(bool logic)
{
    non_divergent_solution_enabled = logic;
}

void DYNAMICS_SIMULATOR::set_automatic_iteration_accelerator_tune_logic(bool logic)
{
    automatic_iteration_accelerator_tune_enabled = logic;
    if(logic==true)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"*******************************\n"
                <<"******************************\n"
                <<"******************************\n"
                <<"ALERT!\n"
                <<"Automatic iteration accelerator tune is enabled for dynamic simulation. Your simulation may malfunction.\n"
                <<"You are supposed to modify function DYNAMIC_SIMULATOR::tune_iteration_accelerator_based_on_maximum_current_mismatch() for you specific simulation.\n"
                <<"******************************\n"
                <<"******************************\n"
                <<"******************************";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::set_rotor_angle_stability_surveillance_flag(bool flag)
{
    flag_rotor_angle_stability_surveillance = flag;
}

void DYNAMICS_SIMULATOR::set_rotor_angle_stability_threshold_in_deg(double angle_th)
{
    if(angle_th<0.0) angle_th = - angle_th;
    rotor_angle_stability_threshold_in_deg = angle_th;
}

void DYNAMICS_SIMULATOR::set_time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled_in_s(double delt)
{
    time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled = delt;
}

void DYNAMICS_SIMULATOR::set_k_for_leading_part_of_bus_frequency_model(double k)
{
    k_for_leading_part_of_bus_frequency_model = k;
}

void DYNAMICS_SIMULATOR::set_change_time_step_adaptively_logic(bool logic)
{
    change_time_step_adaptively_flag = logic;
}

void DYNAMICS_SIMULATOR::set_system_change_detection_time_range_in_s(double t)
{
    system_change_detection_time_range = t;
}

void DYNAMICS_SIMULATOR::set_system_change_detection_bus_voltage_threshold_in_pu(double v)
{
    system_change_detection_bus_voltage_threshold = v;
}

void DYNAMICS_SIMULATOR::set_system_change_detection_rotor_speed_threshold_in_pu(double w)
{
    system_change_detection_rotor_speed_threshold = w;
}

/*void DYNAMICS_SIMULATOR::set_current_simulation_time_in_s(double time)
{
    TIME = time;
}

double DYNAMICS_SIMULATOR::get_current_simulation_time_in_s() const
{
    return TIME;
}*/

unsigned int DYNAMICS_SIMULATOR::get_max_DAE_iteration() const
{
    return max_DAE_iteration;
}

unsigned int DYNAMICS_SIMULATOR::get_min_DAE_iteration() const
{
    return min_DAE_iteration;
}

unsigned int DYNAMICS_SIMULATOR::get_max_network_iteration() const
{
    return max_network_iteration;
}

unsigned int DYNAMICS_SIMULATOR::get_max_update_iteration() const
{
    return max_update_iteration;
}

unsigned int DYNAMICS_SIMULATOR::get_max_event_update_iteration() const
{
    return max_event_update_iteration;
}

unsigned int DYNAMICS_SIMULATOR::get_max_network_solution_divergent_threshold() const
{
    return max_network_solution_divergent_threshold;
}

double DYNAMICS_SIMULATOR::get_allowed_max_power_imbalance_in_MVA() const
{
    return P_threshold_in_MW;
}

double DYNAMICS_SIMULATOR::get_voltage_change_threshold_when_solving_network_in_pu() const
{
    return v_change_threshold_pu;
}

double DYNAMICS_SIMULATOR::get_iteration_accelerator() const
{
    return alpha;
}

bool DYNAMICS_SIMULATOR::get_non_divergent_solution_logic() const
{
    return non_divergent_solution_enabled;
}

bool DYNAMICS_SIMULATOR::get_automatic_iteration_accelerator_tune_logic() const
{
    return automatic_iteration_accelerator_tune_enabled;
}

bool DYNAMICS_SIMULATOR::get_rotor_angle_stability_surveillance_flag() const
{
    return flag_rotor_angle_stability_surveillance;
}

double DYNAMICS_SIMULATOR::get_rotor_angle_stability_threshold_in_deg() const
{
    return rotor_angle_stability_threshold_in_deg;
}

double DYNAMICS_SIMULATOR::get_time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled_in_s() const
{
    return time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled;
}

double DYNAMICS_SIMULATOR::get_k_for_leading_part_of_bus_frequency_model() const
{
    return k_for_leading_part_of_bus_frequency_model;
}

bool DYNAMICS_SIMULATOR::get_change_time_step_adaptively_logic() const
{
    return change_time_step_adaptively_flag;
}

double DYNAMICS_SIMULATOR::get_system_change_detection_time_range_in_s() const
{
    return system_change_detection_time_range;
}

double DYNAMICS_SIMULATOR::get_system_change_detection_bus_voltage_threshold_in_pu() const
{
    return system_change_detection_bus_voltage_threshold;
}

double DYNAMICS_SIMULATOR::get_system_change_detection_rotor_speed_threshold_in_pu() const
{
    return system_change_detection_rotor_speed_threshold;
}

void DYNAMICS_SIMULATOR::add_buses_for_system_change_detection(vector<unsigned int> buses)
{
    unsigned int n = buses.size();
    for(unsigned int i =0; i<n; ++i)
        add_bus_for_system_change_detection(buses[i]);
}

void DYNAMICS_SIMULATOR::add_bus_for_system_change_detection(unsigned int bus_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(psdb.is_bus_exist(bus_number))
    {
        bool is_bus_monitored = false;
        unsigned int n = buses4change_detection.size();
        for(unsigned int i=0; i<n; ++i)
        {
            BUS& bus = *(buses4change_detection[i]);
            if(bus.get_bus_number()==bus_number)
            {
                is_bus_monitored = true;
                break;
            }
        }
        if(not is_bus_monitored)
            buses4change_detection.push_back(psdb.get_bus(bus_number));
    }
}

void DYNAMICS_SIMULATOR::add_generators_for_system_change_detection(vector<DEVICE_ID> dids)
{
    unsigned int n = dids.size();
    for(unsigned int i=0; i<n; ++i)
        add_generator_for_system_change_detection(dids[i]);
}

void DYNAMICS_SIMULATOR::add_generator_for_system_change_detection(DEVICE_ID did)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    if(did.get_device_type()==STEPS_GENERATOR and psdb.is_generator_exist(did))
    {
        bool is_gen_monitored = false;
        unsigned int n = generators4change_detection.size();
        for(unsigned int i=0; i<n; ++i)
        {
            GENERATOR& gen = *(generators4change_detection[i]);
            if(gen.get_device_id()==did)
            {
                is_gen_monitored = true;
                break;
            }
        }
        if(not is_gen_monitored)
            generators4change_detection.push_back(psdb.get_generator(did));
    }
}

void DYNAMICS_SIMULATOR::show_dynamic_simulator_configuration() const
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;
    osstream<<"Configuration of dynamic simulator:\n"
            <<"Fast sin/cos/tan function: "<<(use_steps_fast_math?"Enabled":"Disabled")<<"\n"
            <<"Time step: "<<get_dynamic_simulation_time_step_in_s()<<" s\n"
            <<"Allowed maximum power imbalance: "<<get_allowed_max_power_imbalance_in_MVA()<<" MVA\n"
            <<"Maximum iteration for DAE solution: "<<get_max_DAE_iteration()<<"\n"
            <<"Minimum iteration for DAE solution: "<<get_min_DAE_iteration()<<"\n"
            <<"Maximum iteration for network: "<<get_max_network_iteration()<<"\n"
            <<"Maximum iteration for updating: "<<get_max_update_iteration()<<"\n"
            <<"Maximum iteration for event updating: "<<get_max_event_update_iteration()<<"\n"
            <<"Maximum network solution divergent threshold: "<<get_max_network_solution_divergent_threshold()<<"\n"
            <<"Maximum network solution voltage change threshold: "<<get_voltage_change_threshold_when_solving_network_in_pu()<<" pu\n"
            <<"Network solution accelerator: "<<get_iteration_accelerator()<<"\n"
            <<"Rotor angle stability surveillance: "<<(get_rotor_angle_stability_surveillance_flag()?"Enabled":"Disabled")<<"\n"
            <<"Rotor angle stability threshold: "<<get_rotor_angle_stability_threshold_in_deg()<<" deg\n"
            <<"Time step threshold when leading part of bus frequency model is enabled: "<<get_time_step_threshold_when_leading_part_of_bus_frequency_model_is_enabled_in_s()<<" s\n"
            <<"K for leading part of bus frequency model: "<<get_k_for_leading_part_of_bus_frequency_model()<<"\n"
            <<"Automatically change dynamic simulation time step logic: "<<(get_change_time_step_adaptively_logic()?"Enabled":"Disabled")<<"\n"
            <<"Time range for checking system change: "<<get_system_change_detection_time_range_in_s()<<"s\n"
            <<"Bus voltage change threshold for checking system change: "<<get_system_change_detection_bus_voltage_threshold_in_pu()<<"pu\n"
            <<"Generator rotor speed change threshold for checking system change: "<<get_system_change_detection_rotor_speed_threshold_in_pu()<<"pu\n"
            <<"CSV export: "<<(is_csv_file_export_enabled()?"Enabled":"Disabled")<<"\n"
            <<"BIN export: "<<(is_bin_file_export_enabled()?"Enabled":"Disabled")<<"\n"
            <<"JSON export: "<<(is_json_file_export_enabled()?"Enabled":"Disabled")<<"\n"
            <<"Output file name: "<<get_output_file()<<"\n";

    unsigned int n = meters.size();
    osstream<<"Channel count: "<<n<<"\n";
    for(unsigned int i=0; i!=n; ++i)
        osstream<<"("<<setw(4)<<i+1<<") "<<meters[i].get_meter_name()<<"\n";

    toolkit.show_information_with_leading_time_stamp(osstream);

    if(get_change_time_step_adaptively_logic()==true)
    {
        osstream<<"Monitored devices for automatically change dynamic simulation time step:\n";
        n = buses4change_detection.size();
        osstream<<"Monitored bus count: "<<n<<"\n";
        for(unsigned int i=0; i!=n; ++i)
        {
            BUS& bus = *(buses4change_detection[i]);
            osstream<<"("<<setw(4)<<i+1<<") "<<bus.get_compound_device_name()<<" ["<<bus.get_bus_name()<<"]\n";
        }
        n = generators4change_detection.size();
        osstream<<"Monitored generator count: "<<n<<"\n";
        for(unsigned int i=0; i!=n; ++i)
        {
            GENERATOR& gen = *(generators4change_detection[i]);
            osstream<<"("<<setw(4)<<i+1<<") "<<gen.get_compound_device_name()<<" ["<<psdb.bus_number2bus_name(gen.get_generator_bus())<<"]\n";
        }
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

unsigned int DYNAMICS_SIMULATOR::get_memory_usage_in_bytes()
{
    return in_service_buses.capacity()*sizeof(BUS*)+
           generators.capacity()*sizeof(GENERATOR*)+
           wt_generators.capacity()*sizeof(WT_GENERATOR*)+
           pv_units.capacity()*sizeof(PV_UNIT*)+
           e_storages.capacity()*sizeof(ENERGY_STORAGE*)+
           sv_sources.capacity()*sizeof(SV_SOURCE*)+
           loads.capacity()*sizeof(LOAD*)+
           lines.capacity()*sizeof(AC_LINE*)+
           hvdcs.capacity()*sizeof(LCC_HVDC2T*)+
           vsc_hvdcs.capacity()*sizeof(VSC_HVDC*)+
           e_devices.capacity()*sizeof(EQUIVALENT_DEVICE*)+

           internal_bus_pointers.capacity()*sizeof(BUS*)+
           internal_bus_voltage_in_pu.capacity()*sizeof(double)+
           internal_bus_complex_voltage_in_pu.capacity()*sizeof(complex<double>)+
           I_mismatch.capacity()*sizeof(complex<double>)+
           S_mismatch.capacity()*sizeof(complex<double>)+
           I_vec.capacity()*sizeof(double)+
           delta_V.capacity()*sizeof(complex<double>)+

           jacobian.get_memory_usage_in_bytes()+

           meters.capacity()*sizeof(METER)+
           meter_values.capacity()*sizeof(double);
}

void DYNAMICS_SIMULATOR::set_network_matrix_update_as_unrequired()
{
    network_matrix_update_required = false;
}

void DYNAMICS_SIMULATOR::set_network_matrix_update_as_required()
{
    network_matrix_update_required = true;
}

bool DYNAMICS_SIMULATOR::is_network_matrix_update_required() const
{
    return network_matrix_update_required;
}

void DYNAMICS_SIMULATOR::install_meters(const vector<METER>& meters)
{
    unsigned int n = meters.size();
    for(unsigned int i = 0; i < n; ++i)
        install_meter(meters[i]);
}

void DYNAMICS_SIMULATOR::install_meter(const METER& meter)
{
    if(meter.is_valid())
    {
        meters.push_back(meter);
        //os<< "Meter added. %s.",(meter.get_meter_name()).c_str());
        //show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Warning. Invalid meter is given. No meter will be set in DYNAMIC_SIMULATOR::"<<__FUNCTION__<<"().";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

unsigned int DYNAMICS_SIMULATOR::get_meter_count() const
{
    return meters.size();
}

METER DYNAMICS_SIMULATOR::get_meter(unsigned int i)
{
    METER voidmeter(get_toolkit());

    if(meters.size()!=0)
    {
        if(i<meters.size())
            return meters[i];
        else
            return voidmeter;
    }
    else
        return voidmeter;
}

void DYNAMICS_SIMULATOR::update_all_meters_value()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = meters.size();
    if(n!=0)
    {
        if(meter_values.size()!=n)
            meter_values.resize(n,0.0);

        #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
            set_openmp_number_of_threads(toolkit.get_thread_number());
            #pragma omp parallel for schedule(static)
            //#pragma omp parallel for num_threads(2)
        #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        for(unsigned int i=0; i<n; ++i)
            meter_values[i]=meters[i].get_meter_value();
        /*
        if(meter_values.size()==n)
        {
            for(unsigned int i=0; i!=n; ++i)
                meter_values[i]=meters[i].get_meter_value();
        }
        else
            meter_values.resize(n,0.0);
        */
    }
}

vector<double> DYNAMICS_SIMULATOR::get_all_meters_value()
{
    update_all_meters_value();
    return meter_values;
}

double DYNAMICS_SIMULATOR::get_user_meter_value(unsigned int i)
{
    if(i>0 and i<=meters.size())
        return meter_values[i-1];
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_basic_meter_value(string meter_name)
{
    meter_name = string2upper(meter_name);

    if(meter_name=="DAE ITERATION")
        return ITER_DAE;
    if(meter_name=="NETWORK ITERATION")
        return ITER_NET;
    if(meter_name=="POWER MISMATCH IN MVA")
        return max_power_mismatch_MVA;
    if(meter_name=="BUS WITH MAX POWER MISMATCH")
        return max_mismatch_bus;
    if(meter_name=="ELAPSED TIME OF INTEGRATION IN MS")
        return time_elapse_of_differential_equations_in_a_step;
    if(meter_name=="ELAPSED TIME OF NETWORK SOLUTION IN MS")
        return time_elapse_of_network_solution_in_a_step;
    if(meter_name=="TOTAL ELAPSED TIME IN MS")
        return time_elapse_in_a_step;
    return 0.0;
}

void DYNAMICS_SIMULATOR::clear_meters()
{
    meters.clear();
    meter_values.clear();
}

void DYNAMICS_SIMULATOR::set_output_file(string filename)
{
    output_filename = filename;
}

string DYNAMICS_SIMULATOR::get_output_file() const
{
    return output_filename;
}

void DYNAMICS_SIMULATOR::open_meter_output_files()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(csv_output_file.is_open())
    {
        osstream<<"CSV meter output file stream was connected to some file. Stream will be closed before reopen for exporting dynamic simulation meter values.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(json_output_file.is_open())
    {
        osstream<<"JSON meter output file stream was connected to some file. Stream will be closed before reopen for exporting dynamic simulation meter values.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(bin_output_file.is_open())
    {
        osstream<<"BIN meter output file stream was connected to some file. Stream will be closed before reopen for exporting dynamic simulation meter values.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    close_meter_output_files();

    //osstream<<"output file name is : "<<output_filename<<endl;
    //toolkit.show_information_with_leading_time_stamp(osstream);
    if(output_filename!="")
    {
        if(is_bin_file_export_enabled())
        {
            string bin_filename = output_filename+".bin";
            bin_output_file.open(bin_filename, ios::binary);
            if(not bin_output_file.is_open())
            {
                osstream<<"BIN meter output file "<<bin_filename<<" cannot be opened for exporting dynamic simulation meter values.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }

        if(is_csv_file_export_enabled())
        {
            string csv_filename = output_filename+".csv";
            csv_output_file.open(csv_filename);
            if(not csv_output_file.is_open())
            {
                osstream<<"CSV meter output file "<<csv_filename<<" cannot be opened for exporting dynamic simulation meter values.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }

        if(is_json_file_export_enabled())
        {
            string json_filename = output_filename+".json";
            json_output_file.open(json_filename);
            if(not json_output_file.is_open())
            {
                osstream<<"JSON meter output file "<<json_filename<<" cannot be opened for exporting dynamic simulation meter values.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
    else
    {
        return;

        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);

        char time_stamp[60];
        snprintf(time_stamp, 60, "dynamic_result_%d-%02d-%02d~%02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        output_filename = string(time_stamp);

        osstream<<"Meter output filename is not properly set. Meters will be automatically exported to the following file:"<<endl
          <<output_filename;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_meter_output_files()
{
    ostringstream osstream;
    if(is_csv_file_export_enabled() and csv_output_file.is_open())
    {
        //osstream<<"CSV meter output file stream will be closed.";
        //show_information_with_leading_time_stamp(osstream);

        csv_output_file.close();

        //osstream<<"CSV meter output file stream is closed.";
        //show_information_with_leading_time_stamp(osstream);
    }
    if(is_json_file_export_enabled() and json_output_file.is_open())
    {
        //osstream<<"JSON meter output file stream will be closed.";
        //show_information_with_leading_time_stamp(osstream);

        json_output_file.close();

        //osstream<<"JSON meter output file stream is closed.";
        //show_information_with_leading_time_stamp(osstream);
    }

    if(is_bin_file_export_enabled() and bin_output_file.is_open())
    {
        //osstream<<"BIN meter output file stream will be closed.";
        //show_information_with_leading_time_stamp(osstream);

        bin_output_file.close();

        //osstream<<"BIN meter output file stream is closed.";
        //show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::save_meter_information()
{
    ostringstream osstream;

    unsigned int n = meters.size();
    if(n!=0)
    {
        open_meter_output_files();

        if(csv_output_file.is_open() or json_output_file.is_open() or bin_output_file.is_open())
        {
            // save header to csv file
            if(is_csv_file_export_enabled() and csv_output_file.is_open())
            {
                //csv_output_file<<"TIME,ITERATION,MISMATCH IN MVA";
                //for(unsigned int i=0; i!=n; ++i)
                //    csv_output_file<<","<<meters[i].get_meter_type();
                //csv_output_file<<endl;

                csv_output_file<<"TIME,DAE INTEGRATION,NETWORK ITERATION,MISMATCH IN MVA,MISMATCH BUS,D TIME ELAPSE IN MS,A TIME ELAPSE IN MS,TOTAL TIME ELAPSE IN MS";

                for(unsigned int i=0; i!=n; ++i)
                    csv_output_file<<","<<meters[i].get_meter_name();
                csv_output_file<<"\n";
            }

            // save header to json file
            if(is_json_file_export_enabled() and json_output_file.is_open())
            {
                json_output_file<<"{"<<"\n";

                //json_output_file<<"    \"meter_type\" : [\"TIME\", \"ITERATION\", \"MISMATCH IN MVA\"";
                //for(unsigned int i=0; i!=n; ++i)
                //    json_output_file<<", \""<<meters[i].get_meter_type()<<"\"";
                //json_output_file<<"],"<<endl<<endl;

                json_output_file<<"    \"meter_name\" : [\"TIME\", \"DAE INTEGRATION\", \"NETWORK ITERATION\", \"MISMATCH IN MVA\", \"MISMATCH BUS\", \"D TIME ELAPSE IN MS\", \"A TIME ELAPSE IN MS\", \"TOTAL TIME ELAPSE IN MS\"";
                for(unsigned int i=0; i!=n; ++i)
                    json_output_file<<", \""<<meters[i].get_meter_name()<<"\"";
                json_output_file<<"],"<<"\n\n";
                json_output_file<<"    \"meter_value\" : ["<<"\n";
            }

            // save header to bin file

            if(is_bin_file_export_enabled() and bin_output_file.is_open())
            {
                unsigned int bin_version=0;
                bin_output_file.write((char *)(&bin_version), sizeof(bin_version));

                time_t tt = time(NULL);
                tm* local_time= localtime(&tt);

                unsigned int year = local_time->tm_year + 1900;
                unsigned int month = local_time->tm_mon + 1;
                unsigned int day = local_time->tm_mday;
                unsigned int hour = local_time->tm_hour;
                unsigned int minute = local_time->tm_min;
                unsigned int second = local_time->tm_sec;

                bin_output_file.write((char *)(&year), sizeof(year));
                bin_output_file.write((char *)(&month), sizeof(month));
                bin_output_file.write((char *)(&day), sizeof(day));
                bin_output_file.write((char *)(&hour), sizeof(hour));
                bin_output_file.write((char *)(&minute), sizeof(minute));
                bin_output_file.write((char *)(&second), sizeof(second));

                unsigned int float_size=sizeof(float);
                bin_output_file.write((char *)(&float_size), sizeof(float_size));

                unsigned int m = 4+n;
                bin_output_file.write((char *)(&m), sizeof(m));
                string meter_names ="";

                meter_names += "TIME\n";
                meter_names += "DAE INTEGRATION\n";
                meter_names += "NETWORK INTEGRATION\n";
                meter_names += "MISMATCH IN MVA\n";
                meter_names += "MISMATCH BUS\n";
                meter_names += "D TIME ELAPSE IN MS\n";
                meter_names += "A TIME ELAPSE IN MS\n";
                meter_names += "TOTAL TIME ELAPSE IN MS\n";
                for(unsigned int i=0; i!=n; ++i)
                    meter_names += (meters[i].get_meter_name()+"\n");

                unsigned int n_meter_string_size = strlen(meter_names.c_str());
                bin_output_file.write((char *)(&n_meter_string_size), sizeof(n_meter_string_size));

                char * pmeter_name = new char[n_meter_string_size+1];
                strcpy(pmeter_name, meter_names.c_str());
                pmeter_name[n_meter_string_size]='\0';

                bin_output_file.write((char *)(pmeter_name), strlen(pmeter_name));
                delete [] pmeter_name;
            }
        }
    }
}

void DYNAMICS_SIMULATOR::save_meter_values()
{
    STEPS& toolkit = get_toolkit();
    #ifdef USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
    max_power_mismatch_MVA = max_current_mismatch_pu*toolkit.get_power_system_database().get_bus_positive_sequence_voltage_in_pu(max_mismatch_bus)*toolkit.get_system_base_power_in_MVA();
    #endif // USE_DYNAMIC_CURRENT_MISMATCH_CONTROL

    update_all_meters_value();
    unsigned int n = meters.size();
    if(n!=0 and (csv_output_file.is_open() or json_output_file.is_open() or bin_output_file.is_open()))
    {
        //get_bus_current_mismatch();
        //calculate_bus_power_mismatch_in_MVA();
        //GREATEST_POWER_CURRENT_MISMATCH_STRUCT s_mismatch = get_max_power_mismatch_struct();
        //double smax = s_mismatch.greatest_power_mismatch_in_MVA;
        //unsigned int smax_bus = s_mismatch.bus_with_greatest_power_mismatch;

        if(is_bin_file_export_enabled() and bin_output_file.is_open())
        {
            float fvalue=0.0;
            char* fdata = (char *)(&fvalue);
            int ivalue=0;
            char* idata = (char *)(&ivalue);

            fvalue = TIME;
            bin_output_file.write(fdata, sizeof(float));

            ivalue = ITER_DAE;
            bin_output_file.write(idata, sizeof(int));

            ivalue = ITER_NET;
            bin_output_file.write(idata, sizeof(int));

            fvalue = max_power_mismatch_MVA;
            bin_output_file.write(fdata, sizeof(float));

            ivalue = max_mismatch_bus;
            bin_output_file.write(idata, sizeof(int));

            fvalue = time_elapse_of_differential_equations_in_a_step;
            bin_output_file.write(fdata, sizeof(float));

            fvalue = time_elapse_of_network_solution_in_a_step;
            bin_output_file.write(fdata, sizeof(float));

            fvalue = time_elapse_in_a_step;
            bin_output_file.write(fdata, sizeof(float));

            for (unsigned int i = 0; i != n; ++i)
            {
                fvalue = meter_values[i];
                bin_output_file.write(fdata, sizeof(float));
            }
        }

        if(is_csv_file_export_enabled() or is_json_file_export_enabled())
        {
            char temp_buffer[50];
            string temp_str = "";
            snprintf(temp_buffer, 50, "%8.4f",TIME);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%3u",ITER_DAE);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%3u",ITER_NET);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",max_power_mismatch_MVA);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6u",max_mismatch_bus);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",time_elapse_of_differential_equations_in_a_step);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",time_elapse_of_network_solution_in_a_step);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",time_elapse_in_a_step);
            temp_str += temp_buffer;
            for (unsigned int i = 0; i != n; ++i)
            {
                snprintf(temp_buffer, 50, ",%16.12f", meter_values[i]);
                temp_str += temp_buffer;
            }

            if(is_csv_file_export_enabled() and csv_output_file.is_open())
                csv_output_file<<temp_str<<"\n";

            if(is_json_file_export_enabled() and json_output_file.is_open())
                json_output_file<<","<<"\n"
                                <<"                       ["<<temp_str<<"]";
        }
    }
}

void DYNAMICS_SIMULATOR::optimize_network_ordering()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    network_matrix.optimize_network_ordering();

    set_internal_bus_pointer();
}

void DYNAMICS_SIMULATOR::set_internal_bus_pointer()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int nbus = psdb.get_in_service_bus_count();

    NETWORK_MATRIX& network_matrix = get_network_matrix();
    internal_bus_pointers.clear();
    for(unsigned int internal_bus=0; internal_bus!=nbus; ++internal_bus)
    {
        unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        internal_bus_pointers.push_back(psdb.get_bus(physical_bus));
    }

    internal_bus_complex_voltage_in_pu.resize(nbus, 0.0);
    initialize_internal_bus_voltage_vector();
}

void DYNAMICS_SIMULATOR::initialize_internal_bus_voltage_vector()
{
    STEPS& toolkit = get_toolkit();
    unsigned int nbus = internal_bus_complex_voltage_in_pu.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int internal_bus=0; internal_bus<nbus; ++internal_bus)
    {
        BUS& bus = *(internal_bus_pointers[internal_bus]);
        internal_bus_complex_voltage_in_pu[internal_bus] = bus.get_positive_sequence_complex_voltage_in_pu();
    }
}

void DYNAMICS_SIMULATOR::build_vsc_hvdc_dynamic_dc_network_matrix()
{
    STEPS& toolkit = get_toolkit();
    unsigned int nvsc_hvdc = vsc_hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<nvsc_hvdc; ++i)
    {
        VSC_HVDC& vsc_hvdc = *(vsc_hvdcs[i]);
        VSC_HVDC_NETWORK_MODEL* model_ptr = vsc_hvdc.get_vsc_hvdc_network_model();
        VSC_HVDC_NETWORK_MODEL& model = *model_ptr;
        model.build_dynamic_dc_network_matrix();
    }
}

complex<double> DYNAMICS_SIMULATOR::get_bus_complex_voltage_in_pu_with_internal_bus_number(unsigned int internal_bus) const
{
    return internal_bus_complex_voltage_in_pu[internal_bus];

    /*
    BUS& bus = *(internal_bus_pointers[internal_bus]);
    complex<double> v= bus.get_positive_sequence_complex_voltage_in_pu();
    if(v!=internal_bus_complex_voltage_in_pu[internal_bus])
    {
        ostringstream osstream;
        osstream<<"voltage inconsistent of internal bus "<<internal_bus<<", v = "<<v<<", internal array = "<<internal_bus_complex_voltage_in_pu[internal_bus];
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    return v;
    */
}

void DYNAMICS_SIMULATOR::start()
{
    STEPS& toolkit = get_toolkit();
    microseconds_elapse_of_differential_equations_in_a_step = 0;
    microseconds_elapse_of_network_solution_in_a_step = 0;

    toolkit.update_device_thread_number();

    POWERFLOW_SOLVER& pf_solver = toolkit.get_powerflow_solver();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    detailed_log_enabled = toolkit.is_detailed_log_enabled();

    show_dynamic_simulator_configuration();

    auto clock_start = system_clock::now();

    ostringstream osstream;
    osstream<<"Dynamics initialization starts.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    if(not pf_solver.is_converged())
    {
        osstream<<"Warning. Powerflow is not converged. Please go check powerflow solution.\n"
                <<"Any further simulation cannot be trusted.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    pf_solver.clear();


    meter_values.resize(meters.size(), 0.0);

    TIME = -2.0*DELT;
    flag_rotor_angle_stable = true;

    optimize_network_ordering();

    prepare_devices_for_run();

    prepare_buffers_for_system_change_detection();

    run_all_models(DYNAMIC_INITIALIZE_MODE);
    run_bus_frequency_blocks(DYNAMIC_INITIALIZE_MODE);

    network_matrix.build_dynamic_network_Y_matrix();
    build_jacobian();

    //const SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_Y_matrix();
    //Y.report_brief();
    //network_matrix.report_physical_internal_bus_number_pair();


    if(get_rotor_angle_stability_surveillance_flag()==true)
        update_generators_in_islands();

    ITER_DAE = 0;
    ITER_NET = 0;

    bool converged = false;
    unsigned int iter_count  = 0;
    current_max_network_iteration = get_max_network_iteration();
    while(true)
    {
		++iter_count;
		//osstream<<"Network iteration "<<iter_count;
		//toolkit.show_information_with_leading_time_stamp(osstream);

        if(detailed_log_enabled)
        {
            ostringstream osstream;
            osstream<<"Initialization DAE iteration "<<iter_count<<":";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        converged = solve_network();
        ITER_NET += network_iteration_count;
        if(converged or iter_count>get_max_DAE_iteration())
            break;
    }
    ITER_DAE = iter_count;
    if(ITER_NET>1)
    {
        osstream<<"Warning. Network solution converged in "<<ITER_NET<<" iterations for dynamics simulation initialization.\n"
                <<"Any solution with more than 1 iteration is indicating bad initial powerflow result or exceeding dynamic upper or lower limits.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    initialize_buffers_for_system_change_detection();

    osstream<<"Dynamics initialization finished.";
    toolkit.show_information_with_leading_time_stamp(osstream);


    auto tduration = duration_cast<microseconds>(system_clock::now()-clock_start);
    time_elapse_in_a_step = tduration.count()*0.001;
    time_elapse_of_differential_equations_in_a_step = 0.001*microseconds_elapse_of_differential_equations_in_a_step;
    time_elapse_of_network_solution_in_a_step = 0.001*microseconds_elapse_of_network_solution_in_a_step;

    save_meter_information();
    save_meter_values();
}

void DYNAMICS_SIMULATOR::prepare_devices_for_run()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    in_service_buses = psdb.get_all_in_service_buses();
    generators = psdb.get_all_generators();
    wt_generators = psdb.get_all_wt_generators();
    pv_units = psdb.get_all_pv_units();
    e_storages = psdb.get_all_energy_storages();
    sv_sources = psdb.get_all_sv_sources();

    loads = psdb.get_all_loads();

    lines = psdb.get_all_ac_lines();
    hvdcs = psdb.get_all_lcc_hvdc2ts();

    vsc_hvdcs = psdb.get_all_vsc_hvdcs();
    mt_hvdcs = psdb.get_all_mt_hvdcs();

    e_devices = psdb.get_all_equivalent_devices();
}

void DYNAMICS_SIMULATOR::stop()
{
    //if(TIME >0.0)
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<"Dynamics simulation stops at simulation time: "<<TIME<<"s.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    if(json_output_file.is_open())
        json_output_file<<endl
                        <<"                    ]"<<endl
                        <<"}";
    close_meter_output_files();
}


double DYNAMICS_SIMULATOR::get_system_max_angle_difference_in_deg()
{
    unsigned int n = generators.size();
    double max_angle = -1e10, min_angle = 1e10;
    double angle;
    for(unsigned int i=0; i!=n; ++i)
    {
        GENERATOR& gen = *(generators[i]);
        if(gen.get_status()==true)
        {
            SYNC_GENERATOR_MODEL* model_ptr = gen.get_sync_generator_model();
            if(model_ptr!=NULL)
            {
                SYNC_GENERATOR_MODEL& model = *model_ptr;
                angle = model.get_rotor_angle_in_deg();
                if(angle>max_angle)
                    max_angle = angle;
                if(angle<min_angle)
                    min_angle = angle;
            }
        }
    }
    return max_angle-min_angle;
}

void DYNAMICS_SIMULATOR::run_to(double time)
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = toolkit.get_network_matrix();

    network_matrix.build_dynamic_network_Y_matrix();
    build_jacobian();

    build_vsc_hvdc_dynamic_dc_network_matrix();

    update_with_event(FAULT_OR_OPERATION_EVENT);

    unsigned int jacobian_update_count = 0;

    while(TIME<=time-DOUBLE_EPSILON)
    {
        if(jacobian_update_count<3)
            build_jacobian();
        jacobian_update_count++;
        bool change_time_step_for_precise_time = false;
        double old_DELT = DELT;
        if(TIME+DELT>time and fabs(time-TIME)>FLOAT_EPSILON)
        {
            change_dynamic_simulator_time_step_in_s(time-TIME);
            change_time_step_for_precise_time = true;
        }

        run_a_step();

        if(change_time_step_for_precise_time == true)
            change_dynamic_simulator_time_step_in_s(old_DELT);

        if(get_rotor_angle_stability_surveillance_flag()==true)
        {
            update_generators_in_islands();
            flag_rotor_angle_stable = is_system_angular_stable();
            if(flag_rotor_angle_stable==true)
                ;
            else
            {
                ostringstream osstream;
                osstream<<"At time "<<TIME<<"s, "
                        <<"system is detected to be unstable due to rotor angular stability surveillance logic."<<endl
                        <<"No further simulation will be performed.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
}

void DYNAMICS_SIMULATOR::run_a_step()
{
    STEPS& toolkit = get_toolkit();
    auto clock_start = system_clock::now();
    microseconds_elapse_of_differential_equations_in_a_step = 0;
    microseconds_elapse_of_network_solution_in_a_step = 0;
    ostringstream osstream;

    TIME += DELT;
    if(detailed_log_enabled)
    {
        osstream<<"Run dynamic simulation to time "<<TIME<<"s.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    update_equivalent_devices_buffer();
    update_equivalent_devices_output();

    if(is_network_matrix_update_required())
    {
        NETWORK_MATRIX& network = get_network_matrix();
        network.build_dynamic_network_Y_matrix();
        set_network_matrix_update_as_unrequired();
    }
    //bool network_converged = false;
    //bool DAE_converged = false;
    ITER_DAE = 0;
    ITER_NET = 0;
    current_max_network_iteration = get_max_network_iteration();
    while(true)
    {
        ++ITER_DAE;
        if(detailed_log_enabled)
        {
            osstream<<"Solve DAE with iteration "<<ITER_DAE<<" at time "<<TIME<<"s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        if(ITER_DAE<=max_DAE_iteration)
        {
            integrate();

            double smax_th_MVA_original = get_allowed_max_power_imbalance_in_MVA();
            double alpha_original = get_iteration_accelerator();
            if(ITER_DAE==1 and smax_th_MVA_original<=0.01)
            {
                set_allowed_max_power_imbalance_in_MVA(smax_th_MVA_original*100.0);
                set_iteration_accelerator(alpha_original*0.75);
            }
            if(ITER_DAE==2 and smax_th_MVA_original<=0.01)
            {
                set_allowed_max_power_imbalance_in_MVA(smax_th_MVA_original*10.0);
                set_iteration_accelerator(alpha_original*0.875);
            }

            bool network_converged = solve_network();

            set_allowed_max_power_imbalance_in_MVA(smax_th_MVA_original);
            set_iteration_accelerator(alpha_original);

            run_bus_frequency_blocks(DYNAMIC_INTEGRATE_MODE);


            ITER_NET += network_iteration_count;

            if(detailed_log_enabled and not network_converged)
            {
                osstream<<"Failed to solve network in "<<max_network_iteration<<" iterations during DAE iteration "<<ITER_DAE<<" when integrating at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(ITER_DAE>=get_min_DAE_iteration() and network_converged) // DAE solution converged
                break;
        }
        else
        {
            if(max_DAE_iteration!=2)
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve DAE in %u iterations when integrating at time %f s.",
                         max_DAE_iteration, TIME);
                toolkit.show_information_with_leading_time_stamp(buffer);
            }
            --ITER_DAE;
            break;
        }
    }
    update();
    run_bus_frequency_blocks(DYNAMIC_INTEGRATE_MODE);
    run_bus_frequency_blocks(DYNAMIC_UPDATE_MODE);
    check_relay_events();

    auto tduration = duration_cast<microseconds>(system_clock::now()-clock_start);
    time_elapse_in_a_step = tduration.count()*0.001;
    time_elapse_of_differential_equations_in_a_step = 0.001*microseconds_elapse_of_differential_equations_in_a_step;
    time_elapse_of_network_solution_in_a_step = 0.001*microseconds_elapse_of_network_solution_in_a_step;

    save_meter_values();
}

void DYNAMICS_SIMULATOR::update_with_event(DYNAMIC_EVENT_TYPE event_type)
{
    STEPS& toolkit = get_toolkit();
    auto clock_start = system_clock::now();
    microseconds_elapse_of_differential_equations_in_a_step = 0;
    microseconds_elapse_of_network_solution_in_a_step = 0;

    ostringstream osstream;

    bool network_converged = false;
    unsigned int update_event_iter_max = get_max_event_update_iteration();

    ITER_NET = 0;

    unsigned int iter = 0;
    current_max_network_iteration = get_max_network_iteration();
    while(true)
    {
        network_converged = solve_network();
        ++iter;
        ITER_NET += network_iteration_count;
        if(not network_converged)
        {
            if(iter<=update_event_iter_max)
                continue;
            else
            {
                char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve network in %u iterations when updating with event at time %f s.",
                         update_event_iter_max, TIME);
                toolkit.show_information_with_leading_time_stamp(buffer);
                break;
            }
        }
        else
            break;
    }
    switch(event_type)
    {
        case FAULT_OR_OPERATION_EVENT:
            update();
            break;
        case TIME_STEP_CHANGE_EVENT:
            update_when_time_step_is_change();
            break;
        default:
            break;
    }
    update_bus_frequency_blocks_when_applying_event(event_type);

    auto tduration = duration_cast<microseconds>(system_clock::now()-clock_start);
    time_elapse_in_a_step = tduration.count()*0.001;
    time_elapse_of_differential_equations_in_a_step = 0.001*microseconds_elapse_of_differential_equations_in_a_step;
    time_elapse_of_network_solution_in_a_step = 0.001*microseconds_elapse_of_network_solution_in_a_step;

    if(event_type == FAULT_OR_OPERATION_EVENT)
    {
        save_meter_values();
        update_buffers_for_system_change_detection();
    }
}

void DYNAMICS_SIMULATOR::integrate()
{
    //clock_t start = clock();
    run_all_models(DYNAMIC_INTEGRATE_MODE);
    //cout<<"    elapsed time for integration: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
}

void DYNAMICS_SIMULATOR::update()
{
    update_with_specific_mode(DYNAMIC_UPDATE_MODE);

    update_buffers_for_system_change_detection();
    if(get_change_time_step_adaptively_logic()==true)
    {
        if(is_system_changing_suddenly())
            change_dynamic_simulator_time_step_to_minimum();
        else
        {
            if(is_system_in_quasi_steady_state())
                increase_dynamic_simulator_time_step_automatically();
            else
                decrease_dynamic_simulator_time_step_automatically();
        }
    }
}

void DYNAMICS_SIMULATOR::update_when_time_step_is_change()
{
    update_with_specific_mode(DYNAMIC_UPDATE_TIME_STEP_MODE);
}

void DYNAMICS_SIMULATOR::update_with_specific_mode(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit();
    if(mode==DYNAMIC_UPDATE_MODE or mode==DYNAMIC_UPDATE_TIME_STEP_MODE)
    {
        ostringstream osstream;

        update_equivalent_devices_buffer();
        run_all_models(mode);

        bool network_converged = false;

        current_max_network_iteration = get_max_network_iteration();

        unsigned int max_update_iter = get_max_update_iteration();
        unsigned int iter = 0;
        while(iter<max_update_iter)
        {
            double alpha_original = get_iteration_accelerator();
            if(iter==1)
            {
                set_iteration_accelerator(alpha_original*0.75);
            }
            if(iter==2)
            {
                set_iteration_accelerator(alpha_original*0.875);
            }
            network_converged = solve_network();
            set_iteration_accelerator(alpha_original);
            ITER_NET += network_iteration_count;

            if(network_converged)
                break;
            else
                ++iter;
        }

        if(not network_converged and current_max_network_iteration>1)
        {
            char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
            if(mode==DYNAMIC_UPDATE_MODE)
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve network in %u iterations when updating at time %f s in DYNAMIC_UPDATE_MODE.",
                         current_max_network_iteration, TIME);
            else
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve network in %u iterations when updating at time %f s in DYNAMIC_UPDATE_TIME_STEP_MODE.",
                         current_max_network_iteration, TIME);
            toolkit.show_information_with_leading_time_stamp(buffer);
        }
    }
}

void DYNAMICS_SIMULATOR::check_relay_events()
{
    disable_relay_action_flag();
    run_all_models(DYNAMIC_RELAY_MODE);

    if(is_relay_action_detected()==true)
        update();
}

void DYNAMICS_SIMULATOR::run_all_models(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit();
    //ostringstream osstream;
    auto clock_start = steady_clock::now();

    //auto clock_1 = steady_clock::now();
    unsigned int n = 0;

    n = generators.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_generator_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        GENERATOR& gen = *(generators[i]);
        gen.run(mode);
    }

    /*auto clock_2 = steady_clock::now();
    auto tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", generator models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    n = wt_generators.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_wt_generator_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        WT_GENERATOR& wtg = *(wt_generators[i]);
        wtg.run(mode);
    }

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", wt generator models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    n = pv_units.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_pv_unit_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        PV_UNIT& pvu = *(pv_units[i]);
        pvu.run(mode);
    }

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", pv models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    n = e_storages.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_energy_storage_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        ENERGY_STORAGE& es = *(e_storages[i]);
        es.run(mode);

    }

    n = sv_sources.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_sv_source_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        SV_SOURCE& svs = *(sv_sources[i]);
        svs.run(mode);
    }

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", pv models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    n = loads.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_load_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        LOAD& load = *(loads[i]);
        load.run(mode);
    }

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", load models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    n = hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_lcc_hvdc2t_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        LCC_HVDC2T& hvdc = *(hvdcs[i]);
        if(hvdc.get_status()==true)
            hvdc.run(mode);
    }

    n = vsc_hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_lcc_hvdc2t_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        VSC_HVDC& vsc_hvdc = *(vsc_hvdcs[i]);
        if(vsc_hvdc.get_status()==true)
            vsc_hvdc.run(mode);
    }

    n = mt_hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_mt_hvdc_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        MT_HVDC& mt_hvdc = *(mt_hvdcs[i]);
        if(mt_hvdc.get_status()==true)
            mt_hvdc.run(mode);
    }

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", hvdc models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    n = e_devices.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_equivalent_device_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        EQUIVALENT_DEVICE& edevice = *(e_devices[i]);
        if(edevice.get_status()==true)
            edevice.run(mode);
    }

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", equivalent device models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;*/

    /*clock_2 = steady_clock::now();
    tduration = duration_cast<nanoseconds>(clock_2-clock_1);
    osstream<<"at time "<<TIME<<", bus frequency models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);
    clock_1 = clock_2;

    tduration = duration_cast<nanoseconds>(steady_clock::now()-clock_start);
    osstream<<"at time "<<TIME<<", all models run takes "<<tduration.count()<<" ns.";
    toolkit.show_information_with_leading_time_stamp(osstream);*/

    auto tdurationx = duration_cast<microseconds>(steady_clock::now()-clock_start);
    microseconds_elapse_of_differential_equations_in_a_step += tdurationx.count();
}

void DYNAMICS_SIMULATOR::run_bus_frequency_blocks(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit();
    auto clock_start = steady_clock::now();

    unsigned int n = in_service_buses.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        BUS& in_service_buse = *(in_service_buses[i]);
        BUS_FREQUENCY_MODEL& bus_frequency_model = *(in_service_buse.get_bus_frequency_model());
        bus_frequency_model.run(mode);
    }
    auto tduration = duration_cast<microseconds>(steady_clock::now()-clock_start);
    microseconds_elapse_of_differential_equations_in_a_step += tduration.count();
}

void DYNAMICS_SIMULATOR::update_bus_frequency_blocks_when_applying_event(DYNAMIC_EVENT_TYPE type)
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = in_service_buses.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        BUS& in_service_buse = *(in_service_buses[i]);
        BUS_FREQUENCY_MODEL& bus_frequency_model = *(in_service_buse.get_bus_frequency_model());
        bus_frequency_model.update_for_applying_event(type);
    }
}

void DYNAMICS_SIMULATOR::enable_relay_action_flag()
{
    relay_action_flag = true;
}

void DYNAMICS_SIMULATOR::disable_relay_action_flag()
{
    relay_action_flag = false;
}

bool DYNAMICS_SIMULATOR::is_relay_action_detected() const
{
    return relay_action_flag;
}

void DYNAMICS_SIMULATOR::update_equivalent_devices_buffer()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = e_devices.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_equivalent_device_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        EQUIVALENT_DEVICE& edevice = *(e_devices[i]);
        edevice.update_meter_buffer();
    }
}

void DYNAMICS_SIMULATOR::update_equivalent_devices_output()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = e_devices.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_equivalent_device_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        ostringstream osstream;
        EQUIVALENT_DEVICE& edevice = *(e_devices[i]);
        edevice.update_equivalent_outputs();

        osstream<<"At time "<<TIME<<" s, equivalent load of "<<edevice.get_compound_device_name()<<" is: "<<edevice.get_total_equivalent_power_as_load_in_MVA()<<"MVA";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

bool DYNAMICS_SIMULATOR::get_system_angular_stable_flag() const
{
    return flag_rotor_angle_stable;
}

bool DYNAMICS_SIMULATOR::solve_network()
{
    STEPS& toolkit = get_toolkit();
    auto clock_start = steady_clock::now();

    max_current_mismatch_pu = 0.0;
    max_power_mismatch_MVA = 0.0;
    max_mismatch_bus = 0;

    ostringstream osstream;
    bool converged = false;

    network_iteration_count = 0;

    unsigned int network_iter_max = current_max_network_iteration;
    unsigned int network_iter_count = 0;

    unsigned int network_solution_divergent_count = 0;

    double smax_th_MVA = get_allowed_max_power_imbalance_in_MVA();
    double imax_th_pu = smax_th_MVA*toolkit.get_one_over_system_base_power_in_one_over_MVA()*0.5;

    double original_alpha = get_iteration_accelerator();

    GREATEST_POWER_CURRENT_MISMATCH_STRUCT greatest_mismatch_struct;


    solve_lcc_hvdc2ts_without_integration();

    update_vsc_hvdcs_converter_model();
    solve_mt_hvdc_systems();

    get_bus_current_mismatch();


    #ifndef USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
    calculate_bus_power_mismatch_in_MVA();
    greatest_mismatch_struct = get_max_power_mismatch_struct();
    max_power_mismatch_MVA = greatest_mismatch_struct.greatest_power_mismatch_in_MVA;
    max_mismatch_bus = greatest_mismatch_struct.bus_with_greatest_power_mismatch;
    if(max_power_mismatch_MVA<get_allowed_max_power_imbalance_in_MVA())
    #else
    greatest_mismatch_struct = get_max_current_mismatch_struct();
    max_current_mismatch_pu = greatest_mismatch_struct.greatest_current_mismatch_in_pu;
    max_mismatch_bus = greatest_mismatch_struct.bus_with_greatest_current_mismatch;
    if(max_current_mismatch_pu<imax_th_pu)
    #endif // USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
    {
        return true;
    }
    else
    {
        while(true)
        {
            if(network_iter_count<network_iter_max)
            {
                //if(get_automatic_iteration_accelerator_tune_logic()==true)
                //    tune_iteration_accelerator_based_on_maximum_current_mismatch(max_current_mismatch_pu);
                delta_V = I_mismatch/jacobian;
                if(jacobian.is_lu_factorization_failed())
                {
                    ostringstream osstream;
                    osstream<<"No further network solution will be attempted since LU factorization of dynamic Jacobian matrix is failed at time "<<TIME<<"s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    break;
                }
                if(network_iter_count>5)
                    alpha = original_alpha*steps_pow(0.95, network_iter_count-5);

                update_bus_voltage();
                if(get_non_divergent_solution_logic()==true)
                {
                    unsigned int n_delta_V = delta_V.size();
                    for(unsigned int i=0; i<n_delta_V; ++i)
                        delta_V[i] = -delta_V[i];

                    for(unsigned int i=0; i<5; ++i)
                    {
                        //++network_iter_count;
                        solve_lcc_hvdc2ts_without_integration();
                        update_vsc_hvdcs_converter_model();
                        get_bus_current_mismatch();
                        #ifndef USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                        calculate_bus_power_mismatch_in_MVA();
                        greatest_mismatch_struct = get_max_power_mismatch_struct();
                        double new_smax = greatest_mismatch_struct.greatest_power_mismatch_in_MVA;
                        if(new_smax>=max_power_mismatch_MVA)
                        #else
                        greatest_mismatch_struct = get_max_current_mismatch_struct();
                        double new_imax = greatest_mismatch_struct.greatest_current_mismatch_in_pu;
                        if(new_imax>=max_current_mismatch_pu)
                        #endif // USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                        {
                            alpha *= 0.5;
                            update_bus_voltage();
                        }
                        else
                            break;
                    }
                    set_iteration_accelerator(original_alpha);
                }
                ++network_iter_count;

                solve_lcc_hvdc2ts_without_integration();
                update_vsc_hvdcs_converter_model();
                get_bus_current_mismatch();
                #ifndef USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                calculate_bus_power_mismatch_in_MVA();
                greatest_mismatch_struct = get_max_power_mismatch_struct();
                double new_smax = greatest_mismatch_struct.greatest_power_mismatch_in_MVA;
                max_mismatch_bus = greatest_mismatch_struct.bus_with_greatest_power_mismatch;
                if(new_smax>max_power_mismatch_MVA)
                #else
                double new_imax = get_max_current_mismatch_struct().greatest_current_mismatch_in_pu;
                max_mismatch_bus = greatest_mismatch_struct.bus_with_greatest_current_mismatch;

                if(new_imax>max_current_mismatch_pu)
                #endif // USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                {
                    ++network_solution_divergent_count;
                    if(network_solution_divergent_count>get_max_network_solution_divergent_threshold())
                    {
                        #ifndef USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                        max_power_mismatch_MVA = new_smax;
                        #else
                        max_current_mismatch_pu = new_imax;
                        #endif // USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                        converged = false;
                        break;
                    }
                }
                #ifndef USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                max_power_mismatch_MVA = new_smax;
                if(max_power_mismatch_MVA>get_allowed_max_power_imbalance_in_MVA())
                #else
                if(toolkit.is_detailed_log_enabled())
                {
                    double scale = new_imax/max_current_mismatch_pu;
                    if(scale>1.0)
                    {
                        osstream<<"At time "<<TIME<<" s, when calling DYNAMICS_SIMULATOR::solve_network() in iteration "<<network_iter_count<<", max current mismatch diverges to "<<new_imax<<" pu (increase scale: "<<scale<<").";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                max_current_mismatch_pu = new_imax;
                if(max_current_mismatch_pu>imax_th_pu)
                #endif // USE_DYNAMIC_CURRENT_MISMATCH_CONTROL
                    continue;
                else
                {
                    converged = true;
                    break;
                }
            }
            else
            {
                //--network_iter_count;
                break;
            }
        }
    }

    set_iteration_accelerator(original_alpha);

    network_iteration_count = network_iter_count;

    auto tduration = duration_cast<microseconds>(steady_clock::now()-clock_start);
    microseconds_elapse_of_network_solution_in_a_step += tduration.count();
    return converged;
}

void DYNAMICS_SIMULATOR::tune_iteration_accelerator_based_on_maximum_current_mismatch(double imax)
{
    STEPS& toolkit = get_toolkit();
    double mbase = toolkit.get_system_base_power_in_MVA();
    double smax = imax*mbase;
    if(smax<0.1)
        set_iteration_accelerator(1.0);
    else
    {
        if(smax<1.0)
            set_iteration_accelerator(0.8);
        else
        {
            if(smax<5.0)
                set_iteration_accelerator(0.7);
            else
            {
                if(smax<10.0)
                    set_iteration_accelerator(0.65);
                else
                {
                    if(smax<20.0)
                        set_iteration_accelerator(0.625);
                    else
                        set_iteration_accelerator(0.6);
                    /*{
                        if(smax<50.0)
                            set_iteration_accelerator(0.75);
                        else
                        {
                            if(smax<75.0)
                                set_iteration_accelerator(0.7);
                            else
                            {
                                if(smax<100.0)
                                    set_iteration_accelerator(0.65);
                                else
                                    set_iteration_accelerator(0.6);
                            }
                        }
                    }*/
                }
            }
        }
    }
    //ostringstream osstream;
    //osstream<<"At time "<<TIME<<", dynamic simulation iteration accelerator is automatically tuned: ("<<smax<<"MVA, "<<get_iteration_accelerator()<<").";
    //toolkit.show_information_with_leading_time_stamp(osstream);
}

void DYNAMICS_SIMULATOR::solve_lcc_hvdc2ts_without_integration()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_lcc_hvdc2t_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        LCC_HVDC2T& hvdc = *(hvdcs[i]);
        LCC_HVDC2T_MODEL* model_ptr = hvdc.get_lcc_hvdc2t_model();
        if(model_ptr!=NULL)
        {
            LCC_HVDC2T_MODEL& model = *model_ptr;
            model.solve_lcc_hvdc2t_model_without_integration();
        }
    }
}

void DYNAMICS_SIMULATOR::update_vsc_hvdcs_converter_model()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = vsc_hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_lcc_hvdc2t_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        VSC_HVDC& vsc_hvdc = *(vsc_hvdcs[i]);
        vector<VSC_HVDC_CONVERTER_MODEL*> models = vsc_hvdc.get_vsc_hvdc_converter_models();
        unsigned int m = models.size();
        for(unsigned int j=0; j!=m ; ++j)
        {
            if(models[j]!=NULL)
            {
                VSC_HVDC_CONVERTER_MODEL& model = *(models[j]);
                model.run(DYNAMIC_UPDATE_MODE);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::solve_mt_hvdc_systems()
{
     STEPS& toolkit = get_toolkit();
     unsigned int n = mt_hvdcs.size();
     #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        //set_openmp_number_of_threads(toolkit.get_mt_hvdc_thread_number());
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR

    //osstream<<"Start to solve mt hvdc system in dynamic simulatiion";
    //toolkit.show_information_with_leading_time_stamp(osstream);
    for(unsigned int i=0; i<n; ++i)
    {
        MT_HVDC& mt_hvdc = *(mt_hvdcs[i]);
        mt_hvdc.solve_dynamic_dc_systems();
    }
 }

void DYNAMICS_SIMULATOR::get_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    NETWORK_MATRIX& net = get_network_matrix();

    get_bus_current_into_network();

    unsigned int n = I_mismatch.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i = 0; i<n; ++i)
        I_mismatch[i] = -I_mismatch[i];

    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after get_bus_current_into_network() when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }

    add_generators_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding generators when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_wt_generators_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding wt generators when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_pv_units_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding pv units when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_energy_storage_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding energy storages when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_sv_source_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding sv sources when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_loads_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding loads when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_lcc_hvdc2ts_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding hvdcs when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[j]<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_vsc_hvdcs_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding vsc hvdcs when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[j]<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_mt_hvdcs_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding equivalent devices when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_equivalent_devices_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(unsigned int i = 0; i<n; ++i)
        {
            if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding equivalent devices when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(unsigned int j = 0; j<n; ++j)
                {
                    if(std::isnan(I_mismatch[j].real()) or std::isnan(I_mismatch[j].imag()))
                    {
                        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    /*
    osstream<<"bus current mismatch:"<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);
    for(unsigned int i=0; i<n; ++i)
    {
        unsigned int ibus = net.get_physical_bus_number_of_internal_bus(i);
        osstream<<ibus<<", "<<I_mismatch[i]<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    */

    /*
    double maxmismatch = 0.0;
    complex<double> cmaxmismatch = 0.0;
    unsigned int busmax = 0;
    for(unsigned int i=0; i<n; ++i)
    {
        if(maxmismatch<abs(I_mismatch[i]))
        {
            maxmismatch = abs(I_mismatch[i]);
            cmaxmismatch = I_mismatch[i];
            busmax = net.get_physical_bus_number_of_internal_bus(i);
        }
    }
    osstream<<"max mismatch @ bus "<<busmax<<", "<<maxmismatch<<" or complex "<<cmaxmismatch<<", complex power = "<<psdb.get_bus_positive_sequence_complex_voltage_in_pu(busmax)*conj(cmaxmismatch)*psdb.get_system_base_power_in_MVA()<<setprecision(10)<<", v="<<psdb.get_bus_positive_sequence_voltage_in_pu(busmax)<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    for(unsigned int i=0; i<n; ++i)
    {
        if(abs(I_mismatch[i])>0.0001)
        {
            osstream<<"mismatch @ bus "<<net.get_physical_bus_number_of_internal_bus(i)<<", "<<I_mismatch[i]<<">0.0001"<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        if(std::isnan(I_mismatch[i].real()) or std::isnan(I_mismatch[i].imag()))
        {
            osstream<<"NAN is detected in I_mismatch @ bus "<<net.get_physical_bus_number_of_internal_bus(i)<<", "<<I_mismatch[i]<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    */
}

void DYNAMICS_SIMULATOR::get_bus_current_into_network()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    STEPS_COMPLEX_SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_Y_matrix();

    I_mismatch = Y.multiply_vector_b(internal_bus_complex_voltage_in_pu);

    if(get_toolkit().is_detailed_log_enabled())
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_generators_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int ngen = generators.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_generator_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<ngen; ++i)
    {
        GENERATOR& generator = *(generators[i]);

        if(generator.get_status() == true)
        {
            unsigned int physical_bus = generator.get_generator_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            SYNC_GENERATOR_MODEL* gen_model_ptr = generator.get_sync_generator_model();
            if(gen_model_ptr!=NULL)
            {
                SYNC_GENERATOR_MODEL& gen_model = *gen_model_ptr;
                I_mismatch[internal_bus] += gen_model.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = gen_model.get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = gen_model.get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and ngen != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_wt_generators_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int ngen = wt_generators.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_wt_generator_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<ngen; ++i)
    {
        WT_GENERATOR& generator = *(wt_generators[i]);

        if(generator.get_status() == true)
        {
            unsigned int physical_bus = generator.get_generator_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            WT_GENERATOR_MODEL* gen_model_ptr = generator.get_wt_generator_model();
            if(gen_model_ptr!=NULL)
            {
                WT_GENERATOR_MODEL& gen_model = *gen_model_ptr;
                if(gen_model.is_current_source())
                    I_mismatch[internal_bus] += gen_model.get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                else
                    I_mismatch[internal_bus] += gen_model.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //cout<<generator.get_compound_device_name()<<" terminal or Norton current is: "<<I<<endl;

                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = gen_model.get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = gen_model.get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and ngen != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_pv_units_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int npv = pv_units.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_pv_unit_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<npv; ++i)
    {
        complex<double> I;
        PV_UNIT& pv = *(pv_units[i]);

        if(pv.get_status() == true)
        {
            unsigned int physical_bus = pv.get_unit_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            PV_CONVERTER_MODEL* conv_model_ptr = pv.get_pv_converter_model();
            if(conv_model_ptr!=NULL)
            {
                PV_CONVERTER_MODEL& conv_model = *conv_model_ptr;
                if(conv_model.is_current_source())
                    I = conv_model.get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                else
                    I = conv_model.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //cout<<pv.get_compound_device_name()<<" terminal or Norton current is: "<<I<<endl;

                I_mismatch[internal_bus] += I;

                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = conv_model.get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = conv_model.get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and npv != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_energy_storage_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nes = e_storages.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_energy_storage_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<nes; ++i)
    {
        complex<double> I;
        ENERGY_STORAGE& es = *(e_storages[i]);

        if(es.get_status() == true)
        {
            unsigned int physical_bus = es.get_energy_storage_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            ES_CONVERTER_MODEL* conv_model_ptr = es.get_es_converter_model();
            if(conv_model_ptr!=NULL)
            {
                ES_CONVERTER_MODEL& conv_model = *conv_model_ptr;
                if(conv_model.is_current_source())
                    I = conv_model.get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                else
                    I = conv_model.get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //cout<<es.get_compound_device_name()<<" terminal or Norton current is: "<<I<<endl;

                I_mismatch[internal_bus] += I;
                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = conv_model.get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = conv_model.get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and nes != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_sv_source_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nsvs = sv_sources.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_sv_source_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<nsvs; ++i)
    {
        complex<double> I;
        SV_SOURCE& svs = *(sv_sources[i]);

        if(svs.get_status() == true)
        {
            unsigned int physical_bus = svs.get_sv_source_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            SVS_CONTROL_MODEL* svs_ctrl_model_ptr = svs.get_svs_control_model();
            if(svs_ctrl_model_ptr!=NULL)
            {
                SVS_CONTROL_MODEL& svs_ctrl_model = *svs_ctrl_model_ptr;
                I = svs_ctrl_model.get_terminal_complex_current_in_pu_based_on_sbase();
                I_mismatch[internal_bus] += I;
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and nsvs != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_loads_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nload = loads.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_load_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<nload; ++i)
    {
        LOAD& load = *(loads[i]);
        if(load.get_status()==true)
        {
            unsigned int physical_bus = load.get_load_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            LOAD_MODEL* model_ptr = load.get_load_model();
            if(model_ptr != NULL)
            {
                LOAD_MODEL& model = *model_ptr;
                if(not model.is_voltage_source())
                {
                    model.synchronize_bus_voltage_and_frequency();
                    I_mismatch[internal_bus] -= load.get_dynamics_load_current_in_pu_based_on_system_base_power();
                    if(toolkit.is_detailed_log_enabled() and abs(load.get_dynamics_load_current_in_pu_based_on_system_base_power())>10)
                    {
                        ostringstream osstream;
                        osstream<<load.get_compound_device_name()<<" has current greater than 10 pu:"<<endl
                            <<"I = "<<load.get_dynamics_load_current_in_pu_based_on_system_base_power()<<", V = "<<model.get_bus_positive_sequence_voltage_in_pu();
                        get_toolkit().show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                    I_mismatch[internal_bus] += load.get_dynamics_load_norton_current_in_pu_based_on_system_base_power();
            }
            else
                I_mismatch[internal_bus] -= load.get_dynamics_load_current_in_pu_based_on_system_base_power();
        }
    }

    if(toolkit.is_detailed_log_enabled() and nload != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_lcc_hvdc2ts_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nhvdc = hvdcs.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_lcc_hvdc2t_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<nhvdc; ++i)
    {
        complex<double> I;
        LCC_HVDC2T& hvdc = *(hvdcs[i]);
        if(hvdc.get_status()==true)
        {
            unsigned int physical_bus = hvdc.get_converter_bus(RECTIFIER);

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            //I = hvdc.get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);

            //I_mismatch[internal_bus] -= I;
            I_mismatch[internal_bus] -= hvdc.get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);

            if(not detailed_log_enabled)
                ;
            else
            {
                I = hvdc.get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);
                I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                osstream<<"Current at rectifier side of "<<hvdc.get_compound_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                        <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            physical_bus = hvdc.get_converter_bus(INVERTER);

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            //I = hvdc.get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);

            //I_mismatch[internal_bus] -= I;
            I_mismatch[internal_bus] -= hvdc.get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);

            if(not detailed_log_enabled)
                ;
            else
            {
                I = hvdc.get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);
                I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                osstream<<"Current at inverter side of "<<hvdc.get_compound_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                        <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and nhvdc != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_vsc_hvdcs_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nvsc = vsc_hvdcs.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_lcc_hvdc2t_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<nvsc; ++i)
    {
        complex<double> I;
        VSC_HVDC& vsc_hvdc = *(vsc_hvdcs[i]);
        if(vsc_hvdc.get_status()==true)
        {
            unsigned int ncon = vsc_hvdc.get_converter_count();
            for(unsigned int j=0; j!=ncon; ++j)
            {
                unsigned int physical_bus = vsc_hvdc.get_converter_ac_bus(j);
                unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

                if(vsc_hvdc.is_converter_a_dynamic_voltage_source(j))
                    I_mismatch[internal_bus] += vsc_hvdc.get_converter_dynamic_Norton_current_to_ac_bus_in_pu_on_system_base_as_voltage_source(j);
                else
                    I_mismatch[internal_bus] += vsc_hvdc.get_converter_dynamic_current_to_ac_bus_in_pu_on_system_base_as_current_source(j);
                if(not detailed_log_enabled)
                    ;
                else
                {
                    if(vsc_hvdc.is_converter_a_dynamic_voltage_source(j))
                    {
                        I  = vsc_hvdc.get_converter_dynamic_Norton_current_to_ac_bus_in_pu_on_system_base_as_voltage_source(j);
                        I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                        osstream<<"Nortron current at vsc-hvdc ac side of "<<vsc_hvdc.get_compound_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                                <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    else
                    {
                        I = vsc_hvdc.get_converter_dynamic_current_to_ac_bus_in_pu_on_system_base_as_current_source(j);
                        I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                        osstream<<"Current at vsc-hvdc ac side of "<<vsc_hvdc.get_compound_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                                <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and nvsc != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_mt_hvdcs_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nmtdc = mt_hvdcs.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_mt_hvdc_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR;
    for(unsigned int i=0; i<nmtdc; ++i)
    {
        complex<double> I;
        MT_HVDC& mtdc = *(mt_hvdcs[i]);
        if(mtdc.get_status()==true)
        {
            vector<HLCC*> lccs = mtdc.get_all_lcc_converters();
            unsigned int n_lcc = lccs.size();
            for(unsigned int j=0;j!=n_lcc;++j)
            {
                HLCC& lcc = *(lccs[j]);
                if(lcc.get_status()==true)
                {
                    unsigned int physical_bus = lcc.get_converter_ac_bus();
                    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

                    I_mismatch[internal_bus] -= lcc.get_converter_dynamic_ac_current_in_pu_on_system_base();

                    /*
                    I  = lcc.get_converter_dynamic_ac_current_in_pu_on_system_base();
                    I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                    osstream<<"AC curent at mt-hvdc:"<<mtdc.get_compound_device_name()<<" at LCC side: "<<lcc.get_converter_name()<<"to AC BUS: "<<physical_bus<<" IS :"<<I<<"kA or "<<abs(I)<<"kA"<<endl
                            <<"UAC = "<<psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)<<endl
                            <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    */
                    if(not detailed_log_enabled)
                        ;
                    else
                    {
                        I  = lcc.get_converter_dynamic_ac_current_in_pu_on_system_base();
                        I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                        osstream<<"AC curent at mt-hvdc:"<<mtdc.get_compound_device_name()<<" at LCC side "<<lcc.get_converter_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                                <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                        toolkit.show_information_with_leading_time_stamp(osstream);

                    }
                }

            }

            vector<HVSC*> vscs = mtdc.get_all_vsc_converters();
            unsigned int n_vsc = vscs.size();
            for(unsigned int j=0;j!=n_vsc;++j)
            {
                HVSC& vsc = *(vscs[j]);
                if(vsc.get_status()==true)
                {
                    unsigned int physical_bus = vsc.get_converter_ac_bus();
                    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

                    I_mismatch[internal_bus] -= vsc.get_converter_dynamic_ac_current_in_pu_on_system_base();

                    /*
                    I  = vsc.get_converter_dynamic_ac_current_in_pu_on_system_base();
                    I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                    osstream<<"AC curent at mt-hvdc:"<<mtdc.get_compound_device_name()<<" at VSC side "<<vsc.get_converter_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                            <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    */

                    if(not detailed_log_enabled)
                        ;
                    else
                    {
                        I  = vsc.get_converter_dynamic_ac_current_in_pu_on_system_base();
                        I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                        osstream<<"AC curent at mt-hvdc:"<<mtdc.get_compound_device_name()<<" at VSC side "<<vsc.get_converter_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                                <<"Complex power = "<<SQRT3*psdb.get_bus_positive_sequence_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                        toolkit.show_information_with_leading_time_stamp(osstream);

                    }
                }
            }
        }
    }

    if(toolkit.is_detailed_log_enabled() and nmtdc != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::add_equivalent_devices_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    unsigned int nedevice = e_devices.size();

    //The following codes should not be parallelized. If more than one device is connecting to the same bus, unintended sum of I_mismatch[internal_bus] will occur.
    for(unsigned int i=0; i<nedevice; ++i)
    {
        EQUIVALENT_DEVICE& e_device = *(e_devices[i]);
        if(e_device.get_status() == true)
        {
            unsigned int physical_bus = e_device.get_equivalent_device_bus();

            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            complex<double> S = e_device.get_total_equivalent_power_as_load_in_MVA()*one_over_sbase;

            complex<double> V = get_bus_complex_voltage_in_pu_with_internal_bus_number(internal_bus);

            I_mismatch[internal_bus] -= conj(S/V);
        }
    }

    if(toolkit.is_detailed_log_enabled() and nedevice != 0)
        check_abnormal_current_mismatch_after_calling_function(__FUNCTION__);
}

void DYNAMICS_SIMULATOR::check_abnormal_current_mismatch_after_calling_function(string f)
{
    STEPS& toolkit = get_toolkit();
    GREATEST_POWER_CURRENT_MISMATCH_STRUCT greatest_mismatch_struct = get_max_current_mismatch_struct();
    double max_current_mismatch_pu = greatest_mismatch_struct.greatest_current_mismatch_in_pu;
    unsigned int max_mismatch_bus = greatest_mismatch_struct.bus_with_greatest_current_mismatch;
    if(max_current_mismatch_pu>1e6)
    {
        ostringstream osstream;
        osstream<<"Abnormal bus current mismatch (>10^6 pu) is deteced: "<<max_current_mismatch_pu<<" at bus "<<max_mismatch_bus<<"["
                <<toolkit.get_power_system_database().bus_number2bus_name(max_mismatch_bus)<<" after DYNAMIC_SIMULATOR::"<<f<<"() at time "<<TIME;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

bool DYNAMICS_SIMULATOR::is_converged()
{
    calculate_bus_power_mismatch_in_MVA();

    double smax = get_max_power_mismatch_struct().greatest_power_mismatch_in_MVA;

    double s_allowed = get_allowed_max_power_imbalance_in_MVA();

    return (smax>s_allowed)? false : true;
}

void DYNAMICS_SIMULATOR:: calculate_bus_power_mismatch_in_MVA()
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();

    S_mismatch = I_mismatch;

    unsigned int n = I_mismatch.size();
    complex<double> V;
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i= 0; i<n; ++i)
    {
        //complex<double> V = ;
        S_mismatch[i] = get_bus_complex_voltage_in_pu_with_internal_bus_number(i)*conj(S_mismatch[i])*sbase;
    }
}

GREATEST_POWER_CURRENT_MISMATCH_STRUCT DYNAMICS_SIMULATOR::get_max_power_mismatch_struct()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double smax2 = 0.0;
    unsigned int smax_bus = 0;
    unsigned int n = S_mismatch.size();
    double s2 = 0.0;
    for(unsigned int i=0; i!=n; ++i)
    {
        s2 = steps_complex_abs_square(S_mismatch[i]);
        if(s2>smax2)
        {
            smax2 = s2;
            smax_bus = i;
        }
    }
    double smax = steps_sqrt(smax2);
    smax_bus = network_matrix.get_physical_bus_number_of_internal_bus(smax_bus);
    GREATEST_POWER_CURRENT_MISMATCH_STRUCT s_mismatch;
    s_mismatch.greatest_power_mismatch_in_MVA = smax;
    s_mismatch.bus_with_greatest_power_mismatch = smax_bus;

    return s_mismatch;
}

GREATEST_POWER_CURRENT_MISMATCH_STRUCT DYNAMICS_SIMULATOR::get_max_current_mismatch_struct()
{
    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double Imax2 = 0.0;
    unsigned int Imax_bus = 0, Imax_internal_bus = 0;
    unsigned int n = I_mismatch.size();
    double I2 = 0.0, J2 = 0.0;
    unsigned int i=0;
    while(true)
    {
        I2 = steps_complex_abs_square(I_mismatch[i]);
        J2 = steps_complex_abs_square(I_mismatch[i+1]);
        if(I2>Imax2)
        {
            Imax2 = I2;
            Imax_internal_bus = i;
        }
        if(J2>Imax2)
        {
            Imax2 = J2;
            Imax_internal_bus = i+1;
        }
        i += 2;
        if(i+1<n)
            continue;
        else
        {
            for(;i<n;++i)
            {
                double I2 = steps_complex_abs_square(I_mismatch[i]);
                if(I2>Imax2)
                {
                    Imax2 = I2;
                    Imax_internal_bus = i;
                }
            }
            break;
        }
    }
    double Imax = steps_sqrt(Imax2);
    Imax_bus = network_matrix.get_physical_bus_number_of_internal_bus(Imax_internal_bus);
    GREATEST_POWER_CURRENT_MISMATCH_STRUCT i_mismatch;
    i_mismatch.greatest_current_mismatch_in_pu = Imax;
    i_mismatch.bus_with_greatest_current_mismatch = Imax_bus;

    if(not toolkit.is_detailed_log_enabled())
        return i_mismatch;
    else
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        ostringstream osstream;
        osstream<<"Maximum current mismatch at bus "<<Imax_bus<<"("<<psdb.bus_number2bus_name(Imax_bus)<<"): "<<Imax;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return i_mismatch;
    }
}


void DYNAMICS_SIMULATOR::build_bus_current_mismatch_vector()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = I_mismatch.size();
    I_vec.resize(n*2, 0.0);

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        I_vec[i] = I_mismatch[i].imag();
        I_vec[i+n] = I_mismatch[i].real();
    }
}


void DYNAMICS_SIMULATOR::update_bus_voltage()
{
    STEPS& toolkit = get_toolkit();
    try_to_limit_delta_v_within_change_threshold();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    unsigned int n = delta_V.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_thread_number());
        #pragma omp parallel for schedule(static)
        //#pragma omp parallel for num_threads(2)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(unsigned int i=0; i<n; ++i)
    {
        BUS& bus = *(in_service_buses[i]);

        unsigned int ex_bus = bus.get_bus_number();
        unsigned int in_bus = network.get_internal_bus_number_of_physical_bus(ex_bus);

        complex<double> V0 = bus.get_positive_sequence_complex_voltage_in_pu();
        complex<double> V = V0+delta_V[in_bus]*alpha;
        internal_bus_complex_voltage_in_pu[in_bus] = V;

		double vmag_new = steps_fast_complex_abs(V);
        //vmag_new = (vmag_new<3.0)?vmag_new:3.0;
        bus.set_positive_sequence_voltage_in_pu(vmag_new);

		double x = V.real(), y = V.imag();
		double x0 = V0.real(), y0 = V0.imag();
		complex<double> z(x*x0 + y*y0, x0*y - y0*x);
        double delta_vang = steps_fast_complex_arg(z);
        double vang0 = bus.get_positive_sequence_angle_in_rad();
        double vang_new = vang0+delta_vang*0.5;

        bus.set_positive_sequence_angle_in_rad(vang_new, V/vmag_new);
    }
    /*
    for(unsigned int i=0; i<n; ++i)
    {
        BUS& bus = *(internal_bus_pointers[i]);
        double vang0 = bus.get_positive_sequence_angle_in_rad();
        complex<double> V0 = bus.get_positive_sequence_complex_voltage_in_pu();

        //complex<double> delta_v = complex<double>(delta_V[i], delta_V[i+n]); previous version for real matrix
        //complex<double> V = V0-delta_v*alpha;  previous version for real matrix

        complex<double> delta_v = delta_V[i];
        complex<double> V = V0+delta_v*alpha;

		double vmag_new = steps_fast_complex_abs(V);

		double x = V.real(), y = V.imag();
		double x0 = V0.real(), y0 = V0.imag();
		complex<double> z(x*x0 + y * y0, x0*y - y0 * x);
        double delta_vang = steps_fast_complex_arg(z);
        double vang_new = vang0+delta_vang;

        if(vmag_new<0.0)
            vmag_new = 0.0;

        if(vmag_new>3.0)
            vmag_new = 3.0;


        bus.set_positive_sequence_voltage_in_pu(vmag_new);
        bus.set_positive_sequence_angle_in_rad(vang_new);

        internal_bus_complex_voltage_in_pu[i] = bus.get_positive_sequence_complex_voltage_in_pu();
    }
    */
}

void DYNAMICS_SIMULATOR::try_to_limit_delta_v_within_change_threshold()
{
    STEPS& toolkit = get_toolkit();
    double v_change_threshold = get_voltage_change_threshold_when_solving_network_in_pu();
    if(fabs(v_change_threshold-INFINITE_THRESHOLD)<FLOAT_EPSILON)
        return;
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    unsigned int n = delta_V.size();
    double max_delta_v = 0.0;
    unsigned int max_delta_bus = 0;
    for(unsigned int i=0; i<n; ++i)
    {
        double dv = fabs(delta_V[i]);
        if(dv>max_delta_v)
        {
            max_delta_v = dv;
            max_delta_bus = i;
        }
    }
    if(max_delta_v>v_change_threshold)
    {
        ostringstream osstream;
        unsigned int bus = network.get_physical_bus_number_of_internal_bus(max_delta_bus);
        osstream<<"Maximum voltage change found at "<<psdb.get_bus_extended_name(bus)<<": "<<max_delta_v<<" pu"<<endl
                <<"It will be limited to "<<v_change_threshold<<" pu";
        toolkit.show_information_with_leading_time_stamp(osstream);

        double scale = v_change_threshold/max_delta_v;
        for(unsigned int i=0; i<n; ++i)
            delta_V[i] *= scale;
    }
}

void DYNAMICS_SIMULATOR::build_jacobian()
{
    STEPS& toolkit = get_toolkit();
    clock_t tstart = clock();
    build_basic_jacobian_with_Y_matrix();

    update_jacobian_with_load_models();

    clock_t tfinished = clock();
    ostringstream osstream;
    osstream<<"Successful Jacobian build for dynamic simulation takes "<<(1000.0/double(CLOCKS_PER_SEC))*double(tfinished-tstart)<<" ms\n";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void DYNAMICS_SIMULATOR::build_basic_jacobian_with_Y_matrix()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    jacobian.clear();
    STEPS_COMPLEX_SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_Y_matrix();

    jacobian = Y;
    jacobian.compress_and_merge_duplicate_entries();
    return;

    unsigned int nbus = Y.get_matrix_size();
    complex<double> y;
    double g, b;
    unsigned int row, col;
    unsigned int starting_index=0, ending_index;
    for(unsigned int i=0; i!=nbus; ++i)
    {
        ending_index = Y.get_starting_index_of_column(i+1);
        col = i;
        for(unsigned int k=starting_index; k!=ending_index; ++k)
        {
            y = Y.get_entry_value(k);
            g = y.real();
            b = y.imag();
            row = Y.get_row_number_of_entry_index(k);
            jacobian.add_entry(row,col,-b);
            jacobian.add_entry(row,col+nbus,-g);
            jacobian.add_entry(row+nbus,col,-g);
            jacobian.add_entry(row+nbus,col+nbus,b);
        }
        starting_index = ending_index;
    }
    jacobian.compress_and_merge_duplicate_entries();
}

void DYNAMICS_SIMULATOR::update_jacobian_with_load_models()
{
    STEPS& toolkit = get_toolkit();
    clock_t tstart = clock();
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    unsigned int nload = loads.size();
    for(unsigned int i=0; i<nload; ++i)
    {
        LOAD& load = *(loads[i]);
        if(load.get_status()==true)
        {
            unsigned int physical_bus = load.get_load_bus();
            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            unsigned int k = jacobian.get_entry_index(internal_bus, internal_bus);
            complex<double> s0 = jacobian.get_entry_value(k);

            complex<double> s = load.get_derivative_of_load_current_wrt_voltage_on_SBASE();
            jacobian.change_entry_value(k, s0-s);
        }
    }
    clock_t tfinished = clock();
    ostringstream osstream;
    osstream<<"Successful Jacobian update with load model for dynamic simulation takes "<<(1000.0/double(CLOCKS_PER_SEC))*double(tfinished-tstart)<<" ms\n";
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void DYNAMICS_SIMULATOR::change_dynamic_simulator_time_step_in_s(double delt)
{
    STEPS& toolkit = get_toolkit();
    if(fabs(DELT-delt)>FLOAT_EPSILON)
    {
        ostringstream osstream;
        osstream<<"At time "<<get_dynamic_simulation_time_in_s()<<" s, system dynamic simulation time step is changed from "<<DELT<<" s to "<<delt<<" s.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        set_dynamic_simulation_time_step_in_s(delt);

        update_with_event(TIME_STEP_CHANGE_EVENT);
    }
}


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_set(unsigned int bus_number, const FAULT& fault)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus_number);
    BUS& bus = *busptr;
    if(bus.get_bus_type()==OUT_OF_SERVICE)
        return;

    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = bus.get_positive_sequence_voltage_in_pu();
    double vbase = bus.get_base_voltage_in_kV();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double zbase = vbase*vbase*one_over_sbase;
    double fault_x = -zbase/fault_b;

    if(fault_x<1)
        bus.set_positive_sequence_voltage_in_pu(0.05);
    else
    {
        if(fault_x<5)
            bus.set_positive_sequence_voltage_in_pu(0.1);
        else
        {
            if(fault_x<10)
                bus.set_positive_sequence_voltage_in_pu(0.2);
            else
            {
                if(fault_x<100)
                    bus.set_positive_sequence_voltage_in_pu(0.5);
                else
                    bus.set_positive_sequence_voltage_in_pu(current_voltage-0.1);
            }
        }
    }

    NETWORK_MATRIX& network = get_network_matrix();
    unsigned int internal_bus = network.get_internal_bus_number_of_physical_bus(bus_number);
    internal_bus_complex_voltage_in_pu[internal_bus] = bus.get_positive_sequence_complex_voltage_in_pu();
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_cleared(unsigned int bus_number, const FAULT& fault)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus_number);
    BUS& bus = *busptr;
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = bus.get_positive_sequence_voltage_in_pu();
    bus.set_positive_sequence_voltage_in_pu(0.8);;
    return;

    if(fault_b<-2e8)
        bus.set_positive_sequence_voltage_in_pu(current_voltage+0.95);
    else
    {
        if(fault_b<-2e7)
            bus.set_positive_sequence_voltage_in_pu(current_voltage+0.9);
        else
        {
            if(fault_b<-2e6)
                bus.set_positive_sequence_voltage_in_pu(current_voltage+0.8);
            else
            {
                if(fault_b<-2e5)
                    bus.set_positive_sequence_voltage_in_pu(current_voltage+0.7);
                else
                {
                    if(fault_b<-2e4)
                        bus.set_positive_sequence_voltage_in_pu(current_voltage+0.6);
                    else
                        bus.set_positive_sequence_voltage_in_pu(current_voltage+0.1);
                }
            }
        }
    }
    NETWORK_MATRIX& network = get_network_matrix();
    unsigned int internal_bus = network.get_internal_bus_number_of_physical_bus(bus_number);
    internal_bus_complex_voltage_in_pu[internal_bus] = bus.get_positive_sequence_complex_voltage_in_pu();
}


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_ac_line_fault_set(const DEVICE_ID& did, unsigned int side_bus, double location, const FAULT& fault)
{
    STEPS& toolkit = get_toolkit();
    if(location >=0.0 and location <= 1.0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* line_ptr = psdb.get_ac_line(did);
        AC_LINE& line = *line_ptr;
        if(line.is_connected_to_bus(side_bus))
        {
            if(location<=0.5)
            {
                unsigned int ibus = line.get_sending_side_bus();
                unsigned int jbus = line.get_receiving_side_bus();

                unsigned int this_bus = side_bus;
                unsigned int other_bus = (ibus==side_bus? jbus:ibus);

                double this_location = location;
                double other_location = 1.0-location;

                FAULT this_fault = fault;
                this_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(this_location*2.0));
                FAULT other_fault = fault;
                other_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(other_location*2.0));
                guess_bus_voltage_with_bus_fault_set(this_bus, this_fault);
                guess_bus_voltage_with_bus_fault_set(other_bus, other_fault);
            }
            else
            {
                unsigned int ibus = line.get_sending_side_bus();
                unsigned int jbus = line.get_receiving_side_bus();

                location = 1.0-location;

                side_bus = (ibus==side_bus? ibus:jbus);

                guess_bus_voltage_with_ac_line_fault_set(did, side_bus, location, fault);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_ac_line_fault_cleared(const DEVICE_ID& did, unsigned int side_bus, double location, const FAULT& fault)
{
    STEPS& toolkit = get_toolkit();
    if(location >= 0.0 and location <= 1.0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* line_ptr = psdb.get_ac_line(did);
        AC_LINE& line = *line_ptr;
        if(line.is_connected_to_bus(side_bus))
        {
            if(location<=0.5)
            {
                unsigned int ibus = line.get_sending_side_bus();
                unsigned int jbus = line.get_receiving_side_bus();

                unsigned int this_bus = side_bus;
                unsigned int other_bus = (ibus==side_bus? jbus:ibus);

                double this_location = location;
                double other_location = 1.0-location;

                FAULT this_fault = fault;
                this_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(this_location*2.0));
                FAULT other_fault = fault;
                other_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(other_location*2.0));
                guess_bus_voltage_with_bus_fault_cleared(this_bus, this_fault);
                guess_bus_voltage_with_bus_fault_cleared(other_bus, other_fault);
            }
            else
            {
                unsigned int ibus = line.get_sending_side_bus();
                unsigned int jbus = line.get_receiving_side_bus();

                location = 1.0-location;

                side_bus = (ibus==side_bus? ibus:jbus);

                guess_bus_voltage_with_ac_line_fault_cleared(did, side_bus, location, fault);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::update_generators_in_islands()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    vector< vector<unsigned int> > islands = network_matrix.get_islands_with_physical_bus_number();
    if(islands.size()==generators_in_islands.size()) // won't update if islands doesn't change
        return;

    generators_in_islands.clear();

    unsigned int nislands = islands.size();
    for(unsigned int i=0; i!=nislands; ++i)
    {
        vector<unsigned int> island = islands[i];

        vector<GENERATOR*> generators_in_island;
        vector<GENERATOR*> generators_at_bus;

        unsigned int nisland = island.size();
        for(unsigned int j=0; j!=nisland; ++j)
        {
            unsigned int bus = island[j];
            generators_at_bus = psdb.get_generators_connecting_to_bus(bus);
            unsigned int n = generators_at_bus.size();
            for(unsigned int k=0; k!=n; ++k)
            {
                GENERATOR* generator = generators_at_bus[k];
                generators_in_island.push_back(generator);
            }
        }

        generators_in_islands.push_back(generators_in_island);
    }
}

bool DYNAMICS_SIMULATOR::is_system_angular_stable() const
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    bool system_is_stable = true;
    unsigned int nislands = generators_in_islands.size();
    vector<double> angles;
    for(unsigned int island=0; island!=nislands; island++)
    {
        vector<GENERATOR*> generators_in_island = generators_in_islands[island];
        unsigned int n = generators_in_island.size();
        angles.resize(n, 0.0);
        angles.clear();

        for(unsigned int i=0; i!=n; ++i)
        {
            GENERATOR& generator = *(generators_in_island[i]);
            if(generator.get_status()==true)
            {
                SYNC_GENERATOR_MODEL* genmodel_ptr = generator.get_sync_generator_model();
                SYNC_GENERATOR_MODEL& genmodel = *genmodel_ptr;
                angles.push_back(genmodel.get_rotor_angle_in_deg());
            }
        }
        double angle_max = 0.0, angle_min = 0.0;
        unsigned int nangle = angles.size();
        if(nangle>0)
        {
            angle_max = angles[0];
            angle_min = angles[0];
            for(unsigned int i=1; i!=nangle; ++i)
            {
                if(angles[i]>angle_max)
                    angle_max = angles[i];
            }
            for(unsigned int i=1; i!=nangle; ++i)
            {
                if(angles[i]<angle_min)
                    angle_min = angles[i];
            }
        }
        double angle_difference = angle_max - angle_min;
        double scaled_angle_difference = rad2deg(round_angle_in_rad_to_PI(deg2rad(angle_difference)));

        if(not detailed_log_enabled)
            ;
        else
        {
            osstream<<"angle difference in island "<<island<<" is "<<scaled_angle_difference<<" deg at time "<<TIME<<"s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        if(angle_difference>get_rotor_angle_stability_threshold_in_deg())
        {
            system_is_stable = false;
            osstream<<"The following island is detected to be unstable at time "<<TIME<<" s. Maximum angle difference is :"<<angle_difference<<" deg (a.k.a. "<<scaled_angle_difference<<" deg)"<<endl
                   <<"Generator          Rotor angle in deg"<<endl;

            unsigned int n = generators_in_island.size();
            for(unsigned int i=0; i!=n; ++i)
            {
                GENERATOR& generator = *(generators_in_island[i]);
                if(generator.get_status()==true)
                {
                    SYNC_GENERATOR_MODEL* genmodel_ptr = generator.get_sync_generator_model();
                    SYNC_GENERATOR_MODEL& genmodel = *genmodel_ptr;
                    osstream<<generator.get_compound_device_name()<<"  "<<genmodel.get_rotor_angle_in_deg()<<endl;
                }
            }
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
    return system_is_stable;
}

void DYNAMICS_SIMULATOR::set_bus_fault(unsigned int bus_number, const complex<double>& fault_shunt)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus_number);
    if(busptr!=NULL)
    {
        BUS& bus = *busptr;
        string busname = bus.get_bus_name();
        if(steps_fast_complex_abs(fault_shunt)>DOUBLE_EPSILON)
        {
            FAULT fault;
            fault.set_fault_type(THREE_PHASES_FAULT);
            fault.set_fault_shunt_in_pu(fault_shunt);

            if(bus.get_bus_type()==OUT_OF_SERVICE)
            {
                osstream<<"No "<<fault.get_fault_type_string()<<" will be set for bus "<<bus_number<<"["<<busname<<"] at time "<<TIME<<" s since it is out of service."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
            osstream<<fault.get_fault_type_string()<<" will be set for bus "<<bus_number<<"["<<busname<<"] at time "<<TIME<<" s."<<endl
                   <<"Fault shunt is"<<fault_shunt<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            bus.set_fault(fault);
            guess_bus_voltage_with_bus_fault_set(bus_number, fault);

            //network_matrix.build_dynamic_network_Y_matrix();
            //build_jacobian();
        }
        else
        {
            osstream<<"Zero fault shunt is given for bus "<<bus_number<<"["<<busname<<"]."<<endl
                   <<"No fault will be set for bus "<<bus_number<<"["<<busname<<"] at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Bus "<<bus_number<<" does not exist in power system database."<<endl
               <<"No bus fault will be set at time "<<TIME<<" s.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_bus_fault(unsigned int bus_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    BUS* busptr = psdb.get_bus(bus_number);
    if(busptr!=NULL)
    {
        BUS& bus = *busptr;
        string busname = bus.get_bus_name();
        ostringstream osstream;

        if(bus.get_bus_type()==OUT_OF_SERVICE)
        {
            osstream<<"No fault will be cleared for bus "<<bus_number<<"["<<busname<<"] at time "<<TIME<<" s since it is out of service."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }

        osstream<<"Fault at bus "<<bus_number<<"["<<busname<<"] will be cleared at time "<<TIME<<" s.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        FAULT fault = bus.get_fault();
        bus.clear_fault();
        guess_bus_voltage_with_bus_fault_cleared(bus_number, fault);

        //network_matrix.build_dynamic_network_Y_matrix();
        //build_jacobian();
    }
}

void DYNAMICS_SIMULATOR::trip_bus(unsigned int bus_number)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ostringstream osstream;

    BUS* busptr = psdb.get_bus(bus_number);
    if(busptr!=NULL)
    {
        BUS& bus = *busptr;
        string busname = bus.get_bus_name();
        if(bus.get_bus_type()!=OUT_OF_SERVICE)
        {

            osstream<<"Bus "<<bus_number<<"["<<busname<<"] will be tripped at time "<<TIME<<" s. Devices connecting to bus "<<bus_number<<" will also be tripped.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            psdb.trip_bus(bus_number);

            in_service_buses = psdb.get_all_in_service_buses();

            optimize_network_ordering();
            //network_matrix.build_dynamic_network_Y_matrix();
            //build_jacobian();
        }
        else
        {
            osstream<<"Bus "<<bus_number<<"["<<busname<<"] will not be tripped at time "<<TIME<<" s since it is out of service."<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void DYNAMICS_SIMULATOR::trip_buses(const vector<unsigned int>& buses)
{
    unsigned int n = buses.size();
    for(unsigned int i=0; i!=n; ++i)
        trip_bus(buses[i]);
}

void DYNAMICS_SIMULATOR::set_ac_line_fault(const DEVICE_ID& line_id, unsigned int side_bus, double location, const complex<double>& fault_shunt)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(line_id.get_device_type()==STEPS_AC_LINE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* lineptr = psdb.get_ac_line(line_id);
        if(lineptr!=NULL)
        {
            AC_LINE& line = *lineptr;
            string ibusname = psdb.bus_number2bus_name(line.get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(line.get_receiving_side_bus());
            if(line.is_connected_to_bus(side_bus))
            {
                if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==false)
                {
                    osstream<<"No fault will be set for "<<line.get_compound_device_name()<<" at time "<<TIME<<" s since it is out of service."<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
                if(location>=0.0 and location<=1.0)
                {
                    if(steps_fast_complex_abs(fault_shunt)>DOUBLE_EPSILON)
                    {
                        FAULT fault;
                        fault.set_fault_type(THREE_PHASES_FAULT);
                        fault.set_fault_shunt_in_pu(fault_shunt);

                        osstream<<fault.get_fault_type_string()<<" will be set for "<<line.get_compound_device_name()<<" at time "<<TIME<<" s."<<endl
                               <<"Fault shunt is "<<fault_shunt<<" pu at location "<<location<<" to bus "<<side_bus;
                        toolkit.show_information_with_leading_time_stamp(osstream);


                        line.set_fault(side_bus,location, fault);
                        guess_bus_voltage_with_ac_line_fault_set(line_id, side_bus, location, fault);

                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();
                    }
                    else
                    {
                        osstream<<"Zero fault shunt is given for "<<line.get_compound_device_name()<<"."<<endl
                               <<"No fault will be set for "<<line.get_compound_device_name()<<" at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<line.get_compound_device_name()<<"."<<endl
                           <<"No fault will be set at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<line.get_compound_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be set at location "<<location<<" to bus "<<side_bus<<" at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No fault will be set at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for setting line fault."<<endl
               <<"No line fault will be set at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_ac_line_fault(const DEVICE_ID& line_id, unsigned int side_bus, double location)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(line_id.get_device_type()==STEPS_AC_LINE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* lineptr = psdb.get_ac_line(line_id);
        if(lineptr!=NULL)
        {
            AC_LINE& line = *lineptr;
            string ibusname = psdb.bus_number2bus_name(line.get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(line.get_receiving_side_bus());
            if(line.is_connected_to_bus(side_bus))
            {
                if(line.get_sending_side_breaker_status()==false and line.get_receiving_side_breaker_status()==false)
                {
                    osstream<<"No fault will be cleared for "<<line.get_compound_device_name()<<" at time "<<TIME<<" s since it is out of service."<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
                if(location>=0.0 and location<=1.0)
                {
                    osstream<<"Fault at location "<<location<<" to bus "<<side_bus<<" will be cleared for "
                           <<line.get_compound_device_name()<<" at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    FAULT fault = line.get_fault_at_location(side_bus, location);
                    line.clear_fault_at_location(side_bus, location);
                    guess_bus_voltage_with_ac_line_fault_cleared(line_id, side_bus, location, fault);

                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<line.get_compound_device_name()<<"."<<endl
                           <<"No fault will be cleared at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
            }
            else
            {
                osstream<<line.get_compound_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be cleared at location "<<location<<" to bus "<<side_bus<<" at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No fault will be cleared at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for clearing line fault."<<endl
               <<"No line fault will be cleared at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_ac_line(const DEVICE_ID& line_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(line_id.get_device_type()==STEPS_AC_LINE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* lineptr = psdb.get_ac_line(line_id);
        if(lineptr!=NULL)
        {
            AC_LINE& line = *lineptr;
            string ibusname = psdb.bus_number2bus_name(line.get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(line.get_receiving_side_bus());
            if(line.get_sending_side_breaker_status()==true or line.get_receiving_side_breaker_status()==true)
            {
                osstream<<line.get_compound_device_name()<<" is tripped by taking the following actions at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

                if(line.get_sending_side_breaker_status()==true)
                    trip_ac_line_breaker(line_id, line.get_sending_side_bus());

                if(line.get_receiving_side_breaker_status()==true)
                    trip_ac_line_breaker(line_id, line.get_receiving_side_bus());
            }
            else
            {
                osstream<<line.get_compound_device_name()<<" will not be tripped at time "<<TIME<<" s since it is out of service."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No line will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for tripping line."<<endl
               <<"No line will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_ac_line_breaker(const DEVICE_ID& line_id, unsigned int side_bus)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(line_id.get_device_type()==STEPS_AC_LINE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* lineptr = psdb.get_ac_line(line_id);
        if(lineptr!=NULL)
        {
            AC_LINE& line = *lineptr;
            string ibusname = psdb.bus_number2bus_name(line.get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(line.get_receiving_side_bus());
            if(line.get_sending_side_bus()==side_bus)
            {
                if(line.get_sending_side_breaker_status()==true)
                {
                    line.set_sending_side_breaker_status(false);
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();

                    osstream<<line.get_compound_device_name()<<" breaker at sending side (bus "<<line.get_sending_side_bus()<<") is tripped at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(line.get_receiving_side_bus()==side_bus)
                {
                    if(line.get_receiving_side_breaker_status()==true)
                    {
                        line.set_receiving_side_breaker_status(false);
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();

                        osstream<<line.get_compound_device_name()<<" breaker at receiving side (bus "<<line.get_receiving_side_bus()<<") is tripped at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No line breaker will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for tripping line breaker."<<endl
               <<"No line breaker will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_ac_line(const DEVICE_ID& line_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(line_id.get_device_type()==STEPS_AC_LINE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* lineptr = psdb.get_ac_line(line_id);
        if(lineptr!=NULL)
        {
            AC_LINE& line = *lineptr;
            string ibusname = psdb.bus_number2bus_name(line.get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(line.get_receiving_side_bus());
            if(line.get_sending_side_breaker_status()==false or line.get_receiving_side_breaker_status()==false)
            {
                osstream<<line.get_compound_device_name()<<" is closed by taking the following actions at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

                if(line.get_sending_side_breaker_status()==false)
                    close_ac_line_breaker(line_id, line.get_sending_side_bus());

                if(line.get_receiving_side_breaker_status()==false)
                    close_ac_line_breaker(line_id, line.get_receiving_side_bus());
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No line will be closed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for closing line."<<endl
               <<"No line will be closed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_ac_line_breaker(const DEVICE_ID& line_id, unsigned int side_bus)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(line_id.get_device_type()==STEPS_AC_LINE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        AC_LINE* lineptr = psdb.get_ac_line(line_id);
        if(lineptr!=NULL)
        {
            AC_LINE& line = *lineptr;
            string ibusname = psdb.bus_number2bus_name(line.get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(line.get_receiving_side_bus());
            if(line.get_sending_side_bus()==side_bus)
            {
                if(line.get_sending_side_breaker_status()==false)
                {
                    line.set_sending_side_breaker_status(true);
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                    osstream<<line.get_compound_device_name()<<" breaker at sending side (bus "<<line.get_sending_side_bus()<<") is closed at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(line.get_receiving_side_bus()==side_bus)
                {
                    if(line.get_receiving_side_breaker_status()==false)
                    {
                        line.set_receiving_side_breaker_status(true);
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();
                        osstream<<line.get_compound_device_name()<<" breaker at receiving side (bus "<<line.get_sending_side_bus()<<") is closed at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No line breaker will be closed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for closing line breaker"<<endl
               <<"No line breaker will be closed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_transformer(const DEVICE_ID& trans_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(trans_id.get_device_type()==STEPS_TRANSFORMER)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            TRANSFORMER& trans = *transptr;
            string ibusname = psdb.bus_number2bus_name(trans.get_winding_bus(PRIMARY_SIDE));
            string jbusname = psdb.bus_number2bus_name(trans.get_winding_bus(SECONDARY_SIDE));
            string kbusname = "";
            if(trans.is_three_winding_transformer())
                kbusname = psdb.bus_number2bus_name(trans.get_winding_bus(TERTIARY_SIDE));

            if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true or trans.get_winding_breaker_status(SECONDARY_SIDE)==true or
               (trans.is_three_winding_transformer() and trans.get_winding_breaker_status(TERTIARY_SIDE)==true) )
            {
                osstream<<trans.get_compound_device_name()<<" is tripped by taking the following actions at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

                if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true)
                    trip_transformer_breaker(trans_id, trans.get_winding_bus(PRIMARY_SIDE));

                if(trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
                    trip_transformer_breaker(trans_id, trans.get_winding_bus(SECONDARY_SIDE));

                if(trans.is_three_winding_transformer() and trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
                    trip_transformer_breaker(trans_id, trans.get_winding_bus(TERTIARY_SIDE));
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No transformer will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for tripping transformer."<<endl
               <<"No transformer will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_transformer_breaker(const DEVICE_ID& trans_id, unsigned int side_bus)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(trans_id.get_device_type()==STEPS_TRANSFORMER)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            TRANSFORMER& trans = *transptr;
            string ibusname = psdb.bus_number2bus_name(trans.get_winding_bus(PRIMARY_SIDE));
            string jbusname = psdb.bus_number2bus_name(trans.get_winding_bus(SECONDARY_SIDE));
            string kbusname = "";
            if(trans.is_three_winding_transformer())
                kbusname = psdb.bus_number2bus_name(trans.get_winding_bus(TERTIARY_SIDE));

            if(trans.get_winding_bus(PRIMARY_SIDE)==side_bus)
            {
                if(trans.get_winding_breaker_status(PRIMARY_SIDE)==true)
                {
                    trans.set_winding_breaker_status(PRIMARY_SIDE, false);
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();

                    osstream<<trans.get_compound_device_name()<<" breaker at primary side (bus "<<side_bus<<") is tripped at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(trans.get_winding_bus(SECONDARY_SIDE)==side_bus)
                {
                    if(trans.get_winding_breaker_status(SECONDARY_SIDE)==true)
                    {
                        trans.set_winding_breaker_status(SECONDARY_SIDE, false);
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();

                        osstream<<trans.get_compound_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is tripped at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    if(trans.is_three_winding_transformer() and trans.get_winding_bus(TERTIARY_SIDE)==side_bus)
                    {
                        if(trans.get_winding_breaker_status(TERTIARY_SIDE)==true)
                        {
                            trans.set_winding_breaker_status(TERTIARY_SIDE, false);
                            //network_matrix.build_dynamic_network_Y_matrix();
                            //build_jacobian();

                            osstream<<trans.get_compound_device_name()<<" breaker at tertiary side (bus "<<side_bus<<") is tripped at time "<<TIME<<" s.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }

                }
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No transformer breaker will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for tripping transformer breaker."<<endl
               <<"No transformer breaker will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void DYNAMICS_SIMULATOR::close_transformer(const DEVICE_ID& trans_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(trans_id.get_device_type()==STEPS_TRANSFORMER)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            TRANSFORMER& trans = *transptr;
            string ibusname = psdb.bus_number2bus_name(trans.get_winding_bus(PRIMARY_SIDE));
            string jbusname = psdb.bus_number2bus_name(trans.get_winding_bus(SECONDARY_SIDE));
            string kbusname = "";
            if(trans.is_three_winding_transformer())
                kbusname = psdb.bus_number2bus_name(trans.get_winding_bus(TERTIARY_SIDE));

            if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false or trans.get_winding_breaker_status(SECONDARY_SIDE)==false or
               (trans.is_three_winding_transformer() and trans.get_winding_breaker_status(TERTIARY_SIDE)==false))
            {
                osstream<<trans.get_compound_device_name()<<" is closed by taking the following actions at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

                if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false)
                    close_transformer_breaker(trans_id, trans.get_winding_bus(PRIMARY_SIDE));

                if(trans.get_winding_breaker_status(SECONDARY_SIDE)==false)
                    close_transformer_breaker(trans_id, trans.get_winding_bus(SECONDARY_SIDE));

                if(trans.is_three_winding_transformer() and trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
                    close_transformer_breaker(trans_id, trans.get_winding_bus(TERTIARY_SIDE));
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No transformer will be closed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for closing transformer."<<endl
               <<"No transformer will be closed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_transformer_breaker(const DEVICE_ID& trans_id, unsigned int side_bus)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(trans_id.get_device_type()==STEPS_TRANSFORMER)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            TRANSFORMER& trans = *transptr;
            string ibusname = psdb.bus_number2bus_name(trans.get_winding_bus(PRIMARY_SIDE));
            string jbusname = psdb.bus_number2bus_name(trans.get_winding_bus(SECONDARY_SIDE));
            string kbusname = "";
            if(trans.is_three_winding_transformer())
                kbusname = psdb.bus_number2bus_name(trans.get_winding_bus(TERTIARY_SIDE));

            if(trans.get_winding_bus(PRIMARY_SIDE)==side_bus)
            {
                if(trans.get_winding_breaker_status(PRIMARY_SIDE)==false)
                {
                    trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                    osstream<<trans.get_compound_device_name()<<" breaker at primary side (bus "<<side_bus<<") is closed at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(trans.get_winding_bus(SECONDARY_SIDE)==side_bus)
                {
                    if(trans.get_winding_breaker_status(SECONDARY_SIDE)==false)
                    {
                        trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();
                        osstream<<trans.get_compound_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    if(trans.is_three_winding_transformer() and trans.get_winding_bus(TERTIARY_SIDE)==side_bus)
                    {
                        if(trans.get_winding_breaker_status(TERTIARY_SIDE)==false)
                        {
                            trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                            //network_matrix.build_dynamic_network_Y_matrix();
                            //build_jacobian();
                            osstream<<trans.get_compound_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<TIME<<" s.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No transformer breaker will be closed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for closing transformer breaker"<<endl
               <<"No transformer breaker will be closed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_generator(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_GENERATOR)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        GENERATOR* generator_ptr = psdb.get_generator(gen_id);
        if(generator_ptr!=NULL)
        {
            GENERATOR& generator = *generator_ptr;
            string busname = psdb.bus_number2bus_name(generator.get_generator_bus());

            if(generator.get_status()==true)
            {
                generator.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<generator.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No generator will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for tripping generator."<<endl
               <<"No generator will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_generator(const DEVICE_ID& gen_id,double percent)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_GENERATOR)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        GENERATOR* generator_ptr = psdb.get_generator(gen_id);
        if(generator_ptr!=NULL)
        {
            GENERATOR& generator = *generator_ptr;
            string busname = psdb.bus_number2bus_name(generator.get_generator_bus());

            if(fabs(percent)>DOUBLE_EPSILON)
            {
                if(percent==1.0)
                {
                    osstream<<generator.get_compound_device_name()<<" is shed by "<<100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"Generator is to be tripped.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    trip_generator(gen_id);
                }
                else
                {
                    double mbase = generator.get_mbase_in_MVA();
                    osstream<<generator.get_compound_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"MBASE is changed from "<<mbase<<" MVA to ";

                    generator.set_mbase_in_MVA(mbase*(1.0-percent));
                    osstream<<generator.get_mbase_in_MVA()<<" MVA.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"Generator shed percent is 0.0 for "<<generator.get_compound_device_name()<<endl
                        <<"No generator will be shed at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No generator will be shed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for shedding generator."<<endl
                <<"No generator will be shed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::trip_wt_generator(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_WT_GENERATOR)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        WT_GENERATOR* generator_ptr = psdb.get_wt_generator(gen_id);
        if(generator_ptr!=NULL)
        {
            WT_GENERATOR& generator = *generator_ptr;
            string busname = psdb.bus_number2bus_name(generator.get_generator_bus());

            if(generator.get_status()==true)
            {
                generator.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<generator.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No wt generator will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a WT GENERATOR (it is a "<<gen_id.get_device_type()<<") for tripping wt generator."<<endl
                <<"No wt generator will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_wt_generator(const DEVICE_ID& gen_id,double percent)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_WT_GENERATOR)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        WT_GENERATOR* generator_ptr = psdb.get_wt_generator(gen_id);
        if(generator_ptr!=NULL)
        {
            WT_GENERATOR& generator = *generator_ptr;
            string busname = psdb.bus_number2bus_name(generator.get_generator_bus());

            if(fabs(percent)>DOUBLE_EPSILON)
            {
                if(percent==1.0)
                {
                    osstream<<generator.get_compound_device_name()<<" is shed by "<<100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"WT generator is to be tripped.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    trip_wt_generator(gen_id);
                }
                else
                {
                    double mbase = generator.get_mbase_in_MVA();
                    osstream<<generator.get_compound_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"MBASE is changed from "<<mbase<<" MVA to ";

                    generator.set_mbase_in_MVA(mbase*(1.0-percent));
                    osstream<<generator.get_mbase_in_MVA()<<" MVA.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"WT generator shed percent is 0.0 for "<<generator.get_compound_device_name()<<endl
                        <<"No WT generator will be shed at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No WT generator will be shed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a WT GENERATOR (it is a "<<gen_id.get_device_type()<<") for shedding WT generator."<<endl
                <<"No WT generator will be shed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_pv_unit(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_PV_UNIT)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        PV_UNIT* pv_unit_ptr = psdb.get_pv_unit(gen_id);
        if(pv_unit_ptr!=NULL)
        {
            PV_UNIT& pv_unit = *pv_unit_ptr;
            string busname = psdb.bus_number2bus_name(pv_unit.get_unit_bus());

            if(pv_unit.get_status()==true)
            {
                pv_unit.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<pv_unit.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No pv unit will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a PV UNIT (it is a "<<gen_id.get_device_type()<<") for tripping pv unit."<<endl
                <<"No pv unit will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_pv_unit(const DEVICE_ID& gen_id,double percent)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_PV_UNIT)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        PV_UNIT* pv_unit_ptr = psdb.get_pv_unit(gen_id);
        if(pv_unit_ptr!=NULL)
        {
            PV_UNIT& pv_unit = *pv_unit_ptr;
            string busname = psdb.bus_number2bus_name(pv_unit.get_unit_bus());

            if(fabs(percent)>DOUBLE_EPSILON)
            {
                if(percent==1.0)
                {
                    osstream<<pv_unit.get_compound_device_name()<<" is shed by "<<100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"PV unit is to be tripped.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    trip_pv_unit(gen_id);
                }
                else
                {
                    double mbase = pv_unit.get_mbase_in_MVA();
                    osstream<<pv_unit.get_compound_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"MBASE is changed from "<<mbase<<" MVA to ";

                    pv_unit.set_mbase_in_MVA(mbase*(1.0-percent));
                    osstream<<pv_unit.get_mbase_in_MVA()<<" MVA.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"PV unit shed percent is 0.0 for "<<pv_unit.get_compound_device_name()<<endl
                        <<"No PV unit will be shed at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No PV unit will be shed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a PV UNIT (it is a "<<gen_id.get_device_type()<<") for shedding PV unit."<<endl
                <<"No PV unit will be shed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_energy_storage(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_ENERGY_STORAGE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        ENERGY_STORAGE* energy_storage_ptr = psdb.get_energy_storage(gen_id);
        if(energy_storage_ptr!=NULL)
        {
            ENERGY_STORAGE& energy_storage = *energy_storage_ptr;
            string busname = psdb.bus_number2bus_name(energy_storage.get_energy_storage_bus());

            if(energy_storage.get_status()==true)
            {
                energy_storage.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<energy_storage.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No energy storage will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a ENERGY STORAGE (it is a "<<gen_id.get_device_type()<<") for tripping energy storage."<<endl
                <<"No energy storage will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_energy_storage(const DEVICE_ID& gen_id,double percent)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_ENERGY_STORAGE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        ENERGY_STORAGE* energy_storage_ptr = psdb.get_energy_storage(gen_id);
        if(energy_storage_ptr!=NULL)
        {
            ENERGY_STORAGE& energy_storage = *energy_storage_ptr;
            string busname = psdb.bus_number2bus_name(energy_storage.get_energy_storage_bus());

            if(fabs(percent)>DOUBLE_EPSILON)
            {
                if(percent==1.0)
                {
                    osstream<<energy_storage.get_compound_device_name()<<" is shed by "<<100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"Energy storage is to be tripped.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    trip_energy_storage(gen_id);
                }
                else
                {
                    double mbase = energy_storage.get_mbase_in_MVA();
                    osstream<<energy_storage.get_compound_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"MBASE is changed from "<<mbase<<" MVA to ";

                    energy_storage.set_mbase_in_MVA(mbase*(1.0-percent));
                    osstream<<energy_storage.get_mbase_in_MVA()<<" MVA.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"Energy storage shed percent is 0.0 for "<<energy_storage.get_compound_device_name()<<endl
                        <<"No energy storage will be shed at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No energy storage will be shed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a ENERGY STORAGE (it is a "<<gen_id.get_device_type()<<") for shedding energy storage."<<endl
                <<"No energy storage will be shed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_sv_source(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_SV_SOURCE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        SV_SOURCE* svs_ptr = psdb.get_sv_source(gen_id);
        if(svs_ptr!=NULL)
        {
            SV_SOURCE& svs = *svs_ptr;
            string busname = psdb.bus_number2bus_name(svs.get_sv_source_bus());

            if(svs.get_status()==true)
            {
                svs.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<svs.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No sv source will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a SV SOURCE (it is a "<<gen_id.get_device_type()<<") for tripping sv source."<<endl
                <<"No sv source will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_sv_source(const DEVICE_ID& gen_id,double percent)
{/*
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(gen_id.get_device_type()==STEPS_SV_SOURCE)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        SV_SOURCE* svs_ptr = psdb.get_sv_source(gen_id);
        if(svs_ptr!=NULL)
        {
            SV_SOURCE& svs = *svs_ptr;
            string busname = psdb.bus_number2bus_name(svs.get_sv_source_bus());

            if(fabs(percent)>DOUBLE_EPSILON)
            {
                if(percent==1.0)
                {
                    osstream<<svs.get_compound_device_name()<<" is shed by "<<100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"SV source is to be tripped.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    trip_sv_source(gen_id);
                }
                else
                {
                    double mbase = svs.get_mbase_in_MVA();
                    osstream<<svs.get_compound_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl
                            <<"MBASE is changed from "<<mbase<<" MVA to ";

                    svs.set_mbase_in_MVA(mbase*(1.0-percent));
                    osstream<<svs.get_mbase_in_MVA()<<" MVA.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<" SV source shed percent is 0.0 for "<<svs.get_compound_device_name()<<endl
                        <<"No sv source will be shed at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No sv source will be shed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a SV SOURCE (it is a "<<gen_id.get_device_type()<<") for shedding sv source."<<endl
                <<"No sv source will be shed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }*/
}

void DYNAMICS_SIMULATOR::trip_load(const DEVICE_ID& load_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(load_id.get_device_type()==STEPS_LOAD)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LOAD* load_ptr = psdb.get_load(load_id);
        if(load_ptr!=NULL)
        {
            LOAD& load = *load_ptr;
            string busname = psdb.bus_number2bus_name(load.get_load_bus());

            if(load.get_status()==true)
            {
                load.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<load.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No load will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for tripping load."<<endl
               <<"No load will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_load(const DEVICE_ID& load_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(load_id.get_device_type()==STEPS_LOAD)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LOAD* load_ptr = psdb.get_load(load_id);
        if(load_ptr!=NULL)
        {
            LOAD& load = *load_ptr;
            string busname = psdb.bus_number2bus_name(load.get_load_bus());

            if(load.get_status()==false)
            {
                load.set_status(true);
                LOAD_MODEL& load_model = *(load.get_load_model());
                load_model.initialize_to_start();
                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<load.get_compound_device_name()<<" is closed at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No load will be closed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for closing load."<<endl
               <<"No load will be closed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::scale_load(const DEVICE_ID& load_id, double percent)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(load_id.get_device_type()==STEPS_LOAD)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LOAD* load_ptr = psdb.get_load(load_id);
        if(load_ptr!=NULL)
        {
            LOAD& load = *load_ptr;
            string busname = psdb.bus_number2bus_name(load.get_load_bus());

            if(fabs(percent)>DOUBLE_EPSILON)
            {
                if(load.get_status()==true)
                {
                    osstream<<load.get_compound_device_name()<<" is manually scaled "<<(percent>0.0?"up":"down")<<" by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl;
                    double scale = load.get_load_manually_scale_factor_in_pu();
                    load.set_load_manually_scale_factor_in_pu(scale+percent);
                    osstream<<"Load manual scale is changed from "<<scale<<" to "<<scale+percent<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                else
                {
                    osstream<<load.get_compound_device_name()<<" is out of service. No load scale action can be applied at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"Load scaling percent is 0.0 for "<<load.get_compound_device_name()<<endl
                       <<"No load will be scaled at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No load will be scaled at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for scaling load."<<endl
               <<"No single load will be scaled at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::scale_all_load(double percent)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(fabs(percent)>DOUBLE_EPSILON)
    {
        unsigned int n = loads.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            LOAD& load = *(loads[i]);
            if(load.get_status()==true)
                scale_load(load.get_device_id(), percent);
        }
    }
    else
    {
        osstream<<"Load scaling percent is 0.0."<<endl
               <<"No all load will be scaled at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::trip_fixed_shunt(const DEVICE_ID& shunt_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(shunt_id.get_device_type()==STEPS_FIXED_SHUNT)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        FIXED_SHUNT* shunt_ptr = psdb.get_fixed_shunt(shunt_id);
        if(shunt_ptr!=NULL)
        {
            FIXED_SHUNT& shunt = *shunt_ptr;
            string busname = psdb.bus_number2bus_name(shunt.get_shunt_bus());

            if(shunt.get_status()==true)
            {
                shunt.set_status(false);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<shunt.get_compound_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<shunt_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No fixed shunt will be tripped at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a FIXED SHUNT (it is a "<<shunt_id.get_device_type()<<") for tripping fixed shunt."<<endl
               <<"No fixed shunt will be tripped at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_fixed_shunt(const DEVICE_ID& shunt_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    if(shunt_id.get_device_type()==STEPS_FIXED_SHUNT)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        FIXED_SHUNT* shunt_ptr = psdb.get_fixed_shunt(shunt_id);
        if(shunt_ptr!=NULL)
        {
            FIXED_SHUNT& shunt = *shunt_ptr;
            string busname = psdb.bus_number2bus_name(shunt.get_shunt_bus());

            if(shunt.get_status()==false)
            {
                shunt.set_status(true);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<shunt.get_compound_device_name()<<" is closed at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<shunt_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                   <<"No fixed shunt will be closed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a FIXED SHUNT (it is a "<<shunt_id.get_device_type()<<") for closing fixed shunt."<<endl
               <<"No fixed shunt will be closed at time "<<TIME<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::manual_bypass_lcc_hvdc2t(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LCC_HVDC2T* hvdc_ptr = psdb.get_lcc_hvdc2t(hvdc_id);
    if(hvdc_ptr!=NULL)
    {
        LCC_HVDC2T& hvdc = *hvdc_ptr;
        LCC_HVDC2T_MODEL* model_ptr = hvdc.get_lcc_hvdc2t_model();
        if(model_ptr != NULL)
        {
            LCC_HVDC2T_MODEL& model = *model_ptr;
            if(not model.is_blocked() and not model.is_bypassed())
                model.manual_bypass_lcc_hvdc2t();
        }
    }
}

void DYNAMICS_SIMULATOR::manual_unbypass_lcc_hvdc2t(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LCC_HVDC2T* hvdc_ptr = psdb.get_lcc_hvdc2t(hvdc_id);
    if(hvdc_ptr!=NULL)
    {
        LCC_HVDC2T& hvdc = *hvdc_ptr;
        LCC_HVDC2T_MODEL* model_ptr = hvdc.get_lcc_hvdc2t_model();
        if(model_ptr != NULL)
        {
            LCC_HVDC2T_MODEL& model = *model_ptr;
            if(not model.is_blocked() and model.is_manual_bypassed())
                model.manual_unbypass_lcc_hvdc2t();
        }

    }
}

void DYNAMICS_SIMULATOR::manual_block_lcc_hvdc2t(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LCC_HVDC2T* hvdc_ptr = psdb.get_lcc_hvdc2t(hvdc_id);

    if(hvdc_ptr!=NULL)
    {
        LCC_HVDC2T& hvdc = *hvdc_ptr;
        LCC_HVDC2T_MODEL* model_ptr = hvdc.get_lcc_hvdc2t_model();
        if(model_ptr != NULL)
        {
            LCC_HVDC2T_MODEL& model = *model_ptr;
            if(not model.is_blocked())
                model.manual_block_lcc_hvdc2t();
        }
    }
}

void DYNAMICS_SIMULATOR::manual_unblock_lcc_hvdc2t(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LCC_HVDC2T* hvdc_ptr = psdb.get_lcc_hvdc2t(hvdc_id);
    if(hvdc_ptr!=NULL)
    {
        LCC_HVDC2T& hvdc = *hvdc_ptr;
        LCC_HVDC2T_MODEL* model_ptr = hvdc.get_lcc_hvdc2t_model();
        if(model_ptr != NULL)
        {
            LCC_HVDC2T_MODEL& model = *model_ptr;
            if(model.is_manual_blocked())
                model.manual_unblock_lcc_hvdc2t();
        }
    }
}


double DYNAMICS_SIMULATOR::get_generator_voltage_reference_in_pu(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        EXCITER_MODEL* exciter_ptr = generator.get_exciter_model();
        if(exciter_ptr!=NULL)
        {
            EXCITER_MODEL& exciter = *exciter_ptr;
            return exciter.get_voltage_reference_in_pu();
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_mechanical_power_reference_in_pu_based_on_mbase(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        TURBINE_GOVERNOR_MODEL* tg_ptr = generator.get_turbine_governor_model();
        if(tg_ptr!=NULL)
        {
            TURBINE_GOVERNOR_MODEL& tg = *tg_ptr;
            return tg.get_mechanical_power_reference_in_pu_based_on_mbase();
        }
        else
            return 0.0;
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_mechanical_power_reference_in_MW(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        return get_generator_mechanical_power_reference_in_pu_based_on_mbase(gen_id)*generator.get_mbase_in_MVA();
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_excitation_voltage_in_pu(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        EXCITER_MODEL* exciter_ptr = generator.get_exciter_model();
        if(exciter_ptr!=NULL)
        {
            EXCITER_MODEL& exciter = *exciter_ptr;
            return exciter.get_excitation_voltage_in_pu();
        }
        else
        {
            SYNC_GENERATOR_MODEL* sg_ptr = generator.get_sync_generator_model();
            if(sg_ptr!=NULL)
            {
                SYNC_GENERATOR_MODEL& sg = *sg_ptr;
                return sg.get_initial_excitation_voltage_in_pu();
            }
            else
                return 0.0;
        }
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_mechanical_power_in_pu_based_on_mbase(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        TURBINE_GOVERNOR_MODEL* tg_ptr = generator.get_turbine_governor_model();
        if(tg_ptr!=NULL)
        {
            TURBINE_GOVERNOR_MODEL& tg = *tg_ptr;
            return tg.get_mechanical_power_in_pu_based_on_mbase();
        }
        else
        {
            SYNC_GENERATOR_MODEL* sg_ptr = generator.get_sync_generator_model();
            if(sg_ptr!=NULL)
            {
                SYNC_GENERATOR_MODEL& sg = *sg_ptr;
                return sg.get_initial_mechanical_power_in_pu_based_on_mbase();
            }
            else
                return 0.0;
        }
    }
    else
        return 0.0;
}
double DYNAMICS_SIMULATOR::get_generator_mechanical_power_in_MW(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        return get_generator_mechanical_power_in_pu_based_on_mbase(gen_id)*generator.get_mbase_in_MVA();
    }
    else
        return 0.0;
}


void DYNAMICS_SIMULATOR::change_generator_voltage_reference_in_pu(const DEVICE_ID& gen_id, double vref)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        string busname = psdb.bus_number2bus_name(generator.get_generator_bus());

        EXCITER_MODEL* exciter_ptr = generator.get_exciter_model();
        if(exciter_ptr!=NULL)
        {
            EXCITER_MODEL& exciter = *exciter_ptr;
            exciter.set_voltage_reference_in_pu(vref);
            ostringstream osstream;
            osstream<<"Voltage reference of "<<gen_id.get_compound_device_name()<<" does not exist when trying to change excitation voltage in pu. No change is made";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist when trying to change excitation voltage in pu. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_reference_in_pu_based_on_mbase(const DEVICE_ID& gen_id, double Pref)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        TURBINE_GOVERNOR_MODEL* tg_ptr = generator.get_turbine_governor_model();
        if(tg_ptr!=NULL)
        {
            TURBINE_GOVERNOR_MODEL& tg = *tg_ptr;
            tg.set_initial_mechanical_power_reference_in_pu_based_on_mbase(Pref);
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist when trying to change mechanical power reference in pu based on MBASE. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_reference_in_MW(const DEVICE_ID& gen_id, double Pref)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        double one_over_mbase = generator.get_one_over_mbase_in_one_over_MVA();
        change_generator_mechanical_power_reference_in_pu_based_on_mbase(gen_id, Pref*one_over_mbase);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist when trying to change mechanical power reference in MW. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_excitation_voltage_in_pu(const DEVICE_ID& gen_id, double efd)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        EXCITER_MODEL* ex = generator.get_exciter_model();
        if(ex!=NULL)
        {
            ostringstream osstream;
            osstream<<"Warning. Exciter model exists for "<<generator.get_compound_device_name()<<". No manual change of excitation voltage is allowed.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            SYNC_GENERATOR_MODEL* sg_ptr = generator.get_sync_generator_model();
            if(sg_ptr!=NULL)
            {
                SYNC_GENERATOR_MODEL& sg= *sg_ptr;
                sg.set_initial_excitation_voltage_in_pu(efd);
            }
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist when trying to change excitation voltage. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_in_pu_based_on_mbase(const DEVICE_ID& gen_id, double pmech)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        TURBINE_GOVERNOR_MODEL* tg = generator.get_turbine_governor_model();
        if(tg!=NULL)
        {
            ostringstream osstream;
            osstream<<"Warning. Turbine governor model exists for "<<generator.get_compound_device_name()<<". No manual change of mechanical power is allowed.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            SYNC_GENERATOR_MODEL* sg_ptr = generator.get_sync_generator_model();
            if(sg_ptr!=NULL)
            {
                SYNC_GENERATOR_MODEL& sg = *sg_ptr;
                sg.set_initial_mechanical_power_in_pu_based_on_mbase(pmech);
            }
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist when trying to change mechanical power in pu based on MBASE. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_in_MW(const DEVICE_ID& gen_id, double pmech)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator_ptr = psdb.get_generator(gen_id);
    if(generator_ptr != NULL)
    {
        GENERATOR& generator = *generator_ptr;
        double one_over_mbase = generator.get_one_over_mbase_in_one_over_MVA();
        change_generator_mechanical_power_in_pu_based_on_mbase(gen_id, pmech*one_over_mbase);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_compound_device_name()<<" does not exist when trying to change mechanical power in MW. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_energy_storage_active_power_reference_in_pu_based_on_mbase(const DEVICE_ID& es_id, double pref)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    ENERGY_STORAGE* energy_storage_ptr = psdb.get_energy_storage(es_id);
    if(energy_storage_ptr != NULL)
    {
        ENERGY_STORAGE& energy_storage = *energy_storage_ptr;
        energy_storage.set_manual_power_in_pu(pref);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<es_id.get_compound_device_name()<<" does not exist when trying to change energy storage power reference in pu based on mbase. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

double DYNAMICS_SIMULATOR::get_lcc_hvdc2t_power_order_in_MW(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LCC_HVDC2T* hvdc_ptr = psdb.get_lcc_hvdc2t(hvdc_id);
    if(hvdc_ptr != NULL)
    {
        LCC_HVDC2T& hvdc = *hvdc_ptr;
        return hvdc.get_nominal_dc_power_in_MW();
    }
    else
        return 0.0;
}

void DYNAMICS_SIMULATOR::change_lcc_hvdc2t_power_order_in_MW(const DEVICE_ID& hvdc_id, double porder)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    LCC_HVDC2T* hvdc_ptr = psdb.get_lcc_hvdc2t(hvdc_id);
    if(hvdc_ptr != NULL)
    {
        LCC_HVDC2T& hvdc = *hvdc_ptr;
        hvdc.set_nominal_dc_power_in_MW(porder);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<hvdc_id.get_compound_device_name()<<" does not exist when trying to change hvdc power order in MW. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::set_vsc_hvdc_line_fault(string vsc_name, DC_DEVICE_ID line_did, unsigned int side_bus, double location, double fault_r)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc_ptr = psdb.get_vsc_hvdc(vsc_name);
    if(vsc_hvdc_ptr != NULL)
    {
        VSC_HVDC& vsc_hvdc = *vsc_hvdc_ptr;
        unsigned int line_index = vsc_hvdc.get_dc_line_index(line_did);
        if(line_index!=INDEX_NOT_EXIST)
        {
            vsc_hvdc.set_dc_line_fault_location(line_index, side_bus, location);
            vsc_hvdc.set_dc_line_fault_r_in_ohm(line_index, fault_r);
        }
        else
        {
            osstream<<"Warning. "<<line_did.get_compound_device_name()<<" does not exist when trying to set line fault for VSC-HVDC "<<vsc_name<<". No change is made";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. VSC-HVDC "<<vsc_name<<" does not exist when trying to set vsc-hvdc line fault. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_vsc_hvdc_line_fault(string vsc_name, DC_DEVICE_ID line_did)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc_ptr = psdb.get_vsc_hvdc(vsc_name);
    VSC_HVDC& vsc_hvdc = *vsc_hvdc_ptr;
    unsigned int line_index = vsc_hvdc.get_dc_line_index(line_did);
    if(line_index!=INDEX_NOT_EXIST)
        vsc_hvdc.clear_dc_line_fault(line_index);
}

void DYNAMICS_SIMULATOR::trip_vsc_hvdc_line(string vsc_name, DC_DEVICE_ID line_did)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc_ptr = psdb.get_vsc_hvdc(vsc_name);
    VSC_HVDC& vsc_hvdc = *vsc_hvdc_ptr;
    unsigned int line_index = vsc_hvdc.get_dc_line_index(line_did);
    if(line_index!=INDEX_NOT_EXIST)
        vsc_hvdc.set_dc_line_status(line_index, false);
}

void DYNAMICS_SIMULATOR::close_vsc_hvdc_line(string vsc_name, DC_DEVICE_ID line_did)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    VSC_HVDC* vsc_hvdc_ptr = psdb.get_vsc_hvdc(vsc_name);
    VSC_HVDC& vsc_hvdc = *vsc_hvdc_ptr;
    unsigned int line_index = vsc_hvdc.get_dc_line_index(line_did);
    if(line_index!=INDEX_NOT_EXIST)
        vsc_hvdc.set_dc_line_status(line_index, true);
}

void DYNAMICS_SIMULATOR::switch_on_equivalent_device()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = e_devices.size();
    ostringstream osstream;
    osstream<<"There are "<<n<<" equivalent devices to switch on";
    toolkit.show_information_with_leading_time_stamp(osstream);
    for(unsigned int i=0; i!=n; ++i)
    {
        EQUIVALENT_DEVICE& e_device = *(e_devices[i]);
        e_device.switch_on();
    }
}


void DYNAMICS_SIMULATOR::prepare_buffers_for_system_change_detection()
{
    unsigned int n = 0;
    unsigned int N = (unsigned int)(get_system_change_detection_time_range_in_s()/get_min_dynamic_simulation_time_step_in_s());

    buffer4buses_voltage_change_detection.clear();
    buffer4generators_rotor_speed_change_detection.clear();

    n = buses4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        CONTINUOUS_BUFFER* buffer_ptr = new CONTINUOUS_BUFFER;
        CONTINUOUS_BUFFER& buffer = *buffer_ptr;
        buffer.set_toolkit(*toolkit);
        buffer.set_buffer_size(N);
        buffer4buses_voltage_change_detection.push_back(buffer_ptr);
    }

    n = generators4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        CONTINUOUS_BUFFER* buffer_ptr = new CONTINUOUS_BUFFER;
        CONTINUOUS_BUFFER& buffer = *buffer_ptr;
        buffer.set_toolkit(*toolkit);
        buffer.set_buffer_size(N);
        buffer4generators_rotor_speed_change_detection.push_back(buffer_ptr);
    }
}

void DYNAMICS_SIMULATOR::initialize_buffers_for_system_change_detection()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = 0;

    n = buses4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        BUS& bus = *(buses4change_detection[i]);
        CONTINUOUS_BUFFER& buffer = *(buffer4buses_voltage_change_detection[i]);
        buffer.initialize_buffer(toolkit.get_dynamic_simulation_time_in_s(),
                                  bus.get_positive_sequence_voltage_in_pu());
    }

    n = generators4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        GENERATOR& gen = *(generators4change_detection[i]);
        CONTINUOUS_BUFFER& buffer = *(buffer4generators_rotor_speed_change_detection[i]);
        SYNC_GENERATOR_MODEL* gen_model_ptr = gen.get_sync_generator_model();
        if(gen_model_ptr != NULL)
        {
            SYNC_GENERATOR_MODEL& gen_model = *gen_model_ptr;
            buffer.initialize_buffer(toolkit.get_dynamic_simulation_time_in_s(),
                                      gen_model.get_rotor_speed_deviation_in_pu());
        }
        else
        {
            ostringstream osstream;
            osstream<<"Warning. "<<gen.get_compound_device_name()<<" has NO Sync Generator Model for initializing buffer for system change detection."
                    <<"Check generators for detection.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void DYNAMICS_SIMULATOR::update_buffers_for_system_change_detection()
{
    STEPS& toolkit = get_toolkit();
    unsigned int n = 0;

    n = buses4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        BUS& bus = *(buses4change_detection[i]);
        CONTINUOUS_BUFFER& buffer = *(buffer4buses_voltage_change_detection[i]);
        buffer.append_data(toolkit.get_dynamic_simulation_time_in_s(),
                            bus.get_positive_sequence_voltage_in_pu());
    }

    n = generators4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        GENERATOR& gen = *(generators4change_detection[i]);
        CONTINUOUS_BUFFER& buffer = *(buffer4generators_rotor_speed_change_detection[i]);
        SYNC_GENERATOR_MODEL& gen_model = *(gen.get_sync_generator_model());
        buffer.append_data(toolkit.get_dynamic_simulation_time_in_s(),
                            gen_model.get_rotor_speed_deviation_in_pu());
    }
}

void DYNAMICS_SIMULATOR::change_dynamic_simulator_time_step_to_minimum()
{
    double min_delt = get_min_dynamic_simulation_time_step_in_s();
    double current_delt = get_dynamic_simulation_time_step_in_s();
    if(fabs(current_delt-min_delt)>FLOAT_EPSILON)
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"At time "<<get_dynamic_simulation_time_in_s()<<"s, system simulation time is reduced to minimum: "<<get_min_dynamic_simulation_time_step_in_s()<<"s";
        toolkit.show_information_with_leading_time_stamp(osstream);
        change_dynamic_simulator_time_step_in_s(min_delt);
    }
}

bool DYNAMICS_SIMULATOR::is_system_changing_suddenly() const
{
    return is_buses_voltage_changing_suddenly();
}

bool DYNAMICS_SIMULATOR::is_buses_voltage_changing_suddenly() const
{
    bool voltage_changing_flag = false;
    unsigned int n = buses4change_detection.size();
    for(unsigned int i=0; i<n; ++i)
    {
        CONTINUOUS_BUFFER& buffer = *(buffer4buses_voltage_change_detection[i]);
        double change = buffer.get_buffer_latest_change();
        if(change>get_system_change_detection_bus_voltage_threshold_in_pu())
        {
            voltage_changing_flag = true;
            break;
        }
    }
    return voltage_changing_flag;
}

void DYNAMICS_SIMULATOR::increase_dynamic_simulator_time_step_automatically()
{
    double current_delt = get_dynamic_simulation_time_step_in_s();
    double max_delt = get_max_dynamic_simulation_time_step_in_s();
    double delt = 0.0;
    if(current_delt<0.005)
        delt = current_delt;
    else
        delt = 0.005;
    if(current_delt+delt>max_delt)
        delt = max_delt-current_delt;
    change_dynamic_simulator_time_step_by(delt);
}

void DYNAMICS_SIMULATOR::decrease_dynamic_simulator_time_step_automatically()
{
    double current_delt = get_dynamic_simulation_time_step_in_s();
    double min_delt = get_min_dynamic_simulation_time_step_in_s();
    double delt = 0.0;
    if(current_delt>0.01)
        delt = -0.5*current_delt;
    else
        delt = -0.001;
    if(current_delt+delt<min_delt)
        delt = min_delt-current_delt;
    change_dynamic_simulator_time_step_by(delt);
}

void DYNAMICS_SIMULATOR::change_dynamic_simulator_time_step_by(double delt)
{
    double current_delt = get_dynamic_simulation_time_step_in_s();
    double max_delt = get_max_dynamic_simulation_time_step_in_s();
    double min_delt = get_min_dynamic_simulation_time_step_in_s();
    if((delt>0.0 and current_delt<max_delt) or (delt<0.0 and current_delt>min_delt))
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"At time "<<get_dynamic_simulation_time_in_s()<<"s, system simulation time is automatically increased by "<<delt<<"s";
        toolkit.show_information_with_leading_time_stamp(osstream);

        current_delt += delt;
        change_dynamic_simulator_time_step_in_s(current_delt);
    }
}

bool DYNAMICS_SIMULATOR::is_system_in_quasi_steady_state() const
{
    return is_buses_voltage_not_changing_in_detection_time_range() and
           is_generators_rotor_speed_not_changing_in_detection_time_range();
}

bool DYNAMICS_SIMULATOR::is_buses_voltage_not_changing_in_detection_time_range() const
{
    bool voltage_changing_flag = false;
    unsigned int n = buses4change_detection.size();
    double T = get_system_change_detection_time_range_in_s();
    double v_threshold = get_system_change_detection_bus_voltage_threshold_in_pu();
    for(unsigned int i=0; i<n; ++i)
    {
        CONTINUOUS_BUFFER& buffer = *(buffer4buses_voltage_change_detection[i]);
        double range = buffer.get_buffer_range_in_latest_seconds(T);
        if(range>v_threshold)
        {
            voltage_changing_flag = true;
            break;
        }
    }
    return not voltage_changing_flag;
}

bool DYNAMICS_SIMULATOR::is_generators_rotor_speed_not_changing_in_detection_time_range() const
{
    bool rotor_speed_changing_flag = false;
    unsigned int n = generators4change_detection.size();
    double T = get_system_change_detection_time_range_in_s();
    double w_threshold = get_system_change_detection_rotor_speed_threshold_in_pu();
    for(unsigned int i=0; i<n; ++i)
    {
        CONTINUOUS_BUFFER& buffer = *(buffer4generators_rotor_speed_change_detection[i]);
        double range = buffer.get_buffer_range_in_latest_seconds(T);
        if(range>w_threshold)
        {
            rotor_speed_changing_flag = true;
            break;
        }
    }
    return not rotor_speed_changing_flag;
}

