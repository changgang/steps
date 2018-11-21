#ifndef COMP_H
#define COMP_H

#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/block/integral_block.h"
class COMP: public COMPENSATOR_MODEL
{
    public:
        COMP();
        COMP(const COMP& model);
        virtual ~COMP();
        virtual COMP& operator=(const COMP& model);
    public:
        virtual string get_model_name() const;

        void set_Xe(double Xe);
        double get_Xe() const;
    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_compensated_voltage_in_pu() const;
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
        void copy_from_const_model(const COMP& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        double Xe;
};

#endif // EXCITER_MODEL_H
