#include "header/basic/owner.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <cmath>

using namespace std;


OWNER::OWNER(POWER_SYSTEM_DATABASE* psdb)
{
    set_power_system_database(psdb);
    clear();
}

OWNER::~OWNER()
{
    ;
}

void OWNER::set_owner_number(size_t number)
{
    if(number != 0)
        this->owner_number = number;
    else
    {
        cout<<get_system_time_stamp_string()<<" "<<"0 is not allowed for setting owner number. 0 will be set to indicated invalid owner."<<endl;
        this->owner_number = 0;
    }
}

void OWNER::set_owner_name(string name)
{
    this->owner_name = trim_string(name);
}

size_t OWNER::get_owner_number() const
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
        cout<<get_system_time_stamp_string()<<" "<<"Error. Zero (0) is not allowed for owner number."<<endl;
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
    show_information_with_leading_time_stamp(osstream);
}

OWNER& OWNER::operator=(const OWNER& owner)
{
    if(this==(&owner)) return *this;

    OWNER(owner.get_power_system_database());

    if(owner.get_owner_number()!=0)
        set_owner_number(owner.get_owner_number());

    set_owner_name(owner.get_owner_name());
    return *this;
}
