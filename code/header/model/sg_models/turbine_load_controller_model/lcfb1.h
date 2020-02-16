#ifndef LCFB1_H
#define LCFB1_H

#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model.h"
#include "header/block/first_order_block.h"
#include "header/block/differential_block.h"

class LCFB1 : public TURBINE_LOAD_CONTROLLER_MODEL
{
    public:
        LCFB1();
        LCFB1(const LCFB1& model);
        virtual ~LCFB1();
        virtual LCFB1& operator=(const LCFB1& model);

        virtual string get_model_name() const;
    public:
        void set_frequency_regulation_flag(bool flag);
        void set_power_regulation_flag(bool flag);
        void set_Fb(double f);
        void set_Tpelec_in_s(double t);
        void set_db(double db);
        void set_Emax(double e);
        void set_Kp(double k);
        void set_Ki(double k);
        void set_Irmax(double i);

        bool get_frequency_regulation_flag() const;
        bool get_power_regulation_flag() const;
        double get_Fb() const;
        double get_Tpelec_in_s() const;
        double get_db() const;
        double get_Emax() const;
        double get_Kp() const;
        double get_Ki() const;
        double get_Irmax() const;

    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_mechanical_power_reference_in_pu_based_on_mbase() const;

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const LCFB1& model);

        void set_Pref0(double p);
        void set_Pelec0(double p);
        double get_Pref0() const;
        double get_Pelec0() const;

        bool frequency_regulation_flag, power_regulation_flag;
        double Fb;
        FIRST_ORDER_BLOCK Pelec_sensor;
        double db, Emax;
        double Kp;
        INTEGRAL_BLOCK error_integrator;
        double Pref0, Pelec0;
};

#endif // LCFB1_H
