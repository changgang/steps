#ifndef LVPL_H
#define LVPL_H

#include "header/block/integral_block.h"
#include "header/block/saturation_block.h"

// Low voltage power logic
class LVPL
{
    public:
        LVPL();
        LVPL(const LVPL& lvpl);
        virtual ~LVPL();

    public: // sync generator common
        void set_low_voltage_in_pu(double v);
        void set_high_voltage_in_pu(double v);
        void set_gain_at_high_voltage(double g);

        double get_low_voltage_in_pu() const;
        double get_high_voltage_in_pu() const;
        double get_gain_at_hig_voltage() const;

        double get_LVPL_order(double v) const;

        LVPL& operator=(const LVPL& lvpl);
    public:

    private:
        void copy_from_const_object(const LVPL& lvpl);
        double v_low, v_high, g_high;
};

#endif // LVPL_H
