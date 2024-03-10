#ifndef LOAD_MODEL_H
#define LOAD_MODEL_H

#include "header/model/model.h"
#include "header/basic/steps_enum.h"
#include "header/device/bus.h"
#include <complex>

class LOAD;

class LOAD_MODEL : public MODEL
{
    public:
        LOAD_MODEL(STEPS& toolkit);
        virtual ~LOAD_MODEL();
        LOAD* get_load_pointer() const;

        void set_voltage_source_flag(bool flag);
        bool get_voltage_source_flag() const;
        bool is_voltage_source() const;

        // common load model
        virtual string get_model_type() const;
        // common inputs
        void synchronize_bus_voltage_and_frequency();
        double get_bus_positive_sequence_voltage_in_pu() const;
        double get_bus_positive_sequence_voltage_in_kV() const;
        complex<double> get_bus_positive_sequence_complex_voltage_in_pu() const;
        double get_bus_frequency_deviation_in_pu() const;
        double get_bus_base_frequency_in_Hz() const;
        // common scale
        void set_subsystem_type(SUBSYSTEM_TYPE subtype);
        SUBSYSTEM_TYPE get_subsystem_type() const;
        string get_detailed_model_name() const;

        void initialize_ABCD_matrix_for_linearization();
        STEPS_SPARSE_MATRIX get_linearized_matrix_variable(char var) const;
        virtual STEPS_SPARSE_MATRIX get_linearized_matrix_A() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_matrix_B() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_matrix_C() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_matrix_D() const;
    public: // specific model level
        virtual string get_model_name() const = 0;
        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual complex<double> get_dynamic_source_admittance_in_pu_based_on_SBASE() = 0;
        virtual complex<double> get_additional_admittance_in_pu_based_on_SBASE() = 0;

        virtual void initialize() = 0;
        virtual void initialize_to_start() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual complex<double> get_load_power_in_MVA() = 0;
        virtual complex<double> get_load_current_in_pu_based_on_SBASE() = 0;
        virtual complex<double> get_norton_current_in_pu_based_on_SBASE() = 0;
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

        virtual void build_linearized_matrix_ABCD() = 0;
    private:
        SUBSYSTEM_TYPE subsystem_type;
        bool voltage_source_flag;
        double voltage_pu, frequency_deviation_pu;
        complex<double> complex_voltage_pu;

        STEPS_SPARSE_MATRIX *Aptr, *Bptr, *Cptr, *Dptr;

};
#endif // LOAD_MODEL_H
