#include "header/basic/zone.h"
#include "header/basic/utility.h"

#include <istream>
#include <iostream>

using namespace std;

ZONE::ZONE(POWER_SYSTEM_DATABASE* psdb)
{
    set_power_system_database(psdb);
    clear();
}

ZONE::~ZONE()
{
    ;
}

void ZONE::set_zone_number(size_t number)
{
    if(number!=0)
        this->zone_number = number;
    else
    {
        cout<<get_system_time_stamp_string()<<" "<<"0 is not allowed for setting zone number. 0 will be set to indicated invalid zone."<<endl;
        this->zone_number = 0;
    }
}

void ZONE::set_zone_name(string name)
{
    this->zone_name = trim_string(name);
}

size_t ZONE::get_zone_number() const
{
    return zone_number;
}

string ZONE::get_zone_name() const
{
    return zone_name;
}

bool ZONE::is_valid() const
{
    if(get_zone_number()!=0)
        return true;
    else
        return false;
}

void ZONE::check()
{
    ;
}

void ZONE::clear()
{
    zone_number = 0;
    set_zone_name("");
}

void ZONE::report() const
{
    ostringstream osstream;

    osstream<<"Zone "<<get_zone_number()<<" ("<<get_zone_name()<<")";
    show_information_with_leading_time_stamp(osstream);
}

ZONE& ZONE::operator=(const ZONE& zone)
{
    if(this==(&zone)) return *this;

    clear();

    set_power_system_database(zone.get_power_system_database());

    if(zone.get_zone_number()!=0)
        set_zone_number(zone.get_zone_number());

    set_zone_name(zone.get_zone_name());
    return *this;
}
