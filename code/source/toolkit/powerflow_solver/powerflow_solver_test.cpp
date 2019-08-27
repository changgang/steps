#include "header/toolkit/powerflow_solver/powerflow_solver_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"
#include <cstdio>

using namespace std;


POWERFLOW_SOLVER_TEST::POWERFLOW_SOLVER_TEST()
{
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_constructor);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_max_iteration);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_allowed_max_active_power_imbalance_in_MW);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_allowed_max_reactive_power_imbalance_in_MVar);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_maximum_voltage_change_in_pu);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_maximum_angle_change);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_flat_start_logic);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_transformer_tap_adjustment_logic);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_non_divergent_solution_logic);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_set_get_var_limit_check_logic);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Arthur_R_Bergen_3_bus_model_with_full_Newton_Raphson_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Arthur_R_Bergen_3_bus_model_with_fast_decoupled_solution);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_IEEE_9_bus_model_with_full_Newton_Raphson_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_IEEE_9_bus_model_with_fast_decoupled_solution);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_with_full_Newton_Raphson_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_NPCC_140_bus_model_with_full_Newton_Raphson_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_NPCC_140_bus_model_with_fast_decoupled_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_NPCC_140_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution);


    //TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Shandong_2000_bus_model_with_full_Newton_Raphson_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Shandong_2000_bus_model_with_fast_decoupled_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Shandong_2000_bus_model_with_HVDC_with_fast_decoupled_solution);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_and_Shandong_2000_bus_model_with_fast_decoupled_solution);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Shandong_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Northwest_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_Yunnan_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution);

    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_IEEE_9_bus_model_with_WTG_with_full_Newton_Raphson_solution);
    TEST_ADD(POWERFLOW_SOLVER_TEST::test_solve_IEEE_39_bus_model_with_WTG_with_full_Newton_Raphson_solution);
}

void POWERFLOW_SOLVER_TEST::setup()
{
    ;
}

void POWERFLOW_SOLVER_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.clear();

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    powerflow_solver.clear();

    show_test_end_information();
}


void POWERFLOW_SOLVER_TEST::test_constructor()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    TEST_ASSERT(fabs(powerflow_solver.get_allowed_max_active_power_imbalance_in_MW()-0.001)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(powerflow_solver.get_allowed_max_reactive_power_imbalance_in_MVar()-0.001)<FLOAT_EPSILON);
    TEST_ASSERT(powerflow_solver.get_flat_start_logic()==false);
    TEST_ASSERT(powerflow_solver.get_transformer_tap_adjustment_logic()==true);
}

void POWERFLOW_SOLVER_TEST::test_set_get_max_iteration()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_max_iteration(100);
    TEST_ASSERT(powerflow_solver.get_max_iteration()==100);
}

void POWERFLOW_SOLVER_TEST::test_set_get_allowed_max_active_power_imbalance_in_MW()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    TEST_ASSERT(fabs(powerflow_solver.get_allowed_max_active_power_imbalance_in_MW()-0.001)<FLOAT_EPSILON);

    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0);
    TEST_ASSERT(fabs(powerflow_solver.get_allowed_max_active_power_imbalance_in_MW()-0.01)<FLOAT_EPSILON);
}

void POWERFLOW_SOLVER_TEST::test_set_get_allowed_max_reactive_power_imbalance_in_MVar()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    TEST_ASSERT(fabs(powerflow_solver.get_allowed_max_reactive_power_imbalance_in_MVar()-0.001)<FLOAT_EPSILON);

    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0);
    TEST_ASSERT(fabs(powerflow_solver.get_allowed_max_reactive_power_imbalance_in_MVar()-0.01)<FLOAT_EPSILON);
}

void POWERFLOW_SOLVER_TEST::test_set_get_maximum_voltage_change_in_pu()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_maximum_voltage_change_in_pu(0.1);
    TEST_ASSERT(fabs(powerflow_solver.get_maximum_voltage_change_in_pu()-0.1)<FLOAT_EPSILON);

    powerflow_solver.set_maximum_voltage_change_in_pu(0.2);
    TEST_ASSERT(fabs(powerflow_solver.get_maximum_voltage_change_in_pu()-0.2)<FLOAT_EPSILON);
}

void POWERFLOW_SOLVER_TEST::test_set_get_maximum_angle_change()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_maximum_angle_change_in_deg(20.0);
    TEST_ASSERT(fabs(powerflow_solver.get_maximum_angle_change_in_deg()-20.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(powerflow_solver.get_maximum_angle_change_in_rad()-(20.0/180.*PI))<FLOAT_EPSILON);

    powerflow_solver.set_maximum_angle_change_in_deg(30.0);
    TEST_ASSERT(fabs(powerflow_solver.get_maximum_angle_change_in_deg()-30.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(powerflow_solver.get_maximum_angle_change_in_rad()-(30.0/180.*PI))<FLOAT_EPSILON);
}

void POWERFLOW_SOLVER_TEST::test_set_get_flat_start_logic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_flat_start_logic(true);
    TEST_ASSERT(powerflow_solver.get_flat_start_logic()==true);

    powerflow_solver.set_flat_start_logic(false);
    TEST_ASSERT(powerflow_solver.get_flat_start_logic()==false);
}

void POWERFLOW_SOLVER_TEST::test_set_get_transformer_tap_adjustment_logic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_transformer_tap_adjustment_logic(true);
    TEST_ASSERT(powerflow_solver.get_transformer_tap_adjustment_logic()==true);

    powerflow_solver.set_transformer_tap_adjustment_logic(false);
    TEST_ASSERT(powerflow_solver.get_transformer_tap_adjustment_logic()==false);
}

void POWERFLOW_SOLVER_TEST::test_set_get_non_divergent_solution_logic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_non_divergent_solution_logic(true);
    TEST_ASSERT(powerflow_solver.get_non_divergent_solution_logic()==true);

    powerflow_solver.set_non_divergent_solution_logic(false);
    TEST_ASSERT(powerflow_solver.get_non_divergent_solution_logic()==false);
}

void POWERFLOW_SOLVER_TEST::test_set_get_var_limit_check_logic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_var_limit_check_logic(true);
    TEST_ASSERT(powerflow_solver.get_var_limit_check_logic()==true);

    powerflow_solver.set_var_limit_check_logic(false);
    TEST_ASSERT(powerflow_solver.get_var_limit_check_logic()==false);
}

void POWERFLOW_SOLVER_TEST::test_set_get_export_jacobian_matrix_step_by_step_logic()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    powerflow_solver.set_export_jacobian_matrix_step_by_step_logic(true);
    TEST_ASSERT(powerflow_solver.get_export_jacobian_matrix_step_by_step_logic()==true);

    powerflow_solver.set_export_jacobian_matrix_step_by_step_logic(false);
    TEST_ASSERT(powerflow_solver.get_export_jacobian_matrix_step_by_step_logic()==false);
}

void POWERFLOW_SOLVER_TEST::test_solve_Arthur_R_Bergen_3_bus_model_with_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    prepare_Arthur_R_Bergen_3_bus_model();

    default_toolkit.open_log_file("test_log/test_solve_Arthur_R_Bergen_3_bus_model_with_full_Newton_Raphson_solution.txt");

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);


    //powerflow_solver.initialize();

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_Arthur_R_Bergen_3_bus_model_powerflow_result();
    default_toolkit.close_log_file();
}
void POWERFLOW_SOLVER_TEST::test_solve_IEEE_9_bus_model_with_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");
    ostringstream osstream;

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_IEEE_9_bus_model_with_full_Newton_Raphson_solution.txt");

    prepare_IEEE_9_bus_model();

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    osstream<<"Test 1: Solution with non-flat start option";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_IEEE_9_bus_model_powerflow_result();

    powerflow_solver.save_network_matrix_to_file("test_log/IEEE9_network_matrix.csv");
    powerflow_solver.save_bus_powerflow_result_to_file("test_log/IEEE9_bus_powerflow_result.csv");
    powerflow_solver.save_jacobian_matrix_to_file("test_log/IEEE9_jacobian_matrix.csv");


    osstream<<"Test 2: Solution with flat start option";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_IEEE_9_bus_model_powerflow_result();

    osstream<<"Test 3: Solution with non-flat start option while Transformer 2-4 is 180deg displaced.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.set_flat_start_logic(false);

    DEVICE_ID did;
    did.set_device_type("TRANSFORMER");
    TERMINAL terminal;
    terminal.append_bus(2);
    terminal.append_bus(7);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    TRANSFORMER* trans = psdb.get_transformer(did);

    trans->set_winding_angle_shift_in_deg(PRIMARY_SIDE, 180.0);

    BUS* bus = psdb.get_bus(2);
    bus->set_angle_in_deg(180.0);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());


    osstream<<"Test 4: Solution with flat start option while generator 2 and 3 are Var-limited.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    trans->set_winding_angle_shift_in_deg(PRIMARY_SIDE, 0.0);
    bus->set_angle_in_deg(0.0);
    powerflow_solver.set_flat_start_logic(true);

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    GENERATOR* gen = psdb.get_generator(did);
    gen->set_q_max_in_MVar(100.0);
    gen->set_q_min_in_MVar(100.0);
    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    gen = psdb.get_generator(did);
    gen->set_q_max_in_MVar(100.0);
    gen->set_q_min_in_MVar(70.0);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_with_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    ostringstream osstream;

    default_toolkit.open_log_file("test_log/test_solve_ISO_New_England_39_bus_model_with_full_Newton_Raphson_solution.txt");

    NETWORK_MATRIX& network_matrix = powerflow_solver.get_network_matrix();

    psdb.set_allowed_max_bus_number(200);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);
    importer.load_powerflow_data("../../../bench/IEEE39.raw");

    TEST_ASSERT(psdb.get_bus_count()==39);
    TEST_ASSERT(psdb.get_generator_count()==10);
    TEST_ASSERT(psdb.get_load_count()==19);
    TEST_ASSERT(psdb.get_line_count()==34);
    TEST_ASSERT(psdb.get_transformer_count()==12);

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);


    powerflow_solver.set_flat_start_logic(false);
    osstream<<"Test 1: Solve powerflow with flat start logic disabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    network_matrix.report_network_matrix();
    check_ISO_New_England_39_bus_model_powerflow_result();


    powerflow_solver.set_flat_start_logic(true);
    osstream<<"Test 2: Solve powerflow with flat start logic enabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_ISO_New_England_39_bus_model_powerflow_result();

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    LINE* line = psdb.get_line(did);

    line->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.0000001));
    line->set_line_positive_sequence_y_in_pu(0.0);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    line = psdb.get_line(did);

    line->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.0000001));
    line->set_line_positive_sequence_y_in_pu(0.0);
    osstream<<"Test 3: Solve powerflow with line of zero impedance.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_NPCC_140_bus_model_with_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_NPCC_140_bus_model_with_full_Newton_Raphson_solution.txt");

    ostringstream osstream;

    psdb.set_allowed_max_bus_number(200);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);
    importer.load_powerflow_data("../../../bench/npcc.raw");

    osstream<<"Test 1: Solve powerflow with flat start logic disabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());
    //network_matrix.report_network_matrix();

    check_NPCC_140_bus_model_powerflow_result();
    //default_toolkit.close_log_file();

    //default_toolkit.open_log_file("test_log/test_solve_NPCC_140_bus_model_with_full_Newton_Raphson_solution_2.txt");
    osstream<<"Test 2: Solve powerflow with flat start logic enabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_NPCC_140_bus_model_powerflow_result();
    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_Shandong_2000_bus_model_with_full_Newton_Raphson_solution()
{
    ostringstream osstream;
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();


    default_toolkit.open_log_file("test_log/test_solve_Shandong_2000_bus_model_with_full_Newton_Raphson_solution.txt");

    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    osstream<<"Test 1: Solve non-var limit powerflow with flat start logic enable.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    importer.load_powerflow_data("../../../bench/sd2010_nodc_no_var_limit.raw");

    powerflow_solver.set_max_iteration(100);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());
    //network_matrix.report_network_matrix();


    /*os<< "Test 2: Solve var limit powerflow with flat start logic enable.");
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    reset_simulator();
    importer.load_powerflow_data("../../../bench/sd2010_nodc.raw");

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    */
    default_toolkit.close_log_file();
}


void POWERFLOW_SOLVER_TEST::test_solve_Arthur_R_Bergen_3_bus_model_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();

    prepare_Arthur_R_Bergen_3_bus_model();


    default_toolkit.open_log_file("test_log/test_solve_Arthur_R_Bergen_3_bus_model_with_fast_decoupled_solution.txt");
    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);


    //powerflow_solver.initialize();

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_Arthur_R_Bergen_3_bus_model_powerflow_result();
    default_toolkit.close_log_file();
}
void POWERFLOW_SOLVER_TEST::test_solve_IEEE_9_bus_model_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");
    ostringstream osstream;

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    prepare_IEEE_9_bus_model();

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    default_toolkit.open_log_file("test_log/test_solve_IEEE_9_bus_model_with_fast_decoupled_solution.txt");
    osstream<<"Test 1: Solution with non-flat start option";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_IEEE_9_bus_model_powerflow_result();
    //default_toolkit.close_log_file();

    //default_toolkit.open_log_file("test_log/test_solve_IEEE_9_bus_model_with_fast_decoupled_solution_2.txt");
    osstream<<"Test 2: Solution with flat start option";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_IEEE_9_bus_model_powerflow_result();
    //default_toolkit.close_log_file();

    //default_toolkit.open_log_file("test_log/test_solve_IEEE_9_bus_model_with_fast_decoupled_solution_3.txt");
    osstream<<"Test 3: Solution with flat start option while generator 2 and 3 are Var-limited.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    DEVICE_ID did;
    TERMINAL terminal;

    powerflow_solver.set_flat_start_logic(true);

    did.set_device_type("GENERATOR");
    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    GENERATOR* gen = psdb.get_generator(did);
    gen->set_q_max_in_MVar(100.0);
    gen->set_q_min_in_MVar(100.0);
    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    gen = psdb.get_generator(did);
    gen->set_q_max_in_MVar(100.0);
    gen->set_q_min_in_MVar(70.0);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    ostringstream osstream;

    default_toolkit.open_log_file("test_log/test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_solution_1.txt");

    psdb.set_allowed_max_bus_number(200);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);
    importer.load_powerflow_data("../../../bench/IEEE39.raw");

    TEST_ASSERT(psdb.get_bus_count()==39);
    TEST_ASSERT(psdb.get_generator_count()==10);
    TEST_ASSERT(psdb.get_load_count()==19);
    TEST_ASSERT(psdb.get_line_count()==34);
    TEST_ASSERT(psdb.get_transformer_count()==12);

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);


    powerflow_solver.set_flat_start_logic(false);


    osstream<<"Test 1: Solve powerflow with flat start logic disabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_ISO_New_England_39_bus_model_powerflow_result();
    default_toolkit.close_log_file();

    powerflow_solver.set_flat_start_logic(true);

    default_toolkit.open_log_file("test_log/test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_solution_2.txt");
    osstream<<"Test 2: Solve powerflow with flat start logic enabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_ISO_New_England_39_bus_model_powerflow_result();
    default_toolkit.close_log_file();

    DEVICE_ID did;
    did.set_device_type("LINE");
    TERMINAL terminal;
    terminal.append_bus(1);
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    LINE* line = psdb.get_line(did);

    line->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.0000001));
    line->set_line_positive_sequence_y_in_pu(0.0);

    terminal.clear();
    terminal.append_bus(2);
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");

    line = psdb.get_line(did);
    line->set_line_positive_sequence_z_in_pu(complex<double>(0.0, 0.0000001));
    line->set_line_positive_sequence_y_in_pu(0.0);

    default_toolkit.open_log_file("test_log/test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_solution_3.txt");
    osstream<<"Test 3: Solve powerflow with line of zero impedance.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_NPCC_140_bus_model_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    ostringstream osstream;

    default_toolkit.open_log_file("test_log/test_solve_NPCC_140_bus_model_with_fast_decoupled_solution.txt");

    psdb.set_allowed_max_bus_number(200);

    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);
    importer.load_powerflow_data("../../../bench/npcc.raw");

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.00001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.00001);
    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    osstream<<"Test 1: Solve powerflow with flat start logic disabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_NPCC_140_bus_model_powerflow_result();
    //default_toolkit.close_log_file();

    //default_toolkit.open_log_file("test_log/test_solve_NPCC_140_bus_model_with_fast_decoupled_solution_2.txt");
    osstream<<"Test 2: Solve powerflow with flat start logic enabled.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_NPCC_140_bus_model_powerflow_result();
    default_toolkit.close_log_file();
}
void POWERFLOW_SOLVER_TEST::test_solve_Shandong_2000_bus_model_with_fast_decoupled_solution()
{
    ostringstream osstream;
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    default_toolkit.open_log_file("test_log/test_solve_Shandong_2000_bus_model_with_fast_decoupled_solution_1.txt");
    osstream<<"Test 1: Solve non-var limit powerflow with flat start logic enable.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    importer.load_powerflow_data("../../../bench/sd2010_nodc_no_var_limit.raw");

    powerflow_solver.set_max_iteration(50);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();


    default_toolkit.open_log_file("test_log/test_solve_Shandong_2000_bus_model_with_fast_decoupled_solution_2.txt");
    osstream<<"Test 2: Solve var limit powerflow with flat start logic enable.";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    importer.load_powerflow_data("../../../bench/sd2010_nodc.raw");

    powerflow_solver.set_max_iteration(50);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_Shandong_2000_bus_model_with_HVDC_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_Shandong_2000_bus_model_with_HVDC_with_fast_decoupled_solution.txt");
    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/sd2010.raw");

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}


void POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_ISO_New_England_39_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution.txt");
    psdb.set_allowed_max_bus_number(200);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE39.raw");

    powerflow_solver.set_max_iteration(3);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_max_iteration(30);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_ISO_New_England_39_bus_model_powerflow_result();
    default_toolkit.close_log_file();
}


void POWERFLOW_SOLVER_TEST::test_solve_NPCC_140_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_NPCC_140_bus_model_with_fast_decoupled_and_full_Newton_Raphson_solution.txt");
    psdb.set_allowed_max_bus_number(200);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/npcc.raw");

    powerflow_solver.set_max_iteration(4);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();

    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_max_iteration(30);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    check_NPCC_140_bus_model_powerflow_result();
    default_toolkit.close_log_file();
}


void POWERFLOW_SOLVER_TEST::test_solve_ISO_New_England_39_bus_model_and_Shandong_2000_bus_model_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_ISO_New_England_39_bus_model_and_Shandong_2000_bus_model_with_fast_decoupled_solution.txt");
    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE39.raw");
    importer.load_powerflow_data("../../../bench/sd2010_nodc_no_var_limit.raw");

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::check_Arthur_R_Bergen_3_bus_model_powerflow_result()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    double veps = 1e-5, peps = 1e-3;

    BUS* bus;
    bus = psdb.get_bus(1);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-0.0)<veps);
    bus = psdb.get_bus(2);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.05)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-3.000597))<veps);
    bus = psdb.get_bus(3);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.949475)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-10.00501))<veps);

    GENERATOR* gen;
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;

    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-219.92)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-15.40361)<peps);
    cout<<"gen 1 of Arthur system: p = "<<gen->get_p_generation_in_MW()<<", q = "<<gen->get_q_generation_in_MVar()<<endl;

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-66.61)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-165.8286)<peps);
    cout<<"gen 2 of Arthur system: p = "<<gen->get_p_generation_in_MW()<<", q = "<<gen->get_q_generation_in_MVar()<<endl;
}

void POWERFLOW_SOLVER_TEST::check_IEEE_9_bus_model_powerflow_result()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    double veps = 1e-5, peps = 1e-3;

    BUS* bus;
    bus = psdb.get_bus(1);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.04)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-0.0)<veps);
    bus = psdb.get_bus(2);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.025)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-9.280008)<veps);
    bus = psdb.get_bus(3);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.025)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-4.664753)<veps);
    bus = psdb.get_bus(4);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.025788)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-2.216789))<veps);
    bus = psdb.get_bus(5);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.9956308)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-3.988807))<veps);
    bus = psdb.get_bus(6);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.012654)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-3.687397))<veps);
    bus = psdb.get_bus(7);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.025769)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-3.719703)<veps);
    bus = psdb.get_bus(8);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.015883)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-0.7275366)<veps);
    bus = psdb.get_bus(9);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.032353)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-1.966717)<veps);

    GENERATOR* gen;
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;

    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-71.64101)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-27.04592)<peps);

    terminal.clear();
    terminal.append_bus(2);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-163.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-6.653666)<peps);

    terminal.clear();
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-85.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-(-10.8597))<peps);
}

void POWERFLOW_SOLVER_TEST::check_ISO_New_England_39_bus_model_powerflow_result()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    double veps = 1e-5, peps = 1e-3;

    BUS* bus;
    bus = psdb.get_bus(1);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.047543)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-9.570696))<veps);
    bus = psdb.get_bus(2);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.048955)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-7.011069))<veps);
    bus = psdb.get_bus(3);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.030421)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-9.858091))<veps);
    bus = psdb.get_bus(4);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.003808)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-10.6546))<veps);
    bus = psdb.get_bus(5);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.005019)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-9.468711))<veps);
    bus = psdb.get_bus(6);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.007365)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-8.766821))<veps);
    bus = psdb.get_bus(7);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.9966974)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-10.970223))<veps);
    bus = psdb.get_bus(8);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.995726)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-11.47624))<veps);
    bus = psdb.get_bus(9);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.028092)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-11.29968))<veps);
    bus = psdb.get_bus(10);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.017024)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-6.381594))<veps);
    bus = psdb.get_bus(11);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.01253)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-7.195228))<veps);
    bus = psdb.get_bus(12);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.000001)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-7.210636))<veps);
    bus = psdb.get_bus(13);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.014192)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-7.0959))<veps);
    bus = psdb.get_bus(14);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.011731)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-8.76476))<veps);
    bus = psdb.get_bus(15);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.015785)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-9.180734))<veps);
    bus = psdb.get_bus(16);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.032254)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-7.77659))<veps);
    bus = psdb.get_bus(17);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.033955)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-8.774818))<veps);
    bus = psdb.get_bus(18);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.031295)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-9.615593))<veps);
    bus = psdb.get_bus(19);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.050009)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-3.152467))<veps);
    bus = psdb.get_bus(20);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.9909571)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-4.563902))<veps);
    bus = psdb.get_bus(21);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.032098)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-5.37102))<veps);
    bus = psdb.get_bus(22);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.049976)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-0.9239033))<veps);
    bus = psdb.get_bus(23);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.044976)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-1.122123))<veps);
    bus = psdb.get_bus(24);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.037749)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-7.656979))<veps);
    bus = psdb.get_bus(25);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.05752)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-5.649259))<veps);
    bus = psdb.get_bus(26);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.052155)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-6.905797))<veps);
    bus = psdb.get_bus(27);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.037948)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-8.917284))<veps);
    bus = psdb.get_bus(28);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.050164)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-3.394192))<veps);
    bus = psdb.get_bus(29);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.049971)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-0.6350747))<veps);
    bus = psdb.get_bus(30);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0475)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-4.591804))<veps);
    bus = psdb.get_bus(31);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.982)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-0.0)<veps);
    bus = psdb.get_bus(32);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.9831)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-1.615497)<veps);
    bus = psdb.get_bus(33);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.9972)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-2.064657)<veps);
    bus = psdb.get_bus(34);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0123)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-0.6262833)<veps);
    bus = psdb.get_bus(35);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0493)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-4.036989)<veps);
    bus = psdb.get_bus(36);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0635)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-6.729544)<veps);
    bus = psdb.get_bus(37);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0278)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-1.135481)<veps);
    bus = psdb.get_bus(38);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0265)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-6.428187)<veps);
    bus = psdb.get_bus(39);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.03)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-(-11.10825))<veps);

    GENERATOR* gen;
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;

    terminal.append_bus(30);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-250.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-144.9194)<peps);

    terminal.clear();
    terminal.append_bus(31);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-572.835)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-207.0365)<peps);

    terminal.clear();
    terminal.append_bus(32);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-650.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-205.7312)<peps);

    terminal.clear();
    terminal.append_bus(33);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-632.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-108.9362)<peps);

    terminal.clear();
    terminal.append_bus(34);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-508.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-166.9861)<peps);

    terminal.clear();
    terminal.append_bus(35);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-650.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-211.1118)<peps);

    terminal.clear();
    terminal.append_bus(36);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-560.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-100.4377)<peps);

    terminal.clear();
    terminal.append_bus(37);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-540.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-0.64692)<peps);

    terminal.clear();
    terminal.append_bus(38);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-830.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-22.65877)<peps);

    terminal.clear();
    terminal.append_bus(39);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-1000.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-87.88259)<peps);
}

void POWERFLOW_SOLVER_TEST::check_NPCC_140_bus_model_powerflow_result()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    double veps = 1e-5, peps = 1e-3;

    BUS* bus;
    bus = psdb.get_bus(1);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.01107)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-4.961183)<veps);
    bus = psdb.get_bus(2);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.006724)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-4.168255)<veps);
    bus = psdb.get_bus(3);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.046986)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-4.116957)<veps);
    bus = psdb.get_bus(11);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.9841065)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-6.493938)<veps);
    bus = psdb.get_bus(12);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.02509)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-5.997)<veps);
    bus = psdb.get_bus(13);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.04576)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-10.72433)<veps);
    bus = psdb.get_bus(21);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.98)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-13.0313)<veps);
    bus = psdb.get_bus(22);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-0.99)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-13.26405)<veps);
    bus = psdb.get_bus(23);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.0066)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-11.69455)<veps);
    bus = psdb.get_bus(51);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.04)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-8.822804)<veps);
    bus = psdb.get_bus(52);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.040545)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-19.85086)<veps);
    bus = psdb.get_bus(53);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.03)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-18.43824)<veps);
    bus = psdb.get_bus(101);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.05)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-25.10549)<veps);
    bus = psdb.get_bus(102);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.021117)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-26.24741)<veps);
    bus = psdb.get_bus(103);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.011717)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-26.05651)<veps);
    bus = psdb.get_bus(131);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.026067)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-13.3688)<veps);
    bus = psdb.get_bus(132);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.039277)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-19.53944)<veps);
    bus = psdb.get_bus(133);
    TEST_ASSERT(fabs(bus->get_voltage_in_pu()-1.02)<veps);
    TEST_ASSERT(fabs(bus->get_angle_in_deg()-34.30826)<veps);

    GENERATOR* gen;
    DEVICE_ID did;
    did.set_device_type("GENERATOR");
    TERMINAL terminal;

    terminal.append_bus(21);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-650.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-217.7195)<peps);

    terminal.clear();
    terminal.append_bus(27);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-540.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-44.97655)<peps);

    terminal.clear();
    terminal.append_bus(47);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-400.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-(-3.853355))<peps);

    terminal.clear();
    terminal.append_bus(60);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-410.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-96.04207)<peps);

    terminal.clear();
    terminal.append_bus(78);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-461.3814)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-90.81911)<peps);

    terminal.clear();
    terminal.append_bus(101);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-1200.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-164.5751)<peps);

    terminal.clear();
    terminal.append_bus(139);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    gen = psdb.get_generator(did);
    TEST_ASSERT(fabs(gen->get_p_generation_in_MW()-115.0)<peps);
    TEST_ASSERT(fabs(gen->get_q_generation_in_MVar()-18.48766)<peps);
}


void POWERFLOW_SOLVER_TEST::test_solve_Shandong_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    ostringstream osstream;

    default_toolkit.open_log_file("test_log/test_solve_Shandong_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution.txt");
    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/bench_shandong.raw");

    osstream<<"Solve with constant power HVDC links";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        hvdcs[i]->set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
    }

    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    osstream<<"Solve with constant current HVDC links";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

    for(size_t i=0; i!=n; ++i)
    {
        hvdcs[i]->turn_rectifier_constant_power_mode_into_constant_current_mode();
    }


    powerflow_solver.set_max_iteration(30);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    BPA_IMEXPORTER exporter;
    exporter.set_toolkit(default_toolkit);
    exporter.export_powerflow_data("test_log/bench_shandong.dat");

    default_toolkit.close_log_file();
}


void POWERFLOW_SOLVER_TEST::test_solve_Northwest_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_Northwest_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution.txt");
    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/bench_northwest.raw");

    powerflow_solver.set_max_iteration(50);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    //powerflow_solver.solve_with_fast_decoupled_solution();
    //powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    BPA_IMEXPORTER exporter;
    exporter.set_toolkit(default_toolkit);
    exporter.export_powerflow_data("test_log/bench_northwest.dat");

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_Yunnan_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_Yunnan_benchmark_100_bus_model_with_HVDC_with_fast_decoupled_solution.txt");
    psdb.set_allowed_max_bus_number(160000);
    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/bench_yunnan.raw");

    powerflow_solver.set_max_iteration(100);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(10.0);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(10.0);
    powerflow_solver.set_flat_start_logic(true);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);

    powerflow_solver.solve_with_fast_decoupled_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    powerflow_solver.set_flat_start_logic(false);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.0001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.0001);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    BPA_IMEXPORTER exporter;
    exporter.set_toolkit(default_toolkit);
    exporter.export_powerflow_data("test_log/bench_yunnan.dat");

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_IEEE_9_bus_model_with_WTG_with_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");
    ostringstream osstream;

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_IEEE_9_bus_model_with_WTG_with_full_Newton_Raphson_solution.txt");

    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE9_wind.raw");

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);
    powerflow_solver.set_flat_start_logic(true);

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(3);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    WT_GENERATOR* wtgen = psdb.get_wt_generator(did);

    osstream<<"Test 1: Solution with flat start option when WTG is in Q in (-999.0, 999.0)MVar";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    wtgen->set_q_max_in_MVar(999.0);
    wtgen->set_q_min_in_MVar(-999.0);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    osstream<<"Test 2: Solution with flat start option when WTG is in Q in (-10.0, -10.0)MVar";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    wtgen->set_q_max_in_MVar(-10.0);
    wtgen->set_q_min_in_MVar(-10.0);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    osstream<<"Test 3: Solution with flat start option when WTG is in Q in (0.0, 0.0)MVar";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    wtgen->set_q_max_in_MVar(0.0);
    wtgen->set_q_min_in_MVar(0.0);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}

void POWERFLOW_SOLVER_TEST::test_solve_IEEE_39_bus_model_with_WTG_with_full_Newton_Raphson_solution()
{
    show_test_information_for_function_of_class(__FUNCTION__,"POWERFLOW_SOLVER_TEST");
    ostringstream osstream;

    POWERFLOW_SOLVER& powerflow_solver = default_toolkit.get_powerflow_solver();
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();

    default_toolkit.open_log_file("test_log/test_solve_IEEE_39_bus_model_with_WTG_with_full_Newton_Raphson_solution.txt");

    PSSE_IMEXPORTER importer;
    importer.set_toolkit(default_toolkit);

    importer.load_powerflow_data("../../../bench/IEEE39_wind.raw");

    powerflow_solver.set_max_iteration(20);
    powerflow_solver.set_allowed_max_active_power_imbalance_in_MW(0.001);
    powerflow_solver.set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    powerflow_solver.set_transformer_tap_adjustment_logic(true);
    powerflow_solver.set_flat_start_logic(true);

    DEVICE_ID did;
    did.set_device_type("WT GENERATOR");
    TERMINAL terminal;
    terminal.append_bus(36);
    did.set_device_terminal(terminal);
    did.set_device_identifier("1");
    WT_GENERATOR* wtgen36 = psdb.get_wt_generator(did);
    terminal.clear();
    terminal.append_bus(37);
    did.set_device_terminal(terminal);
    WT_GENERATOR* wtgen37 = psdb.get_wt_generator(did);

    osstream<<"Test 1: Solution with flat start option when WTG is as default";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    osstream<<"Test 2: Solution with flat start option when WTG is in Q in (-999.0, 999.0)MVar";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    wtgen36->set_q_max_in_MVar(999.0);
    wtgen36->set_q_min_in_MVar(-999.0);
    wtgen37->set_q_max_in_MVar(999.0);
    wtgen37->set_q_min_in_MVar(-999.0);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    osstream<<"Test 3: Solution with flat start option when WTG is in Q in (-10.0, -10.0)MVar";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    wtgen36->set_q_max_in_MVar(-10.0);
    wtgen36->set_q_min_in_MVar(-10.0);
    wtgen37->set_q_max_in_MVar(-10.0);
    wtgen37->set_q_min_in_MVar(-10.0);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    osstream<<"Test 4: Solution with flat start option when WTG is in Q in (0.0, 0.0)MVar";
    default_toolkit.show_information_with_leading_time_stamp(osstream);
    wtgen36->set_q_max_in_MVar(0.0);
    wtgen36->set_q_min_in_MVar(0.0);
    wtgen37->set_q_max_in_MVar(0.0);
    wtgen37->set_q_min_in_MVar(0.0);
    powerflow_solver.solve_with_full_Newton_Raphson_solution();
    TEST_ASSERT(powerflow_solver.is_converged());

    default_toolkit.close_log_file();
}
