#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include <complex>
#include "header/device/source.h"
#include "header/model/sg_models/sync_generator_model/sync_generator_model.h"
#include "header/model/sg_models/compensator_model/compensator_model.h"
#include "header/model/sg_models/exciter_model/exciter_model.h"
#include "header/model/sg_models/stabilizer_model/stabilizer_model.h"
#include "header/model/sg_models/turbine_governor_model/turbine_governor_model.h"
#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_model.h"

using namespace std;

class GENERATOR : public SOURCE
{
    public:
        GENERATOR(STEPS& toolkit);
        virtual ~GENERATOR();

        void set_generator_bus(unsigned int bus);
        void set_generator_impedance_in_pu(const complex<double>& z_pu);

        unsigned int get_generator_bus() const;
        complex<double> get_generator_impedance_in_pu() const;

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;
        //virtual string get_compound_device_name() const;

        virtual void set_model(const MODEL* model);
        virtual MODEL* get_model_of_type(string model_type);

        void set_sync_generator_model(SYNC_GENERATOR_MODEL* model);
        void set_compensator_model(COMPENSATOR_MODEL* model);
        void set_exciter_model(EXCITER_MODEL* model);
        void set_stabilizer_model(STABILIZER_MODEL* model);
        void set_turbine_governor_model(TURBINE_GOVERNOR_MODEL* model);
        void set_turbine_load_controller_model(TURBINE_LOAD_CONTROLLER_MODEL* model);

        SYNC_GENERATOR_MODEL* get_sync_generator_model() const;
        COMPENSATOR_MODEL* get_compensator_model() const;
        EXCITER_MODEL* get_exciter_model() const;
        STABILIZER_MODEL* get_stabilizer_model() const;
        TURBINE_GOVERNOR_MODEL* get_turbine_governor_model() const;
        TURBINE_LOAD_CONTROLLER_MODEL* get_turbine_load_controller_model() const;

        virtual void run(DYNAMIC_MODE mode);

        virtual void report() const;
        virtual void save() const;

        virtual GENERATOR& operator=(const GENERATOR& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();
    private:
        SYNC_GENERATOR_MODEL* sync_generator_model;
        COMPENSATOR_MODEL* compensator_model;
        EXCITER_MODEL* exciter_model;
        STABILIZER_MODEL* stabilizer_model;
        TURBINE_GOVERNOR_MODEL* turbine_governor_model;
        TURBINE_LOAD_CONTROLLER_MODEL* turbine_load_controller_model;
};
#endif // GENERATOR_H
