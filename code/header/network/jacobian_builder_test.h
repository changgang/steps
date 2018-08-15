#ifndef JACOBIAN_BUILDER_TEST_H
#define JACOBIAN_BUILDER_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/network/jacobian_builder.h"

using namespace std;

class JACOBIAN_BUILDER_TEST : public Test::Suite
{
    public:
        JACOBIAN_BUILDER_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_form_and_show_seprate_jacobians();
        void test_update_seprate_jacobians();
        void test_get_full_jacobian_for_coupled_P_and_Q_equations();
        void test_get_decoupled_B_jacobian_for_P_equations();
        void test_get_decoupled_B_jacobian_for_Q_equations();
    private:
        POWER_SYSTEM_DATABASE* db;
        NETWORK_DATABASE* network_db;
        JACOBIAN_BUILDER* jacobian_builder;
};

#endif
