#ifndef DYNAMIC_MODEL_DATABASE_H
#define DYNAMIC_MODEL_DATABASE_H

#include "header/basic/base.h"
#include <vector>
using namespace std;

class MODEL;
class SOURCE;

class DYNAMIC_MODEL_DATABASE : public BASE
{
    public:
        DYNAMIC_MODEL_DATABASE();
        ~DYNAMIC_MODEL_DATABASE();

        virtual void check();
        virtual void clear();
    public:
        void add_model(MODEL* model);
        void remove_the_last_model();
    private:
        size_t get_model_size(MODEL* model) const;
        void common_set_model(MODEL* model, size_t model_size);
        void shrink_model_starting_position_table_at_position(void *pos);
        bool load_related_model_is_to_update(MODEL* old_model, MODEL* new_model);

        void *model_warehouse;
        size_t warehouse_capacity;

        bool is_full;
        size_t occupied_warehouse_capacity;
        vector<size_t> model_starting_position_table;
    private:
        virtual bool is_valid() const;
};
#endif // DYNAMIC_MODEL_DATABASE_H
