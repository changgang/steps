#include "header/model/load_model/load_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

using namespace std;

LOAD_MODEL_TEST::LOAD_MODEL_TEST()
{
    TEST_ADD(LOAD_MODEL_TEST::test_get_model_type);
    TEST_ADD(LOAD_MODEL_TEST::test_get_model_name);
    TEST_ADD(LOAD_MODEL_TEST::test_get_detailed_model_name);
    TEST_ADD(LOAD_MODEL_TEST::test_get_bus_voltage);
    TEST_ADD(LOAD_MODEL_TEST::test_get_bus_frequency_deviation);
    TEST_ADD(LOAD_MODEL_TEST::test_set_get_subsystem_type);
    TEST_ADD(LOAD_MODEL_TEST::test_initialize);
    TEST_ADD(LOAD_MODEL_TEST::test_run_voltage_ramp_response);
    TEST_ADD(LOAD_MODEL_TEST::test_run_frequency_ramp_response);
}



void LOAD_MODEL_TEST::setup()
{
    db = get_default_power_system_database_pointer();
    db->set_allowed_max_bus_number(100);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus;
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(21.0);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_rad(0.0);

    db->append_bus(bus);

    LOAD load(db);
    load.set_load_bus(1);
    load.set_identifier("#1");
    load.set_status(true);
    load.set_nominal_constant_power_load_in_MVA(complex<double>(100.0, 50.0));
    load.set_nominal_constant_current_load_in_MVA(complex<double>(100.0, 50.0));
    load.set_nominal_constant_impedance_load_in_MVA(complex<double>(100.0, 50.0));

    db->append_load(load);

    DEVICE_ID did;
    did.set_device_type("LOAD");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    load_ptr = db->get_load(did);
}

void LOAD_MODEL_TEST::tear_down()
{
    db->clear_database();

    show_test_end_information();
}

LOAD* LOAD_MODEL_TEST::get_load()
{
    return load_ptr;
}

void LOAD_MODEL_TEST::test_get_model_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_MODEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    TEST_ASSERT(model->get_model_type()=="LOAD CHARACTERISTICS");
}

void LOAD_MODEL_TEST::test_get_bus_voltage()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_MODEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    TEST_ASSERT(fabs(model->get_bus_voltage_in_pu() - db->get_bus_voltage_in_pu(1))<FLOAT_EPSILON);
}

void LOAD_MODEL_TEST::test_get_bus_frequency_deviation()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_MODEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    TEST_ASSERT(fabs(model->get_bus_frequency_deviation_in_pu() - db->get_bus_frequency_deviation_in_pu(1))<FLOAT_EPSILON);
}

void LOAD_MODEL_TEST::test_set_get_subsystem_type()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_MODEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    model->set_subsystem_type(BUS_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_subsystem_type()==BUS_SUBSYSTEM_TYPE);

    model->set_subsystem_type(AREA_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_subsystem_type()==AREA_SUBSYSTEM_TYPE);

    model->set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_subsystem_type()==ZONE_SUBSYSTEM_TYPE);

    model->set_subsystem_type(OWNER_SUBSYSTEM_TYPE);
    TEST_ASSERT(model->get_subsystem_type()==OWNER_SUBSYSTEM_TYPE);

    model->set_subsystem_type(ALL_SYSTEM_TYPE);
    TEST_ASSERT(model->get_subsystem_type()==ALL_SYSTEM_TYPE);
}


void LOAD_MODEL_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"LOAD_MODEL_TEST");

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    model->initialize();
}


void LOAD_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tVOLT_PU\tFREQ_PU\tP_MW\tQ_MVAR"<<endl;
    show_information_with_leading_time_stamp(osstream);
}

void LOAD_MODEL_TEST::export_meter_values(double time)
{
    ostringstream osstream;

    LOAD* load = get_load();
    LOAD_MODEL* model = load->get_load_model();

    double volt = model->get_bus_voltage_in_pu();
    double freq = model->get_bus_frequency_deviation_in_pu();

    complex<double> S = model->get_load_power_in_MVA();

    osstream<<setprecision(6)<<fixed<<time<<"\t"
      <<setprecision(6)<<fixed<<volt<<"\t"
      <<setprecision(6)<<fixed<<freq<<"\t"
      <<setprecision(6)<<fixed<<S.real()<<"\t"
      <<setprecision(6)<<fixed<<S.imag()<<endl;
    show_information_with_leading_time_stamp(osstream);
}

void LOAD_MODEL_TEST::test_run_voltage_ramp_response()
{
    LOAD* load = get_load();
    POWER_SYSTEM_DATABASE* psdb = load->get_power_system_database();

    LOAD_MODEL* model = load->get_load_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    redirect_stdout_to_file("test_log/test_run_voltage_ramp_response_"+model->get_model_name()+".txt");

    double delt = get_dynamic_simulation_time_step_in_s();
    double TIME = -2.0*delt;
    double P;

    model->initialize();

    export_meter_title();
    export_meter_values(TIME);
    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        P =  model->get_load_power_in_MVA().real();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(P-model->get_load_power_in_MVA().real())>1e-6)
                P =  model->get_load_power_in_MVA().real();
            else
                break;
        }
        model->run(UPDATE_MODE);
        export_meter_values(TIME);
    }

    BUS* busptr = psdb->get_bus(1);
    double rate = 0.1;

    while(true)
    {
        TIME += delt;
        if(TIME>6.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        busptr->set_voltage_in_pu(busptr->get_voltage_in_pu()+rate*delt);
        P =  model->get_load_power_in_MVA().real();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(P - model->get_load_power_in_MVA().real())>1e-6)
                P =  model->get_load_power_in_MVA().real();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;
        if(TIME>10.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        busptr->set_voltage_in_pu(busptr->get_voltage_in_pu()-rate*delt);
        P =  model->get_load_power_in_MVA().real();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(P - model->get_load_power_in_MVA().real())>1e-6)
                P =  model->get_load_power_in_MVA().real();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    recover_stdout();
}

void LOAD_MODEL_TEST::test_run_frequency_ramp_response()
{
    LOAD* load = get_load();
    POWER_SYSTEM_DATABASE* psdb = load->get_power_system_database();

    LOAD_MODEL* model = load->get_load_model();
    show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

    redirect_stdout_to_file("test_log/test_run_frequency_ramp_response_"+model->get_model_name()+".txt");

    double delt = get_dynamic_simulation_time_step_in_s();
    double TIME = -2.0*delt;
    double P;

    model->initialize();

    export_meter_title();
    export_meter_values(TIME);
    while(true)
    {
        TIME += delt;
        if(TIME>1.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        P =  model->get_load_power_in_MVA().real();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(P-model->get_load_power_in_MVA().real())>1e-6)
                P =  model->get_load_power_in_MVA().real();
            else
                break;
        }
        model->run(UPDATE_MODE);
        export_meter_values(TIME);
    }

    BUS* busptr = psdb->get_bus(1);
    BUS_FREQUENCY_MODEL* fmodel = busptr->get_bus_frequency_model();

    double rate = 0.1;

    while(true)
    {
        TIME += delt;
        if(TIME>6.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        fmodel->set_frequency_deviation_in_pu(fmodel->get_frequency_deviation_in_pu()+rate*delt);
        P =  model->get_load_power_in_MVA().real();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(P - model->get_load_power_in_MVA().real())>1e-6)
                P =  model->get_load_power_in_MVA().real();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    while(true)
    {
        TIME += delt;
        if(TIME>10.0+FLOAT_EPSILON)
        {
            TIME -=delt;
            break;
        }
        fmodel->set_frequency_deviation_in_pu(fmodel->get_frequency_deviation_in_pu()-rate*delt);
        P =  model->get_load_power_in_MVA().real();
        while(true)
        {
            model->run(INTEGRATE_MODE);
            if(fabs(P - model->get_load_power_in_MVA().real())>1e-6)
                P =  model->get_load_power_in_MVA().real();
            else
                break;
        }
        model->run(UPDATE_MODE);

        export_meter_values(TIME);
    }

    recover_stdout();
}




