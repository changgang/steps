#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/meter/meter_setter.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <ctime>

using namespace std;

#define ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR

DYNAMICS_SIMULATOR::DYNAMICS_SIMULATOR()
{
    DELT = 0.01;
    clear();
    detailed_log_enabled = false;
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

    set_bin_file_export_enable_flag(false);
    set_csv_file_export_enable_flag(true);
    set_json_file_export_enable_flag(false);

    set_dynamic_simulation_time_in_s(0.0);

    set_max_DAE_iteration(100);
    set_min_DAE_iteration(3);
    set_max_network_iteration(20);
    set_max_update_iteration(100);
    set_allowed_max_power_imbalance_in_MVA(0.001);
    set_iteration_accelerator(1.0);
    set_non_divergent_solution_logic(false);
    set_rotor_angle_stability_surveillance_flag(false);
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

double DYNAMICS_SIMULATOR::get_dynamic_simulation_time_step_in_s() const
{
    return DELT;
}

void DYNAMICS_SIMULATOR::set_dynamic_simulation_time_in_s(double time)
{
    TIME = time;
}

double DYNAMICS_SIMULATOR::get_dynamic_simulation_time_in_s() const
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

void DYNAMICS_SIMULATOR::set_min_DAE_iteration(size_t iteration)
{
    if(iteration>0)
        this->min_DAE_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_max_network_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_network_iteration = iteration;
}

void DYNAMICS_SIMULATOR::set_max_update_iteration(size_t iteration)
{
    if(iteration>0)
        this->max_update_iteration = iteration;
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

void DYNAMICS_SIMULATOR::set_non_divergent_solution_logic(bool logic)
{
    non_divergent_solution_enabled = logic;
}

void DYNAMICS_SIMULATOR::set_rotor_angle_stability_surveillance_flag(bool flag)
{
    this->flag_rotor_angle_stability_surveillance = flag;
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

size_t DYNAMICS_SIMULATOR::get_min_DAE_iteration() const
{
    return min_DAE_iteration;
}

size_t DYNAMICS_SIMULATOR::get_max_network_iteration() const
{
    return max_network_iteration;
}

size_t DYNAMICS_SIMULATOR::get_max_update_iteration() const
{
    return max_update_iteration;
}

double DYNAMICS_SIMULATOR::get_allowed_max_power_imbalance_in_MVA() const
{
    return P_threshold_in_MW;
}

double DYNAMICS_SIMULATOR::get_iteration_accelerator() const
{
    return alpha;
}

bool DYNAMICS_SIMULATOR::get_non_divergent_solution_logic() const
{
    return non_divergent_solution_enabled;
}

bool DYNAMICS_SIMULATOR::get_rotor_angle_stability_surveillance_flag() const
{
    return this->flag_rotor_angle_stability_surveillance;
}

double DYNAMICS_SIMULATOR::get_rotor_angle_stability_threshold_in_deg() const
{
    return this->rotor_angle_stability_threshold_in_deg;
}

void DYNAMICS_SIMULATOR::show_dynamic_simulator_configuration() const
{
    ostringstream osstream;
    osstream<<"Configuration of dynamic simulator:\n"
            <<"Time step: "<<get_dynamic_simulation_time_step_in_s()<<" s\n"
            <<"Allowed maximum power imbalance: "<<get_allowed_max_power_imbalance_in_MVA()<<" MVA\n"
            <<"Maximum iteration for DAE solution: "<<get_max_DAE_iteration()<<"\n"
            <<"Minimum iteration for DAE solution: "<<get_min_DAE_iteration()<<"\n"
            <<"Maximum iteration for network: "<<get_max_network_iteration()<<"\n"
            <<"Maximum iteration for updating: "<<get_max_update_iteration()<<"\n"
            <<"Network solution accelerator: "<<get_iteration_accelerator()<<"\n"
            <<"Rotor angle stability surveillance: "<<(get_rotor_angle_stability_surveillance_flag()?"Enabled":"Disabled")<<"\n"
            <<"Rotor angle stability threshold: "<<get_rotor_angle_stability_threshold_in_deg()<<" deg\n"
            <<"CSV export: "<<(is_csv_file_export_enabled()?"Enabled":"Disabled")<<"\n"
            <<"BIN export: "<<(is_bin_file_export_enabled()?"Enabled":"Disabled")<<"\n"
            <<"JSON export: "<<(is_json_file_export_enabled()?"Enabled":"Disabled")<<"\n"
            <<"Output file name: "<<get_output_file()<<"\n";

    size_t n = meters.size();
    osstream<<"Channel count: "<<n<<"\n";
    for(size_t i=0; i!=n; ++i)
        osstream<<"("<<setw(4)<<i+1<<") "<<meters[i].get_meter_name()<<"\n";

    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
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
        METER meter = setter.prepare_generator_stabilizing_signal_in_pu_meter(generator->get_device_id());
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_pv_unit_count();
    vector<PV_UNIT*> pv_units = psdb.get_all_pv_units();
    PV_UNIT* pv_unit;
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_current_in_kA_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_active_power_in_MW_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_reactive_power_in_MVar_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_active_current_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_current_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_active_power_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_power_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_voltage_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(size_t i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_solar_irradiance_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
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
        meter = setter.prepare_line_current_in_kA_meter(line->get_device_id(),line->get_sending_side_bus());
        append_meter(meter);
        meter = setter.prepare_line_active_power_in_MW_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
        meter = setter.prepare_line_reactive_power_in_MVar_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
        meter = setter.prepare_line_current_in_kA_meter(line->get_device_id(),line->get_receiving_side_bus());
        append_meter(meter);
    }
}


void DYNAMICS_SIMULATOR::prepare_transformer_related_meters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    METER_SETTER setter;
    setter.set_toolkit(toolkit);

    size_t n;

    n = psdb.get_transformer_count();
    vector<TRANSFORMER*> transes = psdb.get_all_transformers();
    TRANSFORMER* trans;
    for(size_t i=0; i!=n; ++i)
    {
        trans = transes[i];
        METER meter = setter.prepare_transformer_active_power_in_MW_meter(trans->get_device_id(), trans->get_winding_bus(PRIMARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_reactive_power_in_MVar_meter(trans->get_device_id(), trans->get_winding_bus(PRIMARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_current_in_kA_meter(trans->get_device_id(), trans->get_winding_bus(PRIMARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_active_power_in_MW_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_reactive_power_in_MVar_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
        append_meter(meter);
        meter = setter.prepare_transformer_current_in_kA_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
        append_meter(meter);
        if(trans->is_three_winding_transformer())
        {
            meter = setter.prepare_transformer_active_power_in_MW_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
            append_meter(meter);
            meter = setter.prepare_transformer_reactive_power_in_MVar_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
            append_meter(meter);
            meter = setter.prepare_transformer_current_in_kA_meter(trans->get_device_id(), trans->get_winding_bus(SECONDARY_SIDE));
            append_meter(meter);
        }
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
            if(meter_type=="STABILIZING SIGNAL IN PU")
                meter = setter.prepare_generator_stabilizing_signal_in_pu_meter(did);
            if(meter_type=="TERMINAL VOLTAGE IN PU")
                meter = setter.prepare_generator_terminal_voltage_in_pu_meter(did);
            if(meter_type=="INTERNAL VOLTAGE IN PU")
                meter = setter.prepare_generator_internal_voltage_in_pu_meter(did);
            if(meter_type=="MECHANICAL POWER IN MW")
                meter = setter.prepare_generator_mechanical_power_in_MW_meter(did);
            if(meter_type=="MECHANICAL POWER IN PU ON MBASE")
                meter = setter.prepare_generator_mechanical_power_in_pu_on_mbase_meter(did);
            if(meter_type=="MECHANICAL POWER IN PU ON SBASE")
                meter = setter.prepare_generator_mechanical_power_in_pu_on_sbase_meter(did);
            if(meter_type=="MECHANICAL POWER REFERENCE IN MW")
                meter = setter.prepare_generator_mechanical_power_reference_in_MW_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_generator_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_generator_terminal_active_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_generator_terminal_active_power_in_pu_on_sbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON MBASE")
                meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_mbase_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN PU ON SBASE")
                meter = setter.prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(did);
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
            if(meter_type=="TURBINE SPEED DEVIATION IN PU")
                meter = setter.prepare_wt_generator_turbine_speed_deviation_in_pu_meter(did);
            if(meter_type=="ROTOR SPEED DEVIATION IN PU")
                meter = setter.prepare_wt_generator_rotor_speed_deviation_in_pu_meter(did);
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
                meter = setter.prepare_pv_unit_terminal_current_in_kA_meter(did);
            if(meter_type=="TERMINAL ACTIVE POWER IN MW")
                meter = setter.prepare_pv_unit_terminal_active_power_in_MW_meter(did);
            if(meter_type=="TERMINAL REACTIVE POWER IN MVAR")
                meter = setter.prepare_pv_unit_terminal_reactive_power_in_MVar_meter(did);
            if(meter_type=="ACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_pv_unit_active_current_command_in_pu_meter(did);
            if(meter_type=="REACTIVE CURRENT COMMAND IN PU")
                meter = setter.prepare_pv_unit_reactive_current_command_in_pu_meter(did);
            if(meter_type=="ACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_pv_unit_active_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE POWER COMMAND IN PU")
                meter = setter.prepare_pv_unit_reactive_power_command_in_pu_meter(did);
            if(meter_type=="REACTIVE VOLTAGE COMMAND IN PU")
                meter = setter.prepare_pv_unit_reactive_voltage_command_in_pu_meter(did);
            if(meter_type=="SOLAR IRRADIANCE IN PU")
                meter = setter.prepare_pv_unit_solar_irradiance_in_pu_meter(did);
            if(meter_type=="PV CONVERTER MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_converter_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV PANEL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_panel_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV ELECTRICAL MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_electrical_model_internal_variable_meter(did, var_name);
            if(meter_type=="PV IRRADIANCE MODEL INTERNAL VARIABLE")
                meter = setter.prepare_pv_irradiance_model_internal_variable_meter(did, var_name);

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
            if(bus==0)
                return;

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

void DYNAMICS_SIMULATOR::prepare_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    if(did.get_device_type()=="TRANSFORMER")
    {
        if(psdb.is_line_exist(did))
        {
            METER_SETTER setter;
            setter.set_toolkit(toolkit);

            METER meter;
            meter.set_toolkit(toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            TRANSFORMER* trans = psdb.get_transformer(did);
            size_t bus = 0;
            if(side=="PRIMARY" or side=="P")
                bus = trans->get_winding_bus(PRIMARY_SIDE);
            if(side=="SECONDARY" or side=="S")
                bus = trans->get_winding_bus(SECONDARY_SIDE);
            if(side=="TERTIARY" or side=="T")
                bus = trans->get_winding_bus(TERTIARY_SIDE);
            if(bus==0)
                return;

            if(meter_type=="ACTIVE POWER IN MW")
                meter = setter.prepare_transformer_active_power_in_MW_meter(did, bus);
            if(meter_type=="REACTIVE POWER IN MVAR")
                meter = setter.prepare_transformer_reactive_power_in_MVar_meter(did, bus);
            if(meter_type=="CURRENT IN KA")
                meter = setter.prepare_transformer_current_in_kA_meter(did, bus);

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
        osstream<<"Warning. The device type of "<<did.get_device_name()<<" is not TRANSFORMER when setting up meter with "<<__FUNCTION__;
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = meters.size();
    if(n!=0)
    {
        if(meter_values.size()!=n)
            meter_values.resize(n,0.0);

        #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
            set_openmp_number_of_threads(toolkit.get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        for(size_t i=0; i<n; ++i)
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

    size_t n = meters.size();
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

                csv_output_file<<"TIME,DAE INTEGRATION,NETWORK ITERATION,MISMATCH IN MVA,MISMATCH BUS,TIME ELAPSE IN MS";

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

                json_output_file<<"    \"meter_name\" : [\"TIME\", \"DAE INTEGRATION\", \"NETWORK ITERATION\", \"MISMATCH IN MVA\", \"MISMATCH BUS\", \"TIME ELAPSE IN MS\"";
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
                meter_names += "MISMATCH BUS\n";
                meter_names += "TIME ELAPSE IN MS\n";
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
    size_t n = meters.size();
    if(n!=0 and (csv_output_file.is_open() or json_output_file.is_open() or bin_output_file.is_open()))
    {
        get_bus_current_mismatch();
        calculate_bus_power_mismatch_in_MVA();
        POWER_MISMATCH_STRUCT s_mismatch = get_max_power_mismatch_struct();
        double smax = s_mismatch.greatest_mismatch_in_MVA;
        size_t smax_bus = s_mismatch.bus_with_greatest_mismatch;

        update_all_meters_value();

        if(is_bin_file_export_enabled() and bin_output_file.is_open())
        {
            float fvalue=0.0;
            int ivalue=0;

            fvalue = TIME;
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            fvalue = float(ITER_DAE);
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            fvalue = float(ITER_NET);
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            fvalue = smax;
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            ivalue = smax_bus;
            bin_output_file.write((char *)(&ivalue), sizeof(int));

            fvalue = time_elapse_in_a_step;
            bin_output_file.write((char *)(&fvalue), sizeof(fvalue));

            for (size_t i = 0; i != n; ++i)
            {
                fvalue = meter_values[i];
                bin_output_file.write((char *)(&fvalue), sizeof(fvalue));
            }
        }

        if(is_csv_file_export_enabled() or is_json_file_export_enabled())
        {
            char temp_buffer[50];
            string temp_str = "";
            snprintf(temp_buffer, 50, "%8.4f",TIME);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%3lu",ITER_DAE);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%3lu",ITER_NET);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",smax);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6d",smax_bus);
            temp_str += temp_buffer;
            snprintf(temp_buffer, 50, ",%6.3f",time_elapse_in_a_step);
            temp_str += temp_buffer;
            for (size_t i = 0; i != n; ++i)
            {
                snprintf(temp_buffer, 50, ",%16.12f", meter_values[i]);
                temp_str += temp_buffer;
            }

            if(is_csv_file_export_enabled() and csv_output_file.is_open())
                csv_output_file<<temp_str<<endl;

            if(is_json_file_export_enabled() and json_output_file.is_open())
                json_output_file<<","<<endl
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    size_t nbus = psdb.get_in_service_bus_count();

    NETWORK_MATRIX& network_matrix = get_network_matrix();
    internal_bus_pointers.clear();
    for(size_t internal_bus=0; internal_bus!=nbus; ++internal_bus)
    {
        size_t physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        internal_bus_pointers.push_back(psdb.get_bus(physical_bus));
    }

    internal_bus_complex_voltage_in_pu.resize(nbus, 0.0);
    initialize_internal_bus_voltage_vector();
}

void DYNAMICS_SIMULATOR::initialize_internal_bus_voltage_vector()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t nbus = internal_bus_complex_voltage_in_pu.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t internal_bus=0; internal_bus<nbus; ++internal_bus)
        internal_bus_complex_voltage_in_pu[internal_bus] = internal_bus_pointers[internal_bus]->get_positive_sequence_complex_voltage_in_pu();
}

complex<double> DYNAMICS_SIMULATOR::get_bus_complex_voltage_in_pu_with_internal_bus_number(size_t internal_bus) const
{
    return internal_bus_complex_voltage_in_pu[internal_bus];

    BUS* busptr = internal_bus_pointers[internal_bus];
    complex<double> v= busptr->get_positive_sequence_complex_voltage_in_pu();
    if(v!=internal_bus_complex_voltage_in_pu[internal_bus])
    {
        ostringstream osstream;
        osstream<<"voltage inconsistent of internal bus "<<internal_bus<<", v = "<<v<<", internal array = "<<internal_bus_complex_voltage_in_pu[internal_bus];
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    return v;
}

void DYNAMICS_SIMULATOR::start()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWERFLOW_SOLVER& pf_solver = toolkit.get_powerflow_solver();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    detailed_log_enabled = toolkit.is_detailed_log_enabled();

    show_dynamic_simulator_configuration();

    time_t clock_start = clock();

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

    optimize_network_ordering();

    prepare_devices_for_run();

    run_all_models(INITIALIZE_MODE);

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
    size_t iter_count  = 0;
    current_max_network_iteration = get_max_network_iteration();
    while(true)
    {
		++iter_count;
		//osstream<<"Network iteration "<<iter_count;
		//toolkit.show_information_with_leading_time_stamp(osstream);
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


    time_elapse_in_a_step = (1000.0/double(CLOCKS_PER_SEC))*double(clock()-clock_start);

    save_meter_information();
    save_meter_values();
}

void DYNAMICS_SIMULATOR::prepare_devices_for_run()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    in_service_buses = psdb.get_all_in_service_buses();
    generators = psdb.get_all_generators();
    wt_generators = psdb.get_all_wt_generators();
    pv_units = psdb.get_all_pv_units();
    e_storages = psdb.get_all_energy_storages();

    loads = psdb.get_all_loads();

    lines = psdb.get_all_lines();
    hvdcs = psdb.get_all_hvdcs();

    e_devices = psdb.get_all_equivalent_devices();
}

void DYNAMICS_SIMULATOR::stop()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    //if(TIME >0.0)
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
    size_t n = generators.size();
    GENERATOR* gen;
    SYNC_GENERATOR_MODEL* model;
    double max_angle = -1e10, min_angle = 1e10;
    double angle;
    for(size_t i=0; i!=n; ++i)
    {
        gen = generators[i];
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

    NETWORK_MATRIX& network_matrix = toolkit.get_network_matrix();

    network_matrix.build_dynamic_network_Y_matrix();
    build_jacobian();

    update_with_event();
    if(get_rotor_angle_stability_surveillance_flag()==false)
    {
        while(TIME<=time-FLOAT_EPSILON)
            run_a_step();
    }
    else
    {
        while(TIME<=time-FLOAT_EPSILON)
        {
            run_a_step();

            update_generators_in_islands();
            bool stable = is_system_angular_stable();
            if(stable==true)
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
    time_t clock_start = clock();
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    TIME += DELT;
    if(not detailed_log_enabled)
        ;
    else
    {
        osstream<<"Run dynamic simulation at time "<<TIME<<"s.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    update_equivalent_devices_buffer();
    update_equivalent_devices_output();

    //bool network_converged = false;
    //bool DAE_converged = false;
    ITER_DAE = 0;
    ITER_NET = 0;
    current_max_network_iteration = get_max_network_iteration();
    while(true)
    {
        ++ITER_DAE;
        if(not detailed_log_enabled)
            ;
        else
        {
            osstream<<"Solve DAE with iteration "<<ITER_DAE<<" at time "<<TIME<<"s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        if(ITER_DAE<=max_DAE_iteration)
        {
            integrate();
            bool network_converged = solve_network();
            ITER_NET += network_iteration_count;

            if(not detailed_log_enabled)
                ;
            else
            {
                if(not network_converged)
                {
                    osstream<<"Failed to solve network in "<<max_network_iteration<<" iterations during DAE iteration "<<ITER_DAE<<" when integrating at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            if(ITER_DAE>=get_min_DAE_iteration() and network_converged) // DAE solution converged
                break;
        }
        else
        {
            if(max_DAE_iteration!=2)
            {
                char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve DAE in %lu iterations when integrating at time %f s.",
                         max_DAE_iteration, TIME);
                toolkit.show_information_with_leading_time_stamp(buffer);
            }
            --ITER_DAE;
            break;
        }
    }
    update();
    update_relay_models();

    time_elapse_in_a_step = (1000.0/double(CLOCKS_PER_SEC))*double(clock()-clock_start);

    save_meter_values();
}

void DYNAMICS_SIMULATOR::update_with_event()
{
    time_t clock_start = clock();
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    bool network_converged = false;
    size_t update_event_iter_max = get_max_update_iteration();

    ITER_DAE = 0;
    ITER_NET = 0;

    size_t iter = 0;
    current_max_network_iteration = get_max_update_iteration();
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
                         update_event_iter_max, TIME);
                toolkit.show_information_with_leading_time_stamp(buffer);
                break;
            }
        }
        else
            break;
    }
    update_bus_frequency_blocks();
    update();

    time_elapse_in_a_step = (1000.0/double(CLOCKS_PER_SEC))*double(clock()-clock_start);

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

    bool network_converged = false;

    current_max_network_iteration = get_max_update_iteration();
    network_converged = solve_network();
    ITER_NET += network_iteration_count;

    if(not network_converged and get_max_update_iteration()>1)
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Failed to solve network in %lu iterations when updating at time %f s.",
                 current_max_network_iteration, TIME);
        toolkit.show_information_with_leading_time_stamp(buffer);
    }
    //update_bus_frequency_blocks();

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

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_generator_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; i++)
    {
        GENERATOR* generator = generators[i];
        if(generator->get_status()==true)
            generator->run(mode);
    }

    n = psdb.get_wt_generator_count();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_wt_generator_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        WT_GENERATOR* wtgen = wt_generators[i];
        if(wtgen->get_status()==true)
            wtgen->run(mode);
    }

    n = psdb.get_pv_unit_count();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_pv_unit_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        PV_UNIT* pv = pv_units[i];
        if(pv->get_status()==true)
            pv->run(mode);
    }

    n = psdb.get_load_count();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_load_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        LOAD* load = loads[i];
        if(load->get_status()==true)
            load->run(mode);
    }

    n = psdb.get_hvdc_count();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_hvdc_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        HVDC* hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
            hvdc->run(mode);
    }

    n = psdb.get_equivalent_device_count();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_equivalent_device_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        EQUIVALENT_DEVICE* edevice = e_devices[i];
        if(edevice->get_status()==true)
            edevice->run(mode);
    }

    n = in_service_buses.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        BUS* bus = in_service_buses[i];
        BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
        model->run(mode);
    }
}

void DYNAMICS_SIMULATOR::update_bus_frequency_blocks()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = in_service_buses.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        BUS* bus = in_service_buses[i];
        BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
        model->update_for_applying_event();
        /*if(bus->get_bus_type()!=OUT_OF_SERVICE)
        {
            BUS_FREQUENCY_MODEL* model = bus->get_bus_frequency_model();
            model->update_for_applying_event();
        }*/
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
    size_t n = e_devices.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_equivalent_device_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        EQUIVALENT_DEVICE* edevice = e_devices[i];
        edevice->update_meter_buffer();
    }
}

void DYNAMICS_SIMULATOR::update_equivalent_devices_output()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = e_devices.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_equivalent_device_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        ostringstream osstream;
        EQUIVALENT_DEVICE* edevice = e_devices[i];
        edevice->update_equivalent_outputs();

        osstream<<"At time "<<TIME<<" s, equivalent load of "<<edevice->get_device_name()<<" is: "<<edevice->get_total_equivalent_power_as_load_in_MVA()<<"MVA";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

bool DYNAMICS_SIMULATOR::solve_network()
{
    ostringstream osstream;

    bool converged = false;

    network_iteration_count = 0;

    size_t network_iter_max = current_max_network_iteration;
    size_t network_iter_count = 0;

    solve_hvdcs_without_integration();
    get_bus_current_mismatch();
    calculate_bus_power_mismatch_in_MVA();
    double smax = get_max_power_mismatch_struct().greatest_mismatch_in_MVA;
    if(smax<get_allowed_max_power_imbalance_in_MVA())
    {
        return true;
    }
    else
    {
        while(true)
        {
            if(network_iter_count<network_iter_max)
            {
                build_bus_current_mismatch_vector();
                delta_V = I_vec/jacobian;
                update_bus_voltage();
                if(get_non_divergent_solution_logic()==true)
                {
                    size_t n_delta_V = delta_V.size();
                    for(size_t i=0; i<n_delta_V; ++i)
                        delta_V[i] = -delta_V[i];

                    double original_alpha = get_iteration_accelerator();
                    for(size_t i=0; i<5; ++i)
                    {
                        //++network_iter_count;
                        solve_hvdcs_without_integration();
                        get_bus_current_mismatch();
                        calculate_bus_power_mismatch_in_MVA();
                        double new_smax = get_max_power_mismatch_struct().greatest_mismatch_in_MVA;
                        if(new_smax>=smax)
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

                solve_hvdcs_without_integration();
                get_bus_current_mismatch();
                calculate_bus_power_mismatch_in_MVA();
                double new_smax = get_max_power_mismatch_struct().greatest_mismatch_in_MVA;
                if(new_smax>get_allowed_max_power_imbalance_in_MVA())
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

    network_iteration_count = network_iter_count;

    return converged;
}


void DYNAMICS_SIMULATOR::solve_hvdcs_without_integration()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_hvdc_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
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
    NETWORK_MATRIX& net = get_network_matrix();

    get_bus_currnet_into_network();

    size_t n = I_mismatch.size();
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i = 0; i<n; ++i)
        I_mismatch[i] = -I_mismatch[i];

    if(not detailed_log_enabled)
        ;
    else
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
    if(not detailed_log_enabled)
        ;
    else
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
    if(not detailed_log_enabled)
        ;
    else
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
    add_pv_units_to_bus_current_mismatch();
    if(not detailed_log_enabled)
        ;
    else
    {
        for(size_t i = 0; i<n; ++i)
        {
            if(isnan(I_mismatch[i].real()) or isnan(I_mismatch[i].imag()))
            {
                osstream<<"warning. NAN is detected when getting bus current mismatch after adding pv units when calling DYNAMICS_SIMULATOR::"<<__FUNCTION__<<"():"<<endl;
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
    if(not detailed_log_enabled)
        ;
    else
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
    if(not detailed_log_enabled)
        ;
    else
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
    if(not detailed_log_enabled)
        ;
    else
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
    osstream<<"max mismatch @ bus "<<busmax<<", "<<maxmismatch<<" or complex "<<cmaxmismatch<<", complex power = "<<psdb.get_bus_complex_voltage_in_pu(busmax)*conj(cmaxmismatch)*psdb.get_system_base_power_in_MVA()<<setprecision(10)<<", v="<<psdb.get_bus_positive_sequence_voltage_in_pu(busmax)<<endl;
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
    const SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_Y_matrix();
    size_t nbus = psdb.get_in_service_bus_count();

	if(I_mismatch.size()!=nbus)
		I_mismatch.resize(nbus, 0.0);

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
	for (size_t i = 0; i<nbus; ++i)
		I_mismatch[i] = 0.0;

    int nsize = Y.get_matrix_size();

    // tho following codes can not be parallelized due to code I_mismatch[row] += Y.get_entry_value(k)*voltage;
    int k_start = Y.get_starting_index_of_column(0);
    for(int column=0; column<nsize; ++column)
    {
		//complex<double> voltage = psdb.get_bus_complex_voltage_in_pu(column_physical_bus);
		complex<double> voltage = get_bus_complex_voltage_in_pu_with_internal_bus_number(column);

        int k_end = Y.get_starting_index_of_column(column+1);

        // parallelization of the following codes is time-consuming since the k_end-k_start is usually very small
        for(int k=k_start; k<k_end; ++k)
        {
            int row = Y.get_row_number_of_entry_index(k);
            I_mismatch[row] += Y.get_entry_value(k)*voltage;
            /*
            if(not detailed_log_enabled)
                continue;
            else
            {
                complex<double> current = Y.get_entry_value(k)*voltage;
                if(isnan(current.real()) or isnan(current.imag()))
                {
                    size_t column_physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(column);
                    osstream<<"NAN is detected in "<<__FUNCTION__<<"() with Y["
                            <<row<<"("<<network_matrix.get_physical_bus_number_of_internal_bus(row)<<", "
                            <<column<<"("<<column_physical_bus<<"] = "<<Y.get_entry_value(k)<<", voltage["
                            <<column<<"("<<column_physical_bus<<"] = "<<voltage;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            */
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
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t ngen = generators.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_generator_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<ngen; ++i)
    {
        GENERATOR* generator = generators[i];

        if(generator->get_status() == true)
        {
            size_t physical_bus = generator->get_generator_bus();

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
            if(gen_model!=NULL)
            {
                //I = gen_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //complex<double> I = gen_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //I_mismatch[internal_bus] += I;
                I_mismatch[internal_bus] += gen_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();

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
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t ngen = wt_generators.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_wt_generator_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<ngen; ++i)
    {
        complex<double> I;
        WT_GENERATOR* generator = wt_generators[i];

        if(generator->get_status() == true)
        {
            size_t physical_bus = generator->get_generator_bus();

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

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

void DYNAMICS_SIMULATOR::add_pv_units_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t npv = pv_units.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_pv_unit_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<npv; ++i)
    {
        complex<double> I;
        PV_UNIT* pv = pv_units[i];

        if(pv->get_status() == true)
        {
            size_t physical_bus = pv->get_unit_bus();

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            PV_CONVERTER_MODEL* conv_model = pv->get_pv_converter_model();
            if(conv_model!=NULL)
            {
                if(conv_model->is_current_source())
                    I = conv_model->get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
                else
                    I = conv_model->get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
                //cout<<generator->get_device_name()<<" terminal or Norton current is: "<<I<<endl;

                I_mismatch[internal_bus] += I;

                /*os<< "Generator %u source current: %f + j%f",physical_bus, I.real(), I.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);
                complex<double> Edq = conv_model->get_internal_voltage_in_pu_in_dq_axis();
                complex<double> Exy = conv_model->get_internal_voltage_in_pu_in_xy_axis();
                osstream<<"Generator %u internal voltage: %f + j%f (dq), %f + j%f (xy)",physical_bus, Edq.real(), Edq.imag(), Exy.real(), Exy.imag());
                toolkit.show_information_with_leading_time_stamp(osstream);*/
            }
        }
    }
}

void DYNAMICS_SIMULATOR::add_loads_to_bus_current_mismatch()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t nload = loads.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_load_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<nload; ++i)
    {
        LOAD* load = loads[i];
        if(load->get_status()==true)
        {
            size_t physical_bus = load->get_load_bus();

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            I_mismatch[internal_bus] -= load->get_dynamics_load_current_in_pu_based_on_system_base_power();
        }
    }
}
void DYNAMICS_SIMULATOR::add_hvdcs_to_bus_current_mismatch()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    size_t nhvdc = hvdcs.size();

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_hvdc_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<nhvdc; ++i)
    {
        complex<double> I;
        HVDC* hvdc = hvdcs[i];
        if(hvdc->get_status()==true)
        {
            size_t physical_bus = hvdcs[i]->get_converter_bus(RECTIFIER);

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            //I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);

            //I_mismatch[internal_bus] -= I;
            I_mismatch[internal_bus] -= hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);

            if(not detailed_log_enabled)
                ;
            else
            {
                I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(RECTIFIER);
                I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                osstream<<"Current at rectifier side of "<<hvdc->get_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                        <<"Complex power = "<<SQRT3*psdb.get_bus_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            physical_bus = hvdcs[i]->get_converter_bus(INVERTER);

            internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            //I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);

            //I_mismatch[internal_bus] -= I;
            I_mismatch[internal_bus] -= hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);

            if(not detailed_log_enabled)
                ;
            else
            {
                I = hvdcs[i]->get_converter_dynamic_current_in_pu_based_on_system_base_power(INVERTER);
                I *= (psdb.get_system_base_power_in_MVA()/(SQRT3*psdb.get_bus_base_voltage_in_kV(physical_bus)));
                osstream<<"Current at inverter side of "<<hvdc->get_device_name()<<": "<<I<<"kA or "<<abs(I)<<"kA"<<endl
                        <<"Complex power = "<<SQRT3*psdb.get_bus_complex_voltage_in_kV(physical_bus)*conj(I)<<" MVA";
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

    double one_over_sbase = psdb.get_one_over_system_base_power_in_one_over_MVA();

    size_t nedevice = e_devices.size();

    //The following codes should not be parallelized. If more than one device is connecting to the same bus, unintended sum of I_mismatch[internal_bus] will occur.
    for(size_t i=0; i<nedevice; ++i)
    {
        if(e_devices[i]->get_status() == true)
        {
            size_t physical_bus = e_devices[i]->get_equivalent_device_bus();

            size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            complex<double> S = e_devices[i]->get_total_equivalent_power_as_load_in_MVA()*one_over_sbase;

            //V = psdb.get_bus_complex_voltage_in_pu(physical_bus);
            complex<double> V = get_bus_complex_voltage_in_pu_with_internal_bus_number(internal_bus);

            I_mismatch[internal_bus] -= conj(S/V);
        }
    }
}

bool DYNAMICS_SIMULATOR::is_converged()
{
    calculate_bus_power_mismatch_in_MVA();

    double smax = get_max_power_mismatch_struct().greatest_mismatch_in_MVA;

    double s_allowed = get_allowed_max_power_imbalance_in_MVA();

    return (smax>s_allowed)? false : true;
}

void DYNAMICS_SIMULATOR:: calculate_bus_power_mismatch_in_MVA()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    double sbase = psdb.get_system_base_power_in_MVA();

    S_mismatch = I_mismatch;

    size_t n = I_mismatch.size();
    complex<double> V;
    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i= 0; i<n; ++i)
    {
        //complex<double> V = ;
        S_mismatch[i] = get_bus_complex_voltage_in_pu_with_internal_bus_number(i)*conj(S_mismatch[i])*sbase;
    }
}

POWER_MISMATCH_STRUCT DYNAMICS_SIMULATOR::get_max_power_mismatch_struct()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double smax = 0.0;
    size_t smax_bus = 0;
    size_t n = S_mismatch.size();
    for(size_t i=0; i!=n; ++i)
    {
        double s = steps_fast_complex_abs(S_mismatch[i]);
        if(s>smax)
        {
            smax = s;
            smax_bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        }
    }
    POWER_MISMATCH_STRUCT s_mismatch;
    s_mismatch.greatest_mismatch_in_MVA = smax;
    s_mismatch.bus_with_greatest_mismatch = smax_bus;

    return s_mismatch;
}


void DYNAMICS_SIMULATOR::build_bus_current_mismatch_vector()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = I_mismatch.size();
    I_vec.resize(n*2, 0.0);

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        I_vec[i] = I_mismatch[i].imag();
        I_vec[i+n] = I_mismatch[i].real();
    }
}


void DYNAMICS_SIMULATOR::update_bus_voltage()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = delta_V.size();
    n = n>>1;

    #ifdef ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
        set_openmp_number_of_threads(toolkit.get_bus_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR
    for(size_t i=0; i<n; ++i)
    {
        BUS* bus = internal_bus_pointers[i];
        double vang0 = bus->get_positive_sequence_angle_in_rad();
        complex<double> V0 = bus->get_positive_sequence_complex_voltage_in_pu();

        complex<double> delta_v = complex<double>(delta_V[i], delta_V[i+n]);

        complex<double> V = V0-delta_v*alpha;

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


        bus->set_positive_sequence_voltage_in_pu(vmag_new);
        bus->set_positive_sequence_angle_in_rad(vang_new);

        internal_bus_complex_voltage_in_pu[i] = bus->get_positive_sequence_complex_voltage_in_pu();
    }
}


void DYNAMICS_SIMULATOR::build_jacobian()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    jacobian.clear();
    SPARSE_MATRIX& Y = network_matrix.get_dynamic_network_Y_matrix();
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


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_set(size_t bus, const FAULT& fault)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus);
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = busptr->get_positive_sequence_voltage_in_pu();
    double vbase = busptr->get_base_voltage_in_kV();
    double one_over_sbase = psdb.get_one_over_system_base_power_in_one_over_MVA();
    double zbase = vbase*vbase*one_over_sbase;
    double fault_x = -zbase/fault_b;
    if(fault_x<1)
        busptr->set_positive_sequence_voltage_in_pu(0.05);
    else
    {
        if(fault_x<5)
            busptr->set_positive_sequence_voltage_in_pu(0.1);
        else
        {
            if(fault_x<10)
                busptr->set_positive_sequence_voltage_in_pu(0.2);
            else
            {
                if(fault_x<100)
                    busptr->set_positive_sequence_voltage_in_pu(0.5);
                else
                    busptr->set_positive_sequence_voltage_in_pu(current_voltage-0.1);
            }
        }
    }
    NETWORK_MATRIX& network = get_network_matrix();
    size_t internal_bus = network.get_internal_bus_number_of_physical_bus(bus);
    internal_bus_complex_voltage_in_pu[internal_bus] = busptr->get_positive_sequence_complex_voltage_in_pu();
}

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_bus_fault_cleared(size_t bus, const FAULT& fault)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    BUS* busptr = psdb.get_bus(bus);
    double fault_b = fault.get_fault_shunt_in_pu().imag();
    double current_voltage = busptr->get_positive_sequence_voltage_in_pu();
    busptr->set_positive_sequence_voltage_in_pu(0.8);;
    return;

    if(fault_b<-2e8)
        busptr->set_positive_sequence_voltage_in_pu(current_voltage+0.95);
    else
    {
        if(fault_b<-2e7)
            busptr->set_positive_sequence_voltage_in_pu(current_voltage+0.9);
        else
        {
            if(fault_b<-2e6)
                busptr->set_positive_sequence_voltage_in_pu(current_voltage+0.8);
            else
            {
                if(fault_b<-2e5)
                    busptr->set_positive_sequence_voltage_in_pu(current_voltage+0.7);
                else
                {
                    if(fault_b<-2e4)
                        busptr->set_positive_sequence_voltage_in_pu(current_voltage+0.6);
                    else
                        busptr->set_positive_sequence_voltage_in_pu(current_voltage+0.1);
                }
            }
        }
    }
    NETWORK_MATRIX& network = get_network_matrix();
    size_t internal_bus = network.get_internal_bus_number_of_physical_bus(bus);
    internal_bus_complex_voltage_in_pu[internal_bus] = busptr->get_positive_sequence_complex_voltage_in_pu();
}


void DYNAMICS_SIMULATOR::guess_bus_voltage_with_line_fault_set(const DEVICE_ID& did, size_t side_bus, double location, const FAULT& fault)
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

void DYNAMICS_SIMULATOR::guess_bus_voltage_with_line_fault_cleared(const DEVICE_ID& did, size_t side_bus, double location, const FAULT& fault)
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
    double TIME = TIME;
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

void DYNAMICS_SIMULATOR::set_bus_fault(size_t bus, const complex<double>& fault_shunt)
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

            osstream<<fault.get_fault_type_string()<<" will be set for bus "<<bus<<" at time "<<TIME<<" s."<<endl
                   <<"Fault shunt is"<<fault_shunt<<" pu.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            busptr->set_fault(fault);
            guess_bus_voltage_with_bus_fault_set(bus, fault);

            //network_matrix.build_dynamic_network_Y_matrix();
            //build_jacobian();
        }
        else
        {
            osstream<<"Zero fault shunt is given for bus "<<bus<<"."<<endl
                   <<"No fault will be set for bus "<<bus<<" at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Bus "<<bus<<" does not exist in power system database."<<endl
               <<"No bus fault will be set at time "<<TIME<<" s.";
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

        osstream<<"Fault at bus "<<bus<<" will be cleared at time "<<TIME<<" s.";
        toolkit.show_information_with_leading_time_stamp(osstream);

        FAULT fault = busptr->get_fault();
        busptr->clear_fault();
        guess_bus_voltage_with_bus_fault_cleared(bus, fault);

        //network_matrix.build_dynamic_network_Y_matrix();
        //build_jacobian();
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

            osstream<<"Bus "<<bus<<" will be tripped at time "<<TIME<<" s. Devices connecting to bus "<<bus<<" will also be tripped.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            psdb.trip_bus(bus);

            optimize_network_ordering();
            //network_matrix.build_dynamic_network_Y_matrix();
            //build_jacobian();
        }
    }
}

void DYNAMICS_SIMULATOR::trip_buses(const vector<size_t>& buses)
{
    size_t n = buses.size();
    for(size_t i=0; i!=n; ++i)
        trip_bus(buses[i]);
}

void DYNAMICS_SIMULATOR::set_line_fault(const DEVICE_ID& line_id, size_t side_bus, double location, const complex<double>& fault_shunt)
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

                        osstream<<fault.get_fault_type_string()<<" will be set for "<<lineptr->get_device_name()<<" at time "<<TIME<<" s."<<endl
                               <<"Fault shunt is "<<fault_shunt<<" pu at location "<<location<<" to bus "<<side_bus;
                        toolkit.show_information_with_leading_time_stamp(osstream);


                        lineptr->set_fault(side_bus,location, fault);
                        guess_bus_voltage_with_line_fault_set(line_id, side_bus, location, fault);

                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();
                    }
                    else
                    {
                        osstream<<"Zero fault shunt is given for "<<lineptr->get_device_name()<<"."<<endl
                               <<"No fault will be set for "<<lineptr->get_device_name()<<" at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_device_name()<<"."<<endl
                           <<"No fault will be set at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<lineptr->get_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be set at location "<<location<<" to bus "<<side_bus<<" at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
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
                           <<lineptr->get_device_name()<<" at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);

                    FAULT fault = lineptr->get_fault_at_location(side_bus, location);
                    lineptr->clear_fault_at_location(side_bus, location);
                    guess_bus_voltage_with_line_fault_cleared(line_id, side_bus, location, fault);

                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_device_name()<<"."<<endl
                           <<"No fault will be cleared at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    return;
                }
            }
            else
            {
                osstream<<lineptr->get_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be cleared at location "<<location<<" to bus "<<side_bus<<" at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
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
                osstream<<lineptr->get_device_name()<<" is tripped by taking the following actions at time "<<TIME<<" s."<<endl;
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
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();

                    osstream<<lineptr->get_device_name()<<" breaker at sending side (bus "<<lineptr->get_sending_side_bus()<<") is tripped at time "<<TIME<<" s.";
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
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();

                        osstream<<lineptr->get_device_name()<<" breaker at receiving side (bus "<<lineptr->get_receiving_side_bus()<<") is tripped at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
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
                osstream<<lineptr->get_device_name()<<" is closed by taking the following actions at time "<<TIME<<" s."<<endl;
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
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                    osstream<<lineptr->get_device_name()<<" breaker at sending side (bus "<<lineptr->get_sending_side_bus()<<") is closed at time "<<TIME<<" s.";
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
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();
                        osstream<<lineptr->get_device_name()<<" breaker at receiving side (bus "<<lineptr->get_sending_side_bus()<<") is closed at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_device_name()<<" does not exist in power system database."<<endl
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
                osstream<<transptr->get_device_name()<<" is tripped by taking the following actions at time "<<TIME<<" s."<<endl;
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
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();

                    osstream<<transptr->get_device_name()<<" breaker at primary side (bus "<<side_bus<<") is tripped at time "<<TIME<<" s.";
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
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();

                        osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is tripped at time "<<TIME<<" s.";
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
                            //network_matrix.build_dynamic_network_Y_matrix();
                            //build_jacobian();

                            osstream<<transptr->get_device_name()<<" breaker at tertiary side (bus "<<side_bus<<") is tripped at time "<<TIME<<" s.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }

                }
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
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
                osstream<<transptr->get_device_name()<<" is closed by taking the following actions at time "<<TIME<<" s."<<endl;
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
                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                    osstream<<transptr->get_device_name()<<" breaker at primary side (bus "<<side_bus<<") is closed at time "<<TIME<<" s.";
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
                        //network_matrix.build_dynamic_network_Y_matrix();
                        //build_jacobian();
                        osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<TIME<<" s.";
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
                            //network_matrix.build_dynamic_network_Y_matrix();
                            //build_jacobian();
                            osstream<<transptr->get_device_name()<<" breaker at secondary side (bus "<<side_bus<<") is closed at time "<<TIME<<" s.";
                            toolkit.show_information_with_leading_time_stamp(osstream);
                        }
                    }
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<trans_id.get_device_name()<<" does not exist in power system database."<<endl
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

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<generator->get_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
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
                osstream<<generator->get_device_name()<<" is shed by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl
                  <<"MBASE is changed from "<<mbase<<" MVA to ";

                generator->set_mbase_in_MVA(mbase*(1.0-percent));
                osstream<<generator->get_mbase_in_MVA()<<" MVA.";
                toolkit.show_information_with_leading_time_stamp(osstream);

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();
            }
            else
            {
                osstream<<"Generator relay percent is 0.0 for "<<generator->get_device_name()<<endl
                       <<"No generator will be shed at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
                   <<"No generator will be shed at time "<<TIME<<" s.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a GENERATOR (it is a "<<gen_id.get_device_type()<<") for relay generator."<<endl
               <<"No generator will be shed at time "<<TIME<<" s."<<endl;
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

                        osstream<<n<<" individual generators of "<<generator->get_device_name()<<" are tripped at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                    else
                    {
                        generator->set_status(false);

                        osstream<<"All individual generators of "<<generator->get_device_name()<<" are tripped at time "<<TIME<<" s.";
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }

                    //network_matrix.build_dynamic_network_Y_matrix();
                    //build_jacobian();
                }
                else
                {
                    osstream<<n<<" = 0, and no individual generators of "<<generator->get_device_name()<<" will be tripped at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
        }
        else
        {
            osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist in power system database."<<endl
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

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<load->get_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
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

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<load->get_device_name()<<" is closed at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
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
                    osstream<<load->get_device_name()<<" is manually scaled "<<(percent>0.0?"up":"down")<<" by "<<percent*100.0<<"% at time "<<TIME<<" s."<<endl;
                    double scale = load->get_load_manually_scale_factor_in_pu();
                    load->set_load_manually_scale_factor_in_pu(scale+percent);
                    osstream<<"Load manual scale is changed from "<<scale<<" to "<<scale+percent<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                else
                {
                    osstream<<load->get_device_name()<<" is out of service. No load scale action can be applied at time "<<TIME<<" s.";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<"Load scaling percent is 0.0 for "<<load->get_device_name()<<endl
                       <<"No load will be scaled at time "<<TIME<<" s."<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                return;
            }
        }
        else
        {
            osstream<<"Warning. "<<load_id.get_device_name()<<" does not exist in power system database."<<endl
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
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(fabs(percent)>FLOAT_EPSILON)
    {
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
               <<"No all load will be scaled at time "<<TIME<<" s."<<endl;
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

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<shunt->get_device_name()<<" is tripped at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<shunt_id.get_device_name()<<" does not exist in power system database."<<endl
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

                //network_matrix.build_dynamic_network_Y_matrix();
                //build_jacobian();

                osstream<<shunt->get_device_name()<<" is closed at time "<<TIME<<" s.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<shunt_id.get_device_name()<<" does not exist in power system database."<<endl
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


double DYNAMICS_SIMULATOR::get_generator_voltage_reference_in_pu(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter!=NULL)
            return exciter->get_voltage_reference_in_pu();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_mechanical_power_reference_in_pu_based_on_mbase(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
            return tg->get_mechanical_power_reference_in_pu_based_on_mbase();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_mechanical_power_reference_in_MW(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
            return tg->get_mechanical_power_reference_in_pu_based_on_mbase()*generator->get_mbase_in_MVA();
        else
            return 0.0;
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_excitation_voltage_in_pu(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        EXCITER_MODEL* exciter = generator->get_exciter_model();
        if(exciter!=NULL)
            return exciter->get_excitation_voltage_in_pu();
        else
        {
            SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
            if(sg!=NULL)
                return sg->get_initial_excitation_voltage_in_pu();
            else
                return 0.0;
        }
    }
    else
        return 0.0;
}

double DYNAMICS_SIMULATOR::get_generator_mechanical_power_in_pu_based_on_mbase(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
            return tg->get_mechanical_power_in_pu_based_on_mbase();
        else
        {
            SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
            if(sg!=NULL)
                return sg->get_initial_mechanical_power_in_pu_based_on_mbase();
            else
                return 0.0;
        }
    }
    else
        return 0.0;
}
double DYNAMICS_SIMULATOR::get_generator_mechanical_power_in_MW(const DEVICE_ID& gen_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
            return tg->get_mechanical_power_in_pu_based_on_mbase()*generator->get_mbase_in_MVA();
        else
        {
            SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
            if(sg!=NULL)
                return sg->get_initial_mechanical_power_in_pu_based_on_mbase()*generator->get_mbase_in_MVA();
            else
                return 0.0;
        }
    }
    else
        return 0.0;
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
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist when trying to change excitation voltage in pu. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_reference_in_pu_based_on_mbase(const DEVICE_ID& gen_id, double Pref)
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
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist when trying to change mechanical power reference in pu based on MBASE. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_reference_in_MW(const DEVICE_ID& gen_id, double Pref)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        double one_over_mbase = generator->get_one_over_mbase_in_one_over_MVA();
        change_generator_mechanical_power_reference_in_pu_based_on_mbase(gen_id, Pref*one_over_mbase);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist when trying to change mechanical power reference in MW. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_excitation_voltage_in_pu(const DEVICE_ID& gen_id, double efd)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        EXCITER_MODEL* ex = generator->get_exciter_model();
        if(ex!=NULL)
        {
            ostringstream osstream;
            osstream<<"Warning. Exciter model exists for "<<generator->get_device_name()<<". No manual change of excitation voltage is allowed.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
            if(sg!=NULL)
                sg->set_initial_excitation_voltage_in_pu(efd);
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist when trying to change excitation voltage. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_in_pu_based_on_mbase(const DEVICE_ID& gen_id, double pmech)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        TURBINE_GOVERNOR_MODEL* tg = generator->get_turbine_governor_model();
        if(tg!=NULL)
        {
            ostringstream osstream;
            osstream<<"Warning. Turbine governor model exists for "<<generator->get_device_name()<<". No manual change of mechanical power is allowed.";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            SYNC_GENERATOR_MODEL* sg = generator->get_sync_generator_model();
            if(sg!=NULL)
                sg->set_initial_mechanical_power_in_pu_based_on_mbase(pmech);
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist when trying to change mechanical power in pu based on MBASE. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::change_generator_mechanical_power_in_MW(const DEVICE_ID& gen_id, double pmech)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    GENERATOR* generator = psdb.get_generator(gen_id);
    if(generator != NULL)
    {
        double one_over_mbase = generator->get_one_over_mbase_in_one_over_MVA();
        change_generator_mechanical_power_in_pu_based_on_mbase(gen_id, pmech*one_over_mbase);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<gen_id.get_device_name()<<" does not exist when trying to change mechanical power in MW. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

double DYNAMICS_SIMULATOR::get_hvdc_power_order_in_MW(const DEVICE_ID& hvdc_id)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVDC* hvdc = psdb.get_hvdc(hvdc_id);
    if(hvdc != NULL)
        return hvdc->get_nominal_dc_power_per_pole_in_MW();
    else
        return 0.0;
}

void DYNAMICS_SIMULATOR::change_hvdc_power_order_in_MW(const DEVICE_ID& hvdc_id, double porder)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    HVDC* hvdc = psdb.get_hvdc(hvdc_id);
    if(hvdc != NULL)
    {
        hvdc->set_nominal_dc_power_per_pole_in_MW(porder);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. "<<hvdc_id.get_device_name()<<" does not exist when trying to change hvdc power order in MW. No change is made";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::switch_on_equivalent_device()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    size_t n = e_devices.size();
    ostringstream osstream;
    osstream<<"There are "<<n<<" equivalent devices to switch on";
    toolkit.show_information_with_leading_time_stamp(osstream);
    for(size_t i=0; i!=n; ++i)
        e_devices[i]->switch_on();
}

bool DYNAMICS_SIMULATOR::is_valid() const
{
    return true; // disabled
}
void DYNAMICS_SIMULATOR::check()
{
    ;// disabled
}
