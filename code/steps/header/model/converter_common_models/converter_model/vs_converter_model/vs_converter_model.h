#ifndef VS_CONVERTER_MODEL_H
#define VS_CONVERTER_MODEL_H

#include "header/model/converter_common_models/converter_model/converter_model.h"
#include "header/STEPS.h"

class BUS;

class VS_CONVERTER_MODEL: public CONVERTER_MODEL
{
    public:
        VS_CONVERTER_MODEL(STEPS& toolkit);
        ~VS_CONVERTER_MODEL();

        void set_Pref_in_pu_based_on_mbase(double P);
        void set_Qref_in_pu_based_on_mbase(double Q);
        void set_Vref_in_pu(double V);
        void set_P_in_pu_based_on_mbase(double P);
        void set_Q_in_pu_based_on_mbase(double Q);
        void set_V_in_pu(double V);

        double get_Pref_in_pu_based_on_mbase() const;
        double get_Qref_in_pu_based_on_mbase() const;
        double get_Vref_in_pu() const;
        double get_P_in_pu_based_on_mbase() const;
        double get_Q_in_pu_based_on_mbase() const;
        double get_V_in_pu() const;

        void set_initial_virtual_voltage_in_pu(double V);
        void set_initial_virtual_angle_in_rad(double angle);
        double get_initial_virtual_voltage_in_pu() const;
        double get_initial_virtual_angle_in_rad() const;

        double get_virtual_frequency_deviation_in_pu() const;
        double get_virtual_frequency_in_pu() const;
        double get_virtual_frequency_in_Hz() const;
        double get_virtual_angle_in_deg() const;

        void initialize_base_frequency_and_base_angle_speed();
        double get_base_frequency_in_Hz() const;
        double get_base_angle_speed_in_radps() const;
    public:
        virtual string get_model_name() const = 0;

        virtual void initialize() = 0;
        virtual void run(DYNAMIC_MODE mode) = 0;

        virtual double get_virtual_frequency_deviation_in_Hz() const = 0;
        virtual double get_virtual_angle_in_rad() const = 0;
        virtual double get_virtual_voltage_in_pu() const = 0;
        virtual string get_standard_psse_string() const = 0;
    private:
        double P, Pref, Q, Qref, V, Vref;
        double angle0, V0;
        double fbase_Hz, wbase_radps;
};

#endif // VS_CONVERTER_MODEL_H
