#ifndef LCC_HVDC_H
#define LCC_HVDC_H

#include "header/device/nonbus_device.h"
#include "header/device/lcc_station.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/lcc_hvdc2t_model/lcc_hvdc2t_model.h"
#include "header/model/auxiliary_signal_model/auxiliary_signal_model.h"

#include <complex>
#include <string>


using namespace std;

class BUS;

class LCC_HVDC : public NONBUS_DEVICE
{
    public:
        LCC_HVDC(STEPS& toolkit);
        virtual ~LCC_HVDC();

        void set_name(const string name);

        void set_converter_count(CONVERTER_SIDE side, const unsigned int n);
        const LCC* get_immutable_converter(CONVERTER_SIDE side, const unsigned int converter_index) const;
        LCC* get_mutable_converter(CONVERTER_SIDE side, const unsigned int converter_index);
        void set_converter_bus(CONVERTER_SIDE side, const unsigned int converter_index, const unsigned int bus);
        void set_converter_valve_side_bus_name(CONVERTER_SIDE side, const unsigned int converter_index, string name);
        void set_converter_power_percent(CONVERTER_SIDE side, const unsigned int converter_index, double percent);

        void set_line_resistance_in_ohm(const double R);
        void set_line_inductance_in_mH(const double L);
        void set_line_capacitance_in_uF(const double C);
        void set_line_smooting_inductance_in_mH(CONVERTER_SIDE side, const double L);
        void set_line_smooting_resistance_in_ohm(CONVERTER_SIDE side, const double R);
        void set_nominal_dc_power_in_MW(const double P);
        void set_nominal_dc_current_in_kA(const double I);
        void set_nominal_dc_voltage_in_kV(const double V);
        void set_compensating_resistance_to_hold_dc_voltage_in_ohm(const double R);
        void set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(const double V);
        void set_current_power_margin(const double margin);
        void set_meter_end(const CONVERTER_SIDE side);

        void set_operation_mode(const LCC_HVDC_OPERATION_MODE mode);

        void set_converter_number_of_bridge(CONVERTER_SIDE side, const unsigned int converter_index, const unsigned int nbridge);
        void set_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE side, const unsigned int converter_index, const double V);
        void set_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index, const double angle);
        void set_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index, const double angle);

        void set_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index, const double V);
        void set_converter_transformer_valve_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index, const double V);
        void set_converter_transformer_impedance_in_ohm(CONVERTER_SIDE side, const unsigned int converter_index, const complex<double> z);
        void set_converter_transformer_admittance_in_siemens(CONVERTER_SIDE side, const unsigned int converter_index, const complex<double> y);
        void set_converter_transformer_max_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index, const double maxtap);
        void set_converter_transformer_min_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index, const double mintap);
        void set_converter_transformer_number_of_taps(CONVERTER_SIDE side, const unsigned int converter_index, const unsigned int ntap);


        string get_name() const;
        unsigned int get_name_index() const;
        unsigned int get_converter_count(CONVERTER_SIDE side) const;
        unsigned int get_converter_bus(CONVERTER_SIDE side, const unsigned int converter_index) const;
        //BUS* get_converter_bus_pointer(CONVERTER_SIDE side, const unsigned int converter_index) const;
        string get_converter_valve_side_bus_name(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_power_percent(CONVERTER_SIDE side, const unsigned int converter_index) const;

        double get_line_resistance_in_ohm() const;
        double get_line_inductance_in_mH() const;
        double get_line_capacitance_in_uF() const;
        double get_line_smooting_inductance_in_mH(CONVERTER_SIDE side) const;
        double get_line_smooting_resistance_in_ohm(CONVERTER_SIDE side) const;
        double get_nominal_dc_power_in_MW() const;
        double get_nominal_dc_current_in_kA() const;
        double get_nominal_dc_voltage_in_kV() const;
        double get_compensating_resistance_to_hold_dc_voltage_in_ohm() const;
        double get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV() const;
        double get_current_power_margin() const;
        CONVERTER_SIDE get_meter_end() const;

        LCC_HVDC_OPERATION_MODE get_operation_mode() const;

        unsigned int get_converter_number_of_bridge(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_voltage_drop_per_bridge_in_kV(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_max_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_min_alpha_or_gamma_in_deg(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_cos_max_alpha_or_gamma(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_cos_min_alpha_or_gamma(CONVERTER_SIDE side, const unsigned int converter_index) const;

        double get_converter_transformer_grid_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_transformer_valve_side_base_voltage_in_kV(CONVERTER_SIDE side, const unsigned int converter_index) const;
        complex<double> get_converter_transformer_impedance_in_ohm(CONVERTER_SIDE side, const unsigned int converter_index) const;
        complex<double> get_converter_transformer_admittance_in_siemens(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_transformer_max_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index) const;
        double get_converter_transformer_min_tap_in_pu(CONVERTER_SIDE side, const unsigned int converter_index) const;
        unsigned int get_converter_transformer_number_of_taps(CONVERTER_SIDE side, const unsigned int converter_index) const;

        const LCC_STATION& get_station(CONVERTER_SIDE side) const;

        void set_status(const bool status);

        bool get_status() const;
        bool is_blocked() const;

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

        virtual void run(DYNAMIC_MODE mode);

        virtual LCC_HVDC& operator=(const LCC_HVDC& hvdc);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        void solve_steady_state();
        void show_solved_steady_state() const;
    private:
        string get_converter_side_name(CONVERTER_SIDE side) const;
        double get_line_dc_current_in_kA() const;
    private:
        LCC_STATION station[2];
        LCC_HVDC_OPERATION_MODE mode;

        unsigned int name_index;
        bool status;

        double line_R_in_ohm, line_L_in_H, line_C_in_F;
        double smoothing_L_in_H[2], smoothing_R_in_ohm[2];
        double demand_power_in_MW, demand_voltage_in_kV, demand_current_in_kA;

        CONVERTER_SIDE side_to_hold_power_or_current;
        CONVERTER_SIDE side_to_hold_voltage;

        double compensating_R_to_hold_dc_voltage_in_ohm;

        double mode_switching_voltage_in_kV;

        double dc_current_power_margin;

        CONVERTER_SIDE meter_end;
    private:
        //LCC_HVDC_MODEL* hvdc_model;
        //AUXILIARY_SIGNAL_MODEL* auxiliary_signal_model;
};
#endif // LCC_HVDC_H
