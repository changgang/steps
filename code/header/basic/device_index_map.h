#ifndef DEVICE_INDEX_MAP_H
#define DEVICE_INDEX_MAP_H

#include "header/basic/device_id.h"
#include <functional>
#include <map>
#include <unordered_map>

using namespace std;

class DEVICE_INDEX_MAP
{
    public:
        DEVICE_INDEX_MAP();
        ~DEVICE_INDEX_MAP();

        void set_device_index(const DEVICE_ID& device_id, unsigned int index);
        void swap_device_index(const DEVICE_ID& device1, const DEVICE_ID& device2);

        void decrease_index_by_1_for_device_with_index_greater_than(unsigned int index);

        void clear();

        bool empty() const;

        unsigned int get_index_of_device(const DEVICE_ID& device_id) const;
        unsigned int operator[](const DEVICE_ID& device_id) const;

        unsigned int get_map_size() const;
    private:
        bool is_given_device_of_the_same_type_as_existing_devices(const DEVICE_ID& device_id) const;
        bool is_given_device_of_the_same_type_as_existing_devices_map(const DEVICE_ID& device_id) const;
        bool is_given_device_of_the_same_type_as_existing_devices_umap(const DEVICE_ID& device_id) const;

        void set_device_index_map(const DEVICE_ID& device_id, unsigned int index);
        void set_device_index_umap(const DEVICE_ID& device_id, unsigned int index);

        void decrease_index_by_1_for_device_with_index_greater_than_map(unsigned int index);
        void decrease_index_by_1_for_device_with_index_greater_than_umap(unsigned int index);

        unsigned int get_index_of_device_map(const DEVICE_ID& device_id) const;
        unsigned int get_index_of_device_umap(const DEVICE_ID& device_id) const;

        map<DEVICE_ID, unsigned int> index_map;
        unordered_map<DEVICE_ID, unsigned int> index_umap;
};
#endif // DEVICE_INDEX_MAP_H
