#ifndef STABILIZER_MODEL_H
#define STABILIZER_MODEL_H

#include "header/model/sg_models/sg_model.h"
#include "header/block/integral_block.h"
#include "header/basic/constants.h"
#include "header/meter/meter.h"

typedef METER SIGNAL;

class STABILIZER_MODEL : public SG_MODEL
{
    /*
    Model of power system stabilizer
    Input:
        generator speed from sync generator model
        generator terminal voltage from bus
        bus frequency from bus
        line power from line
    Output:
        stabilizing signal to exciter model
    */
    public:
        STABILIZER_MODEL();
        virtual ~STABILIZER_MODEL();

    public: // stabilizer common
        virtual string get_model_type() const;
        // common inputs
        void set_input_signal_at_slot(size_t slot, SIGNAL& signal);
        SIGNAL get_input_signal_at_slot(size_t slot) const;
        bool is_slot_valid(size_t slot) const;
        double get_signal_value_of_slot(size_t slot) const;
        size_t convert_signal_type_string_to_number(string signal_type) const;
        string convert_signal_type_number_to_string(size_t signal_type) const;
        //
        SIGNAL prepare_signal_with_signal_type_and_bus(size_t signal_type, size_t bus);
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void set_block_toolkit() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_stabilizing_signal_in_pu() const = 0;

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
        SIGNAL signals[MAX_STABILIZER_INPUT_SIGNAL_SLOT];
};

#endif // STABILIZER_MODEL_H
