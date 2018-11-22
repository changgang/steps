#ifndef MODEL_H
#define MODEL_H

#include "header/basic/base.h"
#include "header/basic/device_id.h"
#include "header/block/block.h"
#include "header/model/model_var_table.h"

class POWER_SYSTEM_DATABASE;
class DEVICE;

class MODEL : public BASE
{
    public:// common model level
        MODEL();
        virtual ~MODEL();

        void set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(string device_type);
        string get_allowed_device_type() const;

        void clear_model_data_table();
        void add_model_data_name_and_index_pair(string var_name, size_t var_index);
        size_t get_model_data_index(string var_name) const;
        string get_model_data_name(size_t var_index) const;
        bool is_model_data_exist(string var_name) const;
        bool is_model_data_exist(size_t var_index) const;

        void clear_model_internal_variable_table();
        void add_model_inernal_variable_name_and_index_pair(string var_name, size_t var_index);
        size_t get_model_inernal_variable_index(string var_name) const;
        string get_model_inernal_variable_name(size_t var_index) const;
        bool is_model_inernal_variable_exist(string var_name) const;
        bool is_model_inernal_variable_exist(size_t var_index) const;

        void set_device_id(DEVICE_ID did);
        DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        string get_device_name() const;

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

        virtual bool setup_model_with_steps_string(string data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;
        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_model_string() const = 0;

        virtual void prepare_model_data_table() = 0;
        virtual double get_model_data_with_name(string par_name) const = 0;
        virtual void set_model_data_with_name(string par_name, double value) = 0;
        void set_model_data_with_index(size_t index, double value);
        double get_model_data_with_index(size_t index);

        virtual void prepare_model_internal_variable_table() = 0;
        virtual double get_model_internal_variable_with_name(string var_name)= 0;
        double get_model_internal_variable_with_index(size_t index);

        virtual string get_dynamic_data_in_psse_format() const = 0;
        virtual string get_dynamic_data_in_bpa_format() const = 0;
        virtual string get_dynamic_data_in_steps_format() const = 0;

    public:
        virtual bool is_valid() const;
    private:
        string allowed_device_type;
        DEVICE* device_pointer;

        bool flag_model_initialized;
        bool flag_model_updated;

        bool flag_model_active;

        MODEL_VAR_TABLE model_data_table, model_internal_variable_table;
};

#endif // MODEL_H
