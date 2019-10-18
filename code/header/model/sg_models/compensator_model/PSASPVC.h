#ifndef PSASPVC_H
#define PSASPVC_H

#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/block/integral_block.h"
class PSASPVC: public COMPENSATOR_MODEL
{
    public:
        PSASPVC();
        PSASPVC(const PSASPVC& model);
        virtual ~PSASPVC();
        virtual PSASPVC& operator=(const PSASPVC& model);
    public:
        virtual string get_model_name() const;

        void set_Xc(double Xc);
        double get_Xc() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_compensated_voltage_in_pu();
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const PSASPVC& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        double Xc;
        double sin_phi;
};

#endif // EXCITER_MODEL_H
