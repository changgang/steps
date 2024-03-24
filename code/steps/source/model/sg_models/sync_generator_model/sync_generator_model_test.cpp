#include "header/basic/test_macro.h"
#include "header/model/sg_models/sync_generator_model/sync_generator_model_test.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>
#include <cmath>

#ifdef ENABLE_STEPS_TEST
using namespace std;

SYNC_GENERATOR_MODEL_TEST::SYNC_GENERATOR_MODEL_TEST()
{
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_get_model_type);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_set_get_parameters);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_is_saturation_considered);

    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_set_get_initial_excitation_voltage);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_set_get_initial_mechanical_power);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_pmech_step_response_of_sync_generator_model);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_efd_step_response_of_sync_generator_model);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_bus_step_response_of_sync_generator_model);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_variable_step_simulation_with_pmech_step_response);
    TEST_ADD(SYNC_GENERATOR_MODEL_TEST::test_get_standard_psse_string);
}



void SYNC_GENERATOR_MODEL_TEST::setup()
{
    SG_MODEL_TEST::setup();
}

void SYNC_GENERATOR_MODEL_TEST::tear_down()
{
    SG_MODEL_TEST::tear_down();
}

void SYNC_GENERATOR_MODEL_TEST::test_get_model_type()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        TEST_ASSERT(model->get_model_type()=="SYNC GENERATOR");
    }
    else
        TEST_ASSERT(false);
}
void SYNC_GENERATOR_MODEL_TEST::test_set_get_parameters()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        model->set_Rs(0.1);
        TEST_ASSERT(fabs(model->get_Rs()-0.1)<FLOAT_EPSILON);

        model->set_Xd(0.01);
        TEST_ASSERT(fabs(model->get_Xd()-0.01)<FLOAT_EPSILON);

        model->set_Xq(0.01);
        TEST_ASSERT(fabs(model->get_Xq()-0.01)<FLOAT_EPSILON);

        model->set_Xdp(0.01);
        TEST_ASSERT(fabs(model->get_Xdp()-0.01)<FLOAT_EPSILON);

        model->set_Xqp(0.01);
        TEST_ASSERT(fabs(model->get_Xqp()-0.01)<FLOAT_EPSILON);

        model->set_Xdpp(0.01);
        TEST_ASSERT(fabs(model->get_Xdpp()-0.01)<FLOAT_EPSILON);

        model->set_Xqpp(0.01);
        TEST_ASSERT(fabs(model->get_Xqpp()-0.01)<FLOAT_EPSILON);

        model->set_Td0p_in_s(0.01);
        TEST_ASSERT(fabs(model->get_Td0p_in_s()-0.01)<FLOAT_EPSILON);

        model->set_Tq0p_in_s(0.01);
        TEST_ASSERT(fabs(model->get_Tq0p_in_s()-0.01)<FLOAT_EPSILON);

        model->set_Td0pp_in_s(0.01);
        TEST_ASSERT(fabs(model->get_Td0pp_in_s()-0.01)<FLOAT_EPSILON);

        model->set_Tq0pp_in_s(0.01);
        TEST_ASSERT(fabs(model->get_Tq0pp_in_s()-0.01)<FLOAT_EPSILON);

        model->set_H_in_s(3.0);
        TEST_ASSERT(fabs(model->get_H_in_s()-3.0)<FLOAT_EPSILON);
        TEST_ASSERT(fabs(model->get_Tj_in_s()-6.0)<FLOAT_EPSILON);

        model->set_Tj_in_s(8.0);
        TEST_ASSERT(fabs(model->get_Tj_in_s()-8.0)<FLOAT_EPSILON);
        TEST_ASSERT(fabs(model->get_H_in_s()-4.0)<FLOAT_EPSILON);

        model->set_D(2.0);
        TEST_ASSERT(fabs(model->get_D()-2.0)<FLOAT_EPSILON);

        model->set_saturation_at_1(0.5);
        TEST_ASSERT(fabs(model->get_saturation_at_1()-0.5)<FLOAT_EPSILON);

        model->set_saturation_at_1p2(1.5);
        TEST_ASSERT(fabs(model->get_saturation_at_1p2()-1.5)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::test_is_saturation_considered()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");
        model->set_saturation_at_1(0.0);
        model->set_saturation_at_1p2(0.0);
        TEST_ASSERT(model->is_saturation_considered()==false);

        model->set_saturation_at_1(0.0);
        model->set_saturation_at_1p2(0.1);
        TEST_ASSERT(model->is_saturation_considered()==true);

        model->set_saturation_at_1(0.1);
        model->set_saturation_at_1p2(0.0);
        TEST_ASSERT(model->is_saturation_considered()==true);

        model->set_saturation_at_1(0.1);
        model->set_saturation_at_1p2(0.1);
        TEST_ASSERT(model->is_saturation_considered()==true);
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::test_set_get_initial_excitation_voltage()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_excitation_voltage_in_pu(1.0);
        TEST_ASSERT(fabs(model->get_initial_excitation_voltage_in_pu()-1.0)<FLOAT_EPSILON);
        model->set_initial_excitation_voltage_in_pu(2.0);
        TEST_ASSERT(fabs(model->get_initial_excitation_voltage_in_pu()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::test_set_get_initial_mechanical_power()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        model->set_initial_mechanical_power_in_pu_based_on_mbase(1.0);
        TEST_ASSERT(fabs(model->get_initial_mechanical_power_in_pu_based_on_mbase()-1.0)<FLOAT_EPSILON);
        model->set_initial_mechanical_power_in_pu_based_on_mbase(2.0);
        TEST_ASSERT(fabs(model->get_initial_mechanical_power_in_pu_based_on_mbase()-2.0)<FLOAT_EPSILON);
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::run_a_step()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    double speed = model->get_rotor_speed_deviation_in_pu();
    while(true)
    {
        model->run(DYNAMIC_INTEGRATE_MODE);
        if(fabs(speed-model->get_rotor_speed_deviation_in_pu())>1e-6)
            speed = model->get_rotor_speed_deviation_in_pu();
        else
            break;
    }
    model->run(DYNAMIC_UPDATE_MODE);
}


void SYNC_GENERATOR_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tANGLE\tSPEED\tPMECH\tEFD\tVTERM\tPELEC\tQELEC";
    osstream<<osstream.str()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void SYNC_GENERATOR_MODEL_TEST::export_meter_values(double time)
{
    ostringstream osstream;

    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();

    double angle = model->get_rotor_angle_in_deg();
    double speed = model->get_rotor_speed_deviation_in_pu();
    double pmech = model->get_mechanical_power_in_pu_based_on_mbase();
    double efd = model->get_excitation_voltage_in_pu();
    double pelec = model->get_terminal_active_power_in_pu_based_on_mbase();
    double qelec = model->get_terminal_reactive_power_in_pu_based_on_mbase();
    double vterm = steps_fast_complex_abs(model->get_terminal_complex_voltage_in_pu());

    osstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<angle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<speed<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pmech<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<efd<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<vterm<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<pelec<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<qelec;

    osstream<<osstream.str()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);

}

void SYNC_GENERATOR_MODEL_TEST::test_pmech_step_response_of_sync_generator_model()
{
    ostringstream osstream;

    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double TIME = -delt*2.0;

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
            run_a_step();
            export_meter_values(TIME);
        }

        double pmech = model->get_initial_mechanical_power_in_pu_based_on_mbase();
        model->set_initial_mechanical_power_in_pu_based_on_mbase(pmech*0.99);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values(TIME);

        while(true)
        {
            TIME += delt;

            if(TIME>6.0+FLOAT_EPSILON)
            {
                TIME -=delt;
                break;
            }

            run_a_step();
            export_meter_values(TIME);
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::test_efd_step_response_of_sync_generator_model()
{
    ostringstream osstream;

    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double TIME = -delt*2.0;

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
            run_a_step();
            export_meter_values(TIME);
        }

        double efd = model->get_initial_excitation_voltage_in_pu();
        model->set_initial_excitation_voltage_in_pu(efd*0.99);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values(TIME);

        while(true)
        {
            TIME += delt;

            if(TIME>6.0+FLOAT_EPSILON)
            {
                TIME -=delt;
                break;
            }

            run_a_step();
            export_meter_values(TIME);
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::test_bus_step_response_of_sync_generator_model()
{
    ostringstream osstream;

    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double TIME = -delt*2.0;

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
            run_a_step();
            export_meter_values(TIME);
        }

        BUS* bus = psdb.get_bus(1);
        double vterm = bus->get_positive_sequence_voltage_in_pu();
        bus->set_positive_sequence_voltage_in_pu(vterm*0.99);

        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values(TIME);

        while(true)
        {
            TIME += delt;

            if(TIME>6.0+FLOAT_EPSILON)
            {
                TIME -=delt;
                break;
            }

            run_a_step();
            export_meter_values(TIME);
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}


void SYNC_GENERATOR_MODEL_TEST::test_variable_step_simulation_with_pmech_step_response()
{
    ostringstream osstream;

    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double TIME = -delt*2.0;

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
            run_a_step();
            export_meter_values(TIME);
        }

        double pmech = model->get_initial_mechanical_power_in_pu_based_on_mbase();
        model->set_initial_mechanical_power_in_pu_based_on_mbase(pmech*0.9);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values(TIME);

        while(true)
        {
            TIME += delt;

            if(TIME>6.0+FLOAT_EPSILON)
            {
                TIME -=delt;
                break;
            }

            run_a_step();
            export_meter_values(TIME);
        }

        delt *= 2.0;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values(TIME);

        while(true)
        {
            TIME += delt;

            if(TIME>11.0+FLOAT_EPSILON)
            {
                TIME -=delt;
                break;
            }

            run_a_step();
            export_meter_values(TIME);
        }

        delt *= 2.0;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);
        model->run(DYNAMIC_UPDATE_MODE);

        export_meter_values(TIME);

        while(true)
        {
            TIME += delt;

            if(TIME>16.0+FLOAT_EPSILON)
            {
                TIME -=delt;
                break;
            }

            run_a_step();
            export_meter_values(TIME);
        }

        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void SYNC_GENERATOR_MODEL_TEST::test_get_standard_psse_string()
{
    SYNC_GENERATOR_MODEL* model = get_test_sync_generator_model();
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        //model->get_standard_psse_string(export_internal_bus_number);
    }
    else
        TEST_ASSERT(false);
}

#endif
