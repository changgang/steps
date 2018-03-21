#ifndef BUS_FREQUENCY_MODEL_H
#define BUS_FREQUENCY_MODEL_H

#include "header/basic/base.h"
#include "header/block/differential_block.h"

class BUS;

class BUS_FREQUENCY_MODEL : public BASE
{
    public:
        BUS_FREQUENCY_MODEL();
        virtual ~BUS_FREQUENCY_MODEL();

        void set_bus(size_t bus);

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
        size_t bus;
        BUS* bus_ptr;
};

#endif // BUS_FREQUENCY_MODEL_H
