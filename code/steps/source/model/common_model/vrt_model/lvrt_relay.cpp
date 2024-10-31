#include "header/model/common_model/vrt_model/lvrt_relay.h"

LVRT_RELAY::LVRT_RELAY(STEPS& toolkit) : VRT_RELAY_MODEL(toolkit)
{
    clear();
}

LVRT_RELAY::~LVRT_RELAY()
{
    ;
}

LVRT_RELAY::LVRT_RELAY(const LVRT_RELAY& model) : VRT_RELAY_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

LVRT_RELAY& LVRT_RELAY::operator=(const LVRT_RELAY& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void LVRT_RELAY::clear()
{
    VRT_RELAY_MODEL::clear();
    set_lvrt_voltage_threshold(0.0);
}

void LVRT_RELAY::copy_from_const_model(const LVRT_RELAY& model)
{
    clear();
    VRT_RELAY_MODEL::copy_from_const_model(model);
}

void LVRT_RELAY::set_lvrt_voltage_threshold(double v)
{
    vth_lvrt = v;
}

double LVRT_RELAY::get_lvrt_voltage_threshold() const
{
    return vth_lvrt;
}

void LVRT_RELAY::initialize_vrt_relay()
{
    MULTI_POINT_LINE LVRT_RELAY_line = get_vrt_line();
    set_lvrt_voltage_threshold(LVRT_RELAY_line.get_max_y());
    set_time_when_vrt_enter_during_status(INFINITE_THRESHOLD);
}

void LVRT_RELAY::check_vrt_relay()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    TIMER& timer = get_vrt_trip_timer();
    double v_th = get_lvrt_voltage_threshold();
    if(get_relay_action_flag()==false)
    {
        if(not timer.is_started())
        {
            double v = get_bus_voltage_in_pu();
            if(v<v_th)
            {
                double tnow = toolkit.get_dynamic_simulation_time_in_s();
                if(get_time_when_vrt_enter_during_status() == INFINITE_THRESHOLD)
                {
                    set_time_when_vrt_enter_during_status(tnow);
                }
                MULTI_POINT_LINE LVRT_RELAY_line = get_vrt_line();
                double t0 = get_time_when_vrt_enter_during_status();
                double vtrip_th = LVRT_RELAY_line.get_y_at_x(tnow-t0);
                if(v<vtrip_th)
                {
                    if(fabs(get_vrt_trip_time_delay_in_s())<DOUBLE_EPSILON)
                    {
                        osstream<<get_compound_device_name()<<" will be tripped at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage exceeding LVRT_RELAY trip threshold: "<<v<<" < "<<vtrip_th<<" and LVRT_RELAY relay timer is Zero."<<endl;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        trip_device();
                        enable_relay_action_flag();
                    }
                    else
                    {
                        timer.start();
                        osstream<<"LVRT_RELAY relay timer of "<<get_compound_device_name()<<" is started at "<<toolkit.get_dynamic_simulation_time_in_s()
                                <<"s due to voltage exceeding LVRT_RELAY trip threshold: "<<v<<" < "<<vtrip_th<<endl;
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
                osstream<<"LVRT_RELAY relay timer of "<<get_compound_device_name()<<" is timed out at "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                trip_device();
                enable_relay_action_flag();

            }
        }
    }
}
