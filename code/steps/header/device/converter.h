#ifndef CONVERTER_H
#define CONVERTER_H

#include "header/device/nonbus_device.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"

#include <complex>
#include <string>

using namespace std;

class BUS;

class CONVERTER
{
    public:
        CONVERTER(STEPS& toolkit);
        virtual ~CONVERTER();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void clear();

        void copy_from_const_converter(const CONVERTER& converter);

        void set_side(CONVERTER_SIDE side);
        CONVERTER_SIDE get_side() const;

        void set_bus(const unsigned int bus);
        void set_valve_side_bus_name(string name);
        void set_name(string name);
        void set_power_percent(double percent);

        void set_dc_voltage_in_kV(double V);
        void set_dc_current_in_kA(double I);

        void set_transformer_grid_side_base_voltage_in_kV(const double V);
        void set_transformer_valve_side_base_voltage_in_kV(const double V);
        void set_transformer_impedance_in_ohm(const complex<double> z);
        void set_transformer_admittance_in_siemens(const complex<double> y);
        void set_transformer_max_tap_in_pu(const double maxtap);
        void set_transformer_min_tap_in_pu(const double mintap);
        void set_transformer_number_of_taps(const unsigned int n);

        void set_transformer_tap_in_pu(double tap);

        unsigned int get_bus() const;
        string get_valve_side_bus_name() const;
        string get_name() const;
        double get_power_percent() const;

        double get_dc_voltage_in_kV() const;
        double get_dc_current_in_kA() const;

        double get_transformer_grid_side_base_voltage_in_kV() const;
        double get_transformer_valve_side_base_voltage_in_kV() const;
        double get_transformer_grid2valve_turn_ratio() const;
        complex<double> get_transformer_impedance_in_ohm() const;
        complex<double> get_transformer_admittance_in_siemens() const;
        double get_transformer_max_tap_in_pu() const;
        double get_transformer_min_tap_in_pu() const;
        unsigned int get_transformer_number_of_taps() const;
        double get_transformer_tap_step_in_pu() const;

        double get_transformer_tap_in_pu() const;
        bool is_transformer_tap_fixed() const;

        double get_bus_positive_sequence_voltage_in_pu() const;
        double get_bus_positive_sequence_voltage_in_kV() const;
        complex<double> get_bus_positive_sequence_complex_voltage_in_pu() const;
        double get_bus_base_voltage_in_kV() const;

        bool is_connected_to_bus(unsigned int bus) const;
        bool is_in_area(unsigned int area) const;
        bool is_in_zone(unsigned int zone) const;

    private:
        BUS* get_bus_pointer() const;

        void try_to_set_transformer_grid2valve_turn_ratio();
        void try_to_set_transformer_tap_step();
        void try_to_set_transformer_tap_fixed_flag();
    private:
        STEPS* toolkit;

        CONVERTER_SIDE side;

        unsigned int bus;
        BUS* busptr;
        string valve_bus_name;
        string converter_name;
        double power_percent;

        double dc_voltage_in_kV, dc_current_in_kA;

        double transformer_grid_side_base_voltage_in_kV, transformer_valve_side_base_voltage_in_kV;
        complex<double> transformer_Z_in_ohm, transformer_Y_in_siemens;  // transformer Z is used for commutation, Y is not used at all.
        double transformer_max_tap_in_pu, transformer_min_tap_in_pu;
        unsigned int transformer_number_of_taps;

        double transformer_tap_in_pu;

        double transformer_turn_ratio;
        double transformer_tap_step_in_pu;
        bool transformer_tap_fixed;
};
#endif // HVDC_H
