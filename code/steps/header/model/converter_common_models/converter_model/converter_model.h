#ifndef CONVERTER_MODEL_H
#define CONVERTER_MODEL_H

#include "header/STEPS.h"

class BUS;

class CONVERTER_MODEL
{
    public:
        CONVERTER_MODEL(STEPS& toolkit);
        ~CONVERTER_MODEL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus_pointer(BUS* busptr);
        BUS* get_bus_pointer() const;
        void set_device_pointer(NONBUS_DEVICE* device);
        NONBUS_DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        string get_compound_device_name() const;
    public:
        virtual string get_model_name() const = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_virtual_frequency_deviation_in_Hz() const = 0;
        virtual double get_virtual_angle_in_rad() const = 0;
        virtual double get_virtual_voltage_in_pu() const = 0;
        virtual string get_standard_psse_string() const = 0;
    private:
        STEPS* toolkit;
        BUS* busptr;
        NONBUS_DEVICE* device;
};

#endif // CONVERTER_MODEL_H
