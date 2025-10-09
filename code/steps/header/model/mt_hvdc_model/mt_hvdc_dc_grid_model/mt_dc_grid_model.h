#ifndef MT_DC_GRID_MODEL_H
#define MT_DC_GRID_MODEL_H

#include "header/model/mt_hvdc_model/mt_hvdc_model.h"

class DC_BUS;
class DC_LINE;

class MT_DC_GRID_MODEL : public MT_HVDC_MODEL
{
    public:
        MT_DC_GRID_MODEL(STEPS& toolkit);
        ~MT_DC_GRID_MODEL();

        DC_BUS* get_dc_bus_pointer_with_bus_number(unsigned int bus) const;
        DC_LINE* get_dc_line_pointer_with_bus_number(unsigned int ibus, unsigned int jbus) const;
    public: // model type
        virtual string get_model_type() const;
    public: // specific model
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void check_limits_in_initialization() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        virtual double get_minimum_nonzero_time_constant_in_s() = 0;

        void set_model_data_with_index(unsigned int index, double value);
        double get_model_data_with_index(unsigned int index);

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;
        double get_model_internal_variable_with_index(unsigned int index);

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;


};

#endif // HLCC_MODEL_H
