#ifndef PVCVX_H
#define PVCVX_H

#include "header/model/pvu_models/pv_converter_model/pv_converter_model.h"
#include "header/model/converter_common_models/lvpl_model/lvpl.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

class PVCVX : public PV_CONVERTER_MODEL
{
    public:
        PVCVX(STEPS& toolkit);
        PVCVX(const PVCVX& model);
        virtual ~PVCVX();
        virtual PVCVX& operator=(const PVCVX&);

        void set_Xeq_in_pu(double xeq);
        void set_KPLL(double K);
        void set_KIPLL(double K);
        void set_PLLmax(double pmax);

        double get_Xeq_in_pu() const;
        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;


        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase();
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase();
        virtual double get_terminal_current_in_pu_based_on_mbase();
        virtual double get_terminal_current_in_pu_based_on_sbase();

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual complex<double> get_terminal_complex_power_in_pu_based_on_mbase();
        virtual complex<double> get_terminal_complex_power_in_MVA();
        virtual double get_terminal_active_power_in_pu_based_on_mbase();
        virtual double get_terminal_active_power_in_MW();
        virtual double get_terminal_reactive_power_in_pu_based_on_mbase();
        virtual double get_terminal_reactive_power_in_MVar();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        virtual double get_active_power_generation_including_stator_loss_in_MW();

        virtual double get_pll_angle_in_rad();
        virtual double get_pll_angle_in_deg();
        virtual double get_pll_frequency_deviation_in_pu();
        virtual double get_pll_frequency_deviation_in_Hz();
        virtual double get_pll_frequency_in_pu();
        virtual double get_pll_frequency_in_Hz();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:
        // the following function are used to model PVCVX as ideal voltage source
        void set_pll_angle_in_deg(double angle);
    private:
        void copy_from_const_model(const PVCVX& model);

        double Xeq;
        FIRST_ORDER_BLOCK active_current_commander;
        FIRST_ORDER_BLOCK reactive_voltage_commander;

        double KPLL;
        INTEGRAL_BLOCK PLL_frequency_integrator, PLL_angle_integrator;
};

#endif // PVCVX_H
