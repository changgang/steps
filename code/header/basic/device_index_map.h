#ifndef DEVICE_INDEX_MAP_H
#define DEVICE_INDEX_MAP_H

#include "header/basic/device_id.h"
#include <map>

using namespace std;

class DEVICE_INDEX_MAP
{
    public:
        DEVICE_INDEX_MAP();
        ~DEVICE_INDEX_MAP();

        void set_device_index(const DEVICE_ID& device_id, size_t index);

        void decrease_index_by_1_for_device_with_index_greater_than(size_t index);

        void clear();

        bool empty() const;

        size_t get_index_of_device(const DEVICE_ID& device_id) const;
        size_t operator[](const DEVICE_ID& device_id) const;

    private:
        bool is_given_device_of_the_same_type_as_existing_devices(const DEVICE_ID& device_id) const;

        map<DEVICE_ID, size_t> index_map;

};
#endif // DEVICE_INDEX_MAP_H
