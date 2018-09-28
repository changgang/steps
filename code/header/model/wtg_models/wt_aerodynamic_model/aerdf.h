#ifndef AERDF_H
#define AERDF_H

#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include <cstdlib>

class AERDF : public WT_AERODYNAMIC_MODEL
{
    public:
        AERDF();
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

        virtual double get_model_data_with_index(size_t index) const;
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_index(size_t index, double value);
        virtual void set_model_data_with_name(string par_name, double value);

        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual double get_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const AERDF& model);
        virtual void prepare_model_variable_table();
        void load_data_from_Cp_file();
        void load_pitch_angles();
        void load_tip_speed_ratios();
        void load_Cp_matrix();
        bool is_Cp_point_exist(double pitch, double lambda);
        // Cp function parameters
        string cp_file_name;
        vector<double> pitch_angles;
        vector<double> tip_speed_ratios;
        vector<vector<double> > Cp_Matrix;

};

#endif // AERDF_H
