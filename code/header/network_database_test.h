#ifndef NETWORK_DATABASE_TEST_H
#define NETWORK_DATABASE_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/network_database.h"

using namespace std;

class NETWORK_DATABASE_TEST : public Test::Suite
{
    public:
        NETWORK_DATABASE_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_build_and_get_network_matrix();
        void test_build_and_get_decoupled_network_matrix();
        void test_build_and_get_dynamic_network_matrix();
        void test_optimize_network_ordering();
        void test_check_network_connectivity();
        void test_get_islands();
        void test_save_network_matrix_to_file();
        void test_build_network_with_bus_out_of_service();
    private:
        POWER_SYSTEM_DATABASE* db;
        NETWORK_DATABASE* network_db;
};

#endif//NETWORK_DATABASE_TEST_H
