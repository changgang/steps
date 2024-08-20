#ifndef FTEMP_H
#define FTEMP_H

#include "header/model/pvu_models/pv_temperature_model/pv_temperature_model.h"
class FTEMP : public PV_TEMPERATURE_MODEL
{
    public:
        FTEMP(STEPS& toolkit);
        FTEMP(const FTEMP& model);
        virtual ~FTEMP();
        virtual FTEMP& operator=(const FTEMP& model);

        // inputs
        void set_temperature_serial_file(string file);
        string get_temperature_serial_file() const;
        void load_temperature_from_file();
    public: // specific model level
        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void prepare_model_data_table();
        virtual void prepare_model_internal_variable_table();

        virtual void setup_block_toolkit_and_parameters();

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual double get_T_in_C_deg();

        virtual void check();
        virtual void clear();
        virtual void report();
        virtual void save();
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const;

        virtual double get_model_data_with_name(string par_name) const;
        virtual void set_model_data_with_name(string par_name, double value);
        virtual double get_minimum_nonzero_time_constant_in_s();
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void clear_temperature_serial_file();
        void clear_temperature_data();
        void copy_from_const_model(const FTEMP& model);
        void search_temperature_data_at_simulation_time();
        void set_previous_position(unsigned int pos);
        unsigned int get_previous_position() const;
        unsigned int get_temperature_record_count() const;

        char temperature_file[STEPS_LONG_STRING_SIZE];
        double temperature_data[STEPS_MAX_TEMPERATURE_RECORD_SIZE][2];
        double current_time, current_temperature;
        unsigned int previous_position;
};

#endif // FTEMP_H
