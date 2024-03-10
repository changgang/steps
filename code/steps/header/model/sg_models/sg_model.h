#ifndef SG_MODEL_H
#define SG_MODEL_H

#include "header/model/model.h"
#include "header/device/bus.h"
#include "header/basic/sparse_matrix_define.h"
#include <complex>

class GENERATOR;

class SG_MODEL : public MODEL
{
    public:
        SG_MODEL(STEPS& toolkit);
        virtual ~SG_MODEL();

        GENERATOR* get_generator_pointer() const;
        double get_mbase_in_MVA() const;
        double get_one_over_mbase_in_one_over_MVA() const;
        double get_bus_base_frequency_in_Hz() const;
        complex<double> get_terminal_complex_voltage_in_pu() const;
        double get_terminal_voltage_in_pu() const;

        void initialize_ABCD_matrix_for_linearization();
        STEPS_SPARSE_MATRIX get_linearized_system_variable(char var) const;
        STEPS_SPARSE_MATRIX get_linearized_system_A() const;
        STEPS_SPARSE_MATRIX get_linearized_system_B() const;
        STEPS_SPARSE_MATRIX get_linearized_system_C() const;
        STEPS_SPARSE_MATRIX get_linearized_system_D() const;
    public: // specific model level
        virtual string get_model_type() const = 0;
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

        virtual void linearize() = 0;
        virtual STEPS_SPARSE_MATRIX* get_linearized_matrix(string matrix_type) = 0;
        virtual void set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix) = 0;

        virtual void build_linearized_matrix_ABCD() = 0;
    private:
        STEPS_SPARSE_MATRIX *Aptr, *Bptr, *Cptr, *Dptr;

};
#endif // SG_MODEL_H
