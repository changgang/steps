#ifndef ENERGY_STORAGE_H
#define ENERGY_STORAGE_H

#include <complex>
#include <string>
#include "header/device/source.h"
#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/model/energy_storage_model/es_electrical_model/es_electrical_model.h"
#include "header/model/energy_storage_model/es_battery_model/es_battery_model.h"
#include "header/model/energy_storage_model/es_vrt_model/es_vrt_model.h"
#include "header/model/energy_storage_model/es_relay_model/es_relay_model.h"

using namespace std;

class ENERGY_STORAGE : public SOURCE
{
    public:
        ENERGY_STORAGE(STEPS& toolkit);
        virtual ~ENERGY_STORAGE();

        void set_energy_storage_bus(unsigned int bus);
        void set_energy_storage_impedance_in_pu(const complex<double>& z_pu);

        unsigned int get_energy_storage_bus() const;
        complex<double> get_energy_storage_impedance_in_pu() const;

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        void set_es_converter_model(ES_CONVERTER_MODEL* model);
        void set_es_battery_model(ES_BATTERY_MODEL* model);
        void set_es_electrical_model(ES_ELECTRICAL_MODEL* model);
        void set_es_vrt_model(ES_VRT_MODEL* model);
        void set_es_relay_model(ES_RELAY_MODEL* model);

        ES_CONVERTER_MODEL* get_es_converter_model() const;
        ES_BATTERY_MODEL* get_es_battery_model() const;
        ES_ELECTRICAL_MODEL* get_es_electrical_model() const;
        ES_VRT_MODEL* get_es_vrt_model() const;
        ES_RELAY_MODEL* get_es_relay_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual void report() const;
        virtual void save() const;

        virtual ENERGY_STORAGE& operator=(const ENERGY_STORAGE& estorage);

        void set_sequence_parameter_import_flag(bool flag);
        bool get_sequence_parameter_import_flag() const;
    private:
        ES_CONVERTER_MODEL* es_converter_model;
        ES_BATTERY_MODEL* es_battery_model;
        ES_ELECTRICAL_MODEL* es_electrical_model;
        ES_VRT_MODEL* es_vrt_model;
        ES_RELAY_MODEL* es_relay_model;
    private:
        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();

        bool sequence_parameter_import_flag;
};
#endif // ENERGY_STORAGE_H
