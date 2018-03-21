#ifndef NETWORK_OPTIMIZER_TEST_H
#define NETWORK_OPTIMIZER_TEST_H

#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest-1.1.2/src/cpptest.h"

#include "header/network/network_optimizer.h"

using namespace std;

class NETWORK_OPTIMIZER_TEST : public Test::Suite
{
    public:
        NETWORK_OPTIMIZER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_optimize_network_ordering();
        void test_check_network_connectivity();
    private:
};

#endif
