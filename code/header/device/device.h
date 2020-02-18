#ifndef DEVICE_H
#define DEVICE_H

#include "header/basic/base.h"
#include "header/basic/ownership.h"
#include "header/basic/device_id.h"
#include "header/block/block.h"
#include "header/model/model.h"
#include <cstddef>  // declearation of unsigned int
#include <sstream>
#include <iomanip>

class POWER_SYSTEM_DATABASE;

class DEVICE : public BASE
{
    public:
        DEVICE(STEPS& toolkit);
        virtual ~DEVICE();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        //double get_dynamic_simulator_time_in_s() const;

        void set_ownership(const OWNERSHIP& ownership);
        OWNERSHIP get_ownership() const;
        unsigned int get_owner_count() const;
        unsigned int get_owner_of_index(unsigned int index) const;
        double get_fraction_of_owner_of_index(unsigned int index) const;

        virtual bool is_connected_to_bus(unsigned int bus) const = 0;
        virtual bool is_in_area(unsigned int area) const = 0;
        virtual bool is_in_zone(unsigned int zone) const = 0;

        virtual bool is_valid() const = 0;
        virtual void check() = 0;
        virtual void clear()  = 0;

        virtual void report() const = 0;
        virtual void save() const = 0;

        virtual DEVICE_ID get_device_id() const = 0;
        string get_device_name() const;

        virtual void set_model(const MODEL* model) = 0;
        virtual MODEL* get_model_of_type(string model_type) = 0;
    private:
        STEPS* toolkit;
        OWNERSHIP ownership;

};
#endif // DEVICE_H
