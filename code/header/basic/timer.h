#ifndef TIMER_H
#define TIMER_H

#include "header/device/device.h"
#include <string>
using namespace std;

class TIMER
{
    public:
        TIMER();
        TIMER(const TIMER& timer);
        virtual ~TIMER();

        void set_attached_device(DEVICE* device);
        DEVICE* get_attached_device() const;

        void set_timer_interval_in_s(double t);
        void start();
        void reset();
        bool is_started() const;
        bool is_timed_out() const;

        double get_timer_interval_in_s() const;
        double get_time_when_started_in_s() const;

        bool is_valid() const;
        void check();
        void report() const;
        void clear();

        virtual TIMER& operator=(const TIMER& timer);
    private:
        double time_interval_in_s;
        double time_when_timer_is_started_in_s;
        DEVICE* device_ptr;
};
#endif // TIMER_H
