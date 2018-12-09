#ifndef LOAD_RELAY_MODEL_TEST_H
#define LOAD_RELAY_MODEL_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"
#include "header/model/load_relay_model/load_relay_model.h"
#include "header/power_system_database.h"
using namespace std;

class LOAD_RELAY_MODEL_TEST : public Test::Suite
{
    public:
        LOAD_RELAY_MODEL_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
        LOAD* get_load();
    private:
        LOAD* loadptr;
};

#endif//LOAD_RELAY_MODEL_TEST_H
