#ifndef BUS_INDEX_H
#define BUS_INDEX_H

#include <vector>

using namespace std;

class BUS_INDEX
{
    public:
        BUS_INDEX();
        ~BUS_INDEX();

        void set_max_bus_number(unsigned int max_bus_number);

        void set_bus_with_index(unsigned int bus, unsigned int thisindex);

        unsigned int get_max_bus_number() const;
        void clear();

        unsigned int get_index_of_bus(const unsigned int bus) const;
        unsigned int operator[](const unsigned int bus) const;
    private:
		unsigned int max_bus_number;
        vector<unsigned int> index;
        unsigned int* index_array;
};

#endif // BUS_INDEX_H
