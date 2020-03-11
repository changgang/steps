#include "header/basic/bus_index.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
using namespace std;

BUS_INDEX::BUS_INDEX()
{
	max_bus_number = 0;
    //index.clear();
    //index.push_back(INDEX_NOT_EXIST);
    index_array=NULL;
}

BUS_INDEX::~BUS_INDEX()
{
    if(index_array!=NULL) delete [] index_array;
}

void BUS_INDEX::set_max_bus_number(unsigned int max_bus_number)
{
    //it is possible to include all buses in China with 7 digits.
    // four digit for area(province+city) last three for bus
    if(max_bus_number>10000000) max_bus_number = 10000000;

	this->max_bus_number = max_bus_number;

    //index.resize(this->max_bus_number+1, INDEX_NOT_EXIST);

    index_array = (unsigned int*) malloc((max_bus_number+1)*sizeof(unsigned int));
    for(unsigned int i=0; i<=max_bus_number; ++i) index_array[i]=INDEX_NOT_EXIST;
}

void BUS_INDEX::set_bus_with_index(unsigned int bus, unsigned int thisindex)
{
    if(bus!=0 and bus<=this->get_max_bus_number())
    {
        //index[bus] = thisindex;
        index_array[bus] = thisindex;
    }
}

unsigned int BUS_INDEX::get_max_bus_number() const
{
    return max_bus_number;
}

void BUS_INDEX::clear()
{
    for(unsigned int i=1; i<=max_bus_number; ++i)
    {
        //index[i]=INDEX_NOT_EXIST;
        index_array[i]=INDEX_NOT_EXIST;
    }
}

unsigned int BUS_INDEX::get_index_of_bus(const unsigned int bus) const
{
    if(bus !=0 and bus <= this->get_max_bus_number())
        return index_array[bus];
        //return index[bus];
    else
        return INDEX_NOT_EXIST;
}

unsigned int BUS_INDEX::operator[](const unsigned int bus) const
{
    return get_index_of_bus(bus);
}
