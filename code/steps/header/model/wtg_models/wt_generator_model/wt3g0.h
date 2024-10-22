#ifndef WT3G0_H
#define WT3G0_H

#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/csconv2.h"

class WT3G0 : public WT_GENERATOR_MODEL
{
    public:
        WT3G0(STEPS& toolkit);
        WT3G0(const WT3G0& model);
        virtual ~WT3G0();
        virtual WT3G0& operator=(const WT3G0&);

        void set_converter_activer_current_command_T_in_s(double t);
        void set_converter_reactiver_voltage_command_T_in_s(double t);
        void set_KPLL(double K);
        void set_KIPLL(double K);
        void set_PLLmax(double pmax);
        void set_PLLmin(double pmin);
        void set_HVRC_voltage_in_pu(double v);
        void set_HVRC_current_in_pu(double i);
        void set_LVPL_max_rate_of_active_current_change(double rate);
        void set_LVPL_voltage_sensor_T_in_s(double t);
        void set_LVPL_low_voltage_in_pu(double v);
        void set_LVPL_high_voltage_in_pu(double v);
        void set_LVPL_gain_at_high_voltage(double k);

        double get_converter_activer_current_command_T_in_s() const;
        double get_converter_reactiver_voltage_command_T_in_s() const;
        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;
        double get_PLLmin() const;
        double get_HVRC_voltage_in_pu() const;
        double get_HVRC_current_in_pu() const;
        double get_LVPL_max_rate_of_active_current_change() const;
        double get_LVPL_voltage_sensor_T_in_s() const;
        double get_LVPL_low_voltage_in_pu() const;
        double get_LVPL_high_voltage_in_pu() const;
        double get_LVPL_gain_at_high_voltage() const;


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

        virtual double get_initial_active_current_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_current_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_voltage_command_in_pu() const;
        virtual double get_initial_active_power_command_in_pu_based_on_mbase() const;
        virtual double get_initial_reactive_power_command_in_pu_based_on_mbase() const;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();

        double get_pll_angle_in_rad();
        double get_pll_angle_in_deg();
        double get_pll_frequency_deviation_in_pu();
        double get_pll_frequency_deviation_in_Hz();
        double get_pll_frequency_in_pu();
        double get_pll_frequency_in_Hz();


        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

    private:
        void copy_from_const_model(const WT3G0& model);

        CSCONV2 csconv2;
};

#endif // GENERATOR_MODEL_H
