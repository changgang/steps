#ifndef ESP0_H
#define ESP0_H

#include "header/model/energy_storage_model/es_primary_model/es_primary_model.h"
#include "header/model/common_model/primary_model/prim0.h"
#include <cstdlib>

class ESP0 : public ES_PRIMARY_MODEL
{
    public:
        ESP0(STEPS& toolkit);
        ESP0(const ESP0& model);
        virtual ~ESP0();
        virtual ESP0& operator=(const ESP0&);
        string get_model_name() const;
    public:
        void set_Tr_in_s(double T);
        void set_Ka(double k);
        void set_Eb(double k);
        void set_Bp(double k);
        void set_Kp(double k);
        void set_Ki(double k);
        void set_INTmax(double k);
        void set_INTmin(double k);
        void set_PImax(double k);
        void set_PImin(double k);
        void set_umax(double k);
        void set_umin(double k);
        void set_Ts_in_s(double T);
        void set_Twg_in_s(double T);

        double get_Tr_in_s() const;
        double get_Ka() const;
        double get_Eb() const;
        double get_Bp() const;
        double get_Kp() const;
        double get_Ki() const;
        double get_INTmax() const;
        double get_INTmin() const;
        double get_PImax() const;
        double get_PImin() const;
        double get_umax() const;
        double get_umin() const;
        double get_Ts_in_s() const;
        double get_Twg_in_s() const;

        virtual double get_valve_in_pu() const;
        virtual double get_delta_Y_in_pu() const;
        virtual double get_delta_Yref_in_pu() const;
        virtual double get_mechanical_power_in_pu_on_mbase() const;
        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void check_limits_in_initialization();
        virtual void run(DYNAMIC_MODE mode);
        virtual void initialize_Pmech_as_zero();

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual void prepare_model_data_table();
        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();

        virtual void prepare_model_internal_variable_table();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:

        void copy_from_const_model(const ESP0& model);
        PRIM0 gov;

};

#endif // ESP0_H
