#ifndef VSC_HVDC_H
#define VSC_HVDC_H

#include "header/device/device.h"
#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_model.h"
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
        void set_identifier(const string identifier);
        void set_name(const string name);
        void set_status(const bool status);
        void set_line_resistance_in_ohm(const double R);

        void set_converter_dc_operation_mode(HVDC_CONVERTER_SIDE converter, const VSC_HVDC_DC_CONTROL_MODE mode);
        void set_converter_ac_operation_mode(HVDC_CONVERTER_SIDE converter, const VSC_HVDC_AC_CONTROL_MODE mode);

        void set_converter_nominal_dc_power_command_in_MW(HVDC_CONVERTER_SIDE converter, double P);
        void set_converter_nominal_dc_voltage_command_in_kV(HVDC_CONVERTER_SIDE converter, double V);

        void set_converter_nominal_ac_voltage_command_in_pu(HVDC_CONVERTER_SIDE converter, double V);
        void set_converter_nominal_ac_power_factor_command(HVDC_CONVERTER_SIDE converter, double pf);

        void set_converter_loss_factor_A_in_kW(HVDC_CONVERTER_SIDE converter, double A);
        void set_converter_loss_factor_B_in_kW_per_amp(HVDC_CONVERTER_SIDE converter, double B);
        void set_converter_minimum_loss_in_kW(HVDC_CONVERTER_SIDE converter, double P);

        void set_converter_MVA_rating_in_MVA(HVDC_CONVERTER_SIDE converter, double S);
        void set_converter_current_rating_in_amp(HVDC_CONVERTER_SIDE converter, double I);

        void set_converter_power_weighting_factor(HVDC_CONVERTER_SIDE converter, double pwf);
        void set_converter_Qmax_in_MVar(HVDC_CONVERTER_SIDE converter, double Q);
        void set_converter_Qmin_in_MVar(HVDC_CONVERTER_SIDE converter, double Q);

        void set_converter_remote_bus_to_regulate(HVDC_CONVERTER_SIDE converter, size_t bus);
        void set_converter_remote_regulation_percent(HVDC_CONVERTER_SIDE converter, double rmpct);

        size_t get_converter_bus(HVDC_CONVERTER_SIDE converter) const;
        BUS* get_converter_bus_pointer(HVDC_CONVERTER_SIDE converter) const;

        string get_identifier() const;
        string get_name() const;
        bool get_status() const;

        double get_line_resistance_in_ohm() const;

        VSC_HVDC_DC_CONTROL_MODE get_converter_dc_operation_mode(HVDC_CONVERTER_SIDE converter) const;
        VSC_HVDC_AC_CONTROL_MODE get_converter_ac_operation_mode(HVDC_CONVERTER_SIDE converter) const;

        double get_converter_nominal_dc_power_command_in_MW(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_nominal_dc_voltage_command_in_kV(HVDC_CONVERTER_SIDE converter) const;

        double get_converter_nominal_ac_voltage_command_in_pu(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_nominal_ac_power_factor_command(HVDC_CONVERTER_SIDE converter) const;

        double get_converter_loss_factor_A_in_kW(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_loss_factor_B_in_kW_per_amp(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_minimum_loss_in_kW(HVDC_CONVERTER_SIDE converter) const;

        double get_converter_MVA_rating_in_MVA(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_current_rating_in_amp(HVDC_CONVERTER_SIDE converter) const;

        double get_converter_power_weighting_factor(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_Qmax_in_MVar(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_Qmin_in_MVar(HVDC_CONVERTER_SIDE converter) const;

        size_t get_converter_remote_bus_to_regulate(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_remote_regulation_percent(HVDC_CONVERTER_SIDE converter) const;

    public:
        void initialize_dc_power_and_voltage_command();

        void set_nominal_dc_current_command_in_kA(double I);
        void set_converter_actual_dc_power_command_in_MW(HVDC_CONVERTER_SIDE converter, double P);
        void set_converter_actual_dc_voltage_command_in_kV(HVDC_CONVERTER_SIDE converter, double V);
        void set_actual_dc_current_command_in_kA(double I);
        void set_converter_actual_ac_power_factor_command(HVDC_CONVERTER_SIDE converter, double eta);

        double get_nominal_dc_current_command_in_kA() const;
        double get_converter_actual_dc_power_command_in_MW(HVDC_CONVERTER_SIDE converter) const;
        double get_converter_actual_dc_voltage_command_in_kV(HVDC_CONVERTER_SIDE converter) const;
        double get_actual_dc_current_command_in_kA() const;
        double get_converter_actual_ac_power_factor_command(HVDC_CONVERTER_SIDE converter) const;

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
        virtual MODEL* get_model_of_type(string model_type);

        void set_vsc_hvdc_model(VSC_HVDC_MODEL* model);
        void set_auxiliary_signal_model(AUXILIARY_SIGNAL_MODEL* model);

        VSC_HVDC_MODEL* get_vsc_hvdc_model() const;
        AUXILIARY_SIGNAL_MODEL* get_auxiliary_signal_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual VSC_HVDC& operator=(const VSC_HVDC& hvdc);

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        void calculate_converter_power_factor_in_ac_voltage_control_mode(HVDC_CONVERTER_SIDE converter, double Qerror, double Qderivative);
        void solve_steady_state();
        void show_solved_hvdc_steady_state() const;

        complex<double> get_converter_dynamic_current_in_pu_based_on_system_base_power(HVDC_CONVERTER_SIDE converter);
    private:
        string get_converter_side_name(HVDC_CONVERTER_SIDE converter) const;

        size_t converter_bus[2];
        BUS* converter_busptr[2];

        string identifier;
        string vsc_hvdc_name;
        bool status;
        double line_R_in_ohm;

        VSC_HVDC_DC_CONTROL_MODE dc_control_mode[2];
        VSC_HVDC_AC_CONTROL_MODE ac_control_mode[2];

        double nominal_dc_voltage_in_kV[2], nominal_dc_power_in_MW[2], nominal_dc_current_in_amp;
        double nominal_ac_voltage_in_pu[2], nominal_ac_power_factor[2];

        double converter_loss_coefficient_A_in_kW[2], converter_loss_coefficient_B_in_kW_per_amp[2], min_converter_loss_in_kW[2];

        double converter_MVA_rating_in_MVA[2], converter_current_rating_in_amp[2];

        double power_weighting_factor[2];
        double Qmax_MVar[2], Qmin_MVar[2];

        size_t remote_bus_to_regulate[2];
        double remote_regulation_percent[2];
    private:
        bool nominal_dc_setpoints_initialized;
        double actual_dc_power_command_MW[2], actual_dc_voltage_command_kV[2], actual_dc_current_command_in_amp, actual_ac_power_factor_command[2];
    private:
        VSC_HVDC_MODEL* vsc_hvdc_model;
        AUXILIARY_SIGNAL_MODEL* auxiliary_signal_model;
};
#endif // VSC_HVDC_H
