#ifndef MOTOR_H
#define MOTOR_H

#include "header/basic/steps_enum.h"
#include "header/model/load_model/load_model.h"
#include <complex>

class MOTOR : public LOAD_MODEL
{
    public:
        MOTOR();
        MOTOR(const MOTOR& model);
        virtual ~MOTOR();

        virtual string get_model_name() const;

        virtual bool setup_model_with_steps_string(string data);
        virtual bool setup_model_with_psse_string(string data);
        virtual bool setup_model_with_bpa_string(string data);

        virtual void initialize();
        virtual void run(DYNAMIC_MODE mode);
        virtual void check();
        virtual void report();
        virtual void save();

        virtual double get_variable_with_index(size_t var_index);
        virtual double get_variable_with_name(string var_name);
        virtual size_t get_variable_index_from_variable_name(string var_name);

        virtual complex<double> get_load_actual_power_in_MVA() const;

    public:
        void set_motor_double_cage_type(DOUBLE_CAGE_MOTOR_TYPE mtype);
        void set_Ra_in_pu(double ra);
        void set_Xa_in_pu(double xa);
        void set_Xm_in_pu(double xm);
        void set_R1_in_pu(double r1);
        void set_X1_in_pu(double x1);
        void set_R2_in_pu(double r2);
        void set_X2_in_pu(double x2);
        void set_X_in_pu(double x);
        void set_X_leakage_in_pu(double x);
        void set_Xp_in_pu(double x);
        void set_Xpp_in_pu(double x);
        void set_T0p_in_s(double T);
        void set_T0pp_in_s(double T);
        void set_motor_mbase_in_MVA(double mbase);
        void set_motor_mbase_multiplier_in_pu(double mult);
        void set_H_in_s(double h);
        void set_damping_A(double a);
        void set_damping_B(double b);
        void set_damping_C(double c);
        void set_damping_D(double d);
        void set_damping_E(double e);
        void set_nominal_torque_for_motor_starting_in_pu(double Tnom);
        void set_voltage_threshold_of_under_voltage_relay_in_pu(double V);
        void set_timer_of_under_voltage_relay_in_s(double T);
        void set_timer_of_breaker_in_s(double T);
        void set_slip_in_pu(double slip);

        DOUBLE_CAGE_MOTOR_TYPE get_motor_double_cage_type() const;
        double get_Ra_in_pu() const;
        double get_Xa_in_pu() const;
        double get_Xm_in_pu() const;
        double get_R1_in_pu() const;
        double get_X1_in_pu() const;
        double get_R2_in_pu() const;
        double get_X2_in_pu() const;
        double get_X_in_pu() const;
        double get_X_leakage_in_pu() const;
        double get_Xp_in_pu() const;
        double get_Xpp_in_pu() const;
        double get_T0p_in_s() const;
        double get_T0pp_in_s() const;
        double get_motor_mbase_in_MVA() const;
        double get_motor_mbase_multiplier_in_pu() const;
        double get_H_in_s() const;
        double get_damping_A() const;
        double get_damping_B() const;
        double get_damping_C() const;
        double get_damping_D() const;
        double get_damping_E() const;
        double get_nominal_torque_for_motor_starting_in_pu() const;
        double get_voltage_threshold_of_under_voltage_relay_in_pu() const;
        double get_timer_of_under_voltage_relay_in_s() const;
        double get_timer_of_breaker_in_s() const;
        double get_slip_in_pu() const;

        void convert_to_flux_linkage_model();
        void convert_to_equivalent_circuit_model();

    private:
        bool is_in_flux_linkage_model_type() const;
        bool is_in_equivalent_circuit_model_type() const;
        complex<double> get_motor_power_with_slip(double slip);


    private:
        DOUBLE_CAGE_MOTOR_TYPE motor_double_cage_type;
        double Ra, Xa, Xm, R1, X1, R2, X2;
        double X, Xleakage, Xp, Xpp, T0p, T0pp;
        double E1, SE1, E2, SE2;
        double mbase, multiplier;
        double H, D;
        double A, B, C, E;
        double torque_for_starting;
        double V_interrupt, T_interrupt, T_breaker;
        double slip;


        complex<double> const_shunt_in_MVA;
};
#endif // MOTOR_H
