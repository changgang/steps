#include "header/model/sg_models/exciter_model/PSASPE14.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;
PSASPE14::PSASPE14(STEPS& toolkit) : EXCITER_MODEL(toolkit),
                                     sensor(toolkit),
                                     regulator(toolkit),
                                     ifd_feedback(toolkit),
                                     rectifier(toolkit),
                                     regulator_pi(toolkit),
                                     current_pi(toolkit)
{
    clear();
}

PSASPE14::~PSASPE14()
{
    ;
}
void PSASPE14::clear()
{
    regulator_pi.set_limiter_type(NON_WINDUP_LIMITER);
    current_pi.set_limiter_type(NON_WINDUP_LIMITER);
}

void PSASPE14::copy_from_const_model(const PSASPE14& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    sensor.set_toolkit(toolkit);
    regulator.set_toolkit(toolkit);
    regulator_pi.set_toolkit(toolkit);
    ifd_feedback.set_toolkit(toolkit);
    current_pi.set_toolkit(toolkit);
    rectifier.set_toolkit(toolkit);

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

PSASPE14::PSASPE14(const PSASPE14& model) : EXCITER_MODEL(model.get_toolkit()),
                                            sensor(model.get_toolkit()),
                                            regulator(model.get_toolkit()),
                                            ifd_feedback(model.get_toolkit()),
                                            rectifier(model.get_toolkit()),
                                            regulator_pi(model.get_toolkit()),
                                            current_pi(model.get_toolkit())
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
            unsigned int i=3;
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
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void PSASPE14::setup_block_toolkit_and_parameters()
{
}

void PSASPE14::initialize()
{
    ostringstream osstream;
    if(not is_model_initialized())
    {
        GENERATOR* generator = get_generator_pointer();
        STEPS& toolkit = get_toolkit();
        SYNC_GENERATOR_MODEL* gen_model = generator->get_sync_generator_model();
        if(gen_model!=NULL)
        {
            if(not gen_model->is_model_initialized())
                gen_model->initialize();

            setup_block_toolkit_and_parameters();

            double Ecomp = get_compensated_voltage_in_pu();
            double Efd =  get_initial_excitation_voltage_in_pu_from_sync_generator_model();
            double Ifd = get_field_current_in_pu();

            if(Efd>get_Efdmax_in_pu())
            {
                osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                        <<"Efd is "<<Efd<<", and Efdmax is "<<get_Efdmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(Efd<get_Efdmin_in_pu())
            {
                osstream<<"Initialization error. Excitation voltage of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                        <<"Efd is "<<Efd<<", and Efdmin is "<<get_Efdmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            rectifier.set_output(Efd);
            rectifier.initialize();

            double VF = Efd/get_Kt();
            if(VF>get_Vfmax_in_pu())
            {
                osstream<<"Initialization error. VF of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                        <<"VF is "<<VF<<", and VFmax is "<<get_Vfmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(VF<get_Vfmin_in_pu())
            {
                osstream<<"Initialization error. VF of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                        <<"VF is "<<VF<<", and VFmin is "<<get_Vfmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            current_pi.set_output(VF);
            current_pi.initialize();

            double Vr = Ifd*get_Kifd();

            ifd_feedback.set_output(Vr);
            ifd_feedback.initialize();

            if(Vr>get_Vrmax_in_pu())
            {
                osstream<<"Initialization error. Vr of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds upper limit."
                        <<"Vr is "<<Vr<<", and VRmax is "<<get_Vrmax_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            if(VF<get_Vrmin_in_pu())
            {
                osstream<<"Initialization error. Vr of '"<<get_model_name()<<"' model of "<<get_compound_device_name()<<" exceeds lower limit."
                        <<"Vr is "<<Vr<<", and VRmin is "<<get_Vrmin_in_pu()<<".";
                toolkit.show_information_with_leading_time_stamp(osstream);
            }
            regulator_pi.set_output(Vr);
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

void PSASPE14::run(DYNAMIC_MODE mode)
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

    if(mode == DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double PSASPE14::get_excitation_voltage_in_pu()
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
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_compound_device_name()<<".\n";
    bool error_found = false;
    if(get_Ta_in_s()==0.0)
    {
        osstream<<"Ta=0 was detected\n";
        error_found = true;
    }
    if(get_Tt_in_s()==0.0)
    {
        osstream<<"Tt=0 was detected\n";
        error_found = true;
    }
    if(get_Tifd_in_s()==0.0)
    {
        osstream<<"Tifd=0 was detected\n";
        error_found = true;
    }
    double vrmax = get_Vrmax_in_pu();
    double vrmin = get_Vrmin_in_pu();
    if(vrmax<=vrmin)
    {
        osstream<<"VRmax<=VRmin was detected: VRmax="<<vrmax<<", VRmin="<<vrmin<<"\n";
        error_found = true;
    }
    if(vrmax<=0.0)
    {
        osstream<<"VRmax<=0.0 was detected: VRmax="<<vrmax<<"\n";
        error_found = true;
    }
    double vfmax = get_Vfmax_in_pu();
    double vfmin = get_Vfmin_in_pu();
    if(vfmax<=vfmin)
    {
        osstream<<"Vfmax<=Vfmin was detected: Vfmax="<<vfmax<<", Vfmin="<<vfmin<<"\n";
        error_found = true;
    }
    if(vfmax<=0.0)
    {
        osstream<<"VFmax<=0.0 was detected: VFmax="<<vfmax<<"\n";
        error_found = true;
    }
    double efdmax = get_Efdmax_in_pu();
    double efdmin = get_Efdmin_in_pu();
    if(efdmax<=efdmin)
    {
        osstream<<"Efdmax<=Efdmin was detected: Efdmax="<<efdmax<<", Efdmin="<<efdmin<<"\n";
        error_found = true;
    }
    if(efdmax<=0.0)
    {
        osstream<<"Efdmax<=0.0 was detected: Efdmax="<<efdmax<<"\n";
        error_found = true;
    }
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPE14::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void PSASPE14::save()
{
    ;
}

string PSASPE14::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    GENERATOR* gen = get_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_Tr_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Ka()<<", "
            <<setw(8)<<setprecision(6)<<get_Ta_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Kp()<<", "
            <<setw(8)<<setprecision(6)<<get_Ki()<<", "
            <<setw(8)<<setprecision(6)<<get_Vrmax_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_Vrmin_in_pu()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Kifd()<<", "
            <<setw(8)<<setprecision(6)<<get_Tifd_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_IKp()<<", "
            <<setw(8)<<setprecision(6)<<get_IKi()<<", "
            <<setw(8)<<setprecision(6)<<get_Vfmax_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_Vfmin_in_pu()<<", "
            <<setw(8)<<setprecision(6)<<get_Kt()<<", "
            <<setw(8)<<setprecision(6)<<get_Tt_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Efdmax_in_pu()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Efdmin_in_pu()<<" /";

    return osstream.str();
}

void PSASPE14::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
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

double PSASPE14::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_Tr_in_s()!=0.0 and mint>get_Tr_in_s())
        mint = get_Tr_in_s();
    if(get_Ta_in_s()!=0.0 and mint>get_Ta_in_s())
        mint = get_Ta_in_s();
    if(get_Tifd_in_s()!=0.0 and mint>get_Tifd_in_s())
        mint = get_Tifd_in_s();
    if(get_Tt_in_s()!=0.0 and mint>get_Tt_in_s())
        mint = get_Tt_in_s();
    return mint;
}


void PSASPE14::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@SENSOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REGULATOR", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@REGULATOR PI", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@IFD FEEDBACKER", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@CURRENT PI", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@RECTIFIER", i); i++;
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

void PSASPE14::linearize()
{
    STEPS_SPARSE_MATRIX* matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR-GEN", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR-PSS", matrix);
    // do linearization
    matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("AVR-COMP", matrix);
    // do linearization
}

void PSASPE14::build_linearized_matrix_ABCD()
{
    return;
}
