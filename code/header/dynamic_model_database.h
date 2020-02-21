#ifndef DYNAMIC_MODEL_DATABASE_H
#define DYNAMIC_MODEL_DATABASE_H

#include "header/model/model.h"
#include <vector>
using namespace std;

class MODEL;
class SOURCE;

class DYNAMIC_MODEL_DATABASE
{
    public:
        DYNAMIC_MODEL_DATABASE(STEPS& toolkit);
        ~DYNAMIC_MODEL_DATABASE();

        STEPS& get_toolkit() const;

        void check();
        void clear();
    public:
        void add_model(MODEL* model);
        void remove_the_last_model();

        void check_device_model_minimum_time_constants();

        unsigned int get_memory_usage_in_bytes();
    private:
        unsigned int get_model_size(MODEL* model) const;
        void common_set_model(MODEL* model, unsigned int model_size);
        void shrink_model_starting_position_table_at_position(void *pos);
        bool load_related_model_is_to_update(MODEL* old_model, MODEL* new_model);

        STEPS* toolkit;

        char *model_warehouse;
        unsigned int warehouse_capacity;

        bool is_full;
        unsigned int occupied_warehouse_capacity;
        vector<unsigned int> model_starting_position_table;
};
#endif // DYNAMIC_MODEL_DATABASE_H
