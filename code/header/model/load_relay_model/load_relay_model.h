#ifndef LOAD_RELAY_MODEL_H
#define LOAD_RELAY_MODEL_H

#include "header/model/model.h"
#include "header/basic/steps_enum.h"

class LOAD;

class LOAD_RELAY_MODEL : public MODEL
{
    public:
        LOAD_RELAY_MODEL();
        virtual ~LOAD_RELAY_MODEL();

        LOAD* get_load_pointer() const;

        void set_subsystem_type(SUBSYSTEM_TYPE subtype);
        SUBSYSTEM_TYPE get_subsystem_type() const;
        string get_detailed_model_name() const;

        virtual string get_model_type() const = 0;
        virtual string get_model_name() const = 0;

        virtual double get_model_data_with_index(size_t index) const = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_index(size_t index, double value) = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;

        virtual bool setup_model_with_steps_string(string data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_total_shed_scale_factor_in_pu() const = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;
    private:
        SUBSYSTEM_TYPE subsystem_type;


};

#endif // LOAD_RELAY_MODEL_H
