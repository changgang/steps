#include "header/model/common_model/vrt_model/hvrt_relay.h"

HVRT_RELAY::HVRT_RELAY(STEPS& toolkit) : VRT_RELAY_MODEL(toolkit)
{
    clear();
}

HVRT_RELAY::~HVRT_RELAY()
{
    ;
}

HVRT_RELAY::HVRT_RELAY(const HVRT_RELAY& model) : VRT_RELAY_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

HVRT_RELAY& HVRT_RELAY::operator=(const HVRT_RELAY& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void HVRT_RELAY::clear()
{
    VRT_RELAY_MODEL::clear();
    set_hvrt_voltage_threshold(0.0);
}

void HVRT_RELAY::copy_from_const_model(const HVRT_RELAY& model)
{
    clear();
    VRT_RELAY_MODEL::copy_from_const_model(model);
}

void HVRT_RELAY::set_hvrt_voltage_threshold(double v)
{
    vth_hvrt = v;
}

double HVRT_RELAY::get_hvrt_voltage_threshold() const
{
    return vth_hvrt;
}

void HVRT_RELAY::initialize_vrt_relay()
{
    MULTI_POINT_LINE HVRT_RELAY_line = get_vrt_line();
    set_hvrt_voltage_threshold(HVRT_RELAY_line.get_min_y());
    set_time_when_vrt_enter_during_status(INFINITE_THRESHOLD);
}

void HVRT_RELAY::check_vrt_relay()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    TIMER& timer = get_vrt_trip_timer();
    DEVICE_ID did = get_device_id();
    double v_th = get_hvrt_voltage_threshold();

    if(get_relay_action_flag()==false)
    {
        if(not timer.is_started())
        {
            double v = get_bus_voltage_in_pu();
            if(v>v_th)
            {
                double tnow = toolkit.get_dynamic_simulation_time_in_s();
                if(get_time_when_vrt_enter_during_status() == INFINITE_THRESHOLD)
                {
                    set_time_when_vrt_enter_during_status(tnow);
                }
                MULTI_POINT_LINE HVRT_RELAY_line = get_vrt_line();
                double t0 = get_time_when_vrt_enter_during_status();
                double vtrip_th = HVRT_RELAY_line.get_y_at_x(tnow-t0);
                if(v>vtrip_th)
                {
                    if(fabs(get_vrt_trip_time_delay_in_s())<DOUBLE_EPSILON)
                    {
                        osstream<<get_compound_device_name()<<" will be tripped at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage exceeding HVRT_RELAY trip threshold: "<<v<<" > "<<vtrip_th<<" and HVRT_RELAY relay timer is Zero."<<endl;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        trip_device();
                        enable_relay_action_flag();
                    }
                    else
                    {
                        timer.start();

                        osstream<<"HVRT_RELAY relay timer of "<<get_compound_device_name()<<" is started at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage exceeding HVRT_RELAY trip threshold: "<<v<<" > "<<vtrip_th<<endl;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                    }
                }
            }
            else
            {
                if(get_time_when_vrt_enter_during_status()!=INFINITE_THRESHOLD)
                    set_time_when_vrt_enter_during_status(INFINITE_THRESHOLD);
            }
        }
        else
        {
            if(timer.is_timed_out())
            {
                osstream<<"HVRT_RELAY relay timer of "<<get_compound_device_name()<<" is timed out at "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                trip_device();
                enable_relay_action_flag();
            }
        }
    }
}

double HVRT_RELAY::get_hvrt_voltage_trip_boundary_in_pu()
{
    STEPS& toolkit = get_toolkit();
    TIMER& timer = get_vrt_trip_timer();
    double v_th = get_hvrt_voltage_threshold();
    double tnow = toolkit.get_dynamic_simulation_time_in_s();
    MULTI_POINT_LINE LVRT_RELAY_line = get_vrt_line();
    double t0 = get_time_when_vrt_enter_during_status();

    if(not timer.is_started())
    {
        if(t0!=INFINITE_THRESHOLD)
            return LVRT_RELAY_line.get_y_at_x(tnow-t0);
        else
            return v_th;
    }
    else
        return LVRT_RELAY_line.get_y_at_x(tnow-t0);

}
