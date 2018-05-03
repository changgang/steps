#ifndef WT_GENERATOR_H
#define WT_GENERATOR_H

#include<string>
#include<complex>
#include "header/device/source.h"
#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"
#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model.h"
#include "header/model/wind_speed_model/wind_speed_model.h"

using namespace std;

class POWER_SYSTEM_DATABASE;

class WT_GENERATOR : public SOURCE
{
    public:
        WT_GENERATOR(POWER_SYSTEM_DATABASE* db);
        virtual ~WT_GENERATOR();

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_device_name() const;

        virtual void report() const;
        virtual void save() const;

        void set_source_type(string type);
        string get_source_type() const;

        void set_model(const MODEL* model);
        void set_wt_generator_model(const WT_GENERATOR_MODEL* model);
        void set_wt_electrical_model(const WT_ELECTRICAL_MODEL* model);
        void set_wt_turbine_model(const WT_TURBINE_MODEL* model);
        void set_wt_pitch_model(const WT_PITCH_MODEL* model);
        void set_wind_speed_model(const WIND_SPEED_MODEL* model);

        WT_GENERATOR_MODEL* get_wt_generator_model();
        WT_ELECTRICAL_MODEL* get_wt_electrical_model();
        WT_TURBINE_MODEL* get_wt_turbine_model();
        WT_PITCH_MODEL* get_wt_pitch_model();
        WIND_SPEED_MODEL* get_wind_speed_model();

        void clear_wt_generator_model();

        WT_GENERATOR& operator=(const WT_GENERATOR& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
    private:
        string source_type;
        WT_GENERATOR_MODEL* wt_generator_model;

        WT_ELECTRICAL_MODEL* wt_electrical_model;
        WT_TURBINE_MODEL* wt_turbine_model;
        WT_PITCH_MODEL* wt_pitch_model;
        WIND_SPEED_MODEL* wind_speed_model;


};
#endif // WT_GENERATOR_H
