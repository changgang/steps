#ifndef DEVICE_H
#define DEVICE_H

#include "header/basic/ownership.h"
#include "header/basic/device_id.h"
#include "header/block/block.h"
#include "header/model/model.h"
#include <cstddef>  // declearation of unsigned int
#include <sstream>
#include <iomanip>

class STEPS;

class DEVICE
{
    public:
        DEVICE(STEPS& toolkit);
        virtual ~DEVICE();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        virtual bool is_in_area(unsigned int area) const = 0;
        virtual bool is_in_zone(unsigned int zone) const = 0;

        virtual bool is_valid() const = 0;
        virtual void check() = 0;
        virtual void clear()  = 0;

        virtual void report() const = 0;

        virtual DEVICE_ID get_device_id() const = 0;
        string get_compound_device_name() const;
    private:
        STEPS* toolkit;

};
#endif // DEVICE_H
