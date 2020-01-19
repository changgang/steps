#include "header/network/jacobian_builder.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/STEPS.h"

#include <istream>
#include <iostream>
#include <fstream>

using namespace std;

JACOBIAN_BUILDER::JACOBIAN_BUILDER()
{
    network_matrix = NULL;
}

JACOBIAN_BUILDER::~JACOBIAN_BUILDER()
{
    /*
    jacobian_delta_p_over_angle.clear();
    jacobian_delta_p_over_voltage.clear();
    jacobian_delta_q_over_angle.clear();
    jacobian_delta_q_over_voltage.clear();
    */
}


void JACOBIAN_BUILDER::set_network_matrix(NETWORK_MATRIX& network_matrix)
{
    this->network_matrix = &network_matrix;
}


NETWORK_MATRIX* JACOBIAN_BUILDER::get_network_Y_matrix() const
{
    return network_matrix;
}


bool JACOBIAN_BUILDER::is_network_matrix_set() const
{
    if(get_network_Y_matrix()!=NULL)
        return true;
    else
    {
        ostringstream osstream;
        osstream<<"Error. Jacobian builder is not connected to any network database."<<endl
          <<"No operation on the jacobian builder will work.";
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        toolkit.show_information_with_leading_time_stamp(osstream);
        return false;
    }
}

void JACOBIAN_BUILDER::build_seprate_jacobians()
{
    if(is_network_matrix_set())
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

        jacobian_delta_p_over_angle.clear();
        jacobian_delta_p_over_voltage.clear();
        jacobian_delta_q_over_angle.clear();
        jacobian_delta_q_over_voltage.clear();

        double der;

        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        int n = Y.get_matrix_entry_count();
        int row, column;
        unsigned int ibus, jbus;
        ostringstream osstream;
        for(int k=0; k!=n; ++k)
        {
            row = Y.get_row_number_of_entry_index(k);
            column = Y.get_column_number_of_entry_index(k);
            ibus = nw_db->get_physical_bus_number_of_internal_bus(row);
            jbus = nw_db->get_physical_bus_number_of_internal_bus(column);

            der = get_jacobian_delta_p_over_angle_of_internal_bus(row,column);
            if(not isnan(der))
                ;
            else
            {
                osstream<<"NAN is detected when building Jacobian matrix, dP/dA is NAN at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus "<<jbus<<")";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            jacobian_delta_p_over_angle.add_entry(row,column, der);
            der = get_jacobian_delta_p_over_voltage_of_internal_bus(row,column);
            if(not isnan(der))
                ;
            else
            {
                osstream<<"NAN is detected when building Jacobian matrix, dP/dV is NAN at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus "<<jbus<<")";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            jacobian_delta_p_over_voltage.add_entry(row,column, der);
            der = get_jacobian_delta_q_over_angle_of_internal_bus(row,column);
            if(not isnan(der))
                ;
            else
            {
                osstream<<"NAN is detected when building Jacobian matrix, dQ/dA is NAN at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus "<<jbus<<")";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            jacobian_delta_q_over_angle.add_entry(row,column, der);
            der = get_jacobian_delta_q_over_voltage_of_internal_bus(row,column);
            if(not isnan(der))
                ;
            else
            {
                osstream<<"NAN is detected when building Jacobian matrix, dQ/dV is NAN at row: "<<row<<"(bus "<<ibus<<"), col: "<<column<<"(bus "<<jbus<<")";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            jacobian_delta_q_over_voltage.add_entry(row,column, der);
        }
        jacobian_delta_p_over_angle.compress_and_merge_duplicate_entries();
        jacobian_delta_p_over_voltage.compress_and_merge_duplicate_entries();
        jacobian_delta_q_over_angle.compress_and_merge_duplicate_entries();
        jacobian_delta_q_over_voltage.compress_and_merge_duplicate_entries();

        osstream<<"Done building separate jacobian matrix.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void JACOBIAN_BUILDER::update_seprate_jacobians()
{
    if(is_network_matrix_set())
    {
        update_jacobian_delta_p_over_angle();
        update_jacobian_delta_p_over_voltage();
        update_jacobian_delta_q_over_angle();
        update_jacobian_delta_q_over_voltage();
    }
}

void JACOBIAN_BUILDER::update_jacobian_delta_p_over_angle()
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        unsigned int nbus = psdb.get_in_service_bus_count();
        double der;
        complex<double> y;
        int k_start, k_end;
        k_start = 0;
        unsigned int ibus;
        for(unsigned int jbus = 0; jbus!=nbus; ++jbus)
        {
            k_end = Y.get_starting_index_of_column(jbus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                ibus = Y.get_row_number_of_entry_index(k);
                if(ibus<=jbus)
                {
                    if(ibus<jbus)
                    {
                        der = get_jacobian_delta_p_over_angle_of_internal_bus(ibus,jbus);
                        jacobian_delta_p_over_angle.change_entry_value(ibus,jbus, der);
                        der = get_jacobian_delta_p_over_angle_of_internal_bus(jbus,ibus);
                        jacobian_delta_p_over_angle.change_entry_value(jbus,ibus, der);
                    }
                    else
                    {
                        der = get_jacobian_delta_p_over_angle_of_internal_bus(jbus,jbus);
                        jacobian_delta_p_over_angle.change_entry_value(jbus,jbus, der);
                    }
                }
                else
                    break;
            }
            k_start = k_end;
        }
        return;

        for(unsigned int i=0; i!=nbus; ++i)
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                if(i!=j)
                {
                    y = Y.get_entry_value(i,j);
                    if(y!=0.0)
                    {
                        der = get_jacobian_delta_p_over_angle_of_internal_bus(i,j);
                        jacobian_delta_p_over_angle.change_entry_value(i,j,der);
                    }
                }
                else
                {
                    der = get_jacobian_delta_p_over_angle_of_internal_bus(i,i);
                    jacobian_delta_p_over_angle.change_entry_value(i,i, der);
                }
            }
        }
    }
}

void JACOBIAN_BUILDER::update_jacobian_delta_p_over_voltage()
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        unsigned int nbus = psdb.get_in_service_bus_count();
        double der;
        complex<double> y;
        int k_start, k_end;
        k_start = 0;
        unsigned int ibus;
        for(unsigned int jbus = 0; jbus!=nbus; ++jbus)
        {
            k_end = Y.get_starting_index_of_column(jbus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                ibus = Y.get_row_number_of_entry_index(k);
                if(ibus<=jbus)
                {
                    if(ibus<jbus)
                    {
                        der = get_jacobian_delta_p_over_voltage_of_internal_bus(ibus,jbus);
                        jacobian_delta_p_over_voltage.change_entry_value(ibus,jbus, der);
                        der = get_jacobian_delta_p_over_voltage_of_internal_bus(jbus,ibus);
                        jacobian_delta_p_over_voltage.change_entry_value(jbus,ibus, der);
                    }
                    else
                    {
                        der = get_jacobian_delta_p_over_voltage_of_internal_bus(jbus,jbus);
                        jacobian_delta_p_over_voltage.change_entry_value(jbus,jbus, der);
                    }
                }
                else
                    break;
            }
            k_start = k_end;
        }
        return;


        for(unsigned int i=0; i!=nbus; ++i)
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                if(i!=j)
                {
                    y = Y.get_entry_value(i,j);
                    if(y!=0.0)
                    {
                        der = get_jacobian_delta_p_over_voltage_of_internal_bus(i,j);
                        jacobian_delta_p_over_voltage.change_entry_value(i,j, der);
                    }
                }
                else
                {
                    der = get_jacobian_delta_p_over_voltage_of_internal_bus(i,i);
                    jacobian_delta_p_over_voltage.change_entry_value(i,i, der);
                }
            }
        }
    }
}

void JACOBIAN_BUILDER::update_jacobian_delta_q_over_angle()
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        unsigned int nbus = psdb.get_in_service_bus_count();
        double der;
        complex<double> y;

        int k_start, k_end;
        k_start = 0;
        unsigned int ibus;
        for(unsigned int jbus = 0; jbus!=nbus; ++jbus)
        {
            k_end = Y.get_starting_index_of_column(jbus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                ibus = Y.get_row_number_of_entry_index(k);
                if(ibus<=jbus)
                {
                    if(ibus<jbus)
                    {
                        der = get_jacobian_delta_q_over_angle_of_internal_bus(ibus,jbus);
                        jacobian_delta_q_over_angle.change_entry_value(ibus,jbus, der);
                        der = get_jacobian_delta_q_over_angle_of_internal_bus(jbus,ibus);
                        jacobian_delta_q_over_angle.change_entry_value(jbus,ibus, der);
                    }
                    else
                    {
                        der = get_jacobian_delta_q_over_angle_of_internal_bus(jbus,jbus);
                        jacobian_delta_q_over_angle.change_entry_value(jbus,jbus, der);
                    }
                }
                else//>
                    break;
            }
            k_start = k_end;
        }
        return;

        for(unsigned int i=0; i!=nbus; ++i)
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                if(i!=j)
                {
                    y = Y.get_entry_value(i,j);
                    if(y!=0.0)
                    {
                        der = get_jacobian_delta_q_over_angle_of_internal_bus(i,j);
                        jacobian_delta_q_over_angle.change_entry_value(i,j, der);
                    }
                }
                else
                {
                    der = get_jacobian_delta_q_over_angle_of_internal_bus(i,i);
                    jacobian_delta_q_over_angle.change_entry_value(i,i, der);
                }
            }
        }
    }
}

void JACOBIAN_BUILDER::update_jacobian_delta_q_over_voltage()
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        unsigned int nbus = psdb.get_in_service_bus_count();
        double der;
        complex<double> y;

        int k_start, k_end;
        k_start = 0;
        unsigned int ibus;
        for(unsigned int jbus = 0; jbus!=nbus; ++jbus)
        {
            k_end = Y.get_starting_index_of_column(jbus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                ibus = Y.get_row_number_of_entry_index(k);
                if(ibus<=jbus)
                {
                    if(ibus<jbus)
                    {
                        der = get_jacobian_delta_q_over_voltage_of_internal_bus(ibus,jbus);
                        jacobian_delta_q_over_voltage.change_entry_value(ibus,jbus, der);
                        der = get_jacobian_delta_q_over_voltage_of_internal_bus(jbus,ibus);
                        jacobian_delta_q_over_voltage.change_entry_value(jbus,ibus, der);
                    }
                    else
                    {
                        der = get_jacobian_delta_q_over_voltage_of_internal_bus(jbus,jbus);
                        jacobian_delta_q_over_voltage.change_entry_value(jbus,jbus, der);
                    }
                }
                else//>
                    break;
            }
            k_start = k_end;
        }
        return;

        for(unsigned int i=0; i!=nbus; ++i)
        {
            for(unsigned int j=0; j!=nbus; ++j)
            {
                if(i!=j)
                {
                    y = Y.get_entry_value(i,j);
                    if(y!=0.0)
                    {
                        der = get_jacobian_delta_q_over_voltage_of_internal_bus(i,j);
                        jacobian_delta_q_over_voltage.change_entry_value(i,j, der);
                    }
                }
                else
                {
                    der = get_jacobian_delta_q_over_voltage_of_internal_bus(i,i);
                    jacobian_delta_q_over_voltage.change_entry_value(i,i, der);
                }
            }
        }
    }
}

double JACOBIAN_BUILDER::get_jacobian_delta_p_over_angle_of_internal_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int IBUS = nw_db->get_physical_bus_number_of_internal_bus(ibus), JBUS;
        complex<double> Vi = psdb.get_bus_positive_sequence_complex_voltage_in_pu(IBUS), Vj;
        complex<double> y;
        double g, b;
        double Vim, Vjm, anglei, anglej, dangle;

        Vim = steps_fast_complex_abs(Vi);
        anglei = steps_fast_complex_arg(Vi);

        double der = 0.0;

        if(ibus!=jbus)
        {
            y = Y.get_entry_value(ibus, jbus);
            if(y!=0.0)
            {
                JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                Vjm = steps_fast_complex_abs(Vj);
                anglej = steps_fast_complex_arg(Vj);
                g = y.real();
                b = y.imag();
                dangle = anglei - anglej;
                der = -Vim*Vjm*(g*sin(dangle)-b*cos(dangle));
            }
        }
        else
        {
            int k_start = Y.get_starting_index_of_column(ibus);
            int k_end = Y.get_starting_index_of_column(ibus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                jbus = Y.get_row_number_of_entry_index(k);

                y = Y.get_entry_value(ibus, jbus);
                if(y!=0.0 and ibus != jbus)
                {
                    JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                    Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                    Vjm = steps_fast_complex_abs(Vj);
                    anglej = steps_fast_complex_arg(Vj);
                    g = y.real();
                    b = y.imag();
                    dangle = anglei - anglej;
                    der += Vjm*(g*sin(dangle)-b*cos(dangle));
                }
            }
            der *= Vim;
        }
        return der;
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_p_over_voltage_of_internal_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int IBUS = nw_db->get_physical_bus_number_of_internal_bus(ibus), JBUS;
        complex<double> Vi = psdb.get_bus_positive_sequence_complex_voltage_in_pu(IBUS), Vj;
        complex<double> y;
        double g, b;
        double Vim, Vjm, anglei, anglej, dangle;

        Vim = steps_fast_complex_abs(Vi);
        anglei = steps_fast_complex_arg(Vi);

        double der = 0.0;

        if(ibus!=jbus)
        {
            y = Y.get_entry_value(ibus, jbus);
            if(y!=0.0)
            {
                JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                Vjm = steps_fast_complex_abs(Vj);
                anglej = steps_fast_complex_arg(Vj);
                g = y.real();
                b = y.imag();
                dangle = anglei - anglej;
                der = -Vim*(g*cos(dangle)+b*sin(dangle));
            }
        }
        else
        {
            int k_start = Y.get_starting_index_of_column(ibus);
            int k_end = Y.get_starting_index_of_column(ibus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                jbus = Y.get_row_number_of_entry_index(k);

                y = Y.get_entry_value(ibus, jbus);
                if(y!=0.0)
                {
                    JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                    Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                    Vjm = steps_fast_complex_abs(Vj);
                    anglej = steps_fast_complex_arg(Vj);
                    g = y.real();
                    b = y.imag();
                    dangle = anglei - anglej;

                    if(ibus != jbus)
                        der -= Vjm*(g*cos(dangle)+b*sin(dangle));
                    else
                        der -= 2.0*Vim*g;
                }
            }
        }
        return der;
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_q_over_angle_of_internal_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int IBUS = nw_db->get_physical_bus_number_of_internal_bus(ibus), JBUS;
        complex<double> Vi = psdb.get_bus_positive_sequence_complex_voltage_in_pu(IBUS), Vj;
        complex<double> y;
        double g, b;
        double Vim, Vjm, anglei, anglej, dangle;

        Vim = steps_fast_complex_abs(Vi);
        anglei = steps_fast_complex_arg(Vi);

        double der = 0.0;

        if(ibus!=jbus)
        {
            y = Y.get_entry_value(ibus, jbus);
            if(y!=0.0)
            {
                JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                Vjm = steps_fast_complex_abs(Vj);
                anglej = steps_fast_complex_arg(Vj);
                g = y.real();
                b = y.imag();
                dangle = anglei - anglej;
                der = Vim*Vjm*(g*cos(dangle)+b*sin(dangle));
            }
        }
        else
        {
            int k_start = Y.get_starting_index_of_column(ibus);
            int k_end = Y.get_starting_index_of_column(ibus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                jbus = Y.get_row_number_of_entry_index(k);

                y = Y.get_entry_value(ibus, jbus);
                if(y!=0.0 and ibus != jbus)
                {
                    JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                    Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                    Vjm = steps_fast_complex_abs(Vj);
                    anglej = steps_fast_complex_arg(Vj);
                    g = y.real();
                    b = y.imag();
                    dangle = anglei - anglej;
                    der -= Vjm*(g*cos(dangle)+b*sin(dangle));
                }
            }
            der *= Vim;
        }
        return der;
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_q_over_voltage_of_internal_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();

        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        unsigned int IBUS = nw_db->get_physical_bus_number_of_internal_bus(ibus), JBUS;
        complex<double> Vi = psdb.get_bus_positive_sequence_complex_voltage_in_pu(IBUS), Vj;
        complex<double> y;
        double g, b;
        double Vim, Vjm, anglei, anglej, dangle;

        Vim = steps_fast_complex_abs(Vi);
        anglei = steps_fast_complex_arg(Vi);

        double der = 0.0;

        if(ibus!=jbus)
        {
            y = Y.get_entry_value(ibus, jbus);
            if(y!=0.0)
            {
                JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                Vjm = steps_fast_complex_abs(Vj);
                anglej = steps_fast_complex_arg(Vj);
                g = y.real();
                b = y.imag();
                dangle = anglei - anglej;
                der = -Vim*(g*sin(dangle)-b*cos(dangle));
            }
        }
        else
        {
            int k_start = Y.get_starting_index_of_column(ibus);
            int k_end = Y.get_starting_index_of_column(ibus+1);
            for(int k=k_start; k!=k_end; ++k)
            {
                jbus = Y.get_row_number_of_entry_index(k);

                y = Y.get_entry_value(ibus, jbus);
                if(y!=0.0)
                {
                    JBUS = nw_db->get_physical_bus_number_of_internal_bus(jbus);
                    Vj = psdb.get_bus_positive_sequence_complex_voltage_in_pu(JBUS);
                    Vjm = steps_fast_complex_abs(Vj);
                    anglej = steps_fast_complex_arg(Vj);
                    g = y.real();
                    b = y.imag();
                    dangle = anglei - anglej;

                    if(ibus != jbus)
                        der -= Vjm*(g*sin(dangle)-b*cos(dangle));
                    else
                        der += 2.0*Vim*b;
                }
            }
        }
        return der;
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_p_over_angle_of_physical_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(ibus) and psdb.is_bus_exist(jbus))
        {
            unsigned int ibus_internal = nw_db->get_internal_bus_number_of_physical_bus(ibus);
            unsigned int jbus_internal = nw_db->get_internal_bus_number_of_physical_bus(jbus);

            return get_jacobian_delta_p_over_angle_of_internal_bus(ibus_internal, jbus_internal);
        }
        else
        {
            ostringstream osstream;
            osstream<<"Either physical bus "<<ibus<<" or "<<jbus<<" doesn't exist when getting jacobian of delta P over bus angle.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            return 0.0;
        }
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_p_over_voltage_of_physical_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(ibus) and psdb.is_bus_exist(jbus))
        {
            unsigned int ibus_internal = nw_db->get_internal_bus_number_of_physical_bus(ibus);
            unsigned int jbus_internal = nw_db->get_internal_bus_number_of_physical_bus(jbus);

            return get_jacobian_delta_p_over_voltage_of_internal_bus(ibus_internal, jbus_internal);
        }
        else
        {
            ostringstream osstream;
            osstream<<"Either physical bus "<<ibus<<" or "<<jbus<<" doesn't exist when getting jacobian of delta P over bus voltage.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            return 0.0;
        }
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_q_over_angle_of_physical_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(ibus) and psdb.is_bus_exist(jbus))
        {
            unsigned int ibus_internal = nw_db->get_internal_bus_number_of_physical_bus(ibus);
            unsigned int jbus_internal = nw_db->get_internal_bus_number_of_physical_bus(jbus);

            return get_jacobian_delta_q_over_angle_of_internal_bus(ibus_internal, jbus_internal);
        }
        else
        {
            ostringstream osstream;
            osstream<<"Either physical bus "<<ibus<<" or "<<jbus<<" doesn't exist when getting jacobian of delta Q over bus angle.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            return 0.0;
        }
    }
    else
        return 0.0;
}

double JACOBIAN_BUILDER::get_jacobian_delta_q_over_voltage_of_physical_bus(unsigned int ibus, unsigned int jbus)
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(ibus) and psdb.is_bus_exist(jbus))
        {
            unsigned int ibus_internal = nw_db->get_internal_bus_number_of_physical_bus(ibus);
            unsigned int jbus_internal = nw_db->get_internal_bus_number_of_physical_bus(jbus);

            return get_jacobian_delta_q_over_voltage_of_internal_bus(ibus_internal, jbus_internal);
        }
        else
        {
            ostringstream osstream;
            osstream<<"Either physical bus "<<ibus<<" or "<<jbus<<" doesn't exist when getting jacobian of delta Q over bus voltage.";
            toolkit.show_information_with_leading_time_stamp(osstream);

            return 0.0;
        }
    }
    else
        return 0.0;
}

STEPS_SPARSE_MATRIX JACOBIAN_BUILDER::get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(
                                                                const vector<unsigned int>& internal_P_equation_buses,
                                                                const vector<unsigned int>& internal_Q_equation_buses)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(not is_network_matrix_set())
    {
        osstream<<"Full decoupled jacobian matrix will not be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    //osstream<<"go forming the full jacobian matrix.";
    //toolkit.show_information_with_leading_time_stamp(osstream);

    NETWORK_MATRIX* nw_db = get_network_Y_matrix();
    const STEPS_COMPLEX_SPARSE_MATRIX& Y = nw_db->get_network_Y_matrix();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    unsigned int n_internal_P_equation_buses = internal_P_equation_buses.size(),
           n_internal_Q_equation_buses = internal_Q_equation_buses.size();

    vector<bool> is_a_P_equation_bus, is_a_Q_equation_bus;
    vector<unsigned int> index_of_a_P_equation_bus, index_of_a_Q_equation_bus;
    unsigned int n = psdb.get_in_service_bus_count();
    is_a_P_equation_bus.reserve(n);
    is_a_Q_equation_bus.reserve(n);
    index_of_a_P_equation_bus.reserve(n);
    index_of_a_Q_equation_bus.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
    {
        is_a_P_equation_bus.push_back(false);
        is_a_Q_equation_bus.push_back(false);
        index_of_a_P_equation_bus.push_back(INDEX_NOT_EXIST);
        index_of_a_Q_equation_bus.push_back(INDEX_NOT_EXIST);
    }
    unsigned int bus;
    for(unsigned int i=0; i!=n_internal_P_equation_buses; ++i)
    {
        bus = internal_P_equation_buses[i];
        is_a_P_equation_bus[bus] = true;
        index_of_a_P_equation_bus[bus] = i;
    }
    for(unsigned int i=0; i!=n_internal_Q_equation_buses; ++i)
    {
        bus = internal_Q_equation_buses[i];
        is_a_Q_equation_bus[bus] = true;
        index_of_a_Q_equation_bus[bus] = i;
    }


    STEPS_SPARSE_MATRIX full_jacobian;

    full_jacobian.clear();

    complex<double> cvalue;
    double rvalue;

    unsigned int i,j;
    /* the following codes are replace on June 28, 2019 for higher performance
    unsigned int ibus, jbus;
    int nentry = Y.get_matrix_entry_count();

    for(int k=0; k!=nentry; ++k)
    {
        ibus = Y.get_row_number_of_entry_index(k);
        jbus = Y.get_column_number_of_entry_index(k);
        if(is_a_P_equation_bus[ibus])
        {
            i = index_of_a_P_equation_bus[ibus];
            if(is_a_P_equation_bus[jbus])
            {
                j = index_of_a_P_equation_bus[jbus];
                cvalue = jacobian_delta_p_over_angle.get_entry_value(ibus,jbus);
                rvalue = cvalue.real();
                full_jacobian.add_entry(i,j,rvalue);
            }
            if(is_a_Q_equation_bus[jbus])
            {
                j = index_of_a_Q_equation_bus[jbus];
                cvalue = jacobian_delta_p_over_voltage.get_entry_value(ibus,jbus);
                rvalue = cvalue.real();
                full_jacobian.add_entry(i,j+n_internal_P_equation_buses,rvalue);
            }
        }
        if(is_a_Q_equation_bus[ibus])
        {
            i = index_of_a_Q_equation_bus[ibus];
            if(is_a_P_equation_bus[jbus])
            {
                j = index_of_a_P_equation_bus[jbus];
                cvalue = jacobian_delta_q_over_angle.get_entry_value(ibus,jbus);
                rvalue = cvalue.real();
                full_jacobian.add_entry(i+n_internal_P_equation_buses,j,rvalue);
            }
            if(is_a_Q_equation_bus[jbus])
            {
                j = index_of_a_Q_equation_bus[jbus];
                cvalue = jacobian_delta_q_over_voltage.get_entry_value(ibus,jbus);
                rvalue = cvalue.real();
                full_jacobian.add_entry(i+n_internal_P_equation_buses,j+n_internal_P_equation_buses,rvalue);
            }
        }
    }*/

    int ncol = Y.get_matrix_size();
    for(int col=0; col!=ncol; ++col)
    {
        int jbus = col;

        int k_start = Y.get_starting_index_of_column(col);
        int k_end = Y.get_starting_index_of_column(col+1);
        for(int k=k_start; k!=k_end; ++k)
        {
            int row = Y.get_row_number_of_entry_index(k);
            int ibus = row;
            if(is_a_P_equation_bus[ibus])
            {
                i = index_of_a_P_equation_bus[ibus];
                if(is_a_P_equation_bus[jbus])
                {
                    j = index_of_a_P_equation_bus[jbus];
                    cvalue = jacobian_delta_p_over_angle.get_entry_value(ibus,jbus);
                    rvalue = cvalue.real();
                    full_jacobian.add_entry(i,j,rvalue);
                }
                if(is_a_Q_equation_bus[jbus])
                {
                    j = index_of_a_Q_equation_bus[jbus];
                    cvalue = jacobian_delta_p_over_voltage.get_entry_value(ibus,jbus);
                    rvalue = cvalue.real();
                    full_jacobian.add_entry(i,j+n_internal_P_equation_buses,rvalue);
                }
            }
            if(is_a_Q_equation_bus[ibus])
            {
                i = index_of_a_Q_equation_bus[ibus];
                if(is_a_P_equation_bus[jbus])
                {
                    j = index_of_a_P_equation_bus[jbus];
                    cvalue = jacobian_delta_q_over_angle.get_entry_value(ibus,jbus);
                    rvalue = cvalue.real();
                    full_jacobian.add_entry(i+n_internal_P_equation_buses,j,rvalue);
                }
                if(is_a_Q_equation_bus[jbus])
                {
                    j = index_of_a_Q_equation_bus[jbus];
                    cvalue = jacobian_delta_q_over_voltage.get_entry_value(ibus,jbus);
                    rvalue = cvalue.real();
                    full_jacobian.add_entry(i+n_internal_P_equation_buses,j+n_internal_P_equation_buses,rvalue);
                }
            }
        }
    }

    full_jacobian.compress_and_merge_duplicate_entries();

    //osstream<<"done forming the full jacobian matrix.\nnow returning the matrix.";
    //toolkit.show_information_with_leading_time_stamp(osstream);
    return full_jacobian;

    /*
    unsigned int n_internal_P_equation_buses = internal_P_equation_buses.size(),
           n_internal_Q_equation_buses = internal_Q_equation_buses.size();
    unsigned int row, col;
    complex<double>cvalue;
    double rvalue;

    static STEPS_SPARSE_MATRIX full_jacobian;

    full_jacobian.clear();

    for(unsigned int i=0; i!=n_internal_P_equation_buses; ++i)
    {
        row = internal_P_equation_buses[i];
        for(unsigned int j=0; j!=n_internal_P_equation_buses; ++j)
        {
            col = internal_P_equation_buses[j];
            cvalue = jacobian_delta_p_over_angle.get_entry_value(row,col);
            rvalue = cvalue.real();
            if(rvalue != 0.0)
                full_jacobian.add_entry(i,j,rvalue);
        }
    }

    for(unsigned int i=0; i!=n_internal_P_equation_buses; ++i)
    {
        row = internal_P_equation_buses[i];
        for(unsigned int j=0; j!=n_internal_Q_equation_buses; ++j)
        {
            col = internal_Q_equation_buses[j];
            cvalue = jacobian_delta_p_over_voltage.get_entry_value(row,col);
            rvalue = cvalue.real();
            if(rvalue != 0.0)
                full_jacobian.add_entry(i,j+n_internal_P_equation_buses,rvalue);
        }
    }

    for(unsigned int i=0; i!=n_internal_Q_equation_buses; ++i)
    {
        row = internal_Q_equation_buses[i];
        for(unsigned int j=0; j!=n_internal_P_equation_buses; ++j)
        {
            col = internal_P_equation_buses[j];
            cvalue = jacobian_delta_q_over_angle.get_entry_value(row,col);
            rvalue = cvalue.real();
            if(rvalue != 0.0)
                full_jacobian.add_entry(i+n_internal_P_equation_buses,j,rvalue);
        }
    }

    for(unsigned int i=0; i!=n_internal_Q_equation_buses; ++i)
    {
        row = internal_Q_equation_buses[i];
        for(unsigned int j=0; j!=n_internal_Q_equation_buses; ++j)
        {
            col = internal_Q_equation_buses[j];
            cvalue = jacobian_delta_q_over_voltage.get_entry_value(row,col);
            rvalue = cvalue.real();
            if(rvalue != 0.0)
                full_jacobian.add_entry(i+n_internal_P_equation_buses,j+n_internal_P_equation_buses,rvalue);
        }
    }
    full_jacobian.compress_and_merge_duplicate_entries();
    return full_jacobian;
    */
}

STEPS_SPARSE_MATRIX JACOBIAN_BUILDER::get_decoupled_B_jacobian_with_P_equation_internal_buses(const vector<unsigned int>& internal_P_equation_buses)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(not is_network_matrix_set())
    {
        osstream<<"Decoupled BP matrix will not be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    osstream<<"Building BP matrix for active power solution.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    NETWORK_MATRIX* nw_db = get_network_Y_matrix();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    const STEPS_SPARSE_MATRIX& BP = nw_db->get_decoupled_network_BP_matrix();

    unsigned int n_internal_P_equation_buses = internal_P_equation_buses.size();

    vector<bool> is_a_P_equation_bus;
    vector<unsigned int> index_of_a_P_equation_bus;
    unsigned int n = psdb.get_in_service_bus_count();
    is_a_P_equation_bus.reserve(n);
    index_of_a_P_equation_bus.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
    {
        is_a_P_equation_bus.push_back(false);
        index_of_a_P_equation_bus.push_back(INDEX_NOT_EXIST);
    }
    unsigned int bus;
    for(unsigned int i=0; i!=n_internal_P_equation_buses; ++i)
    {
        bus = internal_P_equation_buses[i];
        is_a_P_equation_bus[bus] = true;
        index_of_a_P_equation_bus[bus] = i;
    }


    static STEPS_SPARSE_MATRIX B_jacobian;

    B_jacobian.clear();

    complex<double> cvalue;
    double rvalue;

    /* the following codes are replace on June 28, 2019 for higher performance
    unsigned int ibus, jbus;
    unsigned int i,j;
    int nentry = BP.get_matrix_entry_count();

    for(int k=0; k!=nentry; ++k)
    {
        ibus = BP.get_row_number_of_entry_index(k);
        jbus = BP.get_column_number_of_entry_index(k);
        if(is_a_P_equation_bus[ibus] and is_a_P_equation_bus[jbus])
        {
            i = index_of_a_P_equation_bus[ibus];
            j = index_of_a_P_equation_bus[jbus];
            cvalue = BP.get_entry_value(ibus,jbus);
            rvalue = cvalue.imag();
            B_jacobian.add_entry(i,j,rvalue);
        }
    }
    */

    int ncol = BP.get_matrix_size();
    int k_start=0, k_end = 0;
    for(int col=0; col!=ncol; ++col)
    {
        int jbus = col;
        if(is_a_P_equation_bus[jbus])
        {
            int j = index_of_a_P_equation_bus[jbus];

            k_start = BP.get_starting_index_of_column(col);
            k_end = BP.get_starting_index_of_column(col+1);
            for(int k = k_start; k!=k_end; ++k)
            {
                int row = BP.get_row_number_of_entry_index(k);
                int ibus = row;
                if(is_a_P_equation_bus[ibus])
                {
                    int i = index_of_a_P_equation_bus[ibus];
                    cvalue = BP.get_entry_value(k);
                    rvalue = cvalue.imag();
                    B_jacobian.add_entry(i,j,rvalue);
                }
            }
        }
    }

    B_jacobian.compress_and_merge_duplicate_entries();

    osstream<<"Done building BP matrix for active power solution.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return B_jacobian;
}

STEPS_SPARSE_MATRIX JACOBIAN_BUILDER::get_decoupled_B_jacobian_with_Q_equation_internal_buses(const vector<unsigned int>& internal_Q_equation_buses)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    ostringstream osstream;
    if(not is_network_matrix_set())
    {
        osstream<<"Decoupled BQ matrix will not be returned.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    osstream<<"Building BQ matrix for active power solution.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    NETWORK_MATRIX* nw_db = get_network_Y_matrix();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    const STEPS_SPARSE_MATRIX& BQ = nw_db->get_decoupled_network_BQ_matrix();

    unsigned int n_internal_Q_equation_buses = internal_Q_equation_buses.size();

    vector<bool> is_a_Q_equation_bus;
    vector<unsigned int> index_of_a_Q_equation_bus;
    unsigned int n = psdb.get_in_service_bus_count();
    is_a_Q_equation_bus.reserve(n);
    index_of_a_Q_equation_bus.reserve(n);
    for(unsigned int i=0; i!=n; ++i)
    {
        is_a_Q_equation_bus.push_back(false);
        index_of_a_Q_equation_bus.push_back(INDEX_NOT_EXIST);
    }
    unsigned int bus;
    for(unsigned int i=0; i!=n_internal_Q_equation_buses; ++i)
    {
        bus = internal_Q_equation_buses[i];
        is_a_Q_equation_bus[bus] = true;
        index_of_a_Q_equation_bus[bus] = i;
    }


    static STEPS_SPARSE_MATRIX B_jacobian;

    B_jacobian.clear();

    complex<double> cvalue;
    double rvalue;

    /* the following codes are replace on June 28, 2019 for higher performance
    unsigned int ibus, jbus;
    unsigned int i,j;
    int nentry = BQ.get_matrix_entry_count();

    for(int k=0; k!=nentry; ++k)
    {
        ibus = BQ.get_row_number_of_entry_index(k);
        jbus = BQ.get_column_number_of_entry_index(k);
        if(is_a_Q_equation_bus[ibus] and is_a_Q_equation_bus[jbus])
        {
            i = index_of_a_Q_equation_bus[ibus];
            j = index_of_a_Q_equation_bus[jbus];
            cvalue = BQ.get_entry_value(ibus,jbus);
            rvalue = cvalue.imag();
            B_jacobian.add_entry(i,j,rvalue);
        }
    }
    */

    int ncol = BQ.get_matrix_size();
    int k_start=0, k_end = 0;
    for(int col=0; col!=ncol; ++col)
    {
        int jbus = col;
        if(is_a_Q_equation_bus[jbus])
        {
            int j = index_of_a_Q_equation_bus[jbus];

            k_start = BQ.get_starting_index_of_column(col);
            k_end = BQ.get_starting_index_of_column(col+1);
            for(int k = k_start; k!=k_end; ++k)
            {
                int row = BQ.get_row_number_of_entry_index(k);
                int ibus = row;
                if(is_a_Q_equation_bus[ibus])
                {
                    int i = index_of_a_Q_equation_bus[ibus];
                    cvalue = BQ.get_entry_value(k);
                    rvalue = cvalue.imag();
                    B_jacobian.add_entry(i,j,rvalue);
                }
            }
        }
    }

    B_jacobian.compress_and_merge_duplicate_entries();


    osstream<<"Done building BQ matrix for active power solution.";
    toolkit.show_information_with_leading_time_stamp(osstream);

    return B_jacobian;
}

void JACOBIAN_BUILDER::show_seprate_jacobians()
{
    if(is_network_matrix_set())
    {
        NETWORK_MATRIX* nw_db = get_network_Y_matrix();
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

        unsigned int MAX_ENTRIES_TO_SHOW = 50;

        ostringstream osstream;
        int row, col;
        unsigned int row_bus, col_bus;
        complex<double> cvalue;
        double rvalue;

        int nentry = jacobian_delta_p_over_angle.get_matrix_entry_count();
        unsigned int entries_to_show = MAX_ENTRIES_TO_SHOW;
        if(entries_to_show > (unsigned int)(nentry))
            entries_to_show = (unsigned int)(nentry);
        if(entries_to_show == 1)
            entries_to_show = 0;

        osstream<<"The first "<<entries_to_show<<" JACOBIAN_BUILDER entries of P over angle are:"<<endl
          <<"row      row_bus  column   col_bus  value";
        toolkit.show_information_with_leading_time_stamp(osstream);

        for(unsigned int k=0; k!=entries_to_show; ++k)
        {
            row = jacobian_delta_p_over_angle.get_row_number_of_entry_index(k);
            col = jacobian_delta_p_over_angle.get_column_number_of_entry_index(k);

            row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
            col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

            cvalue = jacobian_delta_p_over_angle.get_entry_value(k);
            rvalue = cvalue.real();

            osstream<<setw(8)<<row<<" "
              <<setw(8)<<row_bus<<" "
              <<setw(8)<<col<<" "
              <<setw(8)<<col_bus<<" "
              <<setw(10)<<setprecision(6)<<fixed<<rvalue;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        nentry = jacobian_delta_p_over_voltage.get_matrix_entry_count();
        entries_to_show = MAX_ENTRIES_TO_SHOW;
        if(entries_to_show > (unsigned int)(nentry))
            entries_to_show = (unsigned int)(nentry);
        if(entries_to_show == 1)
            entries_to_show = 0;

        osstream<<"The first "<<entries_to_show<<" JACOBIAN_BUILDER entries of P over voltage are:"<<endl
          <<"row      row_bus  column   col_bus  value";
        toolkit.show_information_with_leading_time_stamp(osstream);

        for(unsigned int k=0; k!=entries_to_show; ++k)
        {
            row = jacobian_delta_p_over_voltage.get_row_number_of_entry_index(k);
            col = jacobian_delta_p_over_voltage.get_column_number_of_entry_index(k);

            row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
            col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

            cvalue = jacobian_delta_p_over_voltage.get_entry_value(k);
            rvalue = cvalue.real();

            osstream<<setw(8)<<row<<" "
              <<setw(8)<<row_bus<<" "
              <<setw(8)<<col<<" "
              <<setw(8)<<col_bus<<" "
              <<setw(10)<<setprecision(6)<<fixed<<rvalue;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        nentry = jacobian_delta_q_over_angle.get_matrix_entry_count();
        entries_to_show = MAX_ENTRIES_TO_SHOW;
        if(entries_to_show > (unsigned int)(nentry))
            entries_to_show = (unsigned int)(nentry);
        if(entries_to_show == 1)
            entries_to_show = 0;

        osstream<<"The first "<<entries_to_show<<" JACOBIAN_BUILDER entries of Q over angle are:"<<endl
          <<"row      row_bus  column   col_bus  value";
        toolkit.show_information_with_leading_time_stamp(osstream);

        for(unsigned int k=0; k!=entries_to_show; ++k)
        {
            row = jacobian_delta_q_over_angle.get_row_number_of_entry_index(k);
            col = jacobian_delta_q_over_angle.get_column_number_of_entry_index(k);

            row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
            col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

            cvalue = jacobian_delta_q_over_angle.get_entry_value(k);
            rvalue = cvalue.real();

            osstream<<setw(8)<<row<<" "
              <<setw(8)<<row_bus<<" "
              <<setw(8)<<col<<" "
              <<setw(8)<<col_bus<<" "
              <<setw(10)<<setprecision(6)<<fixed<<rvalue;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        nentry = jacobian_delta_q_over_voltage.get_matrix_entry_count();
        entries_to_show = MAX_ENTRIES_TO_SHOW;
        if(entries_to_show > (unsigned int)(nentry))
            entries_to_show = (unsigned int)(nentry);
        if(entries_to_show == 1)
            entries_to_show = 0;

        osstream<<"The first "<<entries_to_show<<" JACOBIAN_BUILDER entries of Q over voltage are:"<<endl
          <<"row      row_bus  column   col_bus  value";
        toolkit.show_information_with_leading_time_stamp(osstream);

        for(unsigned int k=0; k!=entries_to_show; ++k)
        {
            row = jacobian_delta_q_over_voltage.get_row_number_of_entry_index(k);
            col = jacobian_delta_q_over_voltage.get_column_number_of_entry_index(k);

            row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
            col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

            cvalue = jacobian_delta_q_over_voltage.get_entry_value(k);
            rvalue = cvalue.real();

            osstream<<setw(8)<<row<<" "
              <<setw(8)<<row_bus<<" "
              <<setw(8)<<col<<" "
              <<setw(8)<<col_bus<<" "
              <<setw(10)<<setprecision(6)<<fixed<<rvalue;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void JACOBIAN_BUILDER::save_jacobian_matrix_to_file(const string& filename) const
{
    if(is_network_matrix_set())
    {
        STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

        ofstream file;
        ostringstream osstream;

        file.open(filename);
        if(file.is_open())
        {
            char buffer[1000];

            file<<"ROW_TYPE,COLUMN_TYPE,ROW,ROW_BUS,COLUMN,COLUMN_BUS,VALUE"<<endl;

            NETWORK_MATRIX* nw_db = get_network_Y_matrix();

            int row, col;
            unsigned int row_bus, col_bus;
            complex<double> cvalue;
            double rvalue;

            unsigned int nentry;
            nentry = (unsigned int) jacobian_delta_p_over_angle.get_matrix_entry_count();
            for(unsigned int k=0; k!=nentry; ++k)
            {
                row = jacobian_delta_p_over_angle.get_row_number_of_entry_index(k);
                col = jacobian_delta_p_over_angle.get_column_number_of_entry_index(k);

                row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
                col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

                cvalue = jacobian_delta_p_over_angle.get_entry_value(k);
                rvalue = cvalue.real();

                /*file<<"P,A,"<<row<<","<<row_bus<<","<<col<<","<<col_bus<<","
                    <<setprecision(6)<<fixed<<rvalue<<endl;*/
                snprintf(buffer, 1000, "P,A,%d,%u,%d,%u,%.6f",row, row_bus, col, col_bus, rvalue);
                file<<buffer<<endl;
            }

            nentry = (unsigned int) jacobian_delta_p_over_voltage.get_matrix_entry_count();
            for(unsigned int k=0; k!=nentry; ++k)
            {
                row = jacobian_delta_p_over_voltage.get_row_number_of_entry_index(k);
                col = jacobian_delta_p_over_voltage.get_column_number_of_entry_index(k);

                row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
                col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

                cvalue = jacobian_delta_p_over_voltage.get_entry_value(k);
                rvalue = cvalue.real();

                /*file<<"P,V,"<<row<<","<<row_bus<<","<<col<<","<<col_bus<<","
                    <<setprecision(6)<<fixed<<rvalue<<endl;*/
                snprintf(buffer, 1000, "P,V,%d,%u,%d,%u,%.6f",row, row_bus, col, col_bus, rvalue);
                file<<buffer<<endl;
            }

            nentry = (unsigned int) jacobian_delta_q_over_angle.get_matrix_entry_count();
            for(unsigned int k=0; k!=nentry; ++k)
            {
                row = jacobian_delta_q_over_angle.get_row_number_of_entry_index(k);
                col = jacobian_delta_q_over_angle.get_column_number_of_entry_index(k);

                row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
                col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

                cvalue = jacobian_delta_q_over_angle.get_entry_value(k);
                rvalue = cvalue.real();

                /*file<<"Q,A,"<<row<<","<<row_bus<<","<<col<<","<<col_bus<<","
                    <<setprecision(6)<<fixed<<rvalue<<endl;*/
                snprintf(buffer, 1000, "Q,A,%d,%u,%d,%u,%.6f",row, row_bus, col, col_bus, rvalue);
                file<<buffer<<endl;
            }

            nentry = (unsigned int) jacobian_delta_q_over_voltage.get_matrix_entry_count();
            for(unsigned int k=0; k!=nentry; ++k)
            {
                row = jacobian_delta_q_over_voltage.get_row_number_of_entry_index(k);
                col = jacobian_delta_q_over_voltage.get_column_number_of_entry_index(k);

                row_bus = nw_db->get_physical_bus_number_of_internal_bus(row);
                col_bus = nw_db->get_physical_bus_number_of_internal_bus(col);

                cvalue = jacobian_delta_q_over_voltage.get_entry_value(k);
                rvalue = cvalue.real();

                /*file<<"Q,V,"<<row<<","<<row_bus<<","<<col<<","<<col_bus<<","
                    <<setprecision(6)<<fixed<<rvalue<<endl;*/
                snprintf(buffer, 1000, "Q,V,%d,%u,%d,%u,%.6f",row, row_bus, col, col_bus, rvalue);
                file<<buffer<<endl;
            }
            file.close();
        }
        else
        {
            osstream<<"File '"<<filename<<"' cannot be opened for saving jacobian matrix."<<endl
              <<"No jacobian matrix will be exported.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            return;
        }
    }
}

bool JACOBIAN_BUILDER::is_valid() const
{
    return true;
}

void JACOBIAN_BUILDER::check()
{
    ;
}

void JACOBIAN_BUILDER::clear()
{
    ;
}
