#ifndef WT3GX_H
#define WT3GX_H

#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/model/converter_common_models/converter/gfrmc_model/vsg_model/vsg0.h"
#include "header/model/converter_common_models/current_order_limiter_model/convcol.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

class WT3GX : public WT_GENERATOR_MODEL
{
    public:
        WT3GX(STEPS& toolkit);
        WT3GX(const WT3GX& model);
        virtual ~WT3GX();
        virtual WT3GX& operator=(const WT3GX&);

        void set_Tj_in_s(double T);
        void set_D(double D);
        void set_Ku(double Ku);
        void set_Te_in_s(double T);
        void set_pq_priority_flag(COL_PQ_PRIORITY flag);
        void set_Imax(double Imax);

        double get_Tj_in_s() const;
        double get_D() const;
        double get_Ku() const;
        double get_Te_in_s() const;
        COL_PQ_PRIORITY get_pq_priority_flag() const;
        double get_Imax() const;

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
    private:
        void copy_from_const_model(const WT3GX& model);

        double get_virtual_voltage_in_pu();
        double get_virtual_angle_in_rad();
        complex<double> get_virtual_voltage_in_pu_in_xy_axis();
        complex<double> get_limited_PQ_current_order();

        VSG0 vsg;
        CONVCOL0 convcol;
};

#endif // WT3GX_H
