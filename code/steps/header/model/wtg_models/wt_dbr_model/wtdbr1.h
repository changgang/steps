#ifndef WTDBR1_H
#define WTDBR1_H

#include "header/model/wtg_models/wt_dbr_model/wt_dbr_model.h"
#include "header/block/integral_block.h"

class WTDBR1 : public WT_DBR_MODEL
{
    public:
        WTDBR1(STEPS& toolkit);
        WTDBR1(const WTDBR1&model);
        virtual ~WTDBR1();
        virtual WTDBR1& operator=(const WTDBR1& model);
    public: // specific model level
        virtual string get_model_name() const;

        void set_Kdbr(double K);

        double get_Kdbr() const;

    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();
        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_braking_resistor_power_in_pu_based_on_mbase();
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
    private:
        void copy_from_const_model(const WTDBR1& model);

        double Kdbr;
        INTEGRAL_BLOCK resistor_energy_integrator;

};

#endif // WTDBR1_H
