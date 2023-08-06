#ifndef AERDF_H
#define AERDF_H

#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include <cstdlib>

class AERDF : public WT_AERODYNAMIC_MODEL
{
    public:
        AERDF(STEPS& toolkit);
        AERDF(const AERDF& model);
        virtual ~AERDF();
        virtual AERDF& operator=(const AERDF& model);
    public:
        void set_Cp_file(string file);
        string get_Cp_file() const;
    public:
        virtual double get_Cp(double lambda, double pitch_deg) const;
        virtual double get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const;
    public:
        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

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
        void copy_from_const_model(const AERDF& model);
        void load_data_from_Cp_file();
        void load_pitch_angles();
        void load_tip_speed_ratios();
        void load_Cp_matrix();
        void save_data_to_Cp_file() const;
        bool is_Cp_point_exist(double pitch, double lambda);
        // Cp function parameters
        string *cp_file_name;
        vector<double> *pitch_angles;
        vector<double> *tip_speed_ratios;
        vector<vector<double> > *Cp_Matrix;

};

#endif // AERDF_H
