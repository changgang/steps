#ifndef FILEWIND_H
#define FILEWIND_H

#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"
class FILEWIND : public WIND_SPEED_MODEL
{
    /*
    wind speed model:
    inputs:
        file: csv file storing wind speed data
    output:
        vwind: wind speed in mps
    */
    public:
        FILEWIND(STEPS& toolkit);
        FILEWIND(const FILEWIND& model);
        virtual ~FILEWIND();
        virtual FILEWIND& operator=(const FILEWIND& model);

        // inputs
        void set_wind_speed_serial_file(string file);
        string get_wind_speed_serial_file() const;
        void load_wind_speed_from_file();
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
        virtual double get_wind_speed_in_pu();
        virtual double get_wind_direction_in_deg();

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
        void clear_wind_speed_serial_file();
        void clear_wind_data();
        void copy_from_const_model(const FILEWIND& model);
        void search_wind_data_at_simulation_time();
        void set_previous_position(unsigned int pos);
        unsigned int get_previous_position() const;
        unsigned int get_wind_record_count() const;

        char wind_speed_file[STEPS_LONG_STRING_SIZE];
        double wind_data[STEPS_MAX_WIND_RECORD_SIZE][3];
        double current_time, current_wind_speed, current_wind_direction;
        unsigned int previous_position;
};

#endif // FILEWIND_H
