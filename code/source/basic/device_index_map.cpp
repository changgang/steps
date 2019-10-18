#include "header/basic/device_index_map.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include <istream>
#include <iostream>

DEVICE_INDEX_MAP::DEVICE_INDEX_MAP()
{
    index_map.clear();
    index_umap.clear();
}

DEVICE_INDEX_MAP::~DEVICE_INDEX_MAP()
{
}

void DEVICE_INDEX_MAP::set_device_index(const DEVICE_ID& device_id, size_t index)
{
    set_device_index_map(device_id, index);
}
void DEVICE_INDEX_MAP::swap_device_index(const DEVICE_ID& device1, const DEVICE_ID& device2)
{
    size_t index1 = get_index_of_device(device1);
    size_t index2 = get_index_of_device(device2);
    if(index1==INDEX_NOT_EXIST and index2==INDEX_NOT_EXIST)
        return;
    else
    {
        if(index1!=INDEX_NOT_EXIST and index2!=INDEX_NOT_EXIST)
        {
            set_device_index(device1, INDEX_NOT_EXIST);
            set_device_index(device1, index2);

            set_device_index(device2, INDEX_NOT_EXIST);
            set_device_index(device2, index1);
        }
        else
        {
            if(index1!=INDEX_NOT_EXIST and index2==INDEX_NOT_EXIST)
            {
                set_device_index(device1, index2);
                set_device_index(device2, index1);
            }
            else
            {
                set_device_index(device2, index1);
                set_device_index(device1, index2);
            }
        }
    }

}

void DEVICE_INDEX_MAP::set_device_index_map(const DEVICE_ID& device_id, size_t index)
{
    if(index!=INDEX_NOT_EXIST)
    {
        if(not empty())
        {
            if(is_given_device_of_the_same_type_as_existing_devices(device_id))
            {
                size_t current_index = get_index_of_device(device_id);
                if(current_index==INDEX_NOT_EXIST)
                    index_map.insert(pair<DEVICE_ID, size_t>(device_id, index));
                else
                {
                    TERMINAL terminal = device_id.get_device_terminal();

                    ostringstream osstream;
                    osstream<<"Warning. The index of "<<device_id.get_device_type()<<" '"
                             <<device_id.get_device_identifier()<<"' at bus"
                             <<(terminal.get_bus_count()>1?"es":"")<<" ";
                    vector<size_t> buses = terminal.get_buses();
                    size_t nbus = terminal.get_bus_count();
                    for(size_t i=0; i!=nbus; ++i)
                    {
                        std::cout<<buses[i]<<" ";
                    }
                    osstream<<"exists. Duplicate index may cause the previous one lost.";
                    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

                    map<DEVICE_ID, size_t>::iterator iter = index_map.find(device_id);
                    iter->second = index;
                }
            }
        }
        else
        {
            index_map.insert(pair<DEVICE_ID, size_t>(device_id, index));
        }
    }
    else
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
}

void DEVICE_INDEX_MAP::set_device_index_umap(const DEVICE_ID& device_id, size_t index)
{
    if(index!=INDEX_NOT_EXIST)
    {
        if(not empty())
        {
            if(is_given_device_of_the_same_type_as_existing_devices(device_id))
            {
                size_t current_index = get_index_of_device(device_id);
                if(current_index==INDEX_NOT_EXIST)
                    index_umap.insert(pair<DEVICE_ID, size_t>(device_id, index));
                else
                {
                    TERMINAL terminal = device_id.get_device_terminal();

                    ostringstream osstream;
                    osstream<<"Warning. The index of "<<device_id.get_device_type()<<" '"
                             <<device_id.get_device_identifier()<<"' at bus"
                             <<(terminal.get_bus_count()>1?"es":"")<<" ";
                    vector<size_t> buses = terminal.get_buses();
                    size_t nbus = terminal.get_bus_count();
                    for(size_t i=0; i!=nbus; ++i)
                    {
                        osstream<<buses[i]<<" ";
                    }
                    osstream<<"exists. Duplicate index may cause the previous one lost.";
                    show_information_with_leading_time_stamp_with_default_toolkit(osstream);

                    unordered_map<DEVICE_ID, size_t>::iterator iter = index_umap.find(device_id);
                    iter->second = index;
                }
            }
        }
        else
        {
            index_umap.insert(pair<DEVICE_ID, size_t>(device_id, index));
        }
    }
    else
    {
        unordered_map<DEVICE_ID,size_t>::iterator iter  = index_umap.begin();
        size_t current_index = get_index_of_device(device_id);
        if(current_index == INDEX_NOT_EXIST)
            return;
        else
        {
            iter = index_umap.find(device_id);
            index_umap.erase(iter);
            return;
        }
    }
}

void DEVICE_INDEX_MAP::decrease_index_by_1_for_device_with_index_greater_than(size_t index)
{
    decrease_index_by_1_for_device_with_index_greater_than_map(index);
}

void DEVICE_INDEX_MAP::decrease_index_by_1_for_device_with_index_greater_than_map(size_t index)
{
    map<DEVICE_ID, size_t>::iterator iter;
    map<DEVICE_ID, size_t>::iterator iter_end = index_map.end();
    for(iter = index_map.begin(); iter!=iter_end; ++iter)
    {
        if(iter->second>index and iter->second!=INDEX_NOT_EXIST)
            --(iter->second);
    }
}

void DEVICE_INDEX_MAP::decrease_index_by_1_for_device_with_index_greater_than_umap(size_t index)
{
    unordered_map<DEVICE_ID, size_t>::iterator iter = index_umap.begin();
    unordered_map<DEVICE_ID, size_t>::iterator iter_end = index_umap.end();
    for(; iter!=iter_end; ++iter)
    {
        if(iter->second>index and iter->second!=INDEX_NOT_EXIST)
            --(iter->second);
    }
}

void DEVICE_INDEX_MAP::clear()
{
    index_map.clear();
    index_umap.clear();
}

bool DEVICE_INDEX_MAP::empty() const
{
    return index_map.empty();
    return index_umap.empty();
}

size_t DEVICE_INDEX_MAP::get_index_of_device(const DEVICE_ID& device_id) const
{
    return get_index_of_device_map(device_id);
}

size_t DEVICE_INDEX_MAP::get_index_of_device_map(const DEVICE_ID& device_id) const
{
    if(not empty())
    {
        if(is_given_device_of_the_same_type_as_existing_devices(device_id))
        {
            map<DEVICE_ID, size_t>::const_iterator iter = index_map.begin();
            iter = index_map.find(device_id);
            if(iter!=index_map.end()) return iter->second;
            else                      return INDEX_NOT_EXIST;
        }
        else
            return INDEX_NOT_EXIST;
    }
    else
        return INDEX_NOT_EXIST;
}

size_t DEVICE_INDEX_MAP::get_index_of_device_umap(const DEVICE_ID& device_id) const
{
    if(not empty())
    {
        if(is_given_device_of_the_same_type_as_existing_devices(device_id))
        {
            unordered_map<DEVICE_ID, size_t>::const_iterator iter = index_umap.begin();
            iter = index_umap.find(device_id);
            if(iter!=index_umap.end()) return iter->second;
            else                       return INDEX_NOT_EXIST;
        }
        else
            return INDEX_NOT_EXIST;
    }
    else
        return INDEX_NOT_EXIST;
}

size_t DEVICE_INDEX_MAP::operator[](const DEVICE_ID& device_id) const
{
    return get_index_of_device(device_id);
}

bool DEVICE_INDEX_MAP::is_given_device_of_the_same_type_as_existing_devices(const DEVICE_ID& device_id) const
{
    return is_given_device_of_the_same_type_as_existing_devices_map(device_id);
}

bool DEVICE_INDEX_MAP::is_given_device_of_the_same_type_as_existing_devices_map(const DEVICE_ID& device_id) const
{
    map<DEVICE_ID, size_t>::const_iterator iter = index_map.begin();

    DEVICE_ID this_device_id = iter->first;

    if(this_device_id.get_device_type() == device_id.get_device_type())
        return true;
    else
        return false;
}

bool DEVICE_INDEX_MAP::is_given_device_of_the_same_type_as_existing_devices_umap(const DEVICE_ID& device_id) const
{
    unordered_map<DEVICE_ID, size_t>::const_iterator iter = index_umap.begin();

    DEVICE_ID this_device_id = iter->first;

    if(this_device_id.get_device_type() == device_id.get_device_type())
        return true;
    else
        return false;
}
