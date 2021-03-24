#ifndef MODEL_VAR_TABLE_H
#define MODEL_VAR_TABLE_H

#include <string>
#include <map>

using namespace std;

class MODEL_VAR_TABLE
{
    public:
        MODEL_VAR_TABLE();
        virtual ~MODEL_VAR_TABLE();
        void clear();
        void add_variable_name_index_pair(string var_name, unsigned int var_index);
        unsigned int size() const;

        unsigned int operator[](const string var_name) const;
        string operator[](const unsigned int var_index) const;
    private:
        unsigned int get_variable_index_from_variable_name(string var_name) const;
        string get_variable_name_from_variable_index(unsigned int var_index) const;

        map<string, unsigned int> var_name2index_map;
        map<unsigned int, string> var_index2name_map;
};

#endif // MODEL_VAR_TABLE_H
