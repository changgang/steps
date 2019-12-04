#ifndef SHORT_CIRCUIT_SOLVER_TEST_H
#define SHORT_CIRCUIT_SOLVER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/toolkit/short_circuit_solver/short_circuit_solver.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
using namespace std;

class SHORT_CIRCUIT_SOLVER_TEST : public Test::Suite
{
    public:
        SHORT_CIRCUIT_SOLVER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constructor();

    private:
};

#endif
