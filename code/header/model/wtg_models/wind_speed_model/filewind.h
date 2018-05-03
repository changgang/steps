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
        virtual ~FILEWIND();

        // inputs
        void set_wind_speed_serial_file(string file);
        string get_wind_speed_serial_file() const;
    public: // specific model level
        virtual string get_model_name() const;

        virtual double get_double_data_with_index(size_t index) const;
        virtual double get_double_data_with_name(string par_name) const;
        virtual void set_double_data_with_index(size_t index, double value);
        virtual void set_double_data_with_name(string par_name, double value);

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
        void load_wind_speed_from_file();
        void copy_from_const_model(const FILEWIND& model);
        string wind_speed_file;
        vector<double> time, wind_speed, wind_direction, wind_speed_sigma;
        size_t current_time_pointer;

};

#endif // FILEWIND_H
