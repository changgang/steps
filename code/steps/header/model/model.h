#ifndef MODEL_H
#define MODEL_H

#include "header/basic/device_id.h"
#include "header/basic/time_series.h"
#include "header/block/block.h"
#include "header/model/model_var_table.h"

class POWER_SYSTEM_DATABASE;
class NONBUS_DEVICE;
class BUS;
class MODEL
{
    public:// common model level
        MODEL(STEPS& toolkit);
        STEPS& get_toolkit() const;
        void set_toolkit(STEPS& toolkit);
        virtual ~MODEL();

        virtual void destroy_manually_allocated_storage();

        void set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(STEPS_DEVICE_TYPE device_type);
        vector<STEPS_DEVICE_TYPE> get_allowed_device_types() const;
        bool has_allowed_device_type(STEPS_DEVICE_TYPE device_type) const;

        void set_model_float_parameter_count(unsigned int n);
        unsigned int get_model_float_parameter_count() const;

        void clear_model_data_table();
        void add_model_data_name_and_index_pair(string var_name, unsigned int var_index);
        unsigned int get_model_data_index(string var_name) const;
        string get_model_data_name(unsigned int var_index) const;
        bool is_model_data_exist(string var_name) const;
        bool is_model_data_exist(unsigned int var_index) const;

        void clear_model_internal_variable_table();
        void add_model_internal_variable_name_and_index_pair(string var_name, unsigned int var_index);
        unsigned int get_model_internal_variable_count() const;
        unsigned int get_model_internal_variable_index(string var_name) const;
        string get_model_internal_variable_name(unsigned int var_index) const;
        bool is_model_internal_variable_exist(string var_name) const;
        bool is_model_internal_variable_exist(unsigned int var_index) const;

        void set_device_id(DEVICE_ID did);
        NONBUS_DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        string get_compound_device_name() const;
        BUS* get_bus_pointer() const;
        //BUS* get_bus_pointer(CONVERTER_SIDE converter) const;
        //BUS* get_bus_pointer(LINE_SIDE side) const;
        //BUS* get_bus_pointer(TRANSFORMER_WINDING_SIDE winding) const;

        void set_flag_model_initialized_as_false();
        void set_flag_model_initialized_as_true();
        bool is_model_initialized() const;

        void set_flag_model_updated_as_false();
        void set_flag_model_updated_as_true();
        bool is_model_updated() const;

        void activate_model();
        void deactivate_model();

        bool is_model_active() const;
    public: // specific type level
        virtual string get_model_type() const = 0;

    public: // specific model level
        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
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

        void allocate_model_variables();
    private:
        STEPS* toolkit;
        STEPS_DEVICE_TYPE allowed_device_types[STEPS_MODEL_MAX_ALLOWED_DEVICE_COUNT];
        NONBUS_DEVICE* device_pointer;
        union
        {
            BUS* bus_pointer;
            BUS* rectifier_bus_pointer;
            BUS* sending_bus_pointer;
            BUS* primary_winding_bus_pointer;
        };
        /*union
        {
            BUS* inverter_bus_pointer;
            BUS* receiving_bus_pointer;
            BUS* secondary_winding_bus_pointer;
        };*/


        unsigned int n_parameters;

        bool flag_model_initialized;
        bool flag_model_updated;

        bool flag_model_active;

        MODEL_VAR_TABLE *model_data_table, *model_internal_variable_table;

        string *user_input_time_series_file;
        TIME_SERIES *user_input_time_series;
};

#endif // MODEL_H
