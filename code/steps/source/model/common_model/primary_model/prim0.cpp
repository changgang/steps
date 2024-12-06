#include "header/model/common_model/primary_model/prim0.h"

PRIM0::PRIM0(STEPS& toolkit):PRIMARY_MODEL(toolkit),
                            speed_sensor(toolkit),
                            Kp(toolkit),
                            governor(toolkit),
                            servo_motor(toolkit),
                            water_turbine(toolkit)
{
    clear();
}

PRIM0::PRIM0(const PRIM0& model) : PRIMARY_MODEL(model.get_toolkit()),
                                    speed_sensor(model.get_toolkit()),
                                    Kp(model.get_toolkit()),
                                    governor(model.get_toolkit()),
                                    servo_motor(model.get_toolkit()),
                                    water_turbine(model.get_toolkit())
{
    copy_from_const_model(model);
}

PRIM0::~PRIM0()
{
    ;
}

PRIM0& PRIM0::operator=(const PRIM0& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}


void PRIM0::clear()
{
    Kp.set_limiter_type(NO_LIMITER);
    governor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_limiter_type(NO_LIMITER);
    water_turbine.set_limiter_type(NO_LIMITER);
}

void PRIM0::copy_from_const_model(const PRIM0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

string PRIM0::get_model_name() const
{
    return "PRIM0";
}

void PRIM0::set_Tr_in_s(double T)
{
    speed_sensor.set_T_in_s(T);
}

void PRIM0::set_Kp(double k)
{
    Kp.set_K(k);
}

void PRIM0::set_Ki(double k)
{
    double T = 0.0;
    if(fabs(k)<DOUBLE_EPSILON)
    {
        T = INFINITE_THRESHOLD;
    }
    else
    {
        T = 1/k;
    }
    governor.set_T_in_s(T);
}

void PRIM0::set_INTmax(double k)
{
    governor.set_upper_limit(k);
}

void PRIM0::set_INTmin(double k)
{
    governor.set_lower_limit(k);
}

void PRIM0::set_PImax(double k)
{
    PImax = k;
}

void PRIM0::set_PImin(double k)
{
    PImin = k;
}

void PRIM0::set_umax(double k)
{
    umax = k;
}

void PRIM0::set_umin(double k)
{
    umin = k;
}

void PRIM0::set_Ts_in_s(double T)
{
    servo_motor.set_T_in_s(T);
}

void PRIM0::set_Twg_in_s(double T)
{
    water_turbine.set_T1_in_s(-T);
    water_turbine.set_T2_in_s(0.5*T);
}

double PRIM0::get_Tr_in_s() const
{
    return speed_sensor.get_T_in_s();
}

double PRIM0::get_Kp() const
{
    return Kp.get_K();
}

double PRIM0::get_Ki() const
{
    double T = governor.get_T_in_s();
    if(T==INFINITE_THRESHOLD)
        return 0.0;
    else
        return 1/T;
}

double PRIM0::get_INTmax() const
{
    return governor.get_upper_limit();
}

double PRIM0::get_INTmin() const
{
    return governor.get_lower_limit();
}

double PRIM0::get_PImax() const
{
    return PImax;
}

double PRIM0::get_PImin() const
{
    return PImin;
}

double PRIM0::get_umax() const
{
    return umax;
}

double PRIM0::get_umin() const
{
    return umin;
}

double PRIM0::get_Ts_in_s() const
{
    return servo_motor.get_T_in_s();
}

double PRIM0::get_Twg_in_s() const
{
    return -water_turbine.get_T1_in_s();
}

void PRIM0::initialize_mechanical_power_as_zero()
{
    speed_sensor.set_output(0.0);
    speed_sensor.initialize();

    water_turbine.set_output(0.0);
    water_turbine.initialize();

    servo_motor.set_output(0.0);
    servo_motor.initialize();


    governor.set_output(0.0);
    governor.initialize();

    Kp.set_output(0.0);
    Kp.initialize();
}

void PRIM0::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;

    speed_sensor.set_output(0.0);
    speed_sensor.initialize();

    double Pmech = get_initial_mechanical_power_in_pu();
    water_turbine.set_output(Pmech);
    water_turbine.initialize();

    servo_motor.set_output(Pmech);
    servo_motor.initialize();
    double bp = get_Bp();

    if(fabs(bp)>DOUBLE_EPSILON)
    {
        double Yref = get_initial_valve_reference_in_pu();
        double deltaY = Pmech - Yref;
        governor.set_output(deltaY);
        governor.initialize();

        double PImax = get_PImax();
        if(deltaY>PImax)
        {
            osstream<<"Initialization warning. DeltaY of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit. "
              <<"DeltaY is "<<deltaY<<", and PImax is "<<PImax<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }

        double PImin = get_PImin();
        if(deltaY<PImin)
        {
            osstream<<"Initialization warning. DeltaY of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit. "
              <<"DeltaY is "<<deltaY<<", and PImin is "<<PImin<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
    else
    {
        governor.set_output(Pmech);
        governor.initialize();
    }

    Kp.set_output(0.0);
    Kp.initialize();

    double umax = get_umax();
    if(Pmech>umax)
    {
        osstream<<"Initialization warning. Y(valve) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit. "
          <<"Y is "<<Pmech<<", and umax is "<<umax<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    double umin = get_umin();
    if(Pmech<umin)
    {
        osstream<<"Initialization warning. Y(valve) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit. "
          <<"Y is "<<Pmech<<", and umin is "<<umin<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

}

void PRIM0::run(DYNAMIC_MODE mode)
{
    double speed = get_speed_in_pu();
    double speedref = get_speed_reference_in_pu();
    double input;

     if(not is_in_charge_mode())
        input = speedref - speed;
    else
        input = speed - speedref;

    speed_sensor.set_input(input);
    speed_sensor.run(mode);

    double bp = get_Bp();
    double Pref = get_active_power_reference_in_pu();
    double Pmech = get_mechanical_power_in_pu_on_mbase();

    input = speed_sensor.get_output()*get_Ka()+(Pref-Pmech)*get_Eb();

    if(fabs(bp)>DOUBLE_EPSILON)
    {
        double deltaY = get_delta_Y_in_pu();
        double deltaYref = get_delta_Yref_in_pu();
        input = input + get_Bp()*(deltaYref-deltaY);
    }

    Kp.set_input(input);
    Kp.run(mode);

    governor.set_input(input);
    governor.run(mode);

    input = get_delta_Y_in_pu();

    if(fabs(bp)>DOUBLE_EPSILON)
    {
        input = input + get_valve_reference_in_pu();
    }

    servo_motor.set_input(input);
    servo_motor.run(mode);

    double Y = get_valve_in_pu();
    water_turbine.set_input(Y);
    water_turbine.run(mode);

}

double PRIM0::get_valve_in_pu() const
{
    double valve = servo_motor.get_output();
    double umax = get_umax();
    double umin = get_umin();
    if(valve>umax)
        valve = umax;
    if(valve<umin)
        valve = umin;
    return valve;
}

double PRIM0::get_delta_Y_in_pu() const
{
    double dY = Kp.get_output()+governor.get_output();
    double PImax = get_PImax();
    double PImin = get_PImin();
    if(dY>PImax)
        dY = PImax;
    if(dY<PImin)
        dY = PImin;
    return dY;
}

double PRIM0::get_delta_Yref_in_pu() const
{
    double Y = get_valve_in_pu();
    double Yref = get_valve_reference_in_pu();
    return Y-Yref;
}

double PRIM0::get_mechanical_power_in_pu_on_mbase() const
{
    return water_turbine.get_output();
}
