#include "header/basic/owner.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>
#include <cmath>

using namespace std;

OWNER::OWNER(STEPS& toolkit)
{
    set_toolkit(toolkit);
    clear();
}

OWNER::~OWNER()
{
    ;
}

void OWNER::set_toolkit(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
}

STEPS& OWNER::get_toolkit() const
{
    return (*toolkit);
}

void OWNER::set_owner_number(unsigned int number)
{
    if(number != 0)
        this->owner_number = number;
    else
    {
        ostringstream osstream;
        osstream<<"0 is not allowed for setting owner number. 0 will be set to indicated invalid owner.";
        toolkit->show_information_with_leading_time_stamp(osstream);

        this->owner_number = 0;
    }
}

void OWNER::set_owner_name(string name)
{
    this->owner_name = trim_string(name);
}

unsigned int OWNER::get_owner_number() const
{
    return owner_number;
}

string OWNER::get_owner_name() const
{
    return owner_name;
}


bool OWNER::is_valid() const
{
    if(get_owner_number()!=0)
        return true;
    else
        return false;
}

void OWNER::check()
{
    if(get_owner_number()==0)
    {
        ostringstream osstream;
        osstream<<"Error. Zero (0) is not allowed for owner number.";
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

void OWNER::clear()
{
    owner_number = 0;
    set_owner_name("");
}

void OWNER::report() const
{
    ostringstream osstream;

    osstream<<"Owner "<<get_owner_number()<<" ("<<get_owner_name()<<").";
    toolkit->show_information_with_leading_time_stamp(osstream);
}

OWNER& OWNER::operator=(const OWNER& owner)
{
    if(this==(&owner)) return *this;

    set_toolkit(owner.get_toolkit());

    if(owner.get_owner_number()!=0)
        set_owner_number(owner.get_owner_number());

    set_owner_name(owner.get_owner_name());
    return *this;
}
