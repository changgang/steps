#ifndef GENERAL_GFM_H
#define GENERAL_GFM_H

#include "header/model/converter_common_model/converter_model/vs_converter_model/vs_converter_model.h"
#include "header/model/converter_common_model/current_order_limiter_model/convcol.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

enum GFM_CONTROL_MODE // GFM Control Mode
{
    TRADITIONAL_DROOP_CONTROL = 0,
    DROOP_WITH_FILTER_CONTROL = 1,
    VSG_CONTROL = 2
};

class GENERAL_GFM : public VS_CONVERTER_MODEL
{
    public:
        GENERAL_GFM(STEPS& toolkit);
        virtual ~GENERAL_GFM();
        GENERAL_GFM(const GENERAL_GFM& model);
        virtual GENERAL_GFM& operator=(const GENERAL_GFM& model);
        void clear();
        void copy_from_const_model(const GENERAL_GFM& model);
    public:
        void set_Kp(double Kp);
        void set_Tp_in_s(double Tp);
        void set_Kq(double Kq);
        void set_Tq_in_s(double Kq);
        void set_pq_priority_flag(COL_PQ_PRIORITY flag);
        void set_Imax(double Imax);

        double get_Kp() const;
        double get_Tp_in_s() const;
        double get_Kq() const;
        double get_Tq_in_s() const;
        COL_PQ_PRIORITY get_pq_priority_flag() const;
        double get_Imax() const;

        double get_active_power_regulator_block_state() const;
        double get_reactive_power_regulator_block_state() const;
        double get_virtual_angle_block_state() const;
        complex<double> get_virtual_complex_voltage_in_pu_in_xy_axis();
    public:
        virtual void setup_block_toolkit_and_parameters();
        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        complex<double> get_limited_PQ_current_order();
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis();
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();

        virtual double get_virtual_frequency_deviation_in_pu() const;
        virtual double get_virtual_angle_in_rad() const;
        virtual double get_virtual_voltage_in_pu() const;
    private:
        FIRST_ORDER_BLOCK active_power_regulator_block, reactive_power_regulator_block;
        INTEGRAL_BLOCK virtual_angle_block;
        CONVCOL0 convcol;
};

#endif // GENERAL_GFM_H
