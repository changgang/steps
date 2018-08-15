#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif
#include <cmath>
#include "thirdparty/cpptest/cpptest.h"

#include "header/data_imexporter/equivalent_model_imexporter_test.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"

#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;


EQUIVALENT_MODEL_IMEXPORTER_TEST::EQUIVALENT_MODEL_IMEXPORTER_TEST()
{
    TEST_ADD(EQUIVALENT_MODEL_IMEXPORTER_TEST::test_load_ieee9_arxl_data);
    TEST_ADD(EQUIVALENT_MODEL_IMEXPORTER_TEST::test_load_ieee39_arxl_data);
}

void EQUIVALENT_MODEL_IMEXPORTER_TEST::setup()
{
    db = new POWER_SYSTEM_DATABASE;
    db->set_allowed_max_bus_number(100000);

    importer = new EQUIVALENT_MODEL_IMEXPORTER;
    importer->set_power_system_database(db);
}

void EQUIVALENT_MODEL_IMEXPORTER_TEST::tear_down()
{
    delete importer;

    delete db;

    importer = NULL;
    db = NULL;

    show_test_end_information();
}

void EQUIVALENT_MODEL_IMEXPORTER_TEST::test_load_ieee9_arxl_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_MODEL_IMEXPORTER_TEST");

    ostringstream osstream;

    PSSE_IMEXPORTER psse_assember;
    psse_assember.set_power_system_database(db);
    psse_assember.load_powerflow_data("ieee9.raw");
    importer->load_equivalent_model("ieee9_arxl_model_demo.eqv");

    TEST_ASSERT(db->get_equivalent_device_count()==2);
    vector<EQUIVALENT_DEVICE*> edevices = db->get_all_equivalent_devices();
    size_t n = edevices.size();
    for(size_t i=0; i!=n; ++i)
    {
        EQUIVALENT_MODEL* model = edevices[i]->get_equivalent_model();
        osstream<<model->get_standard_model_string()<<endl;
    }
    show_information_with_leading_time_stamp(osstream);
}

void EQUIVALENT_MODEL_IMEXPORTER_TEST::test_load_ieee39_arxl_data()
{
    show_test_information_for_function_of_class(__FUNCTION__,"EQUIVALENT_MODEL_IMEXPORTER_TEST");

    ostringstream osstream;

    PSSE_IMEXPORTER psse_assember;
    psse_assember.set_power_system_database(db);
    psse_assember.load_powerflow_data("ieee39.raw");
    importer->load_equivalent_model("ieee39_arxl_model_demo.eqv");

    TEST_ASSERT(db->get_equivalent_device_count()==1);
    vector<EQUIVALENT_DEVICE*> edevices = db->get_all_equivalent_devices();
    size_t n = edevices.size();
    for(size_t i=0; i!=n; ++i)
    {
        EQUIVALENT_MODEL* model = edevices[i]->get_equivalent_model();
        osstream<<model->get_standard_model_string()<<endl;
    }
    show_information_with_leading_time_stamp(osstream);
}


