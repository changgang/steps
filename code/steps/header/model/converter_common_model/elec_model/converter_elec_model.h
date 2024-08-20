#ifndef CONVERTER_ELEC_MODEL_H
#define CONVERTER_ELEC_MODEL_H

#include "header/STEPS.h"

class CONVERTER_ELEC_MODEL
{
    public:
        CONVERTER_ELEC_MODEL(STEPS& toolkit);
        virtual ~CONVERTER_ELEC_MODEL();

        void set_toolkit(STEPS& toolkit);
        STEPS& get_toolkit() const;

        void set_bus_pointer(BUS* bus);
        BUS* get_bus_pointer() const;
        unsigned int get_bus() const;

        void set_device_pointer(NONBUS_DEVICE* device);
        NONBUS_DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        string get_compound_device_name() const;
        void set_model_name(string name);
        string get_model_name() const;
    public:

        void set_terminal_current_in_pu_based_on_mbase(double Iterm);
        void set_active_power_generation_in_pu_based_on_mbase(double Pgen);
        void set_reactive_power_generation_in_pu_based_on_mbase(double Qgen);
        void set_voltage_reference_in_pu(double Vref);
        void set_voltage_reference_in_pu_with_bus_to_regulate();
        void set_power_factor_reference_in_pu(double pf);
        void set_active_power_reference_in_pu_based_on_mbase(double Pref);
        void set_reactive_power_reference_in_pu_based_on_mbase(double Qref);

        double get_terminal_bus_frequency_deviation_in_pu() const;
        double get_terminal_voltage_in_pu() const;
        double get_terminal_current_in_pu_based_on_mbase() const;
        double get_active_power_generation_in_pu_based_on_mbase() const;
        double get_reactive_power_generation_in_pu_based_on_mbase() const;
        double get_voltage_reference_in_pu() const;
        double get_power_factor_reference_in_pu() const;
        double get_active_power_reference_in_pu_based_on_mbase() const;
        double get_reactive_power_reference_in_pu_based_on_mbase() const;

        void set_bus_to_regulate(unsigned int bus);
        unsigned int get_bus_to_regulate()  const;

        void set_initial_active_current_command_in_pu_based_on_mbase(double Ipcmd);
        void set_initial_active_power_command_in_pu_based_on_mbase(double pcmd);

        void set_initial_reactive_current_command_in_pu_based_on_mbase(double Iqcmd);
        void set_initial_reactive_power_command_in_pu_based_on_mbase(double qcmd);
        void set_initial_reactive_voltage_command_in_pu(double eqcmd);

        void set_Xsource(double X);

        double get_initial_active_current_command_in_pu_based_on_mbase() const;
        double get_initial_active_power_command_in_pu_based_on_mbase() const;

        double get_initial_reactive_current_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_power_command_in_pu_based_on_mbase() const;
        double get_initial_reactive_voltage_command_in_pu() const;
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
        STEPS* toolkit;
        BUS* bus_ptr;
        NONBUS_DEVICE* device;
        string model_name;

        //input from outside model
        double active_power_reference_in_pu_based_on_mbase;

        double terminal_current_in_pu_based_on_mbase;
        double active_power_generation_in_pu_based_on_mbase;
        double reactive_power_generation_in_pu_based_on_mbase;

        //reference
        double voltage_reference_in_pu;
        double power_factor_reference_in_pu;
        double reactive_power_reference_in_pu_based_on_mbase;

        unsigned int bus_to_regulate;


        //input from outside model to initialize()
        double initial_active_current_command_in_pu_based_on_mbase;
        double initial_active_power_command_in_pu_based_on_mbase;
        double initial_reactive_current_command_in_pu_based_on_mbase;
        double initial_reactive_power_command_in_pu_based_on_mbase;
        double initial_reactive_voltage_command_in_pu;

        double Xsource;
};

#endif // CONVERTER_ELEC_MODEL_H

