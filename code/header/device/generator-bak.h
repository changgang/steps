#ifndef GENERATOR_H
#define GENERATOR_H

#include<string>
#include<complex>
#include "header/device/source.h"

using namespace std;

class POWER_SYSTEM_DATABASE;

class GENERATOR : public SOURCE
{
    public:
        GENERATOR(POWER_SYSTEM_DATABASE* db);
        ~GENERATOR();

        virtual void update_model_power_system_databse();

        void set_generator_bus(size_t bus);
        void set_generator_impedance_in_pu(complex<double> z_pu);

        size_t get_generator_bus() const;
        complex<double> get_generator_impedance_in_pu() const;

        virtual void clear();

        virtual DEVICE_ID get_device_id() const;
        virtual string get_device_name() const;

        void set_sync_generator_model_index(size_t index);
        void set_exciter_model_index(size_t index);
        void set_stabilizer_model_index(size_t index);
        void set_turbine_governor_model_index(size_t index);

        size_t get_sync_generator_model_index() const;
        size_t get_exciter_model_index() const;
        size_t get_stabilizer_model_index() const;
        size_t get_turbine_governor_model_index() const;

        /*void set_generator_model(SYNC_GENERATOR_MODEL* model);
        void set_exciter_model(EXCITER_MODEL* model);
        void set_stabilizer_model(STABILIZER_MODEL* model);
        void set_turbine_governor_model(TURBINE_GOVERNOR_MODEL* model);

        SYNC_GENERATOR_MODEL* get_generator_model();
        EXCITER_MODEL* get_exciter_model();
        STABILIZER_MODEL* get_stabilizer_model();
        TURBINE_GOVERNOR_MODEL* get_turbine_governor_model();*/

        /*virtual void clear_all_models();
        void clear_generator_model();
        void clear_exciter_model();
        void clear_stabilizer_model();
        void clear_turbine_governor_model();*/

        /*virtual void check_all_models();
        void check_generator_model();
        void check_exciter_model();
        void check_stabilizer_model();
        void check_turbine_governor_model();*/

        virtual void report() const;
        virtual void save() const;

        GENERATOR& operator=(const GENERATOR& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);

        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();

    private:
        void check_and_initialize_generator_model_source_impedance();
        void set_stabilizer_input_signals();
        void set_stabilizer_input_signal_of_slot(size_t i);


        size_t sync_generator_model_index, exciter_model_index, stabilizer_model_index, turbine_governor_model_index;

        /*SYNC_GENERATOR_MODEL* generator_model;
        EXCITER_MODEL* exciter_model;
        STABILIZER_MODEL* stabilizer_model;
        TURBINE_GOVERNOR_MODEL* turbine_governor_model;*/

};
#endif // GENERATOR_H
