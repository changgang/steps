#ifndef ESROT0_H
#define ESROT0_H

#include "header/model/energy_storage_model/es_rotor_model/es_rotor_model.h"
#include <cstdlib>

class ESROT0 : public ES_ROTOR_MODEL
{
    public:
        ESROT0(STEPS& toolkit);
        ESROT0(const ESROT0& model);
        virtual ~ESROT0();
        virtual ESROT0& operator=(const ESROT0&);
    public:

        void set_rotor_unlock_power_threshold_in_pu(double p);
        double get_rotor_unlock_power_threshold_in_pu() const;

        void set_rotor_braking_Damp(double D);
        double get_rotor_braking_Damp() const;

        void set_rotor_braking_mode(unsigned int mode);
        unsigned int get_rotor_braking_mode() const;

        virtual string get_model_name() const;

        virtual void check_rotor_lock_logic();
        virtual void check_rotor_braking_logic();

        virtual double get_rotor_speed_in_pu();

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

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
        void copy_from_const_model(const ESROT0& model);
        unsigned int rotor_braking_mode;
        double rotor_braking_Damp;
        double rotor_unlock_power_threshold_in_pu;
        bool discharge_to_out_service;
};

#endif // ESROT0_H
