#ifndef MT_HVDC_H
#define MT_HVDC_H

#include "header/device/nonbus_device.h"
#include "header/device/dc_component/dc_station.h"
#include "header/device/dc_component/dc_terminal.h"
#include "header/device/dc_component/hlcc.h"
#include "header/device/dc_component/hvsc.h"
#include "header/device/dc_component/dc_bus.h"
#include "header/device/dc_component/dc_line.h"
#include "header/basic/sparse_matrix_define.h"
#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_lcc_model/hlcc_model.h"
#include "header/model/mt_hvdc_model/mt_hvdc_converter_model/mt_hvdc_vsc_model/hvsc_model.h"
#include "header/model/mt_hvdc_model/mt_hvdc_dc_grid_model/mt_dc_grid_model.h"
#include "header/basic/inphno.h"

#include <complex>
#include <string>
#include <istream>
#include <iostream>
#include <cstdio>

using namespace std;

class MT_HVDC_MODEL;


class MT_HVDC : public NONBUS_DEVICE
{
    public:
        MT_HVDC(STEPS& toolkit);
        ~MT_HVDC();

        virtual bool is_connected_to_bus(unsigned int bus) const;
        virtual bool is_lccs_connected_to_bus(unsigned int bus) const;
        virtual bool is_vscs_connected_to_bus(unsigned int bus) const;
        virtual bool is_connected_to_dcbus(unsigned int dcbus) const;
        virtual bool is_in_area(unsigned int area) const;
        virtual bool is_in_zone(unsigned int zone) const;

        virtual bool is_valid() const;
        virtual void check();
        virtual void clear();

        virtual void report() const;
        virtual void save() const;

        virtual DEVICE_ID get_device_id() const;

        string get_identifier() const;
        unsigned int get_identifier_index() const;
        unsigned int get_name_index() const;

        void set_identifier(const string hdc_id);
        void set_name(const string name);
        string get_name() const;

        void set_status(const bool status);
        bool get_status() const;

        //basic set
        void set_dc_terminal_count(unsigned int n);
        void set_dc_station_count(unsigned int n);
        void set_lcc_converter_count(unsigned int n);
        void set_vsc_converter_count(unsigned int n);
        void set_dc_bus_count(unsigned int n);
        void set_dc_line_count(unsigned int n);
        void set_dc_base_voltage(const double vbase);

        void set_dc_system_max_iteration_in_powerflow(const unsigned int n);
        void set_dc_system_allow_power_mismatch_in_powerflow(const double pmis);

        //basic get
        unsigned int get_dc_terminal_count() const;
        unsigned int get_dc_station_count() const;
        unsigned int get_lcc_converter_count() const;
        unsigned int get_vsc_converter_count() const;
        unsigned int get_dc_bus_count() const;
        unsigned int get_dc_line_count() const;
        double get_dc_base_voltage() const;

        unsigned int get_dc_system_max_iteration_in_powerflow() const;
        double get_dc_system_allow_power_mismatch_in_powerflow() const;

        //
        bool dc_terminal_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool dc_station_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool lcc_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool vsc_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool dc_bus_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;
        bool dc_line_index_is_out_of_range_in_function(const unsigned int index, const string& func) const;


        //Terminal set
        void set_terminal_name(const unsigned int index,const string name);
        void set_terminal_number(const unsigned int index,const unsigned int terminal_number);
        void set_terminal_dc_bus_number(const unsigned int index,const unsigned int dcbus);
        void set_termianl_status(const unsigned int index,const bool status);
        void set_terminal_mode(const unsigned int index,const DC_TERMINAL_MODE mode);

        //Terminal get
        vector<DC_TERMINAL*> get_all_dc_terminals();
        DC_TERMINAL* get_dc_terminal_with_name(string name);
        DC_TERMINAL* get_dc_terminal_with_index(unsigned int index);
        DC_TERMINAL* get_dc_terminal_with_terminal_number(unsigned int terminal_number);
        DC_TERMINAL* get_dc_terminal_with_dc_bus_number(unsigned int dc_bus_number);

        string get_terminal_name(unsigned int index) const;
        unsigned int get_terminal_index_with_name(string name);
        unsigned int get_terminal_number(unsigned int index) const;
        unsigned int get_terminal_dc_bus_number(unsigned int index) const;
        bool get_terminal_status(unsigned int index) const;
        DC_TERMINAL_MODE get_terminal_mode(unsigned int index) const;


        //station set
        void set_station_name(unsigned int index,const string name);
        void set_station_number(unsigned int index,const unsigned int station_number);
        void set_station_terminal_number(unsigned int index,const unsigned int terminal_number);
        void set_station_status(unsigned int index,const bool status);

        void set_station_structure(unsigned int index,const DC_STATION_STRUCTURE structure);
        void set_station_mode(unsigned int index,const DC_STATION_MODE mode);
        void set_station_cascade_lcc_number(unsigned int index,const unsigned int n_lcc);
        void set_station_parallel_vsc_number(unsigned int index,const unsigned int n_vsc);

        //station get
        vector<DC_STATION*> get_all_dc_stations();
        vector<DC_STATION*> get_dc_stations_connect_to_terminal(unsigned int terminal_number);
        DC_STATION* get_dc_station_with_station_number(unsigned int station_number);
        DC_STATION* get_dc_station_with_name(string name);
        DC_STATION* get_dc_station_with_index(unsigned int index);
        string get_station_name(unsigned int index) const;
        unsigned int get_station_index_with_name(string name) const;
        unsigned int get_station_number(unsigned int index) const;
        unsigned int get_station_terminal_number(unsigned int index) const;
        bool get_station_status(unsigned int index) const;

        DC_STATION_STRUCTURE get_station_structure(unsigned int index) const;
        DC_STATION_MODE get_station_mode(unsigned int index) const;
        unsigned int get_station_cascade_lcc_number(unsigned int index) const;
        unsigned int get_station_parallel_vsc_number(unsigned int index) const;

        //lcc set
        void set_lcc_converter_status(const unsigned int index,const bool status);
        void set_lcc_converter_ac_bus(const unsigned int index,const unsigned int bus);
        void set_lcc_converter_station_number(const unsigned int index,const unsigned int station);

        void set_lcc_converter_dc_bus(const unsigned int index,const unsigned int bus);
        void set_lcc_converter_name(const unsigned int index,const string name);
        void set_lcc_converter_operating_mode(const unsigned int index,const HLCC_OPERATION_MODE omode);

        void set_lcc_converter_brideg_number(const unsigned int index,const unsigned int nbridge);
        void set_lcc_converter_voltage_drop_per_bridge_in_kV(const unsigned int index,const double vdrop);
        void set_lcc_converter_line_smoothing_L_in_H(const unsigned int index,const double lsmooth);
        void set_lcc_converter_line_smoothing_R_in_ohm(const unsigned int index,const double rsmooth);

        void set_lcc_converter_transformer_grid_side_base_voltage_in_kV(const unsigned int index,const double vbase);
        void set_lcc_converter_transformer_converter_side_base_voltage_in_kV(const unsigned int index,const double vbase);
        void set_lcc_converter_transformer_R_in_ohm(const unsigned int index,const double rt);
        void set_lcc_converter_transformer_X_in_ohm(const unsigned int index,const double xt);
        void set_lcc_converter_transformer_Z_in_ohm(const unsigned int index,const complex<double> zt);
        void set_lcc_converter_transformer_tap_in_pu(const unsigned int index,const double tap);
        void set_lcc_converter_transformer_max_tap_in_pu(const unsigned int index,const double tapmax);
        void set_lcc_converter_transformer_min_tap_in_pu(const unsigned int index,const double tapmin);
        void set_lcc_converter_transformer_number_of_taps(const unsigned int index,const unsigned int ntap);


        void set_lcc_converter_firing_angle_in_deg(const unsigned int index,const double fangle);
        void set_lcc_converter_max_firing_angle_in_deg(const unsigned int index,const double maxangle);
        void set_lcc_converter_min_firing_angle_in_deg(const unsigned int index,const double minangle);
        void set_lcc_converter_udc_in_kV(const unsigned int index,const double udc);
        void set_lcc_converter_idc_in_kA(const unsigned int index,const double idc);
        void set_lcc_converter_pdc_in_MW(const unsigned int index,const double pdc);
        void set_lcc_converter_command_udc_in_kV(const unsigned int index,const double udc);
        void set_lcc_converter_command_idc_in_kA(const unsigned int index,const double idc);
        void set_lcc_converter_command_pdc_in_MW(const unsigned int index,const double pdc);
        void set_lcc_converter_command_firing_angle(const unsigned int index,const double fangle);

        void set_lcc_converter_P_2_ac_bus(const unsigned int index,const double Pac);
        void set_lcc_converter_Q_2_ac_bus(const unsigned int index,const double Qac);
        void set_lcc_converter_rated_capacity_in_MVA(const unsigned int index,const double Sbase);
        void set_lcc_converter_base_voltage_in_kV(const unsigned int index,const double Vbase);
        void set_lcc_converter_current_margin_in_pu(const unsigned int index,const double magrin);

        void set_lcc_converter_control_mode(const unsigned int index,const HLCC_CONTROL_MODE mode);


        //lcc get
        vector<HLCC*> get_all_lcc_converters();
        vector<HLCC*> get_lcc_converters_connect_to_ac_bus(unsigned int ac_bus);
        HLCC* get_lcc_converter_with_name(string name);
        vector<HLCC*> get_angle_control_lcc_converters();
        vector<HLCC*> get_lcc_converters_connect_to_station(unsigned int station_number);
        HLCC* get_lcc_converter_with_index(unsigned int index);
        HLCC* get_lcc_converter_with_dc_bus_number(unsigned int dc_bus);
        unsigned int get_lcc_angle_control_number_of_hcm(unsigned int station_number);
        HLCC* get_lcc_converter_angle_fixed_of_HCM_station(unsigned int station_number);
        HLCC* get_lcc_converter_angle_slack_of_HCM_station(unsigned int station_number);

        string get_lcc_converter_name(unsigned int index) const;
        unsigned int get_lcc_converter_index_with_name(string name) const;

        bool get_lcc_converter_status(unsigned int index) const;
        unsigned int get_lcc_converter_ac_bus(unsigned int index) const;
        unsigned int get_lcc_converter_station_number(unsigned int index) const;
        unsigned int get_lcc_converter_dc_bus(unsigned int index) const;
        HLCC_OPERATION_MODE get_lcc_converter_operating_mode(unsigned int index);

        unsigned int get_lcc_converter_brideg_number(unsigned int index) const;
        double get_lcc_converter_voltage_drop_per_bridge_in_kV(unsigned int index) const;
        double get_lcc_converter_line_smoothing_L_in_H(unsigned int index) const;
        double get_lcc_converter_line_smoothing_R_in_ohm(unsigned int index) const;


        double get_lcc_converter_ac_bus_vlotage(unsigned int index) const;
        complex<double> get_lcc_converter_complex_vac_in_kV(unsigned int index) const;
        double get_lcc_converter_transformer_grid_side_base_voltage_in_kV(unsigned int index) const;
        double get_lcc_converter_transformer_converter_side_base_voltage_in_kV(unsigned int index) const;
        double get_lcc_converter_transformer_R_in_ohm(unsigned int index) const;
        double get_lcc_converter_transformer_X_in_ohm(unsigned int index) const;
        complex<double> get_lcc_converter_transformer_Z_in_ohm(unsigned int index) const;
        double get_lcc_converter_transformer_tap_in_pu(unsigned int index) const;
        double get_lcc_converter_transformer_max_tap_in_pu(unsigned int index) const;
        double get_lcc_converter_transformer_min_tap_in_pu(unsigned int index) const;
        unsigned int get_lcc_converter_transformer_number_of_taps(unsigned int index) const;


        double get_lcc_converter_firing_angle_in_deg(unsigned int index) const;
        double get_lcc_converter_cos_firing_angle(unsigned int index) const;
        double get_lcc_converter_max_firing_angle_in_deg(unsigned int index) const;
        double get_lcc_converter_min_firing_angle_in_deg(unsigned int index) const;
        double get_lcc_converter_udc_in_kV(unsigned int index) const;
        double get_lcc_converter_idc_in_kA(unsigned int index) const;
        double get_lcc_converter_pdc_in_MW(unsigned int index) const;
        double get_lcc_converter_command_udc_in_kV(unsigned int index) const;
        double get_lcc_converter_command_idc_in_kA(unsigned int index) const;
        double get_lcc_converter_command_pdc_in_MW(unsigned int index) const;
        double get_lcc_converter_command_firing_angle(unsigned int index) const;

        double get_lcc_converter_P_2_ac_bus(unsigned int index) const;
        double get_lcc_converter_Q_2_ac_bus(unsigned int index) const;


        HLCC_CONTROL_MODE get_lcc_converter_control_mode(unsigned int index) const;

        //set vsc
        void set_vsc_converter_ac_bus(const unsigned int index,const unsigned int acbus);
        void set_vsc_converter_station_number(const unsigned int index,const unsigned int station);
        void set_vsc_converter_dc_bus(const unsigned int index,const unsigned int dcbus);
        void set_vsc_converter_name(const unsigned int index,const string name);
        void set_vsc_converter_status(const unsigned int index, const bool status);
        void set_vsc_converter_priority(const unsigned int index,unsigned int priority);
        void set_vsc_converter_d_axis_control_mode(const unsigned int index,const HVSC_D_CONTROL_MODE dmode);
        void set_vsc_converter_q_axis_control_mode(const unsigned int index,const HVSC_Q_CONTROL_MODE qmode);
        void set_vsc_converter_pac_command(const unsigned int index,const double pac);
        void set_vsc_converter_qac_command(const unsigned int index,const double qac);
        void set_vsc_converter_udc_command(const unsigned int index,const double udc);
        void set_vsc_converter_us_command(const unsigned int index,const double us);
        void set_vsc_converter_loss_coefficient_A(const unsigned int index,const double A);
        void set_vsc_converter_loss_coefficient_B(const unsigned int index,const double B);

        void set_vsc_converter_loss_coefficient_C(const unsigned int index,const double C);
        void set_vsc_converter_min_loss_in_kW(const unsigned int index,const double min_loss);
        void set_vsc_converter_Z_in_ohm(const unsigned int index,const complex<double> Z);

        void set_vsc_converter_P_to_ac_bus_in_MW(const unsigned int index,const double P2ac);
        void set_vsc_converter_Q_to_ac_bus_in_Mvar(const unsigned int index,const double Q2ac);

        void set_vsc_converter_rated_capacity_in_MVA(const unsigned int index,const double Sbase);
        void set_vsc_converter_base_voltage_in_kV(const unsigned int index,const double vbase);

        //get vsc
        string get_vsc_converter_name(unsigned int index) const;
        vector<HVSC*> get_all_vsc_converters();
        vector<HVSC*> get_vsc_converters_connect_to_ac_bus(unsigned int ac_bus);
        HVSC* get_vsc_converter_with_name(string name);
        HVSC* get_vsc_converter_with_index(unsigned int index);
        vector<HVSC*> get_vsc_converters_connect_to_station(unsigned int station_number);
        HVSC* get_current_voltage_vsc_converter(unsigned int station_number);
        vector<HVSC*> get_vsc_connected_to_ac_bus(unsigned int bus);
        unsigned int get_vsc_converter_ac_bus(unsigned int index) const;


        //set dcbus
        void set_dc_bus_status(const unsigned int index,const bool status);
        void set_dc_bus_name(const unsigned int index,const string name);
        void set_dc_bus_nominal_voltage(const unsigned int index,const double vn);
        void set_dc_bus_number(const unsigned int index,const unsigned int number);

        void set_dc_bus_voltage_in_kV(const unsigned int index,const double vdc);
        void set_dc_bus_current_in_kA(const unsigned int index, double idc);
        void set_dc_bus_power_in_MW(const unsigned int index,const double pdc);
        void set_dc_bus_area(const unsigned int index,const unsigned int area);
        void set_dc_bus_zone(const unsigned int index,const unsigned int zone);

        //get dcbus
        vector<DC_BUS*> get_all_dc_buses();
        DC_BUS* get_dc_bus_with_index(unsigned int index);
        DC_BUS* get_dc_bus_with_name(string name);
        DC_BUS* get_dc_bus_with_bus_number(unsigned int number);
        DC_BUS* get_dc_bus_with_terminal_number(unsigned int terminal_number);

        unsigned int get_number_of_P_node();
        unsigned int get_number_of_I_node();
        unsigned int get_number_of_angle_node();
        unsigned int get_number_of_P_and_I_node();
        //unsigned int get_number_of_vsc_P_node();

        bool get_dc_bus_status(unsigned int index) const;
        string get_dc_bus_name(unsigned int index) const;
        unsigned int get_dc_bus_index_with_name(string name) const;
        unsigned int get_dc_bus_number(unsigned int index) const;
        double get_dc_bus_nominal_voltage(unsigned int index) const;

        double get_dc_bus_voltage_in_kV(unsigned int index) const;
        double get_dc_bus_current_in_kA(unsigned int index) const;
        double get_dc_bus_power_in_MW(unsigned int index) const;
        unsigned int get_dc_bus_area(unsigned int index) const;
        unsigned int get_dc_bus_zone(unsigned int index) const;


        //set dcline
        void set_dc_line_sending_side_bus(const unsigned int index,const unsigned int bus);
        void set_dc_line_receiving_side_bus(const unsigned int index,const unsigned int bus);
        void set_dc_line_name(const unsigned int index,const string name);
        void set_dc_line_status(const unsigned int index,const bool status);
        void set_dc_line_R_in_ohm(const unsigned int index,const double r);
        void set_dc_line_L_in_H(const unsigned int index,const double l);
        void set_dc_line_C_in_uF(const unsigned int index,const double c);

        void set_dc_line_current(const unsigned int index,const double idc);
        void set_dc_line_sengding_side_p(const unsigned int index,const double psend);
        void set_dc_line_receiving_side_p(const unsigned int index,const double prec);

        //get dcline
        vector<DC_LINE*> get_all_dc_lines();
        DC_LINE* get_dc_line_with_line_name(string name);
        DC_LINE* get_dc_line_with_index(unsigned int index);
        DC_LINE* get_dc_line_with_ibus_and_jbus(unsigned int ibus, unsigned int jbus);
        vector<DC_LINE*> get_dc_lines_with_ibus(unsigned int ibus);
        vector<DC_LINE*> get_dc_lines_with_jbus(unsigned int jbus);

        unsigned int get_dc_line_sending_side_bus_number(unsigned int index) const;
        unsigned int get_dc_line_receiving_side_bus_number(unsigned int index) const;
        string get_dc_line_name(unsigned int index) const;
        unsigned int get_dc_line_index_with_name(string name);
        bool get_dc_line_status(unsigned int index) const;
        double get_dc_line_R_in_ohm(unsigned int index) const;
        double get_dc_line_L_in_H(unsigned int index) const;

        double get_dc_line_current(unsigned int index) const;
        double get_dc_line_sending_side_p(unsigned int index) const;
        double get_dc_line_receiving_side_p(unsigned int index) const;

        //solve
        void solve_steady_state();
        bool solve_dc_network();
        void solve_converters();

        void initialize_dc_power_flow();
        void initialize_dc_bus_type();
        void initialize_dc_power();
        void initialize_dc_voltage();
        void initialize_vsc_converters();
        void initialize_lcc_converters();

        double get_active_power_of_power_terminal(unsigned int terminal_number);
        double get_active_power_of_power_station(unsigned int station_number);
        double get_current_of_current_terminal(unsigned int terminal_number);
        double get_current_of_current_station(unsigned int station_number);
        double get_voltage_of_voltage_terminal(unsigned int terminal_number);
        double get_voltage_of_voltage_station(unsigned int station_number);
        double get_active_power_of_voltage_vsc_stations(unsigned int station_number);
        double get_active_power_of_angle_terminal(unsigned int terminal_number);
        double get_active_power_of_HCM_terminal_with_1_angle_fixed(unsigned int station_number);
        double get_active_power_of_HCM_terminal_with_2_angle_fixed(unsigned int station_number);
        double get_jacobian_element_of_angle_terminal(unsigned int terminal_number);
        double get_jacobian_element_of_HCM_terminal_with_1_angle_fixed(unsigned int station_number);
        double get_jacobian_element_of_HCM_terminal_with_2_angle_fixed(unsigned int station_number);

        void initialize_physical_internal_bus_pair();
        void build_dc_network_matrix();
        void build_dc_network_initial_zero_matrix();
        void add_dc_line_to_dc_network_matrix();
        void show_dc_network_matrix();

        unsigned int get_number_of_dc_u_bus();
        void calculate_raw_power_mismatch();
        void calculate_current_to_dc();
        void calculate_power_to_dc();
        double get_dc_power_from_terminal(unsigned int bus_number);
        double get_max_active_power_mismatch_in_MW();

        void build_jacobian();
        double calculate_jacobian_element(unsigned int row, unsigned int col);
        bool is_dc_network_matrix_set();
        void show_jacobian_matrix();

        void solve_converters_of_P_terminal(unsigned int terminal_number);
        void solve_converters_of_I_terminal(unsigned int terminal_number);
        void solve_converters_of_U_terminal(unsigned int terminal_number);
        void solve_converters_of_ANGLE_terminal(unsigned int terminal_number);

        bool check_control_mode_change_flag();

        STEPS_SPARSE_MATRIX get_dc_network_matrix_without_dynamic();
        INPHNO get_dc_network_internal_physical_number_inphno();

        void show_bus_voltage();
        void update_bus_current();
        void update_bus_power();
        void show_converter_power();

        //dynamic
        virtual void set_model(MODEL* model);
        virtual void set_lcc_converter_model(HLCC_MODEL* model);
        virtual void set_vsc_converter_model(HVSC_MODEL* model);
        virtual void set_dc_network_model(MT_DC_GRID_MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);
        virtual MT_DC_GRID_MODEL* get_mtdc_network_model();
        vector<HLCC_MODEL*> get_all_hlcc_models();
        vector<HVSC_MODEL*> get_all_hvsc_models();
        HLCC_MODEL* get_lcc_model_with_index(unsigned int index);
        HLCC_MODEL* get_lcc_model_with_name(string name);
        HVSC_MODEL* get_vsc_model_with_index(unsigned int index);
        HVSC_MODEL* get_vsc_model_with_index(string name);



        void initialize_vsc_station_udc_block(unsigned int station_number);
        void update_vsc_station_dynamic_ueq_input(unsigned int station_number);
        double get_dc_bus_ueq_in_dynamic_simulation(unsigned int dc_bus);
        double get_dc_bus_req_in_dynamic_simulation(unsigned int dc_bus);

        void run(DYNAMIC_MODE mode);
        void solve_dynamic_dc_systems();
        void update_converters_in_dynamic_simulation();
        void update_lcc_station_in_dynamic_simulation(unsigned int station_number,double Idc);
        void update_vsc_station_in_dynamic_simulation(unsigned int station_number,double Idc);

    private:
        unsigned int identifier_index;
        unsigned int name_index;
        bool status;

        double dc_base_voltage;

        vector<DC_STATION> dc_stations;
        vector<DC_TERMINAL> dc_terminals;
        vector<HLCC> Hlcc;
        vector<HVSC> Hvsc;
        vector<DC_BUS> Dcbus;
        vector<DC_LINE> Dcline;

        vector<double> i2dc, p2dc;

        STEPS_SPARSE_MATRIX dc_network_matrix;
        STEPS_SPARSE_MATRIX jacobian;
        INPHNO inphno;

        vector<double>bus_current;
        vector<double>bus_power;
        vector<double>p_mismatch;
        vector<double>u_mismactch;


        vector<double>temp_p_mismatch;
        vector<double>temp_u_mismactch;

        unsigned int iteration_count, max_iteration;
        double allowed_power_mismatch;
        MT_DC_GRID_MODEL* mtdc_network_model;

};
#endif // MT_HVDC_H
