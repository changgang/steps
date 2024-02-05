#ifndef TIMER_H
#define TIMER_H

#include "header/device/device.h"
#include "header/basic/base.h"
#include <string>
using namespace std;

class TIMER : public BASE
{
    public:
        TIMER();
        TIMER(const TIMER& timer);
        virtual ~TIMER();

        void set_timer_interval_in_s(double t);
        void start();
        void reset();
        bool is_started() const;
        bool is_timed_out() const;

        double get_timer_interval_in_s() const;
        double get_time_when_started_in_s() const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();

        void report() const;

        virtual TIMER& operator=(const TIMER& timer);
    private:
        double time_interval_in_s;
        double time_when_timer_is_started_in_s;
};
#endif // TIMER_H
