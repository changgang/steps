#ifndef CONVERTER_MODEL_H
#define CONVERTER_MODEL_H

#include "header/model/common_model/common_model.h"

class CONVERTER_MODEL:public COMMON_MODEL
{
    public:
        CONVERTER_MODEL(STEPS& toolkit);
        ~CONVERTER_MODEL();

        void set_initial_P_generation_in_pu(double P);
        void set_initial_Q_generation_in_pu(double Q);
        double get_initial_P_generation_in_pu() const;
        double get_initial_Q_generation_in_pu() const;

        void set_initial_active_current_command_in_pu_based_on_mbase(double Ipcmd);
        void set_initial_active_power_command_in_pu_based_on_mbase(double pcmd);

        void set_initial_reactive_current_command_in_pu_based_on_mbase(double Iqcmd);
        void set_initial_reactive_power_command_in_pu_based_on_mbase(double qcmd);
        void set_initial_reactive_voltage_command_in_pu(double eqcmd);

        double get_initial_active_current_command_in_pu_based_on_mbase() const;
        double get_initial_active_power_command_in_pu_based_on_mbase() const;

        double get_initial_reactive_current_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_power_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_voltage_command_in_pu() const;

        complex<double> get_source_impedance_in_pu_based_on_mbase() const;
        void set_source_impedance_in_pu_based_on_mbase(complex<double> Z);
        complex<double> get_terminal_complex_power_in_pu_based_on_mbase();
        double get_terminal_active_power_in_pu_based_on_mbase();
        double get_terminal_reactive_power_in_pu_based_on_mbase();
        double get_terminal_current_in_pu_based_on_mbase();

    public:

        virtual void setup_block_toolkit_and_parameters() = 0;
        virtual complex<double> get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_mbase() =0;
        virtual complex<double> get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase() =0;
        virtual complex<double> get_internal_voltage_in_pu_in_xy_axis() =0;
        virtual double get_active_power_generation_including_stator_loss_in_pu_based_on_mbase() =0;
        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

    private:
        double initial_P_generation_in_pu, initial_Q_generation_in_pu;
        double initial_active_current_command_in_pu_based_on_mbase;
        double initial_active_power_command_in_pu_based_on_mbase;
        double initial_reactive_current_command_in_pu_based_on_mbase;
        double initial_reactive_power_command_in_pu_based_on_mbase;
        double initial_reactive_voltage_command_in_pu;

        complex<double> Zsource;
};

#endif // CONVERTER_MODEL_H
