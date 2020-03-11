#include "header/basic/terminal.h"
#include "header/basic/utility.h"
#include "header/basic/exception.h"

#include <istream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>

using namespace std;

TERMINAL::TERMINAL()
{
    buses.clear();
}

TERMINAL::~TERMINAL()
{
}

void TERMINAL::append_bus(unsigned int bus)
{
    if(bus!=0 and (not has_bus(bus)))
        append_and_sort_buses(bus);
}

bool TERMINAL::has_bus(unsigned int bus)
{
    bool exist_flag = false;
    unsigned int n = get_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(buses[i]==bus)
        {
            exist_flag = true;
            break;
        }
    }
    return exist_flag;
}

void TERMINAL::append_and_sort_buses(unsigned int bus)
{
    buses.push_back(bus);
    sort_bus_in_ascending_order();
}

void TERMINAL::sort_bus_in_ascending_order()
{
    sort(buses.begin(), buses.end());
}

unsigned int TERMINAL::get_bus_count() const
{
    return buses.size();
}

vector<unsigned int> TERMINAL::get_buses()
{
    return buses;
}

void TERMINAL::clear()
{
    buses.clear();
}

TERMINAL& TERMINAL::operator= (const TERMINAL& terminal)
{
    if(this==(&terminal)) return *this;

    this->clear();

    unsigned int n = terminal.get_bus_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        append_bus(terminal[i]);
    }
    return *this;
}

unsigned int TERMINAL::operator[] (unsigned int index) const
{
    if(index<get_bus_count())
        return buses[index];
    else
        return 0;
}
