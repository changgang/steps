#ifndef DEVICE_H
#define DEVICE_H

#include "header/basic/base.h"
#include "header/basic/ownership.h"
#include "header/basic/device_id.h"
#include "header/block/block.h"
#include "header/model/model.h"
#include <cstddef>  // declearation of size_t
#include <sstream>
#include <iomanip>

class POWER_SYSTEM_DATABASE;

class DEVICE : public BASE
{
    public:
        DEVICE();
        virtual ~DEVICE();

        //double get_dynamic_simulator_time_in_s() const;

        void set_ownership(const OWNERSHIP& ownership);
        OWNERSHIP get_ownership() const;
        size_t get_owner_count() const;
        size_t get_owner_of_index(size_t index) const;
        double get_fraction_of_owner_of_index(size_t index) const;

        virtual bool is_connected_to_bus(size_t bus) const = 0;
        virtual bool is_in_area(size_t area) const = 0;
        virtual bool is_in_zone(size_t zone) const = 0;


        virtual void report() const = 0;
        virtual void save() const = 0;

        virtual DEVICE_ID get_device_id() const = 0;
        string get_device_name() const;

        virtual void set_model(const MODEL* model) = 0;
    private:
        OWNERSHIP ownership;

};
#endif // DEVICE_H
