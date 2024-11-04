#ifndef CSCONV2_H
#define CSCONV2_H

#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/cs_converter_model.h"
#include "header/model/common_model/converter_common_model/lvpl_model/lvpl.h"
#include "header/model/common_model/converter_common_model/pll_model/pll0.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

class CSCONV2 : public CS_CONVERTER_MODEL
{
    public:
        CSCONV2(STEPS& toolkit);
        virtual ~CSCONV2();
        CSCONV2(const CSCONV2& model);
        virtual CSCONV2& operator=(const CSCONV2& model);
        void clear();
        void copy_from_const_model(const CSCONV2& model);
        string get_model_name() const;
    public:
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

        double get_active_current_commander_state() const;
        double get_reactive_voltage_commander_state() const;
        double get_PLL_frequency_integrator_state() const;
        double get_PLL_angle_integrator_state() const;
        double get_LVPL_voltage_sensor_state() const;

    public:
        virtual void setup_block_toolkit_and_parameters();
        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        virtual double get_pll_angle_in_rad();
        virtual double get_pll_frequency_deviation_in_pu();
        virtual double get_Ip_in_pu();
        virtual double get_Iq_in_pu();


    private:
        INTEGRAL_BLOCK active_current_commander;
        double LVPL_active_power_change_rate;
        FIRST_ORDER_BLOCK LVPL_voltage_sensor;
        LVPL lvpl;

        FIRST_ORDER_BLOCK reactive_voltage_commander;
        double HVRCR_voltage, HVRCR_current;

        PLL0 pll0;

};

#endif // CSCONV2_H
