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
        FILEWIND();
        FILEWIND(const FILEWIND& model);
        virtual ~FILEWIND();
        virtual FILEWIND& operator=(const FILEWIND& model);

        // inputs
        void set_wind_speed_serial_file(string file);
        string get_wind_speed_serial_file() const;
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
        virtual double get_model_internal_variable_with_name(string var_name);

        virtual string get_dynamic_data_in_psse_format() const;
        virtual string get_dynamic_data_in_bpa_format() const;
        virtual string get_dynamic_data_in_steps_format() const;
    private:
        void load_wind_speed_from_file();
        void copy_from_const_model(const FILEWIND& model);
        void search_wind_data_at_simulation_time();
        void set_previous_position(size_t pos);
        size_t get_previous_position() const;
        char wind_speed_file[STEPS_LONG_STRING_SIZE];
        vector<double> *time, *wind_speed, *wind_direction;
        double current_time, current_wind_speed, current_wind_direction;
        size_t previous_position;
};

#endif // FILEWIND_H
