#ifndef BUS_INDEX_H
#define BUS_INDEX_H

#include <vector>

using namespace std;

class BUS_INDEX
{
    public:
        BUS_INDEX();
        ~BUS_INDEX();

        void set_max_bus_number(size_t max_bus_number);

        void set_bus_with_index(size_t bus, size_t thisindex);

        size_t get_max_bus_number() const;
        void clear();

        size_t get_index_of_bus(const size_t bus) const;
        size_t operator[](const size_t bus) const;

    private:
		size_t max_bus_number;
        vector<size_t> index;
};

#endif // BUS_INDEX_H
