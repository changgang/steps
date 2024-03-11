#include "header/model/pvu_models/pv_panel_model/pvpnlx.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

PVPNLX::PVPNLX(STEPS& toolkit) : PV_PANEL_MODEL(toolkit)
{
    clear();
}

PVPNLX::PVPNLX(const PVPNLX& model):PV_PANEL_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

PVPNLX::~PVPNLX()
{
    ;
}

PVPNLX& PVPNLX::operator=(const PVPNLX& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void PVPNLX::copy_from_const_model(const PVPNLX& model)
{
    set_toolkit(model.get_toolkit());

    clear();

    PV_PANEL_MODEL::copy_from_const_model(model);
    set_Pmsta_in_pu_based_on_mbase(model.get_Pmsta_in_pu_based_on_mbase());
    set_b(model.get_b());
    set_Krp(model.get_Krp());

}

void PVPNLX::clear()
{
    set_model_float_parameter_count(4);
    set_b(0.0005);
    set_Krp(0.1);
}

void PVPNLX::set_Pmsta_in_pu_based_on_mbase(double P)
{
    this->Pmsta = P;
}

void PVPNLX::set_b(double b)
{
    this->b = b;
}

void PVPNLX::set_Krp(double Krp)
{
    this->Krp = Krp;
}

double PVPNLX::get_Pmsta_in_pu_based_on_mbase() const
{
    return Pmsta;
}

double PVPNLX::get_b() const
{
    return b;
}

double PVPNLX::get_Krp() const
{
    return Krp;
}

double PVPNLX::get_Pmmp_in_pu(double S)
{
    double Sref = get_Sref_in_Wpm2();
    return (S / Sref * (1 + b / exp(1) * (S - Sref))) * Pmsta;
}

double PVPNLX::get_Pref_in_pu(double S)
{
    double Pmmp = get_Pmmp_in_pu(S);
    return Pmmp * (1 - Krp);
}

void PVPNLX::initialize_S0()
{
    double Sref = get_Sref_in_Wpm2();
    double b_battery = get_b();
    if(Sref-exp(1)/b_battery>0)
    {
        ostringstream osstream;
        STEPS& toolkit = get_toolkit();
        osstream<<"Initialization error. Pmmp (maximum available power from irradiance) of '"<<get_model_name()<<"' model of "<<get_compound_device_name()
          <<" may less than Zero as S (irradiance) changed. Please reduce Sref or b_battery.";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    PV_UNIT* pv_unit = get_pv_unit_pointer();
    PV_CONVERTER_MODEL* pv_converter_model = pv_unit->get_pv_converter_model();
    if(not pv_converter_model->is_model_initialized())
        pv_converter_model->initialize();

    double Pcmd = pv_converter_model->get_initial_active_current_command_in_pu_based_on_mbase() * get_terminal_voltage_in_pu();
    double Pmmax = Pcmd/(1-get_Krp());
    double Pmsta = get_Pmsta_in_pu_based_on_mbase();
    double a = b_battery*Pmsta/(Sref*exp(1));
    double b = Pmsta/Sref-b_battery*Pmsta/exp(1);
    double c = -Pmmax;
    double S0 = 0.5*(-b + steps_sqrt(b*b-4*a*c))/a;
    set_S0_in_Wpm2(S0);
}

double PVPNLX::get_Pref_in_pu_base_on_mbase()
{
    double S = get_solar_irradiance_in_Wpm2();
    return get_Pref_in_pu(S);
}

string PVPNLX::get_model_name() const
{
    return "PVPNLX";
}

bool PVPNLX::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=7)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double sref, pmsta, b, krp;
            unsigned int i=3;
            sref = get_double_data(data[i],"0.0"); i++;
            pmsta = get_double_data(data[i],"0.0"); i++;
            b = get_double_data(data[i],"0.0"); i++;
            krp = get_double_data(data[i],"0.0");

            set_Sref_in_Wpm2(sref);
            set_Pmsta_in_pu_based_on_mbase(pmsta);
            set_b(b);
            set_Krp(krp);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool PVPNLX::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PVPNLX::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}
void PVPNLX::setup_block_toolkit_and_parameters()
{
    ;
}

void PVPNLX::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();
        initialize_S0();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) S0 in Wpm2 = "<<get_S0_in_Wpm2()<<"Wpm2"<<endl
                    <<"(2) Pref in pu = "<<get_Pref_in_pu_base_on_mbase();
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void PVPNLX::run(DYNAMIC_MODE mode)
{
    /*
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not necessary to call. Input mode is provided: "<<mode;
    toolkit.show_information_with_leading_time_stamp(osstream);
    */
}


void PVPNLX::check()
{
    ostringstream osstream;
}

void PVPNLX::report()
{
    ;
}

void PVPNLX::save()
{
    ;
}

string PVPNLX::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(6)<<get_Sref_in_Wpm2()<<", "
            <<setw(8)<<setprecision(6)<<get_Pmsta_in_pu_based_on_mbase()<<", "
            <<setw(8)<<setprecision(6)<<get_b()<<", "
            <<setw(8)<<setprecision(6)<<get_Krp()<<" /";

    return osstream.str();
}

void PVPNLX::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("SREF", i); i++;
    add_model_data_name_and_index_pair("PMSTA", i); i++;
    add_model_data_name_and_index_pair("B", i); i++;
    add_model_data_name_and_index_pair("KRP", i); i++;
}

double PVPNLX::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="SREF")
        return get_Sref_in_Wpm2();
    if(par_name=="PMSTA")
        return get_Pmsta_in_pu_based_on_mbase();
    if(par_name=="B")
        return get_b();
    if(par_name=="KRP")
        return get_Krp();

    return 0.0;
}

void PVPNLX::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="SREF")
        return set_Sref_in_Wpm2(value);
    if(par_name=="PMSTA")
        return set_Pmsta_in_pu_based_on_mbase(value);
    if(par_name=="B")
        return set_b(value);
    if(par_name=="KRP")
        return set_Krp(value);

    return;
}

double PVPNLX::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void PVPNLX::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("S0", i); i++;
}

double PVPNLX::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "S0")
        return get_S0_in_Wpm2();

    return 0.0;
}

string PVPNLX::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PVPNLX::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PVPNLX::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
