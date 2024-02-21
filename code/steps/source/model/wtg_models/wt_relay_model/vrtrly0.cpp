#include "header/model/wtg_models/wt_relay_model/vrtrly0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;

VRTRLY0::VRTRLY0(STEPS& toolkit) : WT_RELAY_MODEL(toolkit)
{
    clear();
}

VRTRLY0::~VRTRLY0()
{
}

void VRTRLY0::clear()
{
    //set_model_float_parameter_count(204);
    lvrt_trip_timer.clear();
    hvrt_trip_timer.clear();
    lvrt_line.clear();
    hvrt_line.clear();
    lvrt_trip_scale = 0.0;
    hvrt_trip_scale = 0.0;
}

void VRTRLY0::copy_from_const_model(const VRTRLY0& model)
{
    set_toolkit(model.get_toolkit());

    clear();
    set_converter_trip_time_delay_in_s(model.get_converter_trip_time_delay_in_s());
    unsigned int n = model.get_lvrt_point_number();
    set_lvrt_point_number(n);
    for(unsigned int i=0; i<n; ++i)
        set_lvrt_time_volt_threshold_pair(i, model.get_lvrt_time_threshold(i), model.get_lvrt_volt_threshold(i));
    set_lvrt_trip_scale(model.get_lvrt_trip_scale());

    n = model.get_hvrt_point_number();
    set_hvrt_point_number(n);
    for(unsigned int i=0; i<n; ++i)
        set_hvrt_time_volt_threshold_pair(i, model.get_hvrt_time_threshold(i), model.get_hvrt_volt_threshold(i));
    set_hvrt_trip_scale(model.get_hvrt_trip_scale());
}

VRTRLY0::VRTRLY0(const VRTRLY0& model) : WT_RELAY_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

VRTRLY0& VRTRLY0::operator=(const VRTRLY0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string VRTRLY0::get_model_name() const
{
    return "VRTRLY0";
}

void VRTRLY0::set_converter_trip_time_delay_in_s(double t)
{
    lvrt_trip_timer.set_timer_interval_in_s(t);
    hvrt_trip_timer.set_timer_interval_in_s(t);
}

void VRTRLY0::set_lvrt_point_number(unsigned int n)
{
    lvrt_line.set_point_number(n);
}

void VRTRLY0::set_lvrt_time_volt_threshold_pair(unsigned int index, double t, double v)
{
    lvrt_line.set_point(index, t, v);
}

void VRTRLY0::set_lvrt_trip_scale(double scale)
{
    lvrt_trip_scale = scale;
}

void VRTRLY0::set_hvrt_point_number(unsigned int n)
{
    hvrt_line.set_point_number(n);
}

void VRTRLY0::set_hvrt_time_volt_threshold_pair(unsigned int index, double t, double v)
{
    hvrt_line.set_point(index, t, v);
}

void VRTRLY0::set_hvrt_trip_scale(double scale)
{
    hvrt_trip_scale = scale;
}

double VRTRLY0::get_converter_trip_time_delay_in_s() const
{
    return lvrt_trip_timer.get_timer_interval_in_s();
}

unsigned int VRTRLY0::get_lvrt_point_number() const
{
    return lvrt_line.get_point_number();
}

double VRTRLY0::get_lvrt_time_threshold(unsigned int index) const
{
    return lvrt_line.get_point_x(index);
}

double VRTRLY0::get_lvrt_volt_threshold(unsigned int index) const
{
    return lvrt_line.get_point_y(index);
}

double VRTRLY0::get_lvrt_trip_scale() const
{
    return lvrt_trip_scale;
}

unsigned int VRTRLY0::get_hvrt_point_number() const
{
    return hvrt_line.get_point_number();
}

double VRTRLY0::get_hvrt_time_threshold(unsigned int index) const
{
    return hvrt_line.get_point_x(index);
}

double VRTRLY0::get_hvrt_volt_threshold(unsigned int index) const
{
    return hvrt_line.get_point_y(index);
}

double VRTRLY0::get_hvrt_trip_scale() const
{
    return hvrt_trip_scale;
}

bool VRTRLY0::setup_model_with_steps_string_vector(vector<string>& data)
{
    /*VRTRLY0  bus  ickt t_trip
              nlvrt  t1  v1  t2  v2  t3  v3  tn  vn  trip_scale
              nhvrt  t1  v1  t2  v2  t3  v3  tm  vm  trip_scale/
    */
    bool is_successful = false;
    if(data.size()>=12)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int n;
            double t, v, s;

            unsigned int i=3;
            t = get_double_data(data[i], "0"); ++i;
            set_converter_trip_time_delay_in_s(t);

            n = get_integer_data(data[i], "0"); ++i;
            for(unsigned int k=0; k<n; ++k)
            {
                t = get_double_data(data[i], "0.0"); ++i;
                v = get_double_data(data[i], "0.0"); ++i;
                set_lvrt_time_volt_threshold_pair(k, t, v);
            }
            s = get_double_data(data[i], "0"); ++i;
            set_lvrt_trip_scale(s);

            n = get_integer_data(data[i], "0"); ++i;
            for(unsigned int k=0; k<n; ++k)
            {
                t = get_double_data(data[i], "0.0"); ++i;
                v = get_double_data(data[i], "0.0"); ++i;
                set_hvrt_time_volt_threshold_pair(k, t, v);
            }
            s = get_double_data(data[i], "0"); ++i;
            set_hvrt_trip_scale(s);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool VRTRLY0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VRTRLY0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VRTRLY0::setup_block_toolkit_and_parameters()
{
    vth_lvrt = lvrt_line.get_max_y();
    vth_hvrt = hvrt_line.get_min_y();
    t0_vrt_activated = INFINITE_THRESHOLD;
    vrt_status = VRT_NORMAL_STATUS;
}

void VRTRLY0::initialize()
{
    setup_block_toolkit_and_parameters();
    lvrt_trip_timer.reset();
    hvrt_trip_timer.reset();
}

void VRTRLY0::run(DYNAMIC_MODE mode)
{
    if(mode==RELAY_MODE)
    {
        check_lvrt_relay();
        check_hvrt_relay();
    }
}


void VRTRLY0::check_lvrt_relay()
{
    WT_GENERATOR* wtgen = (WT_GENERATOR*) get_device_pointer();
    if(wtgen->get_status()==true)
    {
        STEPS& toolkit = get_toolkit();
        DYNAMICS_SIMULATOR& sim = toolkit.get_dynamic_simulator();
        ostringstream osstream;
        DEVICE_ID did = get_device_id();

        double tnow = toolkit.get_dynamic_simulation_time_in_s();
        BUS* bus = wtgen->get_bus_pointer();
        double v = bus->get_positive_sequence_voltage_in_pu();
        if(vrt_status == VRT_NORMAL_STATUS)
        {
            if(v<vth_lvrt)
            {
                t0_vrt_activated = tnow;
                vrt_status = VRT_DURING_STATUS;
            }
        }
        if(vrt_status==VRT_DURING_STATUS)
        {
            double vtrip_th = lvrt_line.interpolate_y_at_x(tnow-t0_vrt_activated);
            if(not lvrt_trip_timer.is_started())
            {
                if(v<vtrip_th)
                {
                    lvrt_trip_timer.start();
                    osstream<<"LVRT relay timer of "<<get_compound_device_name()<<" is started at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage exceeding LVRT trip threshold: "<<v<<" < "<<vtrip_th<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                else
                {
                    if(v>vth_lvrt)
                    {
                        osstream<<"LVRT mode of "<<get_compound_device_name()<<" is exited at "<<toolkit.get_dynamic_simulation_time_in_s()
                                <<"s due to voltage recovered: "<<v<<" > "<<vth_lvrt<<endl;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        vrt_status = VRT_NORMAL_STATUS;
                    }
                }
            }
            else
            {
                if(lvrt_trip_timer.is_timed_out())
                {
                    osstream<<"LVRT relay timer of "<<get_compound_device_name()<<" is timed out at "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    sim.trip_wt_generator(did, wtgen->get_number_of_lumped_wt_generators()*get_lvrt_trip_scale());
                    sim.enable_relay_action_flag();
                    vrt_status = VRT_NORMAL_STATUS;
                }

            }
        }
    }
}

void VRTRLY0::check_hvrt_relay()
{
    WT_GENERATOR* wtgen = (WT_GENERATOR*) get_device_pointer();
    if(wtgen->get_status()==true)
    {
        STEPS& toolkit = get_toolkit();
        DYNAMICS_SIMULATOR& sim = toolkit.get_dynamic_simulator();
        ostringstream osstream;
        DEVICE_ID did = get_device_id();

        double tnow = toolkit.get_dynamic_simulation_time_in_s();
        BUS* bus = wtgen->get_bus_pointer();
        double v = bus->get_positive_sequence_voltage_in_pu();
        if(vrt_status == VRT_NORMAL_STATUS)
        {
            if(v>vth_hvrt)
            {
                t0_vrt_activated = tnow;
                vrt_status = VRT_DURING_STATUS;
            }
        }
        if(vrt_status==VRT_DURING_STATUS)
        {
            double vtrip_th = hvrt_line.interpolate_y_at_x(tnow-t0_vrt_activated);
            if(not hvrt_trip_timer.is_started())
            {
                if(v<vtrip_th)
                {
                    hvrt_trip_timer.start();
                    osstream<<"HVRT relay timer of "<<get_compound_device_name()<<" is started at "<<toolkit.get_dynamic_simulation_time_in_s()
                            <<"s due to voltage exceeding HVRT trip threshold: "<<v<<" < "<<vtrip_th<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                else
                {
                    if(v>vth_hvrt)
                    {
                        osstream<<"HVRT mode of "<<get_compound_device_name()<<" is exited at "<<toolkit.get_dynamic_simulation_time_in_s()
                                <<"s due to voltage recovered: "<<v<<" < "<<vth_hvrt<<endl;
                        toolkit.show_information_with_leading_time_stamp(osstream);
                        vrt_status = VRT_NORMAL_STATUS;
                    }
                }
            }
            else
            {
                if(hvrt_trip_timer.is_timed_out())
                {
                    osstream<<"HVRT relay timer of "<<get_compound_device_name()<<" is timed out at "<<toolkit.get_dynamic_simulation_time_in_s()<<" s"<<endl;
                    toolkit.show_information_with_leading_time_stamp(osstream);
                    sim.trip_wt_generator(did, wtgen->get_number_of_lumped_wt_generators()*get_hvrt_trip_scale());
                    sim.enable_relay_action_flag();
                    vrt_status = VRT_NORMAL_STATUS;
                }
            }
        }
    }
}

void VRTRLY0::check()
{
    ;
}

void VRTRLY0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VRTRLY0::save()
{
    ;
}
string VRTRLY0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;

    DEVICE_ID did = get_device_id();
    unsigned int bus = did.get_device_terminal().get_buses()[0];
    string identifier = "'"+did.get_device_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(6)<<get_converter_trip_time_delay_in_s()<<", \n"
            <<setw(10)<<"";

    unsigned int n=get_lvrt_point_number();

    osstream<<setw(8)<<n<<", ";

    for(unsigned int i=0; i<n; ++i)
    {
        osstream<<setw(8)<<setprecision(6)<<get_lvrt_time_threshold(i)<<", ";
        osstream<<setw(8)<<setprecision(6)<<get_lvrt_time_threshold(i)<<", ";
    }
    osstream<<setw(8)<<setprecision(6)<<get_lvrt_trip_scale()<<", \n";
    osstream<<setw(10)<<"";

    n=get_hvrt_point_number();

    osstream<<setw(8)<<n<<", ";

    for(unsigned int i=0; i<n; ++i)
    {
        osstream<<setw(8)<<setprecision(6)<<get_hvrt_time_threshold(i)<<", ";
        osstream<<setw(8)<<setprecision(6)<<get_hvrt_time_threshold(i)<<", ";
    }
    osstream<<setw(8)<<setprecision(6)<<get_hvrt_trip_scale()<<" /";
    return osstream.str();
}

void VRTRLY0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TRIP TIME DELAY", i); i++;
    add_model_data_name_and_index_pair("N LVRT TABLE", i); i++;
    for(unsigned int j=0; j<get_lvrt_point_number(); ++j)
    {
        string name = "T LVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "V LVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
    }
    add_model_data_name_and_index_pair("LVRT SHED SCALE", i); i++;

    add_model_data_name_and_index_pair("N HVRT TABLE", i); i++;
    for(unsigned int j=0; j<get_hvrt_point_number(); ++j)
    {
        string name = "T HVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
        name = "V HVRT "+num2str(j);
        add_model_data_name_and_index_pair(name, i); i++;
    }
    add_model_data_name_and_index_pair("HVRT SHED SCALE", i); i++;
}

double VRTRLY0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="TRIP TIME DELAY") return get_converter_trip_time_delay_in_s();

    if(par_name=="N LVRT TABLE") return get_lvrt_point_number();

    for(unsigned int j=0; j<get_lvrt_point_number(); ++j)
    {
        string name = "T LVRT "+num2str(j);
        if(par_name==name) return get_lvrt_time_threshold(j);
        name = "V LVRT "+num2str(j);
        if(par_name==name) return get_lvrt_volt_threshold(j);
    }
    if(par_name=="LVRT SHED SCALE") return get_lvrt_trip_scale();

    if(par_name=="N HVRT TABLE") return get_hvrt_point_number();

    for(unsigned int j=0; j<get_hvrt_point_number(); ++j)
    {
        string name = "T HVRT "+num2str(j);
        if(par_name==name) return get_hvrt_time_threshold(j);
        name = "V HVRT "+num2str(j);
        if(par_name==name) return get_hvrt_volt_threshold(j);
    }
    if(par_name=="HVRT SHED SCALE") return get_hvrt_trip_scale();

    return 0.0;
}

void VRTRLY0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name=="TRIP TIME DELAY") return set_converter_trip_time_delay_in_s(value);

    if(par_name=="N LVRT TABLE") return;

    for(unsigned int j=0; j<get_lvrt_point_number(); ++j)
    {
        string name = "T LVRT "+num2str(j);
        if(par_name==name) return set_lvrt_time_volt_threshold_pair(j, value, get_lvrt_volt_threshold(j));
        name = "V LVRT "+num2str(j);
        if(par_name==name) return set_lvrt_time_volt_threshold_pair(j, get_lvrt_time_threshold(j), value);
    }
    if(par_name=="LVRT SHED SCALE") return set_lvrt_trip_scale(value);

    if(par_name=="N HVRT TABLE") return;

    for(unsigned int j=0; j<get_hvrt_point_number(); ++j)
    {
        string name = "T HVRT "+num2str(j);
        if(par_name==name) return set_hvrt_time_volt_threshold_pair(j, value, get_hvrt_volt_threshold(j));
        name = "V HVRT "+num2str(j);
        if(par_name==name) return set_hvrt_time_volt_threshold_pair(j, get_hvrt_time_threshold(j), value);
    }
    if(par_name=="HVRT SHED SCALE") return set_hvrt_trip_scale(value);
}


double VRTRLY0::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void VRTRLY0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    //unsigned int i=1;
    //add_model_internal_variable_name_and_index_pair("STATE@TURBINE SPEED BLOCK", i); i++;
}

double VRTRLY0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    //if(var_name=="STATE@TURBINE SPEED BLOCK") return turbine_inertia_block.get_state();

    return 0.0;
}

string VRTRLY0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VRTRLY0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string VRTRLY0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
