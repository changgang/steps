#ifndef WT_GENERATOR_H
#define WT_GENERATOR_H

#include <string>
#include <complex>
#include "header/device/source.h"
#include "header/model/wtg_models/wt_generator_model/wt_generator_model.h"
#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_model.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_model.h"
#include "header/model/wtg_models/wt_turbine_model/wt_turbine_model.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_model.h"
#include "header/model/wtg_models/wind_speed_model/wind_speed_model.h"
#include "header/model/wtg_models/wt_relay_model/wt_relay_model.h"

using namespace std;

class WT_GENERATOR : public SOURCE
{
    public:
        WT_GENERATOR(STEPS& toolkit);
        virtual ~WT_GENERATOR();
        virtual void clear();
    public:
        virtual DEVICE_ID get_device_id() const;

        void set_generator_bus(unsigned int bus);
        unsigned int get_generator_bus() const;

        void set_number_of_lumped_wt_generators(unsigned int n);
        void set_rated_power_per_wt_generator_in_MW(double P);

        unsigned int get_number_of_lumped_wt_generators() const;
        double get_rated_power_per_wt_generator_in_MW() const;

        virtual void set_model(const MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        void set_wt_generator_model(WT_GENERATOR_MODEL* model);
        void set_wt_aerodynamic_model(WT_AERODYNAMIC_MODEL* model);
        void set_wt_turbine_model(WT_TURBINE_MODEL* model);
        void set_wt_electrical_model(WT_ELECTRICAL_MODEL* model);
        void set_wt_pitch_model(WT_PITCH_MODEL* model);
        void set_wind_speed_model(WIND_SPEED_MODEL* model);
        void set_wt_relay_model(WT_RELAY_MODEL* model);

        WT_GENERATOR_MODEL* get_wt_generator_model();
        WT_AERODYNAMIC_MODEL* get_wt_aerodynamic_model();
        WT_TURBINE_MODEL* get_wt_turbine_model();
        WT_ELECTRICAL_MODEL* get_wt_electrical_model();
        WT_PITCH_MODEL* get_wt_pitch_model();
        WIND_SPEED_MODEL* get_wind_speed_model();
        WT_RELAY_MODEL* get_wt_relay_model();

        virtual void run(DYNAMIC_MODE mode);
        virtual void report() const;
        virtual void save() const;

        virtual WT_GENERATOR& operator=(const WT_GENERATOR& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
    private:

        unsigned int number_of_lumped_wt_generators;
        double rated_power_per_wt_generator_in_MW;

        WT_GENERATOR_MODEL* wt_generator_model;
        WT_AERODYNAMIC_MODEL* wt_aerodynamic_model;
        WT_ELECTRICAL_MODEL* wt_electrical_model;
        WT_TURBINE_MODEL* wt_turbine_model;
        WT_PITCH_MODEL* wt_pitch_model;
        WIND_SPEED_MODEL* wind_speed_model;
        WT_RELAY_MODEL* wt_relay_model;
};
#endif // WT_GENERATOR_H
