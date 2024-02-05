#ifndef PLL_MODEL_H
#define PLL_MODEL_H

#include "header/STEPS.h"

class BUS;

class PLL_MODEL
{
    public:
        PLL_MODEL(STEPS& toolkit);
        ~PLL_MODEL();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus_pointer(BUS* bus);
        BUS* get_bus_pointer() const;
        unsigned int get_bus() const;

        double get_pll_frequency_deviation_in_pu() const;
        double get_pll_frequency_in_pu() const;
        double get_pll_frequency_in_Hz() const;
        double get_pll_angle_in_deg() const;

        void initialize_base_frequency_and_base_period();
        double get_base_frequency_in_Hz() const;
        double get_base_angle_speed_in_radps() const;
        double get_base_period_in_s() const;
        complex<double> get_terminal_complex_voltage_in_pu() const;
        double get_terminal_voltage_angle_in_rad() const;
    public:
        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_pll_frequency_deviation_in_Hz() const = 0;
        virtual double get_pll_angle_in_rad() const = 0;
    private:
        STEPS* toolkit;
        BUS* bus_ptr;
        double fbase_Hz, tbase_s;
};

#endif // PLL_MODEL_H
