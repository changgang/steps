#include "header/meter/meter_setter.h"
#include "header/basic/terminal.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

METER_SETTER::METER_SETTER()
{
    ;
}

METER_SETTER::~METER_SETTER()
{
    ;
}


bool METER_SETTER::prepare_bus_meter(METER& meter, const size_t bus)
{
    bool successful = false;

    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    if(not psdb.is_bus_exist(bus))
    {
        osstream<<"Bus "<<bus<<" doesn't exist in power system database '"<<psdb.get_system_name()<<"'."<<endl
          <<"Invalid meter will be returned.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    BUS* busptr = psdb.get_bus(bus);
    DEVICE_ID did = busptr->get_device_id();

    meter.set_device_id(did);

    successful = true;
    return successful;
}

METER METER_SETTER::prepare_bus_voltage_in_pu_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(not successful)
        return meter;
    else
    {
        meter.set_meter_type("VOLTAGE IN PU");
        return meter;
    }
}
METER METER_SETTER::prepare_bus_voltage_in_kV_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(not successful)
        return meter;
    else
    {
        meter.set_meter_type("VOLTAGE IN KV");
        return meter;
    }
}

METER METER_SETTER::prepare_bus_angle_in_deg_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(not successful)
        return meter;
    else
    {
       meter.set_meter_type("ANGLE IN DEG");
        return meter;
    }
}

METER METER_SETTER::prepare_bus_angle_in_rad_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(not successful)
        return meter;
    else
    {
       meter.set_meter_type("ANGLE IN RAD");
        return meter;
    }
}

METER METER_SETTER::prepare_bus_frequency_in_pu_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("FREQUENCY IN PU");
    return meter;
}

METER METER_SETTER::prepare_bus_frequency_in_Hz_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("FREQUENCY IN HZ");
    return meter;
}

METER METER_SETTER::prepare_bus_frequency_deviation_in_pu_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("FREQUENCY DEVIATION IN PU");
    return meter;
}

METER METER_SETTER::prepare_bus_frequency_deviation_in_Hz_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("FREQUENCY DEVIATION IN HZ");
    return meter;
}

METER METER_SETTER::prepare_bus_ROCOV_in_pu_per_s_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("ROCOV IN PU/S");
    return meter;
}

METER METER_SETTER::prepare_bus_ROCOV_in_kV_per_s_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("ROCOV IN KV/S");
    return meter;
}

METER METER_SETTER::prepare_bus_ROCOF_in_pu_per_s_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("ROCOF IN PU/S");
    return meter;
}

METER METER_SETTER::prepare_bus_ROCOF_in_Hz_per_s_meter(size_t bus)
{
    METER meter;

    bool successful = prepare_bus_meter(meter, bus);

    if(successful)
        meter.set_meter_type("ROCOF IN HZ/S");
    return meter;
}

bool METER_SETTER::prepare_line_meter(METER& meter, const DEVICE_ID& device_id, size_t side_bus)
{
    meter.clear();

    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="LINE")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not a line when trying to set up a line meter. "
          <<"No line meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_line_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No line meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    LINE* lineptr = psdb.get_line(device_id);

    if(lineptr->is_connected_to_bus(side_bus))
        meter.set_meter_side_bus(side_bus);
    else
    {
        osstream<<"Warning. The side bus "<<side_bus<<" is neither the sending bus nor receiving bus of "<<device_id.get_device_name()<<". "
          <<"No line meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    successful = true;
    return successful;
}


METER METER_SETTER::prepare_line_current_in_kA_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("CURRENT IN KA");
    return meter;
}

METER METER_SETTER::prepare_line_active_power_in_MW_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_line_reactive_power_in_MVar_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_line_apparent_impedance_in_Ohm_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("APPARENT IMPEDANCE IN OHM");

    return meter;
}

METER METER_SETTER::prepare_line_apparent_impedance_angle_in_deg_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("APPARENT IMPEDANCE ANGLE IN DEG");

    return meter;
}

METER METER_SETTER::prepare_line_current_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("CURRENT IN PU");
    return meter;
}

METER METER_SETTER::prepare_line_active_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN PU");

    return meter;
}

METER METER_SETTER::prepare_line_reactive_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN PU");

    return meter;
}

METER METER_SETTER::prepare_line_apparent_impedance_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("APPARENT IMPEDANCE IN PU");

    return meter;
}

METER METER_SETTER::prepare_line_apparent_impedance_angle_in_rad_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_line_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("APPARENT IMPEDANCE ANGLE IN RAD");

    return meter;
}


bool METER_SETTER::prepare_transformer_meter(METER& meter, const DEVICE_ID& device_id, size_t side_bus)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="TRANSFORMER")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not a transformer when trying to set up a transformer meter. "
          <<"No line meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }


    if(not psdb.is_transformer_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No transformer meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    TRANSFORMER* transptr = psdb.get_transformer(device_id);

    if(transptr->is_connected_to_bus(side_bus))
        meter.set_meter_side_bus(side_bus);
    else
    {
        osstream<<"Warning. The side bus "<<side_bus<<" is not primary, secondary, or tertiary bus of "<<device_id.get_device_name()<<". "
          <<"No transformer meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    successful = true;
    return successful;
}



METER METER_SETTER::prepare_transformer_current_in_kA_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_transformer_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_transformer_active_power_in_MW_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_transformer_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_transformer_reactive_power_in_MVar_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_transformer_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN MVAR");

    return meter;
}


METER METER_SETTER::prepare_transformer_current_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_transformer_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("CURRENT IN PU");

    return meter;
}

METER METER_SETTER::prepare_transformer_active_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_transformer_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN PU");

    return meter;
}

METER METER_SETTER::prepare_transformer_reactive_power_in_pu_meter(const DEVICE_ID& device_id, size_t side_bus)
{
    METER meter;

    bool successful = prepare_transformer_meter(meter, device_id, side_bus);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN PU");

    return meter;
}

bool METER_SETTER::prepare_load_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="LOAD")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not a load when trying to set up a load meter. "
          <<"No load meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_load_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No load meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}


METER METER_SETTER::prepare_load_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_load_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_load_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_load_current_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("CURRENT IN PU");

    return meter;
}

METER METER_SETTER::prepare_load_active_power_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN PU");

    return meter;
}

METER METER_SETTER::prepare_load_reactive_power_in_pu_meter(const DEVICE_ID& device_id)
{
   METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN PU");

    return meter;
}

METER METER_SETTER::prepare_load_total_scale_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TOTAL SCALE IN PU");

    return meter;
}

METER METER_SETTER::prepare_load_manually_scale_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("MANUALLY SCALE IN PU");

    return meter;
}

METER METER_SETTER::prepare_load_relay_shed_scale_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RELAY SHED SCALE IN PU");

    return meter;
}

METER METER_SETTER::prepare_load_load_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("LOAD MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_load_frequency_relay_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("FREQUENCY RELAY MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_load_voltage_relay_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_load_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("VOLTAGE RELAY MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

bool METER_SETTER::prepare_generator_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="GENERATOR")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not a generator when trying to set up a generator meter. "
          <<"No generator meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_generator_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No generator meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}


METER METER_SETTER::prepare_generator_rotor_angle_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR ANGLE IN DEG");

    return meter;
}

METER METER_SETTER::prepare_generator_rotor_speed_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED IN PU");

    return meter;
}

METER METER_SETTER::prepare_generator_rotor_speed_in_Hz_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED IN HZ");

    return meter;
}

METER METER_SETTER::prepare_generator_rotor_speed_deviation_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED DEVIATION IN PU");

    return meter;
}

METER METER_SETTER::prepare_generator_rotor_speed_deviation_in_Hz_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED DEVIATION IN HZ");

    return meter;
}

METER METER_SETTER::prepare_generator_internal_voltage_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INTERNAL VOLTAGE IN PU");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_current_in_pu_on_mbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN PU ON MBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_current_in_pu_on_sbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN PU ON SBASE");

    return meter;
}


METER METER_SETTER::prepare_generator_terminal_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_active_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL ACTIVE POWER IN PU ON MBASE");

    return meter;
}


METER METER_SETTER::prepare_generator_terminal_active_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL ACTIVE POWER IN PU ON SBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_reactive_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL REACTIVE POWER IN PU ON MBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_reactive_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL REACTIVE POWER IN PU ON SBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_terminal_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_generator_airgap_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("AIRGAP POWER IN PU ON MBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_airgap_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("AIRGAP POWER IN PU ON SBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_airgap_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("AIRGAP POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_generator_accelerating_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACCELERATING POWER IN PU ON MBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_accelerating_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACCELERATING POWER IN PU ON SBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_accelerating_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACCELERATING POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_generator_mechanical_power_in_pu_on_mbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("MECHANICAL POWER IN PU ON MBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_mechanical_power_in_pu_on_sbase_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("MECHANICAL POWER IN PU ON SBASE");

    return meter;
}

METER METER_SETTER::prepare_generator_mechanical_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("MECHANICAL POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_generator_excitation_voltage_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("EXCITATION VOLTAGE IN PU");

    return meter;
}

METER METER_SETTER::prepare_generator_compensated_voltage_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("COMPENSATED VOLTAGE IN PU");

    return meter;
}

METER METER_SETTER::prepare_generator_stabilizing_signal_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("STABILIZING SIGNAL IN PU");

    return meter;
}

METER METER_SETTER::prepare_generator_sync_generator_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("SYNC GENERATOR MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_generator_compensator_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("COMPENSATOR MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_generator_stabilizer_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("STABILIZER MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_generator_exciter_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("EXCITER MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_generator_turbine_governor_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("TURBINE GOVERNOR MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

bool METER_SETTER::prepare_wt_generator_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="WT GENERATOR")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not a wt generator when trying to set up a wt generator meter. "
          <<"No wt generator meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_wt_generator_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No wt generator meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}

METER METER_SETTER::prepare_wt_generator_terminal_current_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_terminal_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_terminal_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_terminal_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_mechanical_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("MECHANICAL POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_max_available_mechanical_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("MAX AVAILABLE MECHANICAL POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_speed_reference_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("SPEED REFERENCE IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_speed_reference_in_rad_per_s_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("SPEED REFERENCE IN RAD/S");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_turbine_speed_deviation_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TURBINE SPEED DEVIATION IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_turbine_speed_deviation_in_Hz_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TURBINE SPEED DEVIATION IN HZ");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_turbine_speed_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TURBINE SPEED IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_turbine_speed_in_Hz_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TURBINE SPEED IN HZ");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_rotor_speed_deviation_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED DEVIATION IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_rotor_speed_deviation_in_Hz_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED DEVIATION IN HZ");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_rotor_speed_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_rotor_speed_in_Hz_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR SPEED IN HZ");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_rotor_angle_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR ANGLE IN DEG");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_rotor_angle_in_rad_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ROTOR ANGLE IN RAD");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_active_current_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CURRENT COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_reactive_current_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CURRENT COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_active_power_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_reactive_power_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_reactive_voltage_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE VOLTAGE COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_pitch_angle_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("PITCH ANGLE IN DEG");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_wind_speed_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("WIND SPEED IN PU");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_wind_speed_in_mps_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("WIND SPEED IN MPS");

    return meter;
}

METER METER_SETTER::prepare_wt_generator_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("WT GENERATOR MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_wt_aerodynamic_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("WT AERODYNAMIC MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_wt_turbine_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("WT TURBINE MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_wt_electrical_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("WT ELECTRICAL MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_wt_pitch_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("WT PITCH MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_wind_speed_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_wt_generator_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("WIND SPEED MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

bool METER_SETTER::prepare_pv_unit_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="PV UNIT")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not a PV unit when trying to set up a PV unit meter. "
          <<"No PV unit meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_pv_unit_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No PV unit meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}

METER METER_SETTER::prepare_pv_unit_terminal_current_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN PU");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_terminal_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_terminal_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_terminal_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_active_current_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CURRENT COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_reactive_current_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CURRENT COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_active_power_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_reactive_power_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_pv_unit_reactive_voltage_command_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE VOLTAGE COMMAND IN PU");

    return meter;
}

METER METER_SETTER::prepare_pv_panel_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("PV PANEL MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_pv_converter_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("PV CONVERTER MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_pv_electrical_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("PV ELECTRICAL MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}

METER METER_SETTER::prepare_pv_irradiance_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_pv_unit_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("PV IRRADIANCE MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}


bool METER_SETTER::prepare_energy_storage_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="ENERGY STORAGE")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not an energy storage when trying to set up an energy storage meter. "
          <<"No energy storage meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_energy_storage_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No energy storage meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}


METER METER_SETTER::prepare_energy_storage_state_of_energy_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("STATE OF ENERGY IN PU");

    return meter;
}


METER METER_SETTER::prepare_energy_storage_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_energy_storage_active_power_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER IN PU");

    return meter;
}

METER METER_SETTER::prepare_energy_storage_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_energy_storage_reactive_power_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER IN PU");

    return meter;
}


METER METER_SETTER::prepare_energy_storage_terminal_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_energy_storage_terminal_current_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("TERMINAL CURRENT IN PU");

    return meter;
}

METER METER_SETTER::prepare_energy_storage_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_energy_storage_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("ENERGY STORAGE MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}



bool METER_SETTER::prepare_hvdc_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="HVDC")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not an hvdc when trying to set up an hvdc meter. "
          <<"No hvdc meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_hvdc_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No hvdc meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}

METER METER_SETTER::prepare_hvdc_dc_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("DC CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_dc_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER DC CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_dc_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER DC CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_dc_voltage_in_kV_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER DC VOLTAGE IN KV");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_ac_voltage_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER AC VOLTAGE IN PU");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_alpha_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER ALPHA IN DEG");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_mu_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER MU IN DEG");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_dc_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER DC POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_ac_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER AC ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_ac_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER AC REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_hvdc_rectifier_ac_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("RECTIFIER AC CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_dc_voltage_in_kV_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER DC VOLTAGE IN KV");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_ac_voltage_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER AC VOLTAGE IN PU");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_gamma_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER GAMMA IN DEG");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_mu_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER MU IN DEG");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_dc_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER DC POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_ac_active_power_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER AC ACTIVE POWER IN MW");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_ac_reactive_power_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER AC REACTIVE POWER IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_hvdc_inverter_ac_current_in_kA_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("INVERTER AC CURRENT IN KA");

    return meter;
}

METER METER_SETTER::prepare_hvdc_model_internal_variable_meter(const DEVICE_ID& device_id, string name)
{
    METER meter;

    bool successful = prepare_hvdc_meter(meter, device_id);

    if(successful)
    {
        meter.set_meter_type("HVDC MODEL INTERNAL VARIABLE");
        meter.set_internal_variable_name(name);
    }

    return meter;
}


bool METER_SETTER::prepare_equivalent_device_meter(METER& meter, const DEVICE_ID& device_id)
{
    bool successful = false;

    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    string device_type = device_id.get_device_type();
    if(device_type!="EQUIVALENT DEVICE")
    {
        osstream<<"Warning. This device given ("<<device_type<<") is not an equivalent device when trying to set up an equivalent device meter. "
          <<"No equivalent device meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    if(not psdb.is_equivalent_device_exist(device_id))
    {
        osstream<<"Warning. "<<device_id.get_device_name()<<" does not exist in current power system database. "
          <<"No equivalent device meter will be added.";
        show_information_with_leading_time_stamp(osstream);
        return successful;
    }

    meter.set_device_id(device_id);

    successful = true;

    return successful;
}

METER METER_SETTER::prepare_equivalent_device_voltage_source_voltage_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("VOLTAGE SOURCE VOLTAGE IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_voltage_source_voltage_angle_in_deg_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("VOLTAGE SOURCE VOLTAGE ANGLE IN DEG");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_voltage_source_resistance_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("VOLTAGE SOURCE RESISTANCE IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_voltage_source_reactance_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("VOLTAGE SOURCE REACTANCE IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_active_constant_power_load_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CONSTANT POWER LOAD IN MW");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_reactive_constant_power_load_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CONSTANT POWER LOAD IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_active_constant_current_load_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CONSTANT CURRENT LOAD IN MW");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_reactive_constant_current_load_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CONSTANT CURRENT LOAD IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_active_constant_impedance_load_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CONSTANT IMPEDANCE LOAD IN MW");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CONSTANT IMPEDANCE LOAD IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_active_power_generation_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER GENERATION IN MW");

    return meter;
}


METER METER_SETTER::prepare_equivalent_device_reactive_power_generation_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER GENERATION IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_active_power_load_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER LOAD IN MW");

    return meter;
}


METER METER_SETTER::prepare_equivalent_device_reactive_power_load_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER LOAD IN MVAR");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_active_power_net_load_in_MW_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER NET LOAD IN MW");

    return meter;
}


METER METER_SETTER::prepare_equivalent_device_reactive_power_net_load_in_MVar_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER NET LOAD IN MVAR");

    return meter;
}



METER METER_SETTER::prepare_equivalent_device_nominal_active_constant_power_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CONSTANT POWER LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_reactive_constant_power_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CONSTANT POWER LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_active_constant_current_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CONSTANT CURRENT LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_reactive_constant_current_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CONSTANT CURRENT LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_active_constant_impedance_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE CONSTANT IMPEDANCE LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_nominal_reactive_constant_impedance_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE CONSTANT IMPEDANCE LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_active_power_generation_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER GENERATION IN PU");

    return meter;
}


METER METER_SETTER::prepare_equivalent_device_reactive_power_generation_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER GENERATION IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_active_power_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER LOAD IN PU");

    return meter;
}


METER METER_SETTER::prepare_equivalent_device_reactive_power_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER LOAD IN PU");

    return meter;
}

METER METER_SETTER::prepare_equivalent_device_active_power_net_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("ACTIVE POWER NET LOAD IN PU");

    return meter;
}


METER METER_SETTER::prepare_equivalent_device_reactive_power_net_load_in_pu_meter(const DEVICE_ID& device_id)
{
    METER meter;

    bool successful = prepare_equivalent_device_meter(meter, device_id);

    if(successful)
        meter.set_meter_type("REACTIVE POWER NET LOAD IN PU");

    return meter;
}



