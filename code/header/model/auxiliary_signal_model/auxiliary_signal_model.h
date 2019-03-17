#ifndef AUXILIARY_SIGNAL_MODEL_H
#define AUXILIARY_SIGNAL_MODEL_H

#include "header/model/model.h"
#include "header/block/integral_block.h"
class AUXILIARY_SIGNAL_MODEL : public MODEL
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
        AUXILIARY_SIGNAL_MODEL();
        virtual ~AUXILIARY_SIGNAL_MODEL();
    public: // exciter common
        virtual string get_model_type() const;
        // get input for initialization
        double get_initial_alpha_or_gamma_in_deg() const;
        // get input for dynamics
        // reference
    public: // specific exciter
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_auxiliary_signal_in_MW() const = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
};


#endif // AUXILIARY_SIGNAL_MODEL_H
