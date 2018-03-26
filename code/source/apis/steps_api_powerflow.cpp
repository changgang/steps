#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"


void api_initialize_powerflow_solver()
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    POWER_SYSTEM_DATABASE* psdb = get_default_power_system_database();
    solver->set_power_system_database(psdb);
    solver->initialize_powerflow_solver();
}


size_t api_get_powerflow_solver_integer_parameter(char* parameter_name)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX ITERATION")
        return solver->get_max_iteration();

    ostringstream sstream;
    sstream<<"Parameter '"<<PARAMETER_NAME<<"' cannot be retrieved for powerflow solver with api "<<__FUNCTION__<<endl
           <<"0 will be returned.";
    show_information_with_leading_time_stamp(sstream);
    return 0;
}

void api_set_powerflow_solver_integer_parameter(char* parameter_name, int value)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX ITERATION")
    {
        solver->set_max_iteration(value);
        return;
    }

    ostringstream sstream;
    sstream<<"Parameter '"<<PARAMETER_NAME<<"' cannot be set for powerflow solver with api "<<__FUNCTION__;
    show_information_with_leading_time_stamp(sstream);
    return;
}

double api_get_powerflow_solver_float_parameter(char* parameter_name)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();
    POWER_SYSTEM_DATABASE* psdb = solver->get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX ACTIVE POWER IMBALANCE IN MW")
        return solver->get_allowed_max_active_power_imbalance_in_MW();
    if(PARAMETER_NAME=="MAX ACTIVE POWER IMBALANCE IN PU")
        return solver->get_allowed_max_active_power_imbalance_in_MW()/psdb->get_system_base_power_in_MVA();
    if(PARAMETER_NAME=="MAX REACTIVE POWER IMBALANCE IN MVAR")
        return solver->get_allowed_max_reactive_power_imbalance_in_MVar();
    if(PARAMETER_NAME=="MAX REACTIVE POWER IMBALANCE IN PU")
        return solver->get_allowed_max_reactive_power_imbalance_in_MVar()/psdb->get_system_base_power_in_MVA();
    if(PARAMETER_NAME=="ITERATION ACCELERATOR")
        return solver->get_iteration_accelerator();

    ostringstream sstream;
    sstream<<"Parameter '"<<PARAMETER_NAME<<"' cannot be retrieved for powerflow solver with api "<<__FUNCTION__<<endl
           <<"0.0 will be returned.";
    show_information_with_leading_time_stamp(sstream);
    return 0.0;
}

void api_set_powerflow_solver_float_parameter(char* parameter_name, double value)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();
    POWER_SYSTEM_DATABASE* psdb = solver->get_power_system_database();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="MAX ACTIVE POWER IMBALANCE IN MW")
    {
        solver->set_allowed_max_active_power_imbalance_in_MW(value);
        return;
    }
    if(PARAMETER_NAME=="MAX ACTIVE POWER IMBALANCE IN PU")
    {
        solver->set_allowed_max_active_power_imbalance_in_MW(value*psdb->get_system_base_power_in_MVA());
        return;
    }
    if(PARAMETER_NAME=="MAX REACTIVE POWER IMBALANCE IN MVAR")
    {
        solver->set_allowed_max_reactive_power_imbalance_in_MVar(value);
        return;
    }
    if(PARAMETER_NAME=="MAX REACTIVE POWER IMBALANCE IN PU")
    {
        solver->set_allowed_max_reactive_power_imbalance_in_MVar(value*psdb->get_system_base_power_in_MVA());
        return;
    }
    if(PARAMETER_NAME=="ITERATION ACCELERATOR")
    {
        solver->set_iteration_accelerator(value);
        return;
    }

    ostringstream sstream;
    sstream<<"Parameter '"<<PARAMETER_NAME<<"' cannot be set for powerflow solver with api "<<__FUNCTION__;
    show_information_with_leading_time_stamp(sstream);
    return;
}

bool api_get_powerflow_solver_boolean_parameter(char* parameter_name)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="FLAT START LOGIC")
        return solver->get_flat_start_logic();
    if(PARAMETER_NAME=="TRANSFORMER TAP ADJUSTMENT LOGIC")
        return solver->get_transformer_tap_adjustment_logic();
    if(PARAMETER_NAME=="NON DIVERGENT SOLUTION LOGIC")
        return solver->get_non_divergent_solution_logic();

    ostringstream sstream;
    sstream<<"Parameter '"<<PARAMETER_NAME<<"' cannot be retrieved for powerflow solver with api "<<__FUNCTION__<<endl
           <<"False will be returned.";
    show_information_with_leading_time_stamp(sstream);
    return false;
}

void api_set_powerflow_solver_boolean_parameter(char* parameter_name, bool value)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    string PARAMETER_NAME = string2upper(parameter_name);
    if(PARAMETER_NAME=="FLAT START LOGIC")
    {
        solver->set_flat_start_logic(value);
        return;
    }
    if(PARAMETER_NAME=="TRANSFORMER TAP ADJUSTMENT LOGIC")
    {
        solver->set_transformer_tap_adjustment_logic(value);
        return;
    }
    if(PARAMETER_NAME=="NON DIVERGENT SOLUTION LOGIC")
    {
        solver->set_non_divergent_solution_logic(value);
        return;
    }

    ostringstream sstream;
    sstream<<"Parameter '"<<PARAMETER_NAME<<"' cannot be set for powerflow solver with api "<<__FUNCTION__;
    show_information_with_leading_time_stamp(sstream);
    return;
}

void api_solve_powerflow(char* method)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    string string_method = string2upper(method);

    if(string_method == "NR")
    {
        solver->solve_with_full_Newton_Raphson_solution();
        return;
    }

    if(string_method == "PQ")
    {
        solver->solve_with_fast_decoupled_solution();
        return;
    }

    if(string_method == "GS")
    {
        solver->solve_with_modified_Gaussian_Seidel_solution();
        return;
    }

    ostringstream sstream;
    sstream<<"Method '"<<string_method<<"' is not supported for solving powerflow with api "<<__FUNCTION__;
    show_information_with_leading_time_stamp(sstream);
    return;
}

bool api_is_powerflow_converged()
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    return solver->is_converged();
}

void api_show_powerflow_result()
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    solver->show_powerflow_result();
}

void api_save_powerflow_result(char* file)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();

    solver->save_bus_powerflow_result_to_file(file);
}

void api_save_network_matrix(char* file)
{
    POWERFLOW_SOLVER* solver = api_get_default_powerflow_solver();
    NETWORK_DATABASE* ntdb = solver->get_network_database();
    ntdb->save_network_matrix_to_file(file);
}
