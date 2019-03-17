#ifndef SEXS_H
#define SEXS_H

#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"
class SEXS: public EXCITER_MODEL
{
    public:
        SEXS();
        SEXS(const SEXS& model);
        virtual ~SEXS();
        virtual SEXS& operator=(const SEXS& model);
    public: // specific exciter
        virtual string get_model_name() const;

        void set_TA_in_s(double T);
        void set_TB_in_s(double T);
        void set_K(double K);
        void set_TE_in_s(double T);
        void set_Efdmax_in_pu(double efd);
        void set_Efdmin_in_pu(double efd);

        double get_TA_in_s() const;
        double get_TB_in_s() const;
        double get_K() const;
        double get_TE_in_s() const;
        double get_Efdmax_in_pu() const;
        double get_Efdmin_in_pu() const;
    public:
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_excitation_voltage_in_pu() const;
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
        void copy_from_const_model(const SEXS& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        LEAD_LAG_BLOCK phase_tuner;
        FIRST_ORDER_BLOCK exciter;

};

#endif // EXCITER_MODEL_H
