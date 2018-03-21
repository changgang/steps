#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include <vector>
#include <string>
#include "terminal.h"
using namespace std;

class DEVICE_ID
{
    public:
        DEVICE_ID();
        DEVICE_ID(const DEVICE_ID& did);
        virtual ~DEVICE_ID();
        void set_device_type(string device_type);
        void set_device_terminal(const TERMINAL& terminal);
        void set_device_identifier(string device_identifier);

        string get_device_type() const;
        TERMINAL get_device_terminal() const;
        string get_device_identifier() const;
        size_t get_minimum_allowed_terminal_count() const;
        size_t get_maximum_allowed_terminal_count() const;

        string get_device_name() const;

        bool is_valid() const;

        void clear();
        DEVICE_ID& operator= (const DEVICE_ID& device_id);
        bool operator< (const DEVICE_ID& device_id) const;
        bool operator==(const DEVICE_ID& device_id) const;
        bool operator!=(const DEVICE_ID& device_id) const;
    private:
        void initialize_minimum_maximum_terminal_count();
        void set_minimum_allowed_terminal_count(size_t n);
        void set_maximum_allowed_terminal_count(size_t n);
        void set_device_type_and_allowed_terminal_count(string device_type);
        bool is_given_terminal_acceptable(const TERMINAL& terminal);

    private:
        string device_type;
        size_t minimum_terminal_count, maximum_terminal_count; bool allow_identifier;
        TERMINAL terminal;
        string device_identifier;
};


DEVICE_ID get_bus_device_id(size_t bus_number);
DEVICE_ID get_generator_device_id(size_t bus, string identifier);
DEVICE_ID get_pe_source_device_id(size_t bus, string identifier);
DEVICE_ID get_load_device_id(size_t bus, string identifier);
DEVICE_ID get_fixed_shunt_device_id(size_t bus, string identifier);
DEVICE_ID get_line_device_id(size_t ibus, size_t jbus, string identifier);
DEVICE_ID get_hvdc_device_id(size_t ibus, size_t jbus, string identifier);
DEVICE_ID get_transformer_device_id(size_t ibus, size_t jbus, size_t kbus, string identifier);
DEVICE_ID get_equivalent_device_id(size_t bus, string identifier);

#endif // DEVICE_ID_H
