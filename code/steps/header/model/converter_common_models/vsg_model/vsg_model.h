#ifndef VSG_MODEL_H
#define VSG_MODEL_H

#include "header/model/model.h"
#include "header/device/bus.h"
#include <complex>

class SOURCE;
class WT_GENERATOR;
class PV_UNIT;
class ENERGY_STORAGE;

class VSG_MODEL : public MODEL
{
    /*
    input: device active, reactive power and terminal voltage
    output: voltage magnitude and angle of internal voltage source
    */
    public:
        VSG_MODEL(STEPS& toolkit);
        virtual ~VSG_MODEL();
        virtual string get_model_type() const;


        void set_Pref_in_pu_based_on_mbase(double P);
        void set_Qref_in_pu_based_on_mbase(double Q);
        void set_Vref_in_pu(double V);

        double get_Pref_in_pu_based_on_mbase() const;
        double get_Qref_in_pu_based_on_mbase() const;
        double get_Vref_in_pu() const;

        double get_virtual_speed_in_pu() const;
    public: // specific model level
        virtual double get_virtual_speed_deviation_in_pu() const = 0;
        virtual double get_virtual_angle_in_rad() const = 0;
        virtual double get_virtual_voltage_in_pu() const = 0;

        virtual string get_model_name() const = 0;

        virtual bool setup_model_with_steps_string_vector(vector<string>& data) = 0;
        virtual bool setup_model_with_psse_string(string data) = 0;
        virtual bool setup_model_with_bpa_string(string data) = 0;

        virtual void setup_block_toolkit_and_parameters() = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual void check() = 0;
        virtual void clear() = 0;
        virtual void report() = 0;
        virtual void save() = 0;
        virtual string get_standard_psse_string(bool export_internal_bus_number=false) const = 0;

    private:
        double Pref, Qref, Vref;
};
#endif // VSG_MODEL_H
