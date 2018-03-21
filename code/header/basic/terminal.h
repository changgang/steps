#ifndef TERMINAL_H
#define TERMINAL_H

#include <vector>
#include <string>

using namespace std;

class TERMINAL
{
    public:
        TERMINAL();
        ~TERMINAL();
        void append_bus(size_t bus);
        bool has_bus(size_t bus);
        size_t get_bus_count() const;
        vector<size_t> get_buses();
        void clear();

        TERMINAL& operator= (const TERMINAL& terminal);
        size_t operator[] (size_t index) const;
    private:
        void reject_to_append_non_positive_bus(size_t bus);
        void append_and_sort_buses(size_t bus);
        void sort_bus_in_ascending_order();
    private:
        vector<size_t> buses;
};

#endif // TERMINAL_H
