#ifndef BUS_FREQUENCY_MODEL_H
#define BUS_FREQUENCY_MODEL_H

#include "header/block/differential_block.h"

class BUS;

class BUS_FREQUENCY_MODEL
{
    public:
        BUS_FREQUENCY_MODEL(STEPS& toolkit);
        ~BUS_FREQUENCY_MODEL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus_pointer(BUS* bus);
        BUS* get_bus_pointer() const;
        unsigned int get_bus() const;

        void initialize();
        void run(DYNAMIC_MODE mode);
        void update_for_applying_event();
    public:
        void set_frequency_deviation_in_pu(double f);
        double get_frequency_deviation_in_pu() const;
        double get_frequency_deviation_in_Hz() const;
        double get_frequency_in_pu() const;
        double get_frequency_in_Hz() const;
    private:
        STEPS* toolkit;
        DIFFERENTIAL_BLOCK frequency_block;
        BUS* bus_ptr;
        double fbase_Hz, tbase_s;
};

#endif // BUS_FREQUENCY_MODEL_H
