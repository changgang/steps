#include "header/model/vsc_hvdc_model/VSCHVDC1.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>


using namespace std;

VSCHVDC1::VSCHVDC1(STEPS& toolkit) : VSC_HVDC_MODEL(toolkit)
{
    clear();
}

VSCHVDC1::VSCHVDC1(const VSCHVDC1& model) : VSC_HVDC_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

VSCHVDC1& VSCHVDC1::operator=(const VSCHVDC1& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}


void VSCHVDC1::copy_from_const_model(const VSCHVDC1& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

    unsigned int n_vsc = model.get_vsc_stations_count();
    this->set_vsc_stations_count(n_vsc);
    for(unsigned int i=0; i!=n_vsc; ++i)
    {
        unsigned int converter_bus = model.get_converter_bus(i);
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_control_mode = model.get_converter_active_control_mode(i);
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_control_mode = model.get_converter_reactive_control_mode(i);
        this->set_converter_bus(i, converter_bus);
        this->set_converter_active_control_mode(i, active_control_mode);
        this->set_converter_reactive_control_mode(i, reactive_control_mode);
        if(active_control_mode==1)  //1-dc_voltage_control  2-active_power_control
        {
            if(reactive_control_mode==1)  //1-ac_voltage_control 2-reactive_power_control
            {
                double kp_ud = get_converter_dc_voltage_kpud(i);
                double Ti_ud = get_converter_dc_voltage_control_T_in_s(i);
                double max_ud = get_converter_dc_voltage_Umax(i);
                double min_ud = get_converter_dc_voltage_Umin(i);
                double kp_uc = get_converter_dc_voltage_kpuc(i);
                double Ti_uc = get_converter_ac_voltage_control_T_in_s(i);
                double max_uc = get_converter_ac_voltage_Umax(i);
                double min_uc = get_converter_dc_voltage_Umin(i);
                double Ceq = get_dc_voltage_ceq(i);
                this->set_converter_dc_voltage_kpud(i,kp_ud);
                this->set_converter_dc_voltage_control_T_in_s(i,Ti_ud);
                this->set_converter_dc_voltage_Umax(i,max_ud);
                this->set_converter_dc_voltage_Umin(i,min_ud);
                this->set_converter_dc_voltage_kpuc(i,kp_uc);
                this->set_converter_ac_voltage_control_T_in_s(i,Ti_uc);
                this->set_converter_ac_voltage_Umax(i,max_uc);
                this->set_converter_dc_voltage_Umin(i,max_uc);
                this->set_dc_voltage_ceq(i,Ceq);
            }
            if(reactive_control_mode==2)
            {
                double kp_ud = get_converter_dc_voltage_kpud(i);
                double Ti_ud = get_converter_dc_voltage_control_T_in_s(i);
                double max_ud = get_converter_dc_voltage_Umax(i);
                double min_ud = get_converter_dc_voltage_Umin(i);
                double kp_Q = get_converter_dc_voltage_kpuc(i);
                double Ti_Q = get_converter_ac_voltage_control_T_in_s(i);
                double max_Q = get_converter_ac_voltage_Umax(i);
                double min_Q = get_converter_dc_voltage_Umin(i);
                double Ceq = get_dc_voltage_ceq(i);
                set_converter_dc_voltage_kpud(i,kp_ud);
                set_converter_dc_voltage_control_T_in_s(i,Ti_ud);
                set_converter_dc_voltage_Umax(i,max_ud);
                set_converter_dc_voltage_Umin(i,min_ud);
                set_converter_dc_voltage_kpuc(i,kp_Q);
                set_converter_ac_voltage_control_T_in_s(i,Ti_Q);
                set_converter_ac_voltage_Umax(i,max_Q);
                set_converter_dc_voltage_Umin(i,min_Q);
                this->set_dc_voltage_ceq(i,Ceq);
            }
        }
        if(active_control_mode==2)
        {
            if(reactive_control_mode==1)
            {
                double kp_P = get_converter_active_power_kpp(i);
                double Ti_P = get_converter_active_power_control_T_in_s(i);
                double max_P = get_converter_active_power_Pmax_in_pu(i);
                double min_P = get_converter_active_power_Pmin_in_pu(i);
                double kp_Q = get_converter_reactive_power_kpq(i);
                double Ti_Q = get_converter_reactive_power_control_T_in_s(i);
                double max_Q = get_converter_reactive_power_Qmax_in_pu(i);
                double min_Q = get_converter_reactive_power_Qmin_in_pu(i);
                double Ceq = get_dc_voltage_ceq(i);
                this->set_converter_active_power_kpp(i,kp_P);
                this->set_converter_active_power_control_T_in_s(i,Ti_P);
                this->set_converter_active_power_Pmax_in_pu(i,max_P);
                this->set_converter_active_power_Pmin_in_pu(i,min_P);
                this->set_converter_reactive_power_kpq(i,kp_Q);
                this->set_converter_reactive_power_control_T_in_s(i,Ti_Q);
                this->set_converter_reactive_power_Qmax_in_pu(i,max_Q);
                this->set_converter_reactive_power_Qmin_in_pu(i,min_Q);
                this->set_dc_voltage_ceq(i,Ceq);
            }
            if(reactive_control_mode==2)
            {
                double kp_P = get_converter_dc_voltage_kpud(i);
                double Ti_P = get_converter_dc_voltage_control_T_in_s(i);
                double max_P = get_converter_dc_voltage_Umax(i);
                double min_P = get_converter_dc_voltage_Umin(i);
                double kp_Q = get_converter_dc_voltage_kpuc(i);
                double Ti_Q = get_converter_ac_voltage_control_T_in_s(i);
                double max_Q = get_converter_ac_voltage_Umax(i);
                double min_Q = get_converter_dc_voltage_Umin(i);
                double Ceq = get_dc_voltage_ceq(i);
                set_converter_dc_voltage_kpud(i,kp_P);
                set_converter_dc_voltage_control_T_in_s(i,Ti_P);
                set_converter_dc_voltage_Umax(i,max_P);
                set_converter_dc_voltage_Umin(i,min_P);
                set_converter_dc_voltage_kpuc(i,kp_Q);
                set_converter_ac_voltage_control_T_in_s(i,Ti_Q);
                set_converter_ac_voltage_Umax(i,max_Q);
                set_converter_dc_voltage_Umin(i,min_Q);
            }
        }
    }

}

VSCHVDC1::~VSCHVDC1()
{
    ;
}

void VSCHVDC1::clear()
{

    //time_when_rectifier_ac_voltage_below_delayed_blocking_voltage = INFINITE_THRESHOLD;
    //time_when_rectifier_ac_voltage_above_delayed_unblocking_voltage = INFINITE_THRESHOLD;
    //time_when_inverter_ac_voltage_below_instataneous_blocking_voltage = INFINITE_THRESHOLD;
    //time_when_inverter_ac_voltage_above_instataneous_unblocking_voltage = INFINITE_THRESHOLD;
    //time_when_inverter_ac_unblocking_signal_is_sent = INFINITE_THRESHOLD;

    //time_when_inverter_ac_voltage_below_delayed_bypassing_voltage = INFINITE_THRESHOLD;
    //time_when_inverter_ac_voltage_above_delayed_unbypassing_voltage = INFINITE_THRESHOLD;
}

string VSCHVDC1::get_model_name() const
{
    return "VSCHVDC1";
}

void VSCHVDC1::set_converter_bus(const unsigned int index, const unsigned int bus)
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    if(index>=get_vsc_stations_count())
    {
        osstream<<"VSC_HVDC converter bus index "<<index<<" exceeds converter count"<<get_vsc_stations_count()<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);

        return;
    }

    if(bus!=0)
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();

        if(psdb.is_bus_exist(bus))
        {
            vsc_stations[index].converter_bus = bus;
        }
        else
        {
            osstream<<"Bus "<<bus<<" does not exist for setting up "<<"converter_name"<<" side bus of vsc_hvdc-hvdc link."<<endl
                    <<"0 will be set to indicate invalid vsc-hvdc link.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            vsc_stations[index].converter_bus = 0;
        }
    }
    else
    {
        osstream<<"Warning. Zero bus number (0) is not allowed for setting up "<<"converter_name"<<" bus of vsc-hvdc link."<<endl
                <<"0 will be set to indicate invalid vsc-hvdc link.";
        STEPS& toolkit = get_toolkit();
        toolkit.show_information_with_leading_time_stamp(osstream);
        vsc_stations[index].converter_bus = bus;
    }
}

void VSCHVDC1::set_converter_active_control_mode(const unsigned int index, VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_control_mode)
{
    vsc_stations[index].active_power_control_mode = active_control_mode;
}

void VSCHVDC1::set_converter_reactive_control_mode(const unsigned int index, const VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_control_mode)
{
    vsc_stations[index].reactive_power_control_mode = reactive_control_mode;
}

void VSCHVDC1::set_converter_active_power_control_T_in_s(const unsigned int index, const double t)
{
    vsc_stations[index].active_power_control_block.set_Ki(t);
}

void VSCHVDC1::set_converter_active_power_Pmax_in_pu(const unsigned int index, const double Pmax)
{
    vsc_stations[index].active_power_control_block.set_upper_limit(Pmax);
}

void VSCHVDC1::set_converter_active_power_Pmin_in_pu(const unsigned int index, const double Pmin)
{
    vsc_stations[index].active_power_control_block.set_lower_limit(Pmin);
}

void VSCHVDC1::set_converter_active_power_kpp(const unsigned int index, const double k)
{
    vsc_stations[index].active_power_control_block.set_Kp(k);
}

void VSCHVDC1::set_converter_reactive_power_control_T_in_s(const unsigned int index, const double t)
{
    vsc_stations[index].reactive_power_control_block.set_Ki(t);
}

void VSCHVDC1::set_converter_reactive_power_Qmax_in_pu(const unsigned int index, const double Qmax)
{
    vsc_stations[index].reactive_power_control_block.set_upper_limit(Qmax);
}

void VSCHVDC1::set_converter_reactive_power_Qmin_in_pu(const unsigned int index, const double Qmin)
{
    vsc_stations[index].reactive_power_control_block.set_lower_limit(Qmin);
}

void VSCHVDC1::set_converter_reactive_power_kpq(const unsigned int index, const double k)
{
    vsc_stations[index].reactive_power_control_block.set_Kp(k);
}

void VSCHVDC1::set_converter_dc_voltage_control_T_in_s(const unsigned int index, const double t)
{
    vsc_stations[index].ud_voltage_control_block.set_Ki(t);
}

void VSCHVDC1::set_converter_dc_voltage_Umax(const unsigned int index, const double Umax)
{
    vsc_stations[index].ud_voltage_control_block.set_upper_limit(Umax);
}

void VSCHVDC1::set_converter_dc_voltage_Umin(const unsigned int index, const double Umin)
{
    vsc_stations[index].ud_voltage_control_block.set_lower_limit(Umin);
}

void VSCHVDC1::set_converter_dc_voltage_kpud(const unsigned int index, const double k)
{
    vsc_stations[index].ud_voltage_control_block.set_Kp(k);
}

void VSCHVDC1::set_converter_ac_voltage_control_T_in_s(const unsigned int index, const double t)
{
    vsc_stations[index].ac_voltage_control_block.set_Ki(t);
}

void VSCHVDC1::set_converter_ac_voltage_Umax(const unsigned int index, const double Umax)
{
    vsc_stations[index].ac_voltage_control_block.set_upper_limit(Umax);
}

void VSCHVDC1::set_converter_ac_voltage_Umin(const unsigned int index, const double Umin)
{
    vsc_stations[index].ac_voltage_control_block.set_lower_limit(Umin);
}

void VSCHVDC1::set_converter_dc_voltage_kpuc(const unsigned int index, const double k)
{
    vsc_stations[index].ac_voltage_control_block.set_Kp(k);
}

void VSCHVDC1::set_dc_voltage_ceq(const unsigned int index, const double ceq)
{
    vsc_stations[index].dc_voltage_block.set_T_in_s(ceq);
}

unsigned int VSCHVDC1::get_converter_bus(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].converter_bus;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 0;
    }
}

VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE VSCHVDC1::get_converter_active_control_mode(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].active_power_control_mode;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return VSC_INVALID_DC_CONTORL;
    }
}

VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE VSCHVDC1::get_converter_reactive_control_mode(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].reactive_power_control_mode;
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return VSC_INVALID_AC_CONTORL;
    }
}

double VSCHVDC1::get_converter_active_power_control_T_in_s(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].active_power_control_block.get_Ki();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_active_power_Pmax_in_pu(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].active_power_control_block.get_upper_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_active_power_Pmin_in_pu(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].active_power_control_block.get_lower_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_active_power_kpp(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].active_power_control_block.get_Kp();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_reactive_power_control_T_in_s(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].reactive_power_control_block.get_Ki();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_reactive_power_Qmax_in_pu(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].reactive_power_control_block.get_upper_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_reactive_power_Qmin_in_pu(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].reactive_power_control_block.get_lower_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_reactive_power_kpq(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].reactive_power_control_block.get_Kp();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_dc_voltage_control_T_in_s(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ud_voltage_control_block.get_Ki();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_dc_voltage_Umax(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ud_voltage_control_block.get_upper_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_dc_voltage_Umin(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ud_voltage_control_block.get_lower_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_dc_voltage_kpud(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ud_voltage_control_block.get_Kp();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_ac_voltage_control_T_in_s(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ac_voltage_control_block.get_Ki();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_ac_voltage_Umax(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ac_voltage_control_block.get_upper_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_ac_voltage_Umin(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ac_voltage_control_block.get_lower_limit();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_converter_dc_voltage_kpuc(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].ac_voltage_control_block.get_Kp();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}

double VSCHVDC1::get_dc_voltage_ceq(unsigned int index) const
{
    if(index<get_vsc_stations_count())
        return vsc_stations[index].dc_voltage_block.get_T_in_s();
    else
    {
        STEPS& toolkit = get_toolkit();
        ostringstream osstream;
        osstream<<"Error. index ("<<index<<") is out of  maximal converter count."<<endl;
        toolkit.show_information_with_leading_time_stamp(osstream);
        return 9999.9;
    }
}
void VSCHVDC1::set_vsc_stations_count(unsigned int n)
{
    vsc_stations.clear();
    if(n<2)
        n = 2;
    n_converter = n;
    vsc_stations.reserve(n_converter);
    VSC_MODEL_VAR var(get_toolkit());
    for(unsigned int i=0; i<n_converter; ++i)
        vsc_stations.push_back(var);
}

unsigned int VSCHVDC1::get_vsc_stations_count() const
{
    return n_converter;
}


bool VSCHVDC1::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()<34)
        return is_successful;

    string model_name = get_string_data(data[0],"");
    if(model_name!=get_model_name())
        return is_successful;
    // iblcok iunblock for instantaneous block or unblock
    // dblcok dunblock for delayed block or unblock
    // ibupass iunbupass for instantaneous bupass or unbupass
    // dbupass dunbupass for delayed bupass or unbupass
    unsigned int ncon = get_integer_data(data[2], "0");
    set_vsc_stations_count(ncon);
    double Ceq = get_double_data(data[3], "0");

    unsigned int data_index=5;
    for(unsigned int i=0; i<ncon; ++i)
    {
        unsigned int converter_bus = get_integer_data(data[data_index],"0"); data_index++;
        VSC_HVDC_CONVERTER_ACTIVE_POWER_CONTROL_MODE active_control_mode = VSC_INVALID_DC_CONTORL;
        string type=get_string_data(data[data_index],""); data_index++;
        if(type=="Vdc_control")
            active_control_mode = VSC_DC_VOLTAGE_CONTORL;
        if(type=="Pac_control")
            active_control_mode = VSC_AC_ACTIVE_POWER_CONTORL;
        VSC_HVDC_CONVERTER_REACTIVE_POWER_CONTROL_MODE reactive_control_mode = VSC_INVALID_AC_CONTORL;
        string mode=get_string_data(data[data_index],""); data_index++;
        if(mode=="Vac_control")
            reactive_control_mode = VSC_AC_VOLTAGE_CONTROL;
        if(mode=="Qac_control")
            reactive_control_mode = VSC_AC_REACTIVE_POWER_CONTROL;
        set_converter_bus(i, converter_bus);
        set_converter_active_control_mode(i, active_control_mode);
        set_converter_reactive_control_mode(i, reactive_control_mode);
        if(active_control_mode==1)  //1-dc_voltage_control  2-active_power_control
        {
            if(reactive_control_mode==1)  //1-ac_voltage_control 2-reactive_power_control
            {
                double kp_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double max_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double min_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double kp_uc = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_uc = get_double_data(data[data_index],"0.0"); data_index++;
                double max_uc = get_double_data(data[data_index],"0.0"); data_index++;
                double min_uc = get_double_data(data[data_index],"0.0"); data_index++;
                set_converter_dc_voltage_control_T_in_s(i,Ti_ud);
                set_converter_dc_voltage_Umax(i,max_ud);
                set_converter_dc_voltage_Umin(i,min_ud);
                set_converter_dc_voltage_kpud(i,kp_ud);
                set_converter_ac_voltage_control_T_in_s(i,Ti_uc);
                set_converter_ac_voltage_Umax(i,max_uc);
                set_converter_dc_voltage_Umin(i,max_uc);
                set_converter_dc_voltage_kpuc(i,kp_uc);
                set_dc_voltage_ceq(i,Ceq);
            }
            if(reactive_control_mode==2)
            {
                double kp_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double max_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double min_ud = get_double_data(data[data_index],"0.0"); data_index++;
                double kp_Q = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_Q = get_double_data(data[data_index],"0.0"); data_index++;
                double max_Q = get_double_data(data[data_index],"0.0"); data_index++;
                double min_Q = get_double_data(data[data_index],"0.0"); data_index++;
                set_converter_dc_voltage_control_T_in_s(i,Ti_ud);
                set_converter_dc_voltage_Umax(i,max_ud);
                set_converter_dc_voltage_Umin(i,min_ud);
                set_converter_dc_voltage_kpud(i,kp_ud);
                set_converter_ac_voltage_control_T_in_s(i,Ti_Q);
                set_converter_ac_voltage_Umax(i,max_Q);
                set_converter_dc_voltage_Umin(i,min_Q);
                set_converter_dc_voltage_kpuc(i,kp_Q);
            }
        }
        if(active_control_mode==2)
        {
            if(reactive_control_mode==1)
            {
                double kp_P = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_P = get_double_data(data[data_index],"0.0"); data_index++;
                double max_P = get_double_data(data[data_index],"0.0"); data_index++;
                double min_P = get_double_data(data[data_index],"0.0"); data_index++;
                double kp_uc = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_uc = get_double_data(data[data_index],"0.0"); data_index++;
                double max_uc = get_double_data(data[data_index],"0.0"); data_index++;
                double min_uc = get_double_data(data[data_index],"0.0"); data_index++;
                set_converter_dc_voltage_control_T_in_s(i,Ti_P);
                set_converter_dc_voltage_Umax(i,max_P);
                set_converter_dc_voltage_Umin(i,min_P);
                set_converter_dc_voltage_kpud(i,kp_P);
                set_converter_ac_voltage_control_T_in_s(i,Ti_uc);
                set_converter_ac_voltage_Umax(i,max_uc);
                set_converter_dc_voltage_Umin(i,max_uc);
                set_converter_dc_voltage_kpuc(i,kp_uc);
            }
            if(reactive_control_mode==2)
            {
                double kp_P = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_P = get_double_data(data[data_index],"0.0"); data_index++;
                double max_P = get_double_data(data[data_index],"0.0"); data_index++;
                double min_P = get_double_data(data[data_index],"0.0"); data_index++;
                double kp_Q = get_double_data(data[data_index],"0.0"); data_index++;
                double Ti_Q = get_double_data(data[data_index],"0.0"); data_index++;
                double max_Q = get_double_data(data[data_index],"0.0"); data_index++;
                double min_Q = get_double_data(data[data_index],"0.0"); data_index++;
                set_converter_dc_voltage_control_T_in_s(i,Ti_P);
                set_converter_dc_voltage_Umax(i,max_P);
                set_converter_dc_voltage_Umin(i,min_P);
                set_converter_dc_voltage_kpud(i,kp_P);
                set_converter_ac_voltage_control_T_in_s(i,Ti_Q);
                set_converter_ac_voltage_Umax(i,max_Q);
                set_converter_dc_voltage_Umin(i,min_Q);
                set_converter_dc_voltage_kpuc(i,kp_Q);
            }
        }
    }

    is_successful = true;

    return is_successful;
}

bool VSCHVDC1::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VSCHVDC1::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VSCHVDC1::setup_block_toolkit_and_parameters()
{
    ;
}
void VSCHVDC1::initialize()
{
    if(not is_model_initialized())
    {

        VSC_HVDC* vsc_hvdc = get_multi_vsc_pointer();
        setup_block_toolkit_and_parameters();
        unsigned int ncon = get_vsc_stations_count();
        for(unsigned int i=0; i!=ncon; ++i)
        {
            unsigned int bus_number=vsc_hvdc->get_converter_ac_bus(i);
            complex<double> Is = vsc_hvdc->get_converter_ac_current_in_kA_with_ac_bus_number(bus_number);
            complex<double> Us = vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(bus_number);
            double angle = arg(Us);

            double Isd0 = Is.real()*cos(angle)+Is.imag()*sin(angle);
            double Isq0 = -Is.real()*sin(angle)+Is.imag()*cos(angle);
            if(vsc_stations[i].active_power_control_mode == 1)  //1-dc_voltage_control  2-active_power_control
            {
                if(vsc_stations[i].reactive_power_control_mode == 1)   //1-ac_voltage_control 2-reactive_power_control
                {
                    vsc_stations[i].dc_voltage_block.set_output(0.0);
                    vsc_stations[i].dc_voltage_block.initialize();
                    vsc_stations[i].ud_voltage_control_block.set_output(Isd0);
                    vsc_stations[i].ud_voltage_control_block.initialize();

                    vsc_stations[i].ac_voltage_control_block.set_output(Isq0);
                    vsc_stations[i].ac_voltage_control_block.initialize();

                }
                if(vsc_stations[i].reactive_power_control_mode == 2)
                {
                    vsc_stations[i].dc_voltage_block.set_output(0.0);
                    vsc_stations[i].dc_voltage_block.initialize();
                    vsc_stations[i].ud_voltage_control_block.set_output(Isd0);
                    vsc_stations[i].ud_voltage_control_block.initialize();

                    vsc_stations[i].reactive_power_control_block.set_output(Isq0);
                    vsc_stations[i].reactive_power_control_block.initialize();
                }
            }

            if(vsc_stations[i].active_power_control_mode == 2)
            {
                if(vsc_stations[i].reactive_power_control_mode == 1)
                {
                    vsc_stations[i].active_power_control_block.set_output(Isd0);
                    vsc_stations[i].active_power_control_block.initialize();

                    vsc_stations[i].ac_voltage_control_block.set_output(Isq0);
                    vsc_stations[i].ac_voltage_control_block.initialize();
                }
                if(vsc_stations[i].reactive_power_control_mode == 2)
                {
                    vsc_stations[i].active_power_control_block.set_output(Isd0);
                    vsc_stations[i].active_power_control_block.initialize();

                    vsc_stations[i].reactive_power_control_block.set_output(Isq0);
                    vsc_stations[i].reactive_power_control_block.initialize();
                }

            }

        }
    }
}

void VSCHVDC1::run(DYNAMIC_MODE mode)
{
    VSC_HVDC* vsc_hvdc = get_multi_vsc_pointer();
    setup_block_toolkit_and_parameters();
    unsigned int ncon = get_vsc_stations_count();
    for(unsigned int i=0; i!=ncon; ++i)
    {
        unsigned int bus_number=vsc_hvdc->get_converter_ac_bus(i);
        complex<double> Is = vsc_hvdc->get_converter_ac_current_in_kA_with_ac_bus_number(bus_number);
        complex<double> Us = vsc_hvdc->get_converter_ac_voltage_in_kV_with_ac_bus_number(bus_number);
        complex<double> j(0.0,1.0);
        double Ps = (Us * conj(Is)).real();
        double Qs = (Us * conj(Is)).imag();
        double Isd0 = Is.real();
        double Isq0 = Is.imag();

        if(vsc_stations[i].active_power_control_mode == 1)  //1-dc_voltage_control  2-active_power_control
        {
            if(vsc_stations[i].reactive_power_control_mode == 1)   //1-ac_voltage_control 2-reactive_power_control
            {
                double udcref = vsc_hvdc->get_converter_initial_dc_voltage_reference_in_kV(i);
                unsigned int dc_bus_index = vsc_hvdc->get_dc_bus_index_with_converter_index(i);
                double udc = vsc_hvdc->get_dc_bus_Vdc_in_kV(dc_bus_index);
                double p_input = udcref-udc;
                vsc_stations[i].ud_voltage_control_block.set_input(p_input);
                vsc_stations[i].ud_voltage_control_block.run(mode);

                double Usref = vsc_hvdc->get_converter_nominal_ac_voltage_command_in_kV(i);
                double q_input = Usref - Us.real();
                vsc_stations[i].ac_voltage_control_block.set_input(q_input);
                vsc_stations[i].ac_voltage_control_block.run(mode);

            }
            if(vsc_stations[i].reactive_power_control_mode == 2)
            {
                double udcref = vsc_hvdc->get_converter_initial_dc_voltage_reference_in_kV(i);
                unsigned int dc_bus_index = vsc_hvdc->get_dc_bus_index_with_converter_index(i);
                double udc = vsc_hvdc->get_dc_bus_Vdc_in_kV(dc_bus_index);
                double p_input = udcref-udc;
                vsc_stations[i].ud_voltage_control_block.set_input(p_input);
                vsc_stations[i].ud_voltage_control_block.run(mode);

                double Qsref = vsc_hvdc->get_converter_nominal_reactive_power_command_in_pu(i);
                double q_input = Qsref-Qs;
                vsc_stations[i].reactive_power_control_block.set_input(q_input);
                vsc_stations[i].reactive_power_control_block.run(mode);
            }
        }

        if(vsc_stations[i].active_power_control_mode == 2)
        {
            if(vsc_stations[i].reactive_power_control_mode == 1)
            {
                double Psref = vsc_hvdc->get_converter_nominal_ac_active_power_command_in_MW(i);
                double p_input = Ps - Psref;
                vsc_stations[i].active_power_control_block.set_input(p_input);
                vsc_stations[i].active_power_control_block.run(mode);

                double Usref = vsc_hvdc->get_converter_nominal_ac_voltage_command_in_kV(i);
                double q_input = Usref - Us.real();
                vsc_stations[i].ac_voltage_control_block.set_input(q_input);
                vsc_stations[i].ac_voltage_control_block.run(mode);
            }
            if(vsc_stations[i].reactive_power_control_mode == 2)
            {
                double Psref = vsc_hvdc->get_converter_nominal_ac_active_power_command_in_MW(i);
                double p_input = Ps - Psref;
                vsc_stations[i].active_power_control_block.set_input(p_input);
                vsc_stations[i].active_power_control_block.run(mode);

                double Qsref = vsc_hvdc->get_converter_nominal_ac_reactive_power_command_in_Mvar(i);
                double q_input = Qs-Qsref;
                vsc_stations[i].reactive_power_control_block.set_input(q_input);
                vsc_stations[i].reactive_power_control_block.run(mode);
            }
        }

    }
}

void VSCHVDC1::solve_vsc_hvdc_model_without_integration()
{
    ;
}

void VSCHVDC1::check_blocking_logic()
{
    ;
}

void VSCHVDC1::check_bypassing_logic()
{
    ;
}

void VSCHVDC1::check_mode_switching_logic()
{
    ;
}

void VSCHVDC1::check()
{
    ;
}

void VSCHVDC1::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VSCHVDC1::save()
{
    ;
}

string VSCHVDC1::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;

    osstream<<setw(16)<<"dcname"<<", "
            <<setw(10)<<"model_name"<<", ";
    return osstream.str();
}


double VSCHVDC1::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return 0.0;
}



string VSCHVDC1::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VSCHVDC1::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string VSCHVDC1::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void VSCHVDC1::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    VSC_HVDC* vsc_hvdc = (VSC_HVDC*) get_device_pointer();
    unsigned int n = vsc_hvdc->get_converter_count();
    for(unsigned int j=0; j!=n; ++j)
    {
        unsigned int bus = vsc_hvdc->get_converter_ac_bus(j);
        string var_name = "IP@CONVERTER "+num2str(bus);
        add_model_internal_variable_name_and_index_pair(var_name, i); i++;

        var_name = "STATE@FREQUENCY CONTROLLER@CONVERTER "+num2str(vsc_hvdc->get_converter_ac_bus(j));
        add_model_internal_variable_name_and_index_pair(var_name, i); i++;
    }
}

void VSCHVDC1::prepare_model_data_table()
{
    ;
}

double VSCHVDC1::get_model_data_with_name(string par_name) const
{
    ;
}

double VSCHVDC1::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void VSCHVDC1::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);


    return;
}
