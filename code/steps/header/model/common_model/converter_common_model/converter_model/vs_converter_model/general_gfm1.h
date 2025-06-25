#ifndef GENERAL_GFM1_H
#define GENERAL_GFM1_H

#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/vs_converter_model.h"
#include "header/model/common_model/converter_common_model/current_order_limiter_model/convcol1.h"
#include "header/model/common_model/converter_common_model/pll_model/pll0.h"
#include "header/block/integral_block.h"
#include "header/block/pi_block.h"
#include "header/block/first_order_block.h"

enum CONTROL_MODE // GFM Control Mode
{
    DROOP_CONTROL_MODE = 0,
    VSG_CONTROL_MODE = 1
};

class GENERAL_GFM1 : public VS_CONVERTER_MODEL
{
    public:
        GENERAL_GFM1(STEPS& toolkit);
        virtual ~GENERAL_GFM1();
        GENERAL_GFM1(const GENERAL_GFM1& model);
        virtual GENERAL_GFM1& operator=(const GENERAL_GFM1& model);
        void clear();
        void copy_from_const_model(const GENERAL_GFM1& model);
        string get_model_name() const;
    public:
        void set_Eref_in_pu(double E);
        void set_voltage_flag(unsigned int flag);
        void set_Kp(double Kp);
        void set_Tp_in_s(double Tp);
        void set_Tpf_in_s(double Tpf);
        void set_Tqf_in_s(double Tqf);
        void set_Tvf_in_s(double Tvf);
        void set_mq(double mq);
        void set_mv(double mv);
        void set_Kpv(double Kpv);
        void set_Kiv(double Kiv);
        void set_Emax(double Emax);
        void set_Emin(double Emin);
        void set_current_limit_mode(CURRENT_LIMIT_MODE current_limit_mode);
        void set_dq_oriented_flag(DQ_ORIENTED_FLAG dq_oriented_flag);
        void set_phi_i_in_deg(double phi_i_in_deg);
        void set_Imax(double Imax);
        void set_KPLL(double Kp);
        void set_KIPLL(double Ki);
        void set_PLLmax(double pmax);
        void set_PLLmin(double pmin);

        double get_Eref_in_pu() const;
        unsigned int get_voltage_flag() const;
        double get_Kp() const;
        double get_Tp_in_s() const;
        double get_Tpf_in_s() const;
        double get_Tqf_in_s() const;
        double get_Tvf_in_s() const;
        double get_mq() const;
        double get_mv() const;
        double get_Kpv() const;
        double get_Kiv() const;
        double get_Emax() const;
        double get_Emin() const;
        CURRENT_LIMIT_MODE get_current_limit_mode() const;
        DQ_ORIENTED_FLAG get_dq_oriented_flag() const;
        double get_phi_i_in_deg() const;
        double get_phi_i_in_rad() const;
        double get_Imax() const;
        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;
        double get_PLLmin() const;

        double get_active_power_sensor_state() const;
        double get_active_power_regulator_block_state() const;
        double get_virtual_angle_block_state() const;
        double get_reactive_power_sensor_state() const;
        double get_terminal_voltage_sensor_state() const;
        double get_reactive_power_regulator_block_state() const;
        double get_PLL_frequency_integrator_state() const;
        double get_PLL_angle_integrator_state() const;
    public:
        virtual void setup_block_toolkit_and_parameters();
        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual double get_virtual_frequency_deviation_in_pu() const;
        virtual double get_virtual_angle_in_rad() const;
        virtual double get_virtual_voltage_in_pu() const;

        complex<double> get_virtual_complex_voltage_in_pu_in_xy_axis();
        complex<double> get_unlimited_xy_current_order();
        complex<double> get_limited_xy_current_order();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
    private:
        double Eref;
        unsigned int Voltage_Flag;
        double mq, mv, Emax, Emin;
        //ACTIVE POWER CONTROL LOOP
        FIRST_ORDER_BLOCK active_power_sensor, active_power_regulator_block;
        INTEGRAL_BLOCK virtual_angle_block;
        //REACTIVE POWER CONTROL LOOP
        FIRST_ORDER_BLOCK reactive_power_sensor, terminal_voltage_sensor;
        PI_BLOCK reactive_power_regulator_block;
        //CURRENT LIMITER
        CONVCOL1 convcol1;
        PLL0 pll0;
};

#endif // GENERAL_GFM1_H
