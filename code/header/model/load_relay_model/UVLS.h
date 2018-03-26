#ifndef UVLS_H
#define UVLS_H

#include "header/model/load_relay_model/load_relay_model.h"

class UVLS : public LOAD_RELAY_MODEL
{
    public:
        UVLS();
        UVLS(const UVLS& model);
        virtual ~UVLS();
        virtual UVLS& operator=(const UVLS& model);

        virtual string get_model_name() const;
        virtual double get_double_data_with_index(size_t index) const;
        virtual double get_double_data_with_name(string par_name) const;
        virtual void set_double_data_with_index(size_t index, double value);
        virtual void set_double_data_with_name(string par_name, double value);

        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();

        virtual double get_variable_with_index(size_t var_index) = 0;
        virtual double get_variable_with_name(string var_name) = 0;
        virtual size_t get_variable_index_from_variable_name(string var_name) = 0;

        virtual double get_shed_scale_factor() const;

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    public:

    private:
        void copy_from_const_model(const UVLS& model);

};

#endif // LOAD_RELAY_MODEL_H
