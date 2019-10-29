#ifndef VSC_HVDC_H
#define VSC_HVDC_H

#include "header/device/device.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/hvdc_model/hvdc_model.h"
#include "header/model/auxiliary_signal_model/auxiliary_signal_model.h"

#include <complex>
#include <string>


using namespace std;

class BUS;
class VSC_HVDC_MODEL;

class VSC_HVDC : public DEVICE
{
    public:
        VSC_HVDC();
        virtual ~VSC_HVDC();

        void set_converter_bus(HVDC_CONVERTER_SIDE converter, const size_t bus);
        void set_converter_valve_side_bus_name(HVDC_CONVERTER_SIDE converter, string name);
        void set_identifier(const string identifier);
        void set_name(const string name);
        void set_status(const bool status);
        void set_line_resistance_in_ohm(const double R);
        void set_converter_operation_mode(HVDC_CONVERTER_SIDE converter, const VSC_HVDC_OPERATION_MODE mode);
        void set_converter_dc_power_command_in_MW(HVDC_CONVERTER_SIDE converter, double P);
        void set_converter_dc_voltage_command_in_kV(HVDC_CONVERTER_SIDE converter, double V);

        size_t get_converter_bus(HVDC_CONVERTER_SIDE converter) const;
        BUS* get_bus_pointer(HVDC_CONVERTER_SIDE converter) const;

        string get_converter_valve_side_bus_name(HVDC_CONVERTER_SIDE converter) const;
        string get_identifier() const;
        string get_name() const;
        bool get_status() const;
        bool is_blocked() const;
        VSC_HVDC_POLE get_number_of_poles() const;
        double get_line_resistance_in_ohm() const;
        double get_line_inductance_in_mH() const;
        double get_line_capacitance_in_uF() const;
        double get_line_smooting_inductance_in_mH(HVDC_CONVERTER_SIDE converter) const;
        double get_line_smooting_resistance_in_ohm(HVDC_CONVERTER_SIDE converter) const;
        double get_nominal_dc_power_per_pole_in_MW() const;
        HVDC_CONVERTER_SIDE get_side_to_hold_dc_power() const;
        double get_nominal_dc_current_per_pole_in_kA() const;
        double get_nominal_dc_voltage_per_pole_in_kV() const;
        double get_compensating_resistance_to_hold_dc_voltage_in_ohm() const;
        double get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV() const;
        double get_current_power_margin() const;
        HVDC_CONVERTER_SIDE get_meter_end() const;

        VSC_HVDC_OPERATION_MODE get_converter_operation_mode(HVDC_CONVERTER_SIDE converter) const;
        size_t get_converter_number_of_bridge(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_voltage_drop_per_bridge_in_kV(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_max_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_min_alpha_or_gamma_in_deg(HVDC_CONVERTER_SIDE converter) const;

        double get_converter_transformer_grid_side_base_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_transformer_converter_side_base_voltage_in_kV(HVDC_CONVERTER_SIDE converter) const;
        complex<double> get_converter_transformer_impedance_in_ohm(HVDC_CONVERTER_SIDE converter) const;
        complex<double> get_converter_transformer_admittance_in_siemens(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_transformer_max_tap_in_pu(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_transformer_min_tap_in_pu(HVDC_CONVERTER_SIDE converter) const;
        size_t get_converter_transformer_number_of_taps(HVDC_CONVERTER_SIDE converter) const;

        void turn_rectifier_constant_power_mode_into_constant_current_mode();
        void turn_rectifier_constant_current_mode_into_constant_power_mode();

        double get_rectifier_nominal_dc_power_command_in_MW() const;
        double get_rectifier_nominal_dc_current_command_in_kA() const;
        double get_inverter_nominal_dc_voltage_command_in_kV() const;

        void reverse_converters();

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual bool is_connected_to_bus(size_t bus) const;
        virtual bool is_in_area(size_t area) const;
        virtual bool is_in_zone(size_t zone) const;
        virtual void report() const;
        virtual void save() const;

        virtual void set_model(const MODEL* model);

        void set_hvdc_model(const VSC_HVDC_MODEL* model);
        void set_auxiliary_signal_model(const AUXILIARY_SIGNAL_MODEL* model);

        void clear_hvdc_model();
        void clear_auxiliary_signal_model();

        VSC_HVDC_MODEL* get_hvdc_model() const;
        AUXILIARY_SIGNAL_MODEL* get_auxiliary_signal_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual VSC_HVDC& operator=(const VSC_HVDC& hvdc);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        void solve_steady_state();
        void show_solved_hvdc_steady_state() const;

        complex<double> get_converter_dynamic_current_in_pu_based_on_system_base_power(HVDC_CONVERTER_SIDE converter);
    private:
        string get_converter_side_name(HVDC_CONVERTER_SIDE converter) const;

        size_t converter_bus[2];
        BUS* converter_busptr[2];

        string converter_valve_bus_name[2];
        string identifier;
        string hvdc_name;
        bool status;

        VSC_HVDC_OPERATION_MODE operation_mode[2];
        size_t bridge_number[2];
        double voltage_drop_per_bridge_in_kV[2];
        double max_firing_angle_in_deg[2], min_firing_angle_in_deg[2];

        double converter_transformer_grid_side_base_voltage_in_kV[2], converter_transformer_converter_side_base_voltage_in_kV[2];
        complex<double> converter_transformer_Z_in_ohm[2], converter_transformer_Y_in_siemens[2];
        // transformer Z is used for commutation, Y is not used at all.
        double converter_transformer_max_tap_in_pu[2], converter_transformer_min_tap_in_pu[2];
        size_t converter_transformer_number_of_taps[2];

        double converter_firing_angle_in_deg[2], converter_transformer_tap_in_pu[2];
        double converter_dc_voltage_in_kV[2], line_dc_current_in_kA[2];
    private:
        VSC_HVDC_MODEL* hvdc_model;
        AUXILIARY_SIGNAL_MODEL* auxiliary_signal_model;

        bool converter_firing_angle_fixed[2];
        bool temp_converter_firing_angle_fixed[2];
};
#endif // VSC_HVDC_H
