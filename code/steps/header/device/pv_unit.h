#ifndef PV_UNIT_H
#define PV_UNIT_H

#include <string>
#include <complex>
#include "header/device/source.h"
#include "header/model/pvu_models/pv_converter_model/pv_converter_model.h"
#include "header/model/pvu_models/pv_panel_model/pv_panel_model.h"
#include "header/model/pvu_models/pv_electrical_model/pv_electrical_model.h"
#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_model.h"

using namespace std;

class PV_UNIT : public SOURCE
{
    public:
        PV_UNIT(STEPS& toolkit);
        virtual ~PV_UNIT();
        virtual void clear();
    public:
        virtual DEVICE_ID get_device_id() const;

        void set_unit_bus(unsigned int bus);
        unsigned int get_unit_bus() const;

        void set_number_of_lumped_pv_units(unsigned int n);
        void set_rated_power_per_pv_unit_in_MW(double P);

        unsigned int get_number_of_lumped_pv_units() const;
        double get_rated_power_per_pv_unit_in_MW() const;

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        void set_pv_converter_model(PV_CONVERTER_MODEL* model);
        void set_pv_panel_model(PV_PANEL_MODEL* model);
        void set_pv_electrical_model(PV_ELECTRICAL_MODEL* model);
        void set_pv_irradiance_model(PV_IRRADIANCE_MODEL* model);

        PV_CONVERTER_MODEL* get_pv_converter_model() const;
        PV_PANEL_MODEL* get_pv_panel_model() const;
        PV_ELECTRICAL_MODEL* get_pv_electrical_model() const;
        PV_IRRADIANCE_MODEL* get_pv_irradiance_model() const;

        virtual void run(DYNAMIC_MODE mode);
        virtual void report() const;
        virtual void save() const;

        virtual PV_UNIT& operator=(const PV_UNIT& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
    private:
        unsigned int number_of_lumped_pv_units;
        double rated_power_per_pv_unit_in_MW;

        PV_CONVERTER_MODEL* pv_converter_model;
        PV_PANEL_MODEL* pv_panel_model;
        PV_ELECTRICAL_MODEL* pv_electrical_model;
        PV_IRRADIANCE_MODEL* pv_irradiance_model;
};
#endif // PV_UNIT_H
