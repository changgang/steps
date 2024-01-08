#ifndef COMPENSATOR_MODEL_H
#define COMPENSATOR_MODEL_H

#include "header/model/sg_models/sg_model.h"
#include "header/block/integral_block.h"
#include <complex>

class COMPENSATOR_MODEL : public SG_MODEL
{
    /*
    Compensator model
    Inputs:
        Generator terminal voltage
        Generator terminal current
    Output:
        Compensated voltage
    */
    public:
        COMPENSATOR_MODEL(STEPS& toolkit);
        virtual ~COMPENSATOR_MODEL();
    public: // compensator common
        virtual string get_model_type() const;
        // common inputs
        complex<double> get_generator_terminal_complex_voltage_in_pu() const;
        complex<double> get_generator_terminal_complex_current_in_pu() const;
        double get_generator_terminal_voltage_in_pu();
        double get_generator_terminal_current_in_pu() const;

        virtual STEPS_SPARSE_MATRIX* get_linearized_matrix(string matrix_type);
        virtual void set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix);
    public: // specific compensator
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_compensated_voltage_in_pu() = 0; // output
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

        virtual STEPS_SPARSE_MATRIX get_linearized_system_A() const = 0;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_B() const = 0;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_C() const = 0;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_D() const = 0;
        virtual void get_linearized_system_ABCD(STEPS_SPARSE_MATRIX* A,
                                                STEPS_SPARSE_MATRIX* B,
                                                STEPS_SPARSE_MATRIX* C,
                                                STEPS_SPARSE_MATRIX* D) const = 0;
    private:
        double ecomp;

        STEPS_SPARSE_MATRIX* comp_matrix;
        STEPS_SPARSE_MATRIX* comp_avr_matrix;
};

#endif // COMPENSATOR_MODEL_H
