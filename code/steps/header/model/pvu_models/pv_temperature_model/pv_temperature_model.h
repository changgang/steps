#ifndef PV_TEMPERATURE_MODEL_H
#define PV_TEMPERATURE_MODEL_H

#include "header/model/pvu_models/pvu_model.h"

class PV_TEMPERATURE_MODEL : public PVU_MODEL
{
    public:
        PV_TEMPERATURE_MODEL(STEPS& toolkit);
        virtual ~PV_TEMPERATURE_MODEL();

        virtual string get_model_type() const;
    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual double get_T_in_C_deg() = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;

        double get_Tref_in_C_deg() const;
    private:

};

#endif // PV_TEMPERATURE_MODEL_H
