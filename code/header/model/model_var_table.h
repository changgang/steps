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
        void add_variable_name_index_pair(string var_name, size_t var_index);

        size_t operator[](const string var_name) const;
        string operator[](const size_t var_index) const;
    private:
        size_t get_variable_index_from_variable_name(string var_name) const;
        string get_variable_name_from_variable_index(size_t var_index) const;

        map<string, size_t> var_name2index_map;
        map<size_t, string> var_index2name_map;
};

#endif // MODEL_VAR_TABLE_H
