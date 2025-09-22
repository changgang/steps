#ifndef CSCONV3_H
#define CSCONV3_H

#include "header/model/common_model/converter_common_model/converter_model/cs_converter_model/cs_converter_model.h"
#include "header/model/common_model/converter_common_model/lvpl_model/lvpl.h"
#include "header/model/common_model/converter_common_model/pll_model/pll0.h"
#include "header/block/integral_block.h"
#include "header/block/first_order_block.h"

class CSCONV3 : public CS_CONVERTER_MODEL
{
    public:
        CSCONV3(STEPS& toolkit);
        virtual ~CSCONV3();
        CSCONV3(const CSCONV3& model);
        virtual CSCONV3& operator=(const CSCONV3& model);
        void clear();
        void copy_from_const_model(const CSCONV3& model);
        string get_model_name() const;
    public:

        void set_KPLL(double K);
        void set_KIPLL(double K);
        void set_PLLmax(double pmax);

        double get_KPLL() const;
        double get_KIPLL() const;
        double get_PLLmax() const;

        double get_active_current_commander_state() const;
        double get_reactive_current_commander_state() const;
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
        virtual double get_Ip_in_pu();
        virtual double get_Iq_in_pu();

    private:

        FIRST_ORDER_BLOCK active_current_commander;
        FIRST_ORDER_BLOCK reactive_current_commander;
        PLL0 pll0;

};

#endif // CSCONV3_H
