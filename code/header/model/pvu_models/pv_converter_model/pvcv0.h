#ifndef PVCV0_H
#define PVCV0_H

#include "header/model/pvu_models/pv_converter_model/pv_converter_model.h"
#include "header/model/wtg_models/wt_generator_model/lvpl.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

class PVCV0 : public PV_CONVERTER_MODEL
{
    public:
        PVCV0();
        PVCV0(const PVCV0& model);
        virtual ~PVCV0();
        virtual PVCV0& operator=(const PVCV0&);

        void set_converter_activer_current_command_T_in_s(double t);
        void set_converter_reactiver_voltage_command_T_in_s(double t);
        void set_KPLL(double K);
        void set_KIPLL(double K);
        void set_PLLmax(double pmax);
        void set_PLLmin(double pmax);
        void set_LVPL(const LVPL& lvpl);
        void set_HVRC_voltage_in_pu(double v);
        void set_HVRC_current_in_pu(double i);
        void set_LVPL_max_rate_of_active_current_change(double rate);
        void set_LVPL_voltage_sensor_T_in_s(double t);

        double get_converter_activer_current_command_T_in_s() const;
        double get_converter_reactiver_voltage_command_T_in_s() const;
        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;
        double get_PLLmin() const;
        LVPL get_LVPL() const;
        double get_HVRC_voltage_in_pu() const;
        double get_HVRC_current_in_pu() const;
        double get_LVPL_max_rate_of_active_current_change() const;
        double get_LVPL_voltage_sensor_T_in_s() const;


        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

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
        virtual string get_standard_model_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
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
        // the following function are used to model PVCV0 as ideal voltage source
        void set_pll_angle_in_deg(double angle);
    private:
        void copy_from_const_model(const PVCV0& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        INTEGRAL_BLOCK active_current_commander;
        double LVPL_active_power_change_rate;
        FIRST_ORDER_BLOCK LVPL_voltage_sensor;
        LVPL lvpl;

        FIRST_ORDER_BLOCK reactive_voltage_commander;
        double HVRCR_voltage, HVRCR_current;

        double KPLL;
        INTEGRAL_BLOCK PLL_frequency_integrator, PLL_angle_integrator;
};

#endif // GENERATOR_MODEL_H
