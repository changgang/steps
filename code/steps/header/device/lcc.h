#ifndef LCC_H
#define LCC_H

#include "header/device/converter.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"

#include <complex>
#include <string>

using namespace std;

class BUS;

class LCC : public CONVERTER
{
    public:
        LCC(STEPS& toolkit);
        virtual ~LCC();
        void clear();

        void set_number_of_bridge(const unsigned int n);
        void set_voltage_drop_per_bridge_in_kV(const double V);
        void set_max_angle_in_deg(const double angle);
        void set_min_angle_in_deg(const double angle);

        void set_transformer_grid_side_base_voltage_in_kV(const double V);
        void set_transformer_valve_side_base_voltage_in_kV(const double V);
        void set_transformer_impedance_in_ohm(const complex<double> z);
        void set_transformer_admittance_in_siemens(const complex<double> y);
        void set_transformer_max_tap_in_pu(const double maxtap);
        void set_transformer_min_tap_in_pu(const double mintap);
        void set_transformer_number_of_taps(const unsigned int n);

        void set_angle_fixed_flag(bool flag);

        unsigned int get_number_of_bridge() const;
        double get_voltage_drop_per_bridge_in_kV() const;
        double get_max_angle_in_deg() const;
        double get_min_angle_in_deg() const;
        double get_cos_max_angle() const;
        double get_cos_min_angle() const;

        double get_transformer_grid_side_base_voltage_in_kV() const;
        double get_transformer_valve_side_base_voltage_in_kV() const;
        double get_transformer_grid2valve_turn_ratio() const;
        complex<double> get_transformer_impedance_in_ohm() const;
        complex<double> get_transformer_admittance_in_siemens() const;
        double get_transformer_max_tap_in_pu() const;
        double get_transformer_min_tap_in_pu() const;
        unsigned int get_transformer_number_of_taps() const;
        double get_transformer_tap_step_in_pu() const;

        bool is_angle_fixed() const;
        bool is_transformer_tap_fixed() const;

        virtual LCC& operator=(const LCC& hvdc);

        void set_angle_in_deg(double angle, double cos_angle=0.0);
        void set_transformer_tap_in_pu(double tap);
        double get_angle_in_deg() const;
        double get_cos_angle() const;
        double get_transformer_tap_in_pu() const;

        double get_commutating_overlap_angle_in_deg() const;
        double get_dc_voltage_in_kV() const;
        double get_line_dc_current_in_kA() const;
        double get_dc_current_in_kA() const;
        double get_dc_power_in_MW() const;
        double get_ac_active_power_in_MW() const;
        double get_ac_reactive_power_in_MVar() const;
        double get_ac_apparent_power_in_MVA() const;
        double get_ac_power_factor() const;
        double get_ac_current_in_kA() const;

    public:
        void solve_transformer_tap_and_angle(double Vdc, double Idc);
        double get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle() const;
        double get_dc_voltage_drop_multiple_in_kV_per_A() const;
    private:
        void try_to_set_transformer_grid2valve_turn_ratio();
        void try_to_set_transformer_tap_step();
        void try_to_set_transformer_tap_fixed_flag();

        double get_bus_positive_sequence_voltage_in_kV() const;

        double solve_best_transformer_tap_with_min_angle(double Vdc, double Idc);
        double solve_desired_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(double Vdc, double Idc, double Tap);

        double solve_dc_voltage_in_kV_with_dc_current_and_transformer_tap(double Idc);
    private:
        unsigned int bridge_number;
        double voltage_drop_per_bridge_in_kV;
        double max_angle_in_deg, min_angle_in_deg;

        double transformer_grid_side_base_voltage_in_kV, transformer_valve_side_base_voltage_in_kV;
        complex<double> transformer_Z_in_ohm, transformer_Y_in_siemens;
        // transformer Z is used for commutation, Y is not used at all.
        double transformer_max_tap_in_pu, transformer_min_tap_in_pu;
        unsigned int transformer_number_of_taps;

        double angle_in_deg;
        double transformer_tap_in_pu;
        double dc_voltage_in_kV, line_dc_current_in_kA;
        double cos_max_angle, cos_min_angle;
        double cos_angle;
    private:
        double transformer_turn_ratio;
        double transformer_tap_step_in_pu;
        bool angle_fixed;
        bool transformer_tap_fixed;
};
#endif // HVDC_H
