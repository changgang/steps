#ifndef HLCC_H
#define HLCC_H


#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_lcc_model/hlcc_model.h"
#include "header/model/model.h"
#include <complex>
#include <string>

using namespace std;

class BUS;
class DC_BUS;
class HLCC_MODEL;


class HLCC
{
    public:
        HLCC(STEPS& toolkit);
        ~HLCC();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual void report() const;
        virtual void save() const;

        virtual void set_model(HLCC_MODEL* model) ;
        virtual HLCC_MODEL* get_model_of_type(string model_type, unsigned int index=0) ;
        virtual HLCC_MODEL* get_hlcc_model();

        //set
        void set_status(const bool status);
        void set_converter_ac_bus(const unsigned int bus);
        void set_converter_station_number(const unsigned int station);
        void set_converter_dc_bus(const unsigned int bus);
        void set_converter_name(const string name);
        void set_operating_mode(const HLCC_OPERATION_MODE omode);

        void set_brideg_number(const unsigned int nbridge);
        void set_voltage_drop_per_bridge_in_kV(const double vdrop);
        void set_line_smoothing_L_in_H(const double lsmooth);
        void set_line_smoothing_R_in_ohm(const double rsmooth);

        void set_converter_transformer_grid_side_base_voltage_in_kV(const double vbase);
        void set_converter_transformer_converter_side_base_voltage_in_kV(const double vbase);
        void set_converter_transformer_R_in_ohm(const double rt);
        void set_converter_transformer_X_in_ohm(const double xt);
        void set_converter_transformer_Z_in_ohm(const complex<double> zt);
        void set_converter_transformer_tap_in_pu(const double tap);
        void set_converter_transformer_max_tap_in_pu(const double tapmax);
        void set_converter_transformer_min_tap_in_pu(const double tapmin);
        void set_converter_transformer_number_of_taps(const unsigned int ntap);


        void set_firing_angle_in_deg(const double fangle);
        void set_cos_firing_angle(const double cosf);
        void set_max_firing_angle_in_deg(const double maxangle);
        void set_min_firing_angle_in_deg(const double minangle);
        void set_udc_in_kV(const double udc);
        void set_idc_in_kA(const double idc);
        void set_pdc_in_MW(const double pdc);
        void set_command_udc_in_kV(const double udc);
        void set_command_idc_in_kA(const double idc);
        void set_command_pdc_in_MW(const double pdc);
        void set_command_firing_angle(const double fangle);

        void set_converter_P_2_ac_bus(const double Pac);
        void set_converter_Q_2_ac_bus(const double Qac);

        void set_control_mode(const HLCC_CONTROL_MODE mode);

        void set_control_mode_change_flag(bool flag);

        void set_converter_rated_capacity_in_MVA(double Sbase);
        void set_converter_base_voltage_in_kV(double Vbase);
        void set_converter_current_margin_in_pu(double margin);

        //get
        bool get_status() const;

        unsigned int get_converter_ac_bus() const;
        unsigned int get_converter_station_number() const;
        unsigned int get_converter_dc_bus() const;
        string get_converter_name() const;
        HLCC_OPERATION_MODE get_converter_operating_mode() const;

        unsigned int get_brideg_number() const;
        double get_voltage_drop_per_bridge_in_kV() const;
        double get_line_smoothing_L_in_H() const;
        double get_line_smoothing_R_in_ohm() const;

        double get_ac_bus_vlotage_in_kV() const;
        complex<double> get_complex_vac_in_kV() const;

        double get_converter_transformer_grid_side_base_voltage_in_kV() const;
        double get_converter_transformer_converter_side_base_voltage_in_kV() const;
        double get_converter_transformer_R_in_ohm() const;
        double get_converter_transformer_X_in_ohm() const;
        complex<double> get_converter_transformer_Z_in_ohm() const;
        double get_converter_transformer_tap_in_pu() const;
        double get_converter_transformer_max_tap_in_pu() const;
        double get_converter_transformer_min_tap_in_pu() const;
        unsigned int get_converter_transformer_number_of_taps() const;

        double get_firing_angle_in_deg() const;
        double get_cos_firing_angle() const;
        double get_max_firing_angle_in_deg() const;
        double get_min_firing_angle_in_deg() const;
        double get_udc_in_kV() const;
        double get_idc_in_kA() const;
        double get_pdc_in_MW() const;
        double get_command_udc_in_kV() const;
        double get_command_idc_in_kA() const;
        double get_command_pdc_in_MW() const;
        double get_command_firing_angle() const;

        double get_converter_P_2_ac_bus() const;
        double get_converter_Q_2_ac_bus() const;
        complex<double> get_lcc_converter_power_to_ac();

        HLCC_CONTROL_MODE get_control_mode() const;

        bool get_control_mode_changed_flag();
        bool is_converter_firing_angle_fixed();

        double get_converter_rated_capacity_in_MVA() const;
        double get_converter_base_voltage_in_kV() const;
        double get_converter_current_margin_in_pu() const;

        //solve
        void solve_converter_to_DC();
        void solve_converter_to_AC(double udc,double idc);
        void solve_best_transformer_tap_with_min_angle(double vdc, double idc);
        double solve_angle_with_fixed_tap(double vdc, double idc);
        void solve_lcc_with_tap_and_angle(double vdc, double idc);

        double solve_inverter_beta_in_rad_with_gamma_in_deg_and_idc_in_kA(double gamma,double idc);

        //output
        HLCC_MODEL* get_hlcc_model() const;
        double get_converter_dc_power_in_MW();


    public:
        //dynamic
        double get_lcc_converter_ueq_in_kV() const;
        double get_lcc_converter_req_in_ohm() const;

        complex<double> get_converter_dynamic_ac_current_in_kA();
        complex<double> get_converter_dynamic_ac_current_in_pu_on_system_base();

        double get_dynamic_ang_ref_in_deg_of_cc();
        double get_dynamic_ang_ref_in_deg_of_cea();
        double get_dynamic_ang_ref_in_deg_of_cv();
        double get_dynamic_idc_ref_in_kA();


    private://parameters
        STEPS* toolkit;
        unsigned int converter_ac_bus;
        BUS* lcc_ac_busptr;
        unsigned int converter_station_number;
        unsigned int converter_dc_bus;
        DC_BUS* lcc_dc_busptr;
        string converter_name;
        bool status;

        HLCC_OPERATION_MODE operating_mode;//0 Rectifier,1 Inverter
        HLCC_CONTROL_MODE hlcc_control_mode;
        double command_Pdc_in_MW, command_Udc_in_kV, command_Idc_in_kA, command_firing_angle_in_deg;

        unsigned int bridge_number;
        double voltage_drop_per_bridge_in_kV;
        double smoothing_L_in_H, smoothing_R_in_ohm;
        double converter_transformer_grid_side_base_voltage_in_kV, converter_transformer_converter_side_base_voltage_in_kV;
        //double converter_transformer_R_in_ohm, converter_transformer_X_in_ohm;
        complex<double> converter_transformer_Z_in_ohm;
        double converter_transformer_max_tap_in_pu, converter_transformer_min_tap_in_pu;
        unsigned int converter_transformer_number_of_taps;
        double max_firing_angle_in_deg, min_firing_angle_in_deg;

        double converter_transformer_tap_in_pu;
        double firing_angle_in_deg;
        double cos_firing_angle;
        double idc_in_kA, udc_in_kV, pdc_in_MW;
        double P2ac,Q2ac;

        bool is_control_mode_changed;
        bool converter_firing_angle_fixed;

        double Sbase;
        double Vbase;
        double margin;

        HLCC_MODEL* model;

};
#endif // HVDC_H
