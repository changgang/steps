#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/toolkit/cct_searcher/cct_searcher.h"
#include "header/steps_namespace.h"

size_t api_get_dynamic_simulator_integer_parameter(char* parameter_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX DAE ITERATION")
        return ds.get_max_DAE_iteration();
    if(PARAMETER_NAME=="MAX NETWORK ITERATION")
        return ds.get_max_network_iteration();

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n"
             "0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
    return 0;
}

void api_set_dynamic_simulator_integer_parameter(char* parameter_name, int value)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX_DAE_ITER" or PARAMETER_NAME=="MAX DAE ITERATION")
    {
        ds.set_max_DAE_iteration(value);
        return;
    }
    if(PARAMETER_NAME=="MAX_NET_ITER" or PARAMETER_NAME=="MAX NETWORK ITERATION")
    {
        ds.set_max_network_iteration(value);
        return;
    }

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

double api_get_dynamic_simulator_float_parameter(char* parameter_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX POWER IMBALANCE IN MVA")
        return ds.get_allowed_max_power_imbalance_in_MVA();
    if(PARAMETER_NAME=="ITERATION ACCELERATOR")
        return ds.get_iteration_accelerator();
    if(PARAMETER_NAME=="ANGLE STABILITY THRESHOLD IN DEG")
        return ds.get_rotor_angle_stability_threshold_in_deg();


    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n"
             "0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
    return 0.0;
}

void api_set_dynamic_simulator_float_parameter(char* parameter_name, double value)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX POWER IMBALANCE IN MVA")
    {
        ds.set_allowed_max_power_imbalance_in_MVA(value);
        return;
    }
    if(PARAMETER_NAME=="ITERATION ACCELERATOR")
    {
        ds.set_iteration_accelerator(value);
        return;
    }
    if(PARAMETER_NAME=="ANGLE STABILITY THRESHOLD IN DEG")
    {
        ds.set_rotor_angle_stability_threshold_in_deg(value);
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

const char* api_get_dynamic_simulator_string_parameter(char* parameter_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

	snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", "");
    string PARAMETER_NAME = string2upper(parameter_name);
    if (PARAMETER_NAME == "OUTPUT FILENAME")
    {
        snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", (ds.get_output_file()).c_str());
        return STEPS::steps_char_buffer;
    }

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n"
             "0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
    return STEPS::steps_char_buffer;
}

void api_set_dynamic_simulator_string_parameter(char* parameter_name, char* value)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="OUTPUT FILENAME")
    {
        ds.set_output_file(value);
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

bool api_get_dynamic_simulator_boolean_parameter(char* parameter_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="ANGLE STABILITY SURVILLIANCE LOGIC")
        return ds.get_rotor_angle_stability_survilliance_flag();
    if(PARAMETER_NAME=="BIN EXPORT LOGIC")
        return ds.is_bin_file_export_enabled();
    if(PARAMETER_NAME=="CSV EXPORT LOGIC")
        return ds.is_csv_file_export_enabled();
    if(PARAMETER_NAME=="JSON EXPORT LOGIC")
        return ds.is_json_file_export_enabled();

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n"
             "0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
    return false;
}
void api_set_dynamic_simulator_boolean_parameter(char* parameter_name, bool value)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="ANGLE STABILITY SURVILLIANCE LOGIC")
    {
        ds.set_rotor_angle_stability_survilliance_flag(value);
        return;
    }
    if(PARAMETER_NAME=="BIN EXPORT LOGIC")
    {
        ds.set_bin_file_export_enable_flag(value);
        return;
    }
    if(PARAMETER_NAME=="CSV EXPORT LOGIC")
    {
        ds.set_csv_file_export_enable_flag(value);
        return;
    }
    if(PARAMETER_NAME=="JSON EXPORT LOGIC")
    {
        ds.set_json_file_export_enable_flag(value);
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for dynamic simulator with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

void api_set_dynamic_simulator_output_file(char* file)
{
    string parameter_name = "OUTPUT FILENAME";
    api_set_dynamic_simulator_string_parameter((char*)parameter_name.c_str(), file);
}

const char* api_get_dynamic_simulator_output_file()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

	snprintf(STEPS::steps_char_buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s", ds.get_output_file().c_str());
	return STEPS::steps_char_buffer;
}

void api_set_dynamic_simulation_time_step(double value)
{
    set_dynamic_simulation_time_step_in_s(value);
}

double api_get_dynamic_simulation_time_step()
{
    return get_dynamic_simulation_time_step_in_s();
}

double api_get_dynamic_simulation_time()
{
    return get_dynamic_simulation_time_in_s();
}

void api_prepare_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_meters();
}

void api_prepare_bus_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_bus_related_meters();
}

void api_prepare_generator_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_generator_related_meters();
}


void api_prepare_wt_generator_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_wt_generator_related_meters();
}

void api_prepare_pv_unit_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_pv_unit_related_meters();
}

void api_prepare_energy_storage_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_energy_storage_related_meters();
}

void api_prepare_load_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_load_related_meters();
}

void api_prepare_line_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_line_related_meters();
}

void api_prepare_hvdc_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_hvdc_related_meters();
}

void api_prepare_equivalent_device_related_meters()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_equivalent_device_related_meters();
}


void api_prepare_bus_related_meter(size_t bus, char* meter_type)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.prepare_bus_related_meter(bus, meter_type);
}

void api_prepare_generator_related_meter(size_t bus, char* id, char* meter_type, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_generator_device_id(bus, id);
    ds.prepare_generator_related_meter(did, meter_type, var_name);
}


void api_prepare_wt_generator_related_meter(size_t bus, char* id, char* meter_type, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_wt_generator_device_id(bus, id);
    ds.prepare_wt_generator_related_meter(did, meter_type, var_name);
}

void api_prepare_pv_unit_related_meter(size_t bus, char* id, char* meter_type, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_pv_unit_device_id(bus, id);
    ds.prepare_pv_unit_related_meter(did, meter_type, var_name);
}

void api_prepare_energy_storage_related_meter(size_t bus, char* id, char* meter_type, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_energy_storage_device_id(bus, id);
    ds.prepare_energy_storage_related_meter(did, meter_type, var_name);
}

void api_prepare_load_related_meter(size_t bus, char* id, char* meter_type, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_load_device_id(bus, id);
    ds.prepare_load_related_meter(did, meter_type, var_name);
}

void api_prepare_line_related_meter(size_t ibus, size_t jbus, char* id, char* meter_type, char* side, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_line_device_id(ibus, jbus, id);
    ds.prepare_line_related_meter(did, meter_type, side, var_name);
}

void api_prepare_hvdc_related_meter(size_t ibus, size_t jbus, char* id, char* meter_type, char* side, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, id);
    ds.prepare_hvdc_related_meter(did, meter_type, side, var_name);
}

void api_prepare_equivalent_device_related_meter(size_t bus, char* id, char* meter_type, char* var_name)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    DEVICE_ID did = get_equivalent_device_id(bus, id);
    ds.prepare_equivalent_device_related_meter(did, meter_type, var_name);
}

void api_start_dynamic_simulation()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.start();
}

void api_stop_dynamic_simulation()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.stop();
}

void api_run_simulation_to_time(double t_end)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.run_to(t_end);
}

void api_run_a_step()
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.run_a_step();
}

void api_set_bus_fault(size_t bus, char* fault_type, double fault_G, double fault_B)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    string string_fault_type = string2upper(fault_type);
    if(string_fault_type=="THREE PHASE FAULT")
    {
        ds.set_bus_fault(bus, complex<double>(fault_G, fault_B));
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Fault type '%s' is not supported for dynamic simulator with api %s.",
             string_fault_type.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

void api_clear_bus_fault(size_t bus, char* fault_type)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    string string_fault_type = string2upper(fault_type);
    if(string_fault_type=="THREE PHASE FAULT")
    {
        ds.clear_bus_fault(bus);
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Fault type '%s' is not supported for dynamic simulator with api %s.",
             string_fault_type.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

void api_trip_bus(size_t bus)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    ds.trip_bus(bus);
}


void api_set_line_fault(size_t ibus, size_t jbus, char* identifier, char* fault_type, double fault_location, double fault_G, double fault_B)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_line_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    string string_fault_type = string2upper(fault_type);
    if(string_fault_type=="THREE PHASE FAULT")
    {
        ds.set_line_fault(did, ibus, fault_location, complex<double>(fault_G, fault_B));
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Fault type '%s' is not supported for dynamic simulator with api %s.",
             string_fault_type.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}

void api_clear_line_fault(size_t ibus, size_t jbus, char* identifier, char* fault_type, double fault_location)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_line_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    string string_fault_type = string2upper(fault_type);
    if(string_fault_type=="THREE PHASE FAULT")
    {
        ds.clear_line_fault(did, ibus, fault_location);
        return;
    }
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Fault type '%s' is not supported for dynamic simulator with api %s.",
             string_fault_type.c_str(), __FUNCTION__);
    show_information_with_leading_time_stamp(buffer);
}


void api_trip_line(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_line_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_line(did);
}

void api_trip_line_breaker(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_line_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_line_breaker(did, ibus);
}

void api_close_line(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_line_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.close_line(did);
}

void api_close_line_breaker(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_line_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.close_line_breaker(did, ibus);
}

void api_trip_transformer(size_t ibus, size_t jbus, size_t kbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    if(not psdb.is_transformer_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_transformer(did);
}

void api_trip_transformer_breaker(size_t ibus, size_t jbus, size_t kbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    if(not psdb.is_transformer_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_transformer_breaker(did, ibus);
}
void api_close_transformer(size_t ibus, size_t jbus, size_t kbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    if(not psdb.is_transformer_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.close_transformer(did);
}

void api_close_transformer_breaker(size_t ibus, size_t jbus, size_t kbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_transformer_device_id(ibus, jbus, kbus, identifier);

    if(not psdb.is_transformer_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.close_transformer_breaker(did, ibus);
}

void api_trip_generator(size_t bus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus, identifier);

    if(not psdb.is_generator_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_generator(did);
}

void api_shed_generator(size_t bus, char* identifier, double percent)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus, identifier);

    if(not psdb.is_generator_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.shed_generator(did, percent);
}


void api_trip_load(size_t bus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_load_device_id(bus, identifier);

    if(not psdb.is_load_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_load(did);
}

void api_close_load(size_t bus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_load_device_id(bus, identifier);

    if(not psdb.is_load_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.close_load(did);
}

void api_scale_load(size_t bus, char* identifier, double percent)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_load_device_id(bus, identifier);

    if(not psdb.is_load_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.scale_load(did, percent);
}

void api_scale_all_loads(double percent)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();

    ds.scale_all_load(percent);
}


void api_trip_fixed_shunt(size_t bus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    if(not psdb.is_fixed_shunt_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.trip_fixed_shunt(did);
}

void api_close_fixed_shunt(size_t bus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_fixed_shunt_device_id(bus, identifier);

    if(not psdb.is_load_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.close_fixed_shunt(did);
}

void api_manually_bypass_hvdc(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    if(not psdb.is_hvdc_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.manual_bypass_hvdc(did);
}

void api_manually_unbypass_hvdc(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    if(not psdb.is_hvdc_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.manual_unbypass_hvdc(did);
}

void api_manually_block_hvdc(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    if(not psdb.is_hvdc_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    ds.manual_block_hvdc(did);
}

void api_manually_unblock_hvdc(size_t ibus, size_t jbus, char* identifier)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_hvdc_device_id(ibus, jbus, identifier);

    if(not psdb.is_hvdc_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }
    ds.manual_unblock_hvdc(did);
}


double api_get_generator_voltage_reference_in_pu(size_t bus, char* identifier)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus, identifier);

    GENERATOR* gen = psdb.get_generator(did);

    if(gen==NULL)
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return 0.0;
    }

    EXCITER_MODEL* avr = gen->get_exciter_model();
    if(avr!=NULL)
        return avr->get_voltage_reference_in_pu();
    else
        return 0.0;
}

double api_get_generator_power_reference_in_MW(size_t bus, char* identifier)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus, identifier);

    GENERATOR* gen = psdb.get_generator(did);

    if(gen==NULL)
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return 0.0;
    }

    TURBINE_GOVERNOR_MODEL* tg = gen->get_turbine_governor_model();
    if(tg!=NULL)
        return tg->get_mechanical_power_reference_in_pu_based_on_mbase()*gen->get_mbase_in_MVA();
    else
        return 0.0;
}

void api_set_generator_voltage_reference_in_pu(size_t bus, char* identifier, double value)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus, identifier);

    if(not psdb.is_generator_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.change_generator_voltage_reference_in_pu(did, value);
}

void api_set_generator_power_reference_in_MW(size_t bus, char* identifier, double value)
{
    DYNAMICS_SIMULATOR& ds = get_default_dynamic_simulator();
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    DEVICE_ID did = get_generator_device_id(bus, identifier);

    if(not psdb.is_generator_exist(did))
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for dynamic simulator with api %s.",
                 (did.get_device_name()).c_str(), __FUNCTION__);
        show_information_with_leading_time_stamp(buffer);
        return;
    }

    ds.change_generator_power_reference_in_MW(did, value);
}

double api_search_cct(char* pf_file, char* dy_file, size_t ibus, size_t jbus, char* id, size_t sidebus, size_t trip_line)
{
    ostringstream osstream;

    set_dynamic_simulation_time_step_in_s(0.01);

    CCT_SEARCHER searcher;
    searcher.set_power_system_database_maximum_bus_number(10000);
    searcher.set_search_title("");
    searcher.set_powerflow_data_filename(pf_file);
    searcher.set_dynamic_data_filename(dy_file);

    DEVICE_ID did;
    TERMINAL terminal;
    terminal.append_bus(ibus);
    terminal.append_bus(jbus);
    did.set_device_type("LINE");
    did.set_device_terminal(terminal);
    did.set_device_identifier(id);

    searcher.set_fault_device(did);
    searcher.set_fault_side_bus(sidebus);
    searcher.set_fault_location_to_fault_side_bus_in_pu(0.0);
    searcher.set_fault_shunt_in_pu(complex<double>(0.0, -2e8));
    if(trip_line==0)
        searcher.set_flag_trip_line_after_clearing_fault(false);
    else
        searcher.set_flag_trip_line_after_clearing_fault(true);


    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Now go searching CCT for fault at side %lu of %s.",
             searcher.get_fault_side_bus(), (did.get_device_name()).c_str());
    show_information_with_leading_time_stamp(buffer);
    double cct = searcher.search_cct();
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Now done searching CCT for fault at side %lu of %s.",
             searcher.get_fault_side_bus(), (did.get_device_name()).c_str());
    show_information_with_leading_time_stamp(buffer);

    searcher.run_case_with_clearing_time(cct);
    searcher.run_case_with_clearing_time(cct+0.1);
    return cct;
}
