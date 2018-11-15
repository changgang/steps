#ifndef TGOV1_H
#define TGOV1_H

#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/block/proportional_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"

class TGOV1 : public TURBINE_GOVERNOR_MODEL
{
    public:
        TGOV1();
        TGOV1(const TGOV1&model);
        virtual ~TGOV1();
        virtual TGOV1& operator=(const TGOV1& model);
    public: // specific model level
        virtual string get_model_name() const;
        virtual double get_model_data_with_index(size_t index) const;
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_index(size_t index, double value);
        virtual void set_model_data_with_name(string par_name, double value);

        void set_R(double R);
        void set_T1_in_s(double T);
        void set_T2_in_s(double T);
        void set_T3_in_s(double T);
        void set_Valvemax_in_pu(double V);
        void set_Valvemin_in_pu(double V);
        void set_D(double D);

        double get_R() const;
        double get_T1_in_s() const;
        double get_T2_in_s() const;
        double get_T3_in_s() const;
        double get_Valvemax_in_pu() const;
        double get_Valvemin_in_pu() const;
        double get_D() const;

    public:
        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_mechanical_power_in_pu_based_on_mbase() const;
        virtual double get_mechanical_power_upper_limit_in_pu_based_on_mbase() const;
        virtual double get_mechanical_power_lower_limit_in_pu_based_on_mbase() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual double get_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const TGOV1& model);
        virtual void prepare_internal_variable_table();

        FIRST_ORDER_BLOCK governor;
        LEAD_LAG_BLOCK turbine;
        PROPORTIONAL_BLOCK damping;
};

#endif // TGOV1_H
