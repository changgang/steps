#ifndef CSCONV4_H
#define CSCONV4_H

#include "header/model/converter_common_model/converter_model/cs_converter_model/cs_converter_model.h"
#include "header/model/converter_common_model/current_order_limiter_model/convcol.h"
#include "header/block/first_order_block.h"
#include "header/model/converter_common_model/pll_model/pll0.h"

class CSCONV4 : public CS_CONVERTER_MODEL
{
    public:
        CSCONV4(STEPS& toolkit);
        virtual ~CSCONV4();
        CSCONV4(const CSCONV4& model);
        virtual CSCONV4& operator=(const CSCONV4& model);
        void clear();
        void copy_from_const_model(const CSCONV4& model);
    public:
        void set_converter_activer_current_command_T_in_s(double t);
        void set_converter_reactiver_voltage_command_T_in_s(double t);
        void set_KPLL(double K);
        void set_KIPLL(double K);
        void set_PLLmax(double pmax);
        void set_PLLmin(double pmin);
        void set_pq_priority_flag(COL_PQ_PRIORITY flag);
        void set_Imax(double Imax);

        double get_converter_activer_current_command_T_in_s() const;
        double get_converter_reactiver_voltage_command_T_in_s() const;
        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;
        double get_PLLmin() const;
        COL_PQ_PRIORITY get_pq_priority_flag() const;
        double get_Imax() const;


        double get_active_current_commander_state() const;
        double get_reactive_voltage_commander_state() const;
        double get_PLL_frequency_integrator_state() const;
        double get_PLL_angle_integrator_state() const;

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


    private:
        FIRST_ORDER_BLOCK active_current_commander;
        FIRST_ORDER_BLOCK reactive_voltage_commander;
        CONVCOL0 convcol;

        PLL0 pll0;


};

#endif // CSCONV4_H
