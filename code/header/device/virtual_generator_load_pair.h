#ifndef VIRTUAL_GENERATOR_LOAD_PAIR_H
#define VIRTUAL_GENERATOR_LOAD_PAIR_H


// it should be rename as MIRROR class
#include "header/basic/device_id.h"
#include "header/power_system_database.h"

#include <string>
#include <complex>

using namespace std;

class VIRTUAL_GENERATOR_LOAD_PAIR : public DEVICE
{
    public:
        VIRTUAL_GENERATOR_LOAD_PAIR();
        virtual ~VIRTUAL_GENERATOR_LOAD_PAIR();

        void set_generator_power_system_name(string name);
        void set_load_power_system_name(string name);

        string get_generator_power_system_name() const;
        string get_load_power_system_name() const;

        void set_generator_power_system_index(size_t index);
        void set_load_power_system_index(size_t index);


        size_t get_generator_power_system_index() const;
        size_t get_load_power_system_index() const;

        void set_virtual_generator(DEVICE_ID generator);
        DEVICE_ID get_virtual_generator() const;

        void set_virtual_load(DEVICE_ID load);
        DEVICE_ID get_virtual_load() const;


        void update();

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(size_t bus) const;
        virtual bool is_in_area(size_t area) const;
        virtual bool is_in_zone(size_t zone) const;
        virtual void report() const;
        virtual void save() const;

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;
    private:
        virtual void set_model(const MODEL* model);
        void set_power_system_database(POWER_SYSTEM_DATABASE* db);
        POWER_SYSTEM_DATABASE* get_power_system_database() const;
        POWER_SYSTEM_DATABASE* generator_power_system_db;
        POWER_SYSTEM_DATABASE* load_power_system_db;

        DEVICE_ID generator_id, load_id;
};
#endif // VIRTUAL_GENERATOR_LOAD_PAIR_H
