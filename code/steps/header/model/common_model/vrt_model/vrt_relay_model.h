#ifndef VRT_RELAY_MODEL_H
#define VRT_RELAY_MODEL_H

#include "header/model/common_model/common_model.h"
#include "header/basic/multi_point_line.h"
#include "header/basic/timer.h"

class VRT_RELAY_MODEL:public COMMON_MODEL
{
    public:
        VRT_RELAY_MODEL(STEPS& toolkit);
        ~VRT_RELAY_MODEL();
        void set_vrt_trip_time_delay_in_s(double t);
        void add_vrt_time_volt_threshold_pair(double t, double v);
        void set_vrt_trip_scale(double scale);
        MULTI_POINT_LINE get_vrt_line() const;
        TIMER& get_vrt_trip_timer();
        double get_vrt_trip_time_delay_in_s() const;
        unsigned int get_vrt_point_number() const;
        double get_vrt_time_threshold(unsigned int index) const;
        double get_vrt_volt_threshold(unsigned int index) const;
        double get_vrt_trip_scale() const;

        double get_time_when_vrt_enter_during_status() const;
        void set_time_when_vrt_enter_during_status(double t);

        void set_point_time_with_index(unsigned int index, double value);
        void set_point_voltage_with_index(unsigned int index, double value);

    public:
        void  copy_from_const_model(const VRT_RELAY_MODEL& model);
        void clear();
        virtual void check_vrt_relay() = 0;
        virtual void initialize_vrt_relay() =0;
    private:
        MULTI_POINT_LINE vrt_line;
        TIMER vrt_trip_timer;
        double vrt_trip_scale;

        double t0_vrt_activated;
};

#endif // VRT_RELAY_MODEL_H
