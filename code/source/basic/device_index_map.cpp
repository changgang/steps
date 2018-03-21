#include "header/basic/device_index_map.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include <iostream>

DEVICE_INDEX_MAP::DEVICE_INDEX_MAP()
{
    index_map.clear();
}

DEVICE_INDEX_MAP::~DEVICE_INDEX_MAP()
{
    index_map.clear();
}

void DEVICE_INDEX_MAP::set_device_index(const DEVICE_ID& device_id, size_t index)
{
    if(index==INDEX_NOT_EXIST)
    {
        map<DEVICE_ID,size_t>::iterator iter  = index_map.begin();
        size_t current_index = get_index_of_device(device_id);
        if(current_index == INDEX_NOT_EXIST)
            return;
        else
        {
            iter = index_map.find(device_id);
            index_map.erase(iter);
            return;
        }
    }

    if(empty())
    {
        index_map.insert(pair<DEVICE_ID, size_t>(device_id, index));
    }
    else
    {
        if(is_given_device_of_the_same_type_as_existing_devices(device_id))
        {
            size_t current_index = get_index_of_device(device_id);
            if(current_index==INDEX_NOT_EXIST)
                index_map.insert(pair<DEVICE_ID, size_t>(device_id, index));
            else
            {
                TERMINAL terminal = device_id.get_device_terminal();

                std::cout<<get_system_time_stamp_string()<<" "<<"Warning. The index of "<<device_id.get_device_type()<<" '"
                         <<device_id.get_device_identifier()<<"' at bus"
                         <<(terminal.get_bus_count()>1?"es":"")<<" ";
                vector<size_t> buses = terminal.get_buses();
                size_t nbus = terminal.get_bus_count();
                for(size_t i=0; i!=nbus; ++i)
                {
                    std::cout<<get_system_time_stamp_string()<<" "<<buses[i]<<" ";
                }
                std::cout<<"exists. Duplicate index may cause the previous one lost"<<endl;

                map<DEVICE_ID, size_t>::iterator iter = index_map.find(device_id);
                iter->second = index;
            }
        }
    }
}

void DEVICE_INDEX_MAP::decrease_index_by_1_for_device_with_index_greater_than(size_t index)
{
    map<DEVICE_ID, size_t>::iterator iter;
    map<DEVICE_ID, size_t>::iterator iter_end = index_map.end();
    for(iter = index_map.begin(); iter!=iter_end; iter++)
    {
        if(iter->second>index and iter->second!=INDEX_NOT_EXIST)
            iter->second --;
    }
}

void DEVICE_INDEX_MAP::clear()
{
    index_map.clear();
}

bool DEVICE_INDEX_MAP::empty() const
{
    return index_map.empty();
}

size_t DEVICE_INDEX_MAP::get_index_of_device(const DEVICE_ID& device_id) const
{
    if(empty()) return INDEX_NOT_EXIST;

    if(not is_given_device_of_the_same_type_as_existing_devices(device_id))
        return INDEX_NOT_EXIST;

    map<DEVICE_ID, size_t>::const_iterator iter = index_map.begin();
    iter = index_map.find(device_id);
    if(iter==index_map.end()) return INDEX_NOT_EXIST;
    else                      return iter->second;
}

size_t DEVICE_INDEX_MAP::operator[](const DEVICE_ID& device_id) const
{
    return get_index_of_device(device_id);
}

bool DEVICE_INDEX_MAP::is_given_device_of_the_same_type_as_existing_devices(const DEVICE_ID& device_id) const
{
    map<DEVICE_ID, size_t>::const_iterator iter = index_map.begin();

    DEVICE_ID this_device_id = iter->first;

    if(this_device_id.get_device_type() == device_id.get_device_type())
        return true;
    else
        return false;
}
