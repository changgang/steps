#include "header/network/jacobian_builder_test.h"
#include "header/basic/utility.h"
#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"

#include <cstdio>

using namespace std;


JACOBIAN_BUILDER_TEST::JACOBIAN_BUILDER_TEST()
{
    TEST_ADD(JACOBIAN_BUILDER_TEST::test_form_and_show_seprate_jacobians);
    TEST_ADD(JACOBIAN_BUILDER_TEST::test_update_seprate_jacobians);
    TEST_ADD(JACOBIAN_BUILDER_TEST::test_get_full_jacobian_for_coupled_P_and_Q_equations);
    TEST_ADD(JACOBIAN_BUILDER_TEST::test_get_decoupled_B_jacobian_for_P_equations);
    TEST_ADD(JACOBIAN_BUILDER_TEST::test_get_decoupled_B_jacobian_for_Q_equations);
}

void JACOBIAN_BUILDER_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    network_db = new NETWORK_DATABASE(db);

    jacobian_builder = new JACOBIAN_BUILDER;
    jacobian_builder->set_network_database(network_db);

    prepare_Arthur_R_Bergen_3_bus_model(db);
}

void JACOBIAN_BUILDER_TEST::tear_down()
{
    delete jacobian_builder;
    delete network_db;
    delete db;

    show_test_end_information();
}

void JACOBIAN_BUILDER_TEST::test_form_and_show_seprate_jacobians()
{
    show_test_information_for_function_of_class(__FUNCTION__,"JACOBIAN_BUILDER_TEST");

    network_db->build_network_matrix();

    jacobian_builder->build_seprate_jacobians();

    ostringstream osstream;
    osstream<<"Jacobian with initial voltage from Arthur R. Arthur_R_Bergen";
    show_information_with_leading_time_stamp(osstream);
    jacobian_builder->show_seprate_jacobians();
}

void JACOBIAN_BUILDER_TEST::test_update_seprate_jacobians()
{
    show_test_information_for_function_of_class(__FUNCTION__,"JACOBIAN_BUILDER_TEST");
    ostringstream osstream;

    network_db->build_network_matrix();

    jacobian_builder->build_seprate_jacobians();

    BUS* bus;
    bus = db->get_bus(2);
    bus->set_angle_in_deg(-2.9395);
    bus = db->get_bus(3);
    bus->set_voltage_in_pu(0.9638);
    bus->set_angle_in_deg(-9.51111);

    osstream<<"Jacobian with voltage of the first iteration from Arthur R. Arthur_R_Bergen";
    show_information_with_leading_time_stamp(osstream);
    jacobian_builder->update_seprate_jacobians();
    jacobian_builder->show_seprate_jacobians();
}

void JACOBIAN_BUILDER_TEST::test_get_full_jacobian_for_coupled_P_and_Q_equations()
{
    show_test_information_for_function_of_class(__FUNCTION__,"JACOBIAN_BUILDER_TEST");
    ostringstream osstream;

    network_db->build_network_matrix();

    jacobian_builder->build_seprate_jacobians();

    vector<size_t> internal_P_equation_buses, internal_Q_equation_buses;
    internal_P_equation_buses.clear();
    internal_Q_equation_buses.clear();
    internal_P_equation_buses.push_back(0);
    internal_P_equation_buses.push_back(1);
    internal_P_equation_buses.push_back(2);

    internal_Q_equation_buses = internal_P_equation_buses;

    SPARSE_MATRIX J = jacobian_builder->get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(internal_P_equation_buses, internal_Q_equation_buses);


    osstream<<"Full Jacobian with initial voltage from Arthur R. Arthur_R_Bergen";
    show_information_with_leading_time_stamp(osstream);

    J.report_brief();

    BUS* bus;
    bus = db->get_bus(2);
    bus->set_angle_in_deg(-2.9395);
    bus = db->get_bus(3);
    bus->set_voltage_in_pu(0.9638);
    bus->set_angle_in_deg(-9.51111);

    jacobian_builder->update_seprate_jacobians();

    J = jacobian_builder->get_full_coupled_jacobian_with_P_and_Q_equation_internal_buses(internal_P_equation_buses, internal_Q_equation_buses);

    osstream<<"Full Jacobian with voltage of the first iteration from Arthur R. Arthur_R_Bergen";
    show_information_with_leading_time_stamp(osstream);

    J.report_brief();
}



void JACOBIAN_BUILDER_TEST::test_get_decoupled_B_jacobian_for_P_equations()
{
    show_test_information_for_function_of_class(__FUNCTION__,"JACOBIAN_BUILDER_TEST");
    ostringstream osstream;

    network_db->build_decoupled_network_matrix();

    vector<size_t> internal_P_equation_buses;
    internal_P_equation_buses.clear();
    internal_P_equation_buses.push_back(0);
    internal_P_equation_buses.push_back(1);
    internal_P_equation_buses.push_back(2);

    SPARSE_MATRIX B = jacobian_builder->get_decoupled_B_jacobian_with_P_equation_internal_buses(internal_P_equation_buses);


    osstream<<"Decoupled B Jacobian for P equations from Arthur R. Arthur_R_Bergen";
    show_information_with_leading_time_stamp(osstream);

    B.report_brief();
}



void JACOBIAN_BUILDER_TEST::test_get_decoupled_B_jacobian_for_Q_equations()
{
    show_test_information_for_function_of_class(__FUNCTION__,"JACOBIAN_BUILDER_TEST");
    ostringstream osstream;

    network_db->build_decoupled_network_matrix();

    vector<size_t> internal_Q_equation_buses;
    internal_Q_equation_buses.clear();
    internal_Q_equation_buses.push_back(0);
    internal_Q_equation_buses.push_back(1);
    internal_Q_equation_buses.push_back(2);

    SPARSE_MATRIX B = jacobian_builder->get_decoupled_B_jacobian_with_Q_equation_internal_buses(internal_Q_equation_buses);


    osstream<<"Decoupled B Jacobian for Q equations from Arthur R. Arthur_R_Bergen";
    show_information_with_leading_time_stamp(osstream);

    B.report_brief();
}

