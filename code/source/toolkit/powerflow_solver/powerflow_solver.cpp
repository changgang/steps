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
    db = NULL;
    network_db = NULL;

    set_flat_start_logic(false);
    set_transformer_tap_adjustment_logic(true);
    set_non_divergent_solution_logic(false);

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
    if(network_db!=NULL)
        delete network_db;

}


void POWERFLOW_SOLVER::set_power_system_database(POWER_SYSTEM_DATABASE* psdb)
{
    if(psdb!=NULL)
    {
        this->db = psdb;
        if(network_db!=NULL)
        {
            delete network_db;
            network_db = NULL;
        }
        network_db = new NETWORK_DATABASE(psdb);
    }
    else
    {
        ostringstream osstream;
        osstream<<"Error. Invalid power system database (NULL) is given. Failed to connect POWERFLOW_SOLVER to POWER_SYSTEM_DATABASE.";
        show_information_with_leading_time_stamp(osstream);
    }
}

POWER_SYSTEM_DATABASE* POWERFLOW_SOLVER::get_power_system_database() const
{
    return db;
}

NETWORK_DATABASE* POWERFLOW_SOLVER::get_network_database() const
{
    return network_db;
}

bool POWERFLOW_SOLVER::is_power_system_database_set() const
{
    if(get_power_system_database()!=NULL)
        return true;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Powerflow solver is not connected to any power system database."<<endl
          <<"No operation on the powerflow solver will work.";
        show_information_with_leading_time_stamp(osstream);
        return false;
    }
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
        ostringstream osstream;
        osstream<<"Non-positive ("<<P<<" MW) is not allowed for setting convergence threshold of "
         <<"active power imbalance for powerflow solution.\n0.01MW will be set automatically.";
        show_information_with_leading_time_stamp(osstream);
        P_threshold_in_MW = 0.01;
    }
}

void POWERFLOW_SOLVER::set_allowed_max_reactive_power_imbalance_in_MVar(double Q)
{
    if(Q>0.0)
        Q_threshold_in_MVar = Q;
    else
    {
        ostringstream osstream;
        osstream<<"Non-positive ("<<Q<<" MVar) is not allowed for setting convergence threshold of "
          <<"reactive power imbalance for powerflow solution.\n0.01MVar will be set automatically.";
        show_information_with_leading_time_stamp(osstream);
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

void POWERFLOW_SOLVER::solve_with_full_Newton_Raphson_solution()
{
    if(not is_power_system_database_set())
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb->get_bus_count()==0)
        return;

    ostringstream osstream;
    osstream<<"Start solve powerflow with Full Newton Raphson solution.";
    show_information_with_leading_time_stamp(osstream);

    initialize_powerflow_solver();

    double max_P_mismatch_in_MW, max_Q_mismatch_in_MW;
    vector<double> bus_power_mismatch, bus_delta_voltage_angle;
    JACOBIAN_BUILDER jacobian_builder;
    jacobian_builder.set_network_database(network_db);

    network_db->build_network_matrix();
    //network_db->report_network_matrix();

    update_P_and_Q_equation_internal_buses();
    jacobian_builder.build_seprate_jacobians();
    //jacobian_builder.show_seprate_jacobians();

    while(true)
    {
        osstream<<"Iteration "<<iteration_count<<":";
        show_information_with_leading_time_stamp(osstream);

        //os<<"start calculating bus power mismatch");
        //show_information_with_leading_time_stamp(osstream);

        calculate_raw_bus_power_mismatch();

        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();


        //os<<"end calculating bus power mismatch\n"
        //        "start checking bus type constraints");
        //show_information_with_leading_time_stamp(osstream);

        bool bus_type_changed = check_bus_type_constraints();

        //os<<"end checking bus type constraints");
        //show_information_with_leading_time_stamp(osstream);
        if(bus_type_changed)
        {
            //os<<"start updating P Q equations.");
            //show_information_with_leading_time_stamp(osstream);
            update_P_and_Q_equation_internal_buses();

            calculate_raw_bus_power_mismatch();

            max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
            max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();
            //os<<"end updating P Q equations.");
            //show_information_with_leading_time_stamp(osstream);
            //continue;
        }
        if(max_P_mismatch_in_MW < get_allowed_max_active_power_imbalance_in_MW() and
           max_Q_mismatch_in_MW < get_allowed_max_reactive_power_imbalance_in_MVar())
        {
            set_convergence_flag(true);

            osstream<<"Powerflow converged within "<<iteration_count<<" iterations.";
            show_information_with_leading_time_stamp(osstream);
            break;
        }

        //os<<"start building bus power mismatch vector.");
        //show_information_with_leading_time_stamp(osstream);

        bus_power_mismatch = get_bus_power_mismatch_vector_for_coupled_solution();

        //os<<"end building bus power mismatch vector.\nstart updating jacobian.");
        //show_information_with_leading_time_stamp(osstream);

        jacobian_builder.update_seprate_jacobians();

        //os<<"end updating jacobian.\nstart getting full jacobian.");
        //show_information_with_leading_time_stamp(osstream);

        jacobian = jacobian_builder.get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(internal_P_equation_buses,
                                                                                                   internal_Q_equation_buses);
        //os<<"Full Jacobian: (equation index)");
        //show_information_with_leading_time_stamp(osstream);
        //jacobian.report_without_bus_number();

        //os<<"end getting full jacobian.\nstart calculating va update");
        //show_information_with_leading_time_stamp(osstream);

        bus_delta_voltage_angle = bus_power_mismatch/jacobian;

        //os<<"end calculating va update.\n start updating va.");
        //show_information_with_leading_time_stamp(osstream);
        update_bus_voltage_and_angle(bus_delta_voltage_angle);
        //os<<"end updating va.");
        //show_information_with_leading_time_stamp(osstream);

		++iteration_count;
        if(get_iteration_count()>get_max_iteration())
        {
            osstream<<"Powerflow failed to converge in "<<get_max_iteration()<<" iterations.";
            show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
}

void POWERFLOW_SOLVER::solve_with_fast_decoupled_solution()
{
    if(not is_power_system_database_set())
        return;

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb->get_bus_count()==0)
        return;

    ostringstream osstream;
    osstream<<"Start solve powerflow with Fast Decoupled solution.";
    show_information_with_leading_time_stamp(osstream);

    initialize_powerflow_solver();

    double max_P_mismatch_in_MW, max_Q_mismatch_in_MW;
    vector<double> P_power_mismatch, Q_power_mismatch, bus_delta_voltage, bus_delta_angle;
    JACOBIAN_BUILDER jacobian_builder;
    jacobian_builder.set_network_database(network_db);

    network_db->build_network_matrix();
    //const SPARSE_MATRIX& Y = network_db->get_network_matrix();
    //Y.report_brief();

    network_db->build_decoupled_network_matrix();

    update_P_and_Q_equation_internal_buses();
    BP = jacobian_builder.get_decoupled_B_jacobian_with_P_equation_internal_buses(internal_P_equation_buses);
    BQ = jacobian_builder.get_decoupled_B_jacobian_with_Q_equation_internal_buses(internal_Q_equation_buses);
    //BP.LU_factorization(1, 1e-6);
    //BQ.LU_factorization(1, 1e-6);

    size_t physical_bus;

    while(true)
    {
        osstream<<"Iteration "<<iteration_count<<":";
        show_information_with_leading_time_stamp(osstream);

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

        if(max_P_mismatch_in_MW < get_allowed_max_active_power_imbalance_in_MW() and
           max_Q_mismatch_in_MW < get_allowed_max_reactive_power_imbalance_in_MVar())
        {
            set_convergence_flag(true);

            osstream<<"Powerflow converged within "<<iteration_count<<" iterations:";
            show_information_with_leading_time_stamp(osstream);
            break;
        }

        //os<<"start building bus P power mismatch vector.");
        //show_information_with_leading_time_stamp(osstream);
        P_power_mismatch = get_bus_P_power_mismatch_vector_for_decoupled_solution();
        size_t n = internal_P_equation_buses.size();
        for(size_t i=0; i!=n; ++i)
        {
            physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]);
            P_power_mismatch[i] /= db->get_bus_voltage_in_pu(physical_bus);
        }

        //os<<"start calculating bus angle update.");
        //show_information_with_leading_time_stamp(osstream);
        bus_delta_angle = P_power_mismatch/BP;
        //for(size_t i=0; i<internal_P_equation_buses.size(); ++i)
        //    bus_delta_angle[i] /= abs(get_bus_complex_voltage_in_pu(get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i])));

        //os<<"start updating bus angle.");
        //show_information_with_leading_time_stamp(osstream);
        update_bus_angle(bus_delta_angle);

        //os<<"start calculating bus power mismatch for Q solution.");
        //show_information_with_leading_time_stamp(osstream);
        calculate_raw_bus_power_mismatch();

        max_P_mismatch_in_MW = get_maximum_active_power_mismatch_in_MW();
        max_Q_mismatch_in_MW = get_maximum_reactive_power_mismatch_in_MVar();

        //os<<"start building bus Q power mismatch vector.");
        //show_information_with_leading_time_stamp(osstream);
        Q_power_mismatch = get_bus_Q_power_mismatch_vector_for_decoupled_solution();
        n = internal_Q_equation_buses.size();
        for(size_t i=0; i!=n; ++i)
        {
            physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]);
            Q_power_mismatch[i] /= db->get_bus_voltage_in_pu(physical_bus);
        }

        //os<<"start calculating bus voltage update.");
        //show_information_with_leading_time_stamp(osstream);
        bus_delta_voltage = Q_power_mismatch/BQ;

        //os<<"start updating bus voltage.");
        //show_information_with_leading_time_stamp(osstream);
        update_bus_voltage(bus_delta_voltage);

        //os<<"end updating va.");
        //show_information_with_leading_time_stamp(osstream);

        iteration_count ++;
        if(get_iteration_count()>get_max_iteration())
        {
            osstream<<"Powerflow failed to converge in "<<get_max_iteration()<<" iterations.";
            show_information_with_leading_time_stamp(osstream);
            break;
        }
    }
}

void POWERFLOW_SOLVER::solve_with_modified_Gaussian_Seidel_solution()
{
    if(not is_power_system_database_set())
        return;

    ;
}


void POWERFLOW_SOLVER::initialize_powerflow_solver()
{
    if(not is_power_system_database_set())
        return;

    ostringstream osstream;
    osstream<<"Initializing powerflow solver.";
    show_information_with_leading_time_stamp(osstream);

    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    psdb->update_in_service_bus_count();

    initialize_generator_regulating_mode_with_bus_type();
    initialize_bus_type_and_voltage_to_regulate();
    initialize_bus_voltage();
    optimize_bus_numbers();
    iteration_count = 0;

    osstream<<"Done initializing powerflow solver.";
    show_information_with_leading_time_stamp(osstream);
}

void POWERFLOW_SOLVER::initialize_bus_type_and_voltage_to_regulate()
{
    vector<BUS*> buses = db->get_all_buses();

    size_t nbus = buses.size();
    for(size_t i=0; i!=nbus; ++i)
    {
        buses[i]->set_voltage_to_regulate_in_pu(0.0);
        if(buses[i]->get_bus_type()!=OUT_OF_SERVICE)
            buses[i]->set_bus_type(PQ_TYPE);
    }

    vector<SOURCE*> sources = db->get_all_sources();
    size_t nsource = sources.size();
    for(size_t i=0; i!=nsource; ++i)
        set_bus_type_and_voltage_to_regulate_with_source(*(sources[i]));
    /*
    size_t ngen = generators.size();
    for(size_t i=0; i!=ngen; ++i)
        set_bus_type_and_voltage_to_regulate_with_source(*(generators[i]));

    vector<
    size_t nwt_generator = get_wt_generator_count();
    for(size_t i=0; i!=nwt_generator; ++i)
        set_bus_type_and_voltage_to_regulate_with_source(*(wt_generators[i]));
    */
}

void POWERFLOW_SOLVER::initialize_generator_regulating_mode_with_bus_type()
{
    vector<GENERATOR*> generators = db->get_all_generators();
    size_t n = generators.size();
    GENERATOR* gen;
    for(size_t i=0; i!=n; ++i)
    {
        gen = generators[i];
        size_t bus = gen->get_generator_bus();

        BUS* busptr = db->get_bus(bus);

        BUS_TYPE bustype = busptr->get_bus_type();

        switch(bustype)
        {
            case PQ_TYPE:
                gen->set_regulating_mode(REGULATING_PQ);
                break;
            case PV_TYPE:
                gen->set_regulating_mode(REGULATING_PV);
                break;
            case SLACK_TYPE:
                gen->set_regulating_mode(REGULATING_VA);
                break;
            default:
                gen->set_regulating_mode(REGULATING_PQ);
                break;
        }
    }
}

void POWERFLOW_SOLVER::set_bus_type_and_voltage_to_regulate_with_source(SOURCE& source)
{
    if(source.get_status()==false)
        return;

    SOURCE_REGULATING_MODE mode = source.get_regulating_mode();

    BUS* busptr;

    if(mode==REGULATING_PV or mode == REGULATING_VA)
    {
        size_t bus = source.get_bus_to_regulate();
        double v = source.get_voltage_to_regulate_in_pu();

        busptr = db->get_bus(bus);

        if(busptr!=NULL)
        {
            busptr->set_voltage_to_regulate_in_pu(v);
            switch(busptr->get_bus_type())
            {
                case PQ_TYPE:
                    if(mode==REGULATING_PV)
                        busptr->set_bus_type(PV_TYPE);
                    else
                        busptr->set_bus_type(SLACK_TYPE);
                    break;
                case PV_TYPE:
                    if(mode==REGULATING_VA)
                        busptr->set_bus_type(SLACK_TYPE);
                    break;
                case SLACK_TYPE:
                default:
                    break;
            }
        }
    }
}

void POWERFLOW_SOLVER::initialize_bus_voltage()
{
    vector<BUS*> buses;

    buses = db->get_all_buses();
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


    ostringstream osstream;
    osstream<<"Initial bus voltage and angle are listed as follows.";
    show_information_with_leading_time_stamp(osstream);
    osstream<<"bus      voltage(pu) angle(deg)";
    show_information_with_leading_time_stamp(osstream);

    if(nbus>200)
        nbus = 200;
    for(size_t i=0; i!=nbus; ++i)
    {
        osstream<<setw(8)<<buses[i]->get_bus_number()<<" "
               <<setw(10)<<setprecision(6)<<buses[i]->get_voltage_in_pu()<<" "
               <<setw(10)<<setprecision(6)<<buses[i]->get_angle_in_deg();
        show_information_with_leading_time_stamp(osstream);
    }
}

void POWERFLOW_SOLVER::optimize_bus_numbers()
{
    network_db->optimize_network_ordering();
}

void POWERFLOW_SOLVER::update_P_and_Q_equation_internal_buses()
{
    ostringstream osstream;
    osstream<<"Updating powerflow P equation buses and Q equation buses.";
    show_information_with_leading_time_stamp(osstream);

    size_t nbus = db->get_in_service_bus_count();

    internal_P_equation_buses.clear();
    internal_Q_equation_buses.clear();
    internal_P_equation_buses.reserve(nbus);
    internal_Q_equation_buses.reserve(nbus);

    size_t bus;


    BUS_TYPE btype;
    BUS* busptr;
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = network_db->get_physical_bus_number_of_internal_bus(i);
        busptr = db->get_bus(bus);
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
        osstream<<network_db->get_physical_bus_number_of_internal_bus(internal_P_equation_buses[i]) << endl;
        show_information_with_leading_time_stamp(osstream);
    }
    osstream<<"Buses with Q equations (physical bus):"<<endl;
    show_information_with_leading_time_stamp(osstream);
    n = internal_Q_equation_buses.size();
    for(size_t i=0; i!=n; ++i)
    {
        osstream<< network_db->get_physical_bus_number_of_internal_bus(internal_Q_equation_buses[i]) << endl;
        show_information_with_leading_time_stamp(osstream);
    }*/

    osstream<<"Done updating powerflow P equation buses and Q equation buses.";
    show_information_with_leading_time_stamp(osstream);
}

void POWERFLOW_SOLVER::set_convergence_flag(bool flag)
{
    converged = flag;
}

bool POWERFLOW_SOLVER::get_convergence_flag() const
{
    if(not is_power_system_database_set())
        return false;

    return converged;
}

bool POWERFLOW_SOLVER::is_converged() const
{
    if(not is_power_system_database_set())
        return false;

    return get_convergence_flag();
}

void POWERFLOW_SOLVER::calculate_raw_bus_power_mismatch()
{
    calculate_raw_bus_power_into_network();

    size_t nbus = db->get_in_service_bus_count();
    for(size_t i=0; i!=nbus; ++i)
        bus_power[i] = -bus_power[i];

    add_source_to_bus_power_mismatch();
    add_load_to_bus_power_mismatch();
    add_hvdc_to_bus_power_mismatch();

/*
    ostringstream osstream;
    osstream<<"Power mismatch of all buses.");
    show_information_with_leading_time_stamp(osstream);
    osstream<<"bus     Pmismatch(MW) Qmismatch(MVar)");
    show_information_with_leading_time_stamp(osstream);

    size_t bus;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    NETWORK_DATABASE* ntdb = get_network_database();
    double sbase = psdb->get_system_base_power_in_MVA();
    for(size_t i=0; i!=nbus; ++i)
    {
        bus = ntdb->get_physical_bus_number_of_internal_bus(i);
        osstream<<"%-8u %-10f %-10f",bus, bus_power[i].real()*sbase, bus_power[i].imag()*sbase);
        show_information_with_leading_time_stamp(osstream);
    }
*/
}

void POWERFLOW_SOLVER::calculate_raw_bus_power_into_network()
{
    calculate_raw_bus_current_into_network();

    size_t nbus = db->get_in_service_bus_count();

    bus_power = bus_current;

    complex<double> voltage;
    size_t physical_bus_number;

    for(size_t i=0; i!=nbus; ++i)
    {
        physical_bus_number = network_db->get_physical_bus_number_of_internal_bus(i);
        voltage = db->get_bus_complex_voltage_in_pu(physical_bus_number);

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
    const SPARSE_MATRIX& Y = network_db->get_network_matrix();

    size_t nbus = db->get_in_service_bus_count();
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
        column_physical_bus = network_db->get_physical_bus_number_of_internal_bus(column);
        voltage = db->get_bus_complex_voltage_in_pu(column_physical_bus);

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
    double Sbase = db->get_system_base_power_in_MVA();

    size_t physical_bus, internal_bus;

    bool status;

    BUS_TYPE btype;

    SOURCE_REGULATING_MODE mode;

    complex<double> Sgen;

    vector<SOURCE*> sources = db->get_all_sources();

    BUS* busptr;

    size_t nsource = sources.size();

    for(size_t i=0; i!=nsource; ++i)
    {
        status = sources[i]->get_status();
        if(status == false)
            continue;

        mode = sources[i]->get_regulating_mode();
        if(mode==REGULATING_VA)
            continue;

        physical_bus = sources[i]->get_source_bus();

        busptr = db->get_bus(physical_bus);

        btype = busptr->get_bus_type();
        if(btype == OUT_OF_SERVICE)
            continue;

        if(mode==REGULATING_PV)
        {
            switch(btype)
            {
                case SLACK_TYPE:
                case PV_TYPE:
                    Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), 0.0);
                    break;
                default: // PV_PQ_TYPE
                    Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), sources[i]->get_q_generation_in_MVar());
                    break;
            }
        }
        else // REGULATING_PQ
        {
            Sgen = complex<double>(sources[i]->get_p_generation_in_MW(), sources[i]->get_q_generation_in_MVar());
        }
        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);
        bus_power[internal_bus] += (Sgen/Sbase);
    }
}

void POWERFLOW_SOLVER::add_load_to_bus_power_mismatch()
{
    double Sbase = db->get_system_base_power_in_MVA();

    size_t physical_bus, internal_bus;

    bool status;

    vector<LOAD*> loads = db->get_all_loads();

    size_t nload = loads.size();

    complex<double> Sload;

    for(size_t i=0; i!=nload; ++i)
    {
        status = loads[i]->get_status();
        if(status == false)
            continue;

        physical_bus = loads[i]->get_load_bus();
        internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

        Sload = loads[i]->get_actual_total_load_in_MVA()/Sbase;

        bus_power[internal_bus] -= Sload;
    }
}

void POWERFLOW_SOLVER::add_hvdc_to_bus_power_mismatch()
{
    double Sbase = db->get_system_base_power_in_MVA();

    size_t physical_bus_rec, internal_bus_rec,
           physical_bus_inv, internal_bus_inv;

    bool status;

    vector<HVDC*> hvdcs = db->get_all_hvdcs();

    size_t nhvdc = hvdcs.size();

    complex<double> S_rec, S_inv;

    for(size_t i=0; i!=nhvdc; ++i)
    {
        status = hvdcs[i]->get_status();
        if(status == false)
            continue;

        physical_bus_rec = hvdcs[i]->get_converter_bus(RECTIFIER);
        internal_bus_rec = network_db->get_internal_bus_number_of_physical_bus(physical_bus_rec);

        physical_bus_inv = hvdcs[i]->get_converter_bus(INVERTER);
        internal_bus_inv = network_db->get_internal_bus_number_of_physical_bus(physical_bus_inv);

        hvdcs[i]->solve_steady_state();
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
    if(not is_power_system_database_set())
        return 0.0;

    size_t nP = internal_P_equation_buses.size();
    size_t internal_bus, physical_bus;

    double max_P_error_in_pu = 0.0, max_P_error_in_MW;
    size_t max_P_error_physical_bus = 0;
    for(size_t i=0; i!=nP; ++i)
    {
        internal_bus = internal_P_equation_buses[i];
        if(fabs(bus_power[internal_bus].real()) > max_P_error_in_pu)
        {
            physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_bus);
            max_P_error_in_pu = fabs(bus_power[internal_bus].real());
            max_P_error_physical_bus = physical_bus;
        }
    }

    max_P_error_in_MW = max_P_error_in_pu*db->get_system_base_power_in_MVA();

    ostringstream osstream;
    osstream<<"Maximum   active power mismatch found: "<<max_P_error_in_MW<<" MW   at bus "<<max_P_error_physical_bus<<".";
    show_information_with_leading_time_stamp(osstream);

    return max_P_error_in_MW;
}

double POWERFLOW_SOLVER::get_maximum_reactive_power_mismatch_in_MVar() const
{
    if(not is_power_system_database_set())
        return 0.0;

    size_t nQ = internal_Q_equation_buses.size();
    size_t internal_bus, physical_bus;

    double max_Q_error_in_pu = 0.0, max_Q_error_in_MVar;
    size_t max_Q_error_physical_bus = 0;
    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        if(fabs(bus_power[internal_bus].imag()) > max_Q_error_in_pu)
        {
            physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_bus);
            max_Q_error_in_pu = fabs(bus_power[internal_bus].imag());
            max_Q_error_physical_bus = physical_bus;
        }
    }
    max_Q_error_in_MVar =  max_Q_error_in_pu*db->get_system_base_power_in_MVA();

    ostringstream osstream;
    osstream<<"Maximum reactive power mismatch found: "<<max_Q_error_in_MVar<<" MVar   at bus "<<max_Q_error_physical_bus<<".";
    show_information_with_leading_time_stamp(osstream);

    return max_Q_error_in_MVar;
}

bool POWERFLOW_SOLVER::check_bus_type_constraints()
{
    size_t physical_bus;
    BUS_TYPE btype;

    vector<size_t> index;

    bool system_bus_type_changed = false;
    bool bus_type_changed;

    vector<BUS*> buses = db->get_all_buses();

    size_t nbus = buses.size();

    for(size_t i=0; i!=nbus; ++i)
    {
        btype = buses[i]->get_bus_type();
        if(btype == PQ_TYPE or btype == OUT_OF_SERVICE)
            continue;

        physical_bus = buses[i]->get_bus_number();

        //os<< "checking bus %u",physical_bus);
        //show_information_with_leading_time_stamp(osstream);

        if(btype==SLACK_TYPE)
        {
            //os<< "start checking as slack bus");
            //show_information_with_leading_time_stamp(osstream);
            check_SLACK_bus_constraint_of_physical_bus(physical_bus);
            //os<< "end checking as slack bus");
            //show_information_with_leading_time_stamp(osstream);
            system_bus_type_changed = false;
        }
        else
        {
            if(btype == PV_TYPE)
            {
                //os<< "start checking as PV bus");
                //show_information_with_leading_time_stamp(osstream);
                bus_type_changed = check_PV_bus_constraint_of_physical_bus(physical_bus);

                //os<< "end checking as PV bus");
                //show_information_with_leading_time_stamp(osstream);
                if(bus_type_changed)
                    system_bus_type_changed = true;
            }
            else
            {
                //os<< "start checking as PV_to_PQ bus");
                //show_information_with_leading_time_stamp(osstream);
                bus_type_changed = check_PV_TO_PQ_bus_constraint_of_physical_bus(physical_bus);
                //os<< "end checking as PV_to_PQ bus");
                //show_information_with_leading_time_stamp(osstream);
                if(bus_type_changed)
                    system_bus_type_changed = true;
            }
        }
    }
    return system_bus_type_changed;
}


void POWERFLOW_SOLVER::check_SLACK_bus_constraint_of_physical_bus(size_t physical_bus)
{
    BUS* bus = db->get_bus(physical_bus);

    if(bus->get_bus_type()!=SLACK_TYPE)
        return;

    size_t internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

    double mvabase = db->get_system_base_power_in_MVA();
    double bus_P_mismatch_in_MW = -bus_power[internal_bus].real()*mvabase;
    double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*mvabase;

    double total_p_max_in_MW = db->get_regulatable_p_max_at_physical_bus_in_MW(physical_bus);
    double total_p_min_in_MW = db->get_regulatable_p_min_at_physical_bus_in_MW(physical_bus);
    double total_q_max_in_MVar = db->get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
    double total_q_min_in_MVar = db->get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

    double P_loading_percentage = (bus_P_mismatch_in_MW-total_p_min_in_MW);
        P_loading_percentage /= (total_p_max_in_MW - total_p_min_in_MW);
    double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
        Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

    vector<SOURCE*> sources = db->get_sources_connecting_to_bus(physical_bus);
    size_t n;
    SOURCE_REGULATING_MODE mode;
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;

        mode = sources[i]->get_regulating_mode();
        if(mode == REGULATING_VA)
        {
            double P_loading_in_MW = sources[i]->get_p_max_in_MW() - sources[i]->get_p_min_in_MW();
            P_loading_in_MW = P_loading_in_MW*P_loading_percentage + sources[i]->get_p_min_in_MW();
            sources[i]->set_p_generation_in_MW(P_loading_in_MW);
        }
        if(mode == REGULATING_VA or mode == REGULATING_PV)
        {
            double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
            Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
            sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
        }
    }
}

bool POWERFLOW_SOLVER::check_PV_bus_constraint_of_physical_bus(size_t physical_bus)
{
    BUS* bus = db->get_bus(physical_bus);

    if(bus->get_bus_type()!=PV_TYPE)
        return false;

    ostringstream osstream;
    //os<<"Checking PV bus %u.", physical_bus);
    //show_information_with_leading_time_stamp(osstream);

    bool bus_type_changed = false;
    size_t internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);

    double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*db->get_system_base_power_in_MVA();

    double total_q_max_in_MVar = db->get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
    double total_q_min_in_MVar = db->get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

    //os<<"desired Q=%fMVar, Qmax=%fMVar, Qmin=%fMVar.", bus_Q_mismatch_in_MVar, total_q_max_in_MVar, total_q_min_in_MVar);
    //show_information_with_leading_time_stamp(osstream);

    if(total_q_max_in_MVar==total_q_min_in_MVar)
    {
        bus->set_bus_type(PV_TO_PQ_TYPE_3);
        bus_type_changed = true;
        set_all_sources_at_physical_bus_to_q_max(physical_bus);
        osstream<<"Bus "<<physical_bus<<" changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q max reached.";
        show_information_with_leading_time_stamp(osstream);
        return bus_type_changed;
    }


    SOURCE_REGULATING_MODE mode;
    size_t n;

    if(bus_Q_mismatch_in_MVar > total_q_max_in_MVar)
    {
        bus->set_bus_type(PV_TO_PQ_TYPE_3);
        bus_type_changed = true;

        osstream<<"Bus "<<physical_bus<<" changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q max reached.";
        show_information_with_leading_time_stamp(osstream);

        set_all_sources_at_physical_bus_to_q_max(physical_bus);
        return bus_type_changed;
    }

    if(bus_Q_mismatch_in_MVar < total_q_min_in_MVar)
    {
        bus->set_bus_type(PV_TO_PQ_TYPE_3);
        bus_type_changed = true;

        osstream<<"Bus "<<physical_bus<<" changed from PV_TYPE to PV_TO_PQ_TYPE_3 with sources q min reached.";
        show_information_with_leading_time_stamp(osstream);

        set_all_sources_at_physical_bus_to_q_min(physical_bus);
        return bus_type_changed;
    }

    double Q_loading_percentage = (bus_Q_mismatch_in_MVar-total_q_min_in_MVar);
    Q_loading_percentage /= (total_q_max_in_MVar - total_q_min_in_MVar);

    vector<SOURCE*> sources = db->get_sources_connecting_to_bus(physical_bus);
    n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        if(sources[i]->get_status() == false)
            continue;
        mode = sources[i]->get_regulating_mode();
        if(mode == REGULATING_PV)
        {
            double Q_loading_in_MVar = sources[i]->get_q_max_in_MVar() - sources[i]->get_q_min_in_MVar();
            Q_loading_in_MVar = Q_loading_in_MVar*Q_loading_percentage + sources[i]->get_q_min_in_MVar();
            sources[i]->set_q_generation_in_MVar(Q_loading_in_MVar);
        }
    }
    return bus_type_changed;
}

bool POWERFLOW_SOLVER::check_PV_TO_PQ_bus_constraint_of_physical_bus(size_t physical_bus)
{
    BUS* bus = db->get_bus(physical_bus);

    if(bus->get_bus_type()!=PV_TO_PQ_TYPE_1 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_2 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_3 and
       bus->get_bus_type()!=PV_TO_PQ_TYPE_4)
        return false;

    ostringstream osstream;

    bool bus_type_changed = false;

    double total_q_max_in_MVar = db->get_regulatable_q_max_at_physical_bus_in_MVar(physical_bus);
    double total_q_min_in_MVar = db->get_regulatable_q_min_at_physical_bus_in_MVar(physical_bus);

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

    double voltage_to_regulated = db->get_voltage_to_regulate_of_physical_bus_in_pu(physical_bus);

    double total_q_generation_in_MVar = db->get_total_regulating_q_generation_at_physical_bus_in_MVar(physical_bus);

    size_t internal_bus = network_db->get_internal_bus_number_of_physical_bus(physical_bus);
    double bus_Q_mismatch_in_MVar = -bus_power[internal_bus].imag()*db->get_system_base_power_in_MVA();

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
    vector<SOURCE*> sources = db->get_sources_connecting_to_bus(physical_bus);

    SOURCE_REGULATING_MODE mode;
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        mode = sources[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode == REGULATING_VA)
            sources[i]->set_q_generation_in_MVar(sources[i]->get_q_min_in_MVar());
    }
}
void POWERFLOW_SOLVER::set_all_sources_at_physical_bus_to_q_max(size_t physical_bus)
{
    vector<SOURCE*> sources = db->get_sources_connecting_to_bus(physical_bus);

    SOURCE_REGULATING_MODE mode;
    size_t n = sources.size();
    for(size_t i=0; i!=n; ++i)
    {
        mode = sources[i]->get_regulating_mode();
        if(mode==REGULATING_PV or mode == REGULATING_VA)
            sources[i]->set_q_generation_in_MVar(sources[i]->get_q_max_in_MVar());
    }
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
    }

    return Q_mismatch;
}

void POWERFLOW_SOLVER::update_bus_voltage_and_angle(const vector<double>& update)
{
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
        physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_bus);

        bus = db->get_bus(physical_bus);

        current_angle = bus->get_angle_in_rad();

        delta_angle = update[i];

        bus->set_angle_in_rad(current_angle + alpha*delta_angle);
    }

    for(size_t i=0; i!=nQ; ++i)
    {
        internal_bus = internal_Q_equation_buses[i];
        physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_bus);

        bus = db->get_bus(physical_bus);

        current_voltage = bus->get_voltage_in_pu();

        delta_voltage = update[nP+i];

        bus->set_voltage_in_pu(current_voltage + alpha*delta_voltage);
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
    ostringstream osstream;

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
        physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_bus);

        bus = db->get_bus(physical_bus);

        current_voltage = bus->get_voltage_in_pu();

        delta_voltage = update[i];

        bus->set_voltage_in_pu(current_voltage + alpha*delta_voltage);

        if(fabs(delta_voltage)> fabs(max_delta_v))
        {
            max_delta_v = delta_voltage;
            max_delta_v_bus = physical_bus;
        }
    }
    osstream<<"Max voltage update: "<<max_delta_v<<" pu at physical bus "<<max_delta_v_bus<<".";
    show_information_with_leading_time_stamp(osstream);
}


void POWERFLOW_SOLVER::update_bus_angle(const vector<double>& update)
{
    ostringstream osstream;

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
        physical_bus = network_db->get_physical_bus_number_of_internal_bus(internal_bus);

        bus = db->get_bus(physical_bus);

        current_angle = bus->get_angle_in_rad();

        delta_angle = update[i];

        bus->set_angle_in_rad(current_angle + alpha*delta_angle);

        if(fabs(delta_angle)> fabs(max_delta_angle))
        {
            max_delta_angle = delta_angle;
            max_delta_angle_bus = physical_bus;
        }
    }
    osstream<<"Max angle update: "<<max_delta_angle<<" rad at physical bus "<<max_delta_angle_bus<<".";
    show_information_with_leading_time_stamp(osstream);
}

void POWERFLOW_SOLVER::show_powerflow_result() const
{
    if(not is_power_system_database_set())
        return;

    ostringstream osstream;

    if(is_converged())
        osstream<<"Powerflow converged within "<<get_iteration_count()<<" iterations. Results are listed as follows.";
    else
        osstream<<"Powerflow failed to converg in "<<get_iteration_count()<<" iterations. Results of the last iteration are listed as follows.";
    show_information_with_leading_time_stamp(osstream);

    osstream<<"Solved bus voltage and angle:";
    show_information_with_leading_time_stamp(osstream);

    osstream<<"bus      voltage(pu) angle(deg)";
    show_information_with_leading_time_stamp(osstream);

    vector<BUS*> buses = db->get_all_buses();
    size_t nbus = buses.size();
    /*if(nbus>200)
        nbus = 200;*/
    for(size_t i=0; i!=nbus; ++i)
    {
        osstream<<setw(8)<<buses[i]->get_bus_number()<<" "
          <<setw(10)<<setprecision(6)<<fixed<<buses[i]->get_voltage_in_pu()<<" "
          <<setw(10)<<setprecision(6)<<fixed<<buses[i]->get_angle_in_deg();
        show_information_with_leading_time_stamp(osstream);
    }
    osstream<<"Solved machine power:";
    show_information_with_leading_time_stamp(osstream);

    osstream<<"bus      id   P(MW)      Q(MVar)";
    show_information_with_leading_time_stamp(osstream);

    vector<SOURCE*> sources = db->get_all_sources();
    size_t nsource = sources.size();
    /*if(nsource>200)
        nsource = 200;*/
    for(size_t i=0; i!=nsource; ++i)
    {
        osstream<<setw(8)<<sources[i]->get_source_bus()<<" "
          <<"'"<<setw(2)<<sources[i]->get_identifier()<<"' "
          <<setw(10)<<setprecision(6)<<fixed<<sources[i]->get_p_generation_in_MW()<<" "
          <<setw(10)<<setprecision(6)<<fixed<<sources[i]->get_q_generation_in_MVar();
        show_information_with_leading_time_stamp(osstream);
    }
}
void POWERFLOW_SOLVER::save_powerflow_result_to_file(string filename) const
{
    if(not is_power_system_database_set())
        return;

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
    snprintf(buffer, 1000, "%s", (db->get_case_title_1()).c_str());
    file<<"% "<<buffer<<endl;
    snprintf(buffer, 1000, "%s", (db->get_case_title_2()).c_str());
    file<<"% "<<buffer<<endl;
    vector<BUS*> buses = db->get_all_buses();
    size_t nbus = buses.size();
    if(nbus>0)
    {
        file<<"% Bus"<<endl;
        file<<"BUS,VBASE/KV,VOLTAGE/PU,ANGLE/DEG,VOLTAGE/KV,ANGLE/RAD"<<endl;
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

    vector<GENERATOR*> generators = db->get_all_generators();
    size_t ngen = generators.size();
    if(ngen>0)
    {
        file<<"% Generator"<<endl;
        file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
    }
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
                 db->get_bus_voltage_in_pu(bus));
        file<<buffer<<endl;
    }

    vector<WT_GENERATOR*> wt_generators = db->get_all_wt_generators();
    size_t nsource = wt_generators.size();
    if(nsource>0)
    {
        file<<"% WT generator"<<endl;
        file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
    }
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
                 db->get_bus_voltage_in_pu(bus));
        file<<buffer<<endl;
    }

    vector<LOAD*> loads = db->get_all_loads();
    size_t nload = loads.size();
    if(nload>0)
    {
        file<<"% Load"<<endl;
        file<<"BUS,ID,P/MW,Q/MVAR,VOLTAGE/PU"<<endl;
    }
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
                 s.real(), s.imag(), db->get_bus_voltage_in_pu(bus));
        file<<buffer<<endl;
    }

    vector<LINE*> lines = db->get_all_lines();
    size_t nline = lines.size();
    if(nline>0)
    {
        file<<"% Line"<<endl;
        file<<"IBUS,JBUS,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,II/KA,IJ/KA"<<endl;
    }
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

    vector<TRANSFORMER*> transformers = db->get_all_transformers();
    size_t ntrans = transformers.size();
    if(ntrans>0)
    {
        file<<"% Transformer"<<endl;
        file<<"IBUS,JBUS,KBUS,ID,PI/MW,QI/MVAR,PJ/MW,QJ/MVAR,PK/MW,QK/MVAR,KI/PU,KJ/PU,KK/PU"<<endl;
    }
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

    vector<HVDC*> hvdcs = db->get_all_hvdcs();
    size_t nhvdc = hvdcs.size();
    if(nhvdc>0)
    {
        file<<"% Hvdc"<<endl;
        file<<"IBUS,JBUS,ID,PR/MW,QR/MVAR,PI/MW,QI/MVAR,ALPHA/DEG,GAMMA/DEG,VDCR/KV,VDCI/KV,IDC/KA,VACR/PU,VACI/PU,KR/PU,KI/PU"<<endl;
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
                 db->get_bus_voltage_in_pu(busr),
                 db->get_bus_voltage_in_pu(busi),
                 hvdcs[i]->get_converter_transformer_tap_in_pu(RECTIFIER),
                 hvdcs[i]->get_converter_transformer_tap_in_pu(INVERTER));
        file<<buffer<<endl;
    }

    file.close();
}
void POWERFLOW_SOLVER::save_network_matrix_to_file(string filename) const
{
    if(network_db!=NULL)
        network_db->save_network_matrix_to_file(filename);
}

void POWERFLOW_SOLVER::save_jacobian_matrix_to_file(string filename) const
{
    JACOBIAN_BUILDER jacobian_builder;
    jacobian_builder.set_network_database(network_db);

    jacobian_builder.build_seprate_jacobians();

    jacobian_builder.save_jacobian_matrix_to_file(filename);
}

void POWERFLOW_SOLVER::save_bus_powerflow_result_to_file(string filename) const
{
    if(not is_power_system_database_set())
        return;

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

    vector<BUS*> buses = db->get_all_buses();
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
    if(not is_power_system_database_set())
        return 0;

    return iteration_count;
}
