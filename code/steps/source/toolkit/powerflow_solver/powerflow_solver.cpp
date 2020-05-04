#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include "header/network/jacobian_builder.h"
#include <cstdio>
#include <istream>
#include <iostream>
#include <fstream>
using namespace std;

#define ENABLE_OPENMP_FOR_POWERFLOW_SOLVER

POWERFLOW_SOLVER::POWERFLOW_SOLVER(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
    jacobian_builder = new JACOBIAN_BUILDER(toolkit);
    clear();
}

POWERFLOW_SOLVER::~POWERFLOW_SOLVER()
{
    toolkit = NULL;
    delete jacobian_builder;
}

STEPS& POWERFLOW_SOLVER::get_toolkit() const
{
    return *toolkit;
}

void POWERFLOW_SOLVER::clear()
{
    set_flat_start_logic(false);
    set_transformer_tap_adjustment_logic(true);
    set_non_divergent_solution_logic(true);
    set_var_limit_check_logic(true);
    set_export_jacobian_matrix_step_by_step_logic(false);
    set_allowed_max_active_power_imbalance_in_MW(0.001);
    set_allowed_max_reactive_power_imbalance_in_MVar(0.001);
    set_maximum_voltage_change_in_pu(999.0);
    set_maximum_angle_change_in_deg(999.0);

    set_convergence_flag(false);

    set_max_iteration(50);

    bus_current.clear();
    bus_power.clear();

    jacobian.clear();

    iteration_count = 0;
    set_iteration_accelerator(1.0);

    S_mismatch.clear();
    P_mismatch.clear();
    Q_mismatch.clear();
}

NETWORK_MATRIX& POWERFLOW_SOLVER::get_network_matrix()
{
    return toolkit->get_network_matrix();
}

void POWERFLOW_SOLVER::set_max_iteration(unsigned int iteration)
{
    this->max_iteration = iteration;
}

unsigned int POWERFLOW_SOLVER::get_max_iteration() const
{
    return max_iteration;
}

void POWERFLOW_SOLVER::set_allowed_max_active_power_imbalance_in_MW(double P)
{
    if(P>0.0)
        P_threshold_in_MW = P;
    else
    {
        char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Non-positive (%fMW) is not allowed for setting convergence threshold of"
                 "active power imbalance for powerflow solution.\n0.01MW will be set automatically.", P);
        toolkit->show_information_with_leading_time_stamp(buffer);
        P_threshold_in_MW = 0.01;
    }
}

void POWERFLOW_SOLVER::set_allowed_max_reactive_power_imbalance_in_MVar(double Q)
{
    if(Q>0.0)
        Q_threshold_in_MVar = Q;
    else
    {
        char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Non-positive (%fMVar) is not allowed for setting convergence threshold of"
                 "reactive power imbalance for powerflow solution.\n0.01MVar will be set automatically.", Q);
        toolkit->show_information_with_leading_time_stamp(buffer);
        Q_threshold_in_MVar = 0.01;
    }
}


void POWERFLOW_SOLVER::set_maximum_voltage_change_in_pu(double v)
{
    maximum_voltage_change_in_pu = v;
}

void POWERFLOW_SOLVER::set_maximum_angle_change_in_deg(double a)
{
    maximum_angle_change_in_deg = a;
}

void POWERFLOW_SOLVER::set_flat_start_logic(bool logic)
{
    flat_start_enabled = logic;
}

void POWERFLOW_SOLVER::set_transformer_tap_adjustment_logic(bool logic)
{
    transformer_tap_adjustment_enabled = logic;
}

void POWERFLOW_SOLVER::set_iteration_accelerator(double iter_alpha)
{
    if(iter_alpha>0.0)
        this->alpha = iter_alpha;
    else
        this->alpha = 1.0;
}

void POWERFLOW_SOLVER::set_non_divergent_solution_logic(bool logic)
{
    non_divergent_solution_enabled = logic;
}

void POWERFLOW_SOLVER::set_var_limit_check_logic(bool logic)
{
    var_limit_check_enabled = logic;
}

void POWERFLOW_SOLVER::set_export_jacobian_matrix_step_by_step_logic(bool flag)
{
    export_jacobian_matrix_step_by_step = flag;
}

double POWERFLOW_SOLVER::get_allowed_max_active_power_imbalance_in_MW() const
{
    return P_threshold_in_MW;
}

double POWERFLOW_SOLVER::get_allowed_max_reactive_power_imbalance_in_MVar() const
{
    return Q_threshold_in_MVar;
}

double POWERFLOW_SOLVER::get_maximum_voltage_change_in_pu() const
{
    return maximum_voltage_change_in_pu;
}

double POWERFLOW_SOLVER::get_maximum_angle_change_in_deg() const
{
    return maximum_angle_change_in_deg;
}

double POWERFLOW_SOLVER::get_maximum_angle_change_in_rad() const
{
    return get_maximum_angle_change_in_deg()*PI_OVER_180;
}

bool POWERFLOW_SOLVER::get_flat_start_logic() const
{
    return flat_start_enabled;
}

bool POWERFLOW_SOLVER::get_transformer_tap_adjustment_logic() const
{
    return transformer_tap_adjustment_enabled;
}

double POWERFLOW_SOLVER::get_iteration_accelerator() const
{
    return alpha;
}

bool POWERFLOW_SOLVER::get_non_divergent_solution_logic() const
{
    return non_divergent_solution_enabled;
}

bool POWERFLOW_SOLVER::get_var_limit_check_logic() const
{
    return var_limit_check_enabled;
}

bool POWERFLOW_SOLVER::get_export_jacobian_matrix_step_by_step_logic() const
{
    return export_jacobian_matrix_step_by_step;
}

void POWERFLOW_SOLVER::show_powerflow_solver_configuration() const
{
    ostringstream osstream;
    osstream<<"Configuration of powerflow solver:\n"
            <<"Fast sin/cos/tan functions: "<<(use_steps_fast_math?"Enabled":"Disabled")<<"\n"
            <<"Maximum iteration: "<<get_max_iteration()<<"\n"
            <<"Iteration accelerator: "<<get_iteration_accelerator()<<"\n"
            <<"Allowed maximum P mismatch: "<<get_allowed_max_active_power_imbalance_in_MW()<<" MW\n"
            <<"Allowed maximum Q mismatch: "<<get_allowed_max_reactive_power_imbalance_in_MVar()<<"MVar\n"
            <<"Maximum voltage change: "<<get_maximum_voltage_change_in_pu()<<" pu\n"
            <<"Maximum angle change: "<<get_maximum_angle_change_in_deg()<<" deg ("<<get_maximum_angle_change_in_rad()<<" rad)\n"
            <<"Flat start: "<<(get_flat_start_logic()?"Enabled":"Disabled")<<"\n"
            <<"Transformer tap adjustment: "<<(get_transformer_tap_adjustment_logic()?"Enabled":"Disabled")<<"\n"
            <<"Non-divergent solution: "<<(get_non_divergent_solution_logic()?"Enabled":"Disabled")<<"\n"
            <<"Var limit check: "<<(get_var_limit_check_logic()?"Enabled":"Disabled")<<"\n"
            <<"Network ordering optimization: "<<toolkit->is_optimize_network_enabled()<<"\n"
            <<"Export jacobian matrix step by step: "<<(get_export_jacobian_matrix_step_by_step_logic()?"Enabled":"Disabled");
    toolkit->show_information_with_leading_time_stamp(osstream);
}

void POWERFLOW_SOLVER::solve_with_full_Newton_Raphson_solution()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    if(psdb.get_bus_count()!=0)
    {
        char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Start solve powerflow with Full Newton Raphson solution.");
        toolkit->show_information_with_leading_time_stamp(buffer);

        initialize_powerflow_solver();

        NETWORK_MATRIX& network_matrix = get_network_matrix();

        double max_P_mismatch_in_MW, max_Q_mismatch_in_MW;
        vector<double> bus_delta_voltage_angle;

        network_matrix.build_network_Y_matrix();

        update_P_and_Q_equation_internal_buses();
        jacobian_builder->build_seprate_jacobians();

        while(true)
        {
            char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Iteration %u:",iteration_count);
            toolkit->show_information_with_leading_time_stamp(buffer);

            try_to_solve_hvdc_steady_state();
            calculate_raw_bus_power_mismatch();

            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();


            bool bus_type_changed = false;
            if(get_var_limit_check_logic()==true)
                bus_type_changed = check_bus_type_constraints();
            else
                update_source_power_without_constraints();

            if(bus_type_changed)
            {
                update_P_and_Q_equation_internal_buses();

                try_to_solve_hvdc_steady_state();
                calculate_raw_bus_power_mismatch();

                max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
                max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();
            }
            if(max_P_mismatch_in_MW < get_allowed_max_active_power_imbalance_in_MW() and
               max_Q_mismatch_in_MW < get_allowed_max_reactive_power_imbalance_in_MVar())
            {
                set_convergence_flag(true);

                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow converged within %u iterations.",iteration_count);
                toolkit->show_information_with_leading_time_stamp(buffer);
                break;
            }
            else
                set_convergence_flag(false);

            if(get_iteration_count()>=get_max_iteration())
            {
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow failed to converge within %u iterations.",get_max_iteration());
                toolkit->show_information_with_leading_time_stamp(buffer);
                break;
            }

            build_bus_power_mismatch_vector_for_coupled_solution();

            jacobian_builder->update_seprate_jacobians();

            jacobian = jacobian_builder->get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(internal_P_equation_buses,
                                                                                                       internal_Q_equation_buses);

            if(get_export_jacobian_matrix_step_by_step_logic()==true)
            {
                jacobian_builder->save_jacobian_matrix_to_file("Jacobian-NR-Iter-"+num2str(get_iteration_count())+".csv");
            }
            bus_delta_voltage_angle = S_mismatch/jacobian;

            update_bus_voltage_and_angle(bus_delta_voltage_angle);

            ++iteration_count;
        }
        //show_powerflow_result();
    }
}

void POWERFLOW_SOLVER::solve_with_fast_decoupled_solution()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    ostringstream osstream;

    if(psdb.get_bus_count()!=0)
    {
        char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Start solve powerflow with Fast Decoupled solution.");
        toolkit->show_information_with_leading_time_stamp(buffer);

        initialize_powerflow_solver();

        NETWORK_MATRIX& network_matrix = get_network_matrix();

        double max_P_mismatch_in_MW, max_Q_mismatch_in_MW;
        vector<double> bus_delta_voltage, bus_delta_angle;

        network_matrix.build_network_Y_matrix();

        network_matrix.build_decoupled_network_B_matrix();
        //const SPARSE_MATRIX& Y = network_matrix.get_network_Y_matrix();
        //cout<<"Y matrix identity is: "<<get_sparse_matrix_identity(Y)<<endl;

        update_P_and_Q_equation_internal_buses();
        BP = jacobian_builder->get_decoupled_B_jacobian_with_P_equation_internal_buses(internal_P_equation_buses);
        BQ = jacobian_builder->get_decoupled_B_jacobian_with_Q_equation_internal_buses(internal_Q_equation_buses);
        //BP.LU_factorization(1, 1e-13);
        //BQ.LU_factorization(1, 1e-13);
        //BP.report_brief();
        //BQ.report_brief();

        //unsigned int physical_bus;

        while(true)
        {

            char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Iteration %u:",iteration_count);
            toolkit->show_information_with_leading_time_stamp(buffer);

            try_to_solve_hvdc_steady_state();
            calculate_raw_bus_power_mismatch();

            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();

            bool bus_type_changed = false;
            if(get_var_limit_check_logic()==true)
                bus_type_changed = check_bus_type_constraints();
            else
                update_source_power_without_constraints();

            if(bus_type_changed)
            {
                update_P_and_Q_equation_internal_buses();
                //BP = jacobian_builder->get_decoupled_B_jacobian_with_P_equation_internal_buses(internal_P_equation_buses);
                BQ = jacobian_builder->get_decoupled_B_jacobian_with_Q_equation_internal_buses(internal_Q_equation_buses);
                BQ.LU_factorization(1, 1e-13);

                try_to_solve_hvdc_steady_state();
                calculate_raw_bus_power_mismatch();

                max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
                max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();
                //continue;
            }

            /*if(get_export_jacobian_matrix_step_by_step_logic()==true)
            {
                BP.save_matrix_to_file("Jacobian-BP-PQ-Iter-"+num2str(get_iteration_count())+".csv");
                BQ.save_matrix_to_file("Jacobian-BQ-PQ-Iter-"+num2str(get_iteration_count())+".csv");
            }*/

            if(max_P_mismatch_in_MW < get_allowed_max_active_power_imbalance_in_MW() and
               max_Q_mismatch_in_MW < get_allowed_max_reactive_power_imbalance_in_MVar())
            {
                set_convergence_flag(true);

                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow converged within %u iterations.",iteration_count);
                toolkit->show_information_with_leading_time_stamp(buffer);
                break;
            }
            else
                set_convergence_flag(false);

            if(get_iteration_count()>=get_max_iteration())
            {
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow failed to converge within %u iterations.",get_max_iteration());
                toolkit->show_information_with_leading_time_stamp(buffer);
                break;
            }

            build_bus_P_power_mismatch_vector_for_decoupled_solution();
            unsigned int n = internal_P_equation_buses.size();

            #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                set_openmp_number_of_threads(toolkit->get_thread_number());
                #pragma omp parallel for schedule(static)
            #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            for(unsigned int i=0; i<n; ++i)
            {
                unsigned int internal_bus = internal_P_equation_buses[i];
                //physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]);
                //P_mismatch[i] /= psdb.get_bus_positive_sequence_voltage_in_pu(physical_bus);
                P_mismatch[i] /= get_bus_positive_sequence_voltage_in_pu_with_internal_bus_number(internal_bus);
            }
            bus_delta_angle = P_mismatch/BP;
            //BP.report_brief();
            //for(unsigned int i=0; i<P_mismatch.size(); i++)
            //    cout<<i<<","<<P_mismatch[i]<<endl;
            //for(unsigned int i=0; i<internal_P_equation_buses.size(); ++i)
            //{
            //    cout<<bus_delta_angle[i]<<endl;
            //    //bus_delta_angle[i] /= abs(psdb.get_bus_positive_sequence_complex_voltage_in_pu(network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i])));
            //}
            update_bus_angle(bus_delta_angle);

            try_to_solve_hvdc_steady_state();
            calculate_raw_bus_power_mismatch();

            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();

            build_bus_Q_power_mismatch_vector_for_decoupled_solution();
            n = internal_Q_equation_buses.size();
            #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                set_openmp_number_of_threads(toolkit->get_thread_number());
                #pragma omp parallel for schedule(static)
            #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            for(unsigned int i=0; i<n; ++i)
            {
                unsigned int internal_bus = internal_Q_equation_buses[i];
                //physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]);
                //Q_mismatch[i] /= psdb.get_bus_positive_sequence_voltage_in_pu(physical_bus);
                Q_mismatch[i] /= get_bus_positive_sequence_voltage_in_pu_with_internal_bus_number(internal_bus);
            }
            bus_delta_voltage = Q_mismatch/BQ;

            update_bus_voltage(bus_delta_voltage);

            iteration_count ++;
        }
        //show_powerflow_result();
    }
}

void POWERFLOW_SOLVER::solve_with_modified_Gaussian_Seidel_solution()
{
    return;
}


void POWERFLOW_SOLVER::initialize_powerflow_solver()
{
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Initializing powerflow solver.");
    toolkit->show_information_with_leading_time_stamp(buffer);

    show_powerflow_solver_configuration();

    NETWORK_MATRIX& network_matrix = get_network_matrix();

    jacobian_builder->set_network_matrix(network_matrix);


    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    psdb.update_in_service_bus_count();

    prepare_devices_for_solution();

    initialize_bus_type();
    initialize_bus_voltage_to_regulate();
    initialize_bus_voltage();
    optimize_bus_numbers();
    iteration_count = 0;
    set_convergence_flag(false);

    unsigned int n_bus = psdb.get_bus_count();
    S_mismatch.reserve(n_bus*2);
    P_mismatch.reserve(n_bus);
    Q_mismatch.reserve(n_bus);

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Done initializing powerflow solver.");
    toolkit->show_information_with_leading_time_stamp(buffer);
}

void POWERFLOW_SOLVER::prepare_devices_for_solution()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    buses = psdb.get_all_buses();
    sources = psdb.get_all_sources();
    generators = psdb.get_all_generators();
    wt_generators = psdb.get_all_wt_generators();
    pv_units = psdb.get_all_pv_units();
    e_storages = psdb.get_all_energy_storages();

    loads = psdb.get_all_loads();

    lines = psdb.get_all_lines();
    transformers = psdb.get_all_transformers();
    hvdcs = psdb.get_all_hvdcs();

    e_devices = psdb.get_all_equivalent_devices();
}

void POWERFLOW_SOLVER::initialize_bus_type()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    //vector<BUS*> buses = psdb.get_all_buses();
    unsigned int nbus = buses.size();

    if(get_flat_start_logic()==true)
    {
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=0; i<nbus; ++i)
        {
            BUS_TYPE btype = buses[i]->get_bus_type();
            if(btype == PV_TO_PQ_TYPE_1 or btype == PV_TO_PQ_TYPE_2 or
               btype == PV_TO_PQ_TYPE_3 or btype == PV_TO_PQ_TYPE_4)
                buses[i]->set_bus_type(PV_TYPE);
        }
    }

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nbus; ++i)
    {
        BUS_TYPE btype = buses[i]->get_bus_type();
        if(btype==PV_TYPE)
        {
            vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(buses[i]->get_bus_number());
            unsigned int nsource = sources.size();
            unsigned int n_inservice = 0;
            for(unsigned int j=0; j!=nsource; ++j)
            {
                if(sources[j]->get_status()==true)
                  ++n_inservice;
            }
            if(n_inservice!=0)
                ;
            else
                buses[i]->set_bus_type(PQ_TYPE);
        }
    }
}

void POWERFLOW_SOLVER::initialize_bus_voltage_to_regulate()
{
    //vector<SOURCE*> sources = psdb.get_all_sources();
    unsigned int nsource = sources.size();
    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nsource; ++i)
    {
        if(sources[i]->get_status()==true)
        {
            double qmax = sources[i]->get_q_max_in_MVar();
            double qmin = sources[i]->get_q_min_in_MVar();
            if(qmax!=qmin)
            {
                //unsigned int bus = sources[i]->get_bus_to_regulate();
                double vreg = sources[i]->get_voltage_to_regulate_in_pu();
                //BUS* busptr = psdb.get_bus(bus);
                BUS* busptr = sources[i]->get_bus_pointer();
                busptr->set_voltage_to_regulate_in_pu(vreg);
            }
        }
    }
    //vector<BUS*> buses = psdb.get_all_buses();
    unsigned int nbus = buses.size();
    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nbus; ++i)
    {
        BUS_TYPE btype = buses[i]->get_bus_type();
        if(btype==PQ_TYPE or btype==OUT_OF_SERVICE)
            continue;
        else // PV, SLACK
        {
            if(fabs(buses[i]->get_voltage_to_regulate_in_pu())<DOUBLE_EPSILON)
                buses[i]->set_voltage_to_regulate_in_pu(1.0);
        }
    }
}

void POWERFLOW_SOLVER::initialize_bus_voltage()
{
    //vector<BUS*> buses = psdb.get_all_buses();
    unsigned int nbus = buses.size();

    if(get_flat_start_logic()==true)
    {
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=0; i<nbus; ++i)
        {
            switch(buses[i]->get_bus_type())
            {
                case PQ_TYPE:
                    buses[i]->set_positive_sequence_voltage_in_pu(1.0);
                    break;
                case OUT_OF_SERVICE:
                    continue;
                default:
                    buses[i]->set_positive_sequence_voltage_in_pu(buses[i]->get_voltage_to_regulate_in_pu());
            }

            buses[i]->set_positive_sequence_angle_in_rad(0.0);
        }
    }

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Initial bus voltage and angle are listed as follows.");
    toolkit->show_information_with_leading_time_stamp(buffer);
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "bus      voltage(pu) angle(deg)");
    toolkit->show_information_with_leading_time_stamp(buffer);
    nbus = nbus>200 ? 200 : nbus;
    for(unsigned int i=0; i!=nbus; ++i)
    {
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %10.6f %10.6f",
                 buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg());
        toolkit->show_information_with_leading_time_stamp(buffer);
    }
}

void POWERFLOW_SOLVER::optimize_bus_numbers()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    network_matrix.optimize_network_ordering();

    set_internal_bus_pointer();

    /*ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    unsigned int n = psdb.get_in_service_bus_count();
    unsigned int ibus=0;
    osstream<<"Powerflow bus number, physical vs internal"<<endl;
    for(unsigned int i=0; i!=n; ++i)
    {
        ibus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        osstream<<ibus<<", "<<i<<endl;
    }
    toolkit->show_information_with_leading_time_stamp(osstream);*/

}

void POWERFLOW_SOLVER::set_internal_bus_pointer()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    unsigned int nbus = psdb.get_in_service_bus_count();

    NETWORK_MATRIX& network_matrix = get_network_matrix();

    internal_bus_pointers.clear();
    for(unsigned int internal_bus=0; internal_bus!=nbus; ++internal_bus)
    {
        unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        internal_bus_pointers.push_back(psdb.get_bus(physical_bus));
    }
}

complex<double> POWERFLOW_SOLVER::get_bus_complex_voltage_in_pu_with_internal_bus_number(unsigned int internal_bus) const
{
    BUS* busptr = internal_bus_pointers[internal_bus];
    return busptr->get_positive_sequence_complex_voltage_in_pu();
}

double POWERFLOW_SOLVER::get_bus_positive_sequence_voltage_in_pu_with_internal_bus_number(unsigned int internal_bus) const
{
    BUS* busptr = internal_bus_pointers[internal_bus];
    return busptr->get_positive_sequence_voltage_in_pu();
}

void POWERFLOW_SOLVER::update_P_and_Q_equation_internal_buses()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Updating powerflow P equation buses and Q equation buses.");
    toolkit->show_information_with_leading_time_stamp(buffer);

    unsigned int nbus = psdb.get_in_service_bus_count();

    internal_P_equation_buses.clear();
    internal_Q_equation_buses.clear();
    internal_P_equation_buses.reserve(nbus);
    internal_Q_equation_buses.reserve(nbus);

    for(unsigned int i=0; i!=nbus; ++i)
    {
        //unsigned int bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        //BUS* busptr = psdb.get_bus(bus);
        BUS* busptr = internal_bus_pointers[i];
        BUS_TYPE btype = busptr->get_bus_type();
        if(btype!=OUT_OF_SERVICE and btype != SLACK_TYPE)
        {
            internal_P_equation_buses.push_back(i);
            if(btype!=PV_TYPE)
                internal_Q_equation_buses.push_back(i);
        }
    }
	/*
    osstream<<"Buses with P equations (physical bus):" << endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    unsigned int n = internal_P_equation_buses.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        osstream<<network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]) << endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    osstream<<"Buses with Q equations (physical bus):"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    n = internal_Q_equation_buses.size();
    for(unsigned int i=0; i!=n; ++i)
    {
        osstream<< network_matrix.get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]) << endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }*/

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Done updating powerflow P equation buses and Q equation buses.");
    toolkit->show_information_with_leading_time_stamp(buffer);
}

void POWERFLOW_SOLVER::set_convergence_flag(bool flag)
{
    converged = flag;
}

bool POWERFLOW_SOLVER::get_convergence_flag() const
{
    return converged;
}

bool POWERFLOW_SOLVER::is_converged()
{
    ostringstream osstream;
    osstream<<"Check maximum active and reactive power mismatch.";
    toolkit->show_information_with_leading_time_stamp(osstream);

    try_to_solve_hvdc_steady_state();
    calculate_raw_bus_power_mismatch();

    double max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
    double max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();

    if(max_P_mismatch_in_MW<get_allowed_max_active_power_imbalance_in_MW() and
       max_Q_mismatch_in_MW<get_allowed_max_reactive_power_imbalance_in_MVar())
        return true;
    else
        return false;

    return get_convergence_flag();
}

void POWERFLOW_SOLVER::try_to_solve_hvdc_steady_state()
{
    //vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    unsigned int nhvdc = hvdcs.size();
    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nhvdc; ++i)
    {
        if(hvdcs[i]->get_status()==true)
        {
            hvdcs[i]->solve_steady_state();
            //hvdcs[i]->show_solved_hvdc_steady_state();
        }
    }
}

void POWERFLOW_SOLVER::calculate_raw_bus_power_mismatch()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int nbus = psdb.get_in_service_bus_count();

    calculate_raw_bus_power_into_network();
    if(toolkit->is_detailed_log_enabled())
    {
        for(unsigned int i=0; i!=nbus; ++i)
        {
            if(std::isnan(bus_power[i].real()) or std::isnan(bus_power[i].imag()))
            {
                osstream<<"after  calculate_raw_bus_power_into_network NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nbus; ++i)
        bus_power[i] = -bus_power[i];

    add_source_to_bus_power_mismatch();
    if(toolkit->is_detailed_log_enabled())
    {
        for(unsigned int i=0; i!=nbus; ++i)
        {
            if(std::isnan(bus_power[i].real()) or std::isnan(bus_power[i].imag()))
            {
                osstream<<"after adding source NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    add_load_to_bus_power_mismatch();
    if(toolkit->is_detailed_log_enabled())
    {
        for(unsigned int i=0; i!=nbus; ++i)
        {
            if(std::isnan(bus_power[i].real()) or std::isnan(bus_power[i].imag()))
            {
                osstream<<"after adding load NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    add_hvdc_to_bus_power_mismatch();
    if(toolkit->is_detailed_log_enabled())
    {
        for(unsigned int i=0; i!=nbus; ++i)
        {
            if(std::isnan(bus_power[i].real()) or std::isnan(bus_power[i].imag()))
            {
                osstream<<"after adding hvdc NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    /*ostringstream osstream;
    osstream<<"Power mismatch of buses.";
    toolkit->show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     Pmismatch(MW) Qmismatch(MVar)";
    toolkit->show_information_with_leading_time_stamp(osstream);

    unsigned int bus;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX* network_matrix = get_network_Y_matrix();
    double sbase = psdb.get_system_base_power_in_MVA();
    for(unsigned int i=0; i!=nbus; ++i)
    {
        bus = network_matrix->get_physical_bus_number_of_internal_bus(i);
        BUS_TYPE btype = psdb.get_bus(bus)->get_bus_type();
        double p = bus_power[i].real()*sbase;
        double q = bus_power[i].imag()*sbase;
        if(btype==PV_TYPE or btype == SLACK_TYPE)
            q = 0.0;
        if(btype==SLACK_TYPE)
            p = 0.0;
        osstream<<setw(6)<<bus<<", "<<setw(8)<<setprecision(6)<<p<<", "<<setw(8)<<setprecision(6)<<q;

        toolkit->show_information_with_leading_time_stamp(osstream);
    }*/
}

void POWERFLOW_SOLVER::calculate_raw_bus_power_into_network()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    calculate_raw_bus_current_into_network();
    bus_power = bus_current;

    unsigned int nbus = psdb.get_in_service_bus_count();
    if(toolkit->is_detailed_log_enabled())
    {
        for(unsigned int i=0; i!=nbus; ++i)
        {
            if(std::isnan(bus_power[i].real()) or std::isnan(bus_power[i].imag()))
            {
                osstream<<"after  calculate_raw_bus_current_into_network NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
    }

    //complex<double> voltage;
    //unsigned int physical_bus_number;

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nbus; ++i)
    {
        //physical_bus_number = network_matrix.get_physical_bus_number_of_internal_bus(i);
        //voltage = psdb.get_bus_positive_sequence_complex_voltage_in_pu(physical_bus_number);
        complex<double> voltage = get_bus_complex_voltage_in_pu_with_internal_bus_number(i);

        bus_power[i] = voltage*conj(bus_current[i]);
    }

    /*ostringstream osstream;
    osstream<<"Power flowing into network (physical bus).");
    toolkit->show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     P(pu)    P(pu)");
    toolkit->show_information_with_leading_time_stamp(osstream);

    unsigned int bus;
    for(unsigned int i=0; i!=nbus; ++i)
    {
        bus = get_physical_bus_number_of_internal_bus(i);
        osstream<<"%-8u %-10f %-10f",bus, bus_power[i].real(), bus_power[i].imag());
        toolkit->show_information_with_leading_time_stamp(osstream);
    }*/
}


void POWERFLOW_SOLVER::calculate_raw_bus_current_into_network()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    const STEPS_COMPLEX_SPARSE_MATRIX& Y = network_matrix.get_network_Y_matrix();

    unsigned int nbus = psdb.get_in_service_bus_count();
    if(bus_current.size()==0)
    {
        bus_current.clear();
        bus_current.reserve(nbus);
        for(unsigned int i=0; i!=nbus; ++i)
            bus_current.push_back(0.0);
    }
    else
    {
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=0; i<nbus; ++i)
            bus_current[i]=0.0;
    }

    complex<double> voltage, y;
    //unsigned int physical_bus_number;

    //unsigned int column_physical_bus;

    int nsize = Y.get_matrix_size();
    int k_start=0;
    for(int column=0; column!=nsize; ++column)
    {
        //column_physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(column);
        //voltage = psdb.get_bus_positive_sequence_complex_voltage_in_pu(column_physical_bus);
        complex<double> voltage = get_bus_complex_voltage_in_pu_with_internal_bus_number(column);

        int k_end = Y.get_starting_index_of_column(column+1);
        for(int k=k_start; k<k_end; ++k)
        {
            int row = Y.get_row_number_of_entry_index(k);
            complex<double> y = Y.get_entry_value(k);
            bus_current[row] += y*voltage;
        }
        k_start = k_end;
    }
}

void POWERFLOW_SOLVER::add_source_to_bus_power_mismatch()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double one_over_sbase = toolkit->get_one_over_system_base_power_in_one_over_MVA();

    unsigned int nsource = sources.size();

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_generator_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nsource; ++i)
    {
        if(sources[i]->get_status() == true)
        {
            unsigned int physical_bus = sources[i]->get_source_bus();

            BUS* busptr = sources[i]->get_bus_pointer();

            complex<double> Sgen;
            switch(busptr->get_bus_type())
            {
                case PV_TYPE:
                    Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), 0.0);
                    break;
                case OUT_OF_SERVICE:
                case SLACK_TYPE:
                    continue;
                default:// PQ, PV2PQ
                    Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), sources[i]->get_q_generation_in_MVar());
                    break;
            }
            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
            bus_power[internal_bus] += (Sgen*one_over_sbase);
        }
    }
}

void POWERFLOW_SOLVER::add_load_to_bus_power_mismatch()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double one_over_sbase = toolkit->get_one_over_system_base_power_in_one_over_MVA();

    unsigned int nload = loads.size();

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_load_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nload; ++i)
    {
        if(loads[i]->get_status() == true)
        {
            unsigned int physical_bus = loads[i]->get_load_bus();
            unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

            complex<double> Sload = loads[i]->get_actual_total_load_in_MVA()*one_over_sbase;

            bus_power[internal_bus] -= Sload;
        }
    }
}

void POWERFLOW_SOLVER::add_hvdc_to_bus_power_mismatch()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double one_over_sbase = toolkit->get_one_over_system_base_power_in_one_over_MVA();

    unsigned int nhvdc = hvdcs.size();

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_hvdc_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nhvdc; ++i)
    {
        if(hvdcs[i]->get_status() == true)
        {
            unsigned int physical_bus_rec = hvdcs[i]->get_converter_bus(RECTIFIER);
            unsigned int internal_bus_rec = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus_rec);

            unsigned int physical_bus_inv = hvdcs[i]->get_converter_bus(INVERTER);
            unsigned int internal_bus_inv = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus_inv);

            //hvdcs[i]->solve_steady_state();
            //hvdcs[i]->show_solved_hvdc_steady_state();

            complex<double> S_rec = complex<double>(hvdcs[i]->get_converter_ac_active_power_in_MW(RECTIFIER), hvdcs[i]->get_converter_ac_reactive_power_in_MVar(RECTIFIER));
            complex<double> S_inv = complex<double>(hvdcs[i]->get_converter_ac_active_power_in_MW(INVERTER), -hvdcs[i]->get_converter_ac_reactive_power_in_MVar(INVERTER));

            //ostringstream osstream;
            //osstream<<hvdcs[i]->get_device_name()<<": Srec = "<<S_rec<<" MVA, Sinv = "<<S_inv<<" MVA."<<endl;
            //toolkit->show_information_with_leading_time_stamp(osstream);

            bus_power[internal_bus_rec] -= S_rec*one_over_sbase;
            bus_power[internal_bus_inv] += S_inv*one_over_sbase;
        }
    }
}

double POWERFLOW_SOLVER::get_maximum_active_power_mismatch_in_MW() const
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = toolkit->get_network_matrix();

    unsigned int nP = internal_P_equation_buses.size();
    unsigned int internal_bus, physical_bus;

    double max_P_error_in_pu = 0.0, max_P_error_in_MW;
    unsigned int max_P_error_physical_bus = 0;
    for(unsigned int i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        if(fabs(bus_power[internal_bus].real()) < max_P_error_in_pu)
            continue;
        else
        {
            physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
            max_P_error_in_pu = fabs(bus_power[internal_bus].real());
            max_P_error_physical_bus = physical_bus;
        }
    }

    max_P_error_in_MW = max_P_error_in_pu*psdb.get_system_base_power_in_MVA();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Maximum   active power mismatch found: %10.6fMW   at bus %u.",
             max_P_error_in_MW,max_P_error_physical_bus);
    toolkit->show_information_with_leading_time_stamp(buffer);

    return max_P_error_in_MW;
}

double POWERFLOW_SOLVER::get_maximum_reactive_power_mismatch_in_MVar() const
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = toolkit->get_network_matrix();

    unsigned int nQ = internal_Q_equation_buses.size();
    unsigned int internal_bus, physical_bus;

    double max_Q_error_in_pu = 0.0, max_Q_error_in_MVar;
    unsigned int max_Q_error_physical_bus = 0;
    for(unsigned int i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        if(fabs(bus_power[internal_bus].imag()) < max_Q_error_in_pu)
            continue;
        else
        {
            physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
            max_Q_error_in_pu = fabs(bus_power[internal_bus].imag());
            max_Q_error_physical_bus = physical_bus;
        }
    }
    max_Q_error_in_MVar =  max_Q_error_in_pu*psdb.get_system_base_power_in_MVA();

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Maximum reactive power mismatch found: %10.6fMVar at bus %u.",
             max_Q_error_in_MVar,max_Q_error_physical_bus);
    toolkit->show_information_with_leading_time_stamp(buffer);

    return max_Q_error_in_MVar;
}

bool POWERFLOW_SOLVER::check_bus_type_constraints()
{
    bool system_bus_type_changed = false;

    unsigned int nbus = buses.size();

    for(unsigned int i=0; i<nbus; ++i)
    {
        BUS_TYPE btype = buses[i]->get_bus_type();
        if(btype == PQ_TYPE or btype == OUT_OF_SERVICE)
            continue;

        unsigned int physical_bus = buses[i]->get_bus_number();

        if(btype==SLACK_TYPE)
        {
            check_SLACK_bus_constraint_of_physical_bus(physical_bus);
            //system_bus_type_changed = false;
        }
        else
        {
            if(btype == PV_TYPE)
            {
                bool bus_type_changed = check_PV_bus_constraint_of_physical_bus(physical_bus);
                if(bus_type_changed)
                    system_bus_type_changed = true;
            }
            else
            {
                bool bus_type_changed = check_PV_TO_PQ_bus_constraint_of_physical_bus(physical_bus);
                if(bus_type_changed)
                    system_bus_type_changed = true;
            }
        }
    }
    return system_bus_type_changed;
}


void POWERFLOW_SOLVER::check_SLACK_bus_constraint_of_physical_bus(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    //BUS* bus = psdb.get_bus(physical_bus);
    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    BUS* bus = internal_bus_pointers[internal_bus];

    if(bus->get_bus_type()==SLACK_TYPE)
    {
        //unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

        double sbase = psdb.get_system_base_power_in_MVA();
        double bus_P_mismatch_in_MW = -bus_power[internal_bus].real()*sbase;
        double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*sbase;

        double total_p_max_in_MW = psdb.get_regulatable_p_max_at_physical_bus_in_MW(physical_bus);
        double total_p_min_in_MW = psdb.get_regulatable_p_min_at_physical_bus_in_MW(physical_bus);
        double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
        double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

        double P_loading_percentage = (bus_P_mismatch_in_MW-total_p_min_in_MW);
            P_loading_percentage /= (total_p_max_in_MW - total_p_min_in_MW);
        double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
            Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

        vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);
        unsigned int n;
        n = sources.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == true)
            {
                double P_loading_in_MW = sources[i]->get_p_max_in_MW() - sources[i]->get_p_min_in_MW();
                P_loading_in_MW = P_loading_in_MW*P_loading_percentage + sources[i]->get_p_min_in_MW();
                sources[i]->set_p_generation_in_MW(P_loading_in_MW);

                double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
                Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
                sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
            }
        }
    }
}

bool POWERFLOW_SOLVER::check_PV_bus_constraint_of_physical_bus(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    BUS* bus = internal_bus_pointers[internal_bus];

    if(bus->get_bus_type()==PV_TYPE)
    {
        char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];

        bool bus_type_changed = false;
        //unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

        double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*psdb.get_system_base_power_in_MVA();

        double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
        double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

        if(total_q_max_in_MVar==total_q_min_in_MVar)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            bus_type_changed = true;
            set_all_sources_at_physical_bus_to_q_max(physical_bus);

            if(toolkit->is_detailed_log_enabled())
            {
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Bus %u changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q max reached.", physical_bus);
                toolkit->show_information_with_leading_time_stamp(buffer);
            }

            return bus_type_changed;
        }

        unsigned int n;

        if(bus_Q_mismatch_in_MVar > total_q_max_in_MVar)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            bus_type_changed = true;

            if(toolkit->is_detailed_log_enabled())
            {
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Bus %u changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q max reached.", physical_bus);
                toolkit->show_information_with_leading_time_stamp(buffer);
            }

            set_all_sources_at_physical_bus_to_q_max(physical_bus);
            return bus_type_changed;
        }

        if(bus_Q_mismatch_in_MVar < total_q_min_in_MVar)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            bus_type_changed = true;

            if(toolkit->is_detailed_log_enabled())
            {
                snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Bus %u changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q min reached.", physical_bus);
                toolkit->show_information_with_leading_time_stamp(buffer);
            }

            set_all_sources_at_physical_bus_to_q_min(physical_bus);
            return bus_type_changed;
        }

        double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
        Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

        vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);
        n = sources.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == true)
            {
                double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
                Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
                sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
            }
        }
        return bus_type_changed;
    }
    else
        return false;
}

bool POWERFLOW_SOLVER::check_PV_TO_PQ_bus_constraint_of_physical_bus(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    BUS* bus = internal_bus_pointers[internal_bus];

    if(bus->get_bus_type()!=PV_TO_PQ_TYPE_1 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_2 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_3 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_4)
        return false;

    ostringstream osstream;

    bool bus_type_changed = false;

    double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
    double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

    if(fabs(total_q_max_in_MVar-total_q_min_in_MVar)>DOUBLE_EPSILON)
        ;
    else
    {
        if(bus->get_bus_type()!=PV_TO_PQ_TYPE_3)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            bus_type_changed = true;
            if(toolkit->is_detailed_log_enabled())
            {
                osstream<<"Bus "<<physical_bus<<" changed from PV_TYPE to PV_TO_PQ_TYPE_3.";
                toolkit->show_information_with_leading_time_stamp(osstream);
            }

            set_all_sources_at_physical_bus_to_q_max(physical_bus);
        }
        return bus_type_changed;
    }



    double bus_voltage = bus->get_positive_sequence_voltage_in_pu();

    double voltage_to_regulated = psdb.get_voltage_to_regulate_of_physical_bus_in_pu(physical_bus);

    double total_q_generation_in_MVar = psdb.get_total_regulating_q_generation_at_physical_bus_in_MVar(physical_bus);

    //unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*psdb.get_system_base_power_in_MVA();

    //bus_Q_mismatch_in_MVar += total_q_generation_in_MVar;

    //os<<"Bus %u: current V= %fpu, V to regulate: %fpu, current Q= %fMVar", physical_bus, bus_voltage, voltage_to_regulated, total_q_generation_in_MVar);
    //show_information_with_leading_time_stamp(osstream);
    //os<<"Qmax= %fMVar, Qmin= %fMVar", total_q_max_in_MVar, total_q_min_in_MVar);
    //show_information_with_leading_time_stamp(osstream);

    if(bus_voltage>voltage_to_regulated)
    {
        if(total_q_generation_in_MVar>total_q_min_in_MVar)
        {
            switch(bus->get_bus_type())
            {
                case PV_TO_PQ_TYPE_4:
                    bus->set_bus_type(PV_TO_PQ_TYPE_3);
                    //os<<"Bus %u changed from PV_TO_PQ_TYPE_4 to PV_TO_PQ_TYPE_3.", physical_bus);
                    //show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_3:
                    bus->set_bus_type(PV_TO_PQ_TYPE_2);
                    //os<<"Bus %u changed from PV_TO_PQ_TYPE_3 to PV_TO_PQ_TYPE_2.", physical_bus);
                    //show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_2:
                    bus->set_bus_type(PV_TO_PQ_TYPE_1);
                    //os<<"Bus %u changed from PV_TO_PQ_TYPE_2 to PV_TO_PQ_TYPE_1.", physical_bus);
                    //show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_1:
                    bus->set_bus_type(PV_TYPE);
                    bus_type_changed = true;
                    bus->set_positive_sequence_voltage_in_pu(voltage_to_regulated);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else // bus_voltage<voltage_to_regulated
    {
        if(total_q_generation_in_MVar<total_q_max_in_MVar)
        {
            switch(bus->get_bus_type())
            {
                case PV_TO_PQ_TYPE_4:
                    bus->set_bus_type(PV_TO_PQ_TYPE_3);
                    //os<<"Bus %u changed from PV_TO_PQ_TYPE_4 to PV_TO_PQ_TYPE_3.", physical_bus);
                    //show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_3:
                    bus->set_bus_type(PV_TO_PQ_TYPE_2);
                    //os<<"Bus %u changed from PV_TO_PQ_TYPE_3 to PV_TO_PQ_TYPE_2.", physical_bus);
                    //show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_2:
                    bus->set_bus_type(PV_TO_PQ_TYPE_1);
                    //os<<"Bus %u changed from PV_TO_PQ_TYPE_2 to PV_TO_PQ_TYPE_1.", physical_bus);
                    //show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_1:
                    bus->set_bus_type(PV_TYPE);
                    bus_type_changed = true;
                    bus->set_positive_sequence_voltage_in_pu(voltage_to_regulated);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return bus_type_changed;

    //
    if(bus_voltage<voltage_to_regulated)
    {
        if(bus_Q_mismatch_in_MVar>total_q_max_in_MVar)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            set_all_sources_at_physical_bus_to_q_max(physical_bus);
            osstream<<"Var of sources at bus "<<physical_bus<<" are set to max.";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            switch(bus->get_bus_type())
            {
                case PV_TO_PQ_TYPE_4:
                    bus->set_bus_type(PV_TO_PQ_TYPE_3);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_4 to PV_TO_PQ_TYPE_3.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                case PV_TO_PQ_TYPE_3:
                    bus->set_bus_type(PV_TO_PQ_TYPE_2);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_3 to PV_TO_PQ_TYPE_2.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                case PV_TO_PQ_TYPE_2:
                    bus->set_bus_type(PV_TO_PQ_TYPE_1);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_2 to PV_TO_PQ_TYPE_1.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                case PV_TO_PQ_TYPE_1:
                    bus->set_bus_type(PV_TYPE);
                    bus_type_changed = true;
                    bus->set_positive_sequence_voltage_in_pu(voltage_to_regulated);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        if(bus_Q_mismatch_in_MVar<total_q_min_in_MVar)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            set_all_sources_at_physical_bus_to_q_min(physical_bus);
            osstream<<"Var of sources at bus "<<physical_bus<<" are set to min.";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            switch(bus->get_bus_type())
            {
                case PV_TO_PQ_TYPE_4:
                    bus->set_bus_type(PV_TO_PQ_TYPE_3);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_4 to PV_TO_PQ_TYPE_3.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                case PV_TO_PQ_TYPE_3:
                    bus->set_bus_type(PV_TO_PQ_TYPE_2);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_3 to PV_TO_PQ_TYPE_2.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                case PV_TO_PQ_TYPE_2:
                    bus->set_bus_type(PV_TO_PQ_TYPE_1);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_2 to PV_TO_PQ_TYPE_1.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                case PV_TO_PQ_TYPE_1:
                    bus->set_bus_type(PV_TYPE);
                    bus_type_changed = true;
                    bus->set_positive_sequence_voltage_in_pu(voltage_to_regulated);
                    if(toolkit->is_detailed_log_enabled())
                    {
                        osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return bus_type_changed;
}

void POWERFLOW_SOLVER::set_all_sources_at_physical_bus_to_q_min(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);

    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        sources[i]->set_q_generation_in_MVar(sources[i]->get_q_min_in_MVar());
}
void POWERFLOW_SOLVER::set_all_sources_at_physical_bus_to_q_max(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);

    unsigned int n = sources.size();
    for(unsigned int i=0; i!=n; ++i)
        sources[i]->set_q_generation_in_MVar(sources[i]->get_q_max_in_MVar());
}


void POWERFLOW_SOLVER::update_source_power_without_constraints()
{
    unsigned int nbus = buses.size();

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nbus; ++i)
    {
        BUS_TYPE btype = buses[i]->get_bus_type();
        if(btype == PQ_TYPE or btype == OUT_OF_SERVICE)
            continue;

        unsigned int physical_bus = buses[i]->get_bus_number();

        if(btype==SLACK_TYPE)
            update_SLACK_bus_source_power_of_physical_bus(physical_bus);
        else
        {
            if(btype == PV_TYPE)
                update_PV_bus_source_power_of_physical_bus(physical_bus);
            else
                continue;
        }
    }
    return;
}

void POWERFLOW_SOLVER::update_SLACK_bus_source_power_of_physical_bus(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    //BUS* bus = psdb.get_bus(physical_bus);
    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    BUS* bus = internal_bus_pointers[internal_bus];

    if(bus->get_bus_type()==SLACK_TYPE)
    {
        //unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

        double sbase = psdb.get_system_base_power_in_MVA();
        double bus_P_mismatch_in_MW = -bus_power[internal_bus].real()*sbase;
        double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*sbase;

        double total_p_max_in_MW = psdb.get_regulatable_p_max_at_physical_bus_in_MW(physical_bus);
        double total_p_min_in_MW = psdb.get_regulatable_p_min_at_physical_bus_in_MW(physical_bus);
        double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
        double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

        double P_loading_percentage = (bus_P_mismatch_in_MW-total_p_min_in_MW);
            P_loading_percentage /= (total_p_max_in_MW - total_p_min_in_MW);
        double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
            Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

        vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);
        unsigned int n;
        n = sources.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == true)
            {
                double P_loading_in_MW = sources[i]->get_p_max_in_MW() - sources[i]->get_p_min_in_MW();
                P_loading_in_MW = P_loading_in_MW*P_loading_percentage + sources[i]->get_p_min_in_MW();
                sources[i]->set_p_generation_in_MW(P_loading_in_MW);

                double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
                Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
                sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
            }
        }
    }
}

void POWERFLOW_SOLVER::update_PV_bus_source_power_of_physical_bus(unsigned int physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    //BUS* bus = psdb.get_bus(physical_bus);
    unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    BUS* bus = internal_bus_pointers[internal_bus];

    if(bus->get_bus_type()==PV_TYPE)
    {
        //unsigned int internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

        double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*psdb.get_system_base_power_in_MVA();

        double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
        double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

        unsigned int n;

        double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
        Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

        vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);
        n = sources.size();
        for(unsigned int i=0; i!=n; ++i)
        {
            if(sources[i]->get_status() == true)
            {
                double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
                Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
                sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
            }
        }
    }
}


void POWERFLOW_SOLVER::build_bus_power_mismatch_vector_for_coupled_solution()
{
    unsigned int nP = internal_P_equation_buses.size();
    unsigned int nQ = internal_Q_equation_buses.size();

    S_mismatch.resize(nP+nQ);

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nP; ++i)
    {
        unsigned int internal_bus = internal_P_equation_buses[i];
        //s_mismatch.push_back(-bus_power[internal_bus].real());
        S_mismatch[i] = -bus_power[internal_bus].real();
    }
    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nQ; ++i)
    {
        unsigned int internal_bus = internal_Q_equation_buses[i];
        //s_mismatch.push_back(-bus_power[internal_bus].imag());
        S_mismatch[nP+i] = -bus_power[internal_bus].imag();
    }
}

void POWERFLOW_SOLVER::build_bus_P_power_mismatch_vector_for_decoupled_solution()
{
    unsigned int nP = internal_P_equation_buses.size();

    P_mismatch.resize(nP);

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nP; ++i)
    {
        unsigned int internal_bus = internal_P_equation_buses[i];
        P_mismatch[i] = -bus_power[internal_bus].real();
        //cout<<"Pmismatch vector, "<<i<<", "<<-bus_power[internal_bus].real()<<endl;
    }
}

void POWERFLOW_SOLVER::build_bus_Q_power_mismatch_vector_for_decoupled_solution()
{
    unsigned int nQ = internal_Q_equation_buses.size();

    Q_mismatch.resize(nQ);

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nQ; ++i)
    {
        unsigned int internal_bus = internal_Q_equation_buses[i];
        Q_mismatch[i] = -bus_power[internal_bus].imag();
        //cout<<"Qmismatch vector, "<<i<<", "<<-bus_power[internal_bus].imag()<<endl;
    }
}

void POWERFLOW_SOLVER::update_bus_voltage_and_angle(vector<double>& update)
{
    ostringstream osstream;
    unsigned int nP = internal_P_equation_buses.size();
    unsigned int nQ = internal_Q_equation_buses.size();

    double max_dangle=0.0, max_dv = 0.0;
    unsigned int nv = update.size();
    for(unsigned int i=0; i!=nP; ++i)
    {
        if(max_dangle<abs(update[i]))
            max_dangle = abs(update[i]);
    }
    for(unsigned int i=nP; i!=nv; ++i)
    {
        if(max_dv<abs(update[i]))
            max_dv = abs(update[i]);
    }
    osstream<<"Maximum angle   change is: "<<max_dangle<<" rad ("<<rad2deg(max_dangle)<<" deg).\n"
            <<"Maximum voltage change is: "<<max_dv<<" pu.";
    toolkit->show_information_with_leading_time_stamp(osstream);

    double limit = get_maximum_angle_change_in_rad();
    if(max_dangle>limit)
    {
        double scale = limit/max_dangle;
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=0; i<nP; ++i)
        {
            update[i] *= scale;
        }
    }
    limit = get_maximum_voltage_change_in_pu();
    if(max_dv>limit)
    {
        double scale = limit/max_dv;
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=nP; i<nv; ++i)
        {
            update[i] *= scale;
        }
    }

    double Perror0 = get_maximum_active_power_mismatch_in_MW();
    double Qerror0 = get_maximum_reactive_power_mismatch_in_MVar();

    double alpha = get_iteration_accelerator();
    double alpha_P = alpha;
    double alpha_Q = alpha;

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nP; ++i)
    {
        unsigned int internal_bus = internal_P_equation_buses[i];
        //unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        //BUS* bus = psdb.get_bus(physical_bus);
        BUS* bus = internal_bus_pointers[internal_bus];

        double current_angle = bus->get_positive_sequence_angle_in_rad();

        double delta_angle = update[i];

        bus->set_positive_sequence_angle_in_rad(current_angle + alpha_P*delta_angle);
    }

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nQ; ++i)
    {
        unsigned int internal_bus = internal_Q_equation_buses[i];
        //unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        //BUS* bus = psdb.get_bus(physical_bus);
        BUS* bus = internal_bus_pointers[internal_bus];

        double current_voltage = bus->get_positive_sequence_voltage_in_pu();

        double delta_voltage = update[nP+i];

        bus->set_positive_sequence_voltage_in_pu(current_voltage + alpha_Q*delta_voltage);
    }

    if(get_non_divergent_solution_logic()==true)
    {
        for(unsigned int i=0; i<10; ++i)
        {
            try_to_solve_hvdc_steady_state();
            calculate_raw_bus_power_mismatch();
            double Perror = get_maximum_active_power_mismatch_in_MW();
            double Qerror = get_maximum_reactive_power_mismatch_in_MVar();
            bool P_is_worse=Perror>Perror0 and Perror>get_allowed_max_active_power_imbalance_in_MW();
            bool Q_is_worse=Qerror>Qerror0 and Qerror>get_allowed_max_reactive_power_imbalance_in_MVar();
            if(not (P_is_worse or Q_is_worse))
                break;
            else
            {
                if(P_is_worse)
                {
                    alpha_P *= 0.5;
                    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                        set_openmp_number_of_threads(toolkit->get_thread_number());
                        #pragma omp parallel for schedule(static)
                    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                    for(unsigned int i=0; i<nP; ++i)
                    {
                        unsigned int internal_bus = internal_P_equation_buses[i];
                        //unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
                        //BUS* bus = psdb.get_bus(physical_bus);
                        BUS* bus = internal_bus_pointers[internal_bus];

                        double current_angle = bus->get_positive_sequence_angle_in_rad();

                        double delta_angle = update[i];

                        bus->set_positive_sequence_angle_in_rad(current_angle - alpha_P*delta_angle);
                    }
                }
                if(Q_is_worse)
                {
                    alpha_Q *= 0.5;
                    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                        set_openmp_number_of_threads(toolkit->get_thread_number());
                        #pragma omp parallel for schedule(static)
                    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                    for(unsigned int i=0; i<nQ; ++i)
                    {
                        unsigned int internal_bus = internal_Q_equation_buses[i];
                        //unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
                        //BUS* bus = psdb.get_bus(physical_bus);
                        BUS* bus = internal_bus_pointers[internal_bus];

                        double current_voltage = bus->get_positive_sequence_voltage_in_pu();

                        double delta_voltage = update[nP+i];

                        bus->set_positive_sequence_voltage_in_pu(current_voltage - alpha_Q*delta_voltage);
                    }
                }
            }
        }
    }
}

void POWERFLOW_SOLVER::update_bus_voltage(vector<double>& update)
{
    ostringstream osstream;
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double limit = get_maximum_voltage_change_in_pu();
    if(fabs(limit)<DOUBLE_EPSILON)
        return;

    double max_dv=0.0;
    unsigned int max_delta_v_bus = 0;
    unsigned int nv = update.size();
    for(unsigned int i=0; i!=nv; ++i)
    {
        if(max_dv<abs(update[i]))
        {
            max_dv = abs(update[i]);
            unsigned int internal_bus = internal_Q_equation_buses[i];
            max_delta_v_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        }
    }
    osstream<<"Maximum voltage change is: "<<max_dv<<" pu at physical bus "<<max_delta_v_bus;
    toolkit->show_information_with_leading_time_stamp(osstream);

    if(max_dv>limit)
    {
        double scale = limit/max_dv;
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=0; i<nv; ++i)
        {
            update[i] *= scale;
        }
    }

    double Qerror0 = get_maximum_reactive_power_mismatch_in_MVar();

    //unsigned int nP = internal_P_equation_buses.size();
    unsigned int nQ = internal_Q_equation_buses.size();

    double alpha = get_iteration_accelerator();

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nQ; ++i)
    {
        unsigned int internal_bus = internal_Q_equation_buses[i];
        //BUS* bus = psdb.get_bus(physical_bus);
        BUS* bus = internal_bus_pointers[internal_bus];

        double current_voltage = bus->get_positive_sequence_voltage_in_pu();

        double delta_voltage = update[i];

        bus->set_positive_sequence_voltage_in_pu(current_voltage + alpha*delta_voltage);
    }

    if(get_non_divergent_solution_logic()==true)
    {
        for(unsigned int i=0; i<10; ++i)
        {
            try_to_solve_hvdc_steady_state();
            calculate_raw_bus_power_mismatch();
            double Qerror = get_maximum_reactive_power_mismatch_in_MVar();
            if(Qerror>Qerror0 and Qerror>get_allowed_max_reactive_power_imbalance_in_MVar())
            {
                alpha *= 0.5;
                #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                    set_openmp_number_of_threads(toolkit->get_thread_number());
                    #pragma omp parallel for schedule(static)
                #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                for(unsigned int i=0; i<nQ; ++i)
                {
                    unsigned int internal_bus = internal_Q_equation_buses[i];
                    //unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
                    //BUS* bus = psdb.get_bus(physical_bus);
                    BUS* bus = internal_bus_pointers[internal_bus];

                    double current_voltage = bus->get_positive_sequence_voltage_in_pu();

                    double delta_voltage = update[i];

                    bus->set_positive_sequence_voltage_in_pu(current_voltage - alpha*delta_voltage);
                }
            }
            else
                break;
        }
    }

}


void POWERFLOW_SOLVER::update_bus_angle(vector<double>& update)
{
    ostringstream osstream;
    NETWORK_MATRIX& network_matrix = get_network_matrix();

    double limit = get_maximum_angle_change_in_rad();
    if(fabs(limit)<DOUBLE_EPSILON)
        return;

    double max_dv=0.0;
    unsigned int max_delta_angle_bus = 0;
    unsigned int nv = update.size();
    for(unsigned int i=0; i!=nv; ++i)
    {
        if(max_dv<abs(update[i]))
        {
            max_dv = abs(update[i]);
            unsigned int internal_bus = internal_P_equation_buses[i];
            max_delta_angle_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
        }
    }
    osstream<<"Maximum angle   change is: "<<max_dv<<" rad ("<<rad2deg(max_dv)<<" deg) at physical bus "<<max_delta_angle_bus;
    toolkit->show_information_with_leading_time_stamp(osstream);

    if(max_dv>limit)
    {
        double scale = limit/max_dv;
        #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
            set_openmp_number_of_threads(toolkit->get_thread_number());
            #pragma omp parallel for schedule(static)
        #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        for(unsigned int i=0; i<nv; ++i)
        {
            update[i] *= scale;
        }
    }

    double Perror0 = get_maximum_active_power_mismatch_in_MW();

    unsigned int nP = internal_P_equation_buses.size();

    double alpha = get_iteration_accelerator();

    #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
        set_openmp_number_of_threads(toolkit->get_thread_number());
        #pragma omp parallel for schedule(static)
    #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
    for(unsigned int i=0; i<nP; ++i)
    {
        unsigned int internal_bus = internal_P_equation_buses[i];
        //BUS* bus = psdb.get_bus(physical_bus);
        BUS* bus = internal_bus_pointers[internal_bus];

        double current_angle = bus->get_positive_sequence_angle_in_rad();

        double delta_angle = update[i];

        bus->set_positive_sequence_angle_in_rad(current_angle + alpha*delta_angle);
    }


    if(get_non_divergent_solution_logic()==true)
    {
        for(unsigned int i=0; i<10; ++i)
        {
            try_to_solve_hvdc_steady_state();
            calculate_raw_bus_power_mismatch();
            double Perror = get_maximum_active_power_mismatch_in_MW();
            if(Perror>Perror0 and Perror>get_allowed_max_active_power_imbalance_in_MW())
            {
                alpha *= 0.5;
                #ifdef ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                    set_openmp_number_of_threads(toolkit->get_thread_number());
                    #pragma omp parallel for schedule(static)
                #endif // ENABLE_OPENMP_FOR_POWERFLOW_SOLVER
                for(unsigned int i=0; i<nP; ++i)
                {
                    unsigned int internal_bus = internal_P_equation_buses[i];
                    //unsigned int physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
                    //BUS* bus = psdb.get_bus(physical_bus);
                    BUS* bus = internal_bus_pointers[internal_bus];

                    double current_angle = bus->get_positive_sequence_angle_in_rad();

                    double delta_angle = update[i];

                    bus->set_positive_sequence_angle_in_rad(current_angle - alpha*delta_angle);
                }
            }
            else
                break;
        }
    }
}

void POWERFLOW_SOLVER::show_powerflow_result()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    ostringstream osstream;

    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];

    if(is_converged())
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow converged within %u iterations. Results are listed as follows.",
                get_iteration_count());
    else
        snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow failed to converge within %u iterations. Results of the last iteration are listed as follows.",
                get_iteration_count());
    toolkit->show_information_with_leading_time_stamp(buffer);

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Solved bus voltage and angle:");
    toolkit->show_information_with_leading_time_stamp(buffer);

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "bus      voltage(pu)     angle(deg)");
    toolkit->show_information_with_leading_time_stamp(buffer);

    NETWORK_MATRIX& network_matrix = toolkit->get_network_matrix();

    //vector<BUS*> buses = psdb.get_all_buses();
    unsigned int nbus = buses.size();
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)>=1000.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<1000.0 and fabs(smismatch)>=500.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<500.0 and fabs(smismatch)>=400.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<400.0 and fabs(smismatch)>=300.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<300.0 and fabs(smismatch)>=200.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<200.0 and fabs(smismatch)>=100.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<100.0 and fabs(smismatch)>=10.0)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<10.0 and fabs(smismatch)>=get_allowed_max_active_power_imbalance_in_MW())
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s (error: %-8.3f MW + %-8.3f MVar)",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str(), smismatch.real(), smismatch.imag());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }
    for(unsigned int i=0; i!=nbus; ++i)
    {
        unsigned int bus = buses[i]->get_bus_number();
        unsigned int inbus = network_matrix.get_internal_bus_number_of_physical_bus(bus);
        complex<double> smismatch = bus_power[inbus]*sbase;
        if(fabs(smismatch)<get_allowed_max_active_power_imbalance_in_MW())
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u %12.6f %12.6f %s",
                    buses[i]->get_bus_number(),buses[i]->get_positive_sequence_voltage_in_pu(),buses[i]->get_positive_sequence_angle_in_deg(),(buses[i]->get_bus_name()).c_str());
            toolkit->show_information_with_leading_time_stamp(buffer);
        }
    }

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Solved machine power:");
    toolkit->show_information_with_leading_time_stamp(buffer);

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "  bus      id       P(MW)        Q(MVar)");
    toolkit->show_information_with_leading_time_stamp(buffer);

    //vector<SOURCE*> sources = psdb.get_all_sources();
    unsigned int nsource = sources.size();
    for(unsigned int i=0; i!=nsource; ++i)
    {
        BUS_TYPE btype = (sources[i]->get_bus_pointer())->get_bus_type();
        if(btype!=SLACK_TYPE)
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u '%4s'  %12.6f  %12.6f  %-30s[%s]",
                     sources[i]->get_source_bus(),(sources[i]->get_identifier()).c_str(),
                     sources[i]->get_p_generation_in_MW(),sources[i]->get_q_generation_in_MVar(), (sources[i]->get_device_name()).c_str(), psdb.bus_number2bus_name(sources[i]->get_source_bus()).c_str());
        }
        else
        {
            snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "%8u '%4s'  %12.6f  %12.6f  %-30s[%s] [slack bus]",
                     sources[i]->get_source_bus(),(sources[i]->get_identifier()).c_str(),
                     sources[i]->get_p_generation_in_MW(),sources[i]->get_q_generation_in_MVar(), (sources[i]->get_device_name()).c_str(), psdb.bus_number2bus_name(sources[i]->get_source_bus()).c_str());
        }
        toolkit->show_information_with_leading_time_stamp(buffer);
    }
}
void POWERFLOW_SOLVER::save_powerflow_result_to_file(const string& filename) const
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ostringstream osstream;

    ofstream file(filename);
    if(file.is_open())
    {
        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);

        char time_stamp[40];
        char buffer[1000];
        snprintf(time_stamp,40, "%d-%02d-%02d %02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

        file<<"% Powerflow result exported at "<<time_stamp<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_information()).c_str());
        file<<"% "<<buffer<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_additional_information()).c_str());
        file<<"% "<<buffer<<endl;
        //vector<BUS*> buses = psdb.get_all_buses();
        unsigned int nbus = buses.size();
        if(nbus>0)
        {
            file<<"% Bus"<<endl;
            file<<"BUS,NAME,VBASE/KV,VOLTAGE/PU,ANGLE/DEG,VOLTAGE/KV,ANGLE/RAD"<<endl;
            for(unsigned int i=0; i!=nbus; ++i)
            {
                snprintf(buffer, 1000, "%u,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f",
                         buses[i]->get_bus_number(),(buses[i]->get_bus_name()).c_str(),
                         buses[i]->get_base_voltage_in_kV(),
                         buses[i]->get_positive_sequence_voltage_in_pu(), buses[i]->get_positive_sequence_angle_in_deg(),
                         buses[i]->get_positive_sequence_voltage_in_kV(), buses[i]->get_positive_sequence_angle_in_rad());
                file<<buffer<<endl;
            }
        }

        //vector<GENERATOR*> generators = psdb.get_all_generators();
        unsigned int ngen = generators.size();
        if(ngen>0)
        {
            file<<"% Generator"<<endl;
            file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=ngen; ++i)
            {
                snprintf(buffer, 1000, "%u,\"%s\",%.6f,%.6f,%.6f",
                         generators[i]->get_generator_bus(),(generators[i]->get_identifier()).c_str(),
                         generators[i]->get_p_generation_in_MW(), generators[i]->get_q_generation_in_MVar(),
                         (generators[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
        unsigned int nsource = wt_generators.size();
        if(nsource>0)
        {
            file<<"% WT generator"<<endl;
            file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=nsource; ++i)
            {
                snprintf(buffer, 1000, "%u,\"%s\",%.6f,%.6f,%.6f",
                         wt_generators[i]->get_generator_bus(),(wt_generators[i]->get_identifier()).c_str(),
                         wt_generators[i]->get_p_generation_in_MW(), wt_generators[i]->get_q_generation_in_MVar(),
                         (wt_generators[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        unsigned int npv = pv_units.size();
        if(npv>0)
        {
            file<<"% PV unit"<<endl;
            file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=npv; ++i)
            {
                snprintf(buffer, 1000, "%u,\"%s\",%.6f,%.6f,%.6f",
                         pv_units[i]->get_unit_bus(),(pv_units[i]->get_identifier()).c_str(),
                         pv_units[i]->get_p_generation_in_MW(), pv_units[i]->get_q_generation_in_MVar(),
                         (pv_units[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<LOAD*> loads = psdb.get_all_loads();
        unsigned int nload = loads.size();
        if(nload>0)
        {
            file<<"% Load"<<endl;
            file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=nload; ++i)
            {
                complex<double> s = loads[i]->get_actual_total_load_in_MVA();
                snprintf(buffer, 1000, "%u,\"%s\",%.6f,%.6f,%.6f",
                         loads[i]->get_load_bus(),(loads[i]->get_identifier()).c_str(),
                         s.real(), s.imag(),
                         (loads[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<LINE*> lines = psdb.get_all_lines();
        unsigned int nline = lines.size();
        if(nline>0)
        {
            file<<"% Line"<<endl;
            file<<"IBUS,JBUS,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,II/KA,IJ/KA"<<endl;
            for(unsigned int i=0; i!=nline; ++i)
            {
                complex<double> si = lines[i]->get_line_complex_power_at_sending_side_in_MVA();
                complex<double> sj = lines[i]->get_line_complex_power_at_receiving_side_in_MVA();
                snprintf(buffer, 1000, "%u,%u,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                         lines[i]->get_sending_side_bus(),lines[i]->get_receiving_side_bus(),(lines[i]->get_identifier()).c_str(),
                         si.real(), si.imag(),sj.real(), sj.imag(),
                         steps_fast_complex_abs(lines[i]->get_line_complex_current_at_sending_side_in_kA()),
                         steps_fast_complex_abs(lines[i]->get_line_complex_current_at_receiving_side_in_kA()));
                file<<buffer<<endl;
            }
        }

        //vector<TRANSFORMER*> transformers = psdb.get_all_transformers();
        unsigned int ntrans = transformers.size();
        if(ntrans>0)
        {
            file<<"% Transformer"<<endl;
            file<<"IBUS,JBUS,KBUS,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,PK/MW,QK/MVAR,KI/PU,KJ/PU,KK/PU"<<endl;
            for(unsigned int i=0; i!=ntrans; ++i)
            {
                complex<double> sp = transformers[i]->get_winding_complex_power_in_MVA(PRIMARY_SIDE);
                complex<double> ss = transformers[i]->get_winding_complex_power_in_MVA(SECONDARY_SIDE);
                complex<double> st = transformers[i]->get_winding_complex_power_in_MVA(TERTIARY_SIDE);
                snprintf(buffer, 1000, "%u,%u,%u,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                         transformers[i]->get_winding_bus(PRIMARY_SIDE),transformers[i]->get_winding_bus(SECONDARY_SIDE),
                         transformers[i]->get_winding_bus(TERTIARY_SIDE), (transformers[i]->get_identifier()).c_str(),
                         sp.real(), sp.imag(), ss.real(), ss.imag(), st.real(), st.imag(),
                         transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE),
                         transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE),
                         transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE));
                file<<buffer<<endl;
            }
        }

        //vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
        unsigned int nhvdc = hvdcs.size();
        if(nhvdc>0)
        {
            file<<"% Hvdc"<<endl;
            file<<"IBUS,JBUS,ID,PR/MW,QR/MVAR,PI/MW,QI/MVAR,ALPHA/DEG,GAMMA/DEG,VDCR/KV,VDCI/KV,IDC/KA,VACR/PU,VACI/PU,KR/PU,KI/PU"<<endl;
            for(unsigned int i=0; i!=nhvdc; ++i)
            {
                snprintf(buffer, 1000, "%u,%u,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                         hvdcs[i]->get_converter_bus(RECTIFIER),hvdcs[i]->get_converter_bus(INVERTER),(hvdcs[i]->get_identifier()).c_str(),
                         hvdcs[i]->get_converter_ac_active_power_in_MW(RECTIFIER),
                         hvdcs[i]->get_converter_ac_reactive_power_in_MVar(RECTIFIER),
                         hvdcs[i]->get_converter_ac_active_power_in_MW(INVERTER),
                         hvdcs[i]->get_converter_ac_reactive_power_in_MVar(INVERTER),
                         hvdcs[i]->get_converter_alpha_or_gamma_in_deg(RECTIFIER),
                         hvdcs[i]->get_converter_alpha_or_gamma_in_deg(INVERTER),
                         hvdcs[i]->get_converter_dc_voltage_in_kV(RECTIFIER),
                         hvdcs[i]->get_converter_dc_voltage_in_kV(INVERTER),
                         hvdcs[i]->get_converter_dc_current_in_kA(RECTIFIER),
                         (hvdcs[i]->get_bus_pointer(RECTIFIER))->get_positive_sequence_voltage_in_pu(),
                         (hvdcs[i]->get_bus_pointer(INVERTER))->get_positive_sequence_voltage_in_pu(),
                         hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER),
                         hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER));
                file<<buffer<<endl;
            }
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving powerflow result to file."<<endl
          <<"No powerflow result will be exported.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void POWERFLOW_SOLVER::save_extended_powerflow_result_to_file(const string& filename) const
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    ostringstream osstream;

    ofstream file(filename);
    if(file.is_open())
    {
        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);

        char time_stamp[40];
        char buffer[1000];
        snprintf(time_stamp,40, "%d-%02d-%02d %02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

        file<<"% Powerflow result exported at "<<time_stamp<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_information()).c_str());
        file<<"% "<<buffer<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_additional_information()).c_str());
        file<<"% "<<buffer<<endl;
        //vector<BUS*> buses = psdb.get_all_buses();
        unsigned int nbus = buses.size();
        if(nbus>0)
        {
            file<<"% Bus"<<endl;
            file<<"BUS,NAME,VBASE/KV,VOLTAGE/PU,ANGLE/DEG,VOLTAGE/KV,ANGLE/RAD"<<endl;
            for(unsigned int i=0; i!=nbus; ++i)
            {
                snprintf(buffer, 1000, "%u,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f",
                         buses[i]->get_bus_number(),(buses[i]->get_bus_name()).c_str(),
                         buses[i]->get_base_voltage_in_kV(),
                         buses[i]->get_positive_sequence_voltage_in_pu(), buses[i]->get_positive_sequence_angle_in_deg(),
                         buses[i]->get_positive_sequence_voltage_in_kV(), buses[i]->get_positive_sequence_angle_in_rad());
                file<<buffer<<endl;
            }
        }

        //vector<GENERATOR*> generators = psdb.get_all_generators();
        unsigned int ngen = generators.size();
        if(ngen>0)
        {
            file<<"% Generator"<<endl;
            file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=ngen; ++i)
            {
                unsigned int bus = generators[i]->get_generator_bus();
                snprintf(buffer, 1000, "%u,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                         bus, psdb.bus_number2bus_name(bus).c_str(),
                         (generators[i]->get_identifier()).c_str(),
                         generators[i]->get_p_generation_in_MW(), generators[i]->get_q_generation_in_MVar(),
                         (generators[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
        unsigned int nsource = wt_generators.size();
        if(nsource>0)
        {
            file<<"% WT generator"<<endl;
            file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=nsource; ++i)
            {
                unsigned int bus = wt_generators[i]->get_source_bus();
                snprintf(buffer, 1000, "%u,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                         bus, psdb.bus_number2bus_name(bus).c_str(),
                         (wt_generators[i]->get_identifier()).c_str(),
                         wt_generators[i]->get_p_generation_in_MW(), wt_generators[i]->get_q_generation_in_MVar(),
                         (wt_generators[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<PV_UNIT*> pv_units = psdb.get_all_pv_units();
        unsigned int npv = pv_units.size();
        if(npv>0)
        {
            file<<"% PV unit"<<endl;
            file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=npv; ++i)
            {
                unsigned int bus = pv_units[i]->get_unit_bus();
                snprintf(buffer, 1000, "%u,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                         bus, psdb.bus_number2bus_name(bus).c_str(),
                         (pv_units[i]->get_identifier()).c_str(),
                         pv_units[i]->get_p_generation_in_MW(), pv_units[i]->get_q_generation_in_MVar(),
                         (pv_units[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<LOAD*> loads = psdb.get_all_loads();
        unsigned int nload = loads.size();
        if(nload>0)
        {
            file<<"% Load"<<endl;
            file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
            for(unsigned int i=0; i!=nload; ++i)
            {
                unsigned int bus = loads[i]->get_load_bus();
                complex<double> s = loads[i]->get_actual_total_load_in_MVA();
                snprintf(buffer, 1000, "%u,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                         bus, psdb.bus_number2bus_name(bus).c_str(),
                         (loads[i]->get_identifier()).c_str(),
                         s.real(), s.imag(),
                         (loads[i]->get_bus_pointer())->get_positive_sequence_voltage_in_pu());
                file<<buffer<<endl;
            }
        }

        //vector<LINE*> lines = psdb.get_all_lines();
        unsigned int nline = lines.size();
        if(nline>0)
        {
            file<<"% Line"<<endl;
            file<<"IBUS,INAME,JBUS,JNAME,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,II/KA,IJ/KA"<<endl;
            for(unsigned int i=0; i!=nline; ++i)
            {
                unsigned int ibus = lines[i]->get_sending_side_bus();
                unsigned int jbus = lines[i]->get_receiving_side_bus();
                complex<double> si = lines[i]->get_line_complex_power_at_sending_side_in_MVA();
                complex<double> sj = lines[i]->get_line_complex_power_at_receiving_side_in_MVA();
                snprintf(buffer, 1000, "%u,\"%s\",%u,\"%s\",\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                         ibus, psdb.bus_number2bus_name(ibus).c_str(),
                         jbus, psdb.bus_number2bus_name(jbus).c_str(),
                         (lines[i]->get_identifier()).c_str(),
                         si.real(), si.imag(),sj.real(), sj.imag(),
                         steps_fast_complex_abs(lines[i]->get_line_complex_current_at_sending_side_in_kA()),
                         steps_fast_complex_abs(lines[i]->get_line_complex_current_at_receiving_side_in_kA()));
                file<<buffer<<endl;
            }
        }

        //vector<TRANSFORMER*> transformers = psdb.get_all_transformers();
        unsigned int ntrans = transformers.size();
        if(ntrans>0)
        {
            file<<"% Transformer"<<endl;
            file<<"IBUS,INAME,JBUS,JNAME,KBUS,KNAME,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,PK/MW,QK/MVAR,KI/PU,KJ/PU,KK/PU"<<endl;
            for(unsigned int i=0; i!=ntrans; ++i)
            {
                unsigned int ibus = transformers[i]->get_winding_bus(PRIMARY_SIDE);
                unsigned int jbus = transformers[i]->get_winding_bus(SECONDARY_SIDE);
                unsigned int kbus = transformers[i]->get_winding_bus(TERTIARY_SIDE);

                complex<double> sp = transformers[i]->get_winding_complex_power_in_MVA(PRIMARY_SIDE);
                complex<double> ss = transformers[i]->get_winding_complex_power_in_MVA(SECONDARY_SIDE);
                complex<double> st = transformers[i]->get_winding_complex_power_in_MVA(TERTIARY_SIDE);
                snprintf(buffer, 1000, "%u,\"%s\",%u,\"%s\",%u,\"%s\",\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                         ibus, psdb.bus_number2bus_name(ibus).c_str(),
                         jbus, psdb.bus_number2bus_name(jbus).c_str(),
                         kbus, psdb.bus_number2bus_name(kbus).c_str(),
                         (transformers[i]->get_identifier()).c_str(),
                         sp.real(), sp.imag(), ss.real(), ss.imag(), st.real(), st.imag(),
                         transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE),
                         transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE),
                         transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE));
                file<<buffer<<endl;
            }
        }

        //vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
        unsigned int nhvdc = hvdcs.size();
        if(nhvdc>0)
        {
            file<<"% Hvdc"<<endl;
            file<<"IBUS,INAME,JBUS,JNAME,ID,PR/MW,QR/MVAR,PI/MW,QI/MVAR,ALPHA/DEG,GAMMA/DEG,VDCR/KV,VDCI/KV,IDC/KA,VACR/PU,VACI/PU,KR/PU,KI/PU"<<endl;
            for(unsigned int i=0; i!=nhvdc; ++i)
            {
                unsigned int busr = hvdcs[i]->get_converter_bus(RECTIFIER);
                unsigned int busi = hvdcs[i]->get_converter_bus(INVERTER);
                snprintf(buffer, 1000, "%u,\"%s\",%u,\"%s\",\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                         busr, psdb.bus_number2bus_name(busr).c_str(),
                         busi, psdb.bus_number2bus_name(busi).c_str(),
                         (hvdcs[i]->get_identifier()).c_str(),
                         hvdcs[i]->get_converter_ac_active_power_in_MW(RECTIFIER),
                         hvdcs[i]->get_converter_ac_reactive_power_in_MVar(RECTIFIER),
                         hvdcs[i]->get_converter_ac_active_power_in_MW(INVERTER),
                         hvdcs[i]->get_converter_ac_reactive_power_in_MVar(INVERTER),
                         hvdcs[i]->get_converter_alpha_or_gamma_in_deg(RECTIFIER),
                         hvdcs[i]->get_converter_alpha_or_gamma_in_deg(INVERTER),
                         hvdcs[i]->get_converter_dc_voltage_in_kV(RECTIFIER),
                         hvdcs[i]->get_converter_dc_voltage_in_kV(INVERTER),
                         hvdcs[i]->get_converter_dc_current_in_kA(RECTIFIER),
                         (hvdcs[i]->get_bus_pointer(RECTIFIER))->get_positive_sequence_voltage_in_pu(),
                         (hvdcs[i]->get_bus_pointer(INVERTER))->get_positive_sequence_voltage_in_pu(),
                         hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER),
                         hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER));
                file<<buffer<<endl;
            }
        }

        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving extended powerflow result to file."<<endl
          <<"No powerflow result will be exported.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void POWERFLOW_SOLVER::save_network_Y_matrix_to_file(const string& filename) const
{
    NETWORK_MATRIX& network_matrix = toolkit->get_network_matrix();
    network_matrix.save_network_Y_matrix_to_file(filename);
}

void POWERFLOW_SOLVER::save_jacobian_matrix_to_file(const string& filename)
{
    jacobian_builder->build_seprate_jacobians();

    jacobian_builder->save_jacobian_matrix_to_file(filename);
}

void POWERFLOW_SOLVER::save_bus_powerflow_result_to_file(const string& filename) const
{
    ostringstream osstream;
    ofstream file(filename);
    if(file.is_open())
    {
        file<<"BUS,VOLTAGE,ANGLE"<<endl;

        //vector<BUS*> buses = psdb.get_all_buses();
        unsigned int nbus = buses.size();
        for(unsigned int i=0; i!=nbus; ++i)
        {
            file<<buses[i]->get_bus_number()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_positive_sequence_voltage_in_pu()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_positive_sequence_angle_in_deg()<<endl;
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving bus powerflow result to file."<<endl
          <<"No bus powerflow result will be exported.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

unsigned int POWERFLOW_SOLVER::get_iteration_count() const
{
    return iteration_count;
}

unsigned int POWERFLOW_SOLVER::get_memory_usage_in_bytes()
{
    return jacobian.get_memory_usage_in_bytes()+
           BP.get_memory_usage_in_bytes()+
           BQ.get_memory_usage_in_bytes()+

           bus_active_power_mismatch_in_pu.capacity()*sizeof(double)+
           bus_reactive_power_mismatch_in_pu.capacity()*sizeof(double)+

           internal_P_equation_buses.capacity()*sizeof(unsigned int)+
           internal_Q_equation_buses.capacity()*sizeof(unsigned int)+

           S_mismatch.capacity()*sizeof(double)+
           P_mismatch.capacity()*sizeof(double)+
           Q_mismatch.capacity()*sizeof(double)+

           bus_current.capacity()*sizeof(complex<double>)+
           bus_power.capacity()*sizeof(complex<double>)+

           buses.capacity()*sizeof(BUS*)+
           sources.capacity()*sizeof(SOURCE*)+
           generators.capacity()*sizeof(GENERATOR*)+
           wt_generators.capacity()*sizeof(WT_GENERATOR*)+
           pv_units.capacity()*sizeof(PV_UNIT*)+
           e_storages.capacity()*sizeof(ENERGY_STORAGE*)+
           loads.capacity()*sizeof(LOAD*)+
           lines.capacity()*sizeof(LINE*)+
           transformers.capacity()*sizeof(TRANSFORMER*)+
           hvdcs.capacity()*sizeof(HVDC*)+
           e_devices.capacity()*sizeof(EQUIVALENT_DEVICE*)+


           internal_bus_pointers.capacity()*sizeof(BUS*);
}
