#include "header/model/common_model/primary_model/gov11.h"

GOV11::GOV11(STEPS& toolkit):PRIMARY_MODEL(toolkit),
                            speed_sensor(toolkit),
                            Kp(toolkit),
                            governor(toolkit),
                            servo_motor(toolkit),
                            water_turbine(toolkit)
{
    clear();
}

GOV11::GOV11(const GOV11& model) : PRIMARY_MODEL(model.get_toolkit()),
                                    speed_sensor(model.get_toolkit()),
                                    Kp(model.get_toolkit()),
                                    governor(model.get_toolkit()),
                                    servo_motor(model.get_toolkit()),
                                    water_turbine(model.get_toolkit())
{
    copy_from_const_model(model);
}

GOV11::~GOV11()
{
    ;
}

GOV11& GOV11::operator=(const GOV11& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return *this;
}


void GOV11::clear()
{
    Kp.set_limiter_type(NO_LIMITER);
    governor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_limiter_type(NO_LIMITER);
    water_turbine.set_limiter_type(NO_LIMITER);
}

void GOV11::copy_from_const_model(const GOV11& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

string GOV11::get_model_name() const
{
    return "GOV11";
}

void GOV11::set_Tr_in_s(double T)
{
    speed_sensor.set_T_in_s(T);
}

void GOV11::set_dbp(double k)
{
    dbp = k;
}

void GOV11::set_dbn(double k)
{
    dbn = k;
}

void GOV11::set_Kp(double k)
{
    Kp.set_K(k);
}

void GOV11::set_Ki(double k)
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

void GOV11::set_INTmax(double k)
{
    governor.set_upper_limit(k);
}

void GOV11::set_INTmin(double k)
{
    governor.set_lower_limit(k);
}

void GOV11::set_PImax(double k)
{
    PImax = k;
}

void GOV11::set_PImin(double k)
{
    PImin = k;
}

void GOV11::set_umax(double k)
{
    umax = k;
}

void GOV11::set_umin(double k)
{
    umin = k;
}

void GOV11::set_Ts_in_s(double T)
{
    servo_motor.set_T_in_s(T);
}

void GOV11::set_Twg_in_s(double T)
{
    water_turbine.set_T1_in_s(-T);
    water_turbine.set_T2_in_s(0.5*T);
}

double GOV11::get_Tr_in_s() const
{
    return speed_sensor.get_T_in_s();
}

double GOV11::get_dbp() const
{
    return dbp;
}

double GOV11::get_dbn() const
{
    return dbn;
}

double GOV11::get_Kp() const
{
    return Kp.get_K();
}

double GOV11::get_Ki() const
{
    double T = governor.get_T_in_s();
    if(T==INFINITE_THRESHOLD)
        return 0.0;
    else
        return 1/T;
}

double GOV11::get_INTmax() const
{
    return governor.get_upper_limit();
}

double GOV11::get_INTmin() const
{
    return governor.get_lower_limit();
}

double GOV11::get_PImax() const
{
    return PImax;
}

double GOV11::get_PImin() const
{
    return PImin;
}

double GOV11::get_umax() const
{
    return umax;
}

double GOV11::get_umin() const
{
    return umin;
}

double GOV11::get_Ts_in_s() const
{
    return servo_motor.get_T_in_s();
}

double GOV11::get_Twg_in_s() const
{
    return -water_turbine.get_T1_in_s();
}

void GOV11::initialize_mechanical_power_as_zero()
{
    speed_sensor.set_output(0.0);
    speed_sensor.initialize();

    water_turbine.set_output(0.0);
    water_turbine.initialize();

    governor.set_output(0.0);
    governor.initialize();

    Kp.set_output(0.0);
    Kp.initialize();
}

void GOV11::initialize()
{
    double speed = get_initial_speed_in_pu();
    speed_sensor.set_output(speed);
    speed_sensor.initialize();

    double Pmech = get_initial_mechanical_power_in_pu();
    water_turbine.set_output(Pmech);
    water_turbine.initialize();

    servo_motor.set_output(Pmech);
    servo_motor.initialize();

    double Yref = get_initial_valve_reference_in_pu();
    double deltaY = Pmech - Yref;
    governor.set_output(deltaY);
    governor.initialize();

    Kp.set_output(0.0);
    Kp.initialize();

    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
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

void GOV11::run(DYNAMIC_MODE mode)
{
    double speed = get_speed_in_pu();
    double speedref = get_speed_reference_in_pu();
    speed_sensor.set_input(speed);
    speed_sensor.run(mode);
    double delta_speed;
    if(not is_in_charge_mode())
        delta_speed = speedref - speed;
    else
        delta_speed = speed - speedref;

    double input = 0.0;
    double dbp = get_dbp();
    double dbn = get_dbn();
    if(delta_speed>=dbn and delta_speed<=dbp)
        ;
    else
    {
        if(delta_speed>dbp)
            input = delta_speed - dbp;
        else
            input = delta_speed - dbn;
    }
    double Y = get_valve_in_pu();
    double deltaY = get_delta_Y_in_pu();
    double Yref = get_valve_reference_in_pu();
    double deltaYref = get_delta_Yref_in_pu();

    input = input*get_Ka()+get_Bp()*(deltaYref-deltaY);
    Kp.set_input(input);
    Kp.run(mode);

    governor.set_input(input);
    governor.run(mode);

    deltaY = get_delta_Y_in_pu();
    input = deltaY + Yref;

    servo_motor.set_input(input);
    servo_motor.run(mode);

    Y = get_valve_in_pu();
    water_turbine.set_input(Y);
    water_turbine.run(mode);

}

double GOV11::get_valve_in_pu() const
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

double GOV11::get_delta_Y_in_pu() const
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

double GOV11::get_delta_Yref_in_pu() const
{
    double Y = get_valve_in_pu();
    double Yref = get_valve_reference_in_pu();
    return Y-Yref;
}

double GOV11::get_mechanical_power_in_pu_on_mbase() const
{
    return water_turbine.get_output();
}
