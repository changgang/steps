#ifndef PSASPVC_H
#define PSASPVC_H

#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/block/integral_block.h"
class PSASPVC: public COMPENSATOR_MODEL
{
    public:
        PSASPVC(STEPS& toolkit);
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

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_compensated_voltage_in_pu();
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;

        virtual void linearize();
        virtual STEPS_SPARSE_MATRIX get_linearized_system_A() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_B() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_C() const;
        virtual STEPS_SPARSE_MATRIX get_linearized_system_D() const;
        virtual void get_linearized_system_ABCD(STEPS_SPARSE_MATRIX* A,
                                                STEPS_SPARSE_MATRIX* B,
                                                STEPS_SPARSE_MATRIX* C,
                                                STEPS_SPARSE_MATRIX* D) const;
    private:
        void copy_from_const_model(const PSASPVC& model);

        double sin_phi;
        double Xc;
};

#endif // EXCITER_MODEL_H
