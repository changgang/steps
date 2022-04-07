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
#include <chrono>

using namespace std;
using namespace chrono;

class STEPS;

#define ENABLE_OPENMP_FOR_DYNAMIC_SIMULATOR

#define USE_DYNAMIC_CURRENT_MISMATCH_CONTROL

void DYNAMICS_SIMULATOR::prepare_meters()
{
    prepare_bus_related_meters();
    prepare_generator_related_meters();
    prepare_wt_generator_related_meters();
    prepare_pv_unit_related_meters();
    prepare_energy_storage_related_meters();
    prepare_load_related_meters();
    prepare_line_related_meters();
    prepare_transformer_related_meters();
    prepare_hvdc_related_meters();
    prepare_vsc_hvdc_related_meters();
    prepare_equivalent_device_related_meters();
}


void DYNAMICS_SIMULATOR::prepare_bus_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n = psdb.get_bus_count();
    vector<BUS*> buses = psdb.get_all_buses();
    BUS* bus;
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_voltage_in_pu_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_angle_in_deg_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_frequency_deviation_in_pu_meter(bus->get_bus_number());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = buses[i];
        METER meter = setter.prepare_bus_frequency_in_Hz_meter(bus->get_bus_number());
        append_meter(meter);
    }
}


void DYNAMICS_SIMULATOR::prepare_generator_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_generator_count();
    vector<GENERATOR*> generators = psdb.get_all_generators();
    GENERATOR* generator;
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_rotor_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_rotor_speed_deviation_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_excitation_voltage_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_stabilizing_signal_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_mechanical_power_reference_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_terminal_active_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_generator_terminal_reactive_power_in_MVar_meter(generator->get_device_id());
        append_meter(meter);
    }
}

void DYNAMICS_SIMULATOR::prepare_wt_generator_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_wt_generator_count();
    vector<WT_GENERATOR*> generators = psdb.get_all_wt_generators();
    WT_GENERATOR* generator;
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_terminal_current_in_kA_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_terminal_active_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_terminal_reactive_power_in_MVar_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_max_available_mechanical_power_in_MW_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_speed_reference_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_turbine_speed_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_rotor_speed_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_rotor_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_active_current_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_reactive_current_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_active_power_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_reactive_power_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_reactive_voltage_command_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_pitch_angle_in_deg_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_wind_speed_in_pu_meter(generator->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        generator = generators[i];
        METER meter = setter.prepare_wt_generator_wind_speed_in_mps_meter(generator->get_device_id());
        append_meter(meter);
    }
}


void DYNAMICS_SIMULATOR::prepare_pv_unit_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_pv_unit_count();
    vector<PV_UNIT*> pv_units = psdb.get_all_pv_units();
    PV_UNIT* pv_unit;
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_current_in_kA_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_active_power_in_MW_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_terminal_reactive_power_in_MVar_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_active_current_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_current_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_active_power_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_power_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
    {
        pv_unit = pv_units[i];
        METER meter = setter.prepare_pv_unit_reactive_voltage_command_in_pu_meter(pv_unit->get_device_id());
        append_meter(meter);
    }
    for(unsigned int i=0; i!=n; ++i)
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
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_load_count();
    vector<LOAD*> loads = psdb.get_all_loads();
    LOAD* load;
    for(unsigned int i=0; i!=n; ++i)
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
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_line_count();
    vector<LINE*> lines = psdb.get_all_lines();
    LINE* line;
    for(unsigned int i=0; i!=n; ++i)
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
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_transformer_count();
    vector<TRANSFORMER*> transes = psdb.get_all_transformers();
    TRANSFORMER* trans;
    for(unsigned int i=0; i!=n; ++i)
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
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_hvdc_count();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    HVDC* hvdc;
    for(unsigned int i=0; i!=n; ++i)
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

void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_vsc_hvdc_count();
    vector<VSC_HVDC*> vsc_hvdcs = psdb.get_all_vsc_hvdcs();
    VSC_HVDC* vsc_hvdc;
    DEVICE_ID did;
    for(unsigned int i=0; i!=n; ++i)
    {
        vsc_hvdc = vsc_hvdcs[i];
        did = vsc_hvdc->get_device_id();

        unsigned int n_converter = vsc_hvdc->get_converter_count();
        for(unsigned int j=0; j!=n_converter; ++j)
        {
            unsigned int meter_side = vsc_hvdc->get_converter_ac_bus(j);
            METER meter = setter.prepare_vsc_hvdc_converter_dc_current_in_kA_meter(did, meter_side);
            append_meter(meter);

            meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, meter_side);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, meter_side);
            append_meter(meter);
        }
        unsigned int n_dc_bus = vsc_hvdc->get_dc_bus_count();
        for(unsigned int j=0; j!=n_dc_bus; ++j)
        {
            unsigned int meter_side = vsc_hvdc->get_dc_bus_number(j);
            METER meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, meter_side);
            append_meter(meter);
        }
        unsigned int n_dc_line = vsc_hvdc->get_dc_line_count();
        for(unsigned int j=0; j!=n_dc_line; ++j)
        {
            DC_DEVICE_ID dc_line_did = vsc_hvdc->get_dc_line_device_id(j);

            unsigned int ibus = vsc_hvdc->get_dc_line_sending_side_bus(j);
            unsigned int jbus = vsc_hvdc->get_dc_line_receiving_side_bus(j);

            METER meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_line_did, ibus);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_line_did, jbus);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_line_did, ibus);
            append_meter(meter);
            meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_line_did, jbus);
            append_meter(meter);
        }
    }
}

void DYNAMICS_SIMULATOR::prepare_equivalent_device_related_meters()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    METER_SETTER setter(*toolkit);

    unsigned int n;

    n = psdb.get_equivalent_device_count();
    vector<EQUIVALENT_DEVICE*> edevices = psdb.get_all_equivalent_devices();
    EQUIVALENT_DEVICE* edevice;
    for(unsigned int i=0; i!=n; ++i)
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

void DYNAMICS_SIMULATOR::prepare_bus_related_meter(unsigned int bus, string meter_type)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(psdb.is_bus_exist(bus))
    {
        METER_SETTER setter(*toolkit);

        METER meter(*toolkit);

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
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. Meter of bus "<<bus<<" cannot be set since the given bus does not exist in the power system database";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_GENERATOR)
    {
        if(psdb.is_generator_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not GENERATOR when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_wt_generator_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_WT_GENERATOR)
    {
        if(psdb.is_wt_generator_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not WT GENERATOR when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_pv_unit_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_PV_UNIT)
    {
        if(psdb.is_pv_unit_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not PV UNIT when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_energy_storage_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_ENERGY_STORAGE)
    {
        if(psdb.is_pv_unit_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not ENERGY STORAGE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_load_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_LOAD)
    {
        if(psdb.is_load_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
            if(meter_type=="LOAD MODEL INTERNAL VARIABLE")
                meter = setter.prepare_load_load_model_internal_variable_meter(did, var_name);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not LOAD when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_line_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_LINE)
    {
        if(psdb.is_line_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            LINE* line = psdb.get_line(did);
            unsigned int bus = 0;
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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not LINE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void DYNAMICS_SIMULATOR::prepare_transformer_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_TRANSFORMER)
    {
        if(psdb.is_transformer_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            side = string2upper(side);

            TRANSFORMER* trans = psdb.get_transformer(did);
            unsigned int bus = 0;
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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not TRANSFORMER when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}


void DYNAMICS_SIMULATOR::prepare_hvdc_related_meter(const DEVICE_ID& did, string meter_type, string side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_HVDC)
    {
        if(psdb.is_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not HVDC when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_meter(const DEVICE_ID& did, string meter_type, unsigned int side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_VSC_HVDC)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="CONVERTER DC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_dc_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN PU")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, side);
            if(meter_type=="CONVERTER DC POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER DC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC ACTIVE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER AC REACTIVE POWER IN MVAR")
                meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, side);
            if(meter_type=="DC BUS VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, side);
            if(meter_type=="VSC HVDC MODEL INTERNAL VARIABLE")
            {
                var_name = string2upper(var_name);
                meter = setter.prepare_vsc_hvdc_model_internal_variable_meter(did, var_name, side);
            }

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not VSC HVDC when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_meter(string vsc_name, string meter_type, unsigned int side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    VSC_HVDC* vsc_hvdc = psdb.get_vsc_hvdc(vsc_name);
    DEVICE_ID did = vsc_hvdc->get_device_id();

    if(vsc_hvdc != NULL)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);
            if(meter_type=="CONVERTER DC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_dc_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_converter_ac_current_in_kA_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC VOLTAGE IN PU")
                meter = setter.prepare_vsc_hvdc_converter_ac_voltage_in_pu_meter(did, side);
            if(meter_type=="CONVERTER DC POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_dc_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER DC VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_converter_dc_voltage_in_kV_meter(did, side);
            if(meter_type=="CONVERTER AC ACTIVE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_converter_ac_active_power_in_MW_meter(did, side);
            if(meter_type=="CONVERTER AC REACTIVE POWER IN MVAR")
                meter = setter.prepare_vsc_hvdc_converter_ac_reactive_power_in_MVar_meter(did, side);
            if(meter_type=="DC BUS VOLTAGE IN KV")
                meter = setter.prepare_vsc_hvdc_dc_bus_voltage_in_kV_meter(did, side);
            if(meter_type=="VSC HVDC MODEL INTERNAL VARIABLE")
            {
                var_name = string2upper(var_name);
                meter = setter.prepare_vsc_hvdc_model_internal_variable_meter(did, var_name, side);
            }

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not VSC HVDC when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}


void DYNAMICS_SIMULATOR::prepare_vsc_hvdc_related_dc_line_meter(const DEVICE_ID& did, string meter_type, const DC_DEVICE_ID dc_did, unsigned int side, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_VSC_HVDC)
    {
        if(psdb.is_vsc_hvdc_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

            meter_type = string2upper(meter_type);

            if(meter_type=="DC LINE CURRENT IN KA")
                meter = setter.prepare_vsc_hvdc_dc_line_current_in_kA_meter(did, dc_did, side);
            if(meter_type=="DC LINE POWER IN MW")
                meter = setter.prepare_vsc_hvdc_dc_line_power_in_MW_meter(did, dc_did, side);

            if(meter.is_valid())
                append_meter(meter);
            else
            {
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not VSC HVDC when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void DYNAMICS_SIMULATOR::prepare_equivalent_device_related_meter(const DEVICE_ID& did, string meter_type, string var_name)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(did.get_device_type()==STEPS_EQUIVALENT_DEVICE)
    {
        if(psdb.is_equivalent_device_exist(did))
        {
            METER_SETTER setter(*toolkit);

            METER meter(*toolkit);

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
                osstream<<"Warning. Invalid meter type ("<<meter_type<<") is given to set up meter for "<<did.get_compound_device_name();
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. Meter of "<<did.get_compound_device_name()<<" cannot be set since the given device does not exist in the power system database";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Warning. The device type of "<<did.get_compound_device_name()<<" is not EQUIVALENT DEVICE when setting up meter with "<<__FUNCTION__;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}
