#include "header/model/sg_models/turbine_governor_model/IEEEG3.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
using namespace std;
IEEEG3::IEEEG3(STEPS& toolkit) : TURBINE_GOVERNOR_MODEL(toolkit),
                                 governor(toolkit),
                                 servo_motor(toolkit),
                                 feedbacker(toolkit),
                                 water_hammer(toolkit)
{
    clear();
}

IEEEG3::~IEEEG3()
{
}
void IEEEG3::clear()
{
    set_model_float_parameter_count(14);

    governor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_limiter_type(NON_WINDUP_LIMITER);
    servo_motor.set_T_in_s(1.0);
    feedbacker.set_limiter_type(NO_LIMITER);
    water_hammer.set_limiter_type(NO_LIMITER);
    set_a11(0.5);
    set_a13(1.5);
    set_a21(1.0);
    set_a23(1.0);
}
void IEEEG3::copy_from_const_model(const IEEEG3& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    governor.set_toolkit(toolkit);
    servo_motor.set_toolkit(toolkit);
    feedbacker.set_toolkit(toolkit);
    water_hammer.set_toolkit(toolkit);

    clear();

    this->set_TG_in_s(model.get_TG_in_s());
    this->set_TP_in_s(model.get_TP_in_s());
    this->set_Uo_in_pu(model.get_Uo_in_pu());
    this->set_Uc_in_pu(model.get_Uc_in_pu());
    this->set_Pmax_in_pu(model.get_Pmax_in_pu());
    this->set_Pmin_in_pu(model.get_Pmin_in_pu());
    this->set_sigma(model.get_sigma());
    this->set_delta(model.get_delta());
    this->set_TR_in_s(model.get_TR_in_s());
    this->set_TW_in_s(model.get_TW_in_s());
    this->set_a11(model.get_a11());
    this->set_a13(model.get_a13());
    this->set_a21(model.get_a21());
    this->set_a23(model.get_a23());
}

IEEEG3::IEEEG3(const IEEEG3&model) : TURBINE_GOVERNOR_MODEL(model.get_toolkit()),
                                     governor(model.get_toolkit()),
                                     servo_motor(model.get_toolkit()),
                                     feedbacker(model.get_toolkit()),
                                     water_hammer(model.get_toolkit())
{
    copy_from_const_model(model);
}

IEEEG3& IEEEG3::operator=(const IEEEG3& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEEG3::get_model_name() const
{
    return "IEEEG3";
}

void IEEEG3::set_TG_in_s(double T)
{
    governor.set_K(1.0/T);
}

void IEEEG3::set_TP_in_s(double T)
{
    governor.set_T_in_s(T);
}

void IEEEG3::set_Uo_in_pu(double U)
{
    governor.set_upper_limit(U);
}

void IEEEG3::set_Uc_in_pu(double U)
{
    governor.set_lower_limit(U);
}

void IEEEG3::set_Pmax_in_pu(double P)
{
    servo_motor.set_upper_limit(P);
}

void IEEEG3::set_Pmin_in_pu(double P)
{
    servo_motor.set_lower_limit(P);
}

void IEEEG3::set_sigma(double sigma)
{
    this->sigma = sigma;
}

void IEEEG3::set_delta(double delta)
{
    this->delta = delta;
}

void IEEEG3::set_TR_in_s(double T)
{
    feedbacker.set_T_in_s(T);
}

void IEEEG3::set_TW_in_s(double T)
{
    this->TW = T;
}

void IEEEG3::set_a11(double a)
{
    this->a11 = a;
}

void IEEEG3::set_a13(double a)
{
    this->a13 = a;
}

void IEEEG3::set_a21(double a)
{
    this->a21 = a;
}

void IEEEG3::set_a23(double a)
{
    this->a23 = a;
}

double IEEEG3::get_TG_in_s() const
{
    return 1.0/governor.get_K();
}

double IEEEG3::get_TP_in_s() const
{
    return governor.get_T_in_s();
}

double IEEEG3::get_Uo_in_pu() const
{
    return governor.get_upper_limit();
}

double IEEEG3::get_Uc_in_pu() const
{
    return governor.get_lower_limit();
}

double IEEEG3::get_Pmax_in_pu() const
{
    return servo_motor.get_upper_limit();
}

double IEEEG3::get_Pmin_in_pu() const
{
    return servo_motor.get_lower_limit();
}

double IEEEG3::get_sigma() const
{
    return sigma;
}

double IEEEG3::get_delta() const
{
    return delta;
}

double IEEEG3::get_TR_in_s() const
{
    return feedbacker.get_T_in_s();
}

double IEEEG3::get_TW_in_s() const
{
    return TW;
}

double IEEEG3::get_a11() const
{
    return a11;
}

double IEEEG3::get_a13() const
{
    return a13;
}

double IEEEG3::get_a21() const
{
    return a21;
}

double IEEEG3::get_a23() const
{
    return a23;
}


bool IEEEG3::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=17)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double tg, tp, uo, uc, pmax, pmin, sigma, delta, tr, tw, a11, a13, a21, a23;

            unsigned int i=3;
            tg = get_double_data(data[i],"0.0"); i++;
            tp = get_double_data(data[i],"0.0"); i++;
            uo = get_double_data(data[i],"0.0"); i++;
            uc = get_double_data(data[i],"0.0"); i++;
            pmax = get_double_data(data[i],"0.0"); i++;
            pmin = get_double_data(data[i],"0.0"); i++;
            sigma = get_double_data(data[i],"0.0"); i++;
            delta = get_double_data(data[i],"0.0"); i++;
            tr = get_double_data(data[i],"0.0"); i++;
            tw = get_double_data(data[i],"0.0"); i++;
            a11 = get_double_data(data[i],"0.0"); i++;
            a13 = get_double_data(data[i],"0.0"); i++;
            a21 = get_double_data(data[i],"0.0"); i++;
            a23 = get_double_data(data[i],"0.0"); i++;

            set_TG_in_s(tg);
            set_TP_in_s(tp);
            set_Uo_in_pu(uo);
            set_Uc_in_pu(uc);
            set_Pmax_in_pu(pmax);
            set_Pmin_in_pu(pmin);
            set_sigma(sigma);
            set_delta(delta);
            set_TR_in_s(tr);
            set_TW_in_s(tw);
            set_a11(a11);
            set_a13(a13);
            set_a21(a21);
            set_a23(a23);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEEEG3::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEEEG3::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void IEEEG3::setup_block_toolkit_and_parameters()
{
}

void IEEEG3::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            STEPS& toolkit = get_toolkit();

            double delta = get_delta();
            double TR = get_TR_in_s();
            feedbacker.set_K(delta*TR);

            double a11 = get_a11();
            double a13 = get_a13();
            double a21 = get_a21();
            double a23 = get_a23();
            double TW = get_TW_in_s();

            water_hammer.set_K(a23);
            water_hammer.set_T1_in_s((a11-a13*a21/a23)*TW);
            water_hammer.set_T2_in_s(a11*TW);


            double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();

            water_hammer.set_output(pmech0);
            water_hammer.initialize();

            double valve = water_hammer.get_input();
            feedbacker.set_input(valve);
            feedbacker.initialize();


            if(valve>get_Pmax_in_pu())
            {
                osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                  <<"Valve is "<<valve<<", and Pmax is "<<get_Pmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(valve<get_Pmin_in_pu())
            {
                osstream<<"Initialization error. Valve of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                  <<"Valve is "<<valve<<", and Pmin is "<<get_Pmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }

            servo_motor.set_output(valve);
            servo_motor.initialize();

            governor.set_output(0.0);
            governor.initialize();

            double Pref = get_sigma()*valve;

            set_initial_mechanical_power_reference_in_pu_based_on_mbase(Pref);

            set_flag_model_initialized_as_true();
        }
    }
}

void IEEEG3::run(DYNAMIC_MODE mode)
{
    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = Pref-speed-get_sigma()*servo_motor.get_output()-feedbacker.get_output();

    governor.set_input(input);
    governor.run(mode);

    input = governor.get_output();
    servo_motor.set_input(input);
    servo_motor.run(mode);

    double valve = servo_motor.get_output();
    feedbacker.set_input(valve);
    feedbacker.run(mode);

    water_hammer.set_input(valve);
    water_hammer.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double IEEEG3::get_mechanical_power_in_pu_based_on_mbase() const
{
    return water_hammer.get_output();
}

double IEEEG3::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_Pmax_in_pu();
}

double IEEEG3::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_Pmin_in_pu();
}

void IEEEG3::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    double pmax = get_Pmax_in_pu();
    double pmin = get_Pmin_in_pu();
    double uo = get_Uo_in_pu();
    double uc = get_Uc_in_pu();
    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_compound_device_name()<<".\n";
    bool error_found = false;
    if(pmax<=0.0)
    {
        osstream<<"Pmax<=0.0 was detected: Pmax="<<pmax<<"\n";
        error_found = true;
    }
    if(pmax<=pmin)
    {
        osstream<<"Pmax<=Pmin was detected: Pmax="<<pmax<<", Pmin="<<pmin<<"\n";
        error_found = true;
    }
    if(uo<=0.0)
    {
        osstream<<"Uo<=0.0 was detected: Uo="<<uo<<"\n";
        error_found = true;
    }
    if(uc>=0.0)
    {
        osstream<<"Uc>=0.0 was detected: Uc>="<<uc<<"\n";
        error_found = true;
    }
    if(uo<=uc)
    {
        osstream<<"Uo<=Uc was detected: Uo="<<uo<<", Uc="<<uc<<"\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void IEEEG3::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void IEEEG3::save()
{
    ;
}
string IEEEG3::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;

    double TG = get_TG_in_s();
    double TP = get_TP_in_s();
    double Uo = get_Uo_in_pu();
    double Uc = get_Uc_in_pu();
    double Pmax = get_Pmax_in_pu();
    double Pmin = get_Pmin_in_pu();
    double sigma = get_sigma();
    double delta = get_delta();
    double TR = get_TR_in_s();
    double TW = get_TW_in_s();
    double a11 = get_a11();
    double a13 = get_a13();
    double a21 = get_a21();
    double a23 = get_a23();

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
            <<setw(8)<<setprecision(6)<<TG<<", "
            <<setw(8)<<setprecision(6)<<TP<<", "
            <<setw(8)<<setprecision(6)<<Uo<<", "
            <<setw(8)<<setprecision(6)<<Uc<<", "
            <<setw(8)<<setprecision(6)<<Pmax<<", "
            <<setw(8)<<setprecision(6)<<Pmin<<", "
            <<setw(8)<<setprecision(6)<<sigma<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<delta<<", "
            <<setw(8)<<setprecision(6)<<TR<<", "
            <<setw(8)<<setprecision(6)<<TW<<", "
            <<setw(8)<<setprecision(6)<<a11<<", "
            <<setw(8)<<setprecision(6)<<a13<<", "
            <<setw(8)<<setprecision(6)<<a21<<", "
            <<setw(8)<<setprecision(6)<<a23<<" /";
    return osstream.str();
}

void IEEEG3::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TG", i); i++;
    add_model_data_name_and_index_pair("TP", i); i++;
    add_model_data_name_and_index_pair("UO", i); i++;
    add_model_data_name_and_index_pair("UC", i); i++;
    add_model_data_name_and_index_pair("PMAX", i); i++;
    add_model_data_name_and_index_pair("PMIN", i); i++;
    add_model_data_name_and_index_pair("SIGMA", i); i++;
    add_model_data_name_and_index_pair("DELTA", i); i++;
    add_model_data_name_and_index_pair("TR", i); i++;
    add_model_data_name_and_index_pair("TW", i); i++;
    add_model_data_name_and_index_pair("A11", i); i++;
    add_model_data_name_and_index_pair("A13", i); i++;
    add_model_data_name_and_index_pair("A21", i); i++;
    add_model_data_name_and_index_pair("A23", i); i++;
}

double IEEEG3::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="TG")
        return get_TG_in_s();
    if(par_name=="TP")
        return get_TP_in_s();
    if(par_name=="UO")
        return get_Uo_in_pu();
    if(par_name=="UC")
        return get_Uc_in_pu();
    if(par_name=="PMAX")
        return get_Pmax_in_pu();
    if(par_name=="PMIN")
        return get_Pmin_in_pu();
    if(par_name=="SIGMA")
        return get_sigma();
    if(par_name=="DELTA")
        return get_delta();
    if(par_name=="TR")
        return get_TR_in_s();
    if(par_name=="TW")
        return get_TW_in_s();
    if(par_name=="A11")
        return get_a11();
    if(par_name=="A13")
        return get_a13();
    if(par_name=="A21")
        return get_a21();
    if(par_name=="A23")
        return get_a23();

    return 0.0;
}

void IEEEG3::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name=="TG")
        return set_TG_in_s(value);
    if(par_name=="TP")
        return set_TP_in_s(value);
    if(par_name=="UO")
        return set_Uo_in_pu(value);
    if(par_name=="UC")
        return set_Uc_in_pu(value);
    if(par_name=="PMAX")
        return set_Pmax_in_pu(value);
    if(par_name=="PMIN")
        return set_Pmin_in_pu(value);
    if(par_name=="SIGMA")
        return set_sigma(value);
    if(par_name=="DELTA")
        return set_delta(value);
    if(par_name=="TR")
        return set_TR_in_s(value);
    if(par_name=="TW")
        return set_TW_in_s(value);
    if(par_name=="A11")
        return set_a11(value);
    if(par_name=="A13")
        return set_a13(value);
    if(par_name=="A21")
        return set_a21(value);
    if(par_name=="A23")
        return set_a23(value);
}

double IEEEG3::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void IEEEG3::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@GOVERNOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@SERVO MOTOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@FEEDBACKER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@WATER HAMMER", i); i++;
}

double IEEEG3::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@GOVERNOR")
        return governor.get_state();

    if(var_name == "STATE@SERVO MOTOR")
        return servo_motor.get_state();

    if(var_name == "STATE@FEEDBACKER")
        return feedbacker.get_state();

    if(var_name == "STATE@WATER HAMMER")
        return water_hammer.get_state();


    return 0.0;
}

string IEEEG3::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEEG3::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEEG3::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void IEEEG3::linearize()
{
    STEPS_SPARSE_MATRIX* matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GOV", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GOV-GEN", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GOV-TLC", matrix);
    // do linearization
}

STEPS_SPARSE_MATRIX IEEEG3::get_linearized_system_A() const
{
    STEPS_SPARSE_MATRIX A;

    return A;
}
STEPS_SPARSE_MATRIX IEEEG3::get_linearized_system_B() const
{
    STEPS_SPARSE_MATRIX B;

    return B;
}

STEPS_SPARSE_MATRIX IEEEG3::get_linearized_system_C() const
{
    STEPS_SPARSE_MATRIX C;

    return C;
}

STEPS_SPARSE_MATRIX IEEEG3::get_linearized_system_D() const
{
    STEPS_SPARSE_MATRIX D;

    return D;
}

void IEEEG3::get_linearized_system_ABCD(STEPS_SPARSE_MATRIX* A, STEPS_SPARSE_MATRIX* B, STEPS_SPARSE_MATRIX* C, STEPS_SPARSE_MATRIX* D) const
{
    return;
}
