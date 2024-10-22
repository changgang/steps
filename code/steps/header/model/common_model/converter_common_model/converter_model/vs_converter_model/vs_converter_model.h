#ifndef VS_CONVERTER_MODEL_H
#define VS_CONVERTER_MODEL_H

#include "header/model/common_model/converter_common_model/converter_model/converter_model.h"

class VS_CONVERTER_MODEL: public CONVERTER_MODEL
{
    public:
        VS_CONVERTER_MODEL(STEPS& toolkit);
        ~VS_CONVERTER_MODEL();

        void set_Pref_in_pu_based_on_mbase(double P);
        void set_Qref_in_pu_based_on_mbase(double Q);
        void set_Vref_in_pu(double V);

        double get_Pref_in_pu_based_on_mbase() const;
        double get_Qref_in_pu_based_on_mbase() const;
        double get_Vref_in_pu() const;

        void set_initial_virtual_voltage_in_pu(double V);
        void set_initial_virtual_angle_in_rad(double angle);
        double get_initial_virtual_voltage_in_pu() const;
        double get_initial_virtual_angle_in_rad() const;

        double get_virtual_frequency_deviation_in_Hz() const;
        double get_virtual_frequency_in_pu() const;
        double get_virtual_frequency_in_Hz() const;
        double get_virtual_angle_in_deg() const;

    public:
        virtual void setup_block_toolkit_and_parameters() = 0;
        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase() =0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() =0;
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis() =0;
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() =0;
        virtual double get_virtual_frequency_deviation_in_pu() const = 0;
        virtual double get_virtual_angle_in_rad() const = 0;
        virtual double get_virtual_voltage_in_pu() const = 0;
    private:
        double Pref, Qref, Vref;//P  Q  V
        double angle0, V0;
};

#endif // VS_CONVERTER_MODEL_H
