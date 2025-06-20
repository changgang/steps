#ifndef HVSC_H
#define HVSC_H

#include "header/basic/device_id.h"
#include "header/basic/steps_enum.h"
#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_vsc_model/hvsc_model.h"
#include <complex>
#include <string>

using namespace std;

class BUS;
class DC_BUS;
class HVSC_MODEL;


class HVSC
{
    public:
        HVSC(STEPS& toolkit);
        ~HVSC();
        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;
        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();
        virtual void report() const;
        virtual void save() const;
        virtual DEVICE_ID get_device_id() const;

        //set
        void set_converter_ac_bus(const unsigned int acbus);
        void set_converter_station_number(const unsigned int station);
        void set_converter_dc_bus(const unsigned int dcbus);
        void set_converter_name(const string name);
        void set_converter_status(const bool status);
        void set_priority(const unsigned int priority);
        void set_converter_d_axis_control_mode(const HVSC_D_CONTROL_MODE dmode);
        void set_converter_q_axis_control_mode(const HVSC_Q_CONTROL_MODE qmode);
        void set_converter_loss_coefficient_A_in_kW(const double a);
        void set_converter_loss_coefficient_B_in_kW_per_amp(const double b);
        void set_converter_loss_coefficient_C_in_kW_per_amp_squard(const double c);
        void set_converter_min_loss_in_kW(const double min_loss);
        void set_converter_commutating_impedance(const complex<double> Z);
        void set_converter_commutating_impedance_RC(const double R);
        void set_converter_commutating_impedance_XC(const double X);
        void set_converter_transformer_base_voltage_grid_side(const double vbase);
        void set_converter_transformer_base_voltage_converter_side(const double vbase);
        void set_udc_in_kV(const double udc);
        void set_idc_in_kA(const double idc);

        void set_converter_P_to_AC_bus_MW(const double P2AC);
        void set_converter_Q_to_AC_bus_Mvar(const double Q2AC);
        void set_converter_P_to_DC_bus_MW(const double P2DC);
        void set_converter_Ploss_MW(const double Ploss);

        void set_converter_Pac_command(const double pac);
        void set_converter_Udc_command(const double udc);
        void set_converter_Qac_command(const double qac);
        void set_converter_Us_command(const double us);

        void set_converter_rated_capacity_in_MVA(const double Sbase);
        void set_converter_base_voltage_in_kV(const double Vbase);

        //
        void set_max_iteration_of_voltage_station_solution(const unsigned int n);
        void set_allowed_max_p_in_MW_of_voltage_station_solution(const double p);


        //get
        unsigned int get_converter_ac_bus() const;
        BUS* get_converter_ac_bus_pointer() const;

        unsigned int get_converter_station_number() const;
        unsigned int get_converter_dc_bus() const;
        string get_converter_name() const;
        bool get_status() const;
        unsigned int get_priority() const;
        HVSC_D_CONTROL_MODE get_converter_d_axis_control_mode() const;
        HVSC_Q_CONTROL_MODE get_converter_q_axis_control_mode() const;
        double get_converter_loss_coefficient_A_in_kW() const;
        double get_converter_loss_coefficient_B_in_kW_per_amp() const;
        double get_converter_loss_coefficient_C_in_kW_per_amp_squard() const;
        double get_converter_min_loss_in_kW() const;
        complex<double> get_converter_commutating_impedance() const;
        double get_converter_commutating_impedance_RC() const;
        double get_converter_commutating_impedance_XC() const;

        double get_converter_P_to_AC_bus_MW() const;
        double get_converter_Q_to_AC_bus_Mvar() const;
        double get_converter_P_to_DC_bus_MW() const;
        double get_converter_Ploss_MW();
        complex<double> get_converter_AC_bus_compelx_voltage_in_kV() const;
        double get_converter_AC_bus_voltage_in_kV() const;
        double get_converter_AC_bus_voltage_in_pu() const;
        double get_converter_transformer_base_voltage_grid_side() const;
        double get_converter_transformer_base_voltage_converter_side() const;
        double get_udc_in_kV() const;
        double get_idc_in_kA() const;

        double get_converter_Pac_command() const;
        double get_converter_Udc_command() const;
        double get_converter_Qac_command() const;
        double get_converter_Us_command() const;
        double get_converter_nominal_ac_voltage_to_regulate() const;

        double get_converter_rated_capacity_in_MVA() const;
        double get_converter_base_voltage_in_kV() const;

        //
        unsigned int get_max_iteration_of_voltage_station_solution() const;
        double get_allowed_max_p_in_MW_of_voltage_station_solution() const;

        //solve
        void initialize_power_stations();
        void update_converter_ac_variables();

        double solve_dc_power_of_p_control(double P, double Q);
        double solve_ac_active_power_of_udc_control(double Pdc);
        double solve_Pac_with_negetaive_Pdc(double Pdc);
        double solve_Pac_with_positive_Pdc(double Pdc);

        double get_converter_pdc_in_MW();
        //dynamic
        void set_model(HVSC_MODEL* model) ;
        HVSC_MODEL* get_model_of_type(string model_type, unsigned int index=0) ;
        HVSC_MODEL* get_hvsc_model();

        complex<double> get_converter_initial_power_of_converter_AC_side();
        complex<double> get_converter_initial_ac_current_in_kA_in_xy_axis();
        complex<double> get_converter_initial_ac_current_in_pu_on_system_base_in_xy_axis();
        complex<double> get_converter_initial_ac_current_in_pu_on_converter_base_in_xy_axis();
        complex<double> get_converter_initial_ac_current_in_pu_on_system_base_in_dq_axis();
        complex<double> get_converter_initial_ac_current_in_pu_on_converter_base_in_dq_axis();
        double get_initial_pll_angle_in_rad();
        double get_initial_pll_angle_in_deg();

    public:
        //dynamic
        double get_vsc_converter_ueq_in_kV() const;
        double get_vsc_converter_req_in_ohm() const;
        void set_converter_Pdc_command_in_MW(double Pdc);
        double get_converter_Pdc_command_in_MW();

        complex<double> get_converter_dynamic_ac_current_in_kA();
        complex<double> get_converter_dynamic_ac_current_in_pu_on_system_base();
        complex<double> get_converter_dynamic_complex_power_in_MVA();
        double get_converter_dynamic_active_power_in_MW();
        double get_converter_dynamic_reactive_power_in_Mvar();
        double get_converter_dynamic_d_axis_current_in_pu();
        double get_converter_dynamic_q_axis_current_in_pu();
    private://parameters
        STEPS* toolkit;

        unsigned int converter_ac_bus;
        unsigned int converter_station_number;
        unsigned int converter_dc_bus;
        BUS* converter_busptr;

        string name;
        bool status;
        unsigned int priority;
        HVSC_D_CONTROL_MODE d_control_mode;
        HVSC_Q_CONTROL_MODE q_control_mode;

        double converter_loss_coefficient_A_in_kW;
        double converter_loss_coefficient_B_in_kW_per_amp;
        double converter_loss_coefficient_C_in_kW_per_amp_squard;
        double min_converter_loss_in_kW;

        double converter_rated_capacity_in_MVA;
        double converter_base_voltage_in_kV;
        double converter_rated_current_in_amp;

        complex<double> converter_commutating_impedance;

        double P_to_AC_bus_MW;
        double Q_to_AC_bus_MVar;
        double P_to_DC_bus_MW;
        double Ploss_MW;
        complex<double> Us_in_kV;
        double Udc_in_kV;
        double Idc_in_kA;

        double Pmax_MW;
        double Pmin_MW;
        double Qmax_MVar;
        double Qmin_MVar;
        double Udmax_kV;
        double Udmin_kV;

        double Pac_command_in_MW,Udc_command_in_kV;
        double Qac_command_in_Mvar,Us_command_in_kV;
        double Pdc_command;
        //
        unsigned int max_iteration_of_voltage_station_solution;
        double allowed_max_p_in_MW_of_voltage_station_solution;

    private:
        HVSC_MODEL* hvsc_model;


};
#endif // HVSC_H
