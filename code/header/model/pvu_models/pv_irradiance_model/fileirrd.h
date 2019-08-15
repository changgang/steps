#ifndef FILEIRRAD_H
#define FILEIRRAD_H

#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_model.h"
class FILEIRRAD : public PV_IRRADIANCE_MODEL
{
    public:
        FILEIRRAD();
        FILEIRRAD(const FILEIRRAD& model);
        virtual ~FILEIRRAD();
        virtual FILEIRRAD& operator=(const FILEIRRAD& model);

        // inputs
        void set_solar_irradiance_serial_file(string file);
        string get_solar_irradiance_serial_file() const;
    public: // specific model level
        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_solar_irradiance_in_pu();
        virtual double get_solar_irradiance_direction_in_deg();

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string() const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void copy_from_const_model(const FILEIRRAD& model);
        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();
};

#endif // FILEIRRAD_H
