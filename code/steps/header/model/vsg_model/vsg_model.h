#ifndef VSG_MODEL_H
#define VSG_MODEL_H

#include "header/model/model.h"
#include "header/device/bus.h"
#include <complex>

class SOURCE;
class WT_GENERATOR;
class PV_UNIT;
class ENERGY_STORAGE;

class VSG_MODEL : public MODEL
{
    /*
    input: device active, reactive power and terminal voltage
    output: voltage magnitude and angle of internal voltage source
    */
    public:
        VSG_MODEL(STEPS& toolkit);
        virtual ~VSG_MODEL();
        virtual string get_model_type() const;

        SOURCE* get_source_pointer() const;
        WT_GENERATOR* get_wt_generator_pointer() const;
        PV_UNIT* get_pv_unit_pointer() const;
        ENERGY_STORAGE* get_enerage_storage_pointer() const;

        double get_mbase_in_MVA() const;
        double get_one_over_mbase_in_one_over_MVA() const;

        complex<double> get_terminal_complex_power_in_pu_based_on_mbase() const;
        double get_terminal_voltage_in_pu() const;
        complex<double> get_terminal_complex_voltage_in_pu() const;
        double get_terminal_voltage_angle_in_rad() const;
        double get_bus_base_frequency_in_Hz() const;
        double get_bus_base_angle_speed_in_radps() const;
        complex<double> get_source_impedance_in_pu_based_on_mbase() const;

        void set_Pref_in_pu_based_on_mbase(double P);
        void set_Qref_in_pu_based_on_mbase(double Q);
        void set_Vref_in_pu(double V);

        double get_Pref_in_pu_based_on_mbase() const;
        double get_Qref_in_pu_based_on_mbase() const;
        double get_Vref_in_pu() const;

        double get_virtual_speed_in_pu() const;
    public: // specific model level
        virtual double get_virtual_speed_deviation_in_pu() const = 0;
        virtual double get_virtual_angle_in_rad() const = 0;
        virtual double get_virtual_voltage_in_pu() const = 0;

        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

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
        double Pref, Qref, Vref;
};
#endif // VSG_MODEL_H
