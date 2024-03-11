#ifndef PVPNLX_H
#define PVPNLX_H

#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include <cstdlib>

class PVPNLX : public PV_PANEL_MODEL
{
    public:
        PVPNLX(STEPS& toolkit);
        PVPNLX(const PVPNLX& model);
        virtual ~PVPNLX();
        virtual PVPNLX& operator=(const PVPNLX& model);
    public:
        void set_Pmsta_in_pu_based_on_mbase(double P);
        void set_b(double b);
        void set_Krp(double Krp);

        double get_Pmsta_in_pu_based_on_mbase() const;
        double get_b() const;
        double get_Krp() const;

        double get_Pmmp_in_pu(double S);
        double get_Pref_in_pu(double S);
        void initialize_S0();

        virtual double get_Pref_in_pu_base_on_mbase();

        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

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
        void copy_from_const_model(const PVPNLX& model);

        double Pmsta, b, Krp;

};

#endif // PVPNLX_H
