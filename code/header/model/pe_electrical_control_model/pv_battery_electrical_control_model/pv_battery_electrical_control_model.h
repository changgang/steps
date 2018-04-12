#ifndef PV_BATTERY_ELECTRICAL_CONTROL_MODEL_H
#define PV_BATTERY_ELECTRICAL_CONTROL_MODEL_H

#include "header/model/pe_electrical_control_model/pe_electrical_control_model.h"
#include "header/block/integral_block.h"
class PV_BATTERY_ELECTRICAL_CONTROL_MODEL : public PE_ELECTRICAL_CONTROL_MODEL
{
    /*
    PE Electrical control model
    Inputs:
        in addition to the inputs of general pe electrical control model, the following inputs are added:
        DC voltage of internal capacitor
        DC voltage reference
    Output:
    */
    public:
        PV_BATTERY_ELECTRICAL_CONTROL_MODEL();
        virtual ~PV_BATTERY_ELECTRICAL_CONTROL_MODEL();
    public: // exciter common
        virtual string get_model_type() const;
        // get input for initialization
        double get_initial_excitation_voltage_in_pu_from_sync_generator_model() const;
        // get input for dynamics
        double get_compensated_voltage_in_pu() const;
        double get_stabilizing_signal_in_pu() const;
        // reference
        void set_voltage_reference_in_pu(double vref);
        double get_voltage_reference_in_pu() const;
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual double get_double_data_with_index(size_t index) const = 0;
        virtual double get_double_data_with_name(string par_name) const = 0;
        virtual void set_double_data_with_index(size_t index, double value) = 0;
        virtual void set_double_data_with_name(string par_name, double value) = 0;

        virtual bool setup_model_with_steps_string(string data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_excitation_voltage_in_pu() const = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;


        virtual size_t get_variable_index_from_variable_name(string var_name)= 0;
        virtual string get_variable_name_from_variable_index(size_t var_index)= 0;
        virtual double get_variable_with_index(size_t var_index)= 0;
        virtual double get_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        double voltage_reference_in_pu;
};

#endif // PV_BATTERY_ELECTRICAL_CONTROL_MODEL_H
