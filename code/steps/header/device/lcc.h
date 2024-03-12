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

        void set_angle_fixed_flag(bool flag);

        unsigned int get_number_of_bridge() const;
        double get_voltage_drop_per_bridge_in_kV() const;
        double get_max_angle_in_deg() const;
        double get_min_angle_in_deg() const;
        double get_cos_max_angle() const;
        double get_cos_min_angle() const;

        bool is_angle_fixed() const;

        virtual LCC& operator=(const LCC& hvdc);

        void set_angle_in_deg(double angle, double cos_angle=0.0);
        double get_angle_in_deg() const;
        double get_cos_angle() const;

        double get_commutating_overlap_angle_in_deg() const;
        double get_dc_power_in_MW() const;
        double get_ac_active_power_in_MW() const;
        double get_ac_reactive_power_in_MVar() const;
        double get_ac_apparent_power_in_MVA() const;
        double get_ac_power_factor() const;
        double get_ac_current_in_kA() const;
    public:
        void solve_with_desired_dc_voltage_and_current(double Vdc, double Idc);
        void show_steady_state();
        double get_no_load_dc_voltage_in_kV_with_solved_transformer_tap_and_angle() const;
        double get_dc_voltage_drop_multiple_in_kV_per_A() const;
    private:
        void solve_transformer_tap_and_angle(double Vdc, double Idc);
        void solve_best_transformer_tap_with_min_angle(double Vdc, double Idc);
        double get_desired_cosAngle_with_desired_dc_voltage_current(double Vdc, double Idc);
        double get_maximum_avaiable_dc_voltage_in_kV();

        double get_actual_dc_voltage_in_kV_with_dc_current(double Idc);
    private:
        unsigned int bridge_number;
        double voltage_drop_per_bridge_in_kV;
        double max_angle_in_deg, min_angle_in_deg;

        double angle_in_deg;
        double cos_max_angle, cos_min_angle;
        double cos_angle;
    private:
        bool angle_fixed;
};
#endif // LCC_H
