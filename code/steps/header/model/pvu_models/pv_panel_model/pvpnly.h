#ifndef PVPNLY_H
#define PVPNLY_H

#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include "header/block/integral_block.h"
#include "header/block/pi_block.h"
#include <cstdlib>

enum PV_DC_VOLTAGE_MODE
{
    PV_UNDERVOLTAGE_MODE = -1,
    PV_MPPT_MODE = 0,
    PV_OVERVOLTAGE_MODE = 1
};

class PVPNLY : public PV_PANEL_MODEL
{
    public:
        PVPNLY(STEPS& toolkit);
        PVPNLY(const PVPNLY& model);
        virtual ~PVPNLY();
        virtual PVPNLY& operator=(const PVPNLY& model);
    public:

        void initialize_S0_in_Wpm2_and_Vdc_in_V_per_solar_cell_with_mppt_mode(double T);
        void initialize_Vdc_in_V_per_solar_cell_with_under_or_over_Vdc_mode(double S, double T, double P);

        void set_initial_Vdc_per_solar_cell_in_V(double v);
        double get_initial_Vdc_per_solar_cell_in_V() const;
        double get_initial_Vdc_in_kV_per_pv_unit();
        double get_Vdc_in_kV_per_pv_unit();
        double get_Vdc_per_solar_cell_in_V();

        double get_derivative_of_power_extracted_power_from_irradiance_over_dc_voltage(double V, double S, double T);
        double get_Ipv_per_solar_cell_in_A(double V, double S, double T);
        double get_Ipv_in_kA_per_pv_unit(double V, double S, double T);
        double get_power_extracted_power_from_irradiance_per_solar_cell_in_W(double V, double S, double T);
        double get_maximum_available_power_extracted_power_from_irradiance_per_solar_cell_in_W(double S, double T);
        double get_Vdc_at_mppt_per_solar_cell_in_V(double S, double T);
        double get_Vdc_reference_per_solar_cell_in_V_without_limit(double S, double T, double P);
        double get_Vdc_reference_per_solar_cell_in_V(double S, double T, double P);
        double get_Vdc_reference_in_kV_per_pv_unit();

        void update_current_Vdc_at_mppt_per_solar_cell_in_V(double S, double T);
        void update_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(double P);

        void set_current_S_in_Wpm2_and_T_in_C_deg(double S, double T);
        void set_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode(double value);
        void set_current_pelec_including_loss_per_solar_cell_in_W(double value);
        void set_current_Vdc_at_mppt_per_solar_cell_in_V(double value);
        void set_current_Ipv_per_solar_cell_in_A(double value);

        double get_current_S_in_Wpm2() const;
        double get_current_T_in_C_deg() const;
        double get_current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode() const;
        double get_current_pelec_including_loss_per_solar_cell_in_W() const;
        double get_current_Vdc_at_mppt_per_solar_cell_in_V() const;
        double get_current_Ipv_per_solar_cell_in_A() const;

        void set_dc_voltage_control_mode(PV_DC_VOLTAGE_MODE mode);
        void set_Im_in_A_per_solar_cell(double I);
        void set_Isc_in_A_per_solar_cell(double I);
        void set_Vm_in_V_per_solar_cell(double v);
        void set_Voc_in_V_per_solar_cell(double v);
        void set_a(double value);
        void set_b(double value);
        void set_c(double value);
        void set_eta(double value);
        void set_Nse(unsigned int value);
        void set_Nsh(unsigned int value);
        void set_Smin_in_Wpm2(double value);
        void set_Smax_in_Wpm2(double value);
        void set_C_in_F(double value);//per pv unit
        void set_Kpdc(double value);
        void set_Kidc(double value);
        void set_Vdcmin_per_solar_cell_in_V(double value);
        void set_Vdcmax_per_solar_cell_in_V(double value);

        PV_DC_VOLTAGE_MODE get_dc_voltage_control_mode() const;
        double get_Im_in_A_per_solar_cell() const;
        double get_Isc_in_A() const;
        double get_Vm_in_V_per_solar_cell() const;
        double get_Voc_in_V_per_solar_cell() const;
        double get_a() const;
        double get_b() const;
        double get_c() const;
        double get_eta() const;
        unsigned int get_Nse() const;
        unsigned int get_Nsh() const;
        double get_Smin_in_Wpm2() const;
        double get_Smax_in_Wpm2() const;
        double get_C_in_F() const;
        double get_Kpdc() const;
        double get_Kidc() const;
        double get_Vdcmin_per_solar_cell_in_V() const;
        double get_Vdcmax_per_solar_cell_in_V() const;

        virtual double get_Pref_in_pu_base_on_mbase();
        virtual double get_maximum_available_power_extracted_power_from_irradiance_in_pu_on_mbase();

        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const PVPNLY& model);

        PV_DC_VOLTAGE_MODE mode;
        double Im_in_A_per_solar_cell;
        double Isc_in_A_per_solar_cell;
        double Vm_in_V_per_solar_cell;
        double Voc_in_V_per_solar_cell;
        double Tref_in_C_deg;//temperature reference
        double a, b, c;
        double eta;//efficiency
        unsigned int Nsh, Nse;//per pv unit solar sell number

        double Smax_in_Wpm2, Smin_in_Wpm2;
        double Vdcmax_per_solar_cell_in_V, Vdcmin_per_solar_cell_in_V;

        double initial_Vdc_per_solar_cell_in_V;//used to initialize  in all mode

        double current_S_in_Wpm2, current_T_in_C_deg;
        double current_Vdc_reference_per_solar_cell_in_V_with_over_or_under_Vdc_mode;
        double current_pelec_including_loss_per_solar_cell_in_W;
        double current_Vdc_at_mppt_per_solar_cell_in_V;
        double current_Ipv_per_solar_cell_in_A;

        INTEGRAL_BLOCK capacitor;//T is per pv unit DC side capacitor in F
        PI_BLOCK Vdc_controller;

};

#endif // PVPNLY_H
