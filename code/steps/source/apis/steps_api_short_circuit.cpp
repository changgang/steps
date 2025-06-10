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
    if(PARAMETER_NAME=="GENERATOR REACTANCE")
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
    else if(PARAMETER_NAME=="UNITS")
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
    else if(PARAMETER_NAME=="COORDINATES")
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
    else if(PARAMETER_NAME=="OPTION OF DC LINES")
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
    if(PARAMETER_NAME=="GENERATOR REACTANCE")
        return solver.get_generator_reactance_option();

    if(PARAMETER_NAME=="UNITS")
        return solver.get_units_of_currents_and_voltages();

    if(PARAMETER_NAME=="COORDINATES")
        return solver.get_coordinates_of_currents_and_voltages();

    if(PARAMETER_NAME=="FAULT TYPE")
        return solver.get_fault_type();

    if(PARAMETER_NAME=="OPTION OF DC LINES")
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
    if(PARAMETER_NAME=="ITERATION FACTOR" or PARAMETER_NAME=="ITER FACTOR")
    {
        if(value>0.0)
        {
            solver.set_iteration_factor(value);
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
    if(PARAMETER_NAME=="ITERATION FACTOR" or PARAMETER_NAME=="ITER FACTOR")
        return solver.get_iteration_factor();

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
    if(PARAMETER_NAME=="EXPORT DETAILED ITERATION LOGIC")
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
    if(PARAMETER_NAME=="EXPORT DETAILED ITERATION LOGIC")
        return solver.get_export_detailed_iteration_log_logic();
    if(PARAMETER_NAME=="CONSIDER NEGATIVE SEQUENCE SUPPORT OF RES LOGIC")
        return solver.get_consider_negative_sequence_support_of_RES_logic();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit solver with api %s.\n"
             "0.0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return false;
}

void api_short_circuit_set_bus_fault(unsigned int bus, char* fault_type, double fault_G, double fault_B, unsigned int toolkit_index)
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

void api_short_circuit_set_line_fault(unsigned int ibus, unsigned int jbus, char* identifier, char* fault_type, double fault_location, double fault_G, double fault_B, unsigned int toolkit_index)
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

void api_short_circuit_clear_fault(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.clear_fault();
}

void api_short_circuit_build_positive_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_positive_sequence_network_Y_matrix();
}

void api_short_circuit_build_negative_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_negative_sequence_network_Y_matrix();
}

void api_short_circuit_build_zero_sequence_network_Y_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_zero_sequence_network_Y_matrix();
}

void api_short_circuit_build_positive_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_positive_sequence_network_Z_matrix();
}
void api_short_circuit_build_negative_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_negative_sequence_network_Z_matrix();
}
void api_short_circuit_build_zero_sequence_network_Z_matrix(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    NETWORK_MATRIX& network_matrix = solver.get_network_matrix();
    network_matrix.build_zero_sequence_network_Z_matrix();
}

void api_short_circuit_update_internal_voltage(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.update_internal_voltage_of_all_generators_and_wt_generators();
}

void api_solve_short_circuit(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.solve();
}

void api_solve_short_circuit_with_iteration(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.solve_with_iteration();
}

void api_short_circuit_current_scanning(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.short_circuit_current_scanning();
}

double api_get_short_circuit_result_float_data(char* parameter_name, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();

    string PARAMETER_NAME = string2upper(parameter_name);

    if(PARAMETER_NAME=="IF1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN PU")
        return abs(solver.get_positive_sequence_fault_current_in_pu());
    if(PARAMETER_NAME=="IF2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN PU")
        return abs(solver.get_negative_sequence_fault_current_in_pu());
    if(PARAMETER_NAME=="IF0_PU" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT IN PU")
        return abs(solver.get_zero_sequence_fault_current_in_pu());

    if(PARAMETER_NAME=="IF1_KA" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN KA")
        return abs(solver.get_positive_sequence_fault_current_in_kA());
    if(PARAMETER_NAME=="IF2_KA" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN KA")
        return abs(solver.get_negative_sequence_fault_current_in_kA());
    if(PARAMETER_NAME=="IF0_KA" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT IN KA")
        return abs(solver.get_zero_sequence_fault_current_in_kA());

    if(PARAMETER_NAME=="IF1_DEG" or PARAMETER_NAME=="POSITIVE SEQUENCE CURRENT IN DEG")
        return rad2deg(arg(solver.get_positive_sequence_fault_current_in_pu()));
    if(PARAMETER_NAME=="IF2_DEG" or PARAMETER_NAME=="NEGATIVE SEQUENCE CURRENT IN DEG")
        return rad2deg(arg(solver.get_negative_sequence_fault_current_in_pu()));
    if(PARAMETER_NAME=="IF0_DEG" or PARAMETER_NAME=="ZERO SEQUENCE CURRENT IN DEG")
        return rad2deg(arg(solver.get_zero_sequence_fault_current_in_pu()));


    if(PARAMETER_NAME=="VF1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE VOLTAGE AT FAULT PLACE IN PU")
        return abs(solver.get_positive_sequence_voltage_of_fault_point_in_pu());
    if(PARAMETER_NAME=="VF2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE VOLTAGE AT FAULT PLACE IN PU")
        return abs(solver.get_negative_sequence_voltage_of_fault_point_in_pu());
    if(PARAMETER_NAME=="VF0_PU" or PARAMETER_NAME=="ZERO SEQUENCE VOLTAGE AT FAULT PLACE IN PU")
        return abs(solver.get_zero_sequence_voltage_of_fault_point_in_pu());

    if(PARAMETER_NAME=="VF1_KA" or PARAMETER_NAME=="POSITIVE SEQUENCE VOLTAGE AT FAULT PLACE IN KA")
        return abs(solver.get_positive_sequence_voltage_of_fault_point_in_kV());
    if(PARAMETER_NAME=="VF2_KA" or PARAMETER_NAME=="NEGATIVE SEQUENCE VOLTAGE AT FAULT PLACE IN KA")
        return abs(solver.get_negative_sequence_voltage_of_fault_point_in_kV());
    if(PARAMETER_NAME=="VF0_KA" or PARAMETER_NAME=="ZERO SEQUENCE VOLTAGE AT FAULT PLACE IN KA")
        return abs(solver.get_zero_sequence_voltage_of_fault_point_in_kV());

    if(PARAMETER_NAME=="VF1_DEG" or PARAMETER_NAME=="POSITIVE SEQUENCE VOLTAGE AT FAULT IN DEG")
        return rad2deg(arg(solver.get_positive_sequence_voltage_of_fault_point_in_pu()));
    if(PARAMETER_NAME=="VF2_DEG" or PARAMETER_NAME=="NEGATIVE SEQUENCE VOLTAGE AT FAULT IN DEG")
        return rad2deg(arg(solver.get_negative_sequence_voltage_of_fault_point_in_pu()));
    if(PARAMETER_NAME=="VF0_DEG" or PARAMETER_NAME=="ZERO SEQUENCE VOLTAGE AT FAULT IN DEG")
        return rad2deg(arg(solver.get_zero_sequence_voltage_of_fault_point_in_pu()));


    if(PARAMETER_NAME=="R1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE EQUIVALENT R IN PU")
        return solver.get_positive_sequence_thevenin_impedance_at_fault_in_pu().real();
    if(PARAMETER_NAME=="X1_PU" or PARAMETER_NAME=="POSITIVE SEQUENCE EQUIVALENT X IN PU")
        return solver.get_positive_sequence_thevenin_impedance_at_fault_in_pu().imag();
    if(PARAMETER_NAME=="R2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE EQUIVALENT R IN PU")
        return solver.get_negative_sequence_thevenin_impedance_at_fault_in_pu().real();
    if(PARAMETER_NAME=="X2_PU" or PARAMETER_NAME=="NEGATIVE SEQUENCE EQUIVALENT X IN PU")
        return solver.get_negative_sequence_thevenin_impedance_at_fault_in_pu().imag();
    if(PARAMETER_NAME=="R0_PU" or PARAMETER_NAME=="ZERO SEQUENCE EQUIVALENT R IN PU")
        return solver.get_zero_sequence_thevenin_impedance_at_fault_in_pu().real();
    if(PARAMETER_NAME=="X0_PU" or PARAMETER_NAME=="ZERO SEQUENCE EQUIVALENT X IN PU")
        return solver.get_zero_sequence_thevenin_impedance_at_fault_in_pu().imag();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Parameter '%s' is not supported for short circuit result with api %s.\n"
             "0.0 will be returned.", PARAMETER_NAME.c_str(), __FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(buffer);
    return 0.0;
}


void api_show_short_circuit_result(unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.show_short_circuit_result();
}

void api_save_short_circuit_result_to_file(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.save_short_circuit_result_to_file(file);
}

void api_save_extended_short_circuit_result_to_file(char* file, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    solver.save_extended_short_circuit_result_to_file(file);
}
double api_get_short_circuit_ratio_of_HVDC(unsigned int hvdc_bus, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    double Kmscr = solver.get_short_circuit_ratio_of_HVDC(hvdc_bus);
    return Kmscr;
}

double api_get_short_circuit_ratio_of_RES(unsigned int res_bus, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    SHORT_CIRCUIT_SOLVER& solver = toolkit.get_short_circuit_solver();
    double Kmrscr = solver.get_short_circuit_ratio_of_HVDC(res_bus);
    return Kmrscr;
}
