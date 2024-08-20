#ifndef CS_CONVERTER_MODEL_H
#define CS_CONVERTER_MODEL_H

#include "header/model/converter_common_model/converter_model/converter_model.h"
#include "header/STEPS.h"

class BUS;

class CS_CONVERTER_MODEL: public CONVERTER_MODEL
{
    public:
        CS_CONVERTER_MODEL(STEPS& toolkit);
        ~CS_CONVERTER_MODEL();

        void set_Ipcmd_in_pu_based_on_mbase(double Ipcmd);
        void set_Iqcmd_in_pu_based_on_mbase(double Iqcmd);
        void set_Eqcmd_in_pu(double Eqcmd);

        double get_Ipcmd_in_pu_based_on_mbase() const;
        double get_Iqcmd_in_pu_based_on_mbase() const;
        double get_Eqcmd_in_pu() const;

        double get_pll_angle_in_deg();
        double get_pll_frequency_deviation_in_Hz();
        double get_pll_frequency_in_pu();
        double get_pll_frequency_in_Hz();

    public:
        virtual void setup_block_toolkit_and_parameters() = 0;
        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase() = 0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() =0;
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis() =0;
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() =0;
        virtual double get_pll_angle_in_rad() = 0;
        virtual double get_pll_frequency_deviation_in_pu() = 0;

    private:
        double Ipcmd, Iqcmd, Eqcmd;
};

#endif // CS_CONVERTER_MODEL_H
