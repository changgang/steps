#ifndef COMMON_MODEL_H
#define COMMON_MODEL_H

#include "header/STEPS.h"
#include "header/basic/constants.h"
#include "header/basic/utility.h"
#include "header/power_system_database.h"
#include <iostream>

class BUS;

class COMMON_MODEL
{
    public:
        COMMON_MODEL(STEPS& toolkit);
        ~COMMON_MODEL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus_pointer(BUS* bus);
        BUS* get_bus_pointer() const;
        unsigned int get_bus_number() const;
        void set_device_pointer(NONBUS_DEVICE* device);
        NONBUS_DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        STEPS_DEVICE_TYPE get_device_type() const;
        string get_compound_device_name() const;

    public:

        double get_bus_voltage_in_pu() const;
        complex<double> get_bus_complex_voltage_in_pu() const;
        double get_bus_voltage_angle_in_rad() const;
        double get_bus_frequency_in_pu() const;
        double get_bus_frequency_in_Hz() const;
        double get_bus_frequency_deviation_in_pu() const;
        double get_bus_frequency_deviation_in_Hz() const;

        double get_base_frequency_in_Hz() const;
        double get_base_angle_speed_in_radps() const;
        double get_base_period_in_s() const;

    private:
        STEPS* toolkit;
        BUS* bus;
        NONBUS_DEVICE* device;
};

#endif // COMMON_MODEL_H

