#ifndef MODEL_H
#define MODEL_H

#include "header/basic/base.h"
#include "header/basic/device_id.h"
#include "header/block/block.h"

class POWER_SYSTEM_DATABASE;
class DEVICE;

class MODEL : public BASE
{
    public:// common model level
        //MODEL(POWER_SYSTEM_DATABASE* db, DEVICE_ID did);
        MODEL();
        virtual ~MODEL();

        //double get_dynamic_simulator_time_in_s() const;

        void set_allowed_device_type_CAN_ONLY_BE_CALLED_BY_SPECIFIC_MODEL_CONSTRUCTOR(string device_type);
        string get_allowed_device_type() const;

        void set_power_system_database_and_attached_device(POWER_SYSTEM_DATABASE* psdb, DEVICE_ID did);
        void set_device_id(DEVICE_ID did);
        DEVICE* get_device_pointer() const;
        DEVICE_ID get_device_id() const;
        string get_device_name() const;

        virtual double get_double_data_with_index(size_t index) const = 0;
        virtual double get_double_data_with_name(string par_name) const = 0;
        virtual void set_double_data_with_index(size_t index, double value) = 0;
        virtual void set_double_data_with_name(string par_name, double value) = 0;

        void set_flag_model_initialized_as_false();
        void set_flag_model_initialized_as_true();
        bool is_model_initialized() const;

        void set_flag_model_updated_as_false();
        void set_flag_model_updated_as_true();
        bool is_model_updated() const;

        void activate_model();
        void deactivate_model();

        bool is_model_active() const;

        size_t get_variable_index_from_variable_name(string var_name, vector<string>& MODEL_VARIABLE_TABLE) const;
        string get_variable_name_from_variable_index(size_t var_index, vector<string>& MODEL_VARIABLE_TABLE) const;
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

        virtual size_t get_variable_index_from_variable_name(string var_name)= 0;
        virtual string get_variable_name_from_variable_index(size_t var_index)= 0;
        virtual double get_variable_with_index(size_t var_index)= 0;
        virtual double get_variable_with_name(string var_name)= 0;

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
};

#endif // MODEL_H
