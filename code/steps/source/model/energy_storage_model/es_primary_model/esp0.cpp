#include "header/model/energy_storage_model/es_primary_model/esp0.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ESP0::ESP0(STEPS& toolkit) : ES_PRIMARY_MODEL(toolkit),
                               gov(toolkit)
{

}

ESP0::~ESP0()
{
    ;
}

void ESP0::clear()
{
    set_model_float_parameter_count(12);
}

void ESP0::copy_from_const_model(const ESP0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

ESP0::ESP0(const ESP0& model):ES_PRIMARY_MODEL(model.get_toolkit()),
                                  gov(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESP0& ESP0::operator=(const ESP0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESP0::get_model_name() const
{
    return "ESP0";
}

void ESP0::set_Tr_in_s(double T)
{
    gov.set_Tr_in_s(T);
}

void ESP0::set_Ka(double k)
{
    gov.set_Ka(k);
}

void ESP0::set_Eb(double k)
{
    gov.set_Eb(k);
}

void ESP0::set_Bp(double k)
{
    gov.set_Bp(k);
}

void ESP0::set_Kp(double k)
{
    gov.set_Kp(k);
}

void ESP0::set_Ki(double k)
{
    gov.set_Ki(k);
}

void ESP0::set_INTmax(double k)
{
    gov.set_INTmax(k);
}

void ESP0::set_INTmin(double k)
{
    gov.set_INTmin(k);
}

void ESP0::set_PImax(double k)
{
    gov.set_PImax(k);
}

void ESP0::set_PImin(double k)
{
    gov.set_PImin(k);
}

void ESP0::set_umax(double k)
{
    gov.set_umax(k);
}

void ESP0::set_umin(double k)
{
    gov.set_umin(k);
}

void ESP0::set_Ts_in_s(double T)
{
    gov.set_Ts_in_s(T);
}

void ESP0::set_Twg_in_s(double T)
{
    gov.set_Twg_in_s(T);
}

double ESP0::get_Tr_in_s() const
{
    return gov.get_Tr_in_s();
}

double ESP0::get_Ka() const
{
    return gov.get_Ka();
}

double ESP0::get_Eb() const
{
    return gov.get_Eb();
}

double ESP0::get_Bp() const
{
    return gov.get_Bp();
}

double ESP0::get_Kp() const
{
    return gov.get_Kp();
}

double ESP0::get_Ki() const
{
    return gov.get_Ki();
}

double ESP0::get_INTmax() const
{
    return gov.get_INTmax();
}

double ESP0::get_INTmin() const
{
    return gov.get_INTmin();
}

double ESP0::get_PImax() const
{
    return gov.get_PImax();
}

double ESP0::get_PImin() const
{
    return gov.get_PImin();
}

double ESP0::get_umax() const
{
    return gov.get_umax();
}

double ESP0::get_umin() const
{
    return gov.get_umin();
}

double ESP0::get_Ts_in_s() const
{
    return gov.get_Ts_in_s();
}

double ESP0::get_Twg_in_s() const
{
    return gov.get_Twg_in_s();
}

bool ESP0::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=15)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            double tr, ka, eb, bp, kp, ki, intmax, intmin,
                    pimax, pimin, umax, umin, ts, twg;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            tr = get_double_data(data[i],"0.0"); i++;
            ka = get_double_data(data[i],"0.0"); i++;
            eb = get_double_data(data[i],"0.0"); i++;
            bp = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            ki = get_double_data(data[i],"0.0"); i++;
            intmax = get_double_data(data[i],"0.0"); i++;
            intmin = get_double_data(data[i],"0.0"); i++;
            pimax = get_double_data(data[i],"0.0"); i++;
            pimin = get_double_data(data[i],"0.0"); i++;
            umax = get_double_data(data[i],"0.0"); i++;
            umin = get_double_data(data[i],"0.0"); i++;
            ts = get_double_data(data[i],"0.0"); i++;
            twg = get_double_data(data[i],"0.0"); i++;

            DEVICE_ID did = get_energy_storage_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            ENERGY_STORAGE* es = psdb.get_energy_storage(did);
            if(es!=NULL)
            {
                set_Tr_in_s(tr);
                set_Ka(ka);
                set_Eb(eb);
                set_Bp(bp);
                set_Kp(kp);
                set_Ki(ki);
                set_INTmax(intmax);
                set_INTmin(intmin);
                set_PImax(pimax);
                set_PImin(pimin);
                set_umax(umax);
                set_umin(umin);
                set_Ts_in_s(ts);
                set_Twg_in_s(twg);

                is_successful = true;

                return is_successful;
            }
            else
            {
                osstream<<"Error when loading data to build "<<get_model_name()<<" model for "<<did.get_compound_device_name()<<endl
                       <<"No such wt generator exists in the power system database.";
                toolkit.show_information_with_leading_time_stamp(osstream);
                return is_successful;
            }
        }
        else
        {
            return is_successful;
        }
    }
    else
    {
        return is_successful;
    }
}

bool ESP0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESP0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESP0::setup_block_toolkit_and_parameters()
{
    gov.set_toolkit(get_toolkit());
    gov.set_device_pointer(get_device_pointer());
}

void ESP0::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        double speed = get_rotor_speed_in_pu();
        ENERGY_STORAGE* es = get_energy_storage_pointer();
        ES_CONVERTER_MODEL* con_model = es->get_es_converter_model();
        double p = con_model->get_active_power_generation_including_stator_loss_in_pu_based_on_mbase();
        double Pref = con_model->get_initial_active_power_command_in_pu_based_on_mbase();
        double Y = get_valve_reference_in_pu();

        ES_MODE mode = get_energy_storage_mode();

        if(mode==ES_CHARGE_MODE)
            gov.set_energy_storage_in_charge_mode();
        else
            gov.set_energy_storage_in_discharge_mode();

        if(mode==ES_OUT_SERVICE_MODE)
            freeze_primary_model();
        else
            unfreeze_primary_model();

        gov.set_initial_speed_in_pu(speed);
        gov.set_initial_mechanical_power_in_pu(p);
        gov.set_initial_active_power_reference_in_pu(Pref);
        gov.set_initial_valve_reference_in_pu(Y);
        gov.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial speedref in pu = "<<speed<<endl
                    <<"(2) Initial mechanical power in pu = "<<p<<endl
                    <<"(3) Initial valve in pu = "<<Y<<endl
                    <<"(4) Initial Pref in pu = "<<Pref<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESP0::run(DYNAMIC_MODE mode)
{
    if(not is_primary_model_frozen())
    {
        double speed = get_rotor_speed_in_pu();
        double speedref = get_rotor_speed_reference_in_pu();
        double Yref = get_valve_reference_in_pu();
        double Pref = get_active_power_reference_in_pu();

        ES_MODE es_mode = get_energy_storage_mode();
        if(es_mode==ES_STOP_CHARGE_MODE or es_mode==ES_START_CHARGE_MODE or es_mode==ES_CHARGE_MODE)
        {
            gov.set_energy_storage_in_charge_mode();
        }
        else
        {
            gov.set_energy_storage_in_discharge_mode();
        }
        gov.set_active_power_reference_in_pu(Pref);
        gov.set_speed_in_pu(speed);
        gov.set_speed_reference_in_pu(speedref);
        gov.set_valve_reference_in_pu(Yref);
        gov.run(mode);

        if(mode==DYNAMIC_UPDATE_MODE)
            set_flag_model_updated_as_true();
    }

}

void ESP0::initialize_Pmech_as_zero()
{
    gov.initialize_mechanical_power_as_zero();
}

double ESP0::get_mechanical_power_in_pu_on_mbase() const
{
    ES_MODE mode = get_energy_storage_mode();
    double pmech = gov.get_mechanical_power_in_pu_on_mbase();

    if(mode==ES_START_DISCHARGE_MODE or mode==ES_STOP_DISCHARGE_MODE or mode==ES_DISCHARGE_MODE)
    {
        if(pmech<0.0)
            pmech = 0.0;
    }
    else
    {
        if(mode==ES_START_CHARGE_MODE or mode==ES_STOP_CHARGE_MODE or mode==ES_CHARGE_MODE)
        {
            if(pmech>0.0)
                pmech = 0.0;
        }
        else
        {
            pmech = 0.0;
        }
    }

    return pmech;
}

double ESP0::get_valve_in_pu() const
{
    return gov.get_valve_in_pu();
}

double ESP0::get_delta_Y_in_pu() const
{
    return gov.get_delta_Y_in_pu();
}

double ESP0::get_delta_Yref_in_pu() const
{
    return gov.get_delta_Yref_in_pu();
}

void ESP0::check()
{
    ;
}

void ESP0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESP0::save()
{
    ;
}

string ESP0::get_standard_psse_string(bool export_internal_bus_number) const
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

    /*osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(4)<<get_Tr_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_Ka()<<", "
            <<setw(8)<<setprecision(4)<<get_dbp()<<", "
            <<setw(8)<<setprecision(4)<<get_dbn()<<", "
            <<setw(8)<<setprecision(4)<<get_Kp()<<", "
            <<setw(8)<<setprecision(4)<<get_Ki()<<", "
            <<setw(8)<<setprecision(4)<<get_INTmax()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<get_INTmin()<<", "
            <<setw(8)<<setprecision(4)<<get_PImax()<<", "
            <<setw(8)<<setprecision(4)<<get_PImin()<<", "
            <<setw(8)<<setprecision(4)<<get_Bp()<<", "
            <<setw(8)<<setprecision(4)<<get_umax()<<", "
            <<setw(8)<<setprecision(4)<<get_umin()<<", "
            <<setw(8)<<setprecision(4)<<get_Ts_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_Twg_in_s()<<" /";
*/
    return osstream.str();
}

void ESP0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TR", i); i++;
    add_model_data_name_and_index_pair("KA", i); i++;
    add_model_data_name_and_index_pair("DBP", i); i++;
    add_model_data_name_and_index_pair("DBN", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KI", i); i++;
    add_model_data_name_and_index_pair("INTMAX", i); i++;
    add_model_data_name_and_index_pair("INTMIN", i); i++;
    add_model_data_name_and_index_pair("PIMAX", i); i++;
    add_model_data_name_and_index_pair("PIMIN", i); i++;
    add_model_data_name_and_index_pair("BP", i); i++;
    add_model_data_name_and_index_pair("UMAX", i); i++;
    add_model_data_name_and_index_pair("UMIN", i); i++;
    add_model_data_name_and_index_pair("TS", i); i++;
    add_model_data_name_and_index_pair("TWG", i); i++;
}

double ESP0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name == "TR")        return get_Tr_in_s();
    if(par_name == "KA")        return get_Ka();
    if(par_name == "KP")        return get_Kp();
    if(par_name == "KI")        return get_Ki();
    if(par_name == "INTMAX")    return get_INTmax();
    if(par_name == "INTMIN")    return get_INTmin();
    if(par_name == "PIMAX")     return get_PImax();
    if(par_name == "PIMIN")     return get_PImin();
    if(par_name == "BP")        return get_Bp();
    if(par_name == "UMAX")      return get_umax();
    if(par_name == "UMIN")      return get_umin();
    if(par_name == "TS")        return get_Ts_in_s();
    if(par_name == "TWG")       return get_Twg_in_s();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void ESP0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name=="TR")      return set_Tr_in_s(value);
    if(par_name=="KA")      return set_Ka(value);
    if(par_name=="KP")      return set_Kp(value);
    if(par_name=="KI")      return set_Ki(value);
    if(par_name=="INTMAX")  return set_INTmax(value);
    if(par_name=="INTMIN")  return set_INTmin(value);
    if(par_name=="PIMAX")   return set_PImax(value);
    if(par_name=="PIMIN")   return set_PImin(value);
    if(par_name=="BP")      return set_Bp(value);
    if(par_name=="UMAX")    return set_umax(value);
    if(par_name=="UMIN")    return set_umin(value);
    if(par_name=="TS")      return set_Ts_in_s(value);
    if(par_name=="TWG")     return set_Twg_in_s(value);

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double ESP0::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tr_in_s()!=0 and mint>get_Tr_in_s())
        mint = get_Tr_in_s();
    if(get_Ts_in_s()!=0 and mint>get_Ts_in_s())
        mint = get_Ts_in_s();
    if(get_Twg_in_s()!=0 and mint>get_Twg_in_s())
        mint = get_Twg_in_s();

    return mint;
}

void ESP0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("DELTA VALVE IN PU", i); i++;
    add_model_internal_variable_name_and_index_pair("DELTA VALVE REFERENCE IN PU", i); i++;
}

double ESP0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "DELTA VALVE IN PU")
        return get_delta_Y_in_pu();
    if(var_name == "DELTA VALVE REFERENCE IN PU")
        return get_delta_Yref_in_pu();

    return 0.0;
}

string ESP0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESP0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESP0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
