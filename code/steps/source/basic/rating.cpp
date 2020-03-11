#include <cstdlib>
#include <cstring>
#include <istream>
#include <iostream>


#include "header/basic/rating.h"

using namespace std;
RATING::RATING()
{
    set_rating_A_MVA(0.0);
    set_rating_B_MVA(0.0);
    set_rating_C_MVA(0.0);
}
RATING::~RATING()
{
    ;
}

void RATING::set_rating_A_MVA(double rate)
{
    if(rate>=0)
        rating_A_MVA = rate;
}

void RATING::set_rating_B_MVA(double rate)
{
    if(rate>=0)
        rating_B_MVA = rate;
}

void RATING::set_rating_C_MVA(double rate)
{
    if(rate>=0)
        rating_C_MVA = rate;
}

double RATING::get_rating_A_MVA() const
{
    return rating_A_MVA;
}

double RATING::get_rating_B_MVA() const
{
    return rating_B_MVA;
}

double RATING::get_rating_C_MVA() const
{
    return rating_C_MVA;
}

RATING& RATING::operator= (const RATING& rating)
{
    if(this==(&rating)) return *this;

    set_rating_A_MVA(rating.get_rating_A_MVA());
    set_rating_B_MVA(rating.get_rating_B_MVA());
    set_rating_C_MVA(rating.get_rating_C_MVA());

    return *this;
}

double RATING::operator[](char index) const
{
    index = toupper(index);
    switch(index)
    {
        case 'A':
        case 1:
            return get_rating_A_MVA();
        case 'B':
        case 2:
            return get_rating_B_MVA();
        case 'C':
        case 3:
            return get_rating_C_MVA();
        default:
            return 0.0;
    }
}

void RATING::clear()
{
    set_rating_A_MVA(0.0);
    set_rating_B_MVA(0.0);
    set_rating_C_MVA(0.0);
}
