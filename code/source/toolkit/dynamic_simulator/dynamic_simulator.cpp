#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <ctime>
//#include <omp.h>

using namespace std;

DYNAMICS_SIMULATOR::DYNAMICS_SIMULATOR()
{
    DELT = 0.01;
    clear();
    check_NAN = true;
}

DYNAMICS_SIMULATOR::~DYNAMICS_SIMULATOR()
{
    stop();
}

void DYNAMICS_SIMULATOR::clear()
{
    clear_meters();

    set_output_file("");

    close_meter_output_files();

    set_bin_file_export_enable_flag(true);
    set_csv_file_export_enable_flag(false);
    set_json_file_export_enable_flag(false);

    set_dynamic_simulation_time_in_s(0.0);

    set_max_DAE_iteration(200);
    set_max_network_iteration(1);
    set_max_update_event_iteration(200);
    set_allowed_max_power_imbalance_in_MVA(0.001);
    set_iteration_accelerator(1.0);
    set_rotor_angle_stability_survilliance_flag(false);
    set_rotor_angle_stability_threshold_in_deg(360.0);
    generators_in_islands.clear();
}

void DYNAMICS_SIMULATOR::set_dynamic_simulation_time_step_in_s(double delt)
{
    DELT = delt;
    ostringstream osstream;
    osstream<<"System dynamic simulation time step is set as :"<<DELT<<" s.";
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

double DYNAMICS_SIMULATOR::get_dynamic_simulation_time_step_in_s()
{
    return DELT;
}

void DYNAMICS_SIMULATOR::set_dynamic_simulation_time_in_s(double time)
{
    TIME = time;
}

double DYNAMICS_SIMULATOR::get_dynamic_simulation_time_in_s()
{
    return TIME;
}

NETWORK_MATRIX& DYNAMICS_SIMULATOR::get_network_matrix()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
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

void DYNAMICS_SIMULATOR::set_max_update_event_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_update_event_iteration = iteration;
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

void DYNAMICS_SIMULATOR::set_rotor_angle_stability_survilliance_flag(bool flag)
{
    this->flag_rotor_angle_stability_survilliance = flag;
}

void DYNAMICS_SIMULATOR::set_rotor_angle_stability_threshold_in_deg(double angle_th)
{
    if(angle_th<0.0) angle_th = - angle_th;
    this->rotor_angle_stability_threshold_in_deg = angle_th;
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

size_t DYNAMICS_SIMULATOR::get_max_update_event_iteration() const
{
    return max_update_event_iteration;
}

double DYNAMICS_SIMULATOR::get_allowed_max_power_imbalance_in_MVA() const
{
    return P_threshold_in_MW;
}

double DYNAMICS_SIMULATOR::get_iteration_accelerator() const
{
    return alpha;
}

bool DYNAMICS_SIMULATOR::get_rotor_angle_stability_survilliance_flag() const
{
    return this->flag_rotor_angle_stability_survilliance;
}

double DYNAMICS_SIMULATOR::get_rotor_angle_stability_threshold_in_deg() const
{
    return this->rotor_angle_stability_threshold_in_deg;
}

void DYNAMICS_SIMULATOR::append_meter(const METER& meter)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    if(meter.is_valid())
    {
        meters.push_back(meter);
        //os<< "Meter added. %s.",(meter.get_meter_name()).c_str());
        //show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Invalid meter is given. No meter will be set.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_meters()
{
    prepare_bus_related_meters();
    prepare_generator_related_meters();
    prepare_wt_generator_related_meters();
    prepare_pv_unit_related_meters();
    prepare_energy_storage_related_meters();
    prepare_load_related_meters();
    prepare_line_related_meters();
    prepare_hvdc_related_meters();
    prepare_equivalent_device_related_meters();
}


void DYNAMICS_SIMULATOR::prepare_bus_related_meters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n = psdb.get_bus_count();
    vector<BUS*> buses = psdb.get_all_buses();
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_generator_count();
    vector<GENERATOR*> generators = psdb.get_all_generators();
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
        METER meter = setter.prepare_generator_mechanical_power_reference_in_MW_meter(generator->get_device_id());
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_wt_generator_count();
    vector<WT_GENERATOR*> generators = psdb.get_all_wt_generators();
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


void DYNAMICS_SIMULATOR::prepare_pv_unit_related_meters()
{
    return;
}

void DYNAMICS_SIMULATOR::prepare_energy_storage_related_meters()
{
    return;
}

void DYNAMICS_SIMULATOR::prepare_load_related_meters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_load_count();
    vector<LOAD*> loads = psdb.get_all_loads();
    LOAD* load;
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        METER meter = setter.prepare_load_active_power_in_MW_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_reactive_power_in_MVar_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_total_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_manually_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
        meter = setter.prepare_load_relay_shed_scale_in_pu_meter(load->get_device_id());
        append_meter(meter);
    }

}

void DYNAMICS_SIMULATOR::prepare_line_related_meters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_line_count();
    vector<LINE*> lines = psdb.get_all_lines();
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_hvdc_count();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_equivalent_device_count();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
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

void DYNAMICS_SIMULATOR::prepare_bus_related_meter(size_t bus, string meter_type)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(psdb.is_bus_exist(bus))
    {
        METER_SETTER setter;
        setter.set_toolkit(toolkit);

        METER meter;
        meter.set_toolkit(toolkit);

        meter_type = string2upper(meter_type);

        if(meter_type=="VOLTAGE IN PU")
            meter = setter.prepare_bus_voltage_in_pu_meter(bus);
        if(meter_type=="ANGLE IN DEG")
            meter = setter.prepare_bus_angle_in_deg_meter(bus);
        if(meter_type=="ANGLE IN RAD")
            meter = setter.prepare_bus_angle_in_rad_meter(bus);
        if(meter_type=="FREQUENCY DEVIATION IN PU")
            meter = setter.prepare_bus_frequency_deviation_in_pu_meter(bus);
        if(meter_type=="FREQUENCY IN PU")
            meter = setter.prepare_bus_frequency_in_pu_meter(bus);
        if(meter_type=="FREQUENCY IN HZ")
            meter = setter.prepare_bus_frequency_in_Hz_meter(bus);

        if(meter.is_valid())
            append_meter(meter);
        else
        {
            osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for bus "<<bus;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. Meter of bus "<<bus<<" cannot be set since the given bus does not exist in the power system database";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="GENERATOR")
    {
        if(psdb.is_generator_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="ROTOR ANGLE IN DEG")
                meter = setter.prepare_generator_rotor_angle_in_deg_meter(did);
            if(meter_type=="ROTOR SPEED DEVIATION IN PU")
                meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(did);
            if(meter_type=="EXCITATION VOLTAGE IN PU")
                meter = setter.prepare_generator_excitation_voltage_in_pu_meter(did);
            if(meter_type=="MECHANICAL POWER IN MW")
                meter = setter.prepare_generator_mechanical_power_in_MW_meter(did);
            if(meter_type=="MECHANICAL POWER REFERENCE IN MW")
                meter = setter.prepare_generator_mechanical_power_reference_in_MW_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_generator_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="SYNC GENERATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_sync_generator_model_internal_variable_meter(did, var_name);
            if(meter_type=="COMPENSATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_compensator_model_internal_variable_meter(did, var_name);
            if(meter_type=="EXCITER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_exciter_model_internal_variable_meter(did, var_name);
            if(meter_type=="STABILIZER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_stabilizer_model_internal_variable_meter(did, var_name);
            if(meter_type=="TURBINE GOVERNOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_generator_turbine_governor_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not GENERATOR when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="WT GENERATOR")
    {
        if(psdb.is_wt_generator_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="TERMINAL CURRENT IN KA")
                meter = setter.prepare_wt_generator_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_wt_generator_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_wt_generator_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="MECHANICAL POWER IN MW")
                meter = setter.prepare_wt_generator_mechanical_power_in_MW_meter(did);
            if(meter_type=="MAX AVAILABLE MECHANICAL POWER IN MW")
                meter = setter.prepare_wt_generator_max_available_mechanical_power_in_MW_meter(did);
            if(meter_type=="SPEED REFERENCE IN PU")
                meter = setter.prepare_wt_generator_speed_reference_in_pu_meter(did);
            if(meter_type=="TURBINE SPEED IN PU")
                meter = setter.prepare_wt_generator_turbine_speed_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED IN PU")
                meter = setter.prepare_wt_generator_rotor_speed_in_pu_meter(did);
            if(meter_type=="ROTOR ANGLE IN DEG")
                meter = setter.prepare_wt_generator_rotor_angle_in_deg_meter(did);
            if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_wt_generator_active_current_command_in_pu_meter(did);
            if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_current_command_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_wt_generator_active_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_voltage_command_in_pu_meter(did);
            if(meter_type=="PITCH ANGLE IN DEG")
                meter = setter.prepare_wt_generator_pitch_angle_in_deg_meter(did);
            if(meter_type=="WIND SPEED IN PU")
                meter = setter.prepare_wt_generator_wind_speed_in_pu_meter(did);
            if(meter_type=="WIND SPEED IN MPS")
                meter = setter.prepare_wt_generator_wind_speed_in_mps_meter(did);
            if(meter_type=="WT GENERATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_generator_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT AERODYNAMIC MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_aerodynamic_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT TURBINE MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_turbine_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT ELECTRICAL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_electrical_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT PITCH MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_pitch_model_internal_variable_meter(did, var_name);
            if(meter_type=="WIND SPEED MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wind_speed_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not WT GENERATOR when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="PV UNIT")
    {
        if(psdb.is_pv_unit_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="TERMINAL CURRENT IN KA")
                meter = setter.prepare_wt_generator_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_wt_generator_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_wt_generator_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="MECHANICAL POWER IN MW")
                meter = setter.prepare_wt_generator_mechanical_power_in_MW_meter(did);
            if(meter_type=="MAX AVAILABLE MECHANICAL POWER IN MW")
                meter = setter.prepare_wt_generator_max_available_mechanical_power_in_MW_meter(did);
            if(meter_type=="SPEED REFERENCE IN PU")
                meter = setter.prepare_wt_generator_speed_reference_in_pu_meter(did);
            if(meter_type=="TURBINE SPEED IN PU")
                meter = setter.prepare_wt_generator_turbine_speed_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED IN PU")
                meter = setter.prepare_wt_generator_rotor_speed_in_pu_meter(did);
            if(meter_type=="ROTOR ANGLE IN DEG")
                meter = setter.prepare_wt_generator_rotor_angle_in_deg_meter(did);
            if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_wt_generator_active_current_command_in_pu_meter(did);
            if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_current_command_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_wt_generator_active_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_voltage_command_in_pu_meter(did);
            if(meter_type=="PITCH ANGLE IN DEG")
                meter = setter.prepare_wt_generator_pitch_angle_in_deg_meter(did);
            if(meter_type=="WIND SPEED IN PU")
                meter = setter.prepare_wt_generator_wind_speed_in_pu_meter(did);
            if(meter_type=="WIND SPEED IN MPS")
                meter = setter.prepare_wt_generator_wind_speed_in_mps_meter(did);
            if(meter_type=="WT GENERATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_generator_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT AERODYNAMIC MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_aerodynamic_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT TURBINE MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_turbine_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT ELECTRICAL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_electrical_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT PITCH MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_pitch_model_internal_variable_meter(did, var_name);
            if(meter_type=="WIND SPEED MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wind_speed_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not PV UNIT when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="ENERGY STORAGE")
    {
        if(psdb.is_pv_unit_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="TERMINAL CURRENT IN KA")
                meter = setter.prepare_wt_generator_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_wt_generator_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_wt_generator_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="MECHANICAL POWER IN MW")
                meter = setter.prepare_wt_generator_mechanical_power_in_MW_meter(did);
            if(meter_type=="MAX AVAILABLE MECHANICAL POWER IN MW")
                meter = setter.prepare_wt_generator_max_available_mechanical_power_in_MW_meter(did);
            if(meter_type=="SPEED REFERENCE IN PU")
                meter = setter.prepare_wt_generator_speed_reference_in_pu_meter(did);
            if(meter_type=="TURBINE SPEED IN PU")
                meter = setter.prepare_wt_generator_turbine_speed_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED IN PU")
                meter = setter.prepare_wt_generator_rotor_speed_in_pu_meter(did);
            if(meter_type=="ROTOR ANGLE IN DEG")
                meter = setter.prepare_wt_generator_rotor_angle_in_deg_meter(did);
            if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_wt_generator_active_current_command_in_pu_meter(did);
            if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_current_command_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_wt_generator_active_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
                meter = setter.prepare_wt_generator_reactive_voltage_command_in_pu_meter(did);
            if(meter_type=="PITCH ANGLE IN DEG")
                meter = setter.prepare_wt_generator_pitch_angle_in_deg_meter(did);
            if(meter_type=="WIND SPEED IN PU")
                meter = setter.prepare_wt_generator_wind_speed_in_pu_meter(did);
            if(meter_type=="WIND SPEED IN MPS")
                meter = setter.prepare_wt_generator_wind_speed_in_mps_meter(did);
            if(meter_type=="WT GENERATOR MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_generator_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT AERODYNAMIC MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_aerodynamic_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT TURBINE MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_turbine_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT ELECTRICAL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_electrical_model_internal_variable_meter(did, var_name);
            if(meter_type=="WT PITCH MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wt_pitch_model_internal_variable_meter(did, var_name);
            if(meter_type=="WIND SPEED MODEL INTERNAL VARIABLE")
                meter = setter.prepare_wind_speed_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not ENERGY STORAGE when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="LOAD")
    {
        if(psdb.is_load_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="ACTIVE POWER IN MW")
                meter = setter.prepare_load_active_power_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER IN MVAR")
                meter = setter.prepare_load_reactive_power_in_MVar_meter(did);
            if(meter_type=="TOTAL SCALE IN PU")
                meter = setter.prepare_load_total_scale_in_pu_meter(did);
            if(meter_type=="MANUALLY SCALE IN PU")
                meter = setter.prepare_load_manually_scale_in_pu_meter(did);
            if(meter_type=="RELAY SHED SCALE IN PU")
                meter = setter.prepare_load_relay_shed_scale_in_pu_meter(did);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not LOAD when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="LINE")
    {
        if(psdb.is_line_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            LINE* line = psdb.get_line(did);
            size_t bus = 0;
            if(side=="SENDING" or side=="S")
                bus = line->get_sending_side_bus();
            if(side=="RECEIVING" or side=="R")
                bus = line->get_receiving_side_bus();

            if(meter_type=="ACTIVE POWER IN MW")
                meter = setter.prepare_line_active_power_in_MW_meter(did, bus);
            if(meter_type=="REACTIVE POWER IN MVAR")
                meter = setter.prepare_line_reactive_power_in_MVar_meter(did, bus);
            if(meter_type=="CURRENT IN KA")
                meter = setter.prepare_line_current_in_kA_meter(did, bus);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not LINE when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
}


void DYNAMICS_SIMULATOR::prepare_hvdc_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="HVDC")
    {
        if(psdb.is_hvdc_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            if(side=="RECTIFIER" or side=="R")
            {
                if(meter_type=="AC VOLTAGE IN PU")
                    meter = setter.prepare_hvdc_rectifier_ac_voltage_in_pu_meter(did);
                if(meter_type=="DC VOLTAGE IN KV")
                    meter = setter.prepare_hvdc_rectifier_dc_voltage_in_kV_meter(did);
                if(meter_type=="DC CURRENT IN KA")
                    meter = setter.prepare_hvdc_rectifier_dc_current_in_kA_meter(did);
                if(meter_type=="ALPHA IN DEG")
                    meter = setter.prepare_hvdc_rectifier_alpha_in_deg_meter(did);
                if(meter_type=="MU IN DEG")
                    meter = setter.prepare_hvdc_rectifier_mu_in_deg_meter(did);
                if(meter_type=="DC POWER IN MW")
                    meter = setter.prepare_hvdc_rectifier_dc_power_in_MW_meter(did);
                if(meter_type=="AC ACTIVE POWER IN MW")
                    meter = setter.prepare_hvdc_rectifier_ac_active_power_in_MW_meter(did);
                if(meter_type=="AC REACTIVE POWER IN MVAR")
                    meter = setter.prepare_hvdc_rectifier_ac_reactive_power_in_MVar_meter(did);
                if(meter_type=="AC CURRENT IN KA")
                    meter = setter.prepare_hvdc_rectifier_ac_current_in_kA_meter(did);
            }

            if(side=="INVERTER" or side=="I")
            {
                if(meter_type=="AC VOLTAGE IN PU")
                    meter = setter.prepare_hvdc_inverter_ac_voltage_in_pu_meter(did);
                if(meter_type=="DC VOLTAGE IN KV")
                    meter = setter.prepare_hvdc_inverter_dc_voltage_in_kV_meter(did);
                if(meter_type=="DC CURRENT IN KA")
                    meter = setter.prepare_hvdc_inverter_dc_current_in_kA_meter(did);
                if(meter_type=="GAMMA IN DEG")
                    meter = setter.prepare_hvdc_inverter_gamma_in_deg_meter(did);
                if(meter_type=="MU IN DEG")
                    meter = setter.prepare_hvdc_inverter_mu_in_deg_meter(did);
                if(meter_type=="DC POWER IN MW")
                    meter = setter.prepare_hvdc_inverter_dc_power_in_MW_meter(did);
                if(meter_type=="AC ACTIVE POWER IN MW")
                    meter = setter.prepare_hvdc_inverter_ac_active_power_in_MW_meter(did);
                if(meter_type=="AC REACTIVE POWER IN MVAR")
                    meter = setter.prepare_hvdc_inverter_ac_reactive_power_in_MVar_meter(did);
                if(meter_type=="AC CURRENT IN KA")
                    meter = setter.prepare_hvdc_inverter_ac_current_in_kA_meter(did);
            }
            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not HVDC when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="EQUIVALENT DEVICE")
    {
        if(psdb.is_equivalent_device_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="ACTIVE POWER GENERATION IN MW")
                meter = setter.prepare_equivalent_device_active_power_generation_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER GENERATION IN MVAR")
                meter = setter.prepare_equivalent_device_reactive_power_generation_in_MVar_meter(did);
            if(meter_type=="ACTIVE POWER LOAD IN MW")
                meter = setter.prepare_equivalent_device_active_power_load_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_reactive_power_load_in_MVar_meter(did);
            if(meter_type=="ACTIVE POWER NET LOAD IN MW")
                meter = setter.prepare_equivalent_device_active_power_net_load_in_MW_meter(did);
            if(meter_type=="REACTIVE POWER NET LOAD IN MVAR")
                meter = setter.prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(did);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_device_name();
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not EQUIVALENT DEVICE when setting up meter with "<<__FUNCTION__;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


size_t DYNAMICS_SIMULATOR::get_meter_count() const
{
    return meters.size();
}

METER DYNAMICS_SIMULATOR::get_meter(size_t i)
{
    METER voidmeter;
    voidmeter.set_toolkit(get_toolkit(__PRETTY_FUNCTION__));

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
    size_t n = meters.size();
    if(n!=0)
    {
        if(meter_values.size()!=n)
            meter_values.resize(n,0.0);

        for(size_t i=0; i!=n; ++i)
            meter_values[i]=meters[i].get_meter_value();
        /*
        if(meter_values.size()==n)
        {
            for(size_t i=0; i!=n; ++i)
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
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

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    size_t n = meters.size();
    //osstream<<"save meter information: "<<n<<" meters were set. output file name is: "<<output_filename;
    //toolkit.show_information_with_leading_time_stamp(osstream);
    if(n!=0)
    {
        open_meter_output_files();

        if(csv_output_file.is_open() or json_output_file.is_open() or bin_output_file.is_open())
        {
            // save header to csv file
            if(is_csv_file_export_enabled() and csv_output_file.is_open())
            {
                //csv_output_file<<"TIME,ITERATION,MISMATCH IN MVA";
                //for(size_t i=0; i!=n; ++i)
                //    csv_output_file<<","<<meters[i].get_meter_type();
                //csv_output_file<<endl;

                csv_output_file<<"TIME,DAE INTEGRATION,NETWORK ITERATION,MISMATCH IN MVA";

                for(size_t i=0; i!=n; ++i)
                    csv_output_file<<","<<meters[i].get_meter_name();
                csv_output_file<<"\n";
            }

            // save header to json file
            if(is_json_file_export_enabled() and json_output_file.is_open())
            {
                json_output_file<<"{"<<"\n";

                //json_output_file<<"    \"meter_type\" : [\"TIME\", \"ITERATION\", \"MISMATCH IN MVA\"";
                //for(size_t i=0; i!=n; ++i)
                //    json_output_file<<", \""<<meters[i].get_meter_type()<<"\"";
                //json_output_file<<"],"<<endl<<endl;

                json_output_file<<"    \"meter_name\" : [\"TIME\", \"DAE INTEGRATION\", \"NETWORK ITERATION\", \"MISMATCH IN MVA\"";
                for(size_t i=0; i!=n; ++i)
                    json_output_file<<", \""<<meters[i].get_meter_name()<<"\"";
                json_output_file<<"],"<<"\n\n";
                json_output_file<<"    \"meter_value\" : ["<<"\n";
            }

            // save header to bin file

            if(is_bin_file_export_enabled() and bin_output_file.is_open())
            {
                size_t bin_version=0;
                bin_output_file.write((char *)(&bin_version), sizeof(bin_version));

                time_t tt = time(NULL);
                tm* local_time= localtime(&tt);

                size_t year = local_time->tm_year + 1900;
                size_t month = local_time->tm_mon + 1;
                size_t day = local_time->tm_mday;
                size_t hour = local_time->tm_hour;
                size_t minute = local_time->tm_min;
                size_t second = local_time->tm_sec;

                bin_output_file.write((char *)(&year), sizeof(year));
                bin_output_file.write((char *)(&month), sizeof(month));
                bin_output_file.write((char *)(&day), sizeof(day));
                bin_output_file.write((char *)(&hour), sizeof(hour));
                bin_output_file.write((char *)(&minute), sizeof(minute));
                bin_output_file.write((char *)(&second), sizeof(second));

                size_t float_size=sizeof(float);
                bin_output_file.write((char *)(&float_size), sizeof(float_size));

                size_t m = 4+n;
                bin_output_file.write((char *)(&m), sizeof(m));
                string meter_names ="";

                meter_names += "TIME\n";
                meter_names += "DAE INTEGRATION\n";
                meter_names += "NETWORK INTEGRATION\n";
                meter_names += "MISMATCH IN MVA\n";
                for(size_t i=0; i!=n; ++i)
                    meter_names += (meters[i].get_meter_name()+"\n");

                size_t n_meter_string_size = strlen(meter_names.c_str());
                bin_output_file.write((char *)(&n_meter_string_size), sizeof(n_meter_string_size));

                char * pmeter_name = new char[n_meter_string_size+1];
                sprintf(pmeter_name, meter_names.c_str());
                pmeter_name[n_meter_string_size]='\0';

                bin_output_file.write((char *)(pmeter_name), strlen(pmeter_name));
                delete [] pmeter_name;
            }
        }
    }
}

void DYNAMICS_SIMULATOR::save_meter_values()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = meters.size();
    if(n!=0 and (csv_output_file.is_open() or json_output_file.is_open() or bin_output_file.is_open()))
    {
        get_bus_current_mismatch();
        get_bus_power_mismatch_in_MVA();
        double smax = get_max_power_mismatch_in_MVA();

        update_all_meters_value();

        if(is_bin_file_export_enabled() and bin_output_file.is_open())
        {
            float fvalue=0.0;

            fvalue = toolkit.get_dynamic_simulation_time_in_s();
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            fvalue = float(ITER_DAE);
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            fvalue = float(ITER_NET);
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            fvalue = smax;
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            for (size_t i = 0; i != n; ++i)
            {
                fvalue = meter_values[i];
                bin_output_file.write((char *)(&fvalue), sizeof(fvalue));
            }
        }

        if(is_csv_file_export_enabled() or is_json_file_export_enabled())
        {
            ostringstream osstream;
            //osstream<<setw(8)<<setprecision(4)<<fixed<<toolkit.get_dynamic_simulation_time_in_s()<<","
            //  <<setw(3)<<ITER_DAE<<","<<setw(3)<<ITER_NET<<","
            //  <<setw(6)<<setprecision(3)<<fixed<<smax;

            char temp_buffer[50];
            string temp_str = "";
            snprintf(temp_buffer, 50, "%8.4f",toolkit.get_dynamic_simulation_time_in_s());
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%3lu",ITER_DAE);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%3lu",ITER_NET);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",smax);
            temp_str += temp_buffer;
            for (size_t i = 0; i != n; ++i)
            {
                snprintf(temp_buffer, 50, ",%16.12f", meter_values[i]);
                temp_str += temp_buffer;
            }
            osstream << temp_str;

            /*for(size_t i=0; i!=n; ++i)
                osstream<<","<<setw(16)<<setprecision(12)<<fixed<<meter_values[i];*/

            if(is_csv_file_export_enabled() and csv_output_file.is_open())
                csv_output_file<<osstream.str()<<endl;

            if(is_json_file_export_enabled() and json_output_file.is_open())
                json_output_file<<","<<endl
                                <<"                       ["<<osstream.str()<<"]";
        }
    }
}

void DYNAMICS_SIMULATOR::start()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWERFLOW_SOLVER& pf_solver = toolkit.get_powerflow_solver();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;
    osstream<<"Dynamics initialization starts.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    if(not pf_solver.is_converged())
    {
        osstream<<"Warning. Powerflow is not converged. Please go check powerflow solution.\n"
                <<"Any further simulation cannot be trusted.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    meter_values.resize(meters.size(), 0.0);

    toolkit.set_dynamic_simulation_time_in_s(-2.0*toolkit.get_dynamic_simulation_time_step_in_s());

    network_matrix.optimize_network_ordering();

    run_all_models(INITIALIZE_MODE);

    network_matrix.build_dynamic_network_matrix();
    build_jacobian();

    //const SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_matrix();
    //Y.report_brief();
    //network_matrix.report_physical_internal_bus_number_pair();


    if(get_rotor_angle_stability_survilliance_flag()==true)
        update_generators_in_islands();

    ITER_DAE = 0;
    ITER_NET = 0;

    bool converged = false;
    size_t iter_count  = 0;
    while(true)
    {
		++iter_count;
        converged = solve_network();
        ITER_NET += network_iteration_count;
        if(converged or iter_count>200)
            break;
    }
    ITER_DAE = iter_count;
    if(ITER_NET>1)
    {
        osstream<<"Warning. Network solution converged in "<<ITER_NET<<" iterations for dynamics simulation initialization.\n"
                <<"Any solution with more than 1 iteration is indicating bad initial powerflow result or exceeding dynamic upper or lower limits.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    osstream<<"Dynamics initialization finished.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    save_meter_information();
    save_meter_values();
}

void DYNAMICS_SIMULATOR::stop()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    //if(toolkit.get_dynamic_simulation_time_in_s() >0.0)
    ostringstream osstream;
    osstream<<"Dynamics simulation stops at simulation time: "<<toolkit.get_dynamic_simulation_time_in_s()<<"s.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    if(json_output_file.is_open())
        json_output_file<<endl
                        <<"                    ]"<<endl
                        <<"}";
    close_meter_output_files();

    //toolkit.set_dynamic_simulation_time_in_s(-2.0*toolkit.get_dynamic_simulation_time_step_in_s());
}


double DYNAMICS_SIMULATOR::get_system_max_angle_difference_in_deg()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<GENERATOR*> gens = psdb.get_all_generators();
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    update_with_event();
    if(get_rotor_angle_stability_survilliance_flag()==false)
    {
        while(toolkit.get_dynamic_simulation_time_in_s()<=time-FLOAT_EPSILON)
            run_a_step();
    }
    else
    {
        while(toolkit.get_dynamic_simulation_time_in_s()<=time-FLOAT_EPSILON)
        {
            run_a_step();

            update_generators_in_islands();
            bool stable = is_system_angular_stable();
            if(stable==true)
                ;
            else
            {
                ostringstream osstream;
                osstream<<"At time "<<toolkit.get_dynamic_simulation_time_in_s()<<"s, "
                        <<"system is detected to be unstable due to rotor angular stability survilliance logic."<<endl
                        <<"No further simulation will be performed.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
}

void DYNAMICS_SIMULATOR::run_a_step()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    toolkit.set_dynamic_simulation_time_in_s(toolkit.get_dynamic_simulation_time_in_s()+ toolkit.get_dynamic_simulation_time_step_in_s());
    if(toolkit.is_detailed_log_enabled())
    {
        osstream<<"Run dynamic simulation at time "<<toolkit.get_dynamic_simulation_time_in_s();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    update_equivalent_devices_buffer();
    update_equivalent_devices_output();

    //bool network_converged = false;
    //bool DAE_converged = false;
    ITER_DAE = 0;
    ITER_NET = 0;
    double max_angle_difference_old = get_system_max_angle_difference_in_deg();
    while(true)
    {
        ++ITER_DAE;
        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<"Solve DAE with iteration "<<ITER_DAE<<" at time "<<toolkit.get_dynamic_simulation_time_in_s();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        if(ITER_DAE<=max_DAE_iteration)
        {
            integrate();
            bool network_converged = solve_network();
            ITER_NET += network_iteration_count;

            double max_angle_difference_new = get_system_max_angle_difference_in_deg();
            if(toolkit.is_detailed_log_enabled())
            {
                osstream<<"maximum angle difference with iteration "<<ITER_DAE<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" is "<<max_angle_difference_new;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(toolkit.is_detailed_log_enabled() and (not network_converged))
            {
                osstream<<"Failed to solve network in "<<max_network_iteration<<" iterations during DAE iteration "<<ITER_DAE<<" when integrating at time "<<TIME<<" s."
                        <<"New max angle difference is "<<max_angle_difference_new<<" deg while old is "<<max_angle_difference_old<<" deg (difference is "<<max_angle_difference_new-max_angle_difference_old<<" deg)";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(fabs(max_angle_difference_new-max_angle_difference_old)>1e-8) // DAE solution not converged
                max_angle_difference_old = max_angle_difference_new;
            else//converged
                break;

        }
        else
        {
            if(max_DAE_iteration!=2)
            {
                char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve DAE in %lu iterations when integrating at time %f s.",
                         max_DAE_iteration, toolkit.get_dynamic_simulation_time_in_s());
                toolkit.show_information_with_leading_time_stamp(buffer);
            }
            --ITER_DAE;
            break;
        }
    }
    update();
    update_relay_models();
    save_meter_values();
}

void DYNAMICS_SIMULATOR::update_with_event()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    bool network_converged = false;
    size_t update_event_iter_max = get_max_update_event_iteration();

    ITER_DAE = 0;
    ITER_NET = 0;

    size_t iter = 0;
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
                char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve network in %lu iterations when updating with event at time %f s.",
                         update_event_iter_max, toolkit.get_dynamic_simulation_time_in_s());
                toolkit.show_information_with_leading_time_stamp(buffer);
                break;
            }
        }
        else
            break;
    }
    update_bus_frequency_blocks();
    update();
    save_meter_values();
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    //clock_t start = clock();
    update_equivalent_devices_buffer();
    run_all_models(UPDATE_MODE);
    //cout<<"    elapsed time for update: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    //start = clock();

    size_t network_iter_max = get_max_network_iteration();
    bool network_converged = false;

    network_converged = solve_network();
    ITER_NET += network_iteration_count;

    if(not network_converged and get_max_network_iteration()>1)
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve network in %lu iterations when updating at time %f s.",
                 network_iter_max, toolkit.get_dynamic_simulation_time_in_s());
        toolkit.show_information_with_leading_time_stamp(buffer);
    }
    update_bus_frequency_blocks();

    //cout<<"    elapsed time for export meters: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
}

void DYNAMICS_SIMULATOR::update_relay_models()
{
    disable_relay_action_flag();
    run_all_models(RELAY_MODE);

    if(get_relay_actiion_flag()==true)
        update();
}

void DYNAMICS_SIMULATOR::run_all_models(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    size_t n = psdb.get_generator_count();
    vector<GENERATOR*> generators = psdb.get_all_generators();
    GENERATOR* generator;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        generator = generators[i];
        if(generator->get_status()==true)
            generator->run(mode);
    }

    n = psdb.get_wt_generator_count();
    vector<WT_GENERATOR*> wtgens = psdb.get_all_wt_generators();
    WT_GENERATOR* wtgen;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        wtgen = wtgens[i];
        if(wtgen->get_status()==true)
            wtgen->run(mode);
    }

    n = psdb.get_load_count();
    vector<LOAD*> loads = psdb.get_all_loads();
    LOAD* load;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        load = loads[i];
        if(load->get_status()==true)
            load->run(mode);
    }

    n = psdb.get_hvdc_count();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    HVDC* hvdc;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
            hvdc->run(mode);
    }

    n = psdb.get_equivalent_device_count();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
    EQUIVALENT_DEVICE* edevice;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        if(edevice->get_status()==true)
            edevice->run(mode);
    }

    if(mode==INITIALIZE_MODE or mode==INTEGRATE_MODE or mode==UPDATE_MODE)
    {
        vector<BUS*> buses = psdb.get_all_in_service_buses();
        n = buses.size();
        BUS* bus;
        //#pragma omp parallel for
        for(size_t i=0; i<n; ++i)
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
}

void DYNAMICS_SIMULATOR::update_bus_frequency_blocks()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<BUS*> buses = psdb.get_all_in_service_buses();
    size_t n = buses.size();
    BUS* bus;
    //#pragma omp parallel for
    for(size_t i=0; i!=n; ++i)
    {
        bus = buses[i];
        if(bus->get_bus_type()!=OUT_OF_SERVICE)
        {
            BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
            model->update_for_applying_event();
        }
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

bool DYNAMICS_SIMULATOR::get_relay_actiion_flag() const
{
    return relay_action_flag;
}

void DYNAMICS_SIMULATOR::update_equivalent_devices_buffer()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
    size_t n = edevices.size();
    EQUIVALENT_DEVICE* edevice;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        edevice->update_meter_buffer();
    }
}

void DYNAMICS_SIMULATOR::update_equivalent_devices_output()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
    size_t n = edevices.size();
    EQUIVALENT_DEVICE* edevice;
    ostringstream osstream;
    for(size_t i=0; i!=n; ++i)
    {
        edevice = edevices[i];
        edevice->update_equivalent_outputs();

        osstream<<"At time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s, equivalent load of "<<edevice->get_device_name()<<" is: "<<edevice->get_total_equivalent_power_as_load_in_MVA()<<"MVA";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

bool DYNAMICS_SIMULATOR::solve_network()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    //clock_t start = clock(), start0 = clock();
    //osstream<<"Now go solve network with DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"() at time "<<toolkit.get_dynamic_simulation_time_in_s()<<"s";
    //toolkit.show_information_with_leading_time_stamp(osstream);

    size_t nbus = psdb.get_in_service_bus_count();
    /*for(int i=0; i!=nbus; ++i)
    {
        size_t bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        complex<double> V = psdb.get_bus_complex_voltage_in_pu(bus);
        osstream<<"Initial voltage of bus "<<bus<<": "<<abs(V)<<" pu, "<<rad2deg(arg(V))<<" deg";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }*/
    bool converged = false;

    size_t network_iter_max = get_max_network_iteration();
    size_t network_iter_count = 0;

    while(true)
    {
        ++network_iter_count;
        if(network_iter_count<=network_iter_max)
        {
            solve_hvdcs_without_integration();
            get_bus_current_mismatch();
            if(not is_converged())
            {
                build_bus_current_mismatch_vector();
                delta_V = I_vec/jacobian;
                update_bus_voltage();
            }
            else
            {
                converged = true;
                break;
            }
        }
        else
        {
            --network_iter_count;
            break;
        }
    }
    network_iteration_count = network_iter_count;

    return converged;
}


void DYNAMICS_SIMULATOR::solve_hvdcs_without_integration()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        HVDC_MODEL* model = hvdcs[i]->get_hvdc_model();
        if(model!=NULL)
            model->solve_hvdc_model_without_integration();
    }
}

void DYNAMICS_SIMULATOR::get_bus_current_mismatch()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& net = get_network_matrix();
    get_bus_currnet_into_network();

    size_t n = I_mismatch.size();
    //#pragma omp parallel for
    for(size_t i = 0; i<n; ++i)
        I_mismatch[i] = -I_mismatch[i];
    if(check_NAN)
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after get_bus_currnet_into_network() when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(size_t j = 0; j<n; ++j)
                {
                    if(isnan(I_mismatch[j].real()) or isnan(I_mismatch[j].imag()))
                    {
                        size_t ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }

    add_generators_to_bus_current_mismatch();
    if(check_NAN)
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding generators when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(size_t j = 0; j<n; ++j)
                {
                    if(isnan(I_mismatch[j].real()) or isnan(I_mismatch[j].imag()))
                    {
                        size_t ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_wt_generators_to_bus_current_mismatch();
    if(check_NAN)
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding wt generators when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(size_t j = 0; j<n; ++j)
                {
                    if(isnan(I_mismatch[j].real()) or isnan(I_mismatch[j].imag()))
                    {
                        size_t ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_loads_to_bus_current_mismatch();
    if(check_NAN)
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding loads when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(size_t j = 0; j<n; ++j)
                {
                    if(isnan(I_mismatch[j].real()) or isnan(I_mismatch[j].imag()))
                    {
                        size_t ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[i].real()<<","<<I_mismatch[i].imag()<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_hvdcs_to_bus_current_mismatch();
    if(check_NAN)
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding hvdcs when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(size_t j = 0; j<n; ++j)
                {
                    if(isnan(I_mismatch[j].real()) or isnan(I_mismatch[j].imag()))
                    {
                        size_t ibus = net.get_physical_bus_number_of_internal_bus(j);
                        osstream<<"Physical bus: "<<ibus<<", internal bus: "<<j<<", "<<I_mismatch[j]<<endl;
                    }
                }
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
    }
    add_equivalent_devices_to_bus_current_mismatch();
    if(check_NAN)
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding equivalent devices when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
                for(size_t j = 0; j<n; ++j)
                {
                    if(isnan(I_mismatch[j].real()) or isnan(I_mismatch[j].imag()))
                    {
                        size_t ibus = net.get_physical_bus_number_of_internal_bus(j);
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
    for(size_t i=0; i<n; ++i)
    {
        size_t ibus = net.get_physical_bus_number_of_internal_bus(i);
        //osstream<<ibus<<", "<<I_mismatch[i]<<endl;
        //toolkit.show_information_with_leading_time_stamp(osstream);
    }
    */
    /*
    double maxmismatch = 0.0;
    complex<double> cmaxmismatch = 0.0;
    size_t busmax = 0;
    for(size_t i=0; i<n; ++i)
    {
        if(maxmismatch<abs(I_mismatch[i]))
        {
            maxmismatch = abs(I_mismatch[i]);
            cmaxmismatch = I_mismatch[i];
            busmax = net.get_physical_bus_number_of_internal_bus(i);
        }
    }
    osstream<<"max mismatch @ bus "<<busmax<<", "<<maxmismatch<<" or complex "<<cmaxmismatch<<", complex power = "<<psdb.get_bus_complex_voltage_in_pu(busmax)*conj(cmaxmismatch)*psdb.get_system_base_power_in_MVA()<<setprecision(10)<<", v="<<psdb.get_bus_voltage_in_pu(busmax)<<endl;
    toolkit.show_information_with_leading_time_stamp(osstream);

    for(size_t i=0; i<n; ++i)
    {
        if(abs(I_mismatch[i])>0.0001)
        {
            osstream<<"mismatch @ bus "<<net.get_physical_bus_number_of_internal_bus(i)<<", "<<I_mismatch[i]<<">0.0001"<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
        {
            osstream<<"NAN is detected in I_mismatch @ bus "<<net.get_physical_bus_number_of_internal_bus(i)<<", "<<I_mismatch[i]<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    */
}

void DYNAMICS_SIMULATOR::get_bus_currnet_into_network()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    const SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_matrix();
    size_t nbus = psdb.get_in_service_bus_count();

	if(I_mismatch.size()!=nbus)
		I_mismatch.resize(nbus, 0.0);

	for (size_t i = 0; i != nbus; ++i)
		I_mismatch[i] = 0.0;

    int nsize = Y.get_matrix_size();
    int k_start=0, k_end=0;

    for(int column=0; column!=nsize; ++column)
    {
        size_t column_physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(column);
		complex<double> voltage = psdb.get_bus_complex_voltage_in_pu(column_physical_bus);

        k_end = Y.get_starting_index_of_column(column+1);
        for(int k=k_start; k!=k_end; ++k)
        {
            int row = Y.get_row_number_of_entry_index(k);
            I_mismatch[row] += Y.get_entry_value(k)*voltage;
            complex<double> current = Y.get_entry_value(k)*voltage;
            if(isnan(current.real()) or isnan(current.imag()))
            {
                osstream<<"NAN is detected in "<<__FUNCTION__<<"() with Y["
                        <<row<<"("<<network_matrix.get_physical_bus_number_of_internal_bus(row)<<", "
                        <<column<<"("<<column_physical_bus<<"] = "<<Y.get_entry_value(k)<<", voltage["
                        <<column<<"("<<column_physical_bus<<"] = "<<voltage;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        k_start = k_end;
    }
/*
    ostringstream osstream;
    osstream<<"Current and power flowing into network (physical bus).");
    toolkit.show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     Ireal(pu)    Iimag(pu)    P(pu)   Q(pu)");
    toolkit.show_information_with_leading_time_stamp(osstream);

    complex<double> s;
    for(size_t i=0; i!=nbus; ++i)Warning. Network solution converged in
    {
        column_physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        voltage = psdb.get_bus_complex_voltage_in_pu(column_physical_bus);
        s = voltage*conj(bus_current[i]);

        osstream<<"%-8u %-10f %-10f %-10f %-10f",column_physical_bus, bus_current[i].real(), bus_current[i].imag(), s.real(), s.imag());
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
*/
}

void DYNAMICS_SIMULATOR::add_generators_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t physical_bus, internal_bus;

    vector<GENERATOR*> generators = psdb.get_all_generators();

    size_t ngen = generators.size();

    complex<double> E, V, Z, I;

    for(size_t i=0; i!=ngen; ++i)
    {
        GENERATOR* generator = generators[i];

        if(generator->get_status() == true)
        {
            physical_bus = generator->get_generator_bus();

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
            if(gen_model!=NULL)
            {
                //I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                I = gen_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();

                I_mismatch[internal_bus] += I;

                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = gen_model->get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = gen_model->get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }
}

void DYNAMICS_SIMULATOR::add_wt_generators_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t physical_bus, internal_bus;

    vector<WT_GENERATOR*> generators = psdb.get_all_wt_generators();

    size_t ngen = generators.size();

    complex<double> E, V, Z, I;

    for(size_t i=0; i!=ngen; ++i)
    {
        WT_GENERATOR* generator = generators[i];

        if(generator->get_status() == true)
        {
            physical_bus = generator->get_generator_bus();

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            WT_GENERATOR_MODEL* gen_model = generator->get_wt_generator_model();
            if(gen_model!=NULL)
            {
                if(gen_model->is_current_source())
                    I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                else
                    I = gen_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //cout<<generator->get_device_name()<<" terminal or Norton current is: "<<I<<endl;

                I_mismatch[internal_bus] += I;

                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = gen_model->get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = gen_model->get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }
}

void DYNAMICS_SIMULATOR::add_loads_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    vector<LOAD*> loads = psdb.get_all_loads();

    size_t nload = loads.size();

    //#pragma omp parallel for
    for(size_t i=0; i<nload; ++i)
    {
        LOAD* load = loads[i];
        if(load->get_status()==true)
        {
            size_t physical_bus = load->get_load_bus();

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            I_mismatch[internal_bus] -= load->get_dynamics_load_current_in_pu_based_on_system_base_power();
            /*ostringstream osstream;
            osstream<<load->get_device_name()<<" source current: "<<load->get_dynamics_load_current_in_pu_based_on_system_base_power()<<", voltage = "<<psdb.get_bus_voltage_in_pu(physical_bus);
            toolkit.show_information_with_leading_time_stamp(osstream);*/
        }
    }
}
void DYNAMICS_SIMULATOR::add_hvdcs_to_bus_current_mismatch()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t physical_bus, internal_bus;

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();

    size_t nhvdc = hvdcs.size();

    complex<double> I;

    for(size_t i=0; i!=nhvdc; ++i)
    {
        HVDC* hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
        {
            physical_bus = hvdcs[i]->get_converter_bus(RECTIFIER);

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);

            I_mismatch[internal_bus] -= I;

            if(toolkit.is_detailed_log_enabled())
            {
                I *= (psdb.get_system_base_power_in_MVA()/(sqrt(3.0)*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                osstream<<"Current at rectifier side of "<<hvdc->get_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                        <<"Complex power = "<<sqrt(3.0)*psdb.get_bus_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            physical_bus = hvdcs[i]->get_converter_bus(INVERTER);

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);

            I_mismatch[internal_bus] -= I;

            if(toolkit.is_detailed_log_enabled())
            {
                I *= (psdb.get_system_base_power_in_MVA()/(sqrt(3.0)*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                osstream<<"Current at inverter side of "<<hvdc->get_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                        <<"Complex power = "<<sqrt(3.0)*psdb.get_bus_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::add_equivalent_devices_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double sbase = psdb.get_system_base_power_in_MVA();

    size_t physical_bus, internal_bus;

    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();

    size_t nedevice = edevices.size();

    complex<double> I, V, S;

    for(size_t i=0; i!=nedevice; ++i)
    {
        if(edevices[i]->get_status() == true)
        {
            physical_bus = edevices[i]->get_equivalent_device_bus();

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            S = edevices[i]->get_total_equivalent_power_as_load_in_MVA()/sbase;

            V = psdb.get_bus_complex_voltage_in_pu(physical_bus);

            I_mismatch[internal_bus] -= conj(S/V);
        }
    }
}

bool DYNAMICS_SIMULATOR::is_converged()
{
    //clock_t start = clock();
    get_bus_power_mismatch_in_MVA();

    double smax = get_max_power_mismatch_in_MVA();

    double s_allowed = get_allowed_max_power_imbalance_in_MVA();
    //cout<<"maximum power mismatch at time "<<toolkit.get_dynamic_simulation_time_in_s()<<"s is "<<smax<<"MVA ги"<<s_allowed<<")"<<endl;

    //cout<<"    time for getting maximum power mismatch: "<<double(clock()-start)/CLOCKS_PER_SEC*1000.0<<" ms"<<endl;
    return (smax>s_allowed)? false : true;
}

void DYNAMICS_SIMULATOR:: get_bus_power_mismatch_in_MVA()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double sbase = psdb.get_system_base_power_in_MVA();

    S_mismatch = I_mismatch;

    size_t n = I_mismatch.size();
    size_t physical_bus;
    complex<double> V;
    for(size_t i= 0; i!=n; ++i)
    {
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        V = psdb.get_bus_complex_voltage_in_pu(physical_bus);
        S_mismatch[i] = V*conj(S_mismatch[i])*sbase;
    }
}

double DYNAMICS_SIMULATOR::get_max_power_mismatch_in_MVA()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double smax = 0.0;
    //complex<double> smax_complex;
    size_t smax_bus = 0;
    size_t n = S_mismatch.size();
    for(size_t i=0; i!=n; ++i)
    {
        double s = steps_fast_complex_abs(S_mismatch[i]);
        if(s>smax)
        {
            smax = s;
            smax_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
            //smax_complex = S_mismatch[i];
        }
    }
    //ostringstream osstream;
    //osstream<<"AT TIME "<<toolkit.get_dynamic_simulation_time_in_s()<<"s, max power mismatch found: "<<smax<<"MVA at bus "<<smax_bus;
    //show_information_with_leading_time_stamp(osstream);
    return smax;

}


void DYNAMICS_SIMULATOR::build_bus_current_mismatch_vector()
{
    size_t n = I_mismatch.size();
    I_vec.resize(n*2, 0.0);
    for(size_t i=0; i!=n; ++i)
    {
        I_vec[i] = I_mismatch[i].imag();
        I_vec[i+n] = I_mismatch[i].real();
    }
}


void DYNAMICS_SIMULATOR::update_bus_voltage()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t n = delta_V.size();
    n = n>>1;

    size_t physical_bus;
    BUS* bus;
    complex<double> V0, delta_v, V;
    double vang0, vmag_new, vang_new, delta_vang;

    for(size_t i=0; i!=n; ++i)
    {
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        bus = psdb.get_bus(physical_bus);
        vang0 = bus->get_angle_in_rad();
        V0 = bus->get_complex_voltage_in_pu();

        delta_v = complex<double>(delta_V[i], delta_V[i+n]);

        V = V0-delta_v*alpha;

		vmag_new = steps_fast_complex_abs(V);

		double x = V.real(), y = V.imag();
		double x0 = V0.real(), y0 = V0.imag();
		complex<double> z(x*x0 + y * y0, x0*y - y0 * x);
        delta_vang = steps_fast_complex_arg(z);
        vang_new = vang0+delta_vang;

        if(vmag_new<0.0)
            vmag_new = 0.0;

        if(vmag_new>3.0)
            vmag_new = 3.0;


        bus->set_voltage_in_pu(vmag_new);
        bus->set_angle_in_rad(vang_new);
    }
}


void DYNAMICS_SIMULATOR::build_jacobian()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    jacobian.clear();
    SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_matrix();
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.set_dynamic_simulation_time_step_in_s(newDELT);
}


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_set(size_t bus, FAULT fault)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus);
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = busptr->get_voltage_in_pu();
    double vbase = busptr->get_base_voltage_in_kV();
    double sbase = psdb.get_system_base_power_in_MVA();
    double zbase = vbase*vbase/sbase;
    double fault_x = -zbase/fault_b;
    if(fault_x<1)
        busptr->set_voltage_in_pu(0.05);
    else
    {
        if(fault_x<5)
            busptr->set_voltage_in_pu(0.1);
        else
        {
            if(fault_x<10)
                busptr->set_voltage_in_pu(0.2);
            else
            {
                if(fault_x<100)
                    busptr->set_voltage_in_pu(0.5);
                else
                    busptr->set_voltage_in_pu(current_voltage-0.1);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_cleared(size_t bus, FAULT fault)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus);
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = busptr->get_voltage_in_pu();
    busptr->set_voltage_in_pu(0.8);;
    return;

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


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_line_fault_set(const DEVICE_ID& did, size_t side_bus, double location, FAULT fault)
{
    if(location >=0.0 and location <= 1.0)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* line = psdb.get_line(did);
        if(line!=NULL and line->is_connected_to_bus(side_bus))
        {
            if(location<=0.5)
            {
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
            else
            {
                size_t ibus = line->get_sending_side_bus();
                size_t jbus = line->get_receiving_side_bus();

                location = 1.0-location;

                side_bus = (ibus==side_bus? ibus:jbus);

                guess_bus_voltage_with_line_fault_set(did, side_bus, location, fault);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_line_fault_cleared(const DEVICE_ID& did, size_t side_bus, double location, FAULT fault)
{
    if(location >= 0.0 and location <= 1.0)
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* line = psdb.get_line(did);
        if(line!=NULL and line->is_connected_to_bus(side_bus))
        {
            if(location<=0.5)
            {
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
            else
            {
                size_t ibus = line->get_sending_side_bus();
                size_t jbus = line->get_receiving_side_bus();

                location = 1.0-location;

                side_bus = (ibus==side_bus? ibus:jbus);

                guess_bus_voltage_with_line_fault_cleared(did, side_bus, location, fault);
            }
        }
    }
}

void DYNAMICS_SIMULATOR::update_generators_in_islands()
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    vector< vector<size_t> > islands = network_matrix.get_islands_with_physical_bus_number();
    if(islands.size()==generators_in_islands.size()) // won't update if islands doesn't change
        return;

    generators_in_islands.clear();

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
            generators_at_bus = psdb.get_generators_connecting_to_bus(bus);
            size_t n = generators_at_bus.size();
            for(size_t k=0; k!=n; ++k)
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
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double TIME = toolkit.get_dynamic_simulation_time_in_s();
    bool system_is_stable = true;
    size_t nislands = generators_in_islands.size();
    vector<double> angles;
    for(size_t island=0; island!=nislands; island++)
    {
        vector<GENERATOR*> generators_in_island = generators_in_islands[island];
        size_t n = generators_in_island.size();
        angles.resize(n, 0.0);
        angles.clear();

        for(size_t i=0; i!=n; ++i)
        {
            GENERATOR* generator = generators_in_island[i];
            if(generator->get_status()==true)
            {
                SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
                angles.push_back(genmodel->get_rotor_angle_in_deg());
            }
        }
        double angle_max = 0.0, angle_min = 0.0;
        size_t nangle = angles.size();
        if(nangle>0)
        {
            angle_max = angles[0];
            angle_min = angles[0];
            for(size_t i=1; i!=nangle; ++i)
            {
                if(angles[i]>angle_max)
                    angle_max = angles[i];
            }
            for(size_t i=1; i!=nangle; ++i)
            {
                if(angles[i]<angle_min)
                    angle_min = angles[i];
            }
        }
        double angle_difference = angle_max - angle_min;
        double scaled_angle_difference = rad2deg(round_angle_in_rad_to_PI(deg2rad(angle_difference)));

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<"angle difference is island is "<<scaled_angle_difference<<" deg at time "<<toolkit.get_dynamic_simulation_time_in_s();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        if(angle_difference>get_rotor_angle_stability_threshold_in_deg())
        {
            system_is_stable = false;
            osstream<<"The following island is detected to be unstable at time "<<TIME<<" s. Maximum angle difference is :"<<angle_difference<<" deg (a.k.a. "<<scaled_angle_difference<<" deg)"<<endl
                   <<"Generator          Rotor angle in deg"<<endl;

            size_t n = generators_in_island.size();
            for(size_t i=0; i!=n; ++i)
            {
                GENERATOR* generator = generators_in_island[i];
                if(generator->get_status()==true)
                {
                    SYNC_GENERATOR_MODEL* genmodel = generator->get_sync_generator_model();
                    osstream<<generator->get_device_name()<<"  "<<genmodel->get_rotor_angle_in_deg()<<endl;
                }
            }
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
    return system_is_stable;
}

void DYNAMICS_SIMULATOR::set_bus_fault(size_t bus, complex<double> fault_shunt)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        if(steps_fast_complex_abs(fault_shunt)>FLOAT_EPSILON)
        {
            FAULT fault;
            fault.set_fault_type(THREE_PHASES_FAULT);
            fault.set_fault_shunt_in_pu(fault_shunt);

            osstream<<fault.get_fault_type_string()<<" will be set for bus "<<bus<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl
                   <<"Fault shunt is"<<fault_shunt<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            busptr->set_fault(fault);
            guess_bus_voltage_with_bus_fault_set(bus, fault);

            network_matrix.build_dynamic_network_matrix();
            build_jacobian();
        }
        else
        {
            osstream<<"Zero fault shunt is given for bus "<<bus<<"."<<endl
                   <<"No fault will be set for bus "<<bus<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Bus "<<bus<<" does not exist in power system database."<<endl
               <<"No bus fault will be set at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_bus_fault(size_t bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        ostringstream osstream;

        osstream<<"Fault at bus "<<bus<<" will be cleared at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        FAULT fault = busptr->get_fault();
        busptr->clear_fault();
        guess_bus_voltage_with_bus_fault_cleared(bus, fault);

        network_matrix.build_dynamic_network_matrix();
        build_jacobian();
    }
}

void DYNAMICS_SIMULATOR::trip_bus(size_t bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        if(busptr->get_bus_type()!=OUT_OF_SERVICE)
        {
            ostringstream osstream;

            osstream<<"Bus "<<bus<<" will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s. Devices connecting to bus "<<bus<<" will also be tripped.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            psdb.trip_bus(bus);

            network_matrix.optimize_network_ordering();
            network_matrix.build_dynamic_network_matrix();
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

void DYNAMICS_SIMULATOR::set_line_fault(const DEVICE_ID& line_id, size_t side_bus, double location, complex<double> fault_shunt)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(line_id.get_device_type()=="LINE")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            if(lineptr->is_connected_to_bus(side_bus))
            {
                if(location>=0.0 and location<=1.0)
                {
                    if(steps_fast_complex_abs(fault_shunt)>FLOAT_EPSILON)
                    {
                        FAULT fault;
                        fault.set_fault_type(THREE_PHASES_FAULT);
                        fault.set_fault_shunt_in_pu(fault_shunt);

                        osstream<<fault.get_fault_type_string()<<" will be set for "<<lineptr->get_device_name()<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl
                               <<"Fault shunt is "<<fault_shunt<<" pu at location "<<location<<" to bus "<<side_bus;
                        toolkit.show_information_with_leading_time_stamp(osstream);


                        lineptr->set_fault(side_bus,location, fault);
                        guess_bus_voltage_with_line_fault_set(line_id, side_bus, location, fault);


                        //network_matrix.save_network_matrix_to_file("dynamic_network_matrix_before_event.csv");
                        //jacobian.save_matrix_to_file("dynamic_jacobian_with_fault_before_evnet.csv");
                        network_matrix.build_dynamic_network_matrix();
                        build_jacobian();
                        //network_matrix.save_network_matrix_to_file("dynamic_network_matrix_after_event.csv");
                        //jacobian.save_matrix_to_file("dynamic_jacobian_with_fault_after_event.csv");
                    }
                    else
                    {
                        osstream<<"Zero fault shunt is given for "<<lineptr->get_device_name()<<"."<<endl
                               <<"No fault will be set for "<<lineptr->get_device_name()<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_device_name()<<"."<<endl
                           <<"No fault will be set at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<lineptr->get_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be set at location "<<location<<" to bus "<<side_bus<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No fault will be set at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for setting line fault."<<endl
               <<"No line fault will be set at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::clear_line_fault(const DEVICE_ID& line_id, size_t side_bus, double location)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(line_id.get_device_type()=="LINE")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            if(lineptr->is_connected_to_bus(side_bus))
            {
                if(location>=0.0 and location<=1.0)
                {
                    osstream<<"Fault at location "<<location<<" to bus "<<side_bus<<" will be cleared for "
                           <<lineptr->get_device_name()<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    FAULT fault = lineptr->get_fault_at_location(side_bus, location);
                    lineptr->clear_fault_at_location(side_bus, location);
                    guess_bus_voltage_with_line_fault_cleared(line_id, side_bus, location, fault);

                    network_matrix.build_dynamic_network_matrix();
                    build_jacobian();
                    //const SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_matrix();
                    //Y.report_brief();
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_device_name()<<"."<<endl
                           <<"No fault will be cleared at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
            }
            else
            {
                osstream<<lineptr->get_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be cleared at location "<<location<<" to bus "<<side_bus<<" at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No fault will be cleared at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for clearing line fault."<<endl
               <<"No line fault will be cleared at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_line(const DEVICE_ID& line_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    if(line_id.get_device_type()=="LINE")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            if(lineptr->get_sending_side_breaker_status()==true or lineptr->get_receiving_side_breaker_status()==true)
            {
                osstream<<lineptr->get_device_name()<<" is tripped by taking the following actions at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

                if(lineptr->get_sending_side_breaker_status()==true)
                    trip_line_breaker(line_id, lineptr->get_sending_side_bus());

                if(lineptr->get_receiving_side_breaker_status()==true)
                    trip_line_breaker(line_id, lineptr->get_receiving_side_bus());
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No line will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for tripping line."<<endl
               <<"No line will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_line_breaker(const DEVICE_ID& line_id, size_t side_bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(line_id.get_device_type()=="LINE")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            if(lineptr->get_sending_side_bus()==side_bus)
            {
                if(lineptr->get_sending_side_breaker_status()==true)
                {
                    lineptr->set_sending_side_breaker_status(false);
                    network_matrix.build_dynamic_network_matrix();
                    build_jacobian();

                    osstream<<lineptr->get_device_name()<<" breaker at sending side (bus "<<lineptr->get_sending_side_bus()<<") is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(lineptr->get_receiving_side_bus()==side_bus)
                {
                    if(lineptr->get_receiving_side_breaker_status()==true)
                    {
                        lineptr->set_receiving_side_breaker_status(false);
                        network_matrix.build_dynamic_network_matrix();
                        build_jacobian();

                        osstream<<lineptr->get_device_name()<<" breaker at receiving side (bus "<<lineptr->get_receiving_side_bus()<<") is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No line breaker will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for tripping line breaker."<<endl
               <<"No line breaker will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_line(const DEVICE_ID& line_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    if(line_id.get_device_type()=="LINE")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            if(lineptr->get_sending_side_breaker_status()==false or lineptr->get_receiving_side_breaker_status()==false)
            {
                osstream<<lineptr->get_device_name()<<" is closed by taking the following actions at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

                if(lineptr->get_sending_side_breaker_status()==false)
                    close_line_breaker(line_id, lineptr->get_sending_side_bus());

                if(lineptr->get_receiving_side_breaker_status()==false)
                    close_line_breaker(line_id, lineptr->get_receiving_side_bus());
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No line will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for closing line."<<endl
               <<"No line will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_line_breaker(const DEVICE_ID& line_id, size_t side_bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(line_id.get_device_type()=="LINE")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            if(lineptr->get_sending_side_bus()==side_bus)
            {
                if(lineptr->get_sending_side_breaker_status()==false)
                {
                    lineptr->set_sending_side_breaker_status(true);
                    network_matrix.build_dynamic_network_matrix();
                    build_jacobian();
                    osstream<<lineptr->get_device_name()<<" breaker at sending side (bus "<<lineptr->get_sending_side_bus()<<") is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(lineptr->get_receiving_side_bus()==side_bus)
                {
                    if(lineptr->get_receiving_side_breaker_status()==false)
                    {
                        lineptr->set_receiving_side_breaker_status(true);
                        network_matrix.build_dynamic_network_matrix();
                        build_jacobian();
                        osstream<<lineptr->get_device_name()<<" breaker at receiving side (bus "<<lineptr->get_sending_side_bus()<<") is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No line breaker will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for closing line breaker"<<endl
               <<"No line breaker will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_transformer(const DEVICE_ID& trans_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    if(trans_id.get_device_type()=="TRANSFORMER")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==true or transptr->get_winding_breaker_status(SECONDARY_SIDE)==true or
               (transptr->is_three_winding_transformer() and transptr->get_winding_breaker_status(TERTIARY_SIDE)==true) )
            {
                osstream<<transptr->get_device_name()<<" is tripped by taking the following actions at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

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
                   <<"No transformer will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for tripping transformer."<<endl
               <<"No transformer will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_transformer_breaker(const DEVICE_ID& trans_id, size_t side_bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(trans_id.get_device_type()=="TRANSFORMER")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            if(transptr->get_winding_bus(PRIMARY_SIDE)==side_bus)
            {
                if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==true)
                {
                    transptr->set_winding_breaker_status(PRIMARY_SIDE, false);
                    network_matrix.build_dynamic_network_matrix();
                    build_jacobian();

                    osstream<<transptr->get_device_name()<<" breaker at primary side (bus "<<side_bus<<") is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(transptr->get_winding_bus(SECONDARY_SIDE)==side_bus)
                {
                    if(transptr->get_winding_breaker_status(SECONDARY_SIDE)==true)
                    {
                        transptr->set_winding_breaker_status(SECONDARY_SIDE, false);
                        network_matrix.build_dynamic_network_matrix();
                        build_jacobian();

                        osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    if(transptr->is_three_winding_transformer() and transptr->get_winding_bus(TERTIARY_SIDE)==side_bus)
                    {
                        if(transptr->get_winding_breaker_status(TERTIARY_SIDE)==true)
                        {
                            transptr->set_winding_breaker_status(TERTIARY_SIDE, false);
                            network_matrix.build_dynamic_network_matrix();
                            build_jacobian();

                            osstream<<transptr->get_device_name()<<" breaker at tertiary side (bus "<<side_bus<<") is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }

                }
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No transformer breaker will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for tripping transformer breaker."<<endl
               <<"No transformer breaker will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void DYNAMICS_SIMULATOR::close_transformer(const DEVICE_ID& trans_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    if(trans_id.get_device_type()=="TRANSFORMER")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==false or transptr->get_winding_breaker_status(SECONDARY_SIDE)==false or
               (transptr->is_three_winding_transformer() and transptr->get_winding_breaker_status(TERTIARY_SIDE)==false))
            {
                osstream<<transptr->get_device_name()<<" is closed by taking the following actions at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);

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
                   <<"No transformer will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for closing transformer."<<endl
               <<"No transformer will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_transformer_breaker(const DEVICE_ID& trans_id, size_t side_bus)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(trans_id.get_device_type()=="TRANSFORMER")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        TRANSFORMER* transptr = psdb.get_transformer(trans_id);
        if(transptr!=NULL)
        {
            if(transptr->get_winding_bus(PRIMARY_SIDE)==side_bus)
            {
                if(transptr->get_winding_breaker_status(PRIMARY_SIDE)==false)
                {
                    transptr->set_winding_breaker_status(PRIMARY_SIDE, true);
                    network_matrix.build_dynamic_network_matrix();
                    build_jacobian();
                    osstream<<transptr->get_device_name()<<" breaker at primary side (bus "<<side_bus<<") is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                if(transptr->get_winding_bus(SECONDARY_SIDE)==side_bus)
                {
                    if(transptr->get_winding_breaker_status(SECONDARY_SIDE)==false)
                    {
                        transptr->set_winding_breaker_status(SECONDARY_SIDE, true);
                        network_matrix.build_dynamic_network_matrix();
                        build_jacobian();
                        osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    if(transptr->is_three_winding_transformer() and transptr->get_winding_bus(TERTIARY_SIDE)==side_bus)
                    {
                        if(transptr->get_winding_breaker_status(TERTIARY_SIDE)==false)
                        {
                            transptr->set_winding_breaker_status(TERTIARY_SIDE, true);
                            network_matrix.build_dynamic_network_matrix();
                            build_jacobian();
                            osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No transformer breaker will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a TRANSFORMER (it is a "<<trans_id.get_device_type()<<") for closing transformer breaker"<<endl
               <<"No transformer breaker will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_generator(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(gen_id.get_device_type()=="GENERATOR")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        GENERATOR* generator = psdb.get_generator(gen_id);
        if(generator!=NULL)
        {
            if(generator->get_status()==true)
            {
                generator->set_status(false);

                network_matrix.build_dynamic_network_matrix();
                build_jacobian();

                osstream<<generator->get_device_name()<<" is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No generator will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for tripping generator."<<endl
               <<"No generator will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::shed_generator(const DEVICE_ID& gen_id,double percent)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(gen_id.get_device_type()=="GENERATOR")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        GENERATOR* generator = psdb.get_generator(gen_id);
        if(generator!=NULL)
        {
            if(fabs(percent)>FLOAT_EPSILON)
            {
                double mbase = generator->get_mbase_in_MVA();
                osstream<<generator->get_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl
                  <<"MBASE is changed from "<<mbase<<" MVA to ";

                generator->set_mbase_in_MVA(mbase*(1.0-percent));
                osstream<<generator->get_mbase_in_MVA()<<" MVA.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                network_matrix.build_dynamic_network_matrix();
                build_jacobian();
            }
            else
            {
                osstream<<"Generator relay percent is 0.0 for "<<generator->get_device_name()<<endl
                       <<"No generator will be shed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No generator will be shed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for relay generator."<<endl
               <<"No generator will be shed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::trip_wt_generator(const DEVICE_ID& gen_id, size_t n)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(gen_id.get_device_type()=="WT GENERATOR")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        WT_GENERATOR* generator = psdb.get_wt_generator(gen_id);
        if(generator!=NULL)
        {
            if(generator->get_status()==true)
            {
                if(n!=0)
                {
                    size_t N = generator->get_number_of_lumped_wt_generators();
                    if(n<N)
                    {
                        generator->set_number_of_lumped_wt_generators(N-n);
                        double mbase = generator->get_mbase_in_MVA();
                        generator->set_mbase_in_MVA(mbase/N*(N-n));

                        osstream<<n<<" individual generators of "<<generator->get_device_name()<<" are tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    else
                    {
                        generator->set_status(false);

                        osstream<<"All individual generators of "<<generator->get_device_name()<<" are tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }

                    network_matrix.build_dynamic_network_matrix();
                    build_jacobian();
                }
                else
                {
                    osstream<<n<<" = 0, and no individual generators of "<<generator->get_device_name()<<" will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No wt generator will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a WT GENERATOR (it is a "<<gen_id.get_device_type()<<") for tripping wt generator."<<endl
               <<"No wt generator will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::trip_load(const DEVICE_ID& load_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(load_id.get_device_type()=="LOAD")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LOAD* load = psdb.get_load(load_id);
        if(load!=NULL)
        {
            if(load->get_status()==true)
            {
                load->set_status(false);

                network_matrix.build_dynamic_network_matrix();
                build_jacobian();

                osstream<<load->get_device_name()<<" is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No load will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for tripping load."<<endl
               <<"No load will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_load(const DEVICE_ID& load_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(load_id.get_device_type()=="LOAD")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LOAD* load = psdb.get_load(load_id);
        if(load!=NULL)
        {
            if(load->get_status()==false)
            {
                load->set_status(true);

                network_matrix.build_dynamic_network_matrix();
                build_jacobian();

                osstream<<load->get_device_name()<<" is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No load will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for closing load."<<endl
               <<"No load will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::scale_load(const DEVICE_ID& load_id, double percent)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    if(load_id.get_device_type()=="LOAD")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        LOAD* load = psdb.get_load(load_id);
        if(load!=NULL)
        {
            if(fabs(percent)>FLOAT_EPSILON)
            {
                if(load->get_status()==true)
                {
                    osstream<<load->get_device_name()<<" is manually scaled "<<(percent>0.0?"up":"down")<<" by "<<percent*100.0<<"% at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                    double scale = load->get_load_manually_scale_factor_in_pu();
                    load->set_load_manually_scale_factor_in_pu(scale+percent);
                    osstream<<"Load manual scale is changed from "<<scale<<" to "<<scale+percent<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                else
                {
                    osstream<<load->get_device_name()<<" is out of service. No load scale action can be applied at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"Load scaling percent is 0.0 for "<<load->get_device_name()<<endl
                       <<"No load will be scaled at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No load will be scaled at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LOAD (it is a "<<load_id.get_device_type()<<") for scaling load."<<endl
               <<"No single load will be scaled at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::scale_all_load(double percent)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(fabs(percent)>FLOAT_EPSILON)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        /*vector<DEVICE_ID> loads = psdb.get_all_loads_device_id();
        DEVICE_ID load;
        size_t n = loads.size();
        for(size_t i=0; i!=n; ++i)
        {
            load = loads[i];
            LOAD* loadptr = psdb.get_load(load);
            if(loadptr==NULL)
                continue;

            if(loadptr->get_status()==false)
                continue;

            scale_load(load, percent);
        }*/
        vector<LOAD*> loads = psdb.get_all_loads();
        size_t n = loads.size();
        for(size_t i=0; i!=n; ++i)
        {
            if(loads[i]->get_status()==true)
                scale_load(loads[i]->get_device_id(), percent);
        }

    }
    else
    {
        osstream<<"Load scaling percent is 0.0."<<endl
               <<"No all load will be scaled at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::trip_fixed_shunt(const DEVICE_ID& shunt_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(shunt_id.get_device_type()=="FIXED SHUNT")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        FIXED_SHUNT* shunt = psdb.get_fixed_shunt(shunt_id);
        if(shunt!=NULL)
        {
            if(shunt->get_status()==true)
            {
                shunt->set_status(false);

                network_matrix.build_dynamic_network_matrix();
                build_jacobian();

                osstream<<shunt->get_device_name()<<" is tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<shunt_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No fixed shunt will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a FIXED SHUNT (it is a "<<shunt_id.get_device_type()<<") for tripping fixed shunt."<<endl
               <<"No fixed shunt will be tripped at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::close_fixed_shunt(const DEVICE_ID& shunt_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    ostringstream osstream;

    if(shunt_id.get_device_type()=="FIXED SHUNT")
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        FIXED_SHUNT* shunt = psdb.get_fixed_shunt(shunt_id);
        if(shunt!=NULL)
        {
            if(shunt->get_status()==false)
            {
                shunt->set_status(true);

                network_matrix.build_dynamic_network_matrix();
                build_jacobian();

                osstream<<shunt->get_device_name()<<" is closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<shunt_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No fixed shunt will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a FIXED SHUNT (it is a "<<shunt_id.get_device_type()<<") for closing fixed shunt."<<endl
               <<"No fixed shunt will be closed at time "<<toolkit.get_dynamic_simulation_time_in_s()<<" s."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::manual_bypass_hvdc(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVDC* hvdc = psdb.get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL and (not model->is_blocked() and not model->is_bypassed()) )
            model->manual_bypass_hvdc();
    }
}

void DYNAMICS_SIMULATOR::manual_unbypass_hvdc(const DEVICE_ID& hvdc_id)
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVDC* hvdc = psdb.get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL and (not model->is_blocked() and model->is_manual_bypassed()))
            model->manual_unbypass_hvdc();
    }
}

void DYNAMICS_SIMULATOR::manual_block_hvdc(const DEVICE_ID& hvdc_id)
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVDC* hvdc = psdb.get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL and (not model->is_blocked()))
            model->manual_block_hvdc();
    }
}

void DYNAMICS_SIMULATOR::manual_unblock_hvdc(const DEVICE_ID& hvdc_id)
{
    ostringstream osstream;

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVDC* hvdc = psdb.get_hvdc(hvdc_id);
    if(hvdc!=NULL)
    {
        HVDC_MODEL* model = hvdc->get_hvdc_model();
        if(model!=NULL and model->is_manual_blocked())
            model->manual_unblock_hvdc();
    }
}


void DYNAMICS_SIMULATOR::change_generator_voltage_reference_in_pu(const DEVICE_ID& gen_id, double vref)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter!=NULL)
            exciter->set_voltage_reference_in_pu(vref);
    }
}

void DYNAMICS_SIMULATOR::change_generator_power_reference_in_MW(const DEVICE_ID& gen_id, double Pref)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
            tg->set_initial_mechanical_power_reference_in_pu_based_on_mbase(Pref);
    }
}

void DYNAMICS_SIMULATOR::switch_on_equivalent_device()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
    size_t n = edevices.size();
    ostringstream osstream;
    osstream<<"There are "<<n<<" equivalent devices to switch on";
    toolkit.show_information_with_leading_time_stamp(osstream);
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
