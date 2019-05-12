#ifndef PVP0_H
#define PVP0_H

#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include <cstdlib>

class PVP0 : public PV_PANEL_MODEL
{
    public:
        PVP0();
        PVP0(const PVP0& model);
        virtual ~PVP0();
        virtual PVP0& operator=(const PVP0& model);
    public:
        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void set_block_toolkit();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const PVP0& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();
};

#endif // PVP0_H
