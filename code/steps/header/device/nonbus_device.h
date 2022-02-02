#ifndef NONBUS_DEVICE_H
#define NONBUS_DEVICE_H

#include "header/basic/ownership.h"
#include "header/device/device.h"
#include <cstddef>  // declearation of unsigned int
#include <sstream>
#include <iomanip>

class NONBUS_DEVICE : public DEVICE
{
    public:
        NONBUS_DEVICE(STEPS& toolkit);
        virtual ~NONBUS_DEVICE();

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

        virtual void set_model(MODEL* model) = 0;
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0) = 0;
    private:
        OWNERSHIP ownership;
};
#endif // NONBUS_DEVICE_H
