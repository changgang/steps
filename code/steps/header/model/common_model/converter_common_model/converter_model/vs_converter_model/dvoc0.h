#ifndef DVOC0_H
#define DVOC0_H

#include "header/model/common_model/converter_common_model/converter_model/vs_converter_model/vs_converter_model.h"
#include "header/model/common_model/converter_common_model/current_order_limiter_model/convcol.h"
#include "header/block/pi_block.h"
#include "header/block/first_order_block.h"
#include "header/block/integral_block.h"

class DVOC0 : public VS_CONVERTER_MODEL
{
    public:
        DVOC0(STEPS& toolkit);
        virtual ~DVOC0();
        DVOC0(const DVOC0& model);
        virtual DVOC0& operator=(const DVOC0& model);
        void clear();
        void copy_from_const_model(const DVOC0& model);
        string get_model_name() const;
    public:
        void set_eta(double eta);
        void set_alpha(double alpha);
        void set_deltafmax_in_pu(double deltafmax);
        void set_deltafmin_in_pu(double deltafmin);
        void set_pq_priority_flag(COL_PQ_PRIORITY flag);
        void set_Imax(double Imax);

        double get_eta() const;
        double get_alpha() const;
        double get_deltafmax_in_pu() const;
        double get_deltafmin_in_pu() const;
        COL_PQ_PRIORITY get_pq_priority_flag() const;
        double get_Imax() const;

        double get_virtual_angle_block_state() const;
        double get_virtual_voltage_block_state() const;

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
        double eta, alpha;
        double deltafmax_pu, deltafmin_pu;
        INTEGRAL_BLOCK virtual_angle_block, virtual_voltage_block;
        CONVCOL0 convcol;
};

#endif // DVOC0_H
