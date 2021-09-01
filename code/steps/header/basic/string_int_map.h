#ifndef STR_INT_MAP_H
#define STR_INT_MAP_H

#include <vector>
#include <string>
#include <map>
#include <functional>
using namespace std;

class STR_INT_MAP
{
    public:
        STR_INT_MAP();
        virtual ~STR_INT_MAP();

        void add_new_string(const string& str);

        void clear();

        unsigned int operator[](const string& str) const;
        string operator[](unsigned int index) const;

        unsigned int get_map_size() const;
    private:
        unsigned int get_index_of_string(const string& str) const;
        string get_string_of_index(unsigned int index) const;

        map<string, unsigned int> str2int_map;
        vector<string> int2str_vector;
        unsigned int map_size;
};

#endif // STR_INT_MAP_H
