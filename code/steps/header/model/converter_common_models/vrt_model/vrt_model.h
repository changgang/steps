#ifndef VRT_MODEL_H
#define VRT_MODEL_H

#include "header/STEPS.h"
#include "header/basic/constants.h"
#include "header/basic/multi_point_line.h"

class BUS;

class VRT_MODEL
{
    public:
        VRT_MODEL(STEPS& toolkit);
        ~VRT_MODEL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_device_pointer(NONBUS_DEVICE* device);
        void set_bus_pointer(BUS* bus);
        NONBUS_DEVICE* get_device_pointer() const;
        BUS* get_bus_pointer() const;
        unsigned int get_bus() const;
        double get_bus_voltage();

        void set_vrt_trip_time_delay_in_s(double t);
        void set_vrt_point_number(unsigned int n);
        void set_vrt_time_volt_threshold_pair(unsigned int index, double t, double v);
        void set_vrt_trip_scale(double scale);

        MULTI_POINT_LINE& get_vrt_line();
        TIMER& get_vrt_trip_timer();

        double get_vrt_trip_time_delay_in_s() const;
        unsigned int get_vrt_point_number() const;
        double get_vrt_time_threshold(unsigned int index) const;
        double get_vrt_volt_threshold(unsigned int index) const;
        double get_vrt_trip_scale() const;

        void set_vrt_status(VRT_STATUS status);
        double get_vrt_activation_time() const;
        VRT_STATUS get_vrt_status() const;
    public:
        void  copy_from_const_model(const VRT_MODEL& model);
        virtual void check_vrt_status() = 0;
    private:
        STEPS* toolkit;
        BUS* bus_ptr;
        NONBUS_DEVICE* device;

        TIMER vrt_trip_timer;
        MULTI_POINT_LINE vrt_line;
        double vrt_trip_scale;

        double t0_vrt_activated;
        VRT_STATUS status;
};

#endif // VRT_MODEL_H
