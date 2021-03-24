#ifndef EXCITER_MODEL_H
#define EXCITER_MODEL_H

#include "header/model/sg_models/sg_model.h"
#include "header/block/integral_block.h"
class EXCITER_MODEL : public SG_MODEL
{
    /*
    Exciter model
    Inputs:
        Generator terminal voltage
        Stabilizer stabilizing signal
    Output:
        Excitation voltage
    */
    public:
        EXCITER_MODEL(STEPS& toolkit);
        virtual ~EXCITER_MODEL();
    public: // exciter common
        virtual string get_model_type() const;
        // get input for initialization
        double get_initial_excitation_voltage_in_pu_from_sync_generator_model() const;
        // get input for dynamics
        double get_compensated_voltage_in_pu();
        double get_stabilizing_signal_in_pu();
        double get_field_current_in_pu() const;
        // reference
        void set_voltage_reference_in_pu(double vref);
        double get_voltage_reference_in_pu() const;

        virtual STEPS_SPARSE_MATRIX* get_linearized_matrix(string matrix_type);
        virtual void set_linearized_matrix(string matrix_type, STEPS_SPARSE_MATRIX* matrix);
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_excitation_voltage_in_pu() = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string() const = 0;

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
        double voltage_reference_in_pu;

        STEPS_SPARSE_MATRIX* avr_matrix;
        STEPS_SPARSE_MATRIX* avr_gen_matrix;
        STEPS_SPARSE_MATRIX* avr_pss_matrix;
        STEPS_SPARSE_MATRIX* avr_comp_matrix;
};

#endif // EXCITER_MODEL_H
