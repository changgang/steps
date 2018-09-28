#ifndef DEVICE_ID_H
#define DEVICE_ID_H

#include "header/basic/constants.h"
#include <vector>
#include <string>
#include <functional>
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
        virtual DEVICE_ID& operator= (const DEVICE_ID& device_id);
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
DEVICE_ID get_wt_generator_device_id(size_t bus, string identifier);
DEVICE_ID get_pv_unit_device_id(size_t bus, string identifier);
DEVICE_ID get_energy_storage_device_id(size_t bus, string identifier);
DEVICE_ID get_load_device_id(size_t bus, string identifier);
DEVICE_ID get_fixed_shunt_device_id(size_t bus, string identifier);
DEVICE_ID get_line_device_id(size_t ibus, size_t jbus, string identifier);
DEVICE_ID get_hvdc_device_id(size_t ibus, size_t jbus, string identifier);
DEVICE_ID get_transformer_device_id(size_t ibus, size_t jbus, size_t kbus, string identifier);
DEVICE_ID get_equivalent_device_id(size_t bus, string identifier);
DEVICE_ID get_general_device_id(vector<size_t> bus, string identifier);

namespace std
{
    template<> class hash<DEVICE_ID>
    {
        public:
        size_t operator()(const DEVICE_ID& did) const
        {
            TERMINAL terminal  = did.get_device_terminal();
            vector<size_t> buses = terminal.get_buses();
            size_t n = buses.size();

            size_t seed = 0;
            for(size_t i=0; i!=n; ++i)
            {
                size_t bus = buses[i];
                if(bus!=0)
                {
                    size_t hash_value = std::hash<std::size_t>{}(bus);
                    switch(i)
                    {
                        case 0:
                            seed ^= hash_value + STEPS_MAGIC1 + (seed << 6) + (seed >> 2);
                            break;
                        case 1:
                            seed ^= hash_value + STEPS_MAGIC2 + (seed << 6) + (seed >> 2);
                            break;
                        case 2:
                            seed ^= hash_value + STEPS_MAGIC3 + (seed << 6) + (seed >> 2);
                            break;
                        case 3:
                        default:
                            seed ^= hash_value + STEPS_MAGIC4 + (seed << 6) + (seed >> 2);
                            break;
                    }
                }
            }
            string id = did.get_device_identifier();
            if(id!="")
            {
                size_t hash_value = std::hash<std::string>{}(id);
                seed ^= hash_value + STEPS_MAGIC1 + (seed << 6) + (seed >> 2);
            }

            return seed;
        }
    };
}
#endif // DEVICE_ID_H
