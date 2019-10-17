#include "header/basic/bus_index.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
using namespace std;

BUS_INDEX::BUS_INDEX()
{
	max_bus_number = 0;
    index.clear();
    index.push_back(INDEX_NOT_EXIST);
}

BUS_INDEX::~BUS_INDEX()
{
	//max_bus_number = 0;
    //index.clear();
}
void BUS_INDEX::set_max_bus_number(size_t max_bus_number)
{
    //it is possible to include all buses in China with 7 digits.
    // four digit for area(province+city) last three for bus
    if(max_bus_number>10000000) max_bus_number = 10000000;

	this->max_bus_number = max_bus_number;

    index.resize(this->max_bus_number+1, INDEX_NOT_EXIST);
}

void BUS_INDEX::set_bus_with_index(size_t bus, size_t thisindex)
{
    if(bus!=0 and bus<=this->get_max_bus_number())
        index[bus] = thisindex;
}

size_t BUS_INDEX::get_max_bus_number() const
{
    return max_bus_number;
}

void BUS_INDEX::clear()
{
    for(size_t i=1; i<=max_bus_number; ++i) index[i]=INDEX_NOT_EXIST;
}

size_t BUS_INDEX::get_index_of_bus(const size_t bus) const
{
    if(bus !=0 and bus <= this->get_max_bus_number())
        return index[bus];
    else
        return INDEX_NOT_EXIST;
}

size_t BUS_INDEX::operator[](const size_t bus) const
{
    return get_index_of_bus(bus);
}
