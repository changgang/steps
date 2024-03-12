#ifndef LCC_HVDC2T_H
#define LCC_HVDC2T_H

#include "header/device/nonbus_device.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/lcc_hvdc2t_model/lcc_hvdc2t_model.h"
#include "header/model/auxiliary_signal_model/auxiliary_signal_model.h"

#include <complex>
#include <string>


using namespace std;

class BUS;
class LCC_HVDC2T_MODEL;

class LCC_HVDC2T : public NONBUS_DEVICE
{
    public:
        LCC_HVDC2T(STEPS& toolkit);
        virtual ~LCC_HVDC2T();

        void set_converter_bus(CONVERTER_SIDE converter, const unsigned int bus);
        void set_converter_valve_side_bus_name(CONVERTER_SIDE converter, string name);
        void set_identifier(const string identifier);
        void set_name(const string name);
        void set_status(const bool status);
        void set_line_resistance_in_ohm(const double R);
        void set_line_inductance_in_mH(const double L);
        void set_line_capacitance_in_uF(const double C);
        void set_line_smooting_inductance_in_mH(CONVERTER_SIDE converter, const double L);
        void set_line_smooting_resistance_in_ohm(CONVERTER_SIDE converter, const double R);
        void set_nominal_dc_power_in_MW(const double P);
        void set_side_to_hold_power(CONVERTER_SIDE side);
        void set_nominal_dc_current_in_kA(const double I);
        void set_nominal_dc_voltage_in_kV(const double V);
        void set_compensating_resistance_to_hold_dc_voltage_in_ohm(const double R);
        void set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(const double V);
        void set_current_power_margin(const double margin);
        void set_meter_end(const CONVERTER_SIDE converter);

        void set_converter_operation_mode(CONVERTER_SIDE converter, const HVDC_OPERATION_MODE mode);
        void set_converter_number_of_bridge(CONVERTER_SIDE converter, const unsigned int nbridge);
        void set_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE converter, const double V);
        void set_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE converter, const double angle);
        void set_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE converter, const double angle);

        void set_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE converter, const double V);
        void set_converter_transformer_converter_side_base_voltage_in_kV(CONVERTER_SIDE converter, const double V);
        void set_converter_transformer_impedance_in_ohm(CONVERTER_SIDE converter, const complex<double> z);
        void set_converter_transformer_admittance_in_siemens(CONVERTER_SIDE converter, const complex<double> y);
        void set_converter_transformer_max_tap_in_pu(CONVERTER_SIDE converter, const double maxtap);
        void set_converter_transformer_min_tap_in_pu(CONVERTER_SIDE converter, const double mintap);
        void set_converter_transformer_number_of_taps(CONVERTER_SIDE converter, const unsigned int ntap);

        unsigned int get_converter_bus(CONVERTER_SIDE converter) const;
        BUS* get_bus_pointer(CONVERTER_SIDE converter) const;

        string get_converter_valve_side_bus_name(CONVERTER_SIDE converter) const;
        string get_identifier() const;
        string get_name() const;
        unsigned int get_identifier_index() const;
        unsigned int get_name_index() const;
        bool get_status() const;
        bool is_blocked() const;
        double get_line_resistance_in_ohm() const;
        double get_line_inductance_in_mH() const;
        double get_line_capacitance_in_uF() const;
        double get_line_smooting_inductance_in_mH(CONVERTER_SIDE converter) const;
        double get_line_smooting_resistance_in_ohm(CONVERTER_SIDE converter) const;
        double get_nominal_dc_power_in_MW() const;
        CONVERTER_SIDE get_side_to_hold_dc_power() const;
        double get_nominal_dc_current_in_kA() const;
        double get_nominal_dc_voltage_in_kV() const;
        double get_compensating_resistance_to_hold_dc_voltage_in_ohm() const;
        double get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV() const;
        double get_current_power_margin() const;
        CONVERTER_SIDE get_meter_end() const;

        HVDC_OPERATION_MODE get_converter_operation_mode(CONVERTER_SIDE converter) const;
        unsigned int get_converter_number_of_bridge(CONVERTER_SIDE converter) const;
        double get_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE converter) const;
        double get_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE converter) const;
        double get_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE converter) const;
        double get_converter_cos_max_alpha_or_gamma(CONVERTER_SIDE converter) const;
        double get_converter_cos_min_alpha_or_gamma(CONVERTER_SIDE converter) const;

        double get_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE converter) const;
        double get_converter_transformer_converter_side_base_voltage_in_kV(CONVERTER_SIDE converter) const;
        complex<double> get_converter_transformer_impedance_in_ohm(CONVERTER_SIDE converter) const;
        complex<double> get_converter_transformer_admittance_in_siemens(CONVERTER_SIDE converter) const;
        double get_converter_transformer_max_tap_in_pu(CONVERTER_SIDE converter) const;
        double get_converter_transformer_min_tap_in_pu(CONVERTER_SIDE converter) const;
        unsigned int get_converter_transformer_number_of_taps(CONVERTER_SIDE converter) const;

        void turn_rectifier_constant_power_mode_into_constant_current_mode();
        void turn_rectifier_constant_current_mode_into_constant_power_mode();

        double get_rectifier_nominal_dc_power_command_in_MW() const;
        double get_rectifier_nominal_dc_current_command_in_kA() const;
        double get_inverter_nominal_dc_voltage_command_in_kV() const;

        void reverse_converters();

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;
        virtual void report() const;
        virtual void save() const;

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        void set_2t_lcc_hvdc_model(LCC_HVDC2T_MODEL* model);
        void set_auxiliary_signal_model(AUXILIARY_SIGNAL_MODEL* model);

        LCC_HVDC2T_MODEL* get_2t_lcc_hvdc_model() const;
        AUXILIARY_SIGNAL_MODEL* get_auxiliary_signal_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual LCC_HVDC2T& operator=(const LCC_HVDC2T& hvdc);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        void solve_steady_state();
        void show_solved_steady_state() const;


        void set_converter_alpha_or_gamma_in_deg(CONVERTER_SIDE converter, double angle, double cos_angle=0.0);
        double get_converter_cos_alpha_or_gamma(CONVERTER_SIDE converter) const;
        void set_converter_transformer_tap_in_pu(CONVERTER_SIDE converter, double tap);

        void set_converter_dc_voltage_in_kV(CONVERTER_SIDE converter, const double V);
        void set_line_dc_current_in_kA(const double I);
        void set_converter_dc_current_in_kA(CONVERTER_SIDE converter, const double I);

        double get_converter_alpha_or_gamma_in_deg(CONVERTER_SIDE converter) const;
        double get_converter_transformer_tap_in_pu(CONVERTER_SIDE converter) const;

        double get_converter_commutating_overlap_angle_in_deg(CONVERTER_SIDE converter) const;
        double get_converter_dc_voltage_in_kV(CONVERTER_SIDE converter) const;
        double get_line_dc_current_in_kA() const;
        double get_converter_dc_current_in_kA(CONVERTER_SIDE converter) const;
        double get_converter_dc_power_in_MW(CONVERTER_SIDE converter) const;
        double get_converter_ac_active_power_in_MW(CONVERTER_SIDE converter) const;
        double get_converter_ac_reactive_power_in_MVar(CONVERTER_SIDE converter) const;
        double get_converter_ac_apparent_power_in_MVA(CONVERTER_SIDE converter) const;
        double get_converter_ac_power_factor(CONVERTER_SIDE converter) const;
        double get_converter_ac_current_in_kA(CONVERTER_SIDE converter) const;

        complex<double> get_converter_dynamic_current_in_pu_based_on_system_base_power(CONVERTER_SIDE converter);
    private:
        string get_converter_side_name(CONVERTER_SIDE converter) const;

        void solve_as_rectifier_regulating_power_and_inverter_regulating_voltage();
        void solve_as_rectifier_regulating_current_and_inverter_regulating_voltage();
        void solve_as_rectifier_regulating_power_and_inverter_regulating_gamma();
        void solve_as_rectifier_regulating_current_and_inverter_regulating_gamma();
        void solve_as_rectifier_regulating_alpha_and_inverter_regulating_current();

        bool solve_converter_transformer_tap_and_desired_firing_angle(CONVERTER_SIDE converter, double Vdc, double Idc);
        void solve_best_converter_transformer_tap_with_min_angle(CONVERTER_SIDE converter, double Vdc_R, double Idc);

        void solve_with_solved_tap_and_firing_angle();

        double solve_desired_converter_cosAngle_with_desired_dc_voltage_current_and_transformer_tap(CONVERTER_SIDE converter, double Vdc, double Idc, double Tap);

        double solve_converter_dc_voltage_in_kV_with_dc_current_and_transformer_tap(CONVERTER_SIDE converter, double Idc, double Tap);

        unsigned int converter_bus[2];
        BUS* converter_busptr[2];

        unsigned int converter_valve_bus_name_index[2];
        unsigned int identifier_index;
        unsigned int hvdc_name_index;
        bool status;
        double line_R_in_ohm, line_L_in_H, line_C_in_F;
        double smoothing_L_in_H[2], smoothing_R_in_ohm[2];
        double demand_power_in_MW, demand_voltage_in_kV, demand_current_in_kA;
        CONVERTER_SIDE hvdc_side_to_hold_power;
        double compensating_R_to_hold_dc_voltage_in_ohm;

        double mode_switching_voltage_in_kV;

        double dc_current_power_margin;

        CONVERTER_SIDE meter_end;

        HVDC_OPERATION_MODE operation_mode[2];
        unsigned int bridge_number[2];
        double voltage_drop_per_bridge_in_kV[2];
        double max_firing_angle_in_deg[2], min_firing_angle_in_deg[2];

        double converter_transformer_grid_side_base_voltage_in_kV[2], converter_transformer_converter_side_base_voltage_in_kV[2];
        complex<double> converter_transformer_Z_in_ohm[2], converter_transformer_Y_in_siemens[2];
        // transformer Z is used for commutation, Y is not used at all.
        double converter_transformer_max_tap_in_pu[2], converter_transformer_min_tap_in_pu[2];
        unsigned int converter_transformer_number_of_taps[2];

        double converter_firing_angle_in_deg[2];
        double converter_transformer_tap_in_pu[2];
        double converter_dc_voltage_in_kV[2], line_dc_current_in_kA[2];
        double cos_max_firing_angle[2], cos_min_firing_angle[2];
        double cos_converter_firing_angle[2];
    private:
        LCC_HVDC2T_MODEL* hvdc_model;
        AUXILIARY_SIGNAL_MODEL* auxiliary_signal_model;

        bool converter_firing_angle_fixed[2];
        bool temp_converter_firing_angle_fixed[2];
};
#endif // LCC_HVDC2T_H
