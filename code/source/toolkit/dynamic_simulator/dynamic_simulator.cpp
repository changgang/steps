#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/utility.h"
#include "header/network/y_matrix_builder.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <iostream>
#include <ctime>

using namespace std;

DYNAMICS_SIMULATOR::DYNAMICS_SIMULATOR(POWER_SYSTEM_DATABASE* psdb)
{
    if(psdb==NULL)
        return;

    psdb->set_dynamic_simulator(this);

    set_power_system_database(psdb);

    network_db = new NETWORK_DATABASE(psdb);

    clear();

    set_dynamic_simulation_time_in_s(0.0);
}

DYNAMICS_SIMULATOR::~DYNAMICS_SIMULATOR()
{
    stop();

    if(network_db!=NULL)
        delete network_db;

}


void DYNAMICS_SIMULATOR::clear()
{
    clear_meters();

    set_output_file("");

    close_meter_output_files();

    set_csv_file_export_enable_flag(true);
    set_json_file_export_enable_flag(false);

    set_dynamic_simulation_time_in_s(0.0);

    set_max_DAE_iteration(2);
    set_max_network_iteration(200);
    set_allowed_max_power_imbalance_in_MVA(0.0001);
    set_iteration_accelerator(1.0);
}

bool DYNAMICS_SIMULATOR::is_power_system_database_set() const
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(psdb!=NULL)
        return true;
    else
    {
        osstream<<"Error. Power system database is not set for the DYNAMICS_SIMULATOR."
          <<"No operation on the dynamics simulator will work.";
        show_information_with_leading_time_stamp(osstream);
        return false;
    }
}

NETWORK_DATABASE* DYNAMICS_SIMULATOR::get_network_database() const
{
    return network_db;
}

void DYNAMICS_SIMULATOR::set_csv_file_export_enable_flag(bool flag)
{
    csv_file_export_enabled = flag;
}

void DYNAMICS_SIMULATOR::set_json_file_export_enable_flag(bool flag)
{
    json_file_export_enabled = flag;
}

bool DYNAMICS_SIMULATOR::is_csv_file_export_enabled() const
{
    return csv_file_export_enabled;
}

bool DYNAMICS_SIMULATOR::is_json_file_export_enabled() const
{
    return json_file_export_enabled;
}

void DYNAMICS_SIMULATOR::set_max_DAE_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_DAE_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_max_network_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_network_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_allowed_max_power_imbalance_in_MVA(double tol)
{
    if(tol>0.0)
    {
        P_threshold_in_MW = tol;
        Q_threshold_in_MVar = tol;
    }
}

void DYNAMICS_SIMULATOR::set_iteration_accelerator(double iter_alpha)
{
    if(iter_alpha>0.0)
        this->alpha = iter_alpha;
}

/*void DYNAMICS_SIMULATOR::set_current_simulation_time_in_s(double time)
{
    TIME = time;
}

double DYNAMICS_SIMULATOR::get_current_simulation_time_in_s() const
{
    return TIME;
}*/

size_t DYNAMICS_SIMULATOR::get_max_DAE_iteration() const
{
    return max_DAE_iteration;
}

size_t DYNAMICS_SIMULATOR::get_max_network_iteration() const
{
    return max_network_iteration;
}

double DYNAMICS_SIMULATOR::get_allowed_max_power_imbalance_in_MVA() const
{
    return P_threshold_in_MW;
}

double DYNAMICS_SIMULATOR::get_iteration_accelerator() const
{
    return alpha;
}

void DYNAMICS_SIMULATOR::append_meter(const METER& meter)
{
    ostringstream osstream;
    if(not is_power_system_database_set())
        return;

    if(meter.is_valid())
    {
        meters.push_back(meter);
        //os<< "Meter added. %s.",meter.get_meter_name().c_str());
        //show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        osstream<<"Warning. Invalid meter is given. No meter will be set.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_meters()
{
    prepare_bus_related_meters();
    prepare_generator_related_meters();
    prepare_wt_generator_related_meters();
    prepare_load_related_meters();
    prepare_line_related_meters();
    prepare_hvdc_related_meters();
    prepare_equivalent_device_related_meters();
}


void DYNAMICS_SIMULATOR::prepare_bus_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n = psdb->get_bus_count();
    vector<BUS*> buses = psdb->get_all_buses();
    BUS* bus;
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_voltage_in_pu_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_angle_in_deg_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_frequency_deviation_in_pu_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_frequency_in_Hz_meter(bus->get_bus_number());
        append_meter(meter);
    }
}


void DYNAMICS_SIMULATOR::prepare_generator_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n;

    n = psdb->get_generator_count();
    vector<GENERATOR*> generators = psdb->get_all_generators();
    GENERATOR* generator;
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_rotor_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_excitation_voltage_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_terminal_active_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(generator->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_wt_generator_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n;

    n = psdb->get_wt_generator_count();
    vector<WT_GENERATOR*> generators = psdb->get_all_wt_generators();
    WT_GENERATOR* generator;
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_terminal_current_in_kA_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_terminal_active_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_terminal_reactive_power_in_MVar_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_max_available_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_speed_reference_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_turbine_speed_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_rotor_speed_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_rotor_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_active_current_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_reactive_current_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_active_power_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_reactive_power_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_reactive_voltage_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_pitch_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_wind_speed_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_wind_speed_in_mps_meter(generator->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_load_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n;

    n = psdb->get_load_count();
    vector<LOAD*> loads = psdb->get_all_loads();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        METER meter = setter.prepare_load_active_power_in_MW_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_reactive_power_in_MVar_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_shed_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
    }

}

void DYNAMICS_SIMULATOR::prepare_line_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n;

    n = psdb->get_line_count();
    vector<LINE*> lines = psdb->get_all_lines();
    LINE* line;
    for(size_t i=0; i!=n; ++i)
    {
        line = lines[i];
        METER meter = setter.prepare_line_active_power_in_MW_meter(line->get_device_id(),line->get_sending_side_bus());
        append_meter(meter);
        meter = setter.prepare_line_reactive_power_in_MVar_meter(line->get_device_id(),line->get_sending_side_bus());
        append_meter(meter);
        meter = setter.prepare_line_active_power_in_MW_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
        meter = setter.prepare_line_reactive_power_in_MVar_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_hvdc_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n;

    n = psdb->get_hvdc_count();
    vector<HVDC*> hvdcs = psdb->get_all_hvdcs();
    HVDC* hvdc;
    for(size_t i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];

        METER meter = setter.prepare_hvdc_rectifier_ac_voltage_in_pu_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_ac_voltage_in_pu_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_dc_voltage_in_kV_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_dc_voltage_in_kV_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_dc_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_dc_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_alpha_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_mu_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_gamma_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_mu_in_deg_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_dc_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_dc_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_ac_active_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_ac_reactive_power_in_MVar_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_ac_active_power_in_MW_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_ac_reactive_power_in_MVar_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_rectifier_ac_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);

        meter = setter.prepare_hvdc_inverter_ac_current_in_kA_meter(hvdc->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_equivalent_device_related_meters()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    METER_SETTER setter;
    setter.set_power_system_database(psdb);

    size_t n;

    n = psdb->get_equivalent_device_count();
    vector<EQUIVALENT_DEVICE*> edevices = psdb->get_all_equivalent_devices();
    EQUIVALENT_DEVICE* edevice;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];

        METER meter = setter.prepare_equivalent_device_active_power_generation_in_MW_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_reactive_power_generation_in_MVar_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_active_power_load_in_MW_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_reactive_power_load_in_MVar_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_active_power_net_load_in_MW_meter(edevice->get_device_id());
        append_meter(meter);

        meter = setter.prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(edevice->get_device_id());
        append_meter(meter);
    }
}


size_t DYNAMICS_SIMULATOR::get_meter_count() const
{
    if(not is_power_system_database_set())
        return 0;

    return meters.size();
}

METER DYNAMICS_SIMULATOR::get_meter(size_t i)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    METER voidmeter(psdb);

    if(not is_power_system_database_set())
        return voidmeter;

    if(meters.size()==0)
        return voidmeter;
    else
    {
        if(i<meters.size())
            return meters[i];
        else
            return voidmeter;
    }
}

void DYNAMICS_SIMULATOR::update_all_meters_value()
{
    if(not is_power_system_database_set())
        return;

    size_t n = meters.size();
    if(n==0)
        return;

    if(meter_values.size()!=n)
        meter_values.resize(n,0.0);

    for(size_t i=0; i!=n; ++i)
        meter_values[i]=meters[i].get_meter_value();
}

vector<double> DYNAMICS_SIMULATOR::get_all_meters_value()
{
    update_all_meters_value();
    return meter_values;
}

void DYNAMICS_SIMULATOR::clear_meters()
{
    if(not is_power_system_database_set())
        return;

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
    ostringstream osstream;
    if(csv_output_file.is_open())
    {
        osstream<<"CSV meter output file stream was connected to some file. Stream will be closed before reopen for exporting dynamic simulation meter values.";
        show_information_with_leading_time_stamp(osstream);
    }
    if(json_output_file.is_open())
    {
        osstream<<"JSON meter output file stream was connected to some file. Stream will be closed before reopen for exporting dynamic simulation meter values.";
        show_information_with_leading_time_stamp(osstream);
    }
    close_meter_output_files();

    if(output_filename=="")
    {
        return;

        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);

        char time_stamp[60];
        sprintf(time_stamp,"dynamic_result_%d-%02d-%02d~%02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        output_filename = string(time_stamp);

        osstream<<"Meter output filename is not properly set. Meters will be automatically exported to the following file:"<<endl
          <<output_filename;
        show_information_with_leading_time_stamp(osstream);
    }


    if(is_csv_file_export_enabled())
    {
        string csv_filename = output_filename+".csv";
        csv_output_file.open(csv_filename);
        if(not csv_output_file.is_open())
        {
            osstream<<"CSV meter output file "<<csv_filename<<" cannot be opened for exporting dynamic simulation meter values.";
            show_information_with_leading_time_stamp(osstream);
        }
    }

    if(is_json_file_export_enabled())
    {
        string json_filename = output_filename+".json";
        json_output_file.open(json_filename);
        if(not json_output_file.is_open())
        {
            osstream<<"JSON meter output file "<<json_filename<<" cannot be opened for exporting dynamic simulation meter values.";
            show_information_with_leading_time_stamp(osstream);
        }
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
}

void DYNAMICS_SIMULATOR::save_meter_information()
{
    size_t n = meters.size();
    if(n==0)
        return;

    open_meter_output_files();
    if(not csv_output_file.is_open() and not json_output_file.is_open())
        return;

    // save header to csv file
    if(is_csv_file_export_enabled() and csv_output_file.is_open())
    {
        //csv_output_file<<"TIME,ITERATION,MISMATCH IN MVA";
        //for(size_t i=0; i!=n; ++i)
        //    csv_output_file<<","<<meters[i].get_meter_type();
        //csv_output_file<<endl;

        csv_output_file<<"TIME,ITERATION,MISMATCH IN MVA";

        for(size_t i=0; i!=n; ++i)
            csv_output_file<<","<<meters[i].get_meter_name();
        csv_output_file<<endl;
    }

    // save header to json file
    if(is_json_file_export_enabled() and json_output_file.is_open())
    {
        json_output_file<<"{"<<endl;

        //json_output_file<<"    \"meter_type\" : [\"TIME\", \"ITERATION\", \"MISMATCH IN MVA\"";
        //for(size_t i=0; i!=n; ++i)
        //    json_output_file<<", \""<<meters[i].get_meter_type()<<"\"";
        //json_output_file<<"],"<<endl<<endl;

        json_output_file<<"    \"meter_name\" : [\"TIME\", \"ITERATION\", \"MISMATCH IN MVA\"";
        for(size_t i=0; i!=n; ++i)
            json_output_file<<", \""<<meters[i].get_meter_name()<<"\"";
        json_output_file<<"],"<<endl<<endl;
        json_output_file<<"    \"meter_value\" : ["<<endl;
    }
}

void DYNAMICS_SIMULATOR::save_meter_values()
{
    size_t n = meters.size();
    if(n==0)
        return;

    if(not csv_output_file.is_open() and not json_output_file.is_open())
        return;

    vector< complex<double> > I_mismatch;
    I_mismatch = get_bus_current_mismatch();
    vector< complex<double> > S_mismatch = get_bus_power_mismatch_in_MVA(I_mismatch);
    double smax = get_max_power_mismatch_in_MVA(S_mismatch);

    update_all_meters_value();

    ostringstream osstream;
    osstream<<setw(8)<<setprecision(4)<<fixed<<get_dynamic_simulation_time_in_s()<<","
      <<setw(3)<<ITER<<","
      <<setw(6)<<setprecision(3)<<fixed<<smax;

    for(size_t i=0; i!=n; ++i)
        osstream<<","<<setw(16)<<setprecision(12)<<fixed<<meter_values[i];

    if(is_csv_file_export_enabled() and csv_output_file.is_open())
        csv_output_file<<osstream.str()<<endl;

    if(is_json_file_export_enabled() and json_output_file.is_open())
        json_output_file<<","<<endl
                        <<"                       ["<<osstream.str()<<"]";
}

void DYNAMICS_SIMULATOR::start()
{
    ostringstream osstream;
    osstream<<"Dynamics initialization starts.";
    show_information_with_leading_time_stamp(osstream);

    meter_values.resize(meters.size(), 0.0);

    set_dynamic_simulation_time_in_s(-2.0*get_dynamic_simulation_time_step_in_s());

    network_db->optimize_network_ordering();

    run_all_models(INITIALIZE_MODE);

    network_db->build_dynamic_network_matrix();
    //const SPARSE_MATRIX& Y = network_db->get_dynamic_network_matrix();
    //Y.report_brief();
    //network_db->report_physical_internal_bus_number_pair();

    build_jacobian();

    bool converged = false;
    size_t iter_count  = 0;
    while(true)
    {
        iter_count++;
        converged = solve_network();
        if(converged or iter_count>2)
            break;
    }
    ITER = iter_count;
    if(iter_count>1)
    {
        osstream<<"Warning. Network solution converged in "<<iter_count<<" iterations for dynamics simulation initialization.";
        show_information_with_leading_time_stamp(osstream);
    }

    osstream<<"Dynamics initialization finished.";
    show_information_with_leading_time_stamp(osstream);

    save_meter_information();
    save_meter_values();
}

void DYNAMICS_SIMULATOR::stop()
{
    if(get_dynamic_simulation_time_in_s() >0.0 or csv_output_file.is_open())
    {
        ostringstream osstream;
        osstream<<"Dynamics simulation stops.";
        show_information_with_leading_time_stamp(osstream);

        if(json_output_file.is_open())
            json_output_file<<endl
                            <<"                    ]"<<endl
                            <<"}";
        close_meter_output_files();

        set_dynamic_simulation_time_in_s(-2.0*get_dynamic_simulation_time_step_in_s());
    }
}


double DYNAMICS_SIMULATOR::get_system_max_angle_difference_in_deg()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    vector<GENERATOR*> gens = psdb->get_all_generators();
    size_t n = gens.size();
    GENERATOR* gen;
    SYNC_GENERATOR_MODEL* model;
    double max_angle = -1e10, min_angle = 1e10;
    double angle;
    for(size_t i=0; i!=n; ++i)
    {
        gen = gens[i];
        if(gen->get_status()==true)
        {
            model = gen->get_sync_generator_model();
            if(model!=NULL)
            {
                angle = model->get_rotor_angle_in_deg();
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
    update_with_event();
    while(get_dynamic_simulation_time_in_s()<=time-FLOAT_EPSILON)
        run_a_step();
}

void DYNAMICS_SIMULATOR::run_a_step()
{
    ostringstream osstream;

    set_dynamic_simulation_time_in_s(get_dynamic_simulation_time_in_s()+ get_dynamic_simulation_time_step_in_s());
    update_equivalent_devices_buffer();
    update_equivalent_devices_output();

    //bool network_converged = false;
    //bool DAE_converged = false;
    size_t DAE_iter_count = 0;//network_iter_count = 0;
    size_t DAE_iter_max = get_max_DAE_iteration();
    //size_t network_iter_max = get_max_network_iteration();
    double max_angle_difference_old = get_system_max_angle_difference_in_deg();
    while(true)
    {
        ++DAE_iter_count;
        if(DAE_iter_count>DAE_iter_max)
        {
            if(DAE_iter_max!=2)
            {
                osstream<<"Failed to solve DAE in "<<DAE_iter_max<<" iterations when integrating at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                show_information_with_leading_time_stamp(osstream);
            }
            break;
        }
        integrate();
        solve_network();
        /*network_converged = solve_network();
        if(not network_converged)
        {
            osstream<<"Failed to solve network in "<<network_iter_max<<" iterations when integrating at time "<<TIME<<" s.";
            show_information_with_leading_time_stamp(osstream);
        }*/
        double max_angle_difference_new = get_system_max_angle_difference_in_deg();
        if(fabs(max_angle_difference_new-max_angle_difference_old)<FLOAT_EPSILON) // DAE solution converged
            break;
        else
            max_angle_difference_old = max_angle_difference_new;
    }
    DAE_iteration_count = DAE_iter_count;
    ITER = DAE_iteration_count;
    update();
}

void DYNAMICS_SIMULATOR::update_with_event()
{
    ostringstream osstream;

    bool network_converged = false;
    size_t network_iter_max = get_max_network_iteration();

    network_converged = solve_network();
    if(not network_converged)
    {
        osstream<<"Failed to solve network in "<<network_iter_max<<" iterations when updating with event at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }

    update_bus_frequency_blocks();
    update();
}

void DYNAMICS_SIMULATOR::integrate()
{
    //clock_t start = clock();
    run_all_models(INTEGRATE_MODE);
    //cout<<"    elapsed time for integration: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
}

void DYNAMICS_SIMULATOR::update()
{
    ostringstream osstream;
    //clock_t start = clock();
    update_equivalent_devices_buffer();
    run_all_models(UPDATE_MODE);
    //cout<<"    elapsed time for update: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    //start = clock();

    size_t network_iter_max = get_max_network_iteration();
    bool network_converged = false;

    network_converged = solve_network();

    if(not network_converged)
    {
        osstream<<"Failed to solve network in "<<network_iter_max<<" iterations when updating at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
    update_bus_frequency_blocks();

    save_meter_values();
    //cout<<"    elapsed time for export meters: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
}

void DYNAMICS_SIMULATOR::run_all_models(DYNAMIC_MODE mode)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    size_t n = db->get_generator_count();
    vector<GENERATOR*> generators = db->get_all_generators();
    GENERATOR* generator;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        if(generator->get_status()==false)
            continue;
        generator->run(mode);
    }

    n = db->get_wt_generator_count();
    vector<WT_GENERATOR*> wtgens = db->get_all_wt_generators();
    WT_GENERATOR* wtgen;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        wtgen = wtgens[i];
        if(wtgen->get_status()==false)
            continue;
        wtgen->run(mode);
    }

    n = db->get_load_count();
    vector<LOAD*> loads = db->get_all_loads();
    LOAD* load;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        if(load->get_status()==false)
            continue;
        load->run(mode);
    }

    n = db->get_hvdc_count();
    vector<HVDC*> hvdcs = db->get_all_hvdcs();
    HVDC* hvdc;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];
        hvdc->run(mode);
    }

    n = db->get_equivalent_device_count();
    vector<EQUIVALENT_DEVICE*> edevices = db->get_all_equivalent_devices();
    EQUIVALENT_DEVICE* edevice;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        edevice->run(mode);
    }

    vector<BUS*> buses = db->get_all_in_service_buses();
    n = buses.size();
    BUS* bus;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
        switch(mode)
        {
            case INITIALIZE_MODE:
            {
                model->initialize();
                break;
            }
            default:
            {
                model->run(mode);
                break;
            }
        }
    }
}

void DYNAMICS_SIMULATOR::update_bus_frequency_blocks()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    vector<BUS*> buses = db->get_all_in_service_buses();
    size_t n = buses.size();
    BUS* bus;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        if(bus->get_bus_type()==OUT_OF_SERVICE)
            continue;
        BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
        model->update_for_applying_event();
    }
}
void DYNAMICS_SIMULATOR::update_equivalent_devices_buffer()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    vector<EQUIVALENT_DEVICE*> edevices = db->get_all_equivalent_devices();
    size_t n = edevices.size();
    EQUIVALENT_DEVICE* edevice;
    ostringstream osstream;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        edevice->update_meter_buffer();
    }
}

void DYNAMICS_SIMULATOR::update_equivalent_devices_output()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    vector<EQUIVALENT_DEVICE*> edevices = db->get_all_equivalent_devices();
    size_t n = edevices.size();
    EQUIVALENT_DEVICE* edevice;
    ostringstream osstream;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        edevice->update_equivalent_outputs();

        osstream<<"At time "<<get_dynamic_simulation_time_in_s()<<" s, equivalent load of "<<edevice->get_device_name()<<" is: "<<edevice->get_total_equivalent_power_as_load_in_MVA()<<"MVA";
        show_information_with_leading_time_stamp(osstream);
    }
}

bool DYNAMICS_SIMULATOR::solve_network()
{
    ostringstream osstream;
    //POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    //clock_t start = clock(), start0 = clock();
    /*size_t nbus = psdb->get_in_service_bus_count();
    for(int i=0; i!=nbus; ++i)
    {
        size_t bus = network_db->get_physical_bus_number_of_internal_bus(i);
        complex<double> V = psdb->get_bus_complex_voltage_in_pu(bus);
        osstream<<"Initial voltage of bus %u: %f pu, %f deg",bus, abs(V), rad2deg(arg(V)));
        show_information_with_leading_time_stamp(osstream);
    }*/
    bool converged = false;

    size_t network_iter_max = get_max_network_iteration();
    size_t network_iter_count = 0;

    vector< complex<double> > I_mismatch;
    vector<double> I_vec;
    vector<double> delta_V;

    while(true)
    {
        ++network_iter_count;
        if(network_iter_count>network_iter_max)
            break;
        solve_hvdcs_without_integration();
        I_mismatch = get_bus_current_mismatch();
        if(not is_converged(I_mismatch))
        {
            I_vec = get_bus_current_mismatch_vector(I_mismatch);
            delta_V = I_vec/jacobian;
            update_bus_voltage(delta_V);
        }
        else
        {
            converged = true;
            break;
        }
    }

    network_iteration_count = network_iter_count;

    return converged;






    //start = clock();
    I_mismatch = get_bus_current_mismatch();
    //osstream<<"Bus current mismatch for network solution at time "<<TIME<<endl;
    //for(size_t i=0; i<I_mismatch.size(); ++i)
    //    osstream<<"Current mismatch ["<<setw(6)<<i<<"("<<network_db->get_physical_bus_number_of_internal_bus(i)<<")]: "<<I_mismatch[i]<<endl;
    //show_information_with_leading_time_stamp(osstream);
    //cout<<"    elapsed time for getting bus current mismatch: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    if(is_converged(I_mismatch))
    {
        converged = true;
    }
    else
    {
        //start = clock();
        vector<double> I_vec = get_bus_current_mismatch_vector(I_mismatch);
        //cout<<"    elapsed time for getting bus current mismatch vector: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
        //start = clock();
        vector<double> delta_V = I_vec/jacobian;
        //cout<<"    elapsed time for solving delta V: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
        //start = clock();
        update_bus_voltage(delta_V);
        //cout<<"    elapsed time for updating voltage: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    }
    //cout<<"    elapsed time for solve network: "<<double(clock()-start0)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;

    /*while(true)
    {
        vector< complex<double> > I_mismatch;
        I_mismatch = get_bus_current_mismatch();
        if(is_converged(I_mismatch))
        {
            converged = true;
            return converged;
        }

        vector<double> I_vec = get_bus_current_mismatch_vector(I_mismatch);
        vector<double> delta_V = I_vec/jacobian;
        update_bus_voltage(delta_V);
    }*/
    /*for(int i=0; i!=nbus; ++i)
    {
        size_t bus = network_db->get_physical_bus_number_of_internal_bus(i);
        complex<double> V = psdb->get_bus_complex_voltage_in_pu(bus);
        osstream<<"Solved  voltage of bus %u: %f pu, %f deg",bus, abs(V), rad2deg(arg(V)));
        show_information_with_leading_time_stamp(osstream);
    }*/

    return converged;
}


void DYNAMICS_SIMULATOR::solve_hvdcs_without_integration()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    vector<HVDC*> hvdcs = psdb->get_all_hvdcs();
    size_t n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        HVDC_MODEL* model = hvdcs[i]->get_hvdc_model();
        if(model!=NULL)
            model->solve_hvdc_model_without_integration();
    }
}

vector< complex<double> > DYNAMICS_SIMULATOR::get_bus_current_mismatch() const
{
    vector< complex<double> > I_mismatch;
    I_mismatch = get_bus_currnet_into_network();

    size_t n = I_mismatch.size();
    for(size_t i = 0; i!=n; ++i)
        I_mismatch[i] = -I_mismatch[i];

    add_generators_to_bus_current_mismatch(I_mismatch);
    add_wt_generators_to_bus_current_mismatch(I_mismatch);
    add_loads_to_bus_current_mismatch(I_mismatch);
    add_hvdcs_to_bus_current_mismatch(I_mismatch);
    add_equivalent_devices_to_bus_current_mismatch(I_mismatch);

    return I_mismatch;
}

vector< complex<double> > DYNAMICS_SIMULATOR::get_bus_currnet_into_network() const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    const SPARSE_MATRIX& Y = network_db->get_dynamic_network_matrix();
    size_t nbus = psdb->get_in_service_bus_count();

    vector< complex<double> > bus_current;
    bus_current.resize(nbus, 0.0);

    complex<double> voltage, y;

    int row;
    size_t column_physical_bus;

    int nsize = Y.get_matrix_size();
    int k_start=0, k_end=0;
    for(int column=0; column!=nsize; column++)
    {
        column_physical_bus = network_db->get_physical_bus_number_of_internal_bus(column);
        voltage = psdb->get_bus_complex_voltage_in_pu(column_physical_bus);
        //cout<<"Bus "<<column_physical_bus<<", voltage "<<abs(voltage)<<", angle="<<rad2deg(arg(voltage))<<endl;

        k_end = Y.get_starting_index_of_column(column+1);
        for(int k=k_start; k!=k_end; ++k)
        {
            row = Y.get_row_number_of_entry_index(k);
            y = Y.get_entry_value(k);
            bus_current[row] += y*voltage;
        }
        k_start = k_end;
    }
/*
    ostringstream osstream;
    osstream<<"Current and power flowing into network (physical bus).");
    show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     Ireal(pu)    Iimag(pu)    P(pu)   Q(pu)");
    show_information_with_leading_time_stamp(osstream);

    complex<double> s;
    for(size_t i=0; i!=nbus; ++i)
    {
        column_physical_bus = network_db->get_physical_bus_number_of_internal_bus(i);
        voltage = psdb->get_bus_complex_voltage_in_pu(column_physical_bus);
        s = voltage*conj(bus_current[i]);

        osstream<<"%-8u %-10f %-10f %-10f %-10f",column_physical_bus, bus_current[i].real(), bus_current[i].imag(), s.real(), s.imag());
        show_information_with_leading_time_stamp(osstream);
    }
*/
    return bus_current;
}

void DYNAMICS_SIMULATOR::add_generators_to_bus_current_mismatch(vector< complex<double> >& I_mismatch) const
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    size_t physical_bus, internal_bus;

    bool status;

    vector<GENERATOR*> generators = psdb->get_all_generators();

    GENERATOR* generator;
    SYNC_GENERATOR_MODEL* gen_model;

    size_t ngen = generators.size();

    complex<double> E, V, Z, I;

    for(size_t i=0; i!=ngen; ++i)
    {
        generator = generators[i];

        status = generator->get_status();
        if(status == false)
            continue;

        physical_bus = generator->get_generator_bus();

        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

        gen_model = generator->get_sync_generator_model();
        if(gen_model==NULL)
            continue;

        //I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
        I = gen_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();

        I_mismatch[internal_bus] += I;

        /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
        show_information_with_leading_time_stamp(osstream);
        complex<double> Edq = gen_model->get_internal_voltage_in_pu_in_dq_axis();
        complex<double> Exy = gen_model->get_internal_voltage_in_pu_in_xy_axis();
        osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
        show_information_with_leading_time_stamp(osstream);*/

    }
}

void DYNAMICS_SIMULATOR::add_wt_generators_to_bus_current_mismatch(vector< complex<double> >& I_mismatch) const
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    size_t physical_bus, internal_bus;

    bool status;

    vector<WT_GENERATOR*> generators = psdb->get_all_wt_generators();

    WT_GENERATOR* generator;
    WT_GENERATOR_MODEL* gen_model;

    size_t ngen = generators.size();

    complex<double> E, V, Z, I;

    for(size_t i=0; i!=ngen; ++i)
    {
        generator = generators[i];

        status = generator->get_status();
        if(status == false)
            continue;

        physical_bus = generator->get_generator_bus();

        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

        gen_model = generator->get_wt_generator_model();
        if(gen_model==NULL)
            continue;

        //I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
        I = gen_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();

        I_mismatch[internal_bus] += I;

        /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
        show_information_with_leading_time_stamp(osstream);
        complex<double> Edq = gen_model->get_internal_voltage_in_pu_in_dq_axis();
        complex<double> Exy = gen_model->get_internal_voltage_in_pu_in_xy_axis();
        osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
        show_information_with_leading_time_stamp(osstream);*/

    }
}

void DYNAMICS_SIMULATOR::add_loads_to_bus_current_mismatch(vector< complex<double> >& I_mismatch) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    size_t physical_bus, internal_bus;

    vector<LOAD*> loads = psdb->get_all_loads();

    LOAD* load;

    size_t nload = loads.size();

    complex<double> I;

    for(size_t i=0; i!=nload; ++i)
    {
        load = loads[i];

        if(load->get_status()) //  == true
        {
            physical_bus = load->get_load_bus();

            internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

            I_mismatch[internal_bus] -= load->get_dynamics_load_current_in_pu_based_on_system_base_power();;
        }

        /*ostringstream osstream;
        osstream<< "Load %u source current: %f + j%f",physical_bus, I.real(), I.imag());
        show_information_with_leading_time_stamp(osstream);*/
    }
}
void DYNAMICS_SIMULATOR::add_hvdcs_to_bus_current_mismatch(vector< complex<double> >& I_mismatch) const
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    size_t physical_bus, internal_bus;

    vector<HVDC*> hvdcs = psdb->get_all_hvdcs();

    size_t nhvdc = hvdcs.size();

    complex<double> I;

    for(size_t i=0; i!=nhvdc; ++i)
    {
        physical_bus = hvdcs[i]->get_converter_bus(RECTIFIER);

        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

        I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);
        //cout<<"Rec current of "<<hvdcs[i]->get_device_name()<<" in pu is "<<I<<endl;

        I_mismatch[internal_bus] -= I;

        physical_bus = hvdcs[i]->get_converter_bus(INVERTER);

        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

        I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);
        //cout<<"Inv current of "<<hvdcs[i]->get_device_name()<<" in pu is "<<I<<endl;

        I_mismatch[internal_bus] -= I;
    }
}

void DYNAMICS_SIMULATOR::add_equivalent_devices_to_bus_current_mismatch(vector< complex<double> >& I_mismatch) const
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double sbase = psdb->get_system_base_power_in_MVA();

    size_t physical_bus, internal_bus;

    bool status;

    vector<EQUIVALENT_DEVICE*> edevices = psdb->get_all_equivalent_devices();

    size_t nedevice = edevices.size();

    complex<double> I, V, S;

    for(size_t i=0; i!=nedevice; ++i)
    {
        status = edevices[i]->get_status();
        if(status == false)
            continue;

        physical_bus = edevices[i]->get_equivalent_device_bus();

        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

        S = edevices[i]->get_total_equivalent_power_as_load_in_MVA()/sbase;

        V = psdb->get_bus_complex_voltage_in_pu(physical_bus);

        I_mismatch[internal_bus] -= conj(S/V);
    }
}

bool DYNAMICS_SIMULATOR::is_converged(vector< complex<double> > &I_mismatch)
{
    //clock_t start = clock();
    vector< complex<double> > S_mismatch = get_bus_power_mismatch_in_MVA(I_mismatch);

    double smax = get_max_power_mismatch_in_MVA(S_mismatch);

    double s_allowed = get_allowed_max_power_imbalance_in_MVA();

    //cout<<"    time for getting maximum power mismatch: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    if(smax<s_allowed)
        return true;
    else
        return false;
}

vector< complex<double> > DYNAMICS_SIMULATOR:: get_bus_power_mismatch_in_MVA(vector< complex<double> > &I_mismatch)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    double sbase = psdb->get_system_base_power_in_MVA();

    vector< complex<double> > S_mismatch = I_mismatch;

    size_t n = I_mismatch.size();
    size_t physical_bus;
    complex<double> V;
    for(size_t i= 0; i!=n; ++i)
    {
        physical_bus = network_db->get_physical_bus_number_of_internal_bus(i);
        V = psdb->get_bus_complex_voltage_in_pu(physical_bus);
        S_mismatch[i] = V*conj(S_mismatch[i])*sbase;
    }
    return S_mismatch;
}

double DYNAMICS_SIMULATOR::get_max_power_mismatch_in_MVA(vector< complex<double> > &S_mismatch)
{
    ostringstream osstream;
    double smax = 0.0, s;
    complex<double> smax_complex;
    //size_t smax_bus = 0;
    size_t n = S_mismatch.size();
    for(size_t i=0; i!=n; ++i)
    {
        s = abs(S_mismatch[i]);
        if(s>smax)
        {
            smax = s;
            //smax_bus = network_db->get_physical_bus_number_of_internal_bus(i);
            smax_complex = S_mismatch[i];
        }
    }
    //os<<"AT TIME %f, Max power mismatch found: %f (%f + j%f) MVA at bus %u.", TIME, smax, smax_complex.real(), smax_complex.imag(), smax_bus);
    //show_information_with_leading_time_stamp(osstream);
    return smax;

}


vector<double> DYNAMICS_SIMULATOR::get_bus_current_mismatch_vector(const vector< complex<double> > I_mismatch) const
{
    size_t n = I_mismatch.size();
    vector<double> I_vec;
    I_vec.resize(n*2, 0.0);
    for(size_t i=0; i!=n; ++i)
    {
        I_vec[i] = I_mismatch[i].imag();
        I_vec[i+n] = I_mismatch[i].real();
    }
    return I_vec;
}


void DYNAMICS_SIMULATOR::update_bus_voltage(const vector<double> delta_V)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    size_t n = delta_V.size();
    n = n>>1;

    size_t physical_bus;
    BUS* bus;
    complex<double> V0, delta_v, V;
    double vmag0, vang0, vmag_new, vang_new, delta_vang;

    for(size_t i=0; i!=n; ++i)
    {
        physical_bus = network_db->get_physical_bus_number_of_internal_bus(i);
        bus = psdb->get_bus(physical_bus);
        vmag0 = bus->get_voltage_in_pu();
        vang0 = bus->get_angle_in_rad();
        V0 = complex<double>(cos(vang0), sin(vang0))*vmag0;

        delta_v = complex<double>(delta_V[i], delta_V[i+n]);

        V = V0-delta_v;

        vmag_new = abs(V);

        delta_vang = arg(V/V0);

        vang_new = vang0+delta_vang;

        bus->set_voltage_in_pu(vmag_new);
        bus->set_angle_in_rad(vang_new);
    }
}


void DYNAMICS_SIMULATOR::build_jacobian()
{
    jacobian.clear();
    SPARSE_MATRIX& Y = network_db->get_dynamic_network_matrix();
    size_t nbus = Y.get_matrix_size();
    complex<double> y;
    double g, b;
    size_t row, col;
    size_t starting_index=0, ending_index;
    for(size_t i=0; i!=nbus; ++i)
    {
        ending_index = Y.get_starting_index_of_column(i+1);
        col = i;
        for(size_t k=starting_index; k!=ending_index; ++k)
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


void DYNAMICS_SIMULATOR::change_dynamic_simulator_time_step(double newDELT)
{
    set_dynamic_simulation_time_step_in_s(newDELT);
}


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_set(size_t bus, FAULT fault)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    BUS* busptr = db->get_bus(bus);
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = busptr->get_voltage_in_pu();
    if(fault_b<-2e8)
        busptr->set_voltage_in_pu(0.05);
    else
    {
        if(fault_b<-2e7)
            busptr->set_voltage_in_pu(0.1);
        else
        {
            if(fault_b<-2e6)
                busptr->set_voltage_in_pu(0.2);
            else
            {
                if(fault_b<-2e5)
                    busptr->set_voltage_in_pu(0.3);
                else
                {
                    if(fault_b<-2e4)
                        busptr->set_voltage_in_pu(0.4);
                    else
                        busptr->set_voltage_in_pu(current_voltage-0.1);
                }
            }
        }
    }
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_cleared(size_t bus, FAULT fault)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    BUS* busptr = db->get_bus(bus);
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = busptr->get_voltage_in_pu();
    if(fault_b<-2e8)
        busptr->set_voltage_in_pu(current_voltage+0.95);
    else
    {
        if(fault_b<-2e7)
            busptr->set_voltage_in_pu(current_voltage+0.9);
        else
        {
            if(fault_b<-2e6)
                busptr->set_voltage_in_pu(current_voltage+0.8);
            else
            {
                if(fault_b<-2e5)
                    busptr->set_voltage_in_pu(current_voltage+0.7);
                else
                {
                    if(fault_b<-2e4)
                        busptr->set_voltage_in_pu(current_voltage+0.6);
                    else
                        busptr->set_voltage_in_pu(current_voltage+0.1);
                }
            }
        }
    }
}


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_line_fault_set(DEVICE_ID did, size_t side_bus, double location, FAULT fault)
{
    if(location <0.0 or location >1.0)
        return;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(location>0.5)
    {
        LINE* line = db->get_line(did);
        if(line!=NULL)
        {
            if(not line->is_connected_to_bus(side_bus))
                return;

            size_t ibus = line->get_sending_side_bus();
            size_t jbus = line->get_receiving_side_bus();

            location = 1.0-location;

            side_bus = (ibus==side_bus? ibus:jbus);

            guess_bus_voltage_with_line_fault_set(did, side_bus, location, fault);
        }
    }
    else
    {
        LINE* line = db->get_line(did);
        if(line!=NULL)
        {
            if(not line->is_connected_to_bus(side_bus))
                return;

            size_t ibus = line->get_sending_side_bus();
            size_t jbus = line->get_receiving_side_bus();

            size_t this_bus = side_bus;
            size_t other_bus = (ibus==side_bus? jbus:ibus);

            double this_location = location;
            double other_location = 1.0-location;

            FAULT this_fault = fault;
            this_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(this_location*2.0));
            FAULT other_fault = fault;
            other_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(other_location*2.0));
            guess_bus_voltage_with_bus_fault_set(this_bus, this_fault);
            guess_bus_voltage_with_bus_fault_set(other_bus, other_fault);
        }
    }
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_line_fault_cleared(DEVICE_ID did, size_t side_bus, double location, FAULT fault)
{
    if(location <0.0 or location >1.0)
        return;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    if(location>0.5)
    {
        LINE* line = db->get_line(did);
        if(line!=NULL)
        {
            if(not line->is_connected_to_bus(side_bus))
                return;

            size_t ibus = line->get_sending_side_bus();
            size_t jbus = line->get_receiving_side_bus();

            location = 1.0-location;

            side_bus = (ibus==side_bus? ibus:jbus);

            guess_bus_voltage_with_line_fault_cleared(did, side_bus, location, fault);
        }
    }
    else
    {
        LINE* line = db->get_line(did);
        if(line!=NULL)
        {
            if(not line->is_connected_to_bus(side_bus))
                return;

            size_t ibus = line->get_sending_side_bus();
            size_t jbus = line->get_receiving_side_bus();

            size_t this_bus = side_bus;
            size_t other_bus = (ibus==side_bus? jbus:ibus);

            double this_location = location;
            double other_location = 1.0-location;

            FAULT this_fault = fault;
            this_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(this_location*2.0));
            FAULT other_fault = fault;
            other_fault.set_fault_shunt_in_pu(fault.get_fault_shunt_in_pu()/exp(other_location*2.0));
            guess_bus_voltage_with_bus_fault_cleared(this_bus, this_fault);
            guess_bus_voltage_with_bus_fault_cleared(other_bus, other_fault);

        }
    }
}

void DYNAMICS_SIMULATOR::set_bus_fault(size_t bus, complex<double> fault_shunt)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    BUS* busptr = db->get_bus(bus);
    if(busptr!=NULL)
    {
        if(abs(fault_shunt)<FLOAT_EPSILON)
        {
            osstream<<"Zero fault shunt is given for bus "<<bus<<"."<<endl
                   <<"No fault will be set for bus "<<bus<<" at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        FAULT fault;
        fault.set_fault_type(THREE_PHASES_FAULT);
        fault.set_fault_shunt_in_pu(fault_shunt);

        osstream<<fault.get_fault_type_string()<<" will be set for bus "<<bus<<" at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl
               <<"Fault shunt is"<<fault_shunt<<" pu.";
        show_information_with_leading_time_stamp(osstream);

        busptr->set_fault(fault);
        guess_bus_voltage_with_bus_fault_set(bus, fault);

        network_db->build_dynamic_network_matrix();
        build_jacobian();
    }
    else
    {
        osstream<<"Bus "<<bus<<" does not exist in power system database."<<endl
               <<"No bus fault will be set at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_bus_fault(size_t bus)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    BUS* busptr = db->get_bus(bus);
    if(busptr!=NULL)
    {
        ostringstream osstream;

        osstream<<"Fault at bus "<<bus<<" will be cleared at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);

        FAULT fault = busptr->get_fault();
        busptr->clear_fault();
        guess_bus_voltage_with_bus_fault_cleared(bus, fault);

        network_db->build_dynamic_network_matrix();
        build_jacobian();
    }
}

void DYNAMICS_SIMULATOR::trip_bus(size_t bus)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    BUS* busptr = db->get_bus(bus);
    if(busptr!=NULL)
    {
        if(busptr->get_bus_type()!=OUT_OF_SERVICE)
        {
            ostringstream osstream;

            osstream<<"Bus "<<bus<<" will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s. Devices connecting to bus "<<bus<<" will also be tripped.";
            show_information_with_leading_time_stamp(osstream);

            db->trip_bus(bus);

            network_db->optimize_network_ordering();
            network_db->build_dynamic_network_matrix();
            build_jacobian();
        }
    }
}

void DYNAMICS_SIMULATOR::trip_buses(const vector<size_t> buses)
{
    size_t n = buses.size();
    for(size_t i=0; i!=n; ++i)
        trip_bus(buses[i]);
}

void DYNAMICS_SIMULATOR::set_line_fault(DEVICE_ID line_id, size_t side_bus, double location, complex<double> fault_shunt)
{
    ostringstream osstream;

    if(line_id.get_device_type()!="LINE")
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for setting line fault."<<endl
               <<"No line fault will be set at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LINE* lineptr = db->get_line(line_id);
    if(lineptr!=NULL)
    {
        if(not lineptr->is_connected_to_bus(side_bus))
        {
            osstream<<lineptr->get_device_name()<<"is not connected to bus "<<side_bus<<endl
                   <<"No fault will be set at location "<<location<<" to bus "<<side_bus<<" at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        if(location<0.0 or location>1.0)
        {
            osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_device_name()<<"."<<endl
                   <<"No fault will be set at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        if(abs(fault_shunt)<FLOAT_EPSILON)
        {
            osstream<<"Zero fault shunt is given for "<<lineptr->get_device_name()<<"."<<endl
                   <<"No fault will be set for "<<lineptr->get_device_name()<<" at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        FAULT fault;
        fault.set_fault_type(THREE_PHASES_FAULT);
        fault.set_fault_shunt_in_pu(fault_shunt);

        osstream<<fault.get_fault_type_string()<<" will be set for "<<lineptr->get_device_name()<<" at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl
               <<"Fault shunt is "<<fault_shunt<<" pu at location "<<location<<" to bus "<<side_bus;
        show_information_with_leading_time_stamp(osstream);


        lineptr->set_fault(side_bus,location, fault);
        guess_bus_voltage_with_line_fault_set(line_id, side_bus, location, fault);


        //network_db->save_network_matrix_to_file("dynamic_network_matrix_before_event.csv");
        //jacobian.save_matrix_to_file("dynamic_jacobian_with_fault_before_evnet.csv");
        network_db->build_dynamic_network_matrix();
        build_jacobian();
        //network_db->save_network_matrix_to_file("dynamic_network_matrix_after_event.csv");
        //jacobian.save_matrix_to_file("dynamic_jacobian_with_fault_after_event.csv");

    }
    else
    {
        osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No fault will be set at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_line_fault(DEVICE_ID line_id, size_t side_bus, double location)
{
    ostringstream osstream;

    if(line_id.get_device_type()!="LINE")
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for clearing line fault."<<endl
               <<"No line fault will be cleared at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LINE* lineptr = db->get_line(line_id);
    if(lineptr!=NULL)
    {
        if(not lineptr->is_connected_to_bus(side_bus))
        {
            osstream<<lineptr->get_device_name()<<"is not connected to bus "<<side_bus<<endl
                   <<"No fault will be cleared at location "<<location<<" to bus "<<side_bus<<" at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        if(location<0.0 or location>1.0)
        {
            osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_device_name()<<"."<<endl
                   <<"No fault will be cleared at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        osstream<<"Fault at location "<<location<<" to bus "<<side_bus<<" will be cleared for "
               <<lineptr->get_device_name()<<" at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);

        FAULT fault = lineptr->get_fault_at_location(side_bus, location);
        lineptr->clear_fault_at_location(side_bus, location);
        guess_bus_voltage_with_line_fault_cleared(line_id, side_bus, location, fault);

        network_db->build_dynamic_network_matrix();
        build_jacobian();
        //const SPARSE_MATRIX& Y = network_db->get_dynamic_network_matrix();
        //Y.report_brief();
    }
    else
    {
        osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No fault will be cleared at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_line(DEVICE_ID line_id)
{
    ostringstream osstream;

    if(line_id.get_device_type()!="LINE")
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for tripping line."<<endl
               <<"No line will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LINE* lineptr = db->get_line(line_id);
    if(lineptr!=NULL)
    {
        if(lineptr->get_sending_side_breaker_status()==true or lineptr->get_receiving_side_breaker_status()==true)
        {
            osstream<<lineptr->get_device_name()<<" is tripped by taking the following actions at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            show_information_with_leading_time_stamp(osstream);

            if(lineptr->get_sending_side_breaker_status()==true)
                trip_line_breaker(line_id, lineptr->get_sending_side_bus());

            if(lineptr->get_receiving_side_breaker_status()==true)
                trip_line_breaker(line_id, lineptr->get_receiving_side_bus());
        }
    }
    else
    {
        osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No line will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_line_breaker(DEVICE_ID line_id, size_t side_bus)
{
    ostringstream osstream;

    if(line_id.get_device_type()!="LINE")
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for tripping line breaker."<<endl
               <<"No line breaker will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LINE* lineptr = db->get_line(line_id);
    if(lineptr!=NULL)
    {
        if(lineptr->get_sending_side_bus()==side_bus)
        {
            if(lineptr->get_sending_side_breaker_status()==true)
            {
                lineptr->set_sending_side_breaker_status(false);
                network_db->build_dynamic_network_matrix();
                build_jacobian();

                osstream<<lineptr->get_device_name()<<" breaker at sending side (bus "<<lineptr->get_sending_side_bus()<<") is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            if(lineptr->get_receiving_side_bus()==side_bus)
            {
                if(lineptr->get_receiving_side_breaker_status()==true)
                {
                    lineptr->set_receiving_side_breaker_status(false);
                    network_db->build_dynamic_network_matrix();
                    build_jacobian();

                    osstream<<lineptr->get_device_name()<<" breaker at receiving side (bus "<<lineptr->get_receiving_side_bus()<<") is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                    show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }
    else
    {
        osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No line breaker will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_line(DEVICE_ID line_id)
{
    ostringstream osstream;

    if(line_id.get_device_type()!="LINE")
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for closing line."<<endl
               <<"No line will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LINE* lineptr = db->get_line(line_id);
    if(lineptr!=NULL)
    {
        if(lineptr->get_sending_side_breaker_status()==false or lineptr->get_receiving_side_breaker_status()==false)
        {
            osstream<<lineptr->get_device_name()<<" is closed by taking the following actions at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            show_information_with_leading_time_stamp(osstream);

            if(lineptr->get_sending_side_breaker_status()==false)
                close_line_breaker(line_id, lineptr->get_sending_side_bus());

            if(lineptr->get_receiving_side_breaker_status()==false)
                close_line_breaker(line_id, lineptr->get_receiving_side_bus());
        }
    }
    else
    {
        osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No line will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_line_breaker(DEVICE_ID line_id, size_t side_bus)
{
    ostringstream osstream;

    if(line_id.get_device_type()!="LINE")
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for closing line breaker"<<endl
               <<"No line breaker will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LINE* lineptr = db->get_line(line_id);
    if(lineptr!=NULL)
    {
        if(lineptr->get_sending_side_bus()==side_bus)
        {
            if(lineptr->get_sending_side_breaker_status()==false)
            {
                lineptr->set_sending_side_breaker_status(true);
                network_db->build_dynamic_network_matrix();
                build_jacobian();
                osstream<<lineptr->get_device_name()<<" breaker at sending side (bus "<<lineptr->get_sending_side_bus()<<") is closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            if(lineptr->get_receiving_side_bus()==side_bus)
            {
                if(lineptr->get_receiving_side_breaker_status()==false)
                {
                    lineptr->set_receiving_side_breaker_status(true);
                    network_db->build_dynamic_network_matrix();
                    build_jacobian();
                    osstream<<lineptr->get_device_name()<<" breaker at receiving side (bus "<<lineptr->get_sending_side_bus()<<") is closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                    show_information_with_leading_time_stamp(osstream);
                }
            }
        }
    }
    else
    {
        osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No line breaker will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_transformer(DEVICE_ID trans_id)
{
    ostringstream osstream;

    if(trans_id.get_device_type()!="TRANSFORMER")
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for tripping transformer."<<endl
               <<"No transformer will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    TRANSFORMER* transptr = db->get_transformer(trans_id);
    if(transptr!=NULL)
    {
        if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==true or transptr->get_winding_breaker_status(SECONDARY_SIDE)==true or
           (transptr->is_three_winding_transformer() and transptr->get_winding_breaker_status(TERTIARY_SIDE)==true) )
        {
            osstream<<transptr->get_device_name()<<" is tripped by taking the following actions at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            show_information_with_leading_time_stamp(osstream);

            if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==true)
                trip_transformer_breaker(trans_id, transptr->get_winding_bus(PRIMARY_SIDE));

            if(transptr->get_winding_breaker_status(SECONDARY_SIDE)==true)
                trip_transformer_breaker(trans_id, transptr->get_winding_bus(SECONDARY_SIDE));

            if(transptr->is_three_winding_transformer() and transptr->get_winding_breaker_status(TERTIARY_SIDE)==true)
                trip_transformer_breaker(trans_id, transptr->get_winding_bus(TERTIARY_SIDE));
        }
    }
    else
    {
        osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No transformer will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_transformer_breaker(DEVICE_ID trans_id, size_t side_bus)
{
    ostringstream osstream;

    if(trans_id.get_device_type()!="TRANSFORMER")
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for tripping transformer breaker."<<endl
               <<"No transformer breaker will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    TRANSFORMER* transptr = db->get_transformer(trans_id);
    if(transptr!=NULL)
    {
        if(transptr->get_winding_bus(PRIMARY_SIDE)==side_bus)
        {
            if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==true)
            {
                transptr->set_winding_breaker_status(PRIMARY_SIDE, false);
                network_db->build_dynamic_network_matrix();
                build_jacobian();

                osstream<<transptr->get_device_name()<<" breaker at primary side (bus "<<side_bus<<") is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            if(transptr->get_winding_bus(SECONDARY_SIDE)==side_bus)
            {
                if(transptr->get_winding_breaker_status(SECONDARY_SIDE)==true)
                {
                    transptr->set_winding_breaker_status(SECONDARY_SIDE, false);
                    network_db->build_dynamic_network_matrix();
                    build_jacobian();

                    osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                    show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(transptr->is_three_winding_transformer() and transptr->get_winding_bus(TERTIARY_SIDE)==side_bus)
                {
                    if(transptr->get_winding_breaker_status(TERTIARY_SIDE)==true)
                    {
                        transptr->set_winding_breaker_status(TERTIARY_SIDE, false);
                        network_db->build_dynamic_network_matrix();
                        build_jacobian();

                        osstream<<transptr->get_device_name()<<" breaker at tertiary side (bus "<<side_bus<<") is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                        show_information_with_leading_time_stamp(osstream);
                    }
                }

            }
        }
    }
    else
    {
        osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No transformer breaker will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_transformer(DEVICE_ID trans_id)
{
    ostringstream osstream;

    if(trans_id.get_device_type()!="TRANSFORMER")
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for closing transformer."<<endl
               <<"No transformer will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    TRANSFORMER* transptr = db->get_transformer(trans_id);
    if(transptr!=NULL)
    {
        if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==false or transptr->get_winding_breaker_status(SECONDARY_SIDE)==false or
           (transptr->is_three_winding_transformer() and transptr->get_winding_breaker_status(TERTIARY_SIDE)==false))
        {
            osstream<<transptr->get_device_name()<<" is closed by taking the following actions at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            show_information_with_leading_time_stamp(osstream);

            if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==false)
                close_transformer_breaker(trans_id, transptr->get_winding_bus(PRIMARY_SIDE));

            if(transptr->get_winding_breaker_status(SECONDARY_SIDE)==false)
                close_transformer_breaker(trans_id, transptr->get_winding_bus(SECONDARY_SIDE));

            if(transptr->is_three_winding_transformer() and transptr->get_winding_breaker_status(TERTIARY_SIDE)==false)
                close_transformer_breaker(trans_id, transptr->get_winding_bus(TERTIARY_SIDE));
        }
    }
    else
    {
        osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No transformer will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_transformer_breaker(DEVICE_ID trans_id, size_t side_bus)
{
    ostringstream osstream;

    if(trans_id.get_device_type()!="TRANSFORMER")
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for closing transformer breaker"<<endl
               <<"No transformer breaker will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    TRANSFORMER* transptr = db->get_transformer(trans_id);
    if(transptr!=NULL)
    {
        if(transptr->get_winding_bus(PRIMARY_SIDE)==side_bus)
        {
            if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==false)
            {
                transptr->set_winding_breaker_status(PRIMARY_SIDE, true);
                network_db->build_dynamic_network_matrix();
                build_jacobian();
                osstream<<transptr->get_device_name()<<" breaker at primary side (bus "<<side_bus<<") is closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            if(transptr->get_winding_bus(SECONDARY_SIDE)==side_bus)
            {
                if(transptr->get_winding_breaker_status(SECONDARY_SIDE)==false)
                {
                    transptr->set_winding_breaker_status(SECONDARY_SIDE, true);
                    network_db->build_dynamic_network_matrix();
                    build_jacobian();
                    osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                    show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(transptr->is_three_winding_transformer() and transptr->get_winding_bus(TERTIARY_SIDE)==side_bus)
                {
                    if(transptr->get_winding_breaker_status(TERTIARY_SIDE)==false)
                    {
                        transptr->set_winding_breaker_status(TERTIARY_SIDE, true);
                        network_db->build_dynamic_network_matrix();
                        build_jacobian();
                        osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
                        show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
    }
    else
    {
        osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No transformer breaker will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_generator(DEVICE_ID gen_id)
{
    ostringstream osstream;

    if(gen_id.get_device_type()!="GENERATOR")
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for tripping generator."<<endl
               <<"No generator will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    GENERATOR* generator = db->get_generator(gen_id);
    if(generator!=NULL)
    {
        if(generator->get_status()==true)
        {
            generator->set_status(false);

            network_db->build_dynamic_network_matrix();
            build_jacobian();

            osstream<<generator->get_device_name()<<" is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No generator will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_generator(DEVICE_ID gen_id,double percent)
{
    ostringstream osstream;

    if(gen_id.get_device_type()!="GENERATOR")
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for shedding generator."<<endl
               <<"No generator will be shed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    GENERATOR* generator = db->get_generator(gen_id);
    if(generator!=NULL)
    {
        if(fabs(percent)<FLOAT_EPSILON)
        {
            osstream<<"Generator shedding percent is 0.0 for "<<generator->get_device_name()<<endl
                   <<"No generator will be shed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        double mbase = generator->get_mbase_in_MVA();
        osstream<<generator->get_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl
          <<"MBASE is changed from "<<mbase<<" MVA to ";

        generator->set_mbase_in_MVA(mbase*(1.0-percent));
        osstream<<generator->get_mbase_in_MVA()<<" MVA.";
        show_information_with_leading_time_stamp(osstream);

        network_db->build_dynamic_network_matrix();
        build_jacobian();
    }
    else
    {
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No generator will be shed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::trip_load(DEVICE_ID load_id)
{
    ostringstream osstream;

    if(load_id.get_device_type()!="LOAD")
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for tripping load."<<endl
               <<"No load will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LOAD* load = db->get_load(load_id);
    if(load!=NULL)
    {
        if(load->get_status()==true)
        {
            load->set_status(false);

            network_db->build_dynamic_network_matrix();
            build_jacobian();

            osstream<<load->get_device_name()<<" is tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No load will be tripped at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_load(DEVICE_ID load_id)
{
    ostringstream osstream;

    if(load_id.get_device_type()!="LOAD")
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for closing load."<<endl
               <<"No load will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LOAD* load = db->get_load(load_id);
    if(load!=NULL)
    {
        if(load->get_status()==false)
        {
            load->set_status(true);

            network_db->build_dynamic_network_matrix();
            build_jacobian();

            osstream<<load->get_device_name()<<" is closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No load will be closed at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::scale_load(DEVICE_ID load_id, double percent)
{
    ostringstream osstream;

    if(load_id.get_device_type()!="LOAD")
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for scaling load."<<endl
               <<"No single load will be scaled at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    LOAD* load = db->get_load(load_id);
    if(load!=NULL)
    {
        if(fabs(percent)<FLOAT_EPSILON)
        {
            osstream<<"Load scaling percent is 0.0 for "<<load->get_device_name()<<endl
                   <<"No load will be scaled at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            show_information_with_leading_time_stamp(osstream);
            return;
        }

        if(load->get_status()==true)
        {
            osstream<<load->get_device_name()<<" is scaled "<<(percent>0.0?"up":"down")<<" by "<<percent*100.0<<"% at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
            LOAD_MODEL* load_model = load->get_load_model();
            if(load_model!=NULL)
            {
                osstream<<"Load dynamic model scale is changed from "<<load_model->get_load_scale()<<" to ";

                load_model->set_load_scale(load_model->get_load_scale()+percent);

                osstream<<load_model->get_load_scale()<<".";
                show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                complex<double> s = load->get_nominal_constant_power_load_in_MVA();
                osstream<<"Load static constant power load is changed from "<<s<<" MVA to ";

                load->set_nominal_constant_power_load_in_MVA(s*(1.0+percent));
                osstream<<load->get_nominal_constant_power_load_in_MVA()<<" MVA.";

                s = load->get_nominal_constant_current_load_in_MVA();
                osstream<<"Load static constant current load is changed from "<<s<<" MVA to ";

                load->set_nominal_constant_current_load_in_MVA(s*(1.0+percent));
                osstream<<load->get_nominal_constant_current_load_in_MVA()<<" MVA.";

                s = load->get_nominal_constant_impedance_load_in_MVA();
                osstream<<"Load static constant impedance load is changed from "<<s<<" MVA to ";

                load->set_nominal_constant_impedance_load_in_MVA(s*(1.0+percent));
                osstream<<load->get_nominal_constant_impedance_load_in_MVA()<<" MVA.";

                show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<load->get_device_name()<<" is out of service. No load scale action can be applied at time "<<get_dynamic_simulation_time_in_s()<<" s.";
            show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
               <<"No load will be scaled at time "<<get_dynamic_simulation_time_in_s()<<" s.";
        show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::scale_all_load(double percent)
{
    ostringstream osstream;
    if(fabs(percent)<FLOAT_EPSILON)
    {
        osstream<<"Load scaling percent is 0.0."<<endl
               <<"No all load will be scaled at time "<<get_dynamic_simulation_time_in_s()<<" s."<<endl;
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    POWER_SYSTEM_DATABASE* db = get_power_system_database();

    vector<DEVICE_ID> loads = db->get_all_loads_device_id();
    DEVICE_ID load;
    size_t n = loads.size();
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        LOAD* loadptr = db->get_load(load);
        if(loadptr==NULL)
            continue;

        if(loadptr->get_status()==false)
            continue;

        scale_load(load, percent);
    }
}


void DYNAMICS_SIMULATOR::manual_bypass_hvdc(DEVICE_ID hvdc_id)
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    HVDC* hvdc = db->get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL)
        {
            if(not model->is_blocked() and not model->is_bypassed())
            {
                model->manual_bypass_hvdc();
            }
        }
    }
}

void DYNAMICS_SIMULATOR::manual_unbypass_hvdc(DEVICE_ID hvdc_id)
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    HVDC* hvdc = db->get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL)
        {
            if(not model->is_blocked() and model->is_manual_bypassed())
            {
                model->manual_unbypass_hvdc();
            }
        }
    }
}

void DYNAMICS_SIMULATOR::manual_block_hvdc(DEVICE_ID hvdc_id)
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    HVDC* hvdc = db->get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL)
        {
            if(not model->is_blocked())
            {
                model->manual_block_hvdc();
            }
        }
    }
}

void DYNAMICS_SIMULATOR::manual_unblock_hvdc(DEVICE_ID hvdc_id)
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    HVDC* hvdc = db->get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL)
        {
            if(model->is_manual_blocked())
            {
                model->manual_unblock_hvdc();
            }
        }
    }
}


void DYNAMICS_SIMULATOR::change_generator_voltage_reference_in_pu(DEVICE_ID gen_id, double vref)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    GENERATOR* generator = db->get_generator(gen_id);
    if(generator != NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter!=NULL)
            exciter->set_voltage_reference_in_pu(vref);
    }
}

void DYNAMICS_SIMULATOR::change_generator_power_reference_in_MW(DEVICE_ID gen_id, double Pref)
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    GENERATOR* generator = db->get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
            tg->set_mechanical_power_reference_in_pu_based_on_mbase(Pref);
    }
}

void DYNAMICS_SIMULATOR::switch_on_equivalent_device()
{
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    vector<EQUIVALENT_DEVICE*> edevices = db->get_all_equivalent_devices();
    size_t n = edevices.size();
    ostringstream osstream;
    osstream<<"There are "<<n<<" equivalent devices to switch on";
    show_information_with_leading_time_stamp(osstream);
    EQUIVALENT_DEVICE* edevice;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        edevice->switch_on();
    }
}

bool DYNAMICS_SIMULATOR::is_valid() const
{
    return true; // disabled
}
void DYNAMICS_SIMULATOR::check()
{
    ;// disabled
}
