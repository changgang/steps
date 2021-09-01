#include "header/basic/string_int_map.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include <istream>
#include <iostream>

STR_INT_MAP::STR_INT_MAP()
{
    clear();
}

STR_INT_MAP::~STR_INT_MAP()
{
    clear();
}

void STR_INT_MAP::add_new_string(const string& str)
{
    if(get_index_of_string(str)==INDEX_NOT_EXIST)
    {
        unsigned int n = get_map_size();
        int2str_vector.push_back(str);
        str2int_map.insert(pair<string, unsigned int>(str, n));
    }
}

void STR_INT_MAP::clear()
{
    int2str_vector.clear();
    str2int_map.clear();
    add_new_string("");
}

unsigned int STR_INT_MAP::operator[](const string& str) const
{
    return get_index_of_string(str);
}

unsigned int STR_INT_MAP::get_index_of_string(const string& str) const
{
    map<string, unsigned int>::const_iterator iter = str2int_map.begin();
    iter = str2int_map.find(str);
    if(iter!=str2int_map.end()) return iter->second;
    else                        return INDEX_NOT_EXIST;
}

string STR_INT_MAP::operator[](unsigned int index) const
{
    return get_string_of_index(index);
}

string STR_INT_MAP::get_string_of_index(unsigned int index) const
{
    if(index<get_map_size())
        return int2str_vector[index];
    else
        return "INVALID STRING FROM STR_INT_MAP";
}

unsigned int STR_INT_MAP::get_map_size() const
{
    return int2str_vector.size();
}
