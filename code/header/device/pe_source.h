#ifndef PE_SOURCE_H
#define PE_SOURCE_H

#include<string>
#include<complex>
#include "header/device/source.h"
#include "header/model/source_model/pe_source_model/pe_source_model.h"

using namespace std;

class POWER_SYSTEM_DATABASE;

class PE_SOURCE : public SOURCE
{
    public:
        PE_SOURCE(POWER_SYSTEM_DATABASE* db);
        virtual ~PE_SOURCE();

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        virtual void report() const;
        virtual void save() const;

        void set_model(const MODEL* model);
        void set_pe_source_model(const PE_SOURCE_MODEL* model);

        PE_SOURCE_MODEL* get_pe_source_model();

        void clear_pe_source_model();

        PE_SOURCE& operator=(const PE_SOURCE& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
    private:
        PE_SOURCE_MODEL* pe_source_model;

};
#endif // PE_SOURCE_H
