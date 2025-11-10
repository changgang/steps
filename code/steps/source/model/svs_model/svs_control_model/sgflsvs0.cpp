#include "header/model/svs_model/svs_control_model/sgflsvs0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
SGFLSVS0::SGFLSVS0(STEPS& toolkit) : SVS_CONTROL_MODEL(toolkit),
                                     voltage_sensor_block(toolkit),
                                     phase_tuner_block(toolkit),
                                     voltage_regulate_block(toolkit),
                                     firing_delay_block(toolkit)
{
    clear();
}

SGFLSVS0::~SGFLSVS0()
{
}

void SGFLSVS0::clear()
{
    set_model_float_parameter_count(15);
    voltage_regulate_block.set_limiter_type(WINDUP_LIMITER);
}

void SGFLSVS0::copy_from_const_model(const SGFLSVS0& model)
{
    set_toolkit(model.get_toolkit());
    voltage_sensor_block.set_toolkit(model.get_toolkit());
    phase_tuner_block.set_toolkit(model.get_toolkit());
    voltage_regulate_block.set_toolkit(model.get_toolkit());
    firing_delay_block.set_toolkit(model.get_toolkit());

    clear();

    set_svs_type(model.get_svs_type());
    set_Tc1_in_s(model.get_Tc1_in_s());
    set_Tb1_in_s(model.get_Tb1_in_s());
    set_Tc2_in_s(model.get_Tc2_in_s());
    set_Tb2_in_s(model.get_Tb2_in_s());
    set_Kp(model.get_Kp());
    set_Ki(model.get_Ki());
    set_T0_in_s(model.get_T0_in_s());
    set_Xc(model.get_Xc());
    set_Qnmax(model.get_Qnmax());
    set_Qnmin(model.get_Qnmin());
    set_Vemax(model.get_Vemax());
    set_Vemin(model.get_Vemin());
}

SGFLSVS0::SGFLSVS0(const SGFLSVS0& model) : SVS_CONTROL_MODEL(model.get_toolkit()),
                                            voltage_sensor_block(model.get_toolkit()),
                                            phase_tuner_block(model.get_toolkit()),
                                            voltage_regulate_block(model.get_toolkit()),
                                            firing_delay_block(model.get_toolkit())
{
    copy_from_const_model(model);
}

SGFLSVS0& SGFLSVS0::operator=(const SGFLSVS0& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

void SGFLSVS0::set_svs_type(SVS_TYPE type)
{
    this->svs_type = type;
}

void SGFLSVS0::set_Tc1_in_s(double Tc1)
{
    voltage_sensor_block.set_T1_in_s(Tc1);
}

void SGFLSVS0::set_Tb1_in_s(double Tb1)
{
    voltage_sensor_block.set_T2_in_s(Tb1);
}

void SGFLSVS0::set_Tc2_in_s(double Tc2)
{
    phase_tuner_block.set_T1_in_s(Tc2);
}

void SGFLSVS0::set_Tb2_in_s(double Tb2)
{
    phase_tuner_block.set_T2_in_s(Tb2);
}

void SGFLSVS0::set_Kp(double Kp)
{
    voltage_regulate_block.set_Kp(Kp);
}

void SGFLSVS0::set_Ki(double Ki)
{
    voltage_regulate_block.set_Ki(Ki);
}

void SGFLSVS0::set_T0_in_s(double T0)
{
    firing_delay_block.set_T_in_s(T0);
}

void SGFLSVS0::set_Xc(double Xc)
{
    this->Xc = Xc;
}

void SGFLSVS0::set_Vemax(double Vemax)
{
    this->Vemax = Vemax;
}

void SGFLSVS0::set_Vemin(double Vemin)
{
    this->Vemin = Vemin;
}

void SGFLSVS0::set_Qnmax(double Qnmax)
{
    voltage_regulate_block.set_upper_limit(Qnmax);
}

void SGFLSVS0::set_Qnmin(double Qnmin)
{
    voltage_regulate_block.set_lower_limit(Qnmin);
}

SVS_TYPE SGFLSVS0::get_svs_type() const
{
    return svs_type;
}

double SGFLSVS0::get_Tc1_in_s() const
{
    return voltage_sensor_block.get_T1_in_s();
}

double SGFLSVS0::get_Tb1_in_s() const
{
    return voltage_sensor_block.get_T2_in_s();
}

double SGFLSVS0::get_Tc2_in_s() const
{
    return phase_tuner_block.get_T1_in_s();
}

double SGFLSVS0::get_Tb2_in_s() const
{
    return phase_tuner_block.get_T2_in_s();
}

double SGFLSVS0::get_Kp() const
{
    return voltage_regulate_block.get_Kp();
}

double SGFLSVS0::get_Ki() const
{
    return voltage_regulate_block.get_Ki();
}

double SGFLSVS0::get_T0_in_s() const
{
    return firing_delay_block.get_T_in_s();
}

double SGFLSVS0::get_Xc() const
{
    return Xc;
}

double SGFLSVS0::get_Vemax() const
{
    return Vemax;
}

double SGFLSVS0::get_Vemin() const
{
    return Vemin;
}

double SGFLSVS0::get_Qnmax() const
{
    return voltage_regulate_block.get_upper_limit();
}

double SGFLSVS0::get_Qnmin() const
{
    return voltage_regulate_block.get_lower_limit();
}

string SGFLSVS0::get_model_name() const
{
    return "SGFLSVS0";
}

bool SGFLSVS0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=16)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int type;
            double Tc1, Tb1, Tc2, Tb2, Kp, Ki, T0, Xc, Qnmax, Qnmin, Vemax, Vemin;

            unsigned int i=3;
            type = get_integer_data(data[i],"0"); i++;
            Tc1 = get_double_data(data[i],"0.0"); i++;
            Tb1 = get_double_data(data[i],"0.0"); i++;
            Tc2 = get_double_data(data[i],"0.0"); i++;
            Tb2 = get_double_data(data[i],"0.0"); i++;
            Kp = get_double_data(data[i],"0.0"); i++;
            Ki = get_double_data(data[i],"0.0"); i++;
            T0 = get_double_data(data[i],"0.0"); i++;
            Xc = get_double_data(data[i],"0.0"); i++;
            Qnmax = get_double_data(data[i],"0.0"); i++;
            Qnmin = get_double_data(data[i],"0.0"); i++;
            Vemax = get_double_data(data[i],"0.0"); i++;
            Vemin = get_double_data(data[i],"0.0");

            switch(type)
            {
                case 1:
                    set_svs_type(SVC);
                    break;
                case 2:
                default:
                    set_svs_type(SVG);
                    break;
            }
            set_Tc1_in_s(Tc1);
            set_Tb1_in_s(Tb1);
            set_Tc2_in_s(Tc2);
            set_Tb2_in_s(Tb2);
            set_Kp(Kp);
            set_Ki(Ki);
            set_T0_in_s(T0);
            set_Xc(Xc);
            set_Qnmax(Qnmax);
            set_Qnmin(Qnmin);
            set_Vemax(Vemax);
            set_Vemin(Vemin);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool SGFLSVS0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool SGFLSVS0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void SGFLSVS0::setup_block_toolkit_and_parameters()
{
}

void SGFLSVS0::initialize()
{
    if(not is_model_initialized())
    {
        setup_block_toolkit_and_parameters();

        double Qn_pu = get_Q_nominal_in_pu_base_on_sbase();
        firing_delay_block.set_output(Qn_pu);
        firing_delay_block.initialize();
        voltage_regulate_block.set_output(Qn_pu);
        voltage_regulate_block.initialize();

        phase_tuner_block.set_output(0.0);
        phase_tuner_block.initialize();

        double V0 = get_terminal_voltage_in_pu();
        voltage_sensor_block.set_output(0);
        voltage_sensor_block.initialize();
        switch(svs_type)
        {
            case SVC:
                V0 += Qn_pu * get_terminal_voltage_in_pu() * Xc;
                break;
            case SVG:
            default:
                V0 += Qn_pu * Xc;
                break;
        }
        set_Vref_in_pu(V0);
        set_flag_model_initialized_as_true();
    }
}

void SGFLSVS0::check_limits_in_initialization()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    double Qn_pu = get_Q_nominal_in_pu_base_on_sbase();
    double Qnmin = voltage_regulate_block.get_lower_limit();
    double Qnmax = voltage_regulate_block.get_upper_limit();
    if(Qn_pu<Qnmin)
    {
        osstream<<"Initialization Error. Qn0 < Qnmin is not allowed for SGFLSVS0 model of "<<get_compound_device_name()<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
    if(Qn_pu>Qnmax)
    {
        osstream<<"Initialization Error. Qn0 > Qnmax is not allowed for SGFLSVS0 model of "<<get_compound_device_name()<<".";
        toolkit.show_information_with_leading_time_stamp(osstream);
    }
}

void SGFLSVS0::run(DYNAMIC_MODE mode)
{
    double v = get_terminal_voltage_in_pu();
    voltage_sensor_block.set_input(v);
    voltage_sensor_block.run(mode);

   double input = get_Vref_in_pu() - voltage_sensor_block.get_output();

    switch(svs_type)
    {
        case SVC:
            input -= firing_delay_block.get_output() * v * Xc;
            break;
        case SVG:
        default:
            input -= firing_delay_block.get_output() * Xc;
            break;
    }
    if(input>Vemax)
    {
        input = Vemax;
    }
    if(input<Vemin)
    {
        input = Vemin;
    }
    phase_tuner_block.set_input(input);
    phase_tuner_block.run(mode);

    input = phase_tuner_block.get_output();
    voltage_regulate_block.set_input(input);
    voltage_regulate_block.run(mode);

    input = voltage_regulate_block.get_output();
    firing_delay_block.set_input(input);
    firing_delay_block.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

void SGFLSVS0::check()
{
    ;
}
void SGFLSVS0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void SGFLSVS0::save()
{
    ;
}
string SGFLSVS0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;

    SV_SOURCE* sv_source = get_sv_source_pointer();
    unsigned int bus = sv_source->get_sv_source_bus();
    string identifier = "'"+sv_source->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    unsigned int type = ((svs_type==SVC)? 1 : 2);
    double Tc1 = get_Tc1_in_s();
    double Tb1 = get_Tb1_in_s();
    double Tc2 = get_Tc2_in_s();
    double Tb2 = get_Tb2_in_s();
    double Kp = get_Kp();
    double Ki = get_Ki();
    double T0 = get_T0_in_s();
    double Xc = get_Xc();
    double Qnmax = get_Qnmax();
    double Qnmin = get_Qnmin();
    double Vemax = get_Vemax();
    double Vemin = get_Vemin();

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(8)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<type<<", "
            <<setw(8)<<setprecision(6)<<Tc1<<", "
            <<setw(8)<<setprecision(6)<<Tb1<<", "
            <<setw(8)<<setprecision(6)<<Tc2<<", "
            <<setw(8)<<setprecision(6)<<Tb2<<", "
            <<setw(8)<<setprecision(6)<<Kp<<", "
            <<setw(8)<<setprecision(6)<<Ki<<", \n"
            <<setw(12)<<""
            <<setw(8)<<setprecision(6)<<T0<<", "
            <<setw(8)<<setprecision(6)<<Xc<<", "
            <<setw(8)<<setprecision(6)<<Qnmax<<", "
            <<setw(8)<<setprecision(6)<<Qnmin<<", "
            <<setw(8)<<setprecision(6)<<Vemax<<", "
            <<setw(8)<<setprecision(6)<<Vemin<<" /";

    return osstream.str();
}

void SGFLSVS0::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("SVS TYPE", i); i++;
    add_model_data_name_and_index_pair("TC1", i); i++;
    add_model_data_name_and_index_pair("TB1", i); i++;
    add_model_data_name_and_index_pair("TC2", i); i++;
    add_model_data_name_and_index_pair("TB2", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KI", i); i++;
    add_model_data_name_and_index_pair("T0", i); i++;
    add_model_data_name_and_index_pair("XC", i); i++;
    add_model_data_name_and_index_pair("QNMAX", i); i++;
    add_model_data_name_and_index_pair("QNMIN", i); i++;
    add_model_data_name_and_index_pair("VEMAX", i); i++;
    add_model_data_name_and_index_pair("VEMIN", i); i++;
}

double SGFLSVS0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="SVS TYPE")
        return get_svs_type();
    if(par_name=="TC1")
        return get_Tc1_in_s();
    if(par_name=="TB1")
        return get_Tb1_in_s();
    if(par_name=="TC2")
        return get_Tc2_in_s();
    if(par_name=="TB2")
        return get_Tb2_in_s();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="KI")
        return get_Ki();
    if(par_name=="T0")
        return get_T0_in_s();
    if(par_name=="XC")
        return get_Xc();
    if(par_name=="QNMAX")
        return get_Qnmax();
    if(par_name=="QNMIN")
        return get_Qnmin();
    if(par_name=="VEMAX")
        return get_Vemax();
    if(par_name=="VEMIN")
        return get_Vemin();

    return 0.0;
}

void SGFLSVS0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name == "SVS TYPE")
    {
        int ivalue = int(value);
        SVS_TYPE type = SVC;
        switch(ivalue)
        {
            case 1:
                type = SVC;
                break;
            case 2:
            default:
                type = SVG;
                break;
        }
        return set_svs_type(type);
    }
    if(par_name=="Tc1")
        return set_Tc1_in_s(value);
    if(par_name=="TB1")
        return set_Tb1_in_s(value);
    if(par_name=="TC2")
        return set_Tc2_in_s(value);
    if(par_name=="TB2")
        return set_Tb2_in_s(value);
    if(par_name=="KP")
        return set_Kp(value);
    if(par_name=="KI")
        return set_Ki(value);
    if(par_name=="T0")
        return set_T0_in_s(value);
    if(par_name=="Xc")
        return set_Xc(value);
    if(par_name=="QNMAX")
        return set_Qnmax(value);
    if(par_name=="QNMIN")
        return set_Qnmin(value);
    if(par_name=="VEMAX")
        return set_Vemax(value);
    if(par_name=="VEMIN")
        return set_Vemin(value);
    return;
}

double SGFLSVS0::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void SGFLSVS0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;

    add_model_internal_variable_name_and_index_pair("Q_NOMINAL_IN_PU_BASED_ON_SBASE", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VOLTAGE SENSOR BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@PHASE TUNER BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@VOLTAGE REGULATE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FIRING DELAY BLOCK", i); i++;
}

double SGFLSVS0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "Q_NOMINAL_IN_PU_BASED_ON_SBASE")
        return firing_delay_block.get_output();
    if(var_name == "STATE@VOLTAGE SENSOR BLOCK")
        return voltage_sensor_block.get_state();
    if(var_name == "STATE@PHASE TUNER BLOCK")
        return phase_tuner_block.get_state();
    if(var_name == "STATE@VOLTAGE REGULATE BLOCK")
        return voltage_regulate_block.get_state();
    if(var_name == "STATE@FIRING DELAY BLOCK")
        return firing_delay_block.get_state();
    return 0.0;
}

double SGFLSVS0::get_Q_nominal_in_pu_base_on_sbase()
{
    SV_SOURCE* sv_source = get_sv_source_pointer();
    STEPS& toolkit = get_toolkit();
    double v = get_terminal_voltage_in_pu();
    double Qgen_pu = sv_source->get_q_generation_in_MVar() * toolkit.get_one_over_system_base_power_in_one_over_MVA();
    switch(svs_type)
    {
        case SVC:
        {
            return Qgen_pu / (v*v);
        }
        case SVG:
        default:
        {
            return Qgen_pu / v;
        }
    }
}

double SGFLSVS0::get_terminal_current_in_pu_based_on_sbase()
{
    double Qn = firing_delay_block.get_output();
    switch(svs_type)
    {
        case SVC:
            return Qn * get_terminal_voltage_in_pu();
        case SVG:
        default:
            return Qn;
    }
}

void SGFLSVS0::set_Vref_in_pu(double Vref)
{
    this->Vref = Vref;
}

double SGFLSVS0::get_Vref_in_pu()
{
    return Vref;
}

string SGFLSVS0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string SGFLSVS0::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string SGFLSVS0::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
