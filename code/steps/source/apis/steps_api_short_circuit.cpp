#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <iostream>
using namespace std;

void api_build_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_sequence_network_Y_matrix();
}

void api_build_positive_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_positive_sequence_network_Y_matrix();
}

void api_build_negative_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_negative_sequence_network_Y_matrix();
}

void api_build_zero_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_zero_sequence_network_Y_matrix();
}

void api_build_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_sequence_network_Z_matrix();
}

void api_build_positive_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_positive_sequence_network_Z_matrix();
}

void api_build_negative_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_negative_sequence_network_Z_matrix();
}

void api_build_zero_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_zero_sequence_network_Z_matrix();
}

void api_save_positive_sequence_network_Y_matrix(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.save_positive_sequence_Y_matrix_to_file(file);
}

void api_save_negative_sequence_network_Y_matrix(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.save_negative_sequence_Y_matrix_to_file(file);
}

void api_save_zero_sequence_network_Y_matrix(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.save_zero_sequence_Y_matrix_to_file(file);
}

void api_save_positive_sequence_network_Z_matrix(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.save_positive_sequence_Z_matrix_to_file(file);
}

void api_save_negative_sequence_network_Z_matrix(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.save_negative_sequence_Z_matrix_to_file(file);
}

void api_save_zero_sequence_network_Z_matrix(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.save_zero_sequence_Z_matrix_to_file(file);
}

void api_set_short_circuit_solver_integer_parameter(char* parameter_name, int value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="GENERATOR REACTANCE OPTION")
    {
        if(value==0)
        {
            solver.set_generator_reactance_option(SUBTRANSIENT_REACTANCE);
            return;
        }
        if(value==1)
        {
            solver.set_generator_reactance_option(TRANSIENT_REACTANCE);
            return;
        }
        if(value==2)
        {
            solver.set_generator_reactance_option(SYNCHRONOUS_REACTANCE);
            return;
        }
    }
    else if(PARAMETER_NAME=="UNIT OPTION")
    {
        if(value==0)
        {
            solver.set_units_of_currents_and_voltages(PU);
            return;
        }
        if(value==1)
        {
            solver.set_units_of_currents_and_voltages(PHYSICAL);
            return;
        }
    }
    else if(PARAMETER_NAME=="COORDINATE OPTION")
    {
        if(value==0)
        {
            solver.set_coordinates_of_currents_and_voltages(RECTANGULAR);
            return;
        }
        if(value==1)
        {
            solver.set_coordinates_of_currents_and_voltages(POLAR);
            return;
        }
    }
    else if(PARAMETER_NAME=="DC LINE OPTION")
    {
        if(value==0)
        {
            solver.set_option_of_DC_lines(BLOCK_AND_IGNORE);
            return;
        }
        if(value==1)
        {
            solver.set_option_of_DC_lines(CONVERT_TO_CONSTANT_ADMITTANCE_LOAD);
            return;
        }
    }
    else if(PARAMETER_NAME=="MAX_ITER_COUNT" or PARAMETER_NAME=="MAX ITER COUNT")
    {
        if(value>0)
        {
            solver.set_max_iteration_cout_for_iterative_method(value);
            return;
        }
    }
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return;
}

unsigned int api_get_short_circuit_solver_integer_parameter(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="GENERATOR REACTANCE OPTION")
        return solver.get_generator_reactance_option();

    if(PARAMETER_NAME=="UNIT OPTION")
        return solver.get_units_of_currents_and_voltages();

    if(PARAMETER_NAME=="COORDINATE OPTION")
        return solver.get_coordinates_of_currents_and_voltages();

    if(PARAMETER_NAME=="FAULT TYPE")
        return solver.get_fault_type();

    if(PARAMETER_NAME=="DC LINE OPTION")
        return solver.get_option_of_DC_lines();

    if(PARAMETER_NAME=="MAX_ITER_COUNT" or PARAMETER_NAME=="MAX ITER COUNT")
        return solver.get_max_iteration_count_for_iterative_method();

    if(PARAMETER_NAME=="ITERATION_COUNT" or PARAMETER_NAME=="ITER COUNT")
        return solver.get_iteration_count();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n"
             "0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return 0;
}

void api_set_short_circuit_solver_float_parameter(char* parameter_name, double value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);

    if(PARAMETER_NAME=="CURRENT THRESHOLD")
    {
        if(value>0.0)
        {
            solver.set_current_threshold_for_iterative_method(value);
            return;
        }
    }
    if(PARAMETER_NAME=="ITERATION ACCELERATION FACTOR" or PARAMETER_NAME=="ACCL FACTOR")
    {
        if(value>0.0)
        {
            solver.set_iteration_acceleration_factor(value);
            return;
        }
    }
    if(PARAMETER_NAME=="HVDC BLOCK VOLTAGE THRESHOLD" or PARAMETER_NAME=="HVDC BLOCK VTH")
    {
        if(value>0.0)
        {
            solver.set_voltage_threshold_for_hvdc_blocking_in_pu(value);
            return;
        }
    }

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return;
}

double api_get_short_circuit_solver_float_parameter(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);

    if(PARAMETER_NAME=="CURRENT THRESHOLD")
        return solver.get_current_threshold_for_iterative_method();
    if(PARAMETER_NAME=="ITERATION ACCELERATION FACTOR" or PARAMETER_NAME=="ACCL FACTOR")
        return solver.get_iteration_acceleration_factor();
    if(PARAMETER_NAME=="HVDC BLOCK VOLTAGE THRESHOLD" or PARAMETER_NAME=="HVDC BLOCK VTH")
        return solver.get_voltage_threshold_for_hvdc_blocking_in_pu();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n"
             "0.0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return 0.0;
}

void api_set_short_circuit_solver_boolean_parameter(char* parameter_name, bool value, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);

    if(PARAMETER_NAME=="CONSIDER LOAD LOGIC")
    {
        solver.set_consider_load_logic(value);
        return;
    }
    if(PARAMETER_NAME=="CONSIDER MOTOR LOAD LOGIC")
    {
        solver.set_consider_motor_load_logic(value);
        return;
    }
    if(PARAMETER_NAME=="EXPORT DETAILED ITERATION LOG LOGIC")
    {
        solver.set_export_detailed_iteration_log_logic(value);
        return;
    }
    if(PARAMETER_NAME=="CONSIDER NEGATIVE SEQUENCE SUPPORT OF RES LOGIC")
    {
        solver.set_consider_negative_sequence_support_of_RES_logic(value);
        return;
    }

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n",
             PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return;
}
bool api_get_short_circuit_solver_boolean_parameter(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="CONSIDER LOAD LOGIC")
        return solver.get_consider_load_logic();
    if(PARAMETER_NAME=="CONSIDER MOTOR LOAD LOGIC")
        return solver.get_consider_motor_load_logic();
    if(PARAMETER_NAME=="CONVERGENCE FLAG")
        return solver.get_convergence_flag();
    if(PARAMETER_NAME=="EXPORT DETAILED ITERATION LOG LOGIC")
        return solver.get_export_detailed_iteration_log_logic();
    if(PARAMETER_NAME=="CONSIDER NEGATIVE SEQUENCE SUPPORT OF RES LOGIC")
        return solver.get_consider_negative_sequence_support_of_RES_logic();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n"
             "0.0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return false;
}

void api_set_short_circuit_bus_fault(unsigned int bus, char* fault_type, double fault_G, double fault_B, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& sc_solver = toolkit.get_short_circuit_solver();
    string string_fault_type = string2upper(fault_type);
    if(string_fault_type=="THREE PHASES FAULT")
    {
        sc_solver.set_bus_fault(bus, THREE_PHASES_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    if(string_fault_type=="SINGLE PHASE GROUNDED FAULT")
    {
        sc_solver.set_bus_fault(bus, SINGLE_PHASE_GROUNDED_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    if(string_fault_type=="DOUBLE PHASES FAULT")
    {
        sc_solver.set_bus_fault(bus, DOUBLE_PHASES_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    if(string_fault_type=="DOUBLE PHASES GROUNDED FAULT")
    {
        sc_solver.set_bus_fault(bus, DOUBLE_PHASES_GROUNDED_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Fault type '%s' is not supported for short circuit solver with api %s.",
             string_fault_type.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void api_set_short_circuit_line_fault(unsigned int ibus, unsigned int jbus, char* identifier, char* fault_type, double fault_location, double fault_G, double fault_B, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& sc_solver = toolkit.get_short_circuit_solver();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_ac_line_device_id(ibus, jbus, identifier);

    if(not psdb.is_ac_line_exist(did))
    {
        char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%s does not exist in database for short circuit with api %s.",
                 (did.get_compound_device_name()).c_str(), __FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(buffer);
        return;
    }

    string string_fault_type = string2upper(fault_type);
    if(string_fault_type=="THREE PHASES FAULT")
    {
        sc_solver.set_ac_line_fault(did, ibus, fault_location, THREE_PHASES_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    if(string_fault_type=="SINGLE PHASE GROUNDED FAULT")
    {
        sc_solver.set_ac_line_fault(did, ibus, fault_location, SINGLE_PHASE_GROUNDED_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    if(string_fault_type=="DOUBLE PHASES FAULT")
    {
        sc_solver.set_ac_line_fault(did, ibus, fault_location, DOUBLE_PHASES_FAULT, complex<double>(fault_G, fault_B));
        return;
    }
    if(string_fault_type=="DOUBLE PHASES GROUNDED FAULT")
    {
        sc_solver.set_ac_line_fault(did, ibus, fault_location, DOUBLE_PHASES_GROUNDED_FAULT, complex<double>(fault_G, fault_B));
        return;
    }

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Fault type '%s' is not supported for short circuit solver with api %s.",
             string_fault_type.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
}

void api_clear_short_circuit_fault(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.clear_fault();
}

void api_solve_short_circuit(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.solve();
}

void api_scan_short_circuit_current_of_all_buses(char* fault_type,  char* result_file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    string fault_type_str = string2upper(fault_type);
    FAULT_TYPE ftype = THREE_PHASES_FAULT;
    if(fault_type_str=="THREE PHASES FAULT") ftype = THREE_PHASES_FAULT;
    if(fault_type_str=="SINGLE PHASE GROUNDED FAULT") ftype = SINGLE_PHASE_GROUNDED_FAULT;
    if(fault_type_str=="DOUBLE PHASES FAULT") ftype = DOUBLE_PHASES_FAULT;
    if(fault_type_str=="DOUBLE PHASES GROUNDED FAULT") ftype = DOUBLE_PHASES_GROUNDED_FAULT;

    string result_file_str = result_file;
    solver.scan_short_circuit_current_of_all_buses(ftype, result_file_str);
}

double api_get_short_circuit_value_at_fault_point(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);

    if(PARAMETER_NAME=="IF1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN PU")
        return abs(solver.get_positive_sequence_fault_current_of_fault_point_in_pu());
    if(PARAMETER_NAME=="IF2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN PU")
        return abs(solver.get_negative_sequence_fault_current_of_fault_point_in_pu());
    if(PARAMETER_NAME=="IF0_PU" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT IN PU")
        return abs(solver.get_zero_sequence_fault_current_of_fault_point_in_pu());

    if(PARAMETER_NAME=="IF1_KA" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN KA")
        return abs(solver.get_positive_sequence_fault_current_of_fault_point_in_kA());
    if(PARAMETER_NAME=="IF2_KA" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN KA")
        return abs(solver.get_negative_sequence_fault_current_of_fault_point_in_kA());
    if(PARAMETER_NAME=="IF0_KA" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT IN KA")
        return abs(solver.get_zero_sequence_fault_current_of_fault_point_in_kA());

    if(PARAMETER_NAME=="IF1ANG_DEG" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT ANGLE IN DEG")
        return rad2deg(arg(solver.get_positive_sequence_fault_current_of_fault_point_in_pu()));
    if(PARAMETER_NAME=="IF2ANG_DEG" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT ANGLE IN DEG")
        return rad2deg(arg(solver.get_negative_sequence_fault_current_of_fault_point_in_pu()));
    if(PARAMETER_NAME=="IF0ANG_DEG" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT ANGLE IN DEG")
        return rad2deg(arg(solver.get_zero_sequence_fault_current_of_fault_point_in_pu()));


    if(PARAMETER_NAME=="VF1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE VOLTAGE IN PU")
        return abs(solver.get_positive_sequence_voltage_of_fault_point_in_pu());
    if(PARAMETER_NAME=="VF2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE VOLTAGE IN PU")
        return abs(solver.get_negative_sequence_voltage_of_fault_point_in_pu());
    if(PARAMETER_NAME=="VF0_PU" or PARAMETER_NAME=="ZERO SEQUENCE VOLTAGE IN PU")
        return abs(solver.get_zero_sequence_voltage_of_fault_point_in_pu());

    if(PARAMETER_NAME=="VF1_KV" or PARAMETER_NAME=="POSITIVE SEQUENCE VOLTAGE IN KV")
        return abs(solver.get_positive_sequence_voltage_of_fault_point_in_kV());
    if(PARAMETER_NAME=="VF2_KV" or PARAMETER_NAME=="NEGATIVE SEQUENCE VOLTAGE IN KV")
        return abs(solver.get_negative_sequence_voltage_of_fault_point_in_kV());
    if(PARAMETER_NAME=="VF0_KV" or PARAMETER_NAME=="ZERO SEQUENCE VOLTAGE IN KV")
        return abs(solver.get_zero_sequence_voltage_of_fault_point_in_kV());

    if(PARAMETER_NAME=="VF1ANG_DEG" or PARAMETER_NAME=="POSITIVE SEQUENCE VOLTAGE ANGLE IN DEG")
        return rad2deg(arg(solver.get_positive_sequence_voltage_of_fault_point_in_pu()));
    if(PARAMETER_NAME=="VF2ANG_DEG" or PARAMETER_NAME=="NEGATIVE SEQUENCE VOLTAGE ANGLE IN DEG")
        return rad2deg(arg(solver.get_negative_sequence_voltage_of_fault_point_in_pu()));
    if(PARAMETER_NAME=="VF0ANG_DEG" or PARAMETER_NAME=="ZERO SEQUENCE VOLTAGE ANGLE IN DEG")
        return rad2deg(arg(solver.get_zero_sequence_voltage_of_fault_point_in_pu()));

    if(PARAMETER_NAME=="S1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE SHORT CIRCUIT CAPACITY IN PU")
        return solver.get_positive_sequence_short_circuit_capacity_of_fault_point_in_pu();
    if(PARAMETER_NAME=="S2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE SHORT CIRCUIT CAPACITY IN PU")
        return solver.get_negative_sequence_short_circuit_capacity_of_fault_point_in_pu();
    if(PARAMETER_NAME=="S0_PU" or PARAMETER_NAME=="ZERO SEQUENCE SHORT CIRCUIT CAPACITY IN PU")
        return solver.get_zero_sequence_short_circuit_capacity_of_fault_point_in_pu();

    if(PARAMETER_NAME=="S1_MVA" or PARAMETER_NAME=="POSITIVE SEQUENCE SHORT CIRCUIT CAPACITY IN MVA")
        return solver.get_positive_sequence_short_circuit_capacity_of_fault_point_in_MVA();
    if(PARAMETER_NAME=="S2_MVA" or PARAMETER_NAME=="NEGATIVE SEQUENCE SHORT CIRCUIT CAPACITY IN MVA")
        return solver.get_negative_sequence_short_circuit_capacity_of_fault_point_in_MVA();
    if(PARAMETER_NAME=="S0_MVA" or PARAMETER_NAME=="ZERO SEQUENCE SHORT CIRCUIT CAPACITY IN MVA")
        return solver.get_zero_sequence_short_circuit_capacity_of_fault_point_in_MVA();


    if(PARAMETER_NAME=="R1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE EQUIVALENT R IN PU")
        return solver.get_positive_sequence_thevenin_impedance_at_fault_point_in_pu().real();
    if(PARAMETER_NAME=="X1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE EQUIVALENT X IN PU")
        return solver.get_positive_sequence_thevenin_impedance_at_fault_point_in_pu().imag();
    if(PARAMETER_NAME=="R2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE EQUIVALENT R IN PU")
        return solver.get_negative_sequence_thevenin_impedance_at_fault_point_in_pu().real();
    if(PARAMETER_NAME=="X2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE EQUIVALENT X IN PU")
        return solver.get_negative_sequence_thevenin_impedance_at_fault_point_in_pu().imag();
    if(PARAMETER_NAME=="R0_PU" or PARAMETER_NAME=="ZERO SEQUENCE EQUIVALENT R IN PU")
        return solver.get_zero_sequence_thevenin_impedance_at_fault_point_in_pu().real();
    if(PARAMETER_NAME=="X0_PU" or PARAMETER_NAME=="ZERO SEQUENCE EQUIVALENT X IN PU")
        return solver.get_zero_sequence_thevenin_impedance_at_fault_point_in_pu().imag();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit result with api %s.\n"
             "0.0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return 0.0;
}


void api_show_short_circuit_result_at_fault_point(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.show_short_circuit_result_at_fault_point();
}

void api_save_short_circuit_result_at_fault_point_to_file(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.save_short_circuit_result_at_fault_point_to_file(file);
}

void api_save_extended_short_circuit_result_to_file(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.save_extended_short_circuit_result_to_file(file);
}

double api_get_short_circuit_capacity_at_bus_in_MVA(unsigned int bus, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    return solver.get_positive_sequence_short_circuit_capacity_of_bus_in_MVA(bus);
}

double api_get_short_circuit_ratio_of_LCC_HVDC2T(unsigned int ibus, unsigned int jbus, char* identifier, char* side, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    DEVICE_ID did = get_2t_lcc_hvdc_device_id(ibus, jbus, identifier);
    LCC_HVDC2T* hvdcptr = psdb.get_2t_lcc_hvdc(did);

    string SIDE = string2upper(side);
    CONVERTER_SIDE con_side = (SIDE=="RECTIFIER"?RECTIFIER:INVERTER);

    return  solver.get_short_circuit_ratio_of_LCC_HVDC2T(*hvdcptr, con_side);
}

double api_get_short_circuit_ratio_of_wt_generator(unsigned int wtg_bus, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_wt_generator_device_id(wtg_bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    WT_GENERATOR* wt_generator = psdb.get_wt_generator(did);

    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    return solver.get_short_circuit_ratio_of_wt_generator(*wt_generator);
}

double api_get_short_circuit_ratio_of_pv_unit(unsigned int pvu_bus, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_pv_unit_device_id(pvu_bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    PV_UNIT* pv_unit = psdb.get_pv_unit(did);

    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    return solver.get_short_circuit_ratio_of_pv_unit(*pv_unit);
}

double api_get_multiple_infeed_short_circuit_ratio_of_LCC_HVDC2T(unsigned int ibus, unsigned int jbus, char* identifier, char* side, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    DEVICE_ID did = get_2t_lcc_hvdc_device_id(ibus, jbus, identifier);
    LCC_HVDC2T* hvdcptr = psdb.get_2t_lcc_hvdc(did);

    string SIDE = string2upper(side);
    CONVERTER_SIDE con_side = (SIDE=="RECTIFIER"?RECTIFIER:INVERTER);

    return  solver.get_multiple_infeed_short_circuit_ratio_of_LCC_HVDC2T(*hvdcptr, con_side);
}

double api_get_multiple_renewable_energy_station_short_circuit_ratio_of_wt_generator(unsigned int wtg_bus, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_wt_generator_device_id(wtg_bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    WT_GENERATOR* wt_generator = psdb.get_wt_generator(did);

    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    return solver.get_multiple_renewable_energy_station_short_circuit_ratio_of_wt_generator(*wt_generator);
}

double api_get_multiple_renewable_energy_station_short_circuit_ratio_of_pv_unit(unsigned int pvu_bus, char* identifier, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    DEVICE_ID did = get_pv_unit_device_id(pvu_bus, identifier);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    PV_UNIT* pv_unit = psdb.get_pv_unit(did);

    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    return solver.get_multiple_renewable_energy_station_short_circuit_ratio_of_pv_unit(*pv_unit);
}
