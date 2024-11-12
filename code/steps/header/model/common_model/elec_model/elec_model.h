#ifndef ELEC_MODEL_H
#define ELEC_MODEL_H

#include "header/model/common_model/common_model.h"

class ELEC_MODEL:public COMMON_MODEL
{
    public:
        ELEC_MODEL(STEPS& toolkit);
        virtual ~ELEC_MODEL();
    public:

        //set
        void set_terminal_current_in_pu_based_on_mbase(double Iterm);
        void set_active_power_generation_in_pu_based_on_mbase(double Pgen);
        void set_reactive_power_generation_in_pu_based_on_mbase(double Qgen);
        void set_speed_in_pu(double s);
        void set_Vdc(double v);


        void set_voltage_reference_in_pu(double Vref);
        void set_voltage_reference_in_pu_with_bus_to_regulate();
        void set_power_factor_reference_in_pu(double pf);
        void set_active_power_reference_in_pu_based_on_mbase(double Pref);
        void set_reactive_power_reference_in_pu_based_on_mbase(double Qref);
        void set_speed_reference_in_pu(double s);
        void set_Vdc_reference(double V);

        void set_bus_to_regulate(unsigned int bus);

        void set_initial_active_current_command_in_pu_based_on_mbase(double Ipcmd);
        void set_initial_active_power_command_in_pu_based_on_mbase(double pcmd);
        void set_initial_reactive_current_command_in_pu_based_on_mbase(double Iqcmd);
        void set_initial_reactive_power_command_in_pu_based_on_mbase(double qcmd);
        void set_initial_reactive_voltage_command_in_pu(double eqcmd);
        void set_initial_Pref(double P);
        void set_initial_speed(double s);
        void set_initial_Vdc(double v);
        void set_initial_Torque(double T);
        void set_Xsource(double X);

        //get
        double get_terminal_current_in_pu_based_on_mbase() const;
        double get_active_power_generation_in_pu_based_on_mbase() const;
        double get_reactive_power_generation_in_pu_based_on_mbase() const;
        double get_speed_in_pu() const;
        double get_Vdc() const;

        double get_voltage_reference_in_pu() const;
        double get_power_factor_reference_in_pu() const;
        double get_active_power_reference_in_pu_based_on_mbase() const;
        double get_reactive_power_reference_in_pu_based_on_mbase() const;
        double get_speed_reference_in_pu() const;
        double get_Vdc_reference() const;

        unsigned int get_bus_to_regulate()  const;

        double get_initial_active_current_command_in_pu_based_on_mbase() const;
        double get_initial_active_power_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_current_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_power_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_voltage_command_in_pu() const;

        double get_initial_Pref() const;
        double get_initial_speed() const;
        double get_initial_Vdc() const;
        double get_initial_Torque() const;
        double get_Xsource() const;
    public:


        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_active_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_active_power_command_in_pu_based_on_mbase() const = 0;
        virtual double get_reactive_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_power_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_voltage_command_in_pu() const = 0;
    private:
        //input from outside model

        double terminal_current_in_pu_based_on_mbase;
        double active_power_generation_in_pu_based_on_mbase;
        double reactive_power_generation_in_pu_based_on_mbase;
        double speed_in_pu;
        double Vdc;

        //reference
        double voltage_reference_in_pu;
        double power_factor_reference_in_pu;
        double active_power_reference_in_pu_based_on_mbase;
        double reactive_power_reference_in_pu_based_on_mbase;
        double speed_reference_in_pu;
        double Vdc_reference;

        unsigned int bus_to_regulate;


        //input from outside model to initialize
        double initial_active_current_command_in_pu_based_on_mbase;
        double initial_active_power_command_in_pu_based_on_mbase;
        double initial_reactive_current_command_in_pu_based_on_mbase;
        double initial_reactive_power_command_in_pu_based_on_mbase;
        double initial_reactive_voltage_command_in_pu;

        double initial_Pref;
        double initial_speed;
        double initial_Vdc;
        double initial_Torque;

        double Xsource;
};

#endif // ELEC_MODEL_H

