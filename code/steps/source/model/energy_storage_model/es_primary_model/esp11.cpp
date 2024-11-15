#include "header/model/energy_storage_model/es_primary_model/esp11.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ESP11::ESP11(STEPS& toolkit) : ES_PRIMARY_MODEL(toolkit),
                               gov(toolkit)
{

}

ESP11::~ESP11()
{
    ;
}

void ESP11::clear()
{
    set_model_float_parameter_count(12);
}

void ESP11::copy_from_const_model(const ESP11& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

ESP11::ESP11(const ESP11& model):ES_PRIMARY_MODEL(model.get_toolkit()),
                                  gov(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESP11& ESP11::operator=(const ESP11& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESP11::get_model_name() const
{
    return "ESP11";
}

void ESP11::set_Tr_in_s(double T)
{
    gov.set_Tr_in_s(T);
}

void ESP11::set_Ka(double k)
{
    gov.set_Ka(k);
}

void ESP11::set_dbp(double k)
{
    gov.set_dbp(k);
}

void ESP11::set_dbn(double k)
{
    gov.set_dbn(k);
}

void ESP11::set_Kp(double k)
{
    gov.set_Kp(k);
}

void ESP11::set_Ki(double k)
{
    gov.set_Ki(k);
}

void ESP11::set_INTmax(double k)
{
    gov.set_INTmax(k);
}

void ESP11::set_INTmin(double k)
{
    gov.set_INTmin(k);
}

void ESP11::set_PImax(double k)
{
    gov.set_PImax(k);
}

void ESP11::set_PImin(double k)
{
    gov.set_PImin(k);
}

void ESP11::set_Bp(double k)
{
    gov.set_Bp(k);
}

void ESP11::set_umax(double k)
{
    gov.set_umax(k);
}

void ESP11::set_umin(double k)
{
    gov.set_umin(k);
}

void ESP11::set_Ts_in_s(double T)
{
    gov.set_Ts_in_s(T);
}

void ESP11::set_Twg_in_s(double T)
{
    gov.set_Twg_in_s(T);
}

double ESP11::get_Tr_in_s() const
{
    return gov.get_Tr_in_s();
}

double ESP11::get_Ka() const
{
    return gov.get_Ka();
}

double ESP11::get_dbp() const
{
    return gov.get_dbp();
}

double ESP11::get_dbn() const
{
    return gov.get_dbn();
}

double ESP11::get_Kp() const
{
    return gov.get_Kp();
}

double ESP11::get_Ki() const
{
    return gov.get_Ki();
}

double ESP11::get_INTmax() const
{
    return gov.get_INTmax();
}

double ESP11::get_INTmin() const
{
    return gov.get_INTmin();
}

double ESP11::get_PImax() const
{
    return gov.get_PImax();
}

double ESP11::get_PImin() const
{
    return gov.get_PImin();
}

double ESP11::get_Bp() const
{
    return gov.get_Bp();
}

double ESP11::get_umax() const
{
    return gov.get_umax();
}

double ESP11::get_umin() const
{
    return gov.get_umin();
}

double ESP11::get_Ts_in_s() const
{
    return gov.get_Ts_in_s();
}

double ESP11::get_Twg_in_s() const
{
    return gov.get_Twg_in_s();
}

bool ESP11::setup_model_with_steps_string_vector(vector<string>& data)
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
            double tr, ka, dbp, dbn, kp, ki, intmax, intmin, pimax, pimin,
                    bp, umax, umin, ts, twg;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            tr = get_double_data(data[i],"0.0"); i++;
            ka = get_double_data(data[i],"0.0"); i++;
            dbp = get_double_data(data[i],"0.0"); i++;
            dbn = get_double_data(data[i],"0.0"); i++;
            kp = get_double_data(data[i],"0.0"); i++;
            ki = get_double_data(data[i],"0.0"); i++;
            intmax = get_double_data(data[i],"0.0"); i++;
            intmin = get_double_data(data[i],"0.0"); i++;
            pimax = get_double_data(data[i],"0.0"); i++;
            pimin = get_double_data(data[i],"0.0"); i++;
            bp = get_double_data(data[i],"0.0"); i++;
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
                set_dbp(dbp);
                set_dbn(dbn);
                set_Kp(kp);
                set_Ki(ki);
                set_INTmax(intmax);
                set_INTmin(intmin);
                set_PImax(pimax);
                set_PImin(pimin);
                set_Bp(bp);
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

bool ESP11::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESP11::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESP11::setup_block_toolkit_and_parameters()
{
    gov.set_toolkit(get_toolkit());
    gov.set_device_pointer(get_device_pointer());
}

void ESP11::initialize()
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
        double Y = get_valve_reference_in_pu();

        ES_MODE mode = get_energy_storage_mode();
        if(mode==ES_CHARGE_MODE)
            gov.set_energy_storage_in_charge_mode();
        if(mode==ES_DISCHARGE_MODE)
            gov.set_energy_storage_in_discharge_mode();

        gov.set_initial_speed_in_pu(speed);
        gov.set_initial_mechanical_power_in_pu(p);
        gov.set_initial_valve_reference_in_pu(Y);
        gov.initialize();
        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial speedref = "<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESP11::run(DYNAMIC_MODE mode)
{
    double speed = get_rotor_speed_in_pu();
    double speedref = get_rotor_speed_reference_in_pu();
    double Yref = get_valve_reference_in_pu();

    gov.set_speed_in_pu(speed);
    gov.set_speed_reference_in_pu(speedref);
    gov.set_valve_reference_in_pu(Yref);
    gov.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double ESP11::get_mechanical_power_in_pu_on_mbase() const
{
    return gov.get_mechanical_power_in_pu_on_mbase();
}

double ESP11::get_valve_in_pu() const
{
    return gov.get_valve_in_pu();
}

double ESP11::get_delta_Y_in_pu() const
{
    return gov.get_delta_Y_in_pu();
}

void ESP11::check()
{
    ;
}

void ESP11::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESP11::save()
{
    ;
}

string ESP11::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(4)<<get_Tr_in_s()<<", "
            <<setw(8)<<setprecision(4)<<get_Ka()<<", "
            <<setw(8)<<setprecision(4)<<get_dbp()<<", "
            <<setw(8)<<setprecision(4)<<get_dbn()<<" /";

    return osstream.str();
}

void ESP11::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double ESP11::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return 0;

    return 0.0;
}

void ESP11::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="A")
        return ;

    return;
}

double ESP11::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    return mint;
}

void ESP11::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("AA", i); i++;
}

double ESP11::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "A")
        return 0.0;

    return 0.0;
}

string ESP11::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESP11::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESP11::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
