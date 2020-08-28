#ifndef CONVERTER_H
#define CONVERTER_H

#include "header/device/nonbus_device.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"

#include <complex>
#include <string>

using namespace std;

class BUS;

class CONVERTER
{
    public:
        CONVERTER(STEPS& toolkit);
        virtual ~CONVERTER();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus(const unsigned int bus);
        void set_valve_side_bus_name(string name);
        void set_name(string name);
        void set_power_percent(double percent);

        unsigned int get_bus() const;
        BUS* get_bus_pointer() const;
        string get_valve_side_bus_name() const;
        string get_name() const;
        double get_power_percent() const;
    private:
        void clear();
    private:
        STEPS* toolkit;

        unsigned int bus;
        BUS* busptr;
        string valve_bus_name;
        string converter_name;
        double power_percent;
};
#endif // HVDC_H
