#include "header/basic/test_macro.h"
#include "header/model/converter_common_models/converter_model/vs_converter_model/vsg_model/vsg_model_test.h"
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

VSG_MODEL_TEST::VSG_MODEL_TEST()
{
    TEST_ADD(VSG_MODEL_TEST::test_set_get_Pref_in_pu_based_on_mbase);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_Qref_in_pu_based_on_mbase);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_Vref_in_pu);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_P_in_pu_based_on_mbase);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_Q_in_pu_based_on_mbase);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_V_in_pu);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_initial_virtual_angle_in_rad);
    TEST_ADD(VSG_MODEL_TEST::test_set_get_initial_virtual_voltage_in_pu);

    TEST_ADD(VSG_MODEL_TEST::test_active_power_step_response_of_vsg_model);
    TEST_ADD(VSG_MODEL_TEST::test_reactive_power_step_response_of_vsg_model);
    TEST_ADD(VSG_MODEL_TEST::test_voltage_step_response_of_vsg_model);
}

void VSG_MODEL_TEST::setup()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100);
    psdb.set_system_base_power_in_MVA(100.0);

    BUS bus(default_toolkit);
    bus.set_bus_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_base_voltage_in_kV(100.0);
    bus.set_base_frequency_in_Hz(50.0);

    psdb.append_bus(bus);
}

void VSG_MODEL_TEST::tear_down()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.clear();
}

VSG_MODEL2* VSG_MODEL_TEST::get_test_model()
{
    return model;
}

void VSG_MODEL_TEST::set_test_model(VSG_MODEL2* model)
{
    this->model = model;
}

void VSG_MODEL_TEST::test_set_get_Pref_in_pu_based_on_mbase()
{
    model->set_Pref_in_pu_based_on_mbase(100.0);
    TEST_ASSERT(fabs(model->get_Pref_in_pu_based_on_mbase()-100.0)<FLOAT_EPSILON);
    model->set_Pref_in_pu_based_on_mbase(200.0);
    TEST_ASSERT(fabs(model->get_Pref_in_pu_based_on_mbase()-200.0)<FLOAT_EPSILON);
}

void VSG_MODEL_TEST::test_set_get_Qref_in_pu_based_on_mbase()
{
    model->set_Qref_in_pu_based_on_mbase(100.0);
    TEST_ASSERT(fabs(model->get_Qref_in_pu_based_on_mbase()-100.0)<FLOAT_EPSILON);
    model->set_Qref_in_pu_based_on_mbase(200.0);
    TEST_ASSERT(fabs(model->get_Qref_in_pu_based_on_mbase()-200.0)<FLOAT_EPSILON);
}

void VSG_MODEL_TEST::test_set_get_Vref_in_pu()
{

}

void VSG_MODEL_TEST::test_set_get_P_in_pu_based_on_mbase()
{

}
void VSG_MODEL_TEST::test_set_get_Q_in_pu_based_on_mbase()
{

}
void VSG_MODEL_TEST::test_set_get_V_in_pu()
{

}
void VSG_MODEL_TEST::test_set_get_initial_virtual_voltage_in_pu()
{

}
void VSG_MODEL_TEST::test_set_get_initial_virtual_angle_in_rad()
{

}

void VSG_MODEL_TEST::test_active_power_step_response_of_vsg_model()
{
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->set_Pref_in_pu_based_on_mbase(1.0);
        model->set_Qref_in_pu_based_on_mbase(0.5);
        model->set_Vref_in_pu(1.05);
        model->set_P_in_pu_based_on_mbase(1.0);
        model->set_Q_in_pu_based_on_mbase(0.5);
        model->set_V_in_pu(1.05);

        model->set_initial_virtual_voltage_in_pu(1.0);
        model->set_initial_virtual_angle_in_rad(0.2);

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        double p0 = model->get_P_in_pu_based_on_mbase();
        model->set_P_in_pu_based_on_mbase(p0*0.99);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void VSG_MODEL_TEST::test_reactive_power_step_response_of_vsg_model()
{
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->set_Pref_in_pu_based_on_mbase(1.0);
        model->set_Qref_in_pu_based_on_mbase(0.5);
        model->set_Vref_in_pu(1.05);
        model->set_P_in_pu_based_on_mbase(1.0);
        model->set_Q_in_pu_based_on_mbase(0.5);
        model->set_V_in_pu(1.05);

        model->set_initial_virtual_voltage_in_pu(1.0);
        model->set_initial_virtual_angle_in_rad(0.2);

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        double q0 = model->get_Q_in_pu_based_on_mbase();
        model->set_Q_in_pu_based_on_mbase(q0*0.99);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);

}
void VSG_MODEL_TEST::test_voltage_step_response_of_vsg_model()
{
    ostringstream osstream;
    if(model!=NULL)
    {
        show_test_information_for_function_of_class(__FUNCTION__,model->get_model_name()+"_TEST");

        default_toolkit.open_log_file("test_log/"+model->get_model_name()+"_"+__FUNCTION__+".txt");

        osstream<<"Model:"<<model->get_standard_psse_string()<<endl;
        default_toolkit.show_information_with_leading_time_stamp(osstream);

        double delt = 0.001;
        default_toolkit.set_dynamic_simulation_time_step_in_s(delt);

        double time = -delt*2.0;

        model->set_Pref_in_pu_based_on_mbase(1.0);
        model->set_Qref_in_pu_based_on_mbase(0.5);
        model->set_Vref_in_pu(1.05);
        model->set_P_in_pu_based_on_mbase(1.0);
        model->set_Q_in_pu_based_on_mbase(0.5);
        model->set_V_in_pu(1.05);

        model->set_initial_virtual_voltage_in_pu(1.0);
        model->set_initial_virtual_angle_in_rad(0.2);

        model->initialize();

        export_meter_title();
        export_meter_values(time);

        while(true)
        {
            time += delt;
            if(time>1.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }
            run_a_step();
            export_meter_values(time);
        }

        double V0 = model->get_V_in_pu();
        model->set_V_in_pu(V0*0.99);
        model->run(UPDATE_MODE);

        export_meter_values(time);

        while(true)
        {
            time += delt;

            if(time>6.0+FLOAT_EPSILON)
            {
                time -=delt;
                break;
            }

            run_a_step();
            export_meter_values(time);
        }
        default_toolkit.close_log_file();
    }
    else
        TEST_ASSERT(false);
}

void VSG_MODEL_TEST::run_a_step()
{
    double angle = model->get_virtual_angle_in_rad();
    double V = model->get_virtual_voltage_in_pu();
    while(true)
    {
        model->run(INTEGRATE_MODE);
        if(fabs(angle-model->get_virtual_angle_in_rad())>FLOAT_EPSILON or
           fabs(V-model->get_virtual_voltage_in_pu())>FLOAT_EPSILON)
        {
            angle = model->get_virtual_angle_in_rad();
            V = model->get_virtual_voltage_in_pu();
        }
        else
            break;
    }
    model->run(UPDATE_MODE);
}

void VSG_MODEL_TEST::export_meter_title()
{
    ostringstream osstream;
    osstream<<"TIME\tP\tQ\tV\tANGLE\tE";
    default_toolkit.show_information_with_leading_time_stamp(osstream);

}
void VSG_MODEL_TEST::export_meter_values(double time)
{
    ostringstream osstream;

    double P = model->get_P_in_pu_based_on_mbase();
    double Q = model->get_Q_in_pu_based_on_mbase();
    double V = model->get_V_in_pu();
    double angle = model->get_virtual_angle_in_rad();
    double E = model->get_virtual_voltage_in_pu();

    osstream<<setw(6)<<setprecision(3)<<fixed<<time<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<P<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<Q<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<V<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<angle<<"\t"
      <<setw(10)<<setprecision(6)<<fixed<<E;

    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

#endif
