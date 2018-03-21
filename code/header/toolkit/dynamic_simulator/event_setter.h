#ifndef EVENT_SETTER_H
#define EVENT_SETTER_H

#include "header/toolkit/dynamics_simulator.h"
#include <complex>
class EVENT_SETTER
{
    public:
        EVENT_SETTER(DYNAMICS_SIMULATOR& simulator);
        ~EVENT_SETTER();

        void change_dynamic_simulator_time_step(double newDELT);

        void set_bus_fault(size_t bus, complex<double> fault_shunt);
        void clear_bus_fault(size_t bus);

        void set_line_fault(DEVICE_ID line_id, size_t side_bus, double location, complex<double> fault_shunt);
        void clear_line_fault(DEVICE_ID line_id);
        void trip_line(DEVICE_ID line_id);
        void trip_line_breaker(DEVICE_ID line_id, size_t side_bus);
        void close_line(DEVICE_ID line_id);
        void close_line_breaker(DEVICE_ID line_id, size_t side_bus);

        void trip_generator(DEVICE_ID gen_id);
        void shed_generator(DEVICE_ID gen_id,double percent);

        void trip_load(DEVICE_ID load_id);
        void scale_load(DEVICE_ID load_id, double percent);
        void scale_all_load(double percent);

        void bypass_hvdc(DEVICE_ID hvdc_id);
        void block_hvdc(DEVICE_ID hvdc_id);

        void change_generator_voltage_reference_in_pu(DEVICE_ID gen_id, double vref);
        void change_generator_power_reference_in_MW(DEVICE_ID gen_id, double Pref);
    private:
        DYNAMICS_SIMULATOR* simulator;
};
#endif // EVENT_SETTER_H
