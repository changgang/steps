#ifndef INPHNO_H
#define INPHNO_H

#include <vector>

using namespace std;

class INPHNO
{
    public:
        INPHNO();
        ~INPHNO();
        INPHNO(const INPHNO& inphno);
        INPHNO& operator=(const INPHNO& inphno);

        //unsigned int get_internal_bus_count() const;

        void set_physical_internal_bus_number_pair(unsigned int physical_bus, unsigned int internal_bus);
        void update_with_new_internal_bus_permutation(const vector<unsigned int>& P);

        bool is_table_full() const;
        bool empty() const;
        void clear();

        unsigned int get_table_size() const;

        unsigned int get_internal_bus_number_of_physical_bus_number(unsigned int bus) const;
        unsigned int get_physical_bus_number_of_internal_bus_number(unsigned int bus) const;

        void report() const;

    private:
        void copy_from_const_inphno(const INPHNO& inphno);

        bool is_new_internal_bus_permutation_correct(const vector<unsigned int>& P);
        vector<unsigned int> physical_to_internal_lookup_table;
        vector<unsigned int> internal_to_physical_lookup_table;
};

#endif // INPHNO_H
