#include "header/toolkit/powerflow_solver/powerflow_solver.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/network/jacobian_builder.h"
#include <cstdio>
#include <istream>
#include <iostream>
#include <fstream>
using namespace std;

POWERFLOW_SOLVER::POWERFLOW_SOLVER()
{
    set_flat_start_logic(false);
    set_transformer_tap_adjustment_logic(true);
    set_non_divergent_solution_logic(true);
    set_export_jacobian_matrix_step_by_step_logic(false);

    set_allowed_max_active_power_imbalance_in_MW(0.001);
    set_allowed_max_reactive_power_imbalance_in_MVar(0.001);

    set_convergence_flag(false);

    set_max_iteration(50);

    bus_current.clear();
    bus_power.clear();

    iteration_count = 0;
    set_iteration_accelerator(1.0);
}

POWERFLOW_SOLVER::~POWERFLOW_SOLVER()
{
    ;
}

NETWORK_MATRIX& POWERFLOW_SOLVER::get_network_matrix()
{
    return network_matrix;
}

void POWERFLOW_SOLVER::set_max_iteration(size_t iteration)
{
    this->max_iteration = iteration;
}

size_t POWERFLOW_SOLVER::get_max_iteration() const
{
    return max_iteration;
}

void POWERFLOW_SOLVER::set_allowed_max_active_power_imbalance_in_MW(double P)
{
    if(P>0.0)
        P_threshold_in_MW = P;
    else
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Non-positive (%fMW) is not allowed for setting convergence threshold of"
                 "active power imbalance for powerflow solution.\n0.01MW will be set automatically.", P);
        show_information_with_leading_time_stamp(buffer);
        P_threshold_in_MW = 0.01;
    }
}

void POWERFLOW_SOLVER::set_allowed_max_reactive_power_imbalance_in_MVar(double Q)
{
    if(Q>0.0)
        Q_threshold_in_MVar = Q;
    else
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Non-positive (%fMVar) is not allowed for setting convergence threshold of"
                 "reactive power imbalance for powerflow solution.\n0.01MVar will be set automatically.", Q);
        show_information_with_leading_time_stamp(buffer);
        Q_threshold_in_MVar = 0.01;
    }
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
    if(iter_alpha<=0.0)
        this->alpha = 1.0;
    else
        this->alpha = iter_alpha;
}

void POWERFLOW_SOLVER::set_non_divergent_solution_logic(bool logic)
{
    non_divergent_solution_enabled = logic;
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

bool POWERFLOW_SOLVER::get_export_jacobian_matrix_step_by_step_logic() const
{
    return export_jacobian_matrix_step_by_step;
}

void POWERFLOW_SOLVER::solve_with_full_Newton_Raphson_solution()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    if(psdb.get_bus_count()==0)
        return;


    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Start solve powerflow with Full Newton Raphson solution.");
    show_information_with_leading_time_stamp(buffer);

    initialize_powerflow_solver();

    double max_P_mismatch_in_MW, max_Q_mismatch_in_MW;
    vector<double> bus_power_mismatch, bus_delta_voltage_angle;
    JACOBIAN_BUILDER jacobian_builder;
    jacobian_builder.set_network_matrix(network_matrix);

    network_matrix.build_network_matrix();
    //network_matrix.report_network_matrix();

    update_P_and_Q_equation_internal_buses();
    jacobian_builder.build_seprate_jacobians();
    //jacobian_builder.show_seprate_jacobians();

    while(true)
    {
        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Iteration %lu:",iteration_count);
        show_information_with_leading_time_stamp(buffer);

        try_to_solve_hvdc_steady_state();

        calculate_raw_bus_power_mismatch();

        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();


        bool bus_type_changed = check_bus_type_constraints();

        if(bus_type_changed)
        {
            update_P_and_Q_equation_internal_buses();

            calculate_raw_bus_power_mismatch();

            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();
        }
        if(max_P_mismatch_in_MW < get_allowed_max_active_power_imbalance_in_MW() and
           max_Q_mismatch_in_MW < get_allowed_max_reactive_power_imbalance_in_MVar())
        {
            set_convergence_flag(true);

            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow converged within %lu iterations.",iteration_count);
            show_information_with_leading_time_stamp(buffer);
            break;
        }
        else
            set_convergence_flag(false);

        if(get_iteration_count()>=get_max_iteration())
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow failed to converge within %lu iterations.",get_max_iteration());
            show_information_with_leading_time_stamp(buffer);
            break;
        }

        bus_power_mismatch = get_bus_power_mismatch_vector_for_coupled_solution();

        jacobian_builder.update_seprate_jacobians();

        jacobian = jacobian_builder.get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(internal_P_equation_buses,
                                                                                                   internal_Q_equation_buses);

        if(get_export_jacobian_matrix_step_by_step_logic()==true)
        {
            jacobian.save_matrix_to_file("Jacobian-NR-Iter-"+num2str(get_iteration_count())+".csv");
        }
        bus_delta_voltage_angle = bus_power_mismatch/jacobian;

        update_bus_voltage_and_angle(bus_delta_voltage_angle);

		++iteration_count;
    }
    show_powerflow_result();
}

void POWERFLOW_SOLVER::solve_with_fast_decoupled_solution()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    if(psdb.get_bus_count()==0)
        return;

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Start solve powerflow with Fast Decoupled solution.");
    show_information_with_leading_time_stamp(buffer);

    initialize_powerflow_solver();

    double max_P_mismatch_in_MW, max_Q_mismatch_in_MW;
    vector<double> P_power_mismatch, Q_power_mismatch, bus_delta_voltage, bus_delta_angle;
    JACOBIAN_BUILDER jacobian_builder;
    jacobian_builder.set_network_matrix(network_matrix);

    network_matrix.build_network_matrix();

    network_matrix.build_decoupled_network_matrix();
    //const SPARSE_MATRIX& Y = network_matrix.get_network_matrix();
    //cout<<"Y matrix identity is: "<<get_sparse_matrix_identity(Y)<<endl;

    update_P_and_Q_equation_internal_buses();
    BP = jacobian_builder.get_decoupled_B_jacobian_with_P_equation_internal_buses(internal_P_equation_buses);
    BQ = jacobian_builder.get_decoupled_B_jacobian_with_Q_equation_internal_buses(internal_Q_equation_buses);
    //BP.LU_factorization(1, 1e-6);
    //BQ.LU_factorization(1, 1e-6);
    //BP.report_brief();
    //BQ.report_brief();

    size_t physical_bus;

    while(true)
    {

        char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Iteration %lu:",iteration_count);
        show_information_with_leading_time_stamp(buffer);

        try_to_solve_hvdc_steady_state();

        calculate_raw_bus_power_mismatch();
        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();

        bool bus_type_changed = check_bus_type_constraints();
        if(bus_type_changed)
        {
            update_P_and_Q_equation_internal_buses();
            //BP = jacobian_builder.get_decoupled_B_jacobian_with_P_equation_internal_buses(internal_P_equation_buses);
            BQ = jacobian_builder.get_decoupled_B_jacobian_with_Q_equation_internal_buses(internal_Q_equation_buses);
            BQ.LU_factorization(1, 1e-6);

            calculate_raw_bus_power_mismatch();
            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();
            //continue;
        }

        if(get_export_jacobian_matrix_step_by_step_logic()==true)
        {
            BP.save_matrix_to_file("Jacobian-BP-PQ-Iter-"+num2str(get_iteration_count())+".csv");
            BQ.save_matrix_to_file("Jacobian-BQ-PQ-Iter-"+num2str(get_iteration_count())+".csv");
        }

        if(max_P_mismatch_in_MW < get_allowed_max_active_power_imbalance_in_MW() and
           max_Q_mismatch_in_MW < get_allowed_max_reactive_power_imbalance_in_MVar())
        {
            set_convergence_flag(true);

            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow converged within %lu iterations.",iteration_count);
            show_information_with_leading_time_stamp(buffer);
            break;
        }
        else
            set_convergence_flag(false);

        if(get_iteration_count()>=get_max_iteration())
        {
            snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow failed to converge within %lu iterations.",get_max_iteration());
            show_information_with_leading_time_stamp(buffer);
            break;
        }

        P_power_mismatch = get_bus_P_power_mismatch_vector_for_decoupled_solution();
        size_t n = internal_P_equation_buses.size();
        for(size_t i=0; i!=n; ++i)
        {
            physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]);
            P_power_mismatch[i] /= psdb.get_bus_voltage_in_pu(physical_bus);
        }
        bus_delta_angle = P_power_mismatch/BP;
        //BP.report_brief();
        //for(size_t i=0; i<P_power_mismatch.size(); i++)
        //    cout<<i<<","<<P_power_mismatch[i]<<endl;
        //for(size_t i=0; i<internal_P_equation_buses.size(); ++i)
        //{
        //    cout<<bus_delta_angle[i]<<endl;
        //    //bus_delta_angle[i] /= abs(psdb.get_bus_complex_voltage_in_pu(network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i])));
        //}
        update_bus_angle(bus_delta_angle);

        calculate_raw_bus_power_mismatch();

        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();

        Q_power_mismatch = get_bus_Q_power_mismatch_vector_for_decoupled_solution();
        n = internal_Q_equation_buses.size();
        for(size_t i=0; i!=n; ++i)
        {
            physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]);
            Q_power_mismatch[i] /= psdb.get_bus_voltage_in_pu(physical_bus);
        }
        bus_delta_voltage = Q_power_mismatch/BQ;

        update_bus_voltage(bus_delta_voltage);

        iteration_count ++;
    }
    show_powerflow_result();
}

void POWERFLOW_SOLVER::solve_with_modified_Gaussian_Seidel_solution()
{
    return;
}


void POWERFLOW_SOLVER::initialize_powerflow_solver()
{
    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Initializing powerflow solver.");
    show_information_with_leading_time_stamp(buffer);


    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    psdb.update_in_service_bus_count();

    initialize_bus_type();
    initialize_bus_voltage_to_regulate();
    initialize_bus_voltage();
    optimize_bus_numbers();
    iteration_count = 0;
    set_convergence_flag(false);

    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Done initializing powerflow solver.");
    show_information_with_leading_time_stamp(buffer);
}

void POWERFLOW_SOLVER::initialize_bus_type()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    bool flat_flag = get_flat_start_logic();

    vector<BUS*> buses = psdb.get_all_buses();

    size_t nbus = buses.size();
    for(size_t i=0; i!=nbus; ++i)
    {
        BUS_TYPE btype = buses[i]->get_bus_type();
        if(flat_flag==true)
            if(btype == PV_TO_PQ_TYPE_1 or btype == PV_TO_PQ_TYPE_2 or
               btype == PV_TO_PQ_TYPE_3 or btype == PV_TO_PQ_TYPE_4)
                buses[i]->set_bus_type(PV_TYPE);
        if(btype==PV_TYPE)
        {
            vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(buses[i]->get_bus_number());
            size_t nsource = sources.size();
            size_t n_inservice = 0;
            for(size_t j=0; j!=nsource; ++j)
                if(sources[j]->get_status()==true)
                    n_inservice++;
            if(n_inservice==0)
                buses[i]->set_bus_type(PQ_TYPE);
        }
    }
}

void POWERFLOW_SOLVER::initialize_bus_voltage_to_regulate()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<SOURCE*> sources = psdb.get_all_sources();
    size_t nsource = sources.size();
    for(size_t i=0; i!=nsource; ++i)
    {
        if(sources[i]->get_status()==false)
            continue;

        double qmax = sources[i]->get_q_max_in_MVar();
        double qmin = sources[i]->get_q_min_in_MVar();
        if(qmax!=qmin)
        {
            size_t bus = sources[i]->get_bus_to_regulate();
            double vreg = sources[i]->get_voltage_to_regulate_in_pu();
            BUS* busptr = psdb.get_bus(bus);
            busptr->set_voltage_to_regulate_in_pu(vreg);
        }
    }
    vector<BUS*> buses = psdb.get_all_buses();
    size_t nbus = buses.size();
    for(size_t i=0; i!=nbus; ++i)
    {
        BUS_TYPE btype = buses[i]->get_bus_type();
        if(btype!=PQ_TYPE and  btype!=OUT_OF_SERVICE)
        {
            if(fabs(buses[i]->get_voltage_to_regulate_in_pu())<FLOAT_EPSILON)
                buses[i]->set_voltage_to_regulate_in_pu(1.0);
        }
    }
}

void POWERFLOW_SOLVER::initialize_bus_voltage()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    vector<BUS*> buses;

    buses = psdb.get_all_buses();
    size_t nbus = buses.size();

    if(get_flat_start_logic()==true)
    {
        BUS_TYPE btype;
        for(size_t i=0; i!=nbus; ++i)
        {
            btype = buses[i]->get_bus_type();
            if(btype == OUT_OF_SERVICE)
                continue;

            if(btype==PQ_TYPE)
                buses[i]->set_voltage_in_pu(1.0);
            else
                buses[i]->set_voltage_in_pu(buses[i]->get_voltage_to_regulate_in_pu());

            buses[i]->set_angle_in_rad(0.0);
        }
    }

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Initial bus voltage and angle are listed as follows.");
    show_information_with_leading_time_stamp(buffer);
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "bus      voltage(pu) angle(deg)");
    show_information_with_leading_time_stamp(buffer);
    if(nbus>200)
        nbus = 200;
    for(size_t i=0; i!=nbus; ++i)
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%8lu %10.6f %10.6f",
                 buses[i]->get_bus_number(),buses[i]->get_voltage_in_pu(),buses[i]->get_angle_in_deg());
        show_information_with_leading_time_stamp(buffer);
    }
}

void POWERFLOW_SOLVER::optimize_bus_numbers()
{
    network_matrix.optimize_network_ordering();
    /*ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    size_t n = psdb.get_in_service_bus_count();
    size_t ibus=0;
    osstream<<"Powerflow bus number, physical vs internal"<<endl;
    for(size_t i=0; i!=n; ++i)
    {
        ibus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        osstream<<ibus<<", "<<i<<endl;
    }
    show_information_with_leading_time_stamp(osstream);*/

}

void POWERFLOW_SOLVER::update_P_and_Q_equation_internal_buses()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Updating powerflow P equation buses and Q equation buses.");
    show_information_with_leading_time_stamp(buffer);

    size_t nbus = psdb.get_in_service_bus_count();

    internal_P_equation_buses.clear();
    internal_Q_equation_buses.clear();
    internal_P_equation_buses.reserve(nbus);
    internal_Q_equation_buses.reserve(nbus);

    size_t bus;


    BUS_TYPE btype;
    BUS* busptr;
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = network_matrix.get_physical_bus_number_of_internal_bus(i);
        busptr = psdb.get_bus(bus);
        btype = busptr->get_bus_type();
        if(btype!=OUT_OF_SERVICE and btype != SLACK_TYPE)
        {
            internal_P_equation_buses.push_back(i);
            if(btype!=PV_TYPE)
                internal_Q_equation_buses.push_back(i);
        }
    }
	/*
    osstream<<"Buses with P equations (physical bus):" << endl;
    show_information_with_leading_time_stamp(osstream);
    size_t n = internal_P_equation_buses.size();
    for(size_t i=0; i!=n; ++i)
    {
        osstream<<network_matrix.get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]) << endl;
        show_information_with_leading_time_stamp(osstream);
    }
    osstream<<"Buses with Q equations (physical bus):"<<endl;
    show_information_with_leading_time_stamp(osstream);
    n = internal_Q_equation_buses.size();
    for(size_t i=0; i!=n; ++i)
    {
        osstream<< network_matrix.get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]) << endl;
        show_information_with_leading_time_stamp(osstream);
    }*/

    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Done updating powerflow P equation buses and Q equation buses.");
    show_information_with_leading_time_stamp(buffer);
}

void POWERFLOW_SOLVER::set_convergence_flag(bool flag)
{
    converged = flag;
}

bool POWERFLOW_SOLVER::get_convergence_flag() const
{
    return converged;
}

bool POWERFLOW_SOLVER::is_converged() const
{
    return get_convergence_flag();
}

void POWERFLOW_SOLVER::try_to_solve_hvdc_steady_state()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t nhvdc = hvdcs.size();
    for(size_t i=0; i!=nhvdc; ++i)
    {
        if(hvdcs[i]->get_status() == true)
        {
            hvdcs[i]->solve_steady_state();
            //hvdcs[i]->show_solved_hvdc_steady_state();
        }
    }
}

void POWERFLOW_SOLVER::calculate_raw_bus_power_mismatch()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    calculate_raw_bus_power_into_network();

    size_t nbus = psdb.get_in_service_bus_count();
    for(size_t i=0; i!=nbus; ++i)
        bus_power[i] = -bus_power[i];

    add_source_to_bus_power_mismatch();

    for(size_t i=0; i!=nbus; ++i)
    {
        if(isnan(bus_power[i].real()) or isnan(bus_power[i].imag()))
            cout<<"after adding source NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
    }
    add_load_to_bus_power_mismatch();

    for(size_t i=0; i!=nbus; ++i)
    {
        if(isnan(bus_power[i].real()) or isnan(bus_power[i].imag()))
            cout<<"after adding load NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
    }
    add_hvdc_to_bus_power_mismatch();

    for(size_t i=0; i!=nbus; ++i)
    {
        if(isnan(bus_power[i].real()) or isnan(bus_power[i].imag()))
            cout<<"after adding hvdc NAN is detected at bus "<<network_matrix.get_physical_bus_number_of_internal_bus(i)<<endl;
    }

    /*ostringstream osstream;
    osstream<<"Power mismatch of buses.";
    show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     Pmismatch(MW) Qmismatch(MVar)";
    show_information_with_leading_time_stamp(osstream);

    size_t bus;
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    NETWORK_MATRIX* network_matrix = get_network_matrix();
    double sbase = psdb.get_system_base_power_in_MVA();
    for(size_t i=0; i!=nbus; ++i)
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

        show_information_with_leading_time_stamp(osstream);
    }*/
}

void POWERFLOW_SOLVER::calculate_raw_bus_power_into_network()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    calculate_raw_bus_current_into_network();

    size_t nbus = psdb.get_in_service_bus_count();

    bus_power = bus_current;

    complex<double> voltage;
    size_t physical_bus_number;

    for(size_t i=0; i!=nbus; ++i)
    {
        physical_bus_number = network_matrix.get_physical_bus_number_of_internal_bus(i);
        voltage = psdb.get_bus_complex_voltage_in_pu(physical_bus_number);

        bus_power[i] = voltage*conj(bus_current[i]);
    }

    /*ostringstream osstream;
    osstream<<"Power flowing into network (physical bus).");
    show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     P(pu)    P(pu)");
    show_information_with_leading_time_stamp(osstream);

    size_t bus;
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = get_physical_bus_number_of_internal_bus(i);
        osstream<<"%-8u %-10f %-10f",bus, bus_power[i].real(), bus_power[i].imag());
        show_information_with_leading_time_stamp(osstream);
    }*/
}


void POWERFLOW_SOLVER::calculate_raw_bus_current_into_network()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    const SPARSE_MATRIX& Y = network_matrix.get_network_matrix();

    size_t nbus = psdb.get_in_service_bus_count();
    bus_current.clear();
    bus_current.reserve(nbus);
    for(size_t i=0; i!=nbus; ++i)
        bus_current.push_back(0.0);

    complex<double> voltage, y;
    //size_t physical_bus_number;
    int row;
    size_t column_physical_bus;

    int nsize = Y.get_matrix_size();
    int k_start=0, k_end=0;
    for(int column=0; column!=nsize; ++column)
    {
        column_physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(column);
        voltage = psdb.get_bus_complex_voltage_in_pu(column_physical_bus);

        k_end = Y.get_starting_index_of_column(column+1);
        for(int k=k_start; k!=k_end; ++k)
        {
            row = Y.get_row_number_of_entry_index(k);
            y = Y.get_entry_value(k);
            bus_current[row] += y*voltage;
        }
        k_start = k_end;
    }

    /*ostringstream osstream;
    osstream<<"Current flowing into network (physical bus).");
    show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     Ireal(pu)    Iimag(pu)");
    show_information_with_leading_time_stamp(osstream);

    size_t bus;
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = get_physical_bus_number_of_internal_bus(i);
        osstream<<"%-8u %-10f %-10f",bus, bus_current[i].real(), bus_current[i].imag());
        show_information_with_leading_time_stamp(osstream);
    }*/
}
void POWERFLOW_SOLVER::add_source_to_bus_power_mismatch()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    double Sbase = psdb.get_system_base_power_in_MVA();

    size_t physical_bus, internal_bus;

    bool status;

    BUS_TYPE btype;

    complex<double> Sgen;

    vector<SOURCE*> sources = psdb.get_all_sources();

    BUS* busptr;

    size_t nsource = sources.size();

    for(size_t i=0; i!=nsource; ++i)
    {
        status = sources[i]->get_status();
        if(status == false)
            continue;

        physical_bus = sources[i]->get_source_bus();

        busptr = psdb.get_bus(physical_bus);

        btype = busptr->get_bus_type();
        if(btype == OUT_OF_SERVICE or btype == SLACK_TYPE)
            continue;

        if(btype == PV_TYPE)
            Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), 0.0);
        else// PQ, PV2PQ
            Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), sources[i]->get_q_generation_in_MVar());

        internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
        bus_power[internal_bus] += (Sgen/Sbase);
    }
}

void POWERFLOW_SOLVER::add_load_to_bus_power_mismatch()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    double Sbase = psdb.get_system_base_power_in_MVA();

    size_t physical_bus, internal_bus;

    bool status;

    vector<LOAD*> loads = psdb.get_all_loads();

    size_t nload = loads.size();

    complex<double> Sload;

    for(size_t i=0; i!=nload; ++i)
    {
        status = loads[i]->get_status();
        if(status == false)
            continue;

        physical_bus = loads[i]->get_load_bus();
        internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

        Sload = loads[i]->get_actual_total_load_in_MVA()/Sbase;

        bus_power[internal_bus] -= Sload;
    }
}

void POWERFLOW_SOLVER::add_hvdc_to_bus_power_mismatch()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    double Sbase = psdb.get_system_base_power_in_MVA();

    size_t physical_bus_rec, internal_bus_rec,
           physical_bus_inv, internal_bus_inv;

    bool status;

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();

    size_t nhvdc = hvdcs.size();

    complex<double> S_rec, S_inv;

    for(size_t i=0; i!=nhvdc; ++i)
    {
        status = hvdcs[i]->get_status();
        if(status == false)
            continue;

        physical_bus_rec = hvdcs[i]->get_converter_bus(RECTIFIER);
        internal_bus_rec = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus_rec);

        physical_bus_inv = hvdcs[i]->get_converter_bus(INVERTER);
        internal_bus_inv = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus_inv);

        //hvdcs[i]->solve_steady_state();
        //hvdcs[i]->show_solved_hvdc_steady_state();

        S_rec = complex<double>(hvdcs[i]->get_converter_ac_active_power_in_MW(RECTIFIER), hvdcs[i]->get_converter_ac_reactive_power_in_MVar(RECTIFIER));
        S_inv = complex<double>(hvdcs[i]->get_converter_ac_active_power_in_MW(INVERTER), -hvdcs[i]->get_converter_ac_reactive_power_in_MVar(INVERTER));

        //ostringstream osstream;
        //osstream<<hvdcs[i]->get_device_name()<<": Srec = "<<S_rec<<" MVA, Sinv = "<<S_inv<<" MVA."<<endl;
        //show_information_with_leading_time_stamp(osstream);

        bus_power[internal_bus_rec] -= S_rec/Sbase;
        bus_power[internal_bus_inv] += S_inv/Sbase;
    }
}

double POWERFLOW_SOLVER::get_maximum_active_power_mismatch_in_MW() const
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t nP = internal_P_equation_buses.size();
    size_t internal_bus, physical_bus;

    double max_P_error_in_pu = 0.0, max_P_error_in_MW;
    size_t max_P_error_physical_bus = 0;
    for(size_t i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        if(fabs(bus_power[internal_bus].real()) > max_P_error_in_pu)
        {
            physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
            max_P_error_in_pu = fabs(bus_power[internal_bus].real());
            max_P_error_physical_bus = physical_bus;
        }
    }

    max_P_error_in_MW = max_P_error_in_pu*psdb.get_system_base_power_in_MVA();

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Maximum   active power mismatch found: %10.6fMW   at bus %lu.",
             max_P_error_in_MW,max_P_error_physical_bus);
    show_information_with_leading_time_stamp(buffer);

    return max_P_error_in_MW;
}

double POWERFLOW_SOLVER::get_maximum_reactive_power_mismatch_in_MVar() const
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t nQ = internal_Q_equation_buses.size();
    size_t internal_bus, physical_bus;

    double max_Q_error_in_pu = 0.0, max_Q_error_in_MVar;
    size_t max_Q_error_physical_bus = 0;
    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        if(fabs(bus_power[internal_bus].imag()) > max_Q_error_in_pu)
        {
            physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);
            max_Q_error_in_pu = fabs(bus_power[internal_bus].imag());
            max_Q_error_physical_bus = physical_bus;
        }
    }
    max_Q_error_in_MVar =  max_Q_error_in_pu*psdb.get_system_base_power_in_MVA();

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Maximum reactive power mismatch found: %10.6fMVar at bus %lu.",
             max_Q_error_in_MVar,max_Q_error_physical_bus);
    show_information_with_leading_time_stamp(buffer);

    return max_Q_error_in_MVar;
}

bool POWERFLOW_SOLVER::check_bus_type_constraints()
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    size_t physical_bus;
    BUS_TYPE btype;

    vector<size_t> index;

    bool system_bus_type_changed = false;
    bool bus_type_changed;

    vector<BUS*> buses = psdb.get_all_buses();

    size_t nbus = buses.size();

    for(size_t i=0; i!=nbus; ++i)
    {
        btype = buses[i]->get_bus_type();
        if(btype == PQ_TYPE or btype == OUT_OF_SERVICE)
            continue;

        physical_bus = buses[i]->get_bus_number();

        if(btype==SLACK_TYPE)
        {
            check_SLACK_bus_constraint_of_physical_bus(physical_bus);
            system_bus_type_changed = false;
        }
        else
        {
            if(btype == PV_TYPE)
            {
                bus_type_changed = check_PV_bus_constraint_of_physical_bus(physical_bus);
                if(bus_type_changed)
                    system_bus_type_changed = true;
            }
            else
            {
                bus_type_changed = check_PV_TO_PQ_bus_constraint_of_physical_bus(physical_bus);
                if(bus_type_changed)
                    system_bus_type_changed = true;
            }
        }
    }
    return system_bus_type_changed;
}


void POWERFLOW_SOLVER::check_SLACK_bus_constraint_of_physical_bus(size_t physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    BUS* bus = psdb.get_bus(physical_bus);

    if(bus->get_bus_type()!=SLACK_TYPE)
        return;

    size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

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
    size_t n;
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        double P_loading_in_MW = sources[i]->get_p_max_in_MW() - sources[i]->get_p_min_in_MW();
        P_loading_in_MW = P_loading_in_MW*P_loading_percentage + sources[i]->get_p_min_in_MW();
        sources[i]->set_p_generation_in_MW(P_loading_in_MW);

        double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
        Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
        sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
    }
}

bool POWERFLOW_SOLVER::check_PV_bus_constraint_of_physical_bus(size_t physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    BUS* bus = psdb.get_bus(physical_bus);

    if(bus->get_bus_type()!=PV_TYPE)
        return false;

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];

    bool bus_type_changed = false;
    size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);

    double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*psdb.get_system_base_power_in_MVA();

    double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
    double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

    if(total_q_max_in_MVar==total_q_min_in_MVar)
    {
        bus->set_bus_type(PV_TO_PQ_TYPE_3);
        bus_type_changed = true;
        set_all_sources_at_physical_bus_to_q_max(physical_bus);

        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Bus %lu changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q max reached.", physical_bus);
        show_information_with_leading_time_stamp(buffer);

        return bus_type_changed;
    }

    size_t n;

    if(bus_Q_mismatch_in_MVar > total_q_max_in_MVar)
    {
        bus->set_bus_type(PV_TO_PQ_TYPE_3);
        bus_type_changed = true;

        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Bus %lu changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q max reached.", physical_bus);
        show_information_with_leading_time_stamp(buffer);

        set_all_sources_at_physical_bus_to_q_max(physical_bus);
        return bus_type_changed;
    }

    if(bus_Q_mismatch_in_MVar < total_q_min_in_MVar)
    {
        bus->set_bus_type(PV_TO_PQ_TYPE_3);
        bus_type_changed = true;

        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Bus %lu changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q min reached.", physical_bus);
        show_information_with_leading_time_stamp(buffer);

        set_all_sources_at_physical_bus_to_q_min(physical_bus);
        return bus_type_changed;
    }

    double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
    Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

    vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;
        double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
        Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
        sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
    }
    return bus_type_changed;
}

bool POWERFLOW_SOLVER::check_PV_TO_PQ_bus_constraint_of_physical_bus(size_t physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    BUS* bus = psdb.get_bus(physical_bus);

    if(bus->get_bus_type()!=PV_TO_PQ_TYPE_1 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_2 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_3 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_4)
        return false;

    ostringstream osstream;

    bool bus_type_changed = false;

    double total_q_max_in_MVar = psdb.get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
    double total_q_min_in_MVar = psdb.get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

    if(total_q_max_in_MVar == total_q_min_in_MVar)
    {
        if(bus->get_bus_type()!=PV_TO_PQ_TYPE_3)
        {
            bus->set_bus_type(PV_TO_PQ_TYPE_3);
            bus_type_changed = true;

            osstream<<"Bus "<<physical_bus<<" changed from PV_TYPE to PV_TO_PQ_TYPE_3.";
            show_information_with_leading_time_stamp(osstream);

            set_all_sources_at_physical_bus_to_q_max(physical_bus);
        }
        return bus_type_changed;
    }



    double bus_voltage = bus->get_voltage_in_pu();

    double voltage_to_regulated = psdb.get_voltage_to_regulate_of_physical_bus_in_pu(physical_bus);

    double total_q_generation_in_MVar = psdb.get_total_regulating_q_generation_at_physical_bus_in_MVar(physical_bus);

    size_t internal_bus = network_matrix.get_internal_bus_number_of_physical_bus(physical_bus);
    double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*psdb.get_system_base_power_in_MVA();

    //bus_Q_mismatch_in_MVar += total_q_generation_in_MVar;

    //os<<"Bus %u: current V= %fpu, V to regulate: %fpu, current Q= %fMVar", physical_bus, bus_voltage, voltage_to_regulated, total_q_generation_in_MVar);
    //show_information_with_leading_time_stamp(osstream);
    //os<<"Qmax= %fMVar, Qmin= %fMVar", total_q_max_in_MVar, total_q_min_in_MVar);
    //show_information_with_leading_time_stamp(osstream);

    if(bus_voltage>voltage_to_regulated)
    {
        if(total_q_generation_in_MVar==total_q_min_in_MVar)
        {
            ;
        }
        else
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
                    bus->set_voltage_in_pu(voltage_to_regulated);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                default:
                    break;
            }
        }
    }
    else // bus_voltage<voltage_to_regulated
    {
        if(total_q_generation_in_MVar==total_q_max_in_MVar)
        {
            ;
        }
        else
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
                    bus->set_voltage_in_pu(voltage_to_regulated);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                    show_information_with_leading_time_stamp(osstream);
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
            show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            switch(bus->get_bus_type())
            {
                case PV_TO_PQ_TYPE_4:
                    bus->set_bus_type(PV_TO_PQ_TYPE_3);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_4 to PV_TO_PQ_TYPE_3.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_3:
                    bus->set_bus_type(PV_TO_PQ_TYPE_2);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_3 to PV_TO_PQ_TYPE_2.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_2:
                    bus->set_bus_type(PV_TO_PQ_TYPE_1);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_2 to PV_TO_PQ_TYPE_1.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_1:
                    bus->set_bus_type(PV_TYPE);
                    bus_type_changed = true;
                    bus->set_voltage_in_pu(voltage_to_regulated);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                    show_information_with_leading_time_stamp(osstream);
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
            show_information_with_leading_time_stamp(osstream);
        }
        else
        {
            switch(bus->get_bus_type())
            {
                case PV_TO_PQ_TYPE_4:
                    bus->set_bus_type(PV_TO_PQ_TYPE_3);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_4 to PV_TO_PQ_TYPE_3.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_3:
                    bus->set_bus_type(PV_TO_PQ_TYPE_2);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_3 to PV_TO_PQ_TYPE_2.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_2:
                    bus->set_bus_type(PV_TO_PQ_TYPE_1);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_2 to PV_TO_PQ_TYPE_1.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                case PV_TO_PQ_TYPE_1:
                    bus->set_bus_type(PV_TYPE);
                    bus_type_changed = true;
                    bus->set_voltage_in_pu(voltage_to_regulated);
                    osstream<<"Bus "<<physical_bus<<" changed from PV_TO_PQ_TYPE_1 to PV_TYPE.";
                    show_information_with_leading_time_stamp(osstream);
                    break;
                default:
                    break;
            }
        }
    }
    return bus_type_changed;
}

void POWERFLOW_SOLVER::set_all_sources_at_physical_bus_to_q_min(size_t physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        sources[i]->set_q_generation_in_MVar(sources[i]->get_q_min_in_MVar());
}
void POWERFLOW_SOLVER::set_all_sources_at_physical_bus_to_q_max(size_t physical_bus)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    vector<SOURCE*> sources = psdb.get_sources_connecting_to_bus(physical_bus);

    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
        sources[i]->set_q_generation_in_MVar(sources[i]->get_q_max_in_MVar());
}
vector<double> POWERFLOW_SOLVER::get_bus_power_mismatch_vector_for_coupled_solution()
{
    size_t nP = internal_P_equation_buses.size();
    size_t nQ = internal_Q_equation_buses.size();

    vector<double> s_mismatch;
    s_mismatch.reserve(nP+nQ);

    size_t internal_bus;

    for(size_t i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        s_mismatch.push_back(-bus_power[internal_bus].real());
    }
    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        s_mismatch.push_back(-bus_power[internal_bus].imag());
    }
/*
    ostringstream osstream;
    osstream<<"Bus power mismatch vector for coupled powerflow solution (equation index)");
    show_information_with_leading_time_stamp(osstream);

    osstream<<"index     mismatch(pu)");
    show_information_with_leading_time_stamp(osstream);

    size_t n = s_mismatch.size();
    for(size_t i=0; i!=n; ++i)
    {
        osstream<<"%-8u %-10f",i,s_mismatch[i]);
        show_information_with_leading_time_stamp(osstream);
    }
    */
    return s_mismatch;
}

vector<double> POWERFLOW_SOLVER::get_bus_P_power_mismatch_vector_for_decoupled_solution()
{
    size_t nP = internal_P_equation_buses.size();

    vector<double> P_mismatch;
    P_mismatch.reserve(nP);
    P_mismatch.clear();

    size_t internal_bus;

    for(size_t i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        P_mismatch.push_back(-bus_power[internal_bus].real());
        //cout<<"Pmismatch vector, "<<i<<", "<<-bus_power[internal_bus].real()<<endl;
    }
    return P_mismatch;
}

vector<double> POWERFLOW_SOLVER::get_bus_Q_power_mismatch_vector_for_decoupled_solution()
{
    size_t nQ = internal_Q_equation_buses.size();

    vector<double> Q_mismatch;
    Q_mismatch.reserve(nQ);
    Q_mismatch.clear();

    size_t internal_bus;
    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        Q_mismatch.push_back(-bus_power[internal_bus].imag());
        //cout<<"Qmismatch vector, "<<i<<", "<<-bus_power[internal_bus].imag()<<endl;
    }

    return Q_mismatch;
}

void POWERFLOW_SOLVER::update_bus_voltage_and_angle(const vector<double>& update)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
/*    ostringstream osstream;
    osstream<<"Bus voltage and angle correction(equation index)");
    show_information_with_leading_time_stamp(osstream);

    osstream<<"index     mismatch(pu)");
    show_information_with_leading_time_stamp(osstream);

    size_t n = update.size();
    for(size_t i=0; i!=n; ++i)
    {
        osstream<<"%-8u %-10f",i,update[i]);
        show_information_with_leading_time_stamp(osstream);
    }
*/

    double Perror0 = get_maximum_active_power_mismatch_in_MW();
    double Qerror0 = get_maximum_reactive_power_mismatch_in_MVar();

    size_t nP = internal_P_equation_buses.size();
    size_t nQ = internal_Q_equation_buses.size();

    vector<size_t> s_mismatch;
    s_mismatch.reserve(nP+nQ);
    s_mismatch.clear();

    size_t internal_bus, physical_bus;

    double delta_voltage, delta_angle, current_voltage, current_angle;

    alpha = get_iteration_accelerator();

    BUS* bus;

    for(size_t i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

        bus = psdb.get_bus(physical_bus);

        current_angle = bus->get_angle_in_rad();

        delta_angle = update[i];

        bus->set_angle_in_rad(current_angle + alpha*delta_angle);
    }

    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

        bus = psdb.get_bus(physical_bus);

        current_voltage = bus->get_voltage_in_pu();

        delta_voltage = update[nP+i];

        bus->set_voltage_in_pu(current_voltage + alpha*delta_voltage);
    }

    if(get_non_divergent_solution_logic()==true)
    {
        try_to_solve_hvdc_steady_state();
        for(size_t i=0; i<10; ++i)
        {
            calculate_raw_bus_power_mismatch();
            double Perror = get_maximum_active_power_mismatch_in_MW();
            double Qerror = get_maximum_reactive_power_mismatch_in_MVar();
            if((Perror<Perror0 or Perror<get_allowed_max_active_power_imbalance_in_MW())
                and (Qerror<Qerror0 or Qerror<get_allowed_max_reactive_power_imbalance_in_MVar()))
                break;
            else
            {
                alpha *= 0.5;
                for(size_t i=0; i!=nP; ++i)
                {
                    internal_bus = internal_P_equation_buses[i];
                    physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

                    bus = psdb.get_bus(physical_bus);

                    current_angle = bus->get_angle_in_rad();

                    delta_angle = update[i];

                    bus->set_angle_in_rad(current_angle - alpha*delta_angle);
                }

                for(size_t i=0; i!=nQ; ++i)
                {
                    internal_bus = internal_Q_equation_buses[i];
                    physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

                    bus = psdb.get_bus(physical_bus);

                    current_voltage = bus->get_voltage_in_pu();

                    delta_voltage = update[nP+i];

                    bus->set_voltage_in_pu(current_voltage - alpha*delta_voltage);
                }
            }
        }
    }
/*
    osstream<<"New bus voltage and angle in this iteration:");
    show_information_with_leading_time_stamp(osstream);

    osstream<<"bus      voltage(pu) angle(deg)");
    show_information_with_leading_time_stamp(osstream);

    size_t nbus = get_bus_count();
    for(size_t i=0; i!=nbus; ++i)
    {
        osstream<<"%-8u %-10f %-10f",STEPS::Bus[i].get_bus_number(),STEPS::Bus[i].get_voltage_in_pu(),
                      STEPS::Bus[i].get_angle_in_deg());
        show_information_with_leading_time_stamp(osstream);
    }
*/
}

void POWERFLOW_SOLVER::update_bus_voltage(const vector<double>& update)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    double Qerror0 = get_maximum_reactive_power_mismatch_in_MVar();

    size_t nP = internal_P_equation_buses.size();
    size_t nQ = internal_Q_equation_buses.size();

    size_t internal_bus, physical_bus;

    double delta_voltage, current_voltage;

    alpha = get_iteration_accelerator();

    double max_delta_v = 0.0;
    size_t max_delta_v_bus = 0;

    BUS* bus;

    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

        bus = psdb.get_bus(physical_bus);

        current_voltage = bus->get_voltage_in_pu();

        delta_voltage = update[i];

        bus->set_voltage_in_pu(current_voltage + alpha*delta_voltage);

        if(fabs(delta_voltage)> fabs(max_delta_v))
        {
            max_delta_v = delta_voltage;
            max_delta_v_bus = physical_bus;
        }
    }

    if(get_non_divergent_solution_logic()==true)
    {
        try_to_solve_hvdc_steady_state();
        for(size_t i=0; i<10; ++i)
        {
            calculate_raw_bus_power_mismatch();
            double Qerror = get_maximum_reactive_power_mismatch_in_MVar();
            if(Qerror<Qerror0 or Qerror<get_allowed_max_reactive_power_imbalance_in_MVar())
                break;
            else
            {
                alpha *= 0.5;
                max_delta_v *= 0.5;
                for(size_t i=0; i!=nQ; ++i)
                {
                    internal_bus = internal_Q_equation_buses[i];
                    physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

                    bus = psdb.get_bus(physical_bus);

                    current_voltage = bus->get_voltage_in_pu();

                    delta_voltage = update[nP+i];

                    bus->set_voltage_in_pu(current_voltage - alpha*delta_voltage);
                }
            }
        }
    }

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Max voltage update: %f pu at physical bus %lu.",
             max_delta_v,max_delta_v_bus);
    show_information_with_leading_time_stamp(buffer);
}


void POWERFLOW_SOLVER::update_bus_angle(const vector<double>& update)
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    ostringstream osstream;

    double Perror0 = get_maximum_active_power_mismatch_in_MW();

    size_t nP = internal_P_equation_buses.size();

    size_t internal_bus, physical_bus;

    double delta_angle, current_angle;

    alpha = get_iteration_accelerator();

    double max_delta_angle = 0.0;
    size_t max_delta_angle_bus = 0;

    BUS* bus;

    for(size_t i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

        bus = psdb.get_bus(physical_bus);

        current_angle = bus->get_angle_in_rad();

        delta_angle = update[i];

        bus->set_angle_in_rad(current_angle + alpha*delta_angle);

        if(fabs(delta_angle)> fabs(max_delta_angle))
        {
            max_delta_angle = delta_angle;
            max_delta_angle_bus = physical_bus;
        }
    }


    if(get_non_divergent_solution_logic()==true)
    {
        for(size_t i=0; i<10; ++i)
        {
            calculate_raw_bus_power_mismatch();
            double Perror = get_maximum_active_power_mismatch_in_MW();
            if(Perror<Perror0 or Perror<get_allowed_max_active_power_imbalance_in_MW())
                break;
            else
            {
                alpha *= 0.5;
                max_delta_angle *=0.5;
                for(size_t i=0; i!=nP; ++i)
                {
                    internal_bus = internal_P_equation_buses[i];
                    physical_bus = network_matrix.get_physical_bus_number_of_internal_bus(internal_bus);

                    bus = psdb.get_bus(physical_bus);

                    current_angle = bus->get_angle_in_rad();

                    delta_angle = update[i];

                    bus->set_angle_in_rad(current_angle - alpha*delta_angle);
                }
            }
        }
    }

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Max angle update: %f rad at physical bus %lu.",
             max_delta_angle,max_delta_angle_bus);
    show_information_with_leading_time_stamp(buffer);
}

void POWERFLOW_SOLVER::show_powerflow_result() const
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    ostringstream osstream;

    char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];


    if(is_converged())
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow converged within %lu iterations. Results are listed as follows.",
                get_iteration_count());
    else
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Powerflow failed to converge within %lu iterations. Results of the last iteration are listed as follows.",
                get_iteration_count());
    show_information_with_leading_time_stamp(buffer);

    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Solved bus voltage and angle:");
    show_information_with_leading_time_stamp(buffer);

    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "bus      voltage(pu) angle(deg)");
    show_information_with_leading_time_stamp(buffer);

    vector<BUS*> buses = psdb.get_all_buses();
    size_t nbus = buses.size();
    /*if(nbus>200)
        nbus = 200;*/
    for(size_t i=0; i!=nbus; ++i)
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%8lu %10.6f %10.6f %s",
                 buses[i]->get_bus_number(),buses[i]->get_voltage_in_pu(),buses[i]->get_angle_in_deg(),(buses[i]->get_bus_name()).c_str());
        show_information_with_leading_time_stamp(buffer);
    }

    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Solved machine power:");
    show_information_with_leading_time_stamp(buffer);

    snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "bus      id   P(MW)      Q(MVar)");
    show_information_with_leading_time_stamp(buffer);

    vector<SOURCE*> sources = psdb.get_all_sources();
    size_t nsource = sources.size();
    /*if(nsource>200)
        nsource = 200;*/
    for(size_t i=0; i!=nsource; ++i)
    {
        snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "%8lu '%2s'  %10.6f  %10.6f  %s(%s)",
                 sources[i]->get_source_bus(),(sources[i]->get_identifier()).c_str(),
                 sources[i]->get_p_generation_in_MW(),sources[i]->get_q_generation_in_MVar(), (sources[i]->get_device_name()).c_str(), psdb.bus_number2bus_name(sources[i]->get_source_bus()).c_str());
        show_information_with_leading_time_stamp(buffer);
    }
}
void POWERFLOW_SOLVER::save_powerflow_result_to_file(string filename) const
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    ostringstream osstream;

    ofstream file(filename);
    if(not file.is_open())
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving powerflow result to file."<<endl
          <<"No powerflow result will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

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
    vector<BUS*> buses = psdb.get_all_buses();
    size_t nbus = buses.size();
    if(nbus>0)
    {
        file<<"% Bus"<<endl;
        file<<"BUS,NAME,VBASE/KV,VOLTAGE/PU,ANGLE/DEG,VOLTAGE/KV,ANGLE/RAD"<<endl;
        for(size_t i=0; i!=nbus; ++i)
        {
            /*file<<buses[i]->get_bus_number()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_base_voltage_in_kV()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_voltage_in_pu()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_angle_in_deg()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_voltage_in_kV()<<","
                <<setprecision(6)<<fixed<<buses[i]->get_angle_in_rad()<<endl;*/
            snprintf(buffer, 1000, "%lu,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f",
                     buses[i]->get_bus_number(),(buses[i]->get_bus_name()).c_str(),
                     buses[i]->get_base_voltage_in_kV(),
                     buses[i]->get_voltage_in_pu(), buses[i]->get_angle_in_deg(),
                     buses[i]->get_voltage_in_kV(), buses[i]->get_angle_in_rad());
            file<<buffer<<endl;
        }
    }

    vector<GENERATOR*> generators = psdb.get_all_generators();
    size_t ngen = generators.size();
    if(ngen>0)
    {
        file<<"% Generator"<<endl;
        file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
        for(size_t i=0; i!=ngen; ++i)
        {
            size_t bus = generators[i]->get_generator_bus();
            /*file<<bus<<",\""<<generators[i]->get_identifier()<<"\","
                <<setprecision(6)<<fixed<<generators[i]->get_p_generation_in_MW()<<","
                <<setprecision(6)<<fixed<<generators[i]->get_q_generation_in_MVar()<<","
                <<setprecision(6)<<db->get_bus_voltage_in_pu(bus)<<endl;*/
            snprintf(buffer, 1000, "%lu,\"%s\",%.6f,%.6f,%.6f",
                     generators[i]->get_generator_bus(),(generators[i]->get_identifier()).c_str(),
                     generators[i]->get_p_generation_in_MW(), generators[i]->get_q_generation_in_MVar(),
                     psdb.get_bus_voltage_in_pu(bus));
            file<<buffer<<endl;
        }
    }

    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    size_t nsource = wt_generators.size();
    if(nsource>0)
    {
        file<<"% WT generator"<<endl;
        file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
        for(size_t i=0; i!=nsource; ++i)
        {
            size_t bus = wt_generators[i]->get_source_bus();
            /*file<<bus<<",\""<<wt_generators[i]->get_identifier()<<"\","
                <<setprecision(6)<<fixed<<wt_generators[i]->get_p_generation_in_MW()<<","
                <<setprecision(6)<<fixed<<wt_generators[i]->get_q_generation_in_MVar()<<","
                <<setprecision(6)<<db->get_bus_voltage_in_pu(bus)<<endl;*/
            snprintf(buffer, 1000, "%lu,\"%s\",%.6f,%.6f,%.6f",
                     wt_generators[i]->get_generator_bus(),(wt_generators[i]->get_identifier()).c_str(),
                     wt_generators[i]->get_p_generation_in_MW(), wt_generators[i]->get_q_generation_in_MVar(),
                     psdb.get_bus_voltage_in_pu(bus));
            file<<buffer<<endl;
        }
    }

    vector<LOAD*> loads = psdb.get_all_loads();
    size_t nload = loads.size();
    if(nload>0)
    {
        file<<"% Load"<<endl;
        file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
        for(size_t i=0; i!=nload; ++i)
        {
            size_t bus = loads[i]->get_load_bus();
            complex<double> s = loads[i]->get_actual_total_load_in_MVA();
            /*file<<bus<<",\""<<loads[i]->get_identifier()<<"\","
                <<setprecision(6)<<fixed<<s.real()<<","
                <<setprecision(6)<<fixed<<s.imag()<<","
                <<setprecision(6)<<db->get_bus_voltage_in_pu(bus)<<endl;*/
            snprintf(buffer, 1000, "%lu,\"%s\",%.6f,%.6f,%.6f",
                     loads[i]->get_load_bus(),(loads[i]->get_identifier()).c_str(),
                     s.real(), s.imag(), psdb.get_bus_voltage_in_pu(bus));
            file<<buffer<<endl;
        }
    }

    vector<LINE*> lines = psdb.get_all_lines();
    size_t nline = lines.size();
    if(nline>0)
    {
        file<<"% Line"<<endl;
        file<<"IBUS,JBUS,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,II/KA,IJ/KA"<<endl;
        for(size_t i=0; i!=nline; ++i)
        {
            complex<double> si = lines[i]->get_line_complex_power_at_sending_side_in_MVA();
            complex<double> sj = lines[i]->get_line_complex_power_at_receiving_side_in_MVA();
            /*file<<lines[i]->get_sending_side_bus()<<","<<lines[i]->get_receiving_side_bus()<<",\""<<lines[i]->get_identifier()<<"\","
                <<setprecision(6)<<fixed<<si.real()<<","
                <<setprecision(6)<<fixed<<si.imag()<<","
                <<setprecision(6)<<fixed<<sj.real()<<","
                <<setprecision(6)<<fixed<<sj.imag()<<","
                <<setprecision(6)<<fixed<<steps_fast_complex_abs(lines[i]->get_line_complex_current_at_sending_side_in_kA())<<","
                <<setprecision(6)<<fixed<<steps_fast_complex_abs(lines[i]->get_line_complex_current_at_receiving_side_in_kA())<<endl;*/

            snprintf(buffer, 1000, "%lu,%lu,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                     lines[i]->get_sending_side_bus(),lines[i]->get_receiving_side_bus(),(lines[i]->get_identifier()).c_str(),
                     si.real(), si.imag(),sj.real(), sj.imag(),
                     steps_fast_complex_abs(lines[i]->get_line_complex_current_at_sending_side_in_kA()),
                     steps_fast_complex_abs(lines[i]->get_line_complex_current_at_receiving_side_in_kA()));
            file<<buffer<<endl;
        }
    }

    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();
    size_t ntrans = transformers.size();
    if(ntrans>0)
    {
        file<<"% Transformer"<<endl;
        file<<"IBUS,JBUS,KBUS,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,PK/MW,QK/MVAR,KI/PU,KJ/PU,KK/PU"<<endl;
        for(size_t i=0; i!=ntrans; ++i)
        {
            complex<double> sp = transformers[i]->get_winding_complex_power_in_MVA(PRIMARY_SIDE);
            complex<double> ss = transformers[i]->get_winding_complex_power_in_MVA(SECONDARY_SIDE);
            complex<double> st = transformers[i]->get_winding_complex_power_in_MVA(TERTIARY_SIDE);

            /*file<<transformers[i]->get_winding_bus(PRIMARY_SIDE)<<","
                <<transformers[i]->get_winding_bus(SECONDARY_SIDE)<<","
                <<transformers[i]->get_winding_bus(TERTIARY_SIDE)<<",\""<<transformers[i]->get_identifier()<<"\","
                <<setprecision(6)<<fixed<<sp.real()<<","
                <<setprecision(6)<<fixed<<sp.imag()<<","
                <<setprecision(6)<<fixed<<ss.real()<<","
                <<setprecision(6)<<fixed<<ss.imag()<<","
                <<setprecision(6)<<fixed<<st.real()<<","
                <<setprecision(6)<<fixed<<st.imag()<<","
                <<setprecision(6)<<fixed<<transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE)<<","
                <<setprecision(6)<<fixed<<transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE)<<","
                <<setprecision(6)<<fixed<<transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE)<<endl;*/


            snprintf(buffer, 1000, "%lu,%lu,%lu,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                     transformers[i]->get_winding_bus(PRIMARY_SIDE),transformers[i]->get_winding_bus(SECONDARY_SIDE),
                     transformers[i]->get_winding_bus(TERTIARY_SIDE), (transformers[i]->get_identifier()).c_str(),
                     sp.real(), sp.imag(), ss.real(), ss.imag(), st.real(), st.imag(),
                     transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE),
                     transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE),
                     transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE));
            file<<buffer<<endl;
        }
    }

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t nhvdc = hvdcs.size();
    if(nhvdc>0)
    {
        file<<"% Hvdc"<<endl;
        file<<"IBUS,JBUS,ID,PR/MW,QR/MVAR,PI/MW,QI/MVAR,ALPHA/DEG,GAMMA/DEG,VDCR/KV,VDCI/KV,IDC/KA,VACR/PU,VACI/PU,KR/PU,KI/PU"<<endl;
        for(size_t i=0; i!=nhvdc; ++i)
        {
            size_t busr = hvdcs[i]->get_converter_bus(RECTIFIER);
            size_t busi = hvdcs[i]->get_converter_bus(INVERTER);
            /*file<<busr<<","<<busi<<",\""<<hvdcs[i]->get_identifier()<<"\","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_active_power_in_MW(RECTIFIER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_reactive_power_in_MVar(RECTIFIER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_active_power_in_MW(INVERTER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_reactive_power_in_MVar(INVERTER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_alpha_or_gamma_in_deg(RECTIFIER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_alpha_or_gamma_in_deg(INVERTER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_dc_voltage_in_kV(RECTIFIER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_dc_voltage_in_kV(INVERTER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_dc_current_in_kA(RECTIFIER)<<","
                <<setprecision(6)<<fixed<<db->get_bus_voltage_in_pu(busr)<<","
                <<setprecision(6)<<fixed<<db->get_bus_voltage_in_pu(busi)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER)<<","
                <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER)<<endl;*/
            snprintf(buffer, 1000, "%lu,%lu,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
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
                     psdb.get_bus_voltage_in_pu(busr),
                     psdb.get_bus_voltage_in_pu(busi),
                     hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER),
                     hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER));
            file<<buffer<<endl;
        }
    }

    file.close();
}

void POWERFLOW_SOLVER::save_extended_powerflow_result_to_file(string filename) const
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();

    ostringstream osstream;

    ofstream file(filename);
    if(not file.is_open())
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving extended powerflow result to file."<<endl
          <<"No powerflow result will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

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
    vector<BUS*> buses = psdb.get_all_buses();
    size_t nbus = buses.size();
    if(nbus>0)
    {
        file<<"% Bus"<<endl;
        file<<"BUS,NAME,VBASE/KV,VOLTAGE/PU,ANGLE/DEG,VOLTAGE/KV,ANGLE/RAD"<<endl;
    }
    for(size_t i=0; i!=nbus; ++i)
    {
        /*file<<buses[i]->get_bus_number()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_base_voltage_in_kV()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_voltage_in_pu()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_angle_in_deg()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_voltage_in_kV()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_angle_in_rad()<<endl;*/
        snprintf(buffer, 1000, "%lu,\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f",
                 buses[i]->get_bus_number(),(buses[i]->get_bus_name()).c_str(),
                 buses[i]->get_base_voltage_in_kV(),
                 buses[i]->get_voltage_in_pu(), buses[i]->get_angle_in_deg(),
                 buses[i]->get_voltage_in_kV(), buses[i]->get_angle_in_rad());
        file<<buffer<<endl;
    }

    vector<GENERATOR*> generators = psdb.get_all_generators();
    size_t ngen = generators.size();
    if(ngen>0)
    {
        file<<"% Generator"<<endl;
        file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
    }
    for(size_t i=0; i!=ngen; ++i)
    {
        size_t bus = generators[i]->get_generator_bus();
        /*file<<bus<<",\""<<generators[i]->get_identifier()<<"\","
            <<setprecision(6)<<fixed<<generators[i]->get_p_generation_in_MW()<<","
            <<setprecision(6)<<fixed<<generators[i]->get_q_generation_in_MVar()<<","
            <<setprecision(6)<<db->get_bus_voltage_in_pu(bus)<<endl;*/
        snprintf(buffer, 1000, "%lu,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                 bus, psdb.bus_number2bus_name(bus).c_str(),
                 (generators[i]->get_identifier()).c_str(),
                 generators[i]->get_p_generation_in_MW(), generators[i]->get_q_generation_in_MVar(),
                 psdb.get_bus_voltage_in_pu(bus));
        file<<buffer<<endl;
    }

    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    size_t nsource = wt_generators.size();
    if(nsource>0)
    {
        file<<"% WT generator"<<endl;
        file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
    }
    for(size_t i=0; i!=nsource; ++i)
    {
        size_t bus = wt_generators[i]->get_source_bus();
        /*file<<bus<<",\""<<wt_generators[i]->get_identifier()<<"\","
            <<setprecision(6)<<fixed<<wt_generators[i]->get_p_generation_in_MW()<<","
            <<setprecision(6)<<fixed<<wt_generators[i]->get_q_generation_in_MVar()<<","
            <<setprecision(6)<<db->get_bus_voltage_in_pu(bus)<<endl;*/
        snprintf(buffer, 1000, "%lu,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                 bus, psdb.bus_number2bus_name(bus).c_str(),
                 (wt_generators[i]->get_identifier()).c_str(),
                 wt_generators[i]->get_p_generation_in_MW(), wt_generators[i]->get_q_generation_in_MVar(),
                 psdb.get_bus_voltage_in_pu(bus));
        file<<buffer<<endl;
    }

    vector<LOAD*> loads = psdb.get_all_loads();
    size_t nload = loads.size();
    if(nload>0)
    {
        file<<"% Load"<<endl;
        file<<"BUS,NAME,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
    }
    for(size_t i=0; i!=nload; ++i)
    {
        size_t bus = loads[i]->get_load_bus();
        complex<double> s = loads[i]->get_actual_total_load_in_MVA();
        /*file<<bus<<",\""<<loads[i]->get_identifier()<<"\","
            <<setprecision(6)<<fixed<<s.real()<<","
            <<setprecision(6)<<fixed<<s.imag()<<","
            <<setprecision(6)<<db->get_bus_voltage_in_pu(bus)<<endl;*/
        snprintf(buffer, 1000, "%lu,\"%s\",\"%s\",%.6f,%.6f,%.6f",
                 bus, psdb.bus_number2bus_name(bus).c_str(),
                 (loads[i]->get_identifier()).c_str(),
                 s.real(), s.imag(), psdb.get_bus_voltage_in_pu(bus));
        file<<buffer<<endl;
    }

    vector<LINE*> lines = psdb.get_all_lines();
    size_t nline = lines.size();
    if(nline>0)
    {
        file<<"% Line"<<endl;
        file<<"IBUS,INAME,JBUS,JNAMEID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,II/KA,IJ/KA"<<endl;
    }
    for(size_t i=0; i!=nline; ++i)
    {
        size_t ibus = lines[i]->get_sending_side_bus();
        size_t jbus = lines[i]->get_receiving_side_bus();
        complex<double> si = lines[i]->get_line_complex_power_at_sending_side_in_MVA();
        complex<double> sj = lines[i]->get_line_complex_power_at_receiving_side_in_MVA();
        /*file<<lines[i]->get_sending_side_bus()<<","<<lines[i]->get_receiving_side_bus()<<",\""<<lines[i]->get_identifier()<<"\","
            <<setprecision(6)<<fixed<<si.real()<<","
            <<setprecision(6)<<fixed<<si.imag()<<","
            <<setprecision(6)<<fixed<<sj.real()<<","
            <<setprecision(6)<<fixed<<sj.imag()<<","
            <<setprecision(6)<<fixed<<steps_fast_complex_abs(lines[i]->get_line_complex_current_at_sending_side_in_kA())<<","
            <<setprecision(6)<<fixed<<steps_fast_complex_abs(lines[i]->get_line_complex_current_at_receiving_side_in_kA())<<endl;*/

        snprintf(buffer, 1000, "%lu,\"%s\",%lu,\"%s\",\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
                 ibus, psdb.bus_number2bus_name(ibus).c_str(),
                 jbus, psdb.bus_number2bus_name(jbus).c_str(),
                 (lines[i]->get_identifier()).c_str(),
                 si.real(), si.imag(),sj.real(), sj.imag(),
                 steps_fast_complex_abs(lines[i]->get_line_complex_current_at_sending_side_in_kA()),
                 steps_fast_complex_abs(lines[i]->get_line_complex_current_at_receiving_side_in_kA()));
        file<<buffer<<endl;
    }

    vector<TRANSFORMER*> transformers = psdb.get_all_transformers();
    size_t ntrans = transformers.size();
    if(ntrans>0)
    {
        file<<"% Transformer"<<endl;
        file<<"IBUS,INAME,JBUS,JNAME,KBUS,KNAME,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,PK/MW,QK/MVAR,KI/PU,KJ/PU,KK/PU"<<endl;
    }
    for(size_t i=0; i!=ntrans; ++i)
    {
        size_t ibus = transformers[i]->get_winding_bus(PRIMARY_SIDE);
        size_t jbus = transformers[i]->get_winding_bus(SECONDARY_SIDE);
        size_t kbus = transformers[i]->get_winding_bus(TERTIARY_SIDE);

        complex<double> sp = transformers[i]->get_winding_complex_power_in_MVA(PRIMARY_SIDE);
        complex<double> ss = transformers[i]->get_winding_complex_power_in_MVA(SECONDARY_SIDE);
        complex<double> st = transformers[i]->get_winding_complex_power_in_MVA(TERTIARY_SIDE);

        /*file<<transformers[i]->get_winding_bus(PRIMARY_SIDE)<<","
            <<transformers[i]->get_winding_bus(SECONDARY_SIDE)<<","
            <<transformers[i]->get_winding_bus(TERTIARY_SIDE)<<",\""<<transformers[i]->get_identifier()<<"\","
            <<setprecision(6)<<fixed<<sp.real()<<","
            <<setprecision(6)<<fixed<<sp.imag()<<","
            <<setprecision(6)<<fixed<<ss.real()<<","
            <<setprecision(6)<<fixed<<ss.imag()<<","
            <<setprecision(6)<<fixed<<st.real()<<","
            <<setprecision(6)<<fixed<<st.imag()<<","
            <<setprecision(6)<<fixed<<transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(PRIMARY_SIDE)<<","
            <<setprecision(6)<<fixed<<transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(SECONDARY_SIDE)<<","
            <<setprecision(6)<<fixed<<transformers[i]->get_winding_off_nominal_turn_ratio_in_pu(TERTIARY_SIDE)<<endl;*/


        snprintf(buffer, 1000, "%lu,\"%s\",%lu,\"%s\",%lu,\"%s\",\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
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

    vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
    size_t nhvdc = hvdcs.size();
    if(nhvdc>0)
    {
        file<<"% Hvdc"<<endl;
        file<<"IBUS,INAME,JBUS,JNAME,ID,PR/MW,QR/MVAR,PI/MW,QI/MVAR,ALPHA/DEG,GAMMA/DEG,VDCR/KV,VDCI/KV,IDC/KA,VACR/PU,VACI/PU,KR/PU,KI/PU"<<endl;
    }
    for(size_t i=0; i!=nhvdc; ++i)
    {
        size_t busr = hvdcs[i]->get_converter_bus(RECTIFIER);
        size_t busi = hvdcs[i]->get_converter_bus(INVERTER);
        /*file<<busr<<","<<busi<<",\""<<hvdcs[i]->get_identifier()<<"\","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_active_power_in_MW(RECTIFIER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_reactive_power_in_MVar(RECTIFIER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_active_power_in_MW(INVERTER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_ac_reactive_power_in_MVar(INVERTER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_alpha_or_gamma_in_deg(RECTIFIER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_alpha_or_gamma_in_deg(INVERTER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_dc_voltage_in_kV(RECTIFIER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_dc_voltage_in_kV(INVERTER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_dc_current_in_kA(RECTIFIER)<<","
            <<setprecision(6)<<fixed<<db->get_bus_voltage_in_pu(busr)<<","
            <<setprecision(6)<<fixed<<db->get_bus_voltage_in_pu(busi)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER)<<","
            <<setprecision(6)<<fixed<<hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER)<<endl;*/
        snprintf(buffer, 1000, "%lu,\"%s\",%lu,\"%s\",\"%s\",%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f",
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
                 psdb.get_bus_voltage_in_pu(busr),
                 psdb.get_bus_voltage_in_pu(busi),
                 hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER),
                 hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER));
        file<<buffer<<endl;
    }

    file.close();
}

void POWERFLOW_SOLVER::save_network_matrix_to_file(string filename) const
{
    network_matrix.save_network_matrix_to_file(filename);
}

void POWERFLOW_SOLVER::save_jacobian_matrix_to_file(string filename)
{
    JACOBIAN_BUILDER jacobian_builder;
    jacobian_builder.set_network_matrix(network_matrix);

    jacobian_builder.build_seprate_jacobians();

    jacobian_builder.save_jacobian_matrix_to_file(filename);
}

void POWERFLOW_SOLVER::save_bus_powerflow_result_to_file(string filename) const
{
    POWER_SYSTEM_DATABASE& psdb = get_default_power_system_database();
    ostringstream osstream;

    ofstream file(filename);
    if(not file.is_open())
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving bus powerflow result to file."<<endl
          <<"No bus powerflow result will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    file<<"BUS,VOLTAGE,ANGLE"<<endl;

    vector<BUS*> buses = psdb.get_all_buses();
    size_t nbus = buses.size();
    for(size_t i=0; i!=nbus; ++i)
    {
        file<<buses[i]->get_bus_number()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_voltage_in_pu()<<","
            <<setprecision(6)<<fixed<<buses[i]->get_angle_in_deg()<<endl;
    }
    file.close();
}

size_t POWERFLOW_SOLVER::get_iteration_count() const
{
    return iteration_count;
}
