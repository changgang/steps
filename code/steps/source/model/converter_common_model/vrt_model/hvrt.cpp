#include "header/model/converter_common_models/vrt_model/hvrt.h"
#include "header/power_system_database.h"
#include "header/STEPS.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>

using namespace std;

HVRT::HVRT(STEPS& toolkit) : VRT_MODEL(toolkit)
{
    clear();
}

HVRT::~HVRT()
{
    ;
}

HVRT::HVRT(const HVRT& model) : VRT_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

HVRT& HVRT::operator=(const HVRT& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void HVRT::clear()
{
}

void HVRT::copy_from_const_model(const HVRT& model)
{
    clear();
    VRT_MODEL::copy_from_const_model(model);
}

void HVRT::initialize_vrt()
{
    MULTI_POINT_LINE& hvrt_line = get_vrt_line();
    vth_hvrt = hvrt_line.get_min_y();
    set_vrt_status(VRT_NORMAL_MODE);
}

void HVRT::check_vrt_status()
{
    NONBUS_DEVICE* device = get_device_pointer();
    STEPS& toolkit = get_toolkit();
    DYNAMICS_SIMULATOR& sim = toolkit.get_dynamic_simulator();
    ostringstream osstream;
    DEVICE_ID did = device->get_device_id();

    MULTI_POINT_LINE& hvrt_line = get_vrt_line();

    double tnow = toolkit.get_dynamic_simulation_time_in_s();
    double v = get_bus_voltage();
    VRT_STATUS status = get_vrt_status();
    if(status == VRT_NORMAL_MODE)
    {
        if(v>vth_hvrt)
            set_vrt_status(HVRT_DRURING_MODE);
    }

    status = get_vrt_status();
    if(status==HVRT_DRURING_MODE)
    {
        TIMER& timer = get_vrt_trip_timer();
        double t0 = get_vrt_activation_time();
        double vtrip_th = hvrt_line.interpolate_y_at_x(tnow-t0);
        if(not timer.is_started())
        {
            if(v>vtrip_th)
            {
                timer.start();
                osstream<<"HVRT relay timer of "<<device->get_compound_device_name()<<" is started at "<<toolkit.get_dynamic_simulation_time_in_s()
                        <<"s due to voltage exceeding HVRT trip threshold: "<<v<<" > "<<vtrip_th<<endl;
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                if(v<vth_hvrt)
                {
                    osstream<<"HVRT mode of "<<device->get_compound_device_name()<<" is exited at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage recovered: "<<v<<" < "<<vth_hvrt<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    set_vrt_status(VRT_NORMAL_MODE);
                }
            }
        }
        else
        {
            if(timer.is_timed_out())
            {
                osstream<<"HVRT relay timer of "<<device->get_compound_device_name()<<" is timed out at "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
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
                set_vrt_status(VRT_NORMAL_MODE);
            }
        }
    }
}
