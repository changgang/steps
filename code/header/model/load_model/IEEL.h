#ifndef IEEL_H
#define IEEL_H

#include "header/model/load_model/load_model.h"
#include <complex>

class IEEL : public LOAD_MODEL
{
    public:
        IEEL();
        IEEL(const IEEL& model);
        virtual ~IEEL();
        virtual IEEL& operator=(const IEEL& model);

        virtual string get_model_name() const;
        virtual double get_model_data_with_index(size_t index) const;
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_index(size_t index, double value);
        virtual void set_model_data_with_name(string par_name, double value);

        void set_P_alpha_1(double alpha);
        void set_P_alpha_2(double alpha);
        void set_P_alpha_3(double alpha);
        void set_P_n_power_1(double n);
        void set_P_n_power_2(double n);
        void set_P_n_power_3(double n);
        void set_P_Kf(double kf);
        void set_Q_alpha_1(double alpha);
        void set_Q_alpha_2(double alpha);
        void set_Q_alpha_3(double alpha);
        void set_Q_n_power_1(double n);
        void set_Q_n_power_2(double n);
        void set_Q_n_power_3(double n);
        void set_Q_Kf(double kf);

        double get_P_alpha_1() const;
        double get_P_alpha_2() const;
        double get_P_alpha_3() const;
        double get_P_n_power_1() const;
        double get_P_n_power_2() const;
        double get_P_n_power_3() const;
        double get_P_Kf() const;

        double get_Q_alpha_1() const;
        double get_Q_alpha_2() const;
        double get_Q_alpha_3() const;
        double get_Q_n_power_1() const;
        double get_Q_n_power_2() const;
        double get_Q_n_power_3() const;
        double get_Q_Kf() const;
    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual complex<double> get_load_power_in_MVA() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual size_t get_variable_index_from_variable_name(string var_name);
        virtual string get_variable_name_from_variable_index(size_t var_index);
        virtual double get_variable_with_index(size_t var_index);
        virtual double get_variable_with_name(string var_name);

        complex<double> get_initial_load_power_in_MVA() const;

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const IEEL& model);
        double P0, Q0;
        double P_alpha_1, P_alpha_2, P_alpha_3, Q_alpha_1, Q_alpha_2, Q_alpha_3;
        double P_n_power_1, P_n_power_2, P_n_power_3, Q_n_power_1, Q_n_power_2, Q_n_power_3;
        double P_Kf, Q_Kf;

};
#endif // IEEL_H
