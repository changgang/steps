#ifndef ENERGY_STORAGE_H
#define ENERGY_STORAGE_H

#include <complex>
#include <string>
#include "header/device/source.h"
#include "header/model/energy_storage_model/es_converter_model/es_converter_model.h"
#include "header/model/energy_storage_model/es_electrical_model/es_electrical_model.h"
#include "header/model/energy_storage_model/es_soc_model/es_soc_model.h"
#include "header/model/energy_storage_model/es_vrt_model/es_vrt_model.h"
#include "header/model/energy_storage_model/es_relay_model/es_relay_model.h"
#include "header/model/energy_storage_model/es_auxiliary_model/es_auxiliary_model.h"
#include "header/model/energy_storage_model/es_rotor_model/es_rotor_model.h"
#include "header/model/energy_storage_model/es_primary_model/es_primary_model.h"

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

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        void set_es_converter_model(ES_CONVERTER_MODEL* model);
        void set_es_soc_model(ES_SOC_MODEL* model);
        void set_es_electrical_model(ES_ELECTRICAL_MODEL* model);
        void set_es_vrt_model(ES_VRT_MODEL* model);
        void set_es_relay_model(ES_RELAY_MODEL* model);
        void set_es_auxiliary_model(ES_AUXILIARY_MODEL* model);
        void set_es_rotor_model(ES_ROTOR_MODEL* model);
        void set_es_primary_model(ES_PRIMARY_MODEL* model);

        ES_CONVERTER_MODEL* get_es_converter_model() const;
        ES_SOC_MODEL* get_es_soc_model() const;
        ES_ELECTRICAL_MODEL* get_es_electrical_model() const;
        ES_VRT_MODEL* get_es_vrt_model() const;
        ES_RELAY_MODEL* get_es_relay_model() const;
        ES_AUXILIARY_MODEL* get_es_auxiliary_model() const;
        ES_ROTOR_MODEL* get_es_rotor_model() const;
        ES_PRIMARY_MODEL* get_es_primary_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual void report() const;
        virtual void save() const;

        virtual ENERGY_STORAGE& operator=(const ENERGY_STORAGE& estorage);

        void set_sequence_parameter_import_flag(bool flag);
        bool get_sequence_parameter_import_flag() const;

        void set_energy_storage_mode(ES_MODE m);
        ES_MODE get_energy_storage_mode() const;

        void set_manual_power_in_pu(double p);
        double get_manual_power_in_pu() const;

        void initialize_energy_storage_mode();
        void initialize_manual_power_in_pu();
    private:
        ES_CONVERTER_MODEL* es_converter_model;
        ES_SOC_MODEL* es_soc_model;
        ES_ELECTRICAL_MODEL* es_electrical_model;
        ES_VRT_MODEL* es_vrt_model;
        ES_RELAY_MODEL* es_relay_model;
        ES_AUXILIARY_MODEL* es_auxiliary_model;
        ES_ROTOR_MODEL* es_rotor_model;
        ES_PRIMARY_MODEL* es_primary_model;
    private:
        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();

        bool sequence_parameter_import_flag;

        ES_MODE mode;

        double manual_power;
};
#endif // ENERGY_STORAGE_H
