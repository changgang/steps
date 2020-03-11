#ifndef TERMINAL_H
#define TERMINAL_H

#include <vector>
#include <string>

using namespace std;

class TERMINAL
{
    public:
        TERMINAL();
        virtual ~TERMINAL();
        void append_bus(unsigned int bus);
        bool has_bus(unsigned int bus);
        unsigned int get_bus_count() const;
        vector<unsigned int> get_buses();
        void clear();

        virtual TERMINAL& operator= (const TERMINAL& terminal);
        unsigned int operator[] (unsigned int index) const;
    private:
        void reject_to_append_non_positive_bus(unsigned int bus);
        void append_and_sort_buses(unsigned int bus);
        void sort_bus_in_ascending_order();
    private:
        vector<unsigned int> buses;
};

#endif // TERMINAL_H
