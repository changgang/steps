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

        virtual void set_model(MODEL* model);
        virtual MODEL* get_model_of_type(string model_type, unsigned int index=0);

        void set_wt_generator_model(WT_GENERATOR_MODEL* model);
        void set_wt_aerodynamic_model(WT_AERODYNAMIC_MODEL* model);
        void set_wt_turbine_model(WT_TURBINE_MODEL* model);
        void set_wt_electrical_model(WT_ELECTRICAL_MODEL* model);
        void set_wt_pitch_model(WT_PITCH_MODEL* model);
        void set_wind_speed_model(WIND_SPEED_MODEL* model);
        void set_wt_relay_model(WT_RELAY_MODEL* model);

        WT_GENERATOR_MODEL* get_wt_generator_model() const;
        WT_AERODYNAMIC_MODEL* get_wt_aerodynamic_model() const;
        WT_TURBINE_MODEL* get_wt_turbine_model() const;
        WT_ELECTRICAL_MODEL* get_wt_electrical_model() const;
        WT_PITCH_MODEL* get_wt_pitch_model() const;
        WIND_SPEED_MODEL* get_wind_speed_model() const;
        WT_RELAY_MODEL* get_wt_relay_model() const;

        virtual void run(DYNAMIC_MODE mode);
        virtual void report() const;
        virtual void save() const;

        virtual WT_GENERATOR& operator=(const WT_GENERATOR& gen);

        virtual complex<double> get_complex_internal_voltage_in_pu_in_xy_axis() const;
        virtual complex<double> get_source_dynamic_current_in_pu_based_on_system_base_power();

        void set_positive_sequence_resistance_in_pu(double r);
        void set_positive_sequence_syncronous_reactance_in_pu(double x);
        void set_positive_sequence_transient_reactance_in_pu(double x);
        void set_positive_sequence_subtransient_reactance_in_pu(double x);
        void set_negative_sequence_resistance_in_pu(double r);
        void set_negative_sequence_reactance_in_pu(double x);
        void set_zero_sequence_resistance_in_pu(double r);
        void set_zero_sequence_reactance_in_pu(double x);
        void set_grounding_resistance_in_pu(double r);
        void set_grounding_reactance_in_pu(double x);
        double get_positive_sequence_resistance_in_pu() const;
        double get_positive_sequence_syncronous_reactance_in_pu() const;
        double get_positive_sequence_transient_reactance_in_pu() const;
        double get_positive_sequence_subtransient_reactance_in_pu() const;
        double get_negative_sequence_resistance_in_pu() const;
        double get_negative_sequence_reactance_in_pu() const;
        double get_zero_sequence_resistance_in_pu() const;
        double get_zero_sequence_reactance_in_pu() const;
        double get_grounding_resistance_in_pu() const;
        double get_grounding_reactance_in_pu() const;

        void set_generator_reactance_option(GENERATOR_REACTANCE_OPTION option);
        GENERATOR_REACTANCE_OPTION get_generator_reactance_option() const;
        void update_E();
        complex<double> get_complex_E_in_pu();
        complex<double> get_positive_sequence_complex_current_in_pu();
        complex<double> get_negative_sequence_complex_current_in_pu();
        complex<double> get_zero_sequence_complex_current_in_pu();
        complex<double> get_positive_sequence_complex_current_in_kA();
        complex<double> get_negative_sequence_complex_current_in_kA();
        complex<double> get_zero_sequence_complex_current_in_kA();

        void set_wt_generator_type(WT_GENERATOR_TYPE type);
        WT_GENERATOR_TYPE get_wt_generator_type() const;

        void set_sequence_parameter_import_flag(bool flag);
        bool get_sequence_parameter_import_flag() const;


        void set_motor_rated_voltage_in_kV(double v);
        void set_mbase_in_MVA(double m);
        void set_motor_Ra_in_pu(double r);
        void set_motor_Xa_in_pu(double x);
        void set_motor_Xm_in_pu(double x);
        void set_motor_R1_in_pu(double r);
        void set_motor_X1_in_pu(double x);
        void set_motor_R2_in_pu(double r);
        void set_motor_X2_in_pu(double x);
        void set_motor_internal_voltage_in_pu(complex<double> v);
        void set_motor_positive_sequence_impedance_in_pu(complex<double> z);
        void set_motor_negative_sequence_impedance_in_pu(complex<double> z);
        void set_motor_zero_sequence_impedance_in_pu(complex<double> z);
        void set_motor_power_in_MVA(complex<double> s);

        double get_motor_rated_voltage_in_kV() const;
        double get_motor_mbase_in_MVA() const;
        double get_motor_Ra_in_pu() const;
        double get_motor_Xa_in_pu() const;
        double get_motor_Xm_in_pu() const;
        double get_motor_R1_in_pu() const;
        double get_motor_X1_in_pu() const;
        double get_motor_R2_in_pu() const;
        double get_motor_X2_in_pu() const;
        complex<double> get_motor_internal_voltage_in_pu() const;
        complex<double> get_motor_equivalent_injection_current_in_pu() const;
        complex<double> get_motor_positive_sequence_impedance_in_pu() const;
        complex<double> get_motor_negative_sequence_impedance_in_pu() const;
        complex<double> get_motor_zero_sequence_impedance_in_pu() const;
        complex<double> get_motor_power_in_MVA() const;
        complex<double> get_static_power_in_MVA() const;
        void update_motor_data();

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

        double R1, X1_sync, X1_transient, X1_subtransient;
        double R2, X2, R0, X0;
        double Rground, Xground;
        complex<double> E;
        GENERATOR_REACTANCE_OPTION gen_X_option;
        WT_GENERATOR_TYPE wtg_type;

        bool sequence_parameter_import_flag;


        double motor_rated_voltge;
        double mbase;
        double motor_Ra, motor_Xa, motor_Xm, motor_R1, motor_X1, motor_R2, motor_X2;
        complex<double> motor_power_in_MVA;
        complex<double> motor_internal_voltage;
        complex<double> motor_Z1, motor_Z2, motor_Z0;
};
#endif // WT_GENERATOR_H
