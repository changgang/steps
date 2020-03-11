#ifndef MODEL_INDEX_H
#define MODEL_INDEX_H

#include <vector>
#include <string>
#include "device_id.h"

using namespace std;


class MODEL_INDEX
{
    public:
        MODEL_INDEX();

        void set_device_id(const DEVICE_ID&);
        void set_icon_index(size_t);
        void set_con_index(size_t);
        void set_state_index(size_t);
        void set_var_index(size_t);

        DEVICE_ID get_device_id() const;
        size_t get_icon_index() const;
        size_t get_con_index() const;
        size_t get_state_index() const;
        size_t get_var_index() const;
        size_t get_device_index() const;

    private:
        void set_device_index();

    private:
        DEVICE_ID device_id;
        size_t icon_index;
        size_t con_index;
        size_t state_index;
        size_t var_index;
        size_t device_index;
};

#endif // MODEL_INDEX_H
