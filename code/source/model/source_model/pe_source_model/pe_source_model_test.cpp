#include "header/model/source_model/pe_source_model/pe_source_model_test.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

PE_SOURCE_MODEL_TEST::PE_SOURCE_MODEL_TEST()
{
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_get_model_type);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_get_model_name);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_set_get_parameters);

    TEST_ADD(PE_SOURCE_MODEL_TEST::test_set_get_initial_active_current_command);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_set_get_initial_reactive_current_command);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_set_get_initial_reactive_voltage_command);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_active_current_step_response_of_pe_source_model);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_reactive_current_step_response_of_pe_source_model);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_reactive_voltage_step_response_of_pe_source_model);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_bus_magnitude_step_response_of_pe_source_model);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_bus_angle_step_response_of_pe_source_model);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_variable_step_simulation_with_active_current_step_response);
    TEST_ADD(PE_SOURCE_MODEL_TEST::test_get_standard_model_string);
}



void PE_SOURCE_MODEL_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_allowed_max_bus_number(100);
    db->set_system_base_frequency_in_Hz(50.0);
    db->set_system_base_power_in_MVA(100.0);

    BUS bus(db);
    bus.set_bus_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_base_voltage_in_kV(0.69);
    bus.set_voltage_in_pu(1.0);
    bus.set_angle_in_deg(30.0);

    db->append_bus(bus);

    PE_SOURCE pesource(db);
    pesource.set_source_bus(1);
    pesource.set_identifier("#1");
    pesource.set_status(true);
    pesource.set_mbase_in_MVA(100.0);
    pesource.set_source_impedance_in_pu(complex<double>(0.0, 0.1));
    pesource.set_p_generation_in_MW(70.0);
    pesource.set_q_generation_in_MVar(30.0);

    db->append_pe_source(pesource);

    DEVICE_ID did;
    did.set_device_type("PE SOURCE");
    TERMINAL terminal;
    terminal.append_bus(1);
    did.set_device_terminal(terminal);
    did.set_device_identifier("#1");

    pesource_ptr = db->get_pe_source(did);
}

void PE_SOURCE_MODEL_TEST::tear_down()
{
    delete db;
}

PE_SOURCE* PE_SOURCE_MODEL_TEST::get_pe_source()
{
    return pesource_ptr;
}

void PE_SOURCE_MODEL_TEST::test_get_model_type()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        TEST_ASSERT(model->get_model_type()=="PE SOURCE");
    }
    else
        TEST_ASSERT(false);
}


void PE_SOURCE_MODEL_TEST::test_update_source_impedance()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        model->update_source_impedance();
        TEST_ASSERT(abs(model->get_source_impedance_in_pu_based_on_mbase()-pesource_ptr->get_source_impedance_in_pu())<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_set_get_initial_active_current_command()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_active_current_command_in_pu_based_on_mbase(1.0);
        TEST_ASSERT(fabs(model->get_initial_active_current_command_in_pu_based_on_mbase()-1.0)<FLOAT_EPSILON);
        model->set_initial_active_current_command_in_pu_based_on_mbase(2.0);
        TEST_ASSERT(fabs(model->get_initial_active_current_command_in_pu_based_on_mbase()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_set_get_initial_reactive_current_command()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_reactive_current_command_in_pu_based_on_mbase(1.0);
        TEST_ASSERT(fabs(model->get_initial_reactive_current_command_in_pu_based_on_mbase()-1.0)<FLOAT_EPSILON);
        model->set_initial_reactive_current_command_in_pu_based_on_mbase(2.0);
        TEST_ASSERT(fabs(model->get_initial_reactive_current_command_in_pu_based_on_mbase()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_set_get_initial_reactive_voltage_command()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_reactive_voltage_command_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_initial_reactive_voltage_command_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_initial_reactive_voltage_command_in_pu(2.0);
        TEST_ASSERT(fabs(model->get_initial_reactive_voltage_command_in_pu()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::run_a_step()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    double pelec = model->get_terminal_active_power_in_MW();
    while(true)
    {
        model->run(INTEGRATE_MODE);
        if(fabs(pelec-model->get_terminal_active_power_in_MW())>1e-6)
            pelec = model->get_terminal_active_power_in_MW();
        else
            break;
    }
    model->run(UPDATE_MODE);
}

void PE_SOURCE_MODEL_TEST::show_simulation_title()
{
    ostringstream sstream;
    sstream<<"TIME\tVTERM\tBUSANGLE\tPLL_FREQ\tPLL_ANGLE\tIPCMD\tIQCMD\tEQCMD\tPELEC\tQELEC";
    sstream<<sstream.str()<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void PE_SOURCE_MODEL_TEST::show_simulation_result(double time)
{
    ostringstream sstream;

    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();

    complex<double> Vxy = model->get_terminal_complex_voltage_in_pu();
    double vterm = abs(Vxy);
    double busangle = rad2deg(atan2(Vxy.imag(), Vxy.real()));
    double freq = model->get_pll_frequency_deviation_in_pu();
    double angle = model->get_pll_angle_in_deg();
    double ipcmd = model->get_active_current_command_in_pu_based_on_mbase();
    double iqcmd = model->get_reactive_current_command_in_pu_based_on_mbase();
    double eqcmd = model->get_reactive_voltage_command_in_pu();
    double pelec = model->get_terminal_active_power_in_MW();
    double qelec = model->get_terminal_reactive_power_in_MVar();

    sstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<vterm<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<busangle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<freq<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<angle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<ipcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<iqcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<eqcmd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pelec<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<qelec;

    sstream<<sstream.str()<<endl;
    show_information_with_leading_time_stamp(sstream);
}

void PE_SOURCE_MODEL_TEST::test_active_current_step_response_of_pe_source_model()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        show_simulation_title();
        show_simulation_result(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            show_simulation_result(time);
        }

        double ipcmd = model->get_initial_active_current_command_in_pu_based_on_mbase();
        model->set_initial_active_current_command_in_pu_based_on_mbase(ipcmd*0.99);
        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_reactive_current_step_response_of_pe_source_model()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        show_simulation_title();
        show_simulation_result(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            show_simulation_result(time);
        }

        double iqcmd = model->get_initial_reactive_current_command_in_pu_based_on_mbase();
        model->set_initial_reactive_current_command_in_pu_based_on_mbase(iqcmd*0.99);
        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_reactive_voltage_step_response_of_pe_source_model()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        show_simulation_title();
        show_simulation_result(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            show_simulation_result(time);
        }

        double eqcmd = model->get_initial_reactive_voltage_command_in_pu();
        model->set_initial_reactive_voltage_command_in_pu(eqcmd*0.99);
        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_bus_magnitude_step_response_of_pe_source_model()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        show_simulation_title();
        show_simulation_result(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            show_simulation_result(time);
        }

        BUS* bus = db->get_bus(1);
        double vterm = bus->get_voltage_in_pu();
        bus->set_voltage_in_pu(vterm*0.99);

        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_bus_angle_step_response_of_pe_source_model()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        show_simulation_title();
        show_simulation_result(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            show_simulation_result(time);
        }

        BUS* bus = db->get_bus(1);
        double angle = bus->get_angle_in_deg();
        bus->set_angle_in_deg(angle+10.0);

        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }
        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}


void PE_SOURCE_MODEL_TEST::test_variable_step_simulation_with_active_current_step_response()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    ostringstream sstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        redirect_stdout_to_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        sstream<<"Model:"<<model->get_standard_model_string()<<endl;
        show_information_with_leading_time_stamp(sstream);

        double delt = 0.001;
        set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->initialize();

        show_simulation_title();
        show_simulation_result(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            show_simulation_result(time);
        }

        double ipcmd = model->get_initial_active_current_command_in_pu_based_on_mbase();
        model->set_initial_active_current_command_in_pu_based_on_mbase(ipcmd*0.9);
        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }

        delt *= 2.0;
        set_dynamic_simulation_time_step_in_s(delt);
        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>11.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }

        delt *= 2.0;
        set_dynamic_simulation_time_step_in_s(delt);
        model->run(UPDATE_MODE);

        show_simulation_result(time);

        while(true)
        {
            time += delt;

            if(time>16.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            show_simulation_result(time);
        }

        recover_stdout();
    }
    else
        TEST_ASSERT(false);
}

void PE_SOURCE_MODEL_TEST::test_get_standard_model_string()
{
    PE_SOURCE_MODEL* model = pesource_ptr->get_pe_source_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        //model->get_standard_model_string();
    }
    else
        TEST_ASSERT(false);
}
