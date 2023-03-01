#include "header/basic/complex3.h"
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;



COMPLEX3::COMPLEX3()
{
    set_complex_value_0(0);
    set_complex_value_1(0);
    set_complex_value_2(0);
}

COMPLEX3::COMPLEX3(complex<double> x, complex<double> y, complex<double> z)
{
    set_complex_value_0(x);
    set_complex_value_1(y);
    set_complex_value_2(z);
}

COMPLEX3::COMPLEX3(const COMPLEX3& V)
{
    copy_from_const_complex3(V);
}

COMPLEX3& COMPLEX3::operator=(const COMPLEX3& V)
{
    if(this==(&V)) return *this;

    copy_from_const_complex3(V);

    return *this;
}

void COMPLEX3::copy_from_const_complex3(const COMPLEX3& V)
{
    set_complex_value_0(V.get_complex_value_0());
    set_complex_value_1(V.get_complex_value_1());
    set_complex_value_2(V.get_complex_value_2());
}

COMPLEX3::~COMPLEX3()
{
    ;
}

void COMPLEX3::set_complex_value_0(complex<double> x)
{
    this->x = x;
}

void COMPLEX3::set_complex_value_1(complex<double> y)
{
    this->y = y;
}

void COMPLEX3::set_complex_value_2(complex<double> z)
{
    this->z = z;
}

complex<double> COMPLEX3::get_complex_value_0() const
{
    return x;
}

complex<double> COMPLEX3::get_complex_value_1() const
{
    return y;
}

complex<double> COMPLEX3::get_complex_value_2() const
{
    return z;
}

complex<double> COMPLEX3::operator[](char index) const
{
    index = toupper(index);
    switch(index)
    {
        case 'A':
        case 0:
            return get_complex_value_0();
        case 'B':
        case 1:
            return get_complex_value_1();
        case 'C':
        case 2:
            return get_complex_value_2();
        default:
            return 0.0;
    }
}
