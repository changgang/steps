#ifndef ENERGY_STORAGE_H
#define ENERGY_STORAGE_H

#include <complex>
#include <string>
#include "header/device/source.h"
#include "header/model/energy_storage_model/energy_storage_model.h"

using namespace std;

class ENERGY_STORAGE : public SOURCE
{
    public:
        ENERGY_STORAGE();
        virtual ~ENERGY_STORAGE();

        void set_energy_storage_bus(size_t bus);
        void set_energy_storage_impedance_in_pu(const complex<double>& z_pu);

        size_t get_energy_storage_bus() const;
        complex<double> get_energy_storage_impedance_in_pu() const;

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        virtual void set_model(const MODEL* model);

        void set_energy_storage_model(const ENERGY_STORAGE_MODEL* model);

        void clear_energy_storage_model();

        ENERGY_STORAGE_MODEL* get_energy_storage_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual void report() const;
        virtual void save() const;

        virtual ENERGY_STORAGE& operator=(const ENERGY_STORAGE& estorage);

    private:
        ENERGY_STORAGE_MODEL* energy_storage_model;

    private:
        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
};
#endif // ENERGY_STORAGE_H
