#include "header/model/converter_common_models/vrt_model/lvrt_relay.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

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
}

void LVRT_RELAY::copy_from_const_model(const LVRT_RELAY& model)
{
    clear();
    VRT_RELAY_MODEL::copy_from_const_model(model);
}

void LVRT_RELAY::initialize_vrt()
{
    MULTI_POINT_LINE& LVRT_RELAY_line = get_vrt_line();
    vth_lvrt = LVRT_RELAY_line.get_max_y();
    set_vrt_status(VRT_NORMAL_STATUS);
}

void LVRT_RELAY::check_vrt_status()
{
    NONBUS_DEVICE* device = get_device_pointer();
    STEPS& toolkit = get_toolkit();
    DYNAMICS_SIMULATOR& sim = toolkit.get_dynamic_simulator();
    ostringstream osstream;
    DEVICE_ID did = device->get_device_id();

    MULTI_POINT_LINE& LVRT_RELAY_line = get_vrt_line();

    double tnow = toolkit.get_dynamic_simulation_time_in_s();
    double v = get_bus_voltage();
    VRT_STATUS status = get_vrt_status();
    if(status == VRT_NORMAL_STATUS)
    {
        if(v<vth_lvrt)
            set_vrt_status(VRT_DURING_STATUS);
    }

    status = get_vrt_status();
    if(status==VRT_DURING_STATUS)
    {
        TIMER& timer = get_vrt_trip_timer();
        double t0 = get_time_when_vrt_enter_during_status();
        double vtrip_th = LVRT_RELAY_line.interpolate_y_at_x(tnow-t0);
        if(not timer.is_started())
        {
            if(v<vtrip_th)
            {
                timer.start();
                osstream<<"LVRT_RELAY relay timer of "<<device->get_compound_device_name()<<" is started at "<<toolkit.get_dynamic_simulation_time_in_s()
                        <<"s due to voltage exceeding LVRT_RELAY trip threshold: "<<v<<" < "<<vtrip_th<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                if(v>vth_lvrt)
                {
                    osstream<<"LVRT_RELAY mode of "<<device->get_compound_device_name()<<" is exited at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage recovered: "<<v<<" > "<<vth_lvrt<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    set_vrt_status(VRT_NORMAL_STATUS);
                }
            }
        }
        else
        {
            if(timer.is_timed_out())
            {
                osstream<<"LVRT_RELAY relay timer of "<<device->get_compound_device_name()<<" is timed out at "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
                switch(did.get_device_type())
                {
                    case STEPS_WT_GENERATOR:
                    {
                        sim.trip_wt_generator(did, ((WT_GENERATOR*)device)->get_number_of_lumped_wt_generators()*get_vrt_trip_scale());
                        sim.enable_relay_action_flag();
                        break;
                    }
                    case STEPS_PV_UNIT:
                    case STEPS_ENERGY_STORAGE:
                    default:
                    {
                        break;
                    }
                }
                set_vrt_status(VRT_NORMAL_STATUS);
            }
        }
    }
}
