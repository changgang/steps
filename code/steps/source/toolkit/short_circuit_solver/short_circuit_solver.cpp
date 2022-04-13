#include "header/toolkit/short_circuit_solver/short_circuit_solver.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
#include <fstream>
using namespace std;

SHORT_CIRCUIT_SOLVER::SHORT_CIRCUIT_SOLVER(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
    clear();
}

SHORT_CIRCUIT_SOLVER::~SHORT_CIRCUIT_SOLVER()
{
    toolkit = NULL;
}

STEPS& SHORT_CIRCUIT_SOLVER::get_toolkit() const
{
    return *toolkit;
}

void SHORT_CIRCUIT_SOLVER::clear()
{
    set_generator_reactance_option(SUBTRANSIENT_REACTANCE);
    set_units_of_currents_and_voltages(PU);
    set_coordinates_of_currents_and_voltages(RECTANGULAR);

    faulted_bus_pointer = NULL;
    faulted_line_pointer = NULL;

    If1 = 0.0;
    If2 = 0.0;
    If0 = 0.0;

    Z1 = 0.0;
    Z2 = 0.0;
    Z0 = 0.0;

    fault.clear();
}

NETWORK_MATRIX& SHORT_CIRCUIT_SOLVER::get_network_matrix()
{
    return toolkit->get_network_matrix();
}

void SHORT_CIRCUIT_SOLVER::initialize_short_circuit_solver()
{
    char buffer[STEPS_MAX_TEMP_CHAR_BUFFER_SIZE];
    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Initializing short circuit solver.");
    toolkit->show_information_with_leading_time_stamp(buffer);

    show_short_circuit_solver_configuration();

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    psdb.update_in_service_bus_count();

    store_bus_initial_voltage_before_short_circuit();
    update_all_generator_E();

    snprintf(buffer, STEPS_MAX_TEMP_CHAR_BUFFER_SIZE, "Done initializing short circuit solver.");
    toolkit->show_information_with_leading_time_stamp(buffer);
}

void SHORT_CIRCUIT_SOLVER::show_short_circuit_solver_configuration()
{
    ostringstream osstream;

    string gen_X;
    switch(get_generator_reactance_option())
    {
        case SUBTRANSIENT_REACTANCE: gen_X = "Subtransient reactance";break;
        case TRANSIENT_REACTANCE:    gen_X = "Transient reactance";   break;
        case SYNCHRONOUS_REACTANCE:  gen_X = "Synchronous reactance"; break;
    }
    osstream<<"Configuration of short circuit solver:\n"
            <<"Generator reactance option: "<<gen_X<<"\n";

}

void SHORT_CIRCUIT_SOLVER::store_bus_initial_voltage_before_short_circuit()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    vector<BUS*> buses = psdb.get_all_buses();

    unsigned int n = buses.size();
    for(unsigned int i=0; i<n; i++)
    {
        unsigned int bus = i+1;
        complex<double> V = psdb.get_bus_positive_sequence_complex_voltage_in_pu(bus);
        bus_initial_voltage_before_short_circuit.push_back(V);
    }
}
complex<double> SHORT_CIRCUIT_SOLVER::get_bus_initial_voltage_before_short_circuit(unsigned int bus)
{
    if(bus > bus_initial_voltage_before_short_circuit.size())
        return 0.0;
    return bus_initial_voltage_before_short_circuit[bus - 1];
}

void SHORT_CIRCUIT_SOLVER::update_all_generator_E()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<GENERATOR*> gens = psdb.get_all_generators();
    unsigned int n = gens.size();
    for(unsigned int i=0; i<n; i++)
        gens[i]->update_E();

    vector<WT_GENERATOR*> wt_gens = psdb.get_all_wt_generators();
    n = wt_gens.size();
    for(unsigned int i=0; i<n; i++)
        wt_gens[i]->update_E();
}

void SHORT_CIRCUIT_SOLVER::set_generator_reactance_option(GENERATOR_REACTANCE_OPTION gen_X_option)
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.set_generator_reactance_option(gen_X_option);

    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    vector<GENERATOR*> generators = psdb.get_all_generators();
    unsigned int n = generators.size();
    for(unsigned int i=0; i<n; i++)
        generators[i]->set_generator_reactance_option(gen_X_option);

    vector<WT_GENERATOR*> wt_generators = psdb.get_all_wt_generators();
    n = wt_generators.size();
    for(unsigned int i=0; i<n; i++)
        wt_generators[i]->set_generator_reactance_option(gen_X_option);
}

GENERATOR_REACTANCE_OPTION SHORT_CIRCUIT_SOLVER::get_generator_reactance_option()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    return network_matrix.get_generator_reactance_option();
}

void SHORT_CIRCUIT_SOLVER::set_units_of_currents_and_voltages(UNITS_OPTION option)
{
    units_of_currents_and_voltages = option;
}
UNITS_OPTION SHORT_CIRCUIT_SOLVER::get_units_of_currents_and_voltages()
{
    return units_of_currents_and_voltages;
}

void SHORT_CIRCUIT_SOLVER::set_coordinates_of_currents_and_voltages(COORDINATES_OPTION option)
{
    coordindates_of_currents_and_volatges = option;
}
COORDINATES_OPTION SHORT_CIRCUIT_SOLVER::get_coordinates_of_currents_and_voltages()
{
    return coordindates_of_currents_and_volatges;
}

void SHORT_CIRCUIT_SOLVER::set_bus_fault(unsigned int bus, FAULT_TYPE type, const complex<double>& fault_shunt)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(is_line_fault() || is_bus_fault())
    {
        osstream<<"Fault has been set. The calculation of mulitiple faults is not supported."<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    initialize_short_circuit_solver();

    BUS* busptr = psdb.get_bus(bus);
    if(busptr!=NULL)
    {
        string busname = busptr->get_bus_name();
        if(steps_fast_complex_abs(fault_shunt)>DOUBLE_EPSILON)
        {
            // FAULT fault;
            fault.set_fault_type(type);
            fault.set_fault_shunt_in_pu(fault_shunt);

            if(busptr->get_bus_type()==OUT_OF_SERVICE)
            {
                osstream<<"No "<<fault.get_fault_type_string()<<" will be set for bus "<<bus<<"["<<busname<<"], since it is out of service."<<endl;
                toolkit->show_information_with_leading_time_stamp(osstream);
                return;
            }
            osstream<<fault.get_fault_type_string()<<" will be set for bus "<<bus<<"["<<busname<<"]."<<endl
                   <<"Fault shunt is"<<fault_shunt<<" pu.";
            toolkit->show_information_with_leading_time_stamp(osstream);

            busptr->set_fault(fault);
            faulted_bus_pointer = busptr;
        }
        else
        {
            osstream<<"Zero fault shunt is given for bus "<<bus<<"["<<busname<<"]."<<endl
                    <<"No bus fault will be set.";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"Bus "<<bus<<" does not exist in power system database."<<endl
                <<"No bus fault will be set "<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void SHORT_CIRCUIT_SOLVER::set_line_fault(const DEVICE_ID& line_id, unsigned int side_bus, double location, FAULT_TYPE fault_type, const complex<double>& fault_shunt)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if(is_line_fault() || is_bus_fault())
    {
        osstream<<"Fault has been set. The calculation of mulitiple faults is not supported."<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }

    initialize_short_circuit_solver();

    if(line_id.get_device_type()==STEPS_LINE)
    {
        LINE* lineptr = psdb.get_line(line_id);
        if(lineptr!=NULL)
        {
            string ibusname = psdb.bus_number2bus_name(lineptr->get_sending_side_bus());
            string jbusname = psdb.bus_number2bus_name(lineptr->get_receiving_side_bus());
            if(lineptr->is_connected_to_bus(side_bus))
            {
                if(lineptr->get_sending_side_breaker_status()==false and lineptr->get_receiving_side_breaker_status()==false)
                {
                    osstream<<"No fault will be set for "<<lineptr->get_compound_device_name()<<", since it is out of service."<<endl;
                    toolkit->show_information_with_leading_time_stamp(osstream);
                    return;
                }
                if(location>=0.0 and location<=1.0)
                {
                    if(steps_fast_complex_abs(fault_shunt)>DOUBLE_EPSILON)
                    {
                        //  FAULT fault;
                        fault.set_fault_type(fault_type);
                        fault.set_fault_shunt_in_pu(fault_shunt);

                        osstream<<fault.get_fault_type_string()<<" will be set for "<<lineptr->get_compound_device_name()<<endl
                               <<"Fault shunt is "<<fault_shunt<<" pu at location "<<location<<" to bus "<<side_bus;
                        toolkit->show_information_with_leading_time_stamp(osstream);

                        lineptr->set_fault(side_bus,location, fault);
                        faulted_line_pointer = lineptr;
                    }
                    else
                    {
                        osstream<<"Zero fault shunt is given for "<<lineptr->get_compound_device_name()<<"."<<endl
                               <<"No fault will be set for "<<lineptr->get_compound_device_name();
                        toolkit->show_information_with_leading_time_stamp(osstream);
                    }
                }
                else
                {
                    osstream<<"Warning. Fault location ("<<location<<") is out of allowed range [0.0, 1.0] for "<<lineptr->get_compound_device_name()<<"."<<endl
                           <<"No fault will be set";
                    toolkit->show_information_with_leading_time_stamp(osstream);
                }
            }
            else
            {
                osstream<<lineptr->get_compound_device_name()<<"is not connected to bus "<<side_bus<<endl
                       <<"No fault will be set at location "<<location<<" to bus "<<side_bus;
                toolkit->show_information_with_leading_time_stamp(osstream);
            }
        }
        else
        {
            osstream<<"Warning. "<<line_id.get_compound_device_name()<<" does not exist in power system database."<<endl
                    <<"No fault will be set";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        osstream<<"The given device is not a LINE (it is a "<<line_id.get_device_type()<<") for setting line fault."<<endl
                <<"No line fault will be set"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

string SHORT_CIRCUIT_SOLVER::get_fault_information()
{
    char infor[200];
    if(is_bus_fault())
    {
        FAULT fault = faulted_bus_pointer->get_fault();
        string fault_type = fault.get_fault_type_string();
        complex<double> fault_shunt = fault.get_fault_shunt_in_pu();
        unsigned int busnum = faulted_bus_pointer->get_bus_number();
        string busname = faulted_bus_pointer->get_bus_name();

        snprintf(infor, 200, "%s at bus %u [%s].\nFault shunt is (%.f %.f) pu.",
                fault_type.c_str(), busnum, busname.c_str(), fault_shunt.real(), fault_shunt.imag());
    }
    else if(is_line_fault())
    {
        unsigned int ibus = faulted_line_pointer->get_sending_side_bus();
        double fault_location = faulted_line_pointer->get_fault_location_of_fault(0);
        FAULT fault = faulted_line_pointer->get_fault_at_location(ibus, fault_location);
        string fault_type = fault.get_fault_type_string();
        complex<double> fault_shunt = fault.get_fault_shunt_in_pu();
        string linename = faulted_line_pointer->get_compound_device_name();
        snprintf(infor, 200, "%s at line [%s].\nFault shunt is (%.f %.f) pu at location %.2f to %u.",
                fault_type.c_str(), linename.c_str(), fault_shunt.real(), fault_shunt.imag(), fault_location, ibus);
    }
    return infor;
}

bool SHORT_CIRCUIT_SOLVER::is_bus_fault()
{
    if(faulted_bus_pointer!=NULL)
        return true;
    else
        return false;
}
bool SHORT_CIRCUIT_SOLVER::is_line_fault()
{
    if(faulted_line_pointer!=NULL)
        return true;
    else
        return false;
}
bool SHORT_CIRCUIT_SOLVER::is_fault()
{
    if(is_bus_fault()==false and is_line_fault()==false)
        return false;
    else
        return true;
}

FAULT_TYPE SHORT_CIRCUIT_SOLVER::get_fault_type()
{
    return fault.get_fault_type();
}

void SHORT_CIRCUIT_SOLVER::build_sequence_network()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.build_sequence_network_Y_matrix();
}

unsigned int SHORT_CIRCUIT_SOLVER::get_internal_bus_number_of_physical_bus(unsigned int bus)
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    return network_matrix.get_internal_bus_number_of_physical_bus(bus);
}
unsigned int SHORT_CIRCUIT_SOLVER::get_physical_bus_number_of_internal_bus(unsigned int bus)
{
    NETWORK_MATRIX& network_metrix = get_network_matrix();
    return network_metrix.get_physical_bus_number_of_internal_bus(bus);
}


void SHORT_CIRCUIT_SOLVER::calculate_and_store_equivalent_impedance_between_bus_and_fault_place()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    if(is_bus_fault())
    {
        unsigned int bus = faulted_bus_pointer->get_bus_number();
        Z1_between_internal_bus_to_fault_place = network_matrix.get_positive_sequence_complex_impedance_of_column_with_physical_bus(bus);
        Z2_between_internal_bus_to_fault_place = network_matrix.get_negative_sequence_complex_impedance_of_column_with_physical_bus(bus);
        Z0_between_internal_bus_to_fault_place = network_matrix.get_zero_sequence_complex_impedance_of_column_with_physical_bus(bus);

        unsigned int internal_bus = get_internal_bus_number_of_physical_bus(bus);
        Z1 = Z1_between_internal_bus_to_fault_place[internal_bus];
        Z2 = Z2_between_internal_bus_to_fault_place[internal_bus];
        Z0 = Z0_between_internal_bus_to_fault_place[internal_bus];
    }
    else if(is_line_fault())
    {
        double fault_locaton = faulted_line_pointer->get_fault_location_of_fault(0);

        unsigned int ibus = faulted_line_pointer->get_sending_side_bus();
        unsigned int jbus = faulted_line_pointer->get_receiving_side_bus();
        unsigned int internal_ibus = get_internal_bus_number_of_physical_bus(ibus);
        unsigned int internal_jbus = get_internal_bus_number_of_physical_bus(jbus);

        vector<complex<double> > Z1i = network_matrix.get_positive_sequence_complex_impedance_of_column_with_physical_bus(ibus);
        vector<complex<double> > Z2i = network_matrix.get_negative_sequence_complex_impedance_of_column_with_physical_bus(ibus);
        vector<complex<double> > Z0i = network_matrix.get_zero_sequence_complex_impedance_of_column_with_physical_bus(ibus);

        vector<complex<double> > Z1j = network_matrix.get_positive_sequence_complex_impedance_of_column_with_physical_bus(jbus);
        vector<complex<double> > Z2j = network_matrix.get_negative_sequence_complex_impedance_of_column_with_physical_bus(jbus);
        vector<complex<double> > Z0j = network_matrix.get_zero_sequence_complex_impedance_of_column_with_physical_bus(jbus);

        complex<double> I, Ii, Ij, Zij, Zif, Zjf, Vi, Vj, V;

        // positive
        Zij = faulted_line_pointer->get_line_positive_sequence_z_in_pu();
        Zif = Zij*fault_locaton;
        Zjf = Zij*(1.0-fault_locaton);
        Ii = 1.0/Zif;
        Ij = 1.0/Zjf;

        Vi = Z1i[internal_ibus]*Ii + Z1j[internal_ibus]*Ij;
        Vj = Z1i[internal_jbus]*Ii + Z1j[internal_jbus]*Ij;
        I = (1.0-Vi)/Zif + (1.0-Vj)/Zjf;
        Z1 = 1.0/I;     // equivalent self impedance

        unsigned int n = Z1i.size();
        for(unsigned int k=0; k<n; k++)
        {
            V = Z1i[k]*Ii +Z1j[k]*Ij;
            Z1_between_internal_bus_to_fault_place.push_back(V/I);
        }


        // negative
        Zij = faulted_line_pointer->get_line_positive_sequence_z_in_pu();
        Zif = Zij*fault_locaton;
        Zjf = Zij*(1.0-fault_locaton);
        Ii = 1.0/Zif;
        Ij = 1.0/Zjf;

        Vi = Z2i[internal_ibus]*Ii + Z2j[internal_ibus]*Ij;
        Vj = Z2i[internal_jbus]*Ii + Z2j[internal_jbus]*Ij;
        I = (1.0-Vi)/Zif + (1.0-Vj)/Zjf;
        Z2 = 1.0/I;     // equivalent self impedance

        n = Z2i.size();
        for(unsigned int k=0; k<n; k++)
        {
            V = Z2i[k]*Ii + Z2j[k]*Ij;
            Z2_between_internal_bus_to_fault_place.push_back(V/I);
        }

        // zero
        Zij = faulted_line_pointer->get_line_zero_sequence_z_in_pu();
        Zif = Zij*fault_locaton;
        Zjf = Zij*(1.0-fault_locaton);
        Ii = 1.0/Zif;
        Ij = 1.0/Zjf;

        Vi = Z0i[internal_ibus]*Ii + Z0j[internal_ibus]*Ij;
        Vj = Z0i[internal_jbus]*Ii + Z0j[internal_jbus]*Ij;
        I = (1.0-Vi)/Zif + (1.0-Vj)/Zjf;
        Z0 = 1.0/I;     // equivalent self impedance

        n = Z0i.size();
        for(unsigned int k=0; k<n; k++)
        {
            V = Z0i[k]*Ij + Z0j[k]*Ij;
            Z0_between_internal_bus_to_fault_place.push_back(V/I);
        }

    }
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(unsigned int bus)
{
    unsigned int b = get_internal_bus_number_of_physical_bus(bus);
    return Z1_between_internal_bus_to_fault_place[b];
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(unsigned int bus)
{
    unsigned int b = get_internal_bus_number_of_physical_bus(bus);
    return Z2_between_internal_bus_to_fault_place[b];
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(unsigned int bus)
{
    unsigned int b = get_internal_bus_number_of_physical_bus(bus);
    return Z0_between_internal_bus_to_fault_place[b];
}

void SHORT_CIRCUIT_SOLVER::solve()
{
    ostringstream osstream;
    if(is_fault())
    {
        build_sequence_network();
        calculate_and_store_equivalent_impedance_between_bus_and_fault_place();
        Zf = 1.0/fault.get_fault_shunt_in_pu();
        complex<double> Uf;
        if(is_bus_fault())
            Uf = get_bus_initial_voltage_before_short_circuit(faulted_bus_pointer->get_bus_number());
        else if(is_line_fault())
            Uf = get_voltage_of_faulted_line_point_before_short_circuit();

        solve_fault_current(fault.get_fault_type(), Uf);

        update_bus_sequence_voltage();
    }
    else
    {
        osstream<<"No fault has been set."<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void SHORT_CIRCUIT_SOLVER::update_bus_sequence_voltage()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    vector<BUS*> buses = psdb.get_all_buses();
    unsigned int n = buses.size();
    for(unsigned int i = 0; i!=n; ++i)
    {
        BUS& bus = *buses[i];
        unsigned int busnum = bus.get_bus_number();
        complex<double> Z1 = get_positive_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(busnum);
        complex<double> Z2 = get_negative_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(busnum);
        complex<double> Z0 = get_zero_sequence_equivalent_mutual_impedance_in_pu_between_bus_to_fault_place(busnum);

        complex<double> U1 = get_bus_initial_voltage_before_short_circuit(busnum);
        complex<double> U2 = 0.0;
        complex<double> U0 = 0.0;

        U1 = U1 - Z1 * If1;
        U2 = U2 - Z2 * If2;
        U0 = U0 - Z0 * If0;

        bus.set_positive_sequence_voltage_in_pu(abs(U1));
        bus.set_positive_sequence_angle_in_rad(arg(U1));
        bus.set_negative_sequence_voltage_in_pu(abs(U2));
        bus.set_negative_sequence_angle_in_rad(arg(U2));
        bus.set_zero_sequence_voltage_in_pu(abs(U0));
        bus.set_zero_sequence_angle_in_rad(arg(U0));
    }
}


void SHORT_CIRCUIT_SOLVER::solve_fault_current(FAULT_TYPE fault_type, complex<double> Uf)
{
    if(fault_type == SINGLE_PHASE_GROUNDED_FAULT)
    {
        If1 = Uf / (Z1 + Z2 + Z0 + 3.0*Zf);
        If2 = If1;
        If0 = If1;
    }
    else if(fault_type == DOUBLE_PHASES_FAULT)
    {
        If1 = Uf / (Z1 + Z2 + Zf);
        If2 = -If1;
        If0 = complex<double> (0.0, 0.0);
    }
    else if(fault_type == DOUBLE_PHASES_GROUNDED_FAULT)
    {
        If1 = Uf / (Z1 + Z2*(Z0+3.0*Zf)/(Z2+Z0+3.0*Zf));
        If2 = -(Z0 + 3.0*Zf)/(Z2 + Z0 + 3.0*Zf) * If1;
        If0 = -Z2/(Z2 + Z0 + 3.0*Zf) * If1;
    }
    else if(fault_type == THREE_PHASES_FAULT)
    {
        If1 = Uf / (Z1 + Zf);
        If2 = complex<double> (0.0, 0.0);
        If0 = complex<double> (0.0, 0.0);
    }
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_fault_current_in_pu()
{
    return If1;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_fault_current_in_pu()
{
    return If2;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_fault_current_in_pu()
{
    return If0;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_fault_current_in_kA()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    if(is_bus_fault())
        return If1*sbase/(SQRT3*faulted_bus_pointer->get_base_voltage_in_kV());
    else
        return If1*sbase/(SQRT3*faulted_line_pointer->get_line_base_voltage_in_kV());
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_fault_current_in_kA()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    if(is_bus_fault())
        return If2*sbase/(SQRT3*faulted_bus_pointer->get_base_voltage_in_kV());
    else
        return If2*sbase/(SQRT3*faulted_line_pointer->get_line_base_voltage_in_kV());
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_fault_current_in_kA()
{
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    if(is_bus_fault())
        return If0*sbase/(SQRT3*faulted_bus_pointer->get_base_voltage_in_kV());
    else
        return If0*sbase/(SQRT3*faulted_line_pointer->get_line_base_voltage_in_kV());
}




complex<double> SHORT_CIRCUIT_SOLVER::get_voltage_of_faulted_line_point_before_short_circuit()
{
    double fault_location = faulted_line_pointer->get_fault_location_of_fault(0);
    unsigned int ibus = faulted_line_pointer->get_sending_side_bus();
    unsigned int jbus = faulted_line_pointer->get_receiving_side_bus();

    complex<double> Ui = get_bus_initial_voltage_before_short_circuit(ibus);
    complex<double> Uj = get_bus_initial_voltage_before_short_circuit(jbus);

    complex<double> Zline = faulted_line_pointer->get_line_zero_sequence_z_in_pu();
    complex<double> Yline = faulted_line_pointer->get_line_zero_sequence_y_in_pu();

    complex<double> Zif = Zline*fault_location;
    complex<double> Zjf = Zline*(1.0-fault_location);

    // complex<double> Yeq = 0.5*Yline + 1.0/Zif + 1.0/Zjf;
    complex<double> Yeq = 1.0/Zif + 1.0/Zjf;
    complex<double> Istar = Ui/Zif + Uj/Zjf;

    return Istar/Yeq;
}



complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_equivalent_fault_admittance()
{
    complex<double> U1 = get_positive_sequence_voltage_of_fault_point_in_pu();
    return If1/U1;
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_short_circuit_capacity_in_pu()
{
    complex<double> Uf1 = get_positive_sequence_voltage_of_fault_point_in_pu();
    return If1*Uf1;
}

complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_short_circuit_capacity_in_pu()
{
    complex<double> Uf2 = get_negative_sequence_voltage_of_fault_point_in_pu();
    return If2*Uf2;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_short_circuit_capacity_in_pu()
{
    complex<double> Uf0 = get_zero_sequence_voltage_of_fault_point_in_pu();
    return If0*Uf0;
}

void SHORT_CIRCUIT_SOLVER::show_contributions_of_fault_current_with_line_fault()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    UNITS_OPTION units = get_units_of_currents_and_voltages();
    double Ibase = (units==PU)?1.0:sbase/(SQRT3*faulted_line_pointer->get_line_base_voltage_in_kV());
    string id = faulted_line_pointer->get_identifier();
    double Vbase = faulted_line_pointer->get_line_base_voltage_in_kV();

    unsigned int ibus = faulted_line_pointer->get_sending_side_bus();
    unsigned int jbus = faulted_line_pointer->get_receiving_side_bus();
    BUS* ibusptr = faulted_line_pointer->get_sending_side_bus_pointer();
    BUS* jbusptr = faulted_line_pointer->get_receiving_side_bus_pointer();

    double fault_location = faulted_line_pointer->get_fault_location_of_fault(0);
    complex<double> Z1ij = faulted_line_pointer->get_line_positive_sequence_z_in_pu();
    complex<double> Z1if = Z1ij*fault_location;
    complex<double> Z1jf = Z1ij*(1.0-fault_location);

    complex<double> Z0ij = faulted_line_pointer->get_line_zero_sequence_z_in_pu();
    complex<double> Z0if = Z0ij*fault_location;
    complex<double> Z0jf = Z0ij*(1.0-fault_location);

    complex<double> U1f = get_positive_sequence_voltage_of_fault_point_in_pu();
    complex<double> U2f = get_negative_sequence_voltage_of_fault_point_in_pu();
    complex<double> U0f = get_zero_sequence_voltage_of_fault_point_in_pu();

    complex<double> U1i = ibusptr->get_positive_sequence_complex_voltage_in_pu();
    complex<double> U2i = ibusptr->get_negative_sequence_complex_voltage_in_pu();
    complex<double> U0i = ibusptr->get_zero_sequence_complex_voltage_in_pu();
    complex<double> I1if = (U1i - U1f)/Z1if;
    complex<double> I2if = (U2i - U2f)/Z1if;
    complex<double> I0if = (U0i - U0f)/Z0if;
    if(units == PHYSICAL)
    {
        I1if = I1if*Ibase;
        I2if = I2if*Ibase;
        I0if = I0if*Ibase;
    }
    toolkit->show_information_with_leading_time_stamp(get_formatted_information1(ibus,id, I1if, I2if, I0if));
    toolkit->show_information_with_leading_time_stamp(get_formatted_information2(ibusptr->get_bus_name(), Vbase,I1if, I2if, I0if));

    complex<double> U1j = jbusptr->get_positive_sequence_complex_voltage_in_pu();
    complex<double> U2j = jbusptr->get_negative_sequence_complex_voltage_in_pu();
    complex<double> U0j = jbusptr->get_zero_sequence_complex_voltage_in_pu();
    complex<double> I1jf = (U1j - U1f)/Z1jf;
    complex<double> I2jf = (U2j - U2f)/Z1jf;
    complex<double> I0jf = (U0j - U0f)/Z0jf;
    if(units == PHYSICAL)
    {
        I1jf = I1jf*Ibase;
        I2jf = I2jf*Ibase;
        I0jf = I0jf*Ibase;
    }
    toolkit->show_information_with_leading_time_stamp(get_formatted_information1(jbus,id, I1jf, I2jf, I0jf));
    toolkit->show_information_with_leading_time_stamp(get_formatted_information2(ibusptr->get_bus_name(),Vbase,I1jf, I2jf, I0jf));
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_voltage_of_fault_point_in_pu()
{
    if(is_bus_fault())
        return faulted_bus_pointer->get_positive_sequence_complex_voltage_in_pu();
    else if(is_line_fault())
        return get_positive_sequence_voltage_at_line_fault_location_in_pu();
    else
        return 0.0;
}

complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_voltage_of_fault_point_in_pu()
{
    if(is_bus_fault())
        return faulted_bus_pointer->get_negative_sequence_complex_voltage_in_pu();
    else if(is_line_fault())
        return get_negative_sequence_voltage_at_line_fault_location_in_pu();
    else
        return 0.0;
}

complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_voltage_of_fault_point_in_pu()
{
    if(is_bus_fault())
        return faulted_bus_pointer->get_zero_sequence_complex_voltage_in_pu();
    else if(is_line_fault())
        return get_zero_sequence_voltage_at_line_fault_location_in_pu();
    else
        return 0.0;
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_voltage_of_fault_point_in_kV()
{
    if(is_bus_fault())
        return faulted_bus_pointer->get_positive_sequence_complex_voltage_in_kV()/SQRT3;
    else if(is_line_fault())
        return get_positive_sequence_voltage_at_line_fault_location_in_kV();
    else
        return 0.0;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_voltage_of_fault_point_in_kV()
{
    if(is_bus_fault())
        return faulted_bus_pointer->get_negative_sequence_complex_voltage_in_kV()/SQRT3;
    else if(is_line_fault())
        return get_negative_sequence_voltage_at_line_fault_location_in_kV();
    else
        return 0.0;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_voltage_of_fault_point_in_kV()
{
    if(is_bus_fault())
        return faulted_bus_pointer->get_zero_sequence_complex_voltage_in_kV()/SQRT3;
    else if(is_line_fault())
        return get_zero_sequence_voltage_at_line_fault_location_in_kV();
    else
        return 0.0;
}

complex<double>* SHORT_CIRCUIT_SOLVER::convert_sequence_data_to_phase_data(complex<double> F1, complex<double> F2, complex<double> F0)
{
    complex<double> Fa, Fb, Fc;
    complex<double> *F_abc = new complex<double>[3];
    F_abc[0] = F1 + F2 + F0;
    F_abc[1] = complex<double>(-0.5, -SQRT3/2)*F1 + complex<double>(-0.5,  SQRT3/2)*F2 + F0;
    F_abc[2] = complex<double>(-0.5,  SQRT3/2)*F1 + complex<double>(-0.5, -SQRT3/2)*F2 + F0;

    return F_abc;
}


string SHORT_CIRCUIT_SOLVER::get_formatted_information1(unsigned int bus,string ID,complex<double> F1, complex<double>F2, complex<double> F0, bool to_file)
{
    ostringstream osstream;
    string msg = (ID=="")?"":"CKT ";
    string s = (to_file)?",":"";
    osstream<< left << setw(12) << bus << s << setw(4) << msg << setw(4) << ID;

    return get_formatted_information1(osstream.str(), F1, F2, F0, to_file);
}

string SHORT_CIRCUIT_SOLVER::get_formatted_information1(string str, complex<double> F1, complex<double> F2, complex<double> F0, bool to_file)
{
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();
    string s = (to_file)?",":"";
    ostringstream osstream;
    if(coordinate ==  RECTANGULAR)
    {
        osstream<< left << setw(20) << str << s
                << right << fixed << setprecision(4)
                << setw(10) << F0.real() << s << setw(10) << F0.imag() << s
                << setw(10) << F1.real() << s << setw(10) << F1.imag() << s
                << setw(10) << F2.real() << s << setw(10) << F2.imag() << s
                << setw(10) << 3*F0.real()<< s << setw(10) << 3*F0.imag() << s <<endl;
    }
    else if(coordinate == POLAR)
    {
        double amp1 = abs(F1), angle1 = rad2deg(arg(F1));
        double amp2 = abs(F2), angle2 = rad2deg(arg(F2));
        double amp0 = abs(F0), angle0 = rad2deg(arg(F0));
        osstream<< left << setw(20) << str << s
                << right << fixed << setprecision(4)
                << setw(10) << amp0 << s << setw(10) << angle0 << s
                << setw(10) << amp1 << s << setw(10) << angle1 << s
                << setw(10) << amp2 << s << setw(10) << angle2 << s
                << setw(10) << 3*amp0 << s << setw(10) << angle0 << s<<endl;
    }
    return osstream.str();
}

string SHORT_CIRCUIT_SOLVER::get_formatted_information2(string busname,double Vbase, complex<double> F1, complex<double>F2, complex<double> F0, bool to_file)
{
    ostringstream osstream;
    string s = (to_file)?",":"";
    osstream<< resetiosflags(ios::fixed)
            << left << setw(12) << busname << s
            << setprecision(5) << showpoint << setw(8) << Vbase;
    return get_formatted_information2(osstream.str(), F1, F2, F0, to_file);
}

string SHORT_CIRCUIT_SOLVER::get_formatted_information2(string str, complex<double> F1, complex<double> F2, complex<double> F0, bool to_file)
{
    string s = (to_file)?",":"";
    complex<double> *Fabc = convert_sequence_data_to_phase_data(F1, F2, F0);
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();
    ostringstream osstream;

    if(coordinate ==  RECTANGULAR)
    {
        osstream<< left << setw(20) << str << s
                << right << fixed << setprecision(4)
                << setw(10) << Fabc[0].real() << s << setw(10) << Fabc[0].imag() << s
                << setw(10) << Fabc[1].real() << s << setw(10) << Fabc[1].imag() << s
                << setw(10) << Fabc[2].real() << s << setw(10) << Fabc[2].imag() << s <<endl;
    }
    else if(coordinate == POLAR)
    {
        double amp0 = abs(Fabc[0]), angle0 = rad2deg(arg(Fabc[0]));
        double amp1 = abs(Fabc[1]), angle1 = rad2deg(arg(Fabc[1]));
        double amp2 = abs(Fabc[2]), angle2 = rad2deg(arg(Fabc[2]));

        osstream<< left << setw(20) << str << s
                << right << fixed << setprecision(4)
                << setw(10) << amp0 << s << setw(10) << angle0 << s
                << setw(10) << amp1 << s << setw(10) << angle1 << s
                << setw(10) << amp2 << s << setw(10) << angle2 << s <<endl;
    }
    return osstream.str();
}

void SHORT_CIRCUIT_SOLVER::show_voltage_table_header()
{
    ostringstream osstream;

    UNITS_OPTION units = get_units_of_currents_and_voltages();
    string unit = (units==PU)?"(P.U.)":"(kV)";
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();

    if(coordinate == RECTANGULAR)
    {
        osstream<< left << setw(12) << "SEQUENCE" << setw(8) << unit
                << right<< setw(10) <<"RE(V0)" << setw(10) << "IM(V0)"
                        << setw(10) <<"RE(V+)" << setw(10) << "IM(V+)"
                        << setw(10) <<"RE(V-)" << setw(10) << "IM(V-)"
                        << setw(10) <<"RE(3V0)"<< setw(10) << "IM(3V0)"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        osstream<< left << setw(20) << "PHASE"
                << right<< setw(10) << "RE(VA)" << setw(10) << "IM(VA)"
                        << setw(10) << "RE(VB)" << setw(10) << "IM(VB)"
                        << setw(10) << "RE(VC)" << setw(10) << "IM(VC)"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    else if(coordinate == POLAR)
    {
        osstream<< left << setw(12) << "SEQUENCE" << setw(8) << unit
                << right<< setw(10) << "/V0/" << setw(10) << "AN(V0)"
                        << setw(10) << "/V+/" << setw(10) << "AN(V+)"
                        << setw(10) << "/V-/" << setw(10) << "AN(V-)"
                        << setw(10) << "/3V0/"<< setw(10) << "AN(3V0)"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        osstream<< left << setw(20) <<  "PHASE"
                << right<< setw(10) << "/VA/" << setw(10) <<"AN(VA)"
                        << setw(10) << "/VB/" << setw(10) <<"AN(VB)"
                        << setw(10) << "/VC/" << setw(10) <<"AN(VC)" <<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}
void SHORT_CIRCUIT_SOLVER::show_current_table_header()
{
    ostringstream osstream;

    UNITS_OPTION units = get_units_of_currents_and_voltages();
    string unit = (units==PU)?"(P.U.)":"(kA)";
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();

    if(coordinate == RECTANGULAR)
    {
        osstream<< left << setw(12) << "SEQUENCE" << setw(8) << unit
                << right<< setw(10) <<"RE(I0)" << setw(10) << "IM(I0)"
                        << setw(10) <<"RE(I+)" << setw(10) << "IM(I+)"
                        << setw(10) <<"RE(I-)" << setw(10) << "IM(I-)"
                        << setw(10) <<"RE(3I0)"<< setw(10) << "IM(3I0)"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        osstream<< left << setw(20) << "PHASE"
                << right<< setw(10) << "RE(IA)" << setw(10) << "IM(IA)"
                        << setw(10) << "RE(IB)" << setw(10) << "IM(IB)"
                        << setw(10) << "RE(IC)" << setw(10) << "IM(IC)"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    else if(coordinate == POLAR)
    {
        osstream<< left << setw(12) << "SEQUENCE" << setw(8) << unit
                << right<< setw(10) << "/I0/" << setw(10) << "AN(I0)"
                        << setw(10) << "/I+/" << setw(10) << "AN(I+)"
                        << setw(10) << "/I-/" << setw(10) << "AN(I-)"
                        << setw(10) << "/3I0/"<< setw(10) << "AN(3I0)"<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
        osstream<< left << setw(20) <<  "PHASE"
                << right<< setw(10) << "/IA/" << setw(10) <<"AN(IA)"
                        << setw(10) << "/IB/" << setw(10) <<"AN(IB)"
                        << setw(10) << "/IC/" << setw(10) <<"AN(IC)" <<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void SHORT_CIRCUIT_SOLVER::show_short_circuit_result()
{
    if(is_bus_fault())
        show_short_circuit_with_bus_fault();
    else if(is_line_fault())
        show_short_circuit_with_line_fault();
}

void SHORT_CIRCUIT_SOLVER::show_short_circuit_with_bus_fault()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();

    UNITS_OPTION unit = get_units_of_currents_and_voltages();

    unsigned int busnum = faulted_bus_pointer->get_bus_number();
    string busname  = faulted_bus_pointer->get_bus_name();
    double Vbase = faulted_bus_pointer->get_base_voltage_in_kV();

    osstream<<"Sequence equivalent impedance (P.U.):"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    osstream<<"BUS     NAME           <--------- ZERO ---------> <-------- POSITIVE -------> <-------- NEGATIVE ------->"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    osstream<< left << setw(8) << faulted_bus_pointer->get_bus_number() << setw(12) << faulted_bus_pointer->get_bus_name()
            << right << fixed << setprecision(5)
            << setw(14) << Z0.real() << setw(14) << Z0.imag()
            << setw(14) << Z1.real() << setw(14) << Z1.imag()
            << setw(14) << Z2.real() << setw(14) << Z2.imag()<< endl;
    toolkit->show_information_with_leading_time_stamp(osstream);

    complex<double> V1, V2, V0;
    if(unit == PU)
    {
        V1 = faulted_bus_pointer->get_positive_sequence_complex_voltage_in_pu();
        V2 = faulted_bus_pointer->get_negative_sequence_complex_voltage_in_pu();
        V0 = faulted_bus_pointer->get_zero_sequence_complex_voltage_in_pu();
    }
    else if(unit == PHYSICAL)
    {
        V1 = faulted_bus_pointer->get_positive_sequence_complex_voltage_in_kV()/SQRT3;
        V2 = faulted_bus_pointer->get_negative_sequence_complex_voltage_in_kV()/SQRT3;
        V0 = faulted_bus_pointer->get_zero_sequence_complex_voltage_in_kV()/SQRT3;
    }
    show_voltage_table_header();
    toolkit->show_information_with_leading_time_stamp(get_formatted_information1(busnum, "", V1, V2, V0));
    toolkit->show_information_with_leading_time_stamp(get_formatted_information2(busname, Vbase , V1, V2, V0));

    show_current_table_header();
    show_contributions_of_fault_current_with_bus_fault();

    complex<double> I1, I2, I0;
    if(unit == PU)
    {
        I1 = get_positive_sequence_fault_current_in_pu();
        I2 = get_negative_sequence_fault_current_in_pu();
        I0 = get_zero_sequence_fault_current_in_pu();
    }
    else if(unit == PHYSICAL)
    {
        I1 = get_positive_sequence_fault_current_in_kA();
        I2 = get_negative_sequence_fault_current_in_kA();
        I0 = get_zero_sequence_fault_current_in_kA();
    }
    osstream<< "Fault current at bus " << faulted_bus_pointer->get_bus_number() << ":" <<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    toolkit->show_information_with_leading_time_stamp(get_formatted_information1(busnum, "", I1, I2, I0));
    toolkit->show_information_with_leading_time_stamp(get_formatted_information2(busname, Vbase, I1, I2, I0));

    osstream<< "Short circuit capacity:  (P.U.) (MVA)" <<endl
            << right << fixed
            << "S1: " << setprecision(4) << V1*I1 << setprecision(2) << V1*I1*sbase <<endl
            << "S2: " << setprecision(4) << V2*I2 << setprecision(2) << V2*I2*sbase <<endl
            << "S0: " << setprecision(4) << V0*I0 << setprecision(2) << V0*I0*sbase <<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);

    osstream<< "Positive sequence equivalent fault admittance: "
            << fixed << setprecision(4) << get_positive_sequence_equivalent_fault_admittance()<<"PU"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
}

void SHORT_CIRCUIT_SOLVER::show_contributions_of_fault_current_with_bus_fault()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    unsigned int fault_bus = faulted_bus_pointer->get_bus_number();
    vector<LINE*> lines = psdb.get_lines_connecting_to_bus(fault_bus);

    UNITS_OPTION units = get_units_of_currents_and_voltages();

    unsigned int n = lines.size();
    for(unsigned int i=0; i<n; i++)
    {
        LINE& line = *lines[i];

        complex<double> I1, I2, I0;
        unsigned int current_from_bus ;
        if(line.get_sending_side_bus() == fault_bus)
        {
            current_from_bus = line.get_receiving_side_bus();
            if(units==PU)
            {
                I1 = line.get_line_positive_sequence_complex_current_at_sending_side_in_pu();
                I2 = line.get_line_negative_sequence_complex_current_at_sending_side_in_pu();
                I0 = line.get_line_zero_sequence_complex_current_at_sending_side_in_pu();
            }
            else
            {
                I1 = line.get_line_positive_sequence_complex_current_at_sending_side_in_kA();
                I2 = line.get_line_negative_sequence_complex_current_at_sending_side_in_kA();
                I0 = line.get_line_zero_sequence_complex_current_at_sending_side_in_kA();
            }
        }
        else if(line.get_receiving_side_bus() == fault_bus)
        {
            current_from_bus = line.get_sending_side_bus();
            if(units == PU)
            {
                I1 = line.get_line_positive_sequence_complex_current_at_receiving_side_in_pu();
                I2 = line.get_line_negative_sequence_complex_current_at_receiving_side_in_pu();
                I0 = line.get_line_zero_sequence_complex_current_at_receiving_side_in_pu();
            }
            else
            {
                I1 = line.get_line_positive_sequence_complex_current_at_receiving_side_in_kA();
                I2 = line.get_line_negative_sequence_complex_current_at_receiving_side_in_kA();
                I0 = line.get_line_zero_sequence_complex_current_at_receiving_side_in_kA();
            }
        }

        BUS* busptr = psdb.get_bus(current_from_bus);
        string busname = busptr->get_bus_name();

        toolkit->show_information_with_leading_time_stamp(get_formatted_information1(current_from_bus, line.get_identifier(), I1, I2, I0));
        toolkit->show_information_with_leading_time_stamp(get_formatted_information2(busname, busptr->get_base_voltage_in_kV(), I1, I2, I0));
    }

    vector<TRANSFORMER*> transformers = psdb.get_transformers_connecting_to_bus(fault_bus);
    n = transformers.size();
    for(unsigned int i=0; i<n; i++)
    {
        TRANSFORMER& trans = *transformers[i];
        complex<double> I1, I2, I0;
        if(trans.is_two_winding_transformer())
        {
            unsigned int current_from_bus;
            if(trans.get_winding_bus(PRIMARY_SIDE) == fault_bus)
            {
                current_from_bus = trans.get_winding_bus(SECONDARY_SIDE);
                if(units==PU)
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_pu(PRIMARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_pu(PRIMARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_pu(PRIMARY_SIDE);
                }
                else
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_kA(PRIMARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_kA(PRIMARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_kA(PRIMARY_SIDE);
                }

            }
            else if(trans.get_winding_bus(SECONDARY_SIDE) == fault_bus)
            {
                current_from_bus = trans.get_winding_bus(PRIMARY_SIDE);
                if(units==PU)
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_pu(SECONDARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_pu(SECONDARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_pu(SECONDARY_SIDE);
                }
                else
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                }
            }

            BUS* busptr = psdb.get_bus(current_from_bus);
            string busname = busptr->get_bus_name();

            toolkit->show_information_with_leading_time_stamp(get_formatted_information1(current_from_bus,trans.get_identifier(), I1, I2, I0));
            toolkit->show_information_with_leading_time_stamp(get_formatted_information2(busname, busptr->get_base_voltage_in_kV(), I1, I2, I0));
        }
        else
        {
            string msg;
            if(trans.get_winding_bus(PRIMARY_SIDE) == fault_bus)
            {
                msg = "Winding 1";
                if(units==PU)
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_pu(PRIMARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_pu(PRIMARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_pu(PRIMARY_SIDE);
                }
                else
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_kA(PRIMARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_kA(PRIMARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_kA(PRIMARY_SIDE);
                }
            }
            else if(trans.get_winding_bus(SECONDARY_SIDE) == fault_bus)
            {
                msg = "Winding 2";
                if(units==PU)
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_pu(SECONDARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_pu(SECONDARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_pu(SECONDARY_SIDE);
                }
                else
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                }
            }
            else if(trans.get_winding_bus(TERTIARY_SIDE) == fault_bus)
            {
                msg = "Winding 3";
                if(units==PU)
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_pu(TERTIARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_pu(TERTIARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_pu(TERTIARY_SIDE);
                }
                else
                {
                    I1 = trans.get_winding_positive_sequence_complex_current_in_kA(TERTIARY_SIDE);
                    I2 = trans.get_winding_negative_sequence_complex_current_in_kA(TERTIARY_SIDE);
                    I0 = trans.get_winding_zero_sequence_complex_current_in_kA(TERTIARY_SIDE);
                }
            }
            ostringstream osstream;
            osstream<< left << setw(12) << msg <<"CKT "<< setw(4)<< trans.get_identifier();
            toolkit->show_information_with_leading_time_stamp(get_formatted_information1(osstream.str(), I1, I2, I0));
            toolkit->show_information_with_leading_time_stamp(get_formatted_information2("+", I1, I2, I0));
        }
    }

    vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(fault_bus);
    n = gens.size();
    for(unsigned int i=0; i<n; i++)
    {
        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = gens[i]->get_positive_sequence_complex_current_in_pu();
            I2 = gens[i]->get_negative_sequence_complex_current_in_pu();
            I0 = gens[i]->get_zero_sequence_complex_current_in_pu();
        }
        else
        {
            I1 = gens[i]->get_positive_sequence_complex_current_in_kA();
            I2 = gens[i]->get_negative_sequence_complex_current_in_kA();
            I0 = gens[i]->get_zero_sequence_complex_current_in_kA();
        }
        ostringstream osstream;
        osstream<< left <<setw(12) << "Generator" << " ID " << setw(4) << gens[i]->get_identifier();
        toolkit->show_information_with_leading_time_stamp(get_formatted_information1(osstream.str(), I1, I2, I0));
        toolkit->show_information_with_leading_time_stamp(get_formatted_information2("+", I1, I2, I0));
    }

    vector<WT_GENERATOR*> wt_gens = psdb.get_wt_generators_connecting_to_bus(fault_bus);
    n = wt_gens.size();
    for(unsigned int i=0; i<n; i++)
    {
        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = wt_gens[i]->get_positive_sequence_complex_current_in_pu();
            I2 = wt_gens[i]->get_negative_sequence_complex_current_in_pu();
            I0 = wt_gens[i]->get_zero_sequence_complex_current_in_pu();
        }
        else
        {
            I1 = wt_gens[i]->get_positive_sequence_complex_current_in_kA();
            I2 = wt_gens[i]->get_negative_sequence_complex_current_in_kA();
            I0 = wt_gens[i]->get_zero_sequence_complex_current_in_kA();
        }
        ostringstream osstream;
        osstream<< left <<setw(12) << "WT generator" << " ID " << setw(4) << wt_gens[i]->get_identifier();
        toolkit->show_information_with_leading_time_stamp(get_formatted_information1(osstream.str(), I1, I2, I0));
        toolkit->show_information_with_leading_time_stamp(get_formatted_information2("+", I1, I2, I0));
    }

    vector<LOAD*> loads = psdb.get_loads_connecting_to_bus(fault_bus);
    n = loads.size();
    for(unsigned int i=0; i<n; i++)
    {
        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = loads[i]->get_positive_sequence_complex_current_in_pu();
            I2 = loads[i]->get_negative_sequence_complex_current_in_pu();
            I0 = loads[i]->get_zero_sequence_complex_current_in_pu();
        }
        else
        {
            I1 = loads[i]->get_positive_sequence_complex_current_in_kA();
            I2 = loads[i]->get_negative_sequence_complex_current_in_kA();
            I0 = loads[i]->get_zero_sequence_complex_current_in_kA();
        }
        ostringstream osstream;
        osstream<< left <<setw(12) << "Load" << " ID " << setw(4) << loads[i]->get_identifier();
        toolkit->show_information_with_leading_time_stamp(get_formatted_information1(osstream.str(), I1, I2, I0));
        toolkit->show_information_with_leading_time_stamp(get_formatted_information2("+", I1, I2, I0));
    }

    vector<FIXED_SHUNT*> shunts = psdb.get_fixed_shunts_connecting_to_bus(fault_bus);
    n = shunts.size();
    for(unsigned int i=0; i<n; i++)
    {
        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = shunts[i]->get_positive_sequence_complex_current_in_pu();
            I2 = shunts[i]->get_negative_sequence_complex_current_in_pu();
            I0 = shunts[i]->get_zero_sequence_complex_current_in_pu();
        }
        else
        {
            I1 = shunts[i]->get_positive_sequence_complex_current_in_kA();
            I2 = shunts[i]->get_negative_sequence_complex_current_in_kA();
            I0 = shunts[i]->get_zero_sequence_complex_current_in_kA();
        }
        ostringstream osstream;
        osstream<< left <<setw(12) << "Shunt" << " ID " << setw(4) << shunts[i]->get_identifier();
        toolkit->show_information_with_leading_time_stamp(get_formatted_information1(osstream.str(), I1, I2, I0));
        toolkit->show_information_with_leading_time_stamp(get_formatted_information2("+", I1, I2, I0));
    }

    vector<PV_UNIT*> pv_units = psdb.get_pv_units_connecting_to_bus(fault_bus);
    n = pv_units.size();
    for(unsigned int i=0; i<n; i++)
    {
        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = pv_units[i]->get_positive_sequence_complex_current_in_pu();
            I2 = pv_units[i]->get_negative_sequence_complex_current_in_pu();
            I0 = pv_units[i]->get_zero_sequence_complex_current_in_pu();
        }
        else
        {
            I1 = pv_units[i]->get_positive_sequence_complex_current_in_kA();
            I2 = pv_units[i]->get_negative_sequence_complex_current_in_kA();
            I0 = pv_units[i]->get_zero_sequence_complex_current_in_kA();
        }
        ostringstream osstream;
        osstream<< left <<setw(12) << "PV_unit" << " ID " << setw(4) << pv_units[i]->get_identifier();
        toolkit->show_information_with_leading_time_stamp(get_formatted_information1(osstream.str(), I1, I2, I0));
        toolkit->show_information_with_leading_time_stamp(get_formatted_information2("+", I1, I2, I0));
    }
}

void SHORT_CIRCUIT_SOLVER::show_short_circuit_with_line_fault()
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    UNITS_OPTION unit = get_units_of_currents_and_voltages();
    unsigned int busnum = 99999;
    string busname = "DUMMY BUS";
    double Vbase = faulted_line_pointer->get_line_base_voltage_in_kV();

    osstream<<"Sequence equivalent impedance (P.U.):"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    osstream<<"BUS     NAME           <--------- ZERO ---------> <-------- POSITIVE -------> <-------- NEGATIVE ------->"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    osstream<< left << setw(8) << "99999" << setw(12) << "DUMMY BUS"
            << right << fixed << setprecision(5)
            << setw(14) << Z0.real() << setw(14) << Z0.imag()
            << setw(14) << Z1.real() << setw(14) << Z1.imag()
            << setw(14) << Z2.real() << setw(14) << Z2.imag()<< endl;
    toolkit->show_information_with_leading_time_stamp(osstream);

    complex<double> V1,V2,V0;
    if(unit == PU)
    {
        V1 = get_positive_sequence_voltage_at_line_fault_location_in_pu();
        V2 = get_negative_sequence_voltage_at_line_fault_location_in_pu();
        V0 = get_zero_sequence_voltage_at_line_fault_location_in_pu();
    }
    else if(unit == PHYSICAL)
    {
        V1 = get_positive_sequence_voltage_at_line_fault_location_in_kV();
        V2 = get_negative_sequence_voltage_at_line_fault_location_in_kV();
        V0 = get_zero_sequence_voltage_at_line_fault_location_in_kV();
    }
    show_voltage_table_header();
    toolkit->show_information_with_leading_time_stamp(get_formatted_information1(busnum, "", V1, V2, V0));
    toolkit->show_information_with_leading_time_stamp(get_formatted_information2(busname, Vbase , V1, V2, V0));

    show_current_table_header();
    show_contributions_of_fault_current_with_line_fault();

    complex<double> I1, I2, I0;
    if(unit == PU)
    {
        I1 = get_positive_sequence_fault_current_in_pu();
        I2 = get_positive_sequence_fault_current_in_pu();
        I0 = get_zero_sequence_fault_current_in_pu();
    }
    else if(unit == PHYSICAL)
    {
        I1 = get_positive_sequence_fault_current_in_kA();
        I2 = get_positive_sequence_fault_current_in_kA();
        I0 = get_zero_sequence_fault_current_in_kA();
    }
    osstream<< "Fault current:"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
    toolkit->show_information_with_leading_time_stamp(get_formatted_information1(busnum, "", I1, I2, I0));
    toolkit->show_information_with_leading_time_stamp(get_formatted_information2(busname, Vbase, I1, I2, I0));

    osstream<< "Short circuit capacity:  (P.U.) (MVA)" <<endl
            << right << fixed
            << "S1: " << setprecision(4) << V1*I1 << setprecision(2) << V1*I1*sbase <<endl
            << "S2: " << setprecision(4) << V2*I2 << setprecision(2) << V2*I2*sbase <<endl
            << "S0: " << setprecision(4) << V0*I0 << setprecision(2) << V0*I0*sbase <<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);

    osstream<< "Positive sequence equivalent fault admittance: "
            << fixed << setprecision(4) << get_positive_sequence_equivalent_fault_admittance()<<"PU"<<endl;
    toolkit->show_information_with_leading_time_stamp(osstream);
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_voltage_at_line_fault_location_in_pu()
{
    complex<double> Uf = get_voltage_of_faulted_line_point_before_short_circuit();
    complex<double> Z1 = get_positive_sequence_thevenin_impedance_at_fault_in_pu();
    complex<double> U1 = Uf - Z1 * If1;

    return U1;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_voltage_at_line_fault_location_in_kV()
{
    return get_positive_sequence_voltage_at_line_fault_location_in_pu()*faulted_line_pointer->get_line_base_voltage_in_kV()/SQRT3;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_voltage_at_line_fault_location_in_pu()
{
    complex<double> Z2 = get_negative_sequence_thevenin_impedance_at_fault_in_pu();
    complex<double> U2 = - Z2 * If2;

    return U2;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_voltage_at_line_fault_location_in_kV()
{
    return get_negative_sequence_voltage_at_line_fault_location_in_pu()*faulted_line_pointer->get_line_base_voltage_in_kV()/SQRT3;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_voltage_at_line_fault_location_in_pu()
{
    complex<double> Z0 = get_zero_sequence_thevenin_impedance_at_fault_in_pu();
    complex<double> U0 =  - Z0 * If0;

    return U0;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_voltage_at_line_fault_location_in_kV()
{
    return get_zero_sequence_voltage_at_line_fault_location_in_pu()*faulted_line_pointer->get_line_base_voltage_in_kV()/SQRT3;
}

complex<double> SHORT_CIRCUIT_SOLVER::get_positive_sequence_thevenin_impedance_at_fault_in_pu()
{
    return Z1;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_negative_sequence_thevenin_impedance_at_fault_in_pu()
{
    return Z2;
}
complex<double> SHORT_CIRCUIT_SOLVER::get_zero_sequence_thevenin_impedance_at_fault_in_pu()
{
    return Z0;
}

void SHORT_CIRCUIT_SOLVER::save_short_circuit_result_to_file(const string& filename)
{
    if(is_bus_fault())
        save_short_circuit_result_to_file_with_bus_fault(filename);
    else if(is_line_fault())
        save_short_circuit_result_to_file_with_line_fault(filename);
}

void SHORT_CIRCUIT_SOLVER::save_short_circuit_result_to_file_with_bus_fault(const string& filename)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    UNITS_OPTION units = get_units_of_currents_and_voltages();
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();

    ofstream file(filename);
    if(file.is_open())
    {
        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);
        char time_stamp[40];
        char buffer[1000];
        snprintf(time_stamp,40, "%d-%02d-%02d %02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        file<<"% Short circuit result exported at "<<time_stamp<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_information()).c_str());
        file<<"% "<<buffer<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_additional_information()).c_str());
        file<<"% "<<buffer<<endl;

        string I_unit = (units==PU)?"(P.U.)":"(kA)";
        string V_unit = (units==PU)?"(P.U.)":"(kV)";
        unsigned int fault_bus = faulted_bus_pointer->get_bus_number();
        string busname = faulted_bus_pointer->get_bus_name();
        double Vbase = faulted_bus_pointer->get_base_voltage_in_kV();

        file<<get_fault_information()<<endl;

        file<<"Sequence thevenin impedance:"<<endl;
        snprintf(buffer, 1000, "RE(Z0),IM(Z0),RE(Z1),IM(Z1),RE(Z2),IM(Z2)");
        file<<buffer<<endl;
        snprintf(buffer, 1000, "%.5f,%.5f,%.5f,%.5f,%.5f,%.5f",
                    Z0.real(),Z0.imag(),Z1.real(),Z1.imag(),Z2.real(),Z2.imag());
        file<<buffer<<endl;

        if(coordinate==RECTANGULAR)
        {
            file<<"SEQUENCE,"<<V_unit<<right<<",RE(V0)"<<",IM(V0),RE(V+),IM(V+),RE(V-),IM(V-),RE(3V0),IM(3V0)"<<endl;
            file<<"PHASE,\t,RE(VA),IM(VA),RE(VB),IM(VB),RE(VC),IM(VC)"<<endl;
        }
        else if(coordinate==POLAR)
        {
            file<<"SEQUENCE,"<<V_unit<<",/V0/,AN(V0),/V+/,AN(V+),/V-/,AN(V-),/3V0/,AN(3V0)"<<endl;
            file<<"PHASE,\t,RE(VA),IM(VA),RE(VB),IM(VB),RE(VC),IM(VC)"<<endl;
        }

        complex<double> V1, V2, V0;
        if(units == PU)
        {
            V1 = faulted_bus_pointer->get_positive_sequence_complex_voltage_in_pu();
            V2 = faulted_bus_pointer->get_negative_sequence_complex_voltage_in_pu();
            V0 = faulted_bus_pointer->get_zero_sequence_complex_voltage_in_pu();
        }
        else if(units == PHYSICAL)
        {
            V1 = faulted_bus_pointer->get_positive_sequence_complex_voltage_in_kV()/SQRT3;
            V2 = faulted_bus_pointer->get_negative_sequence_complex_voltage_in_kV()/SQRT3;
            V0 = faulted_bus_pointer->get_zero_sequence_complex_voltage_in_kV()/SQRT3;
        }
        file << get_formatted_information1(fault_bus, "", V1, V2, V0, true);
        file << get_formatted_information2(busname, Vbase , V1, V2, V0, true);

        if(coordinate==RECTANGULAR)
        {
            file<<"SEQUENCE,"<<I_unit<<",RE(I0)"<<",IM(I0),RE(I+),IM(I+),RE(I-),IM(I-),RE(3I0),IM(3I0)"<<endl;
            file<<"PHASE,\t,RE(IA),IM(IA),RE(IB),IM(IB),RE(IC),IM(IC)"<<endl;
        }
        else if(coordinate==POLAR)
        {
            file<<"SEQUENCE,"<<I_unit<<",/I0/,AN(I0),/I+/,AN(I+),/I-/,AN(I-),/3I0/,AN(3I0)"<<endl;
            file<<"PHASE,\t,RE(IA),IM(IA),RE(IB),IM(IB),RE(IC),IM(IC)"<<endl;
        }

        vector<LINE*> lines = psdb.get_lines_connecting_to_bus(fault_bus);
        unsigned int n = lines.size();
        for(unsigned int i=0; i<n; i++)
        {
            LINE& line = *lines[i];
            complex<double> I1, I2, I0;
            unsigned int current_from_bus ;
            if(line.get_sending_side_bus() == fault_bus)
            {
                current_from_bus = line.get_receiving_side_bus();
                if(units==PU)
                {
                    I1 = line.get_line_positive_sequence_complex_current_at_sending_side_in_pu();
                    I2 = line.get_line_negative_sequence_complex_current_at_sending_side_in_pu();
                    I0 = line.get_line_zero_sequence_complex_current_at_sending_side_in_pu();
                }
                else
                {
                    I1 = line.get_line_positive_sequence_complex_current_at_sending_side_in_kA();
                    I2 = line.get_line_negative_sequence_complex_current_at_sending_side_in_kA();
                    I0 = line.get_line_zero_sequence_complex_current_at_sending_side_in_kA();
                }
            }
            else if(line.get_receiving_side_bus() == fault_bus)
            {
                current_from_bus = line.get_sending_side_bus();
                if(units==PU)
                {
                    I1 = line.get_line_positive_sequence_complex_current_at_receiving_side_in_pu();
                    I2 = line.get_line_negative_sequence_complex_current_at_receiving_side_in_pu();
                    I0 = line.get_line_zero_sequence_complex_current_at_receiving_side_in_pu();
                }
                else
                {
                    I1 = line.get_line_positive_sequence_complex_current_at_receiving_side_in_kA();
                    I2 = line.get_line_negative_sequence_complex_current_at_receiving_side_in_kA();
                    I0 = line.get_line_zero_sequence_complex_current_at_receiving_side_in_kA();
                }
            }
            BUS* busptr = psdb.get_bus(current_from_bus);
            string busname = busptr->get_bus_name();

            file<< get_formatted_information1(current_from_bus,line.get_identifier(), I1, I2, I0, true);
            file<< get_formatted_information2(busname, busptr->get_base_voltage_in_kV(), I1, I2, I0, true);
        }

        vector<TRANSFORMER*> transformers = psdb.get_transformers_connecting_to_bus(fault_bus);
        n = transformers.size();
        for(unsigned int i=0; i<n; i++)
        {
            TRANSFORMER& trans = *(transformers[i]);
            complex<double> I1, I2, I0;
            if(trans.is_two_winding_transformer())
            {
                unsigned int current_from_bus;
                if(trans.get_winding_bus(PRIMARY_SIDE) == fault_bus)
                {
                    current_from_bus = trans.get_winding_bus(SECONDARY_SIDE);
                    if(units==PU)
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_pu(PRIMARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_pu(PRIMARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_pu(PRIMARY_SIDE);
                    }
                    else
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_kA(PRIMARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_kA(PRIMARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_kA(PRIMARY_SIDE);
                    }
                }
                else if(trans.get_winding_bus(SECONDARY_SIDE) == fault_bus)
                {
                    current_from_bus = trans.get_winding_bus(PRIMARY_SIDE);
                    if(units==PU)
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    }
                    else
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    }
                }

                BUS* busptr = psdb.get_bus(current_from_bus);

                file<< get_formatted_information1(current_from_bus,trans.get_identifier(), I1, I2, I0, true);
                file<< get_formatted_information2(busptr->get_bus_name(), busptr->get_base_voltage_in_kV(), I1, I2, I0, true);
            }
            else
            {
                string msg;
                if(trans.get_winding_bus(PRIMARY_SIDE) == fault_bus)
                {
                    msg = "Winding 1";
                    if(units==PU)
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_pu(PRIMARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_pu(PRIMARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_pu(PRIMARY_SIDE);
                    }
                    else
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_kA(PRIMARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_kA(PRIMARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_kA(PRIMARY_SIDE);
                    }
                }
                else if(trans.get_winding_bus(SECONDARY_SIDE) == fault_bus)
                {
                    msg = "Winding 2";
                    if(units==PU)
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_pu(SECONDARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_pu(SECONDARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_pu(SECONDARY_SIDE);
                    }
                    else
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                    }
                }
                else if(trans.get_winding_bus(TERTIARY_SIDE) == fault_bus)
                {
                    msg = "Winding 3";
                    if(units==PU)
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_pu(TERTIARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_pu(TERTIARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_pu(TERTIARY_SIDE);
                    }
                    else
                    {
                        I1 = trans.get_winding_positive_sequence_complex_current_in_kA(TERTIARY_SIDE);
                        I2 = trans.get_winding_negative_sequence_complex_current_in_kA(TERTIARY_SIDE);
                        I0 = trans.get_winding_zero_sequence_complex_current_in_kA(TERTIARY_SIDE);
                    }
                }
                ostringstream osstream;
                osstream<< msg <<",";
                file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
                file<< get_formatted_information2(",", I1, I2, I0, true);
            }
        }

        vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(fault_bus);
        n = gens.size();
        for(unsigned int i=0; i<n; i++)
        {
            complex<double> I1, I2, I0;
            if(units==PU)
            {
                I1 = gens[i]->get_positive_sequence_complex_current_in_pu();
                I2 = gens[i]->get_negative_sequence_complex_current_in_pu();
                I0 = gens[i]->get_zero_sequence_complex_current_in_pu();
            }
            else
            {
                I1 = gens[i]->get_positive_sequence_complex_current_in_kA();
                I2 = gens[i]->get_negative_sequence_complex_current_in_kA();
                I0 = gens[i]->get_zero_sequence_complex_current_in_kA();
            }
            ostringstream osstream;
            osstream<< "Generator,ID "<< gens[i]->get_identifier();
            file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
            file<< get_formatted_information2(",", I1, I2, I0, true);
        }

        vector<WT_GENERATOR*> wt_gens = psdb.get_wt_generators_connecting_to_bus(fault_bus);
        n = wt_gens.size();
        for(unsigned int i=0; i<n; i++)
        {
            complex<double> I1, I2, I0;
            if(units==PU)
            {
                I1 = wt_gens[i]->get_positive_sequence_complex_current_in_pu();
                I2 = wt_gens[i]->get_negative_sequence_complex_current_in_pu();
                I0 = wt_gens[i]->get_zero_sequence_complex_current_in_pu();
            }
            else
            {
                I1 = wt_gens[i]->get_positive_sequence_complex_current_in_kA();
                I2 = wt_gens[i]->get_negative_sequence_complex_current_in_kA();
                I0 = wt_gens[i]->get_zero_sequence_complex_current_in_kA();
            }
            ostringstream osstream;
            osstream<< "WT generator,ID "<< wt_gens[i]->get_identifier();
            file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
            file<< get_formatted_information2(",", I1, I2, I0, true);
        }

        vector<LOAD*> loads = psdb.get_loads_connecting_to_bus(fault_bus);
        n = loads.size();
        for(unsigned int i=0; i<n; i++)
        {
            complex<double> I1, I2, I0;
            if(units==PU)
            {
                I1 = loads[i]->get_positive_sequence_complex_current_in_pu();
                I2 = loads[i]->get_negative_sequence_complex_current_in_pu();
                I0 = loads[i]->get_zero_sequence_complex_current_in_pu();
            }
            else
            {
                I1 = loads[i]->get_positive_sequence_complex_current_in_kA();
                I2 = loads[i]->get_negative_sequence_complex_current_in_kA();
                I0 = loads[i]->get_zero_sequence_complex_current_in_kA();
            }
            ostringstream osstream;
            osstream<< "Load,ID "<< loads[i]->get_identifier();
            file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
            file<< get_formatted_information2(",", I1, I2, I0, true);
        }

        vector<FIXED_SHUNT*> shunts = psdb.get_fixed_shunts_connecting_to_bus(fault_bus);
        n = shunts.size();
        for(unsigned int i=0; i<n; i++)
        {
            complex<double> I1, I2, I0;
            if(units==PU)
            {
                I1 = shunts[i]->get_positive_sequence_complex_current_in_pu();
                I2 = shunts[i]->get_negative_sequence_complex_current_in_pu();
                I0 = shunts[i]->get_zero_sequence_complex_current_in_pu();
            }
            else
            {
                I1 = shunts[i]->get_positive_sequence_complex_current_in_kA();
                I2 = shunts[i]->get_negative_sequence_complex_current_in_kA();
                I0 = shunts[i]->get_zero_sequence_complex_current_in_kA();
            }
            ostringstream osstream;
            osstream<< "Shunt,ID "<< shunts[i]->get_identifier();
            file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
            file<< get_formatted_information2(",", I1, I2, I0, true);
        }

        vector<PV_UNIT*> pv_units = psdb.get_pv_units_connecting_to_bus(fault_bus);
        n = pv_units.size();
        for(unsigned int i=0; i<n; i++)
        {
            complex<double> I1, I2, I0;
            if(units==PU)
            {
                I1 = pv_units[i]->get_positive_sequence_complex_current_in_pu();
                I2 = pv_units[i]->get_negative_sequence_complex_current_in_pu();
                I0 = pv_units[i]->get_zero_sequence_complex_current_in_pu();
            }
            else
            {
                I1 = pv_units[i]->get_positive_sequence_complex_current_in_kA();
                I2 = pv_units[i]->get_negative_sequence_complex_current_in_kA();
                I0 = pv_units[i]->get_zero_sequence_complex_current_in_kA();
            }
            ostringstream osstream;
            osstream<< "PV_unit,ID "<< pv_units[i]->get_identifier();
            file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
            file<< get_formatted_information2(",", I1, I2, I0, true);
        }

        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = get_positive_sequence_fault_current_in_pu();
            I2 = get_negative_sequence_fault_current_in_pu();
            I0 = get_zero_sequence_fault_current_in_pu();
        }
        else if(units==PHYSICAL)
        {
            I1 = get_positive_sequence_fault_current_in_kA();
            I2 = get_negative_sequence_fault_current_in_kA();
            I0 = get_zero_sequence_fault_current_in_kA();
        }
        file<<"%Total fault current"<<endl;
        file<< get_formatted_information1(fault_bus, "", I1, I2, I0, true);
        file<< get_formatted_information2(busname, Vbase, I1, I2, I0, true);

        complex<double> S = get_positive_sequence_short_circuit_capacity_in_pu();
        file<< "%Positive sequence equivalent fault admittance:,"
            << fixed << setprecision(4) << S.real() << "," << S.imag() <<",PU"<<endl;

        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving short circuit result to file."<<endl
                <<"No short circuit result will be exported.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}
void SHORT_CIRCUIT_SOLVER::save_short_circuit_result_to_file_with_line_fault(const string& filename)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
    double sbase = psdb.get_system_base_power_in_MVA();
    UNITS_OPTION units = get_units_of_currents_and_voltages();
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();

    ofstream file(filename);
    if(file.is_open())
    {
        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);
        char time_stamp[40];
        char buffer[1000];
        snprintf(time_stamp,40, "%d-%02d-%02d %02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        file<<"% Short circuit result exported at "<<time_stamp<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_information()).c_str());
        file<<"% "<<buffer<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_additional_information()).c_str());
        file<<"% "<<buffer<<endl;

        string I_unit = (units==PU)?"(P.U.)":"(kA)";
        string V_unit = (units==PU)?"(P.U.)":"(kV)";
        unsigned int busnum = 99999;
        string busname = "DUMMY BUS";
        string id = faulted_line_pointer->get_identifier();
        double Vbase = faulted_line_pointer->get_line_base_voltage_in_kV();
        double Ibase = sbase/(SQRT3*Vbase);

        file<<get_fault_information()<<endl;

        file<<"Sequence thevenin impedance:"<<endl;
        snprintf(buffer, 1000, "RE(Z0),IM(Z0),RE(Z1),IM(Z1),RE(Z2),IM(Z2)");
        file<<buffer<<endl;
        snprintf(buffer, 1000, "%.5f,%.5f,%.5f,%.5f,%.5f,%.5f",
                    Z0.real(),Z0.imag(),Z1.real(),Z1.imag(),Z2.real(),Z2.imag());
        file<<buffer<<endl;

        if(coordinate==RECTANGULAR)
        {
            file<<"SEQUENCE,"<<V_unit<<right<<",RE(V0)"<<",IM(V0),RE(V+),IM(V+),RE(V-),IM(V-),RE(3V0),IM(3V0)"<<endl;
            file<<"PHASE,\t,RE(VA),IM(VA),RE(VB),IM(VB),RE(VC),IM(VC)"<<endl;
        }
        else if(coordinate==POLAR)
        {
            file<<"SEQUENCE,"<<V_unit<<",/V0/,AN(V0),/V+/,AN(V+),/V-/,AN(V-),/3V0/,AN(3V0)"<<endl;
            file<<"PHASE,\t,RE(VA),IM(VA),RE(VB),IM(VB),RE(VC),IM(VC)"<<endl;
        }

        complex<double> V1, V2, V0;
        if(units == PU)
        {
            V1 = get_positive_sequence_voltage_at_line_fault_location_in_pu();
            V2 = get_negative_sequence_voltage_at_line_fault_location_in_pu();
            V0 = get_zero_sequence_voltage_at_line_fault_location_in_pu();
        }
        else if(units == PHYSICAL)
        {
            V1 = get_positive_sequence_voltage_at_line_fault_location_in_kV();
            V2 = get_negative_sequence_voltage_at_line_fault_location_in_kV();
            V0 = get_zero_sequence_voltage_at_line_fault_location_in_kV();
        }

        file << get_formatted_information1(busnum, "", V1, V2, V0, true);
        file << get_formatted_information2(busname, Vbase , V1, V2, V0, true);

        if(coordinate==RECTANGULAR)
        {
            file<<"SEQUENCE,"<<I_unit<<",RE(I0)"<<",IM(I0),RE(I+),IM(I+),RE(I-),IM(I-),RE(3I0),IM(3I0)"<<endl;
            file<<"PHASE,\t,RE(IA),IM(IA),RE(IB),IM(IB),RE(IC),IM(IC)"<<endl;
        }
        else if(coordinate==POLAR)
        {
            file<<"SEQUENCE,"<<I_unit<<",/I0/,AN(I0),/I+/,AN(I+),/I-/,AN(I-),/3I0/,AN(3I0)"<<endl;
            file<<"PHASE,\t,RE(IA),IM(IA),RE(IB),IM(IB),RE(IC),IM(IC)"<<endl;
        }


        unsigned int ibus = faulted_line_pointer->get_sending_side_bus();
        unsigned int jbus = faulted_line_pointer->get_receiving_side_bus();
        BUS* ibusptr = faulted_line_pointer->get_sending_side_bus_pointer();
        BUS* jbusptr = faulted_line_pointer->get_receiving_side_bus_pointer();

        double fault_location = faulted_line_pointer->get_fault_location_of_fault(0);
        complex<double> Z1ij = faulted_line_pointer->get_line_positive_sequence_z_in_pu();
        complex<double> Z1if = Z1ij*fault_location;
        complex<double> Z1jf = Z1ij*(1.0-fault_location);

        complex<double> Z0ij = faulted_line_pointer->get_line_zero_sequence_z_in_pu();
        complex<double> Z0if = Z0ij*fault_location;
        complex<double> Z0jf = Z0ij*(1.0-fault_location);

        complex<double> U1f = get_positive_sequence_voltage_of_fault_point_in_pu();
        complex<double> U2f = get_negative_sequence_voltage_of_fault_point_in_pu();
        complex<double> U0f = get_zero_sequence_voltage_of_fault_point_in_pu();

        complex<double> U1i = ibusptr->get_positive_sequence_complex_voltage_in_pu();
        complex<double> U2i = ibusptr->get_negative_sequence_complex_voltage_in_pu();
        complex<double> U0i = ibusptr->get_zero_sequence_complex_voltage_in_pu();
        complex<double> U1j = jbusptr->get_positive_sequence_complex_voltage_in_pu();
        complex<double> U2j = jbusptr->get_negative_sequence_complex_voltage_in_pu();
        complex<double> U0j = jbusptr->get_zero_sequence_complex_voltage_in_pu();

        complex<double> I1if = (U1i - U1f)/Z1if;
        complex<double> I2if = (U2i - U2f)/Z1if;
        complex<double> I0if = (U0i - U0f)/Z0if;
        complex<double> I1jf = (U1j - U1f)/Z1jf;
        complex<double> I2jf = (U2j - U2f)/Z1jf;
        complex<double> I0jf = (U0j - U0f)/Z0jf;

        if(units == PHYSICAL)
        {
            I1if = I1if*Ibase;
            I2if = I2if*Ibase;
            I0if = I0if*Ibase;
            I1jf = I1jf*Ibase;
            I2jf = I2jf*Ibase;
            I0jf = I0jf*Ibase;
        }

        file<< get_formatted_information1(ibus, id, I1if, I2if, I0if, true);
        file<< get_formatted_information2(ibusptr->get_bus_name(),ibusptr->get_base_voltage_in_kV(), I1if, I2if, I0if, true);

        file<< get_formatted_information1(jbus, id, I1jf, I2jf, I0jf, true);
        file<< get_formatted_information2(jbusptr->get_bus_name(),jbusptr->get_base_voltage_in_kV(), I1jf, I2jf, I0jf, true);

        complex<double> I1, I2, I0;
        if(units==PU)
        {
            I1 = get_positive_sequence_fault_current_in_pu();
            I2 = get_negative_sequence_fault_current_in_pu();
            I0 = get_zero_sequence_fault_current_in_pu();
        }
        else if(units==PHYSICAL)
        {
            I1 = get_positive_sequence_fault_current_in_kA();
            I2 = get_negative_sequence_fault_current_in_kA();
            I0 = get_zero_sequence_fault_current_in_kA();
        }
        file<<"%Total fault current"<<endl;
        file<< get_formatted_information1(busnum, "", I1, I2, I0, true);
        file<< get_formatted_information2(busname, Vbase, I1, I2, I0, true);

        file<< "%Positive sequence equivalent fault admittance: "
            << fixed << setprecision(4) << get_positive_sequence_equivalent_fault_admittance()<<"PU"<<endl;

        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving short circuit result to file."<<endl
                <<"No short circuit result will be exported.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}


void SHORT_CIRCUIT_SOLVER::save_extended_short_circuit_result_to_file(const string& filename)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    if((!is_bus_fault()) && (!is_line_fault()))
        return;

    UNITS_OPTION units = get_units_of_currents_and_voltages();
    string I_unit = (units==PU)?"(P.U.)":"(kA)";
    string V_unit = (units==PU)?"(P.U.)":"(kV)";
    COORDINATES_OPTION coordinate = get_coordinates_of_currents_and_voltages();

    ofstream file(filename);
    if(file.is_open())
    {
        time_t tt = time(NULL);
        tm* local_time= localtime(&tt);

        char time_stamp[40];
        char buffer[1000];
        snprintf(time_stamp,40, "%d-%02d-%02d %02d:%02d:%02d", local_time->tm_year + 1900, local_time->tm_mon + 1,
                local_time->tm_mday, local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

        file<<"% Extended short circuit result exported at "<<time_stamp<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_information()).c_str());
        file<<"% "<<buffer<<endl;
        snprintf(buffer, 1000, "%s", (psdb.get_case_additional_information()).c_str());
        file<<"% "<<buffer<<endl;

        file<<get_fault_information()<<endl;

        vector<BUS*> buses = psdb.get_all_buses();
        unsigned int n = buses.size();
        for(unsigned int i=0; i<n; i++)
        {
            if(coordinate==RECTANGULAR)
            {
                file<<"SEQUENCE,"<<V_unit<<",RE(V0)"<<",IM(V0),RE(V+),IM(V+),RE(V-),IM(V-),RE(3V0),IM(3V0)"<<endl;
                file<<"PHASE,\t,RE(VA),IM(VA),RE(VB),IM(VB),RE(VC),IM(VC)"<<endl;
            }
            else if(coordinate==POLAR)
            {
                file<<"SEQUENCE,"<<V_unit<<",/V0/,AN(V0),/V+/,AN(V+),/V-/,AN(V-),/3V0/,AN(3V0)"<<endl;
                file<<"PHASE,\t,RE(VA),IM(VA),RE(VB),IM(VB),RE(VC),IM(VC)"<<endl;
            }

            BUS& bus = *(buses[i]);
            complex<double> V1, V2, V0;
            if(units==PU)
            {
                V1 = bus.get_positive_sequence_complex_voltage_in_pu();
                V2 = bus.get_negative_sequence_complex_voltage_in_pu();
                V0 = bus.get_zero_sequence_complex_voltage_in_pu();
            }
            else if(units==PHYSICAL)
            {
                V1 = bus.get_positive_sequence_complex_voltage_in_kV()/SQRT3;
                V2 = bus.get_negative_sequence_complex_voltage_in_kV()/SQRT3;
                V0 = bus.get_zero_sequence_complex_voltage_in_kV()/SQRT3;
            }

            file<< get_formatted_information1(bus.get_bus_number(),"", V1, V2, V0, true);
            file<< get_formatted_information2(bus.get_bus_name(), bus.get_base_voltage_in_kV(), V1, V2, V0, true);

            if(coordinate==RECTANGULAR)
            {
                file<<"SEQUENCE,"<<I_unit<<right<<",RE(I0)"<<",IM(I0),RE(I+),IM(I+),RE(I-),IM(I-),RE(3I0),IM(3I0)"<<endl;
                file<<"PHASE,\t,RE(IA),IM(IA),RE(IB),IM(IB),RE(IC),IM(IC)"<<endl;
            }
            else if(coordinate==POLAR)
            {
                file<<"SEQUENCE,"<<I_unit<<",/I0/,AN(I0),/I+/,AN(I+),/V-/,AN(I-),/3I0/,AN(3I0)"<<endl;
                file<<"PHASE,\t,RE(IA),IM(IA),RE(IB),IM(IB),RE(IC),IM(IC)"<<endl;
            }
            unsigned int busnum = bus.get_bus_number();
            vector<LINE*> lines = psdb.get_lines_connecting_to_bus(busnum);
            unsigned int n = lines.size();
            for(unsigned int i=0; i<n; i++)
            {
                LINE& line = *lines[i];

                complex<double> I1, I2, I0;
                unsigned int to_bus ;

                if(line.is_faulted()==true)
                {
                    to_bus = 999999;
                    double fault_location = line.get_fault_location_of_fault(0);
                    complex<double> Z1 = line.get_line_positive_sequence_z_in_pu();
                    complex<double> Y1 = line.get_line_positive_sequence_y_in_pu();

                    complex<double> Z0 = line.get_line_zero_sequence_z_in_pu();
                    complex<double> Y0 = line.get_line_zero_sequence_y_in_pu();

                    complex<double> U1f = get_positive_sequence_voltage_of_fault_point_in_pu();
                    complex<double> U2f = get_negative_sequence_voltage_of_fault_point_in_pu();
                    complex<double> U0f = get_zero_sequence_voltage_of_fault_point_in_pu();

                    if(line.get_sending_side_bus()==busnum)
                    {
                        complex<double> Z1if = Z1*fault_location;
                        complex<double> Z0if = Z0*fault_location;
                        complex<double> Y1if = 0.5*Y1 + line.get_shunt_positive_sequence_y_at_sending_side_in_pu();
                        complex<double> Y0if = 0.5*Y0 + line.get_shunt_zero_sequence_y_at_sending_side_in_pu();

                        complex<double> U1i = psdb.get_bus_positive_sequence_complex_voltage_in_pu(line.get_sending_side_bus());
                        complex<double> U2i = psdb.get_bus_negative_sequence_complex_voltage_in_pu(line.get_sending_side_bus());
                        complex<double> U0i = psdb.get_bus_zero_sequence_complex_voltage_in_pu(line.get_sending_side_bus());

                        I1 = (U1i-U1f)/Z1if + U1i*Y1if;
                        I2 = (U2i-U2f)/Z1if + U2i*Y1if;
                        I0 = (U0i-U0f)/Z0if + U0i*Y0if;
                    }
                    else if(line.get_receiving_side_bus()==busnum)
                    {
                        complex<double> Z1jf = Z1*(1.0-fault_location);
                        complex<double> Z0jf = Z0*(1.0-fault_location);
                        complex<double> Y1jf = 0.5*Y1 + line.get_shunt_positive_sequence_y_at_receiving_side_in_pu();
                        complex<double> Y0jf = 0.5*Y0 + line.get_shunt_zero_sequence_y_at_receiving_side_in_pu();

                        complex<double> U1j = psdb.get_bus_positive_sequence_complex_voltage_in_pu(line.get_receiving_side_bus());
                        complex<double> U2j = psdb.get_bus_negative_sequence_complex_voltage_in_pu(line.get_receiving_side_bus());
                        complex<double> U0j = psdb.get_bus_zero_sequence_complex_voltage_in_pu(line.get_receiving_side_bus());

                        I1 = (U1j-U1f)/Z1jf + U1j*Y1jf;
                        I2 = (U2j-U2f)/Z1jf + U2j*Y1jf;
                        I0 = (U0j-U0f)/Z0jf + U0j*Y0jf;
                    }
                }
                else
                {
                    if(line.get_sending_side_bus() == busnum)
                    {
                        to_bus = line.get_receiving_side_bus();
                        if(units==PU)
                        {
                            I1 = line.get_line_positive_sequence_complex_current_at_sending_side_in_pu();
                            I2 = line.get_line_negative_sequence_complex_current_at_sending_side_in_pu();
                            I0 = line.get_line_zero_sequence_complex_current_at_sending_side_in_pu();
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = line.get_line_positive_sequence_complex_current_at_sending_side_in_kA();
                            I2 = line.get_line_negative_sequence_complex_current_at_sending_side_in_kA();
                            I0 = line.get_line_zero_sequence_complex_current_at_sending_side_in_kA();
                        }
                    }
                    else if(line.get_receiving_side_bus() == busnum)
                    {
                        to_bus = line.get_sending_side_bus();

                        if(units==PU)
                        {
                            I1 = line.get_line_positive_sequence_complex_current_at_receiving_side_in_pu();
                            I2 = line.get_line_negative_sequence_complex_current_at_receiving_side_in_pu();
                            I0 = line.get_line_zero_sequence_complex_current_at_receiving_side_in_pu();
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = line.get_line_positive_sequence_complex_current_at_receiving_side_in_kA();
                            I2 = line.get_line_negative_sequence_complex_current_at_receiving_side_in_kA();
                            I0 = line.get_line_zero_sequence_complex_current_at_receiving_side_in_kA();
                        }
                    }
                }

                string to_bus_name;
                if(to_bus == 999999)
                    to_bus_name = "DUMMY BUS";
                else
                {
                    BUS* busptr = psdb.get_bus(to_bus);
                    to_bus_name = busptr->get_bus_name();
                }
                file<< get_formatted_information1(to_bus, line.get_identifier(), I1, I2, I0, true);
                file<< get_formatted_information2(to_bus_name, line.get_line_base_voltage_in_kV(), I1, I2, I0, true);
            }

            vector<TRANSFORMER*> transformers = psdb.get_transformers_connecting_to_bus(busnum);
            n = transformers.size();
            for(unsigned int i=0; i<n; i++)
            {
                TRANSFORMER& trans = *transformers[i];
                complex<double> I1, I2, I0;
                if(trans.is_two_winding_transformer())
                {
                    unsigned int to_bus;
                    if(trans.get_winding_bus(PRIMARY_SIDE) == busnum)
                    {
                        to_bus = trans.get_winding_bus(SECONDARY_SIDE);
                        if(units==PU)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_pu(PRIMARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_pu(PRIMARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_pu(PRIMARY_SIDE);
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_kA(PRIMARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_kA(PRIMARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_kA(PRIMARY_SIDE);
                        }
                    }
                    else if(trans.get_winding_bus(SECONDARY_SIDE) == busnum)
                    {
                        to_bus = trans.get_winding_bus(PRIMARY_SIDE);
                        if(units==PU)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_pu(SECONDARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_pu(SECONDARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_pu(SECONDARY_SIDE);
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        }
                    }

                    BUS* busptr = psdb.get_bus(to_bus);

                    file<< get_formatted_information1(to_bus, trans.get_identifier(), I1, I2, I0, true);
                    file<< get_formatted_information2(busptr->get_bus_name(), busptr->get_base_voltage_in_kV(), I1, I2, I0, true);
                }
                else
                {
                    string msg;
                    if(trans.get_winding_bus(PRIMARY_SIDE) == busnum)
                    {
                        msg = "Winding 1";
                        if(units==PU)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_pu(PRIMARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_pu(PRIMARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_pu(PRIMARY_SIDE);
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_kA(PRIMARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_kA(PRIMARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_kA(PRIMARY_SIDE);
                        }
                    }
                    else if(trans.get_winding_bus(SECONDARY_SIDE) == busnum)
                    {
                        msg = "Winding 2";
                        if(units==PU)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_pu(SECONDARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_pu(SECONDARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_pu(SECONDARY_SIDE);
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_kA(SECONDARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_kA(SECONDARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_kA(SECONDARY_SIDE);
                        }
                    }
                    else if(trans.get_winding_bus(TERTIARY_SIDE) == busnum)
                    {
                        msg = "Winding 3";
                        if(units==PU)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_pu(TERTIARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_pu(TERTIARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_pu(TERTIARY_SIDE);
                        }
                        else if(units==PHYSICAL)
                        {
                            I1 = trans.get_winding_positive_sequence_complex_current_in_kA(TERTIARY_SIDE);
                            I2 = trans.get_winding_negative_sequence_complex_current_in_kA(TERTIARY_SIDE);
                            I0 = trans.get_winding_zero_sequence_complex_current_in_kA(TERTIARY_SIDE);
                        }
                    }

                    snprintf(buffer, 1000, "%s,CKT %s",msg.c_str(), trans.get_identifier().c_str());
                    file<< get_formatted_information1(buffer, I1, I2, I0, true);
                    file<< get_formatted_information2(",", I1, I2,I0, true);
                }
            }

            vector<GENERATOR*> gens = psdb.get_generators_connecting_to_bus(busnum);
            n = gens.size();
            for(unsigned int i=0; i<n; i++)
            {
                complex<double> I1, I2, I0;
                if(units==PU)
                {
                    I1 = gens[i]->get_positive_sequence_complex_current_in_pu();
                    I2 = gens[i]->get_negative_sequence_complex_current_in_pu();
                    I0 = gens[i]->get_zero_sequence_complex_current_in_pu();
                }
                else if(units==PHYSICAL)
                {
                    I1 = gens[i]->get_positive_sequence_complex_current_in_kA();
                    I2 = gens[i]->get_negative_sequence_complex_current_in_kA();
                    I0 = gens[i]->get_zero_sequence_complex_current_in_kA();
                }

                snprintf(buffer, 1000, "Generator,ID %s", gens[i]->get_identifier().c_str());
                file<< get_formatted_information1(buffer, I1,I2, I0, true);
                file<< get_formatted_information2(",", I1, I2, I0, true);

            }

            vector<WT_GENERATOR*> wt_gens = psdb.get_wt_generators_connecting_to_bus(busnum);
            n = wt_gens.size();
            for(unsigned int i=0; i<n; i++)
            {
                complex<double> I1, I2, I0;
                if(units==PU)
                {
                    I1 = wt_gens[i]->get_positive_sequence_complex_current_in_pu();
                    I2 = wt_gens[i]->get_negative_sequence_complex_current_in_pu();
                    I0 = wt_gens[i]->get_zero_sequence_complex_current_in_pu();
                }
                else if(units==PHYSICAL)
                {
                    I1 = wt_gens[i]->get_positive_sequence_complex_current_in_kA();
                    I2 = wt_gens[i]->get_negative_sequence_complex_current_in_kA();
                    I0 = wt_gens[i]->get_zero_sequence_complex_current_in_kA();
                }

                snprintf(buffer, 1000, "WT generator,ID %s", wt_gens[i]->get_identifier().c_str());
                file<< get_formatted_information1(buffer, I1,I2, I0, true);
                file<< get_formatted_information2(",", I1, I2, I0, true);
            }

            vector<LOAD*> loads = psdb.get_loads_connecting_to_bus(busnum);
            n = loads.size();
            for(unsigned int i=0; i<n; i++)
            {
                complex<double> I1, I2, I0;
                if(units==PU)
                {
                    I1 = loads[i]->get_positive_sequence_complex_current_in_pu();
                    I2 = loads[i]->get_negative_sequence_complex_current_in_pu();
                    I0 = loads[i]->get_zero_sequence_complex_current_in_pu();
                }
                else if(units==PHYSICAL)
                {
                    I1 = loads[i]->get_positive_sequence_complex_current_in_kA();
                    I2 = loads[i]->get_negative_sequence_complex_current_in_kA();
                    I0 = loads[i]->get_zero_sequence_complex_current_in_kA();
                }

                snprintf(buffer, 1000, "Load,ID %s", loads[i]->get_identifier().c_str());
                file<< get_formatted_information1(buffer, I1,I2, I0, true);
                file<< get_formatted_information2(",", I1, I2, I0, true);
            }

            vector<FIXED_SHUNT*> shunts = psdb.get_fixed_shunts_connecting_to_bus(busnum);
            n = shunts.size();
            for(unsigned int i=0; i<n; i++)
            {
                complex<double> I1, I2, I0;
                if(units==PU)
                {
                    I1 = shunts[i]->get_positive_sequence_complex_current_in_pu();
                    I2 = shunts[i]->get_negative_sequence_complex_current_in_pu();
                    I0 = shunts[i]->get_zero_sequence_complex_current_in_pu();
                }
                else
                {
                    I1 = shunts[i]->get_positive_sequence_complex_current_in_kA();
                    I2 = shunts[i]->get_negative_sequence_complex_current_in_kA();
                    I0 = shunts[i]->get_zero_sequence_complex_current_in_kA();
                }
                snprintf(buffer, 1000, "Shunt,ID %s", shunts[i]->get_identifier().c_str());
                file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
                file<< get_formatted_information2(",", I1, I2, I0, true);
            }

            vector<PV_UNIT*> pv_units = psdb.get_pv_units_connecting_to_bus(busnum);
            n = pv_units.size();
            for(unsigned int i=0; i<n; i++)
            {
                complex<double> I1, I2, I0;
                if(units==PU)
                {
                    I1 = pv_units[i]->get_positive_sequence_complex_current_in_pu();
                    I2 = pv_units[i]->get_negative_sequence_complex_current_in_pu();
                    I0 = pv_units[i]->get_zero_sequence_complex_current_in_pu();
                }
                else
                {
                    I1 = pv_units[i]->get_positive_sequence_complex_current_in_kA();
                    I2 = pv_units[i]->get_negative_sequence_complex_current_in_kA();
                    I0 = pv_units[i]->get_zero_sequence_complex_current_in_kA();
                }
                snprintf(buffer, 1000, "PV_unit,ID %s", pv_units[i]->get_identifier().c_str());
                file<< get_formatted_information1(osstream.str(), I1, I2, I0, true);
                file<< get_formatted_information2(",", I1, I2, I0, true);
            }

            file<<endl;
        }
        file.close();
    }
    else
    {
        osstream<<"File '"<<filename<<"' cannot be opened for saving short circuit result to file."<<endl
                <<"No short circuit result will be exported.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void SHORT_CIRCUIT_SOLVER::report_positive_sequence_Y_matrix()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.report_positive_sequence_Y_matrix();
}
void SHORT_CIRCUIT_SOLVER::report_negative_sequence_Y_matrix()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.report_negative_sequence_Y_matrix();
}
void SHORT_CIRCUIT_SOLVER::report_zero_sequence_Y_matrix()
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.report_zero_sequence_Y_matrix();
}

void SHORT_CIRCUIT_SOLVER::save_positive_sequence_Y_matrix_to_file(const string& filename)
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.save_positive_sequence_Y_matrix_to_file(filename);
}

void SHORT_CIRCUIT_SOLVER::save_negative_sequence_Y_matrix_to_file(const string& filename)
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.save_negative_sequence_Y_matrix_to_file(filename);
}

void SHORT_CIRCUIT_SOLVER::save_zero_sequence_Y_matrix_to_file(const string& filename)
{
    NETWORK_MATRIX& network_matrix = get_network_matrix();
    network_matrix.save_zero_sequence_Y_matrix_to_file(filename);
}
