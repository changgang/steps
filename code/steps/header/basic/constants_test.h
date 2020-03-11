#ifndef CONSTANTS_TEST_H
#define CONSTANTS_TEST_H

#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

using namespace std;

class CONSTANTS_TEST : public Test::Suite
{
    public:
        CONSTANTS_TEST();
    protected:
        virtual void setup();
        virtual void tear_down();
    private:
        void test_constants();

};
#endif // CONSTANTS_TEST_H
