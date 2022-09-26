#include "header/device/mutual_data.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

MUTUAL_DATA::MUTUAL_DATA(STEPS& toolkit): DEVICE(toolkit)
{
    clear();
}

MUTUAL_DATA::~MUTUAL_DATA()
{
}

void MUTUAL_DATA::clear()
{
    first_lineptr = NULL;
    second_lineptr = NULL;
    Zm = 0.0;
    starting_location_of_first_line = 0.0;
    ending_location_of_first_line = 0.0;
    starting_location_of_second_line = 0.0;
    ending_location_of_second_line = 0.0;
}

bool MUTUAL_DATA::is_in_area(unsigned int area) const
{
    ;
}

bool MUTUAL_DATA::is_in_zone(unsigned int zone) const
{
    ;
}

bool MUTUAL_DATA::is_valid() const
{
    ;
}

void MUTUAL_DATA::check()
{
    ;
}

void MUTUAL_DATA::report() const
{
    ;
}

DEVICE_ID MUTUAL_DATA::get_device_id() const
{
    ;
}


void MUTUAL_DATA::set_first_line_pointer(LINE* lineptr)
{
    first_lineptr = lineptr;
}

void MUTUAL_DATA::set_second_line_pointer(LINE* lineptr)
{
    second_lineptr = lineptr;
}

void MUTUAL_DATA::set_mutual_impedance(complex<double> z)
{
    Zm = z;
}

void MUTUAL_DATA::set_starting_location_of_first_line(double location)
{
    starting_location_of_first_line = location;
}

void MUTUAL_DATA::set_ending_location_of_first_line(double location)
{
    ending_location_of_first_line = location;
}

void MUTUAL_DATA::set_starting_location_of_second_line(double location)
{
    starting_location_of_second_line = location;
}

void MUTUAL_DATA::set_ending_location_of_second_line(double location)
{
    ending_location_of_second_line = location;
}

LINE* MUTUAL_DATA::get_first_line_pointer() const
{
    return first_lineptr;
}

LINE* MUTUAL_DATA::get_second_line_pointer() const
{
    return second_lineptr;
}

LINE* MUTUAL_DATA::get_mutual_line(LINE* lineptr) const
{
    if(lineptr==first_lineptr)
        return second_lineptr;
    else
        return first_lineptr;
}

complex<double> MUTUAL_DATA::get_mutual_impedance() const
{
    return Zm;
}

double MUTUAL_DATA::get_starting_location_of_line(LINE* lineptr) const
{
    if(lineptr == first_lineptr)
        return starting_location_of_first_line;
    else
        return starting_location_of_second_line;
}

double MUTUAL_DATA::get_ending_location_of_line(LINE* lineptr) const
{
    if(lineptr == first_lineptr)
        return ending_location_of_first_line;
    else
        return ending_location_of_second_line;
}

bool MUTUAL_DATA::is_related_to_line(unsigned int ibus, unsigned int jbus, const string& identifier)
{
    STEPS& toolkit = get_toolkit();
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

    DEVICE_ID did = get_line_device_id(ibus, jbus, identifier);
    LINE* lineptr = psdb.get_line(did);
    if(lineptr==first_lineptr)
        return true;
    else if(lineptr==second_lineptr)
        return true;
    else
        return false;
}

