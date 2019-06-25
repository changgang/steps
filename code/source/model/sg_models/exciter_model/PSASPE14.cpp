#include "header/model/sg_models/exciter_model/PSASPE14.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
PSASPE14::PSASPE14()
{
    clear();
}

PSASPE14::~PSASPE14()
{
    ;
}
void PSASPE14::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();
}

void PSASPE14::copy_from_const_model(const PSASPE14& model)
{
    clear();

    set_Tr_in_s(model.get_Tr_in_s());
    set_Ka(model.get_Ka());
    set_Ta_in_s(model.get_Ta_in_s());
    set_Kp(model.get_Kp());
    set_Ki(model.get_Ki());
    set_Vrmax_in_pu(model.get_Vrmax_in_pu());
    set_Vrmin_in_pu(model.get_Vrmin_in_pu());
    set_Kifd(model.get_Kifd());
    set_Tifd_in_s(model.get_Tifd_in_s());
    set_IKp(model.get_IKp());
    set_IKi(model.get_IKi());
    set_Vfmax_in_pu(model.get_Vfmax_in_pu());
    set_Vfmin_in_pu(model.get_Vfmin_in_pu());
    set_Kt(model.get_Kt());
    set_Tt_in_s(model.get_Tt_in_s());
    set_Efdmax_in_pu(model.get_Efdmax_in_pu());
    set_Efdmin_in_pu(model.get_Efdmin_in_pu());
}

PSASPE14::PSASPE14(const PSASPE14& model) : EXCITER_MODEL()
{
    copy_from_const_model(model);
}

PSASPE14& PSASPE14::operator=(const PSASPE14& model)
{
    if(this==(&model)) return *this;

    copy_from_const_model(model);

    return (*this);
}

string PSASPE14::get_model_name() const
{
    return "PSASPE14";
}

void PSASPE14::set_Tr_in_s(double T)
{
    sensor.set_T_in_s(T);
}

void PSASPE14::set_Ka(double K)
{
    regulator.set_K(K);
}

void PSASPE14::set_Ta_in_s(double T)
{
    regulator.set_T_in_s(T);
}

void PSASPE14::set_Kp(double K)
{
    regulator_pi.set_Kp(K);
}

void PSASPE14::set_Ki(double K)
{
    regulator_pi.set_Ki(K);
}

void PSASPE14::set_Vrmax_in_pu(double vmax)
{
    regulator_pi.set_upper_limit(vmax);
}

void PSASPE14::set_Vrmin_in_pu(double vmin)
{
    regulator_pi.set_lower_limit(vmin);
}

void PSASPE14::set_Kifd(double K)
{
    ifd_feedback.set_K(K);
}

void PSASPE14::set_Tifd_in_s(double T)
{
    ifd_feedback.set_T_in_s(T);
}

void PSASPE14::set_IKp(double K)
{
    current_pi.set_Kp(K);
}

void PSASPE14::set_IKi(double K)
{
    current_pi.set_Ki(K);
}

void PSASPE14::set_Vfmax_in_pu(double vmax)
{
    current_pi.set_upper_limit(vmax);
}

void PSASPE14::set_Vfmin_in_pu(double vmin)
{
    current_pi.set_lower_limit(vmin);
}

void PSASPE14::set_Kt(double K)
{
    rectifier.set_K(K);
}

void PSASPE14::set_Tt_in_s(double T)
{
    rectifier.set_T_in_s(T);
}

void PSASPE14::set_Efdmax_in_pu(double emax)
{
    Efdmax = emax;
}

void PSASPE14::set_Efdmin_in_pu(double emin)
{
    Efdmin = emin;
}

double PSASPE14::get_Tr_in_s() const
{
    return sensor.get_T_in_s();
}

double PSASPE14::get_Ka() const
{
    return regulator.get_K();
}

double PSASPE14::get_Ta_in_s() const
{
    return regulator.get_T_in_s();
}

double PSASPE14::get_Kp() const
{
    return regulator_pi.get_Kp();
}

double PSASPE14::get_Ki() const
{
    return regulator_pi.get_Ki();
}

double PSASPE14::get_Vrmax_in_pu() const
{
    return regulator_pi.get_upper_limit();
}

double PSASPE14::get_Vrmin_in_pu() const
{
    return regulator_pi.get_lower_limit();
}

double PSASPE14::get_Kifd() const
{
    return ifd_feedback.get_K();
}

double PSASPE14::get_Tifd_in_s() const
{
    return ifd_feedback.get_T_in_s();
}

double PSASPE14::get_IKp() const
{
    return current_pi.get_Kp();
}

double PSASPE14::get_IKi() const
{
    return current_pi.get_Ki();
}

double PSASPE14::get_Vfmax_in_pu() const
{
    return current_pi.get_upper_limit();
}

double PSASPE14::get_Vfmin_in_pu() const
{
    return current_pi.get_lower_limit();
}

double PSASPE14::get_Kt() const
{
    return rectifier.get_K();
}

double PSASPE14::get_Tt_in_s() const
{
    return rectifier.get_T_in_s();
}

double PSASPE14::get_Efdmax_in_pu() const
{
    return Efdmax;
}

double PSASPE14::get_Efdmin_in_pu() const
{
    return Efdmin;
}

bool PSASPE14::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=20)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            size_t i=3;
            set_Tr_in_s(get_double_data(data[i],"0.0")); ++i;
            set_Ka(get_double_data(data[i],"0.0")); ++i;
            set_Ta_in_s(get_double_data(data[i],"0.0")); ++i;
            set_Kp(get_double_data(data[i],"0.0")); ++i;
            set_Ki(get_double_data(data[i],"0.0")); ++i;
            set_Vrmax_in_pu(get_double_data(data[i],"0.0")); ++i;
            set_Vrmin_in_pu(get_double_data(data[i],"0.0")); ++i;
            set_Kifd(get_double_data(data[i],"0.0")); ++i;
            set_Tifd_in_s(get_double_data(data[i],"0.0")); ++i;
            set_IKp(get_double_data(data[i],"0.0")); ++i;
            set_IKi(get_double_data(data[i],"0.0")); ++i;
            set_Vfmax_in_pu(get_double_data(data[i],"0.0")); ++i;
            set_Vfmin_in_pu(get_double_data(data[i],"0.0")); ++i;
            set_Kt(get_double_data(data[i],"0.0")); ++i;
            set_Tt_in_s(get_double_data(data[i],"0.0")); ++i;
            set_Efdmax_in_pu(get_double_data(data[i],"0.0")); ++i;
            set_Efdmin_in_pu(get_double_data(data[i],"0.0")); ++i;

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool PSASPE14::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool PSASPE14::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPE14::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);

    sensor.set_toolkit(toolkit);
    regulator.set_toolkit(toolkit);
    regulator_pi.set_toolkit(toolkit); regulator_pi.set_limiter_type(NON_WINDUP_LIMITER);
    ifd_feedback.set_toolkit(toolkit);
    current_pi.set_toolkit(toolkit); current_pi.set_limiter_type(NON_WINDUP_LIMITER);
    rectifier.set_toolkit(toolkit);
}

void PSASPE14::initialize()
{
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        if(generator!=NULL)
        {
            SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
            if(gen_model!=NULL)
            {
                if(not gen_model->is_model_initialized())
                    gen_model->initialize();

                setup_block_toolkit_and_parameters();

                double Ecomp = get_compensated_voltage_in_pu();
                double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();
                double Ifd = get_field_current_in_pu();

                rectifier.set_output(Efd);
                rectifier.initialize();

                current_pi.set_output(Efd/get_Kt());
                current_pi.initialize();

                ifd_feedback.set_output(Ifd);
                ifd_feedback.initialize();

                regulator_pi.set_output(Ifd);
                regulator_pi.initialize();

                regulator.set_output(0.0);
                regulator.initialize();

                sensor.set_output(Ecomp);
                sensor.initialize();

                set_voltage_reference_in_pu(Ecomp);

                set_flag_model_initialized_as_true();
            }
        }
    }
}

void PSASPE14::run(DYNAMIC_MODE mode)
{
    GENERATOR* generator = get_generator_pointer();
    if(generator!=NULL)
    {
        double Ecomp = get_compensated_voltage_in_pu();
        double Vref = get_voltage_reference_in_pu();
        double Vs = get_stabilizing_signal_in_pu();
        double Ifd = get_field_current_in_pu();


        sensor.set_input(Ecomp);
        sensor.run(mode);

        double Verror = sensor.get_output()-Vref;
        double input = Vs - Verror;

        regulator.set_input(input);
        regulator.run(mode);

        regulator_pi.set_input(regulator.get_output());
        regulator_pi.run(mode);

        ifd_feedback.set_input(Ifd);
        ifd_feedback.run(mode);

        current_pi.set_input(regulator_pi.get_output()-ifd_feedback.get_output());
        current_pi.run(mode);

        rectifier.set_input(current_pi.get_output());
        rectifier.run(mode);

        if(mode == UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

double PSASPE14::get_excitation_voltage_in_pu() const
{
    double efd = rectifier.get_output();
    if(efd>get_Efdmax_in_pu())
        efd = get_Efdmax_in_pu();
    else
    {
        if(efd<get_Efdmin_in_pu())
            efd = get_Efdmin_in_pu();
    }
    return efd;
}

void PSASPE14::check()
{
    ;
}

void PSASPE14::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPE14::save()
{
    ;
}

string PSASPE14::get_standard_model_string() const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(8)<<setprecision(6)<<get_Tr_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Ka()<<", "
      <<setw(8)<<setprecision(6)<<get_Ta_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Kp()<<", "
      <<setw(8)<<setprecision(6)<<get_Ki()<<", "
      <<setw(8)<<setprecision(6)<<get_Vrmax_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Vrmin_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Kifd()<<", "
      <<setw(8)<<setprecision(6)<<get_Tifd_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_IKp()<<", "
      <<setw(8)<<setprecision(6)<<get_IKi()<<", "
      <<setw(8)<<setprecision(6)<<get_Vfmax_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Vfmin_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Kt()<<", "
      <<setw(8)<<setprecision(6)<<get_Tt_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_Efdmax_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_Efdmin_in_pu()<<"  /";

    return osstream.str();
}

void PSASPE14::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("TR", i); i++;
    add_model_data_name_and_index_pair("KA", i); i++;
    add_model_data_name_and_index_pair("TA", i); i++;
    add_model_data_name_and_index_pair("KP", i); i++;
    add_model_data_name_and_index_pair("KI", i); i++;
    add_model_data_name_and_index_pair("VRMAX", i); i++;
    add_model_data_name_and_index_pair("VRMIN", i); i++;
    add_model_data_name_and_index_pair("KIFD", i); i++;
    add_model_data_name_and_index_pair("TIFD", i); i++;
    add_model_data_name_and_index_pair("IKP", i); i++;
    add_model_data_name_and_index_pair("IKI", i); i++;
    add_model_data_name_and_index_pair("VFMAX", i); i++;
    add_model_data_name_and_index_pair("VFMIN", i); i++;
    add_model_data_name_and_index_pair("KT", i); i++;
    add_model_data_name_and_index_pair("TT", i); i++;
    add_model_data_name_and_index_pair("EFDMAX", i); i++;
    add_model_data_name_and_index_pair("EFDMIN", i); i++;
}

double PSASPE14::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="TR")
        return get_Tr_in_s();
    if(par_name=="KA")
        return get_Ka();
    if(par_name=="TA")
        return get_Ta_in_s();
    if(par_name=="KP")
        return get_Kp();
    if(par_name=="KI")
        return get_Ki();
    if(par_name=="VRMAX")
        return get_Vrmax_in_pu();
    if(par_name=="VRMIN")
        return get_Vrmin_in_pu();
    if(par_name=="KIFD")
        return get_Kifd();
    if(par_name=="TIFD")
        return get_Tifd_in_s();
    if(par_name=="IKP")
        return get_IKp();
    if(par_name=="IKI")
        return get_IKi();
    if(par_name=="VFMAX")
        return get_Vfmax_in_pu();
    if(par_name=="VFMIN")
        return get_Vfmin_in_pu();
    if(par_name=="KT")
        return get_Kt();
    if(par_name=="TT")
        return get_Tt_in_s();
    if(par_name=="EFDMAX")
        return get_Efdmax_in_pu();
    if(par_name=="EFDMIN")
        return get_Efdmin_in_pu();

    return 0.0;
}

void PSASPE14::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="KA")
        return;

    return;
}


void PSASPE14::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REGULATOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@REGULATOR PI", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@IFD FEEDBACKER", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@CURRENT PI", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@RECTIFIER", i); i++;
}

double PSASPE14::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@SENSOR")
        return sensor.get_state();

    if(var_name == "STATE@REGULATOR")
        return regulator.get_state();

    if(var_name == "STATE@REGULATOR PI")
        return regulator_pi.get_state();

    if(var_name == "STATE@IFD FEEDBACKER")
        return ifd_feedback.get_state();

    if(var_name == "STATE@CURRENT PI")
        return current_pi.get_state();

    if(var_name == "STATE@RECTIFIER")
        return rectifier.get_state();

    return 0.0;
}


string PSASPE14::get_dynamic_data_in_psse_format() const
{
    return "";
}

string PSASPE14::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string PSASPE14::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
