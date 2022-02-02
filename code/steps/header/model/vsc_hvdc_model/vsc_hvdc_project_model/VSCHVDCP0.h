#ifndef VSCHVDCP0_H_INCLUDED
#define VSCHVDCP0_H_INCLUDED

#include "header/model/vsc_hvdc_model/vsc_hvdc_project_model/vsc_hvdc_project_model.h"

class VSCHVDCP0: public VSC_HVDC_PROJECT_MODEL
{
    public:
        VSCHVDCP0(STEPS& toolkit);
        VSCHVDCP0(const VSCHVDCP0& model);
        virtual ~VSCHVDCP0();
        virtual VSCHVDCP0& operator=(const VSCHVDCP0& model);
    public:
        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        void set_model_data_with_index(unsigned int index, double value);
        double get_model_data_with_index(unsigned int index);

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);
        double get_model_internal_variable_with_index(unsigned int index);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const VSCHVDCP0& model);
};
#endif // VSCHVDCP0_H_INCLUDED
