#ifndef BUS_FREQUENCY_MODEL_H
#define BUS_FREQUENCY_MODEL_H

#include "header/block/differential_block.h"

class BUS;

class BUS_FREQUENCY_MODEL : public BASE
{
    public:
        BUS_FREQUENCY_MODEL();
        virtual ~BUS_FREQUENCY_MODEL();

        void set_bus_pointer(BUS* bus);
        BUS* get_bus_pointer() const;
        unsigned int get_bus() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        void update_for_applying_event();
    public:
        void set_frequency_deviation_in_pu(double f);
        double get_frequency_deviation_in_pu() const;
        double get_frequency_deviation_in_Hz() const;
        double get_frequency_in_pu() const;
        double get_frequency_in_Hz() const;

    private:
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        DIFFERENTIAL_BLOCK frequency_block;
        BUS* bus_ptr;
        float fbase_Hz, tbase_s;
};

#endif // BUS_FREQUENCY_MODEL_H
