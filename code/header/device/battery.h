#ifndef PE_SOURCE_H
#define PE_SOURCE_H

#include<string>
#include<complex>
#include "header/device/source.h"
#include "header/model/source_model/pe_source_model/pe_source_model.h"
#include "header/model/pe_electrical_control_model/pe_electrical_control_model.h"
#include "header/model/wind_turbine_model/wind_turbine_model.h"
#include "header/model/wind_pitch_control_model/wind_pitch_control_model.h"
#include "header/model/wind_speed_model/wind_speed_model.h"

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

        void set_source_type(string type);
        string get_source_type() const;

        void set_model(const MODEL* model);
        void set_pe_source_model(const PE_SOURCE_MODEL* model);
        void set_pe_electrical_control_model(const PE_ELECTRICAL_CONTROL_MODEL* model);
        void set_wind_turbine_model(const WIND_TURBINE_MODEL* model);
        void set_wind_pitch_control_model(const WIND_PITCH_CONTROL_MODEL* model);
        void set_wind_speed_model(const WIND_SPEED_MODEL* model);

        PE_SOURCE_MODEL* get_pe_source_model();
        PE_ELECTRICAL_CONTROL_MODEL* get_pe_electrical_control_model();
        WIND_TURBINE_MODEL* get_wind_turbine_model();
        WIND_PITCH_CONTROL_MODEL* get_wind_pitch_control_model();
        WIND_SPEED_MODEL* get_wind_speed_model();

        void clear_pe_source_model();

        PE_SOURCE& operator=(const PE_SOURCE& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
    private:
        string source_type;
        PE_SOURCE_MODEL* pe_source_model;

        PE_ELECTRICAL_CONTROL_MODEL* pe_electrical_control_model;
        WIND_TURBINE_MODEL* wind_turbine_model;
        WIND_PITCH_CONTROL_MODEL* wind_pitch_control_model;
        WIND_SPEED_MODEL* wind_speed_model;


};
#endif // PE_SOURCE_H
