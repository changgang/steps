#ifndef WBLINE_H
#define WBLINE_H

#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"
#include "header/block/proportional_block.h"
#include "header/block/first_order_block.h"
#include "header/block/lead_lag_block.h"

class WBLINE : public WIND_SPEED_MODEL
{
    public:
        WBLINE();
        WBLINE(const WBLINE&model);
        virtual ~WBLINE();
        virtual WBLINE& operator=(const WBLINE& model);
    public: // specific model level
        virtual string get_model_name() const;
        virtual double get_double_data_with_index(size_t index) const;
        virtual double get_double_data_with_name(string par_name) const;
        virtual void set_double_data_with_index(size_t index, double value);
        virtual void set_double_data_with_name(string par_name, double value);

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
        virtual double get_wind_speed_in_mps() const;
        virtual double get_wind_speed_in_pu() const;
        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_model_string() const;

        virtual size_t get_variable_index_from_variable_name(string var_name);
        virtual string get_variable_name_from_variable_index(size_t var_index);
        virtual double get_variable_with_index(size_t var_index);
        virtual double get_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const WBLINE& model);
        FIRST_ORDER_BLOCK governor;
        LEAD_LAG_BLOCK turbine;
        PROPORTIONAL_BLOCK damping;
};

#endif // WBLINE_H
