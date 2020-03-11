#include "header/basic/test_macro.h"
#include "header/model/wtg_models/wind_speed_model/filewind_test.h"
#include "header/model/wtg_models/wind_speed_model/filewind.h"
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

FILEWIND_TEST::FILEWIND_TEST() : WIND_SPEED_MODEL_TEST()
{
    TEST_ADD(FILEWIND_TEST::test_get_model_name);
    TEST_ADD(FILEWIND_TEST::test_set_get_parameters);
    TEST_ADD(FILEWIND_TEST::test_initialize);
    TEST_ADD(FILEWIND_TEST::test_get_wind_speed);
    TEST_ADD(FILEWIND_TEST::test_get_wind_direction);
}

void FILEWIND_TEST::setup()
{
    WIND_SPEED_MODEL_TEST::setup();

    WT_GENERATOR* wt_gen = get_test_wt_generator();

    string file = "filewind_demo.csv";
    prepare_wind_speed_file(file);
    FILEWIND model(default_toolkit);
    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    model.set_device_id(wt_gen->get_device_id());

    model.set_wind_speed_serial_file(file);
    model.load_wind_speed_from_file();
    dmdb.add_model(&model);
}

void FILEWIND_TEST::tear_down()
{
    WIND_SPEED_MODEL_TEST::tear_down();

    DYNAMIC_MODEL_DATABASE& dmdb = default_toolkit.get_dynamic_model_database();
    dmdb.remove_the_last_model();

    show_test_end_information();
}

void FILEWIND_TEST::prepare_wind_speed_file(string file)
{
    ostringstream osstream;
    ofstream fid(file);
    if(not fid.is_open())
    {
        osstream<<"File '"<<file<<"' cannot be opened for preparing wind speed file. Check FILEWIND_TEST::"<<__FUNCTION__<<"()";
        default_toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    fid<<"TIME,SPEED,DIRECTION"<<endl;
    fid<<"0.0, 1.0, 0.0"<<endl;
    fid<<"1.0, 1.1, 5.0"<<endl;
    fid<<"5.0, 0.7, 9.0"<<endl;
    fid.close();
}

void FILEWIND_TEST::test_get_model_name()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEWIND_TEST");

    WIND_SPEED_MODEL* model = get_test_wind_speed_model();
    if(model!=NULL)
    {
        TEST_ASSERT(model->get_model_name()=="FILEWIND");
    }
    else
        TEST_ASSERT(false);
}

void FILEWIND_TEST::test_set_get_parameters()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEWIND_TEST");

    FILEWIND* model = (FILEWIND*) get_test_wind_speed_model();

    model->set_wind_speed_serial_file("thisfile.csv");
    TEST_ASSERT(model->get_wind_speed_serial_file()=="thisfile.csv");
}

void FILEWIND_TEST::test_initialize()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEWIND_TEST");
    FILEWIND* model = (FILEWIND*) get_test_wind_speed_model();
    model->initialize();
}


void FILEWIND_TEST::test_get_wind_speed()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEWIND_TEST");
    FILEWIND* model = (FILEWIND*) get_test_wind_speed_model();

    model->initialize();

    double vn = model->get_nominal_wind_speed_in_mps();

    /*fid<<"0.0, 1.0, 0.0"<<endl;
    fid<<"1.0, 1.1, 5.0"<<endl;
    fid<<"5.0, 0.7, 9.0"<<endl;*/

    default_toolkit.set_dynamic_simulation_time_in_s(-0.02);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-1.0*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(0.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-1.0*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(0.5);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-1.05)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-1.05*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(0.7);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-1.07)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-1.07*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(1.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-1.1)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-1.1*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(2.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-1.0)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-1.0*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(3.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-0.9)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-0.9*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(4.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-0.8)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-0.8*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(4.9);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-0.71)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-0.71*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(5.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-0.7*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(6.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-0.7*vn)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(100.0);
    TEST_ASSERT(fabs(model->get_wind_speed_in_pu()-0.7)<FLOAT_EPSILON);
    TEST_ASSERT(fabs(model->get_wind_speed_in_mps()-0.7*vn)<FLOAT_EPSILON);
}

void FILEWIND_TEST::test_get_wind_direction()
{
    show_test_information_for_function_of_class(__FUNCTION__,"FILEWIND_TEST");
    FILEWIND* model = (FILEWIND*) get_test_wind_speed_model();

    model->initialize();

    /*fid<<"0.0, 1.0, 0.0"<<endl;
    fid<<"1.0, 1.1, 5.0"<<endl;
    fid<<"5.0, 0.7, 9.0"<<endl;*/

    default_toolkit.set_dynamic_simulation_time_in_s(-0.02);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-0.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(0.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-0.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(0.5);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-2.5)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(0.7);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-3.5)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(1.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-5.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(2.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-6.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(3.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-7.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(4.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-8.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(4.9);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-8.9)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(5.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-9.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(6.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-9.0)<FLOAT_EPSILON);

    default_toolkit.set_dynamic_simulation_time_in_s(100.0);
    TEST_ASSERT(fabs(model->get_wind_direction_in_deg()-9.0)<FLOAT_EPSILON);
}

#endif
