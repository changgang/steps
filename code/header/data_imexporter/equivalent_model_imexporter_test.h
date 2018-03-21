#ifndef EQUIVALENT_MODEL_IMEXPORTER_TEST_H
#define EQUIVALENT_MODEL_IMEXPORTER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/data_imexporter/equivalent_model_imexporter.h"

using namespace std;

class EQUIVALENT_MODEL_IMEXPORTER_TEST : public Test::Suite
{
    public:
        EQUIVALENT_MODEL_IMEXPORTER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_load_ieee9_arxl_data();
        void test_load_ieee39_arxl_data();
    private:
        POWER_SYSTEM_DATABASE* db;
        EQUIVALENT_MODEL_IMEXPORTER* importer;
};
#endif // EQUIVALENT_MODEL_IMEXPORTER_TEST_H
