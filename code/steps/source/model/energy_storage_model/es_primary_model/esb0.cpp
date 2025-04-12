#include "header/model/energy_storage_model/es_primary_model/esb0.h"
#include "header/basic/utility.h"
#include "header/basic/constants.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

ESB0::ESB0(STEPS& toolkit) : ES_PRIMARY_MODEL(toolkit),
                                Rb_block(toolkit),
                               battery_delay_block(toolkit),
                               Vdc_controller(toolkit),
                               Capacitor(toolkit)
{

}

ESB0::~ESB0()
{
    ;
}

void ESB0::clear()
{
    set_model_float_parameter_count(11);
}

void ESB0::copy_from_const_model(const ESB0& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);

    clear();

}

ESB0::ESB0(const ESB0& model):ES_PRIMARY_MODEL(model.get_toolkit()),
                                    Rb_block(model.get_toolkit()),
                                  battery_delay_block(model.get_toolkit()),
                                  Vdc_controller(model.get_toolkit()),
                                  Capacitor(model.get_toolkit())
{
    copy_from_const_model(model);
}

ESB0& ESB0::operator=(const ESB0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string ESB0::get_model_name() const
{
    return "ESB0";
}

void ESB0::set_converter_number(unsigned int n)
{
    Converter_number = n;
}

void ESB0::set_Nse_per_converter(unsigned int n)
{
    Nse_per_converter = n;
}

void ESB0::set_Nsh_per_converter(unsigned int n)
{
    Nsh_per_converter = n;
}

void ESB0::set_Eoc_per_cell_in_V(double v)
{
    Eoc_per_cell_in_V = v;
}

void ESB0::set_Rb_per_cell_in_mOhm(double r)
{
    double Rb_per_cell_in_Ohm = r/1000;
    Rb_block.set_K(Rb_per_cell_in_Ohm);
}

void ESB0::set_Rp_per_cell_in_mOhm(double r)
{
    Rp_per_cell_in_mOhm = r;
    double Rp_per_cell_in_Ohm = r/1000;
    double Cp_in_F = get_Cp_per_cell_in_mF()/1000;
    battery_delay_block.set_K(Rp_per_cell_in_Ohm);
    battery_delay_block.set_T_in_s(Rp_per_cell_in_Ohm*Cp_in_F);
}

void ESB0::set_Cp_per_cell_in_mF(double c)
{
    Cp_per_cell_in_mF = c;
    double Rp_per_cell_in_Ohm = get_Rp_per_cell_in_mOhm()/1000;
    double Cp_per_cell_in_F = Cp_per_cell_in_mF/1000;
    battery_delay_block.set_T_in_s(Rp_per_cell_in_Ohm*Cp_per_cell_in_F);
}

void ESB0::set_capacitor_nominal_dc_voltage_in_kV(double v)
{
    VdcN_in_kV = v;
}

void ESB0::set_C_per_converter_in_F(double c)
{
    Capacitor.set_T_in_s(c);
}

void ESB0::set_Kpdc_per_converter(double k)
{
    Vdc_controller.set_Kp(k);
}

void ESB0::set_Kidc_per_converter(double k)
{
    Vdc_controller.set_Ki(k);
}

unsigned int ESB0::get_converter_number() const
{
    return Converter_number;
}

unsigned int ESB0::get_Nse_per_converter() const
{
    return Nse_per_converter;
}

unsigned int ESB0::get_Nsh_per_converter() const
{
    return Nsh_per_converter;
}

double ESB0::get_Eoc_per_cell_in_V() const
{
    return Eoc_per_cell_in_V;
}

double ESB0::get_Rb_per_cell_in_mOhm() const
{
    double Rb_per_cell_in_Ohm = Rb_block.get_K();
    return Rb_per_cell_in_Ohm*1000;
}

double ESB0::get_Rp_per_cell_in_mOhm() const
{
    return Rp_per_cell_in_mOhm;
}

double ESB0::get_Cp_per_cell_in_mF() const
{
    return Cp_per_cell_in_mF;
}

double ESB0::get_capacitor_nominal_dc_voltage_in_kV() const
{
    return VdcN_in_kV;
}

double ESB0::get_C_per_converter_in_F() const
{
    return Capacitor.get_T_in_s();
}

double ESB0::get_Kpdc_per_converter() const
{
    return Vdc_controller.get_Kp();
}

double ESB0::get_Kidc_per_converter() const
{
    return Vdc_controller.get_Ki();
}

bool ESB0::setup_model_with_steps_string_vector(vector<string>& data)
{
    ostringstream osstream;

    bool is_successful = false;

    if(data.size()>=14)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int ibus;
            string id;
            unsigned int nConverter, Nse, Nsh;
            double Eoc, Rb, Rp, Cp, VdcN, C, Kpdc, Kidc;

            ibus = (unsigned int)(get_integer_data(data[1],"0"));
            id = get_string_data(data[2],"");

            unsigned int i=3;
            nConverter = (unsigned int)(get_integer_data(data[i],"0")); i++;
            Nse = (unsigned int)(get_integer_data(data[i],"0")); i++;
            Nsh = (unsigned int)(get_integer_data(data[i],"0")); i++;
            VdcN = get_double_data(data[i],"0.0"); i++;
            Eoc = get_double_data(data[i],"0.0"); i++;
            Rb = get_double_data(data[i],"0.0"); i++;
            Rp = get_double_data(data[i],"0.0"); i++;
            Cp = get_double_data(data[i],"0.0"); i++;
            C = get_double_data(data[i],"0.0"); i++;
            Kpdc = get_double_data(data[i],"0.0"); i++;
            Kidc = get_double_data(data[i],"0.0"); i++;

            DEVICE_ID did = get_energy_storage_device_id(ibus, id);
            STEPS& toolkit = get_toolkit();
            POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
            ENERGY_STORAGE* es = psdb.get_energy_storage(did);
            if(es!=NULL)
            {
                set_converter_number(nConverter);
                set_Nse_per_converter(Nse);
                set_Nsh_per_converter(Nsh);
                set_capacitor_nominal_dc_voltage_in_kV(VdcN);
                set_Eoc_per_cell_in_V(Eoc);
                set_Rb_per_cell_in_mOhm(Rb);
                set_Rp_per_cell_in_mOhm(Rp);
                set_Cp_per_cell_in_mF(Cp);
                set_C_per_converter_in_F(C);
                set_Kpdc_per_converter(Kpdc);
                set_Kidc_per_converter(Kidc);

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

bool ESB0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool ESB0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void ESB0::setup_block_toolkit_and_parameters()
{

}

void ESB0::initialize()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        unsigned int nConveter = get_converter_number();
        unsigned int Nse = get_Nse_per_converter();
        unsigned int Nsh = get_Nsh_per_converter();
        double Rb_per_cell_in_Ohm = get_Rb_per_cell_in_mOhm()/1000;
        double Rp_per_cell_in_Ohm = get_Rp_per_cell_in_mOhm()/1000;

        double Vdc_capacitor_in_kV = get_capacitor_nominal_dc_voltage_in_kV();
        double Vdc_capacitor_in_V = Vdc_capacitor_in_kV*1000;

        ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
        ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();
        double P_plant_in_MW = converter_model->get_active_power_generation_including_stator_loss_in_MW();
        double P_per_converter_in_MW = P_plant_in_MW/nConveter;
        double P_per_cell_in_W = P_per_converter_in_MW/Nse/Nsh*1e6;

        double Eoc = get_Eoc_per_cell_in_V();

        double Idc_per_cell_in_A = INFINITE_THRESHOLD;
        double V_battery_per_cell_in_V = INFINITE_THRESHOLD;

        if(fabs(Eoc)<=DOUBLE_EPSILON)
            stage_number = 1;
        else
            stage_number = 2;

        if(stage_number==1)//no DC/DC loop
        {
            V_battery_per_cell_in_V = Vdc_capacitor_in_V/Nse;
            if(fabs(V_battery_per_cell_in_V)<FLOAT_EPSILON)
            {
                osstream<<"Initialization error. The battery dc voltage per cell is Zero in "<<get_model_name()
                        <<" model of "<<get_compound_device_name()<<". Please modify the parameter VdcN.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                Idc_per_cell_in_A = P_per_cell_in_W/V_battery_per_cell_in_V;
            }
            double Eoc_per_cell_in_V = V_battery_per_cell_in_V + Idc_per_cell_in_A*(Rb_per_cell_in_Ohm+Rp_per_cell_in_Ohm);
            set_Eoc_per_cell_in_V(Eoc_per_cell_in_V);

        }
        else  //have DC/DC loop
        {
            double a = 1.0;
            double b = -Eoc;
            double c = P_per_cell_in_W*(Rb_per_cell_in_Ohm+Rp_per_cell_in_Ohm);
            double delta = b*b-4*a*c;
            if(delta>=0)
                V_battery_per_cell_in_V = 0.5*(-b + steps_sqrt(delta))/a;
            else
            {
                osstream<<"Initialization error. The solution for battery dc voltage per cell failed in "<<get_model_name()
                        <<" model of "<<get_compound_device_name()<<". Please increase the parameter Eoc or decrease the parameters Rp and Rb.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            if(fabs(V_battery_per_cell_in_V)<FLOAT_EPSILON)
            {
                osstream<<"Initialization error. The battery dc voltage per cell is Zero in "<<get_model_name()
                        <<" model of "<<get_compound_device_name()<<". Please modify the parameter Eoc, Rp and Rb.";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            else
            {
                Idc_per_cell_in_A = P_per_cell_in_W/V_battery_per_cell_in_V;
            }

        }

        Rb_block.set_output(Idc_per_cell_in_A*Rb_per_cell_in_Ohm);
        Rb_block.initialize();

        Vdc_controller.set_output(P_per_converter_in_MW);
        Vdc_controller.initialize();

        battery_delay_block.set_output(Idc_per_cell_in_A*Rp_per_cell_in_Ohm);
        battery_delay_block.initialize();

        Capacitor.set_output(Vdc_capacitor_in_kV);
        Capacitor.initialize();

        set_flag_model_initialized_as_true();

        if(toolkit.is_detailed_log_enabled())
        {
            osstream<<get_model_name()<<" model of "<<get_compound_device_name()<<" is initialized."<<endl
                    <<"(1) Initial battery Eoc per cell in V = "<<get_Eoc_per_cell_in_V()<<endl
                    <<"(2) Initial battery Idc per cell in A = "<<Idc_per_cell_in_A<<endl
                    <<"(3) Initial battery Vdc per cell in V = "<<V_battery_per_cell_in_V<<endl
                    <<"(4) Initial battery power per cell in W = "<<P_per_cell_in_W<<endl
                    <<"(5) Initial battery power per converter in MW = "<<P_per_converter_in_MW<<endl
                    <<"(6) Initial Capacitor Vdc per converter in kV = "<<Vdc_capacitor_in_kV<<endl
                    <<"(6) Stage number = "<<stage_number<<endl;
            toolkit.show_information_with_leading_time_stamp(osstream);
        }
    }
}

void ESB0::run(DYNAMIC_MODE mode)
{
    double input = 0.0;

    //capacitor
    double Pbattery_per_cell_in_W = get_battery_dc_power_per_cell_in_W();
    double Pbattery_per_cell_in_MW = Pbattery_per_cell_in_W*1e-6;
    double Pbattery_per_converter_in_MW = Pbattery_per_cell_in_MW*get_Nse_per_converter()*get_Nsh_per_converter();

    ENERGY_STORAGE* energy_storage = get_energy_storage_pointer();
    ES_CONVERTER_MODEL* converter_model = energy_storage->get_es_converter_model();
    double P_plant_in_MW = converter_model->get_active_power_generation_including_stator_loss_in_MW();
    double P_per_converter_in_MW = P_plant_in_MW/get_converter_number();

    double Vdc_capacitor_in_kV = Capacitor.get_output();

    input = (Pbattery_per_converter_in_MW-P_per_converter_in_MW)/Vdc_capacitor_in_kV;//kA

    Capacitor.set_input(input);
    Capacitor.run(mode);

    //Vdc controller
    if(stage_number==2)
    {
        double VdcN_per_converter_in_kV = get_capacitor_nominal_dc_voltage_in_kV();
        input = VdcN_per_converter_in_kV - Capacitor.get_output();
        Vdc_controller.set_input(input);
        Vdc_controller.run(mode);
    }


    //battery
    double Idc_per_cell_in_A = get_battery_dc_current_per_cell_in_A();

    Rb_block.set_input(Idc_per_cell_in_A);
    Rb_block.run(mode);

    battery_delay_block.set_input(Idc_per_cell_in_A);
    battery_delay_block.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();


}

double ESB0::get_battery_dc_power_per_cell_in_W() const
{
    return get_battery_dc_voltage_per_cell_in_V()*get_battery_dc_current_per_cell_in_A();
}

double ESB0::get_battery_dc_current_per_cell_in_A() const
{
    double I = 0;
    double Vdc = 0;
    if(stage_number==1)
    {
        double Eoc = get_Eoc_per_cell_in_V();
        double Rb = get_Rb_per_cell_in_mOhm()/1000;
        double Rp = get_Rp_per_cell_in_mOhm()/1000;
        Vdc = Capacitor.get_output()*1000/get_Nse_per_converter();
        I = (Eoc - Vdc)/(Rb + Rp);
    }
    else
    {
        double Preq_per_converter_in_MW = Vdc_controller.get_output();
        double Preq_per_converter_in_W = Preq_per_converter_in_MW*1e6;
        double Preq_per_cell_in_W = Preq_per_converter_in_W/get_Nsh_per_converter()/get_Nse_per_converter();
        Vdc = get_battery_dc_voltage_per_cell_in_V();
        I = Preq_per_cell_in_W/Vdc;
    }
    return I;
}

double ESB0::get_battery_dc_voltage_per_cell_in_V() const
{
    double V = get_Eoc_per_cell_in_V()-Rb_block.get_output()-battery_delay_block.get_output();
    return V;
}

void ESB0::initialize_Pmech_as_zero()//no use
{
    return;
}

double ESB0::get_mechanical_power_in_pu_on_mbase() const//no use
{
    return 0.0;
}

double ESB0::get_valve_in_pu() const//no use
{
    return 0.0;
}

double ESB0::get_delta_Y_in_pu() const//no use
{
    return 0.0;
}

double ESB0::get_delta_Yref_in_pu() const//no use
{
    return 0.0;
}

void ESB0::check()
{
    ;
}

void ESB0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void ESB0::save()
{
    ;
}

string ESB0::get_standard_psse_string(bool export_internal_bus_number) const
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

    double Eoc;
    if(stage_number==1)
        Eoc = 0.0;
    else
        Eoc = get_Eoc_per_cell_in_V();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<get_converter_number()<<", "
            <<setw(8)<<get_Nse_per_converter()<<", "
            <<setw(8)<<get_Nsh_per_converter()<<", "
            <<setw(8)<<setprecision(4)<<get_capacitor_nominal_dc_voltage_in_kV()<<", "
            <<setw(8)<<setprecision(4)<<Eoc<<", "
            <<setw(8)<<setprecision(4)<<get_Rb_per_cell_in_mOhm()<<", "
            <<setw(8)<<setprecision(4)<<get_Rp_per_cell_in_mOhm()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<get_Cp_per_cell_in_mF()<<", "
            <<setw(8)<<setprecision(4)<<get_C_per_converter_in_F()<<", "
            <<setw(8)<<setprecision(4)<<get_Kpdc_per_converter()<<", "
            <<setw(8)<<setprecision(4)<<get_Kidc_per_converter()<<" /";

    return osstream.str();
}

void ESB0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("CONVERTER NUMBER", i); i++;
    add_model_data_name_and_index_pair("NSE PER CONVERTER", i); i++;
    add_model_data_name_and_index_pair("NSH PER CONVERTER", i); i++;
    add_model_data_name_and_index_pair("CAPACITOR VDCN PER CONVERTER IN KV", i); i++;
    add_model_data_name_and_index_pair("EOC PER CELL IN V", i); i++;
    add_model_data_name_and_index_pair("RB PER CELL IN MOHM", i); i++;
    add_model_data_name_and_index_pair("RP PER CELL IN MOHM", i); i++;
    add_model_data_name_and_index_pair("CP PER CELL IN MF", i); i++;
    add_model_data_name_and_index_pair("CAPACITOR C PER CONVERTER IN F", i); i++;
    add_model_data_name_and_index_pair("KPDC", i); i++;
    add_model_data_name_and_index_pair("KIDC", i); i++;
}

double ESB0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name == "CONVERTER NUMBER")                      return get_converter_number();
    if(par_name == "NSE PER CONVERTER")                     return get_Nse_per_converter();
    if(par_name == "NSH PER CONVERTER")                     return get_Nsh_per_converter();
    if(par_name == "CAPACITOR VDCN PER CONVERTER IN KV")    return get_capacitor_nominal_dc_voltage_in_kV();
    if(par_name == "EOC PER CELL IN V")                     return get_Eoc_per_cell_in_V();
    if(par_name == "RB PER CELL IN MOHM")                   return get_Rb_per_cell_in_mOhm();
    if(par_name == "RP PER CELL IN MOHM")                   return get_Rp_per_cell_in_mOhm();
    if(par_name == "CP PER CELL IN MF")                     return get_Cp_per_cell_in_mF();
    if(par_name == "CAPACITOR C PER CONVERTER IN F")        return get_C_per_converter_in_F();
    if(par_name == "KPDC")                                  return get_Kpdc_per_converter();
    if(par_name == "KIDC")                                  return get_Kidc_per_converter();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void ESB0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name == "CONVERTER NUMBER")                      set_converter_number((unsigned int)(value));
    if(par_name == "NSE PER CONVERTER")                     set_Nse_per_converter((unsigned int)(value));
    if(par_name == "NSH PER CONVERTER")                     set_Nsh_per_converter((unsigned int)(value));
    if(par_name == "CAPACITOR VDCN PER CONVERTER IN KV")    set_capacitor_nominal_dc_voltage_in_kV(value);
    if(par_name == "EOC PER CELL IN V")                     set_Eoc_per_cell_in_V(value);
    if(par_name == "RB PER CELL IN MOHM")                   set_Rb_per_cell_in_mOhm(value);
    if(par_name == "RP PER CELL IN MOHM")                   set_Rp_per_cell_in_mOhm(value);
    if(par_name == "CP PER CELL IN MF")                     set_Cp_per_cell_in_mF(value);
    if(par_name == "CAPACITOR C PER CONVERTER IN F")        set_C_per_converter_in_F(value);
    if(par_name == "KPDC")                                  set_Kpdc_per_converter(value);
    if(par_name == "KIDC")                                  set_Kidc_per_converter(value);

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double ESB0::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    return mint;
}

void ESB0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("BATTERY DC POWER PER CELL IN W", i); i++;
    add_model_internal_variable_name_and_index_pair("BATTERY DC CURRENT PER CELL IN A", i); i++;
    add_model_internal_variable_name_and_index_pair("BATTERY DC VOLTAGE PER CELL IN V", i); i++;
    add_model_internal_variable_name_and_index_pair("CAPACITOR DC VOLTAGE IN KV", i); i++;
}

double ESB0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "BATTERY DC POWER PER CELL IN W")
        return get_battery_dc_power_per_cell_in_W();
    if(var_name == "BATTERY DC CURRENT PER CELL IN A")
        return get_battery_dc_current_per_cell_in_A();
    if(var_name == "BATTERY DC VOLTAGE PER CELL IN V")
        return get_battery_dc_voltage_per_cell_in_V();
    if(var_name == "CAPACITOR DC VOLTAGE IN KV")
        return Capacitor.get_output();

    return 0.0;
}

string ESB0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string ESB0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string ESB0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
