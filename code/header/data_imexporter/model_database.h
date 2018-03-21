#ifndef MODEL_DATABASE_H
#define MODEL_DATABASE_H

#include "header/model/model.h"
#include "header/device/generator.h"
#include "header/device/pe_source.h"
#include "header/device/load.h"
#include "header/device/hvdc.h"

#include "header/model/source_model/sync_generator_model/sync_generator_model.h"
#include "header/model/compensator_model/compensator_model.h"
#include "header/model/exciter_model/exciter_model.h"
#include "header/model/stabilizer_model/stabilizer_model.h"
#include "header/model/turbine_governor_model/turbine_governor_model.h"
#include "header/model/load_model/load_model.h"
#include "header/model/load_relay_model/load_frequency_relay_model.h"
#include "header/model/load_relay_model/load_voltage_relay_model.h"

#include <string>

using namespace std;

class MODEL_DATABASE
{
    public:
        MODEL_DATABASE();
        ~MODEL_DATABASE();

        void clear_database();
        size_t get_model_table_count() const;
        size_t get_valid_model_count() const;

        void append_model(MODEL* model);
        void append_model2(const MODEL& model);


        MODEL* get_model_of_index(size_t index) const;
        void delete_model_of_index(size_t index);

        void set_all_models_initialize_flag_as_false();
        void set_all_models_initialize_flag_as_true();
        void set_all_models_update_flag_as_false();
        void set_all_models_update_flag_as_true();

        void initialize_all_models();
        void run_all_models(DYNAMIC_MODE mode);
    private:
        void set_generator_related_model_index(GENERATOR* device, string model_type, size_t model_index);
        void set_pe_source_related_model_index(PE_SOURCE* device, string model_type, size_t model_index);
        void set_load_related_model_index(LOAD* device, string model_type, size_t model_index);
        void set_hvdc_related_model_index(HVDC* device, string model_type, size_t model_index);

        void append_generator_related_model(const MODEL& model);
        void append_sync_generator_model(const SYNC_GENERATOR_MODEL& model);
        void append_compensator_model(const COMPENSATOR_MODEL& model);
        void append_exciter_model(const EXCITER_MODEL& model);
        void append_stabilizer_model(const STABILIZER_MODEL& model);
        void append_turbine_governor_model(const TURBINE_GOVERNOR_MODEL& model);


        void append_pe_source_related_model(const MODEL& model);

        void append_load_related_model(const MODEL& model);
        void append_load_model(const LOAD_MODEL& model);
        void append_load_voltage_relay_model(const LOAD_VOLTAGE_RELAY_MODEL& model);
        void append_load_frequency_relay_model(const LOAD_FREQUENCY_RELAY_MODEL& model);


        void append_hvdc_related_model(const MODEL& model);

        vector<MODEL*> models;
};
#endif // MODEL_DATABASE_H
