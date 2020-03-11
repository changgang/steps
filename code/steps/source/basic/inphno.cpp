#include "header/basic/inphno.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"

#include <istream>
#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

INPHNO::INPHNO()
{
    clear();
}

INPHNO::~INPHNO()
{
    ;
}

void INPHNO::set_physical_internal_bus_number_pair(unsigned int physical_bus, unsigned int internal_bus)
{
    if(physical_bus != 0)
    {
        unsigned int current_internal_bus_of_input_physical_bus = get_internal_bus_number_of_physical_bus_number(physical_bus);

        if(current_internal_bus_of_input_physical_bus!=internal_bus)
        {
            unsigned int current_physical_bus_of_input_internal_bus = get_physical_bus_number_of_internal_bus_number(internal_bus);

            if(current_internal_bus_of_input_physical_bus!=INDEX_NOT_EXIST)
            {
                physical_to_internal_lookup_table[physical_bus] = INDEX_NOT_EXIST;
                internal_to_physical_lookup_table[current_internal_bus_of_input_physical_bus] = INDEX_NOT_EXIST;
            }

            if(current_physical_bus_of_input_internal_bus!=INDEX_NOT_EXIST)
            {
                internal_to_physical_lookup_table[internal_bus] = INDEX_NOT_EXIST;
                physical_to_internal_lookup_table[current_physical_bus_of_input_internal_bus] = INDEX_NOT_EXIST;
            }

            unsigned int n_physical = physical_to_internal_lookup_table.size();
            unsigned int n_internal = internal_to_physical_lookup_table.size();

            if(n_physical<physical_bus+1)
                physical_to_internal_lookup_table.resize(physical_bus+1, INDEX_NOT_EXIST);
            if(n_internal<=internal_bus)
                internal_to_physical_lookup_table.resize(internal_bus+1, INDEX_NOT_EXIST);


            physical_to_internal_lookup_table[physical_bus] = internal_bus;
            internal_to_physical_lookup_table[internal_bus] = physical_bus;
        }
    }
}

void INPHNO::update_with_new_internal_bus_permutation(const vector<unsigned int>& P)
{
    if(is_new_internal_bus_permutation_correct(P))
    {
        unsigned int n = P.size();
        unsigned int new_internal_bus, old_internal_bus, physical_bus;

        vector<unsigned int> old_internal_to_physical_lookup_table = internal_to_physical_lookup_table;
        for(unsigned int i=0; i!=n; ++i)
        {
            new_internal_bus = i;
            old_internal_bus = P[i];

            physical_bus = old_internal_to_physical_lookup_table[old_internal_bus];

            set_physical_internal_bus_number_pair(physical_bus, new_internal_bus);
        }
    }
    else
    {
        ostringstream osstream;
        osstream<<"Warning. Internal bus permutation is incorrect. Physical-internal bus pair will not be updated.";
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        return;
    }
}

bool INPHNO::is_new_internal_bus_permutation_correct(const vector<unsigned int>& P)
{
    vector<unsigned int> Psort = P;

    sort(Psort.begin(), Psort.end(), less<unsigned int>());

    unsigned int nP = P.size(), nPsort = Psort.size();

    bool flag = true;
    if(nP == nPsort)
    {
        for(unsigned int i = 0; i!=nPsort; ++i)
        {
            if(Psort[i] == i)
                continue;
            else
            {
                flag = false;
                break;
            }
        }
    }
    return flag;
}

bool INPHNO::is_table_full() const
{
    unsigned int nbus = internal_to_physical_lookup_table.size();

    bool full_logic = true;

    unsigned int physical_bus;

    for(unsigned int internal_bus=0; internal_bus!=nbus; ++internal_bus)
    {
        physical_bus = get_physical_bus_number_of_internal_bus_number(internal_bus);

        if(physical_bus != INDEX_NOT_EXIST)
            continue;
        else
        {
            full_logic = false;
            break;
        }
    }
    return full_logic;
}

bool INPHNO::empty() const
{
    if(physical_to_internal_lookup_table.size()<=1 or internal_to_physical_lookup_table.size()==0)
        return true;
    else
        return false;
}

void INPHNO::clear()
{
    physical_to_internal_lookup_table.clear();
    internal_to_physical_lookup_table.clear();
}

unsigned int INPHNO::get_table_size() const
{
    return physical_to_internal_lookup_table.size();
}

unsigned int INPHNO::get_internal_bus_number_of_physical_bus_number(unsigned int bus) const
{
    if(not empty())
    {
        unsigned int nmax = physical_to_internal_lookup_table.size();
        if(bus<=nmax-1)
            return physical_to_internal_lookup_table[bus];
        else
            return INDEX_NOT_EXIST;
    }
    else
        return INDEX_NOT_EXIST;
}

unsigned int INPHNO::get_physical_bus_number_of_internal_bus_number(unsigned int bus) const
{
    if(not empty())
    {
        unsigned int nmax = internal_to_physical_lookup_table.size();
        if(bus<nmax)
            return internal_to_physical_lookup_table[bus];
        else
            return INDEX_NOT_EXIST;
    }
    else
        return INDEX_NOT_EXIST;
}

void INPHNO::report() const
{
    ostringstream osstream;

    osstream<<"Physical<-->internal bus number pair:";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

    unsigned int n = internal_to_physical_lookup_table.size();

    unsigned int bus;
    for(unsigned int i=0; i!=n; ++i)
    {
        bus = get_physical_bus_number_of_internal_bus_number(i);
        osstream<<setw(8)<<bus<<"<-->"<<setw(8)<<i;
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
    osstream<<"All physical<-->internal bus number pair exported.";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
}
