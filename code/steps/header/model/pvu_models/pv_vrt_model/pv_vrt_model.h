#ifndef PV_VRT_MODEL_H
#define PV_VRT_MODEL_H

#include "header/model/pvu_models/pvu_model.h"
#include <complex>
class PV_VRT_MODEL : public PVU_MODEL
{
    /*
    PV voltage ride through control model
    General inputs:
        vterm: PV terminal or remote AC voltage
    General output:
        ipcmd: active current command in pu
        iqcmd: reactive current command in pu
    */
    public:
        PV_VRT_MODEL(STEPS& toolkit);
        virtual ~PV_VRT_MODEL();
    public: // pe elctricla control common
        virtual string get_model_type() const;
        // get input
        complex<double> get_terminal_bus_complex_voltage_in_pu() const;
        double get_terminal_bus_voltage_in_pu() const;

        complex<double> get_pv_unit_terminal_generation_in_MVA() const;
        complex<double> get_pv_unit_terminal_generation_in_pu_based_on_mbase() const;
        complex<double> get_pv_unit_terminal_complex_current_in_pu() const;
        double get_pv_unit_terminal_current_in_pu() const;
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual VRT_STATUS get_lvrt_status() const = 0;
        virtual VRT_STATUS get_hvrt_status() const = 0;

        virtual double get_active_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_active_power_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_current_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_power_command_in_pu_based_on_mbase() = 0;
        virtual double get_reactive_voltage_command_in_pu() = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
};

#endif // PV_VRT_MODEL_H
