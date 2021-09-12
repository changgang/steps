#include "header/model/sg_models/sync_generator_model/genrou.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;
GENROU::GENROU(STEPS& toolkit) : SYNC_GENERATOR_MODEL(toolkit)
{
    clear();
}

GENROU::~GENROU()
{
}
void GENROU::clear()
{
    set_model_float_parameter_count(14);
}
void GENROU::copy_from_const_model(const GENROU& model)
{
    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    set_blocks_toolkit(toolkit);

    clear();

    //this->set_power_system_database(model.toolkit.get_power_system_database());
    //this->set_device_id(model.get_device_id());

    this->set_Rs(model.get_Rs());
    this->set_Xd(model.get_Xd());
    this->set_Xq(model.get_Xq());
    this->set_Xdp(model.get_Xdp());
    this->set_Xqp(model.get_Xqp());
    this->set_Xpp(model.get_Xpp());
    this->set_Xl(model.get_Xl());
    this->set_Td0p_in_s(model.get_Td0p_in_s());
    this->set_Td0pp_in_s(model.get_Td0pp_in_s());
    this->set_Tq0p_in_s(model.get_Tq0p_in_s());
    this->set_Tq0pp_in_s(model.get_Tq0pp_in_s());
    this->set_Tj_in_s(model.get_Tj_in_s());
    this->set_D(model.get_D());
    this->set_saturation_at_1(model.get_saturation_at_1());
    this->set_saturation_at_1p2(model.get_saturation_at_1p2());
}
GENROU::GENROU(const GENROU& model):SYNC_GENERATOR_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

GENROU& GENROU::operator=(const GENROU& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string GENROU::get_model_name() const
{
    return "GENROU";
}

void GENROU::update_source_impedance()
{
    STEPS& toolkit = get_toolkit();
    GENERATOR* generator = get_generator_pointer();
    complex<double> Z(get_Rs(),get_Xpp());
    complex<double> Zsource = generator->get_source_impedance_in_pu();
    if(Zsource.imag()!=Z.imag())
    {
        ostringstream osstream;
        osstream<<"Warning. The subtransient reactance ("<<get_Xpp()<<") is not equal to generator source reactance ("<<Zsource.imag()<<") for '"<<get_model_name()<<"' of "<<get_compound_device_name()<<"."<<endl
          <<"Source reactance will be updated with subtransient reactance.";
        toolkit.show_information_with_leading_time_stamp(osstream);
        Zsource = complex<double>(Zsource.real(), Z.imag());
        generator->set_source_impedance_in_pu(Zsource);
    }
    set_Rs(Zsource.real());
}

bool GENROU::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=17)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            double xd, xq, xdp, xqp, xpp, xl, td0p, tq0p, td0pp, tq0pp;
            double H, D;
            double s1, s1p2;

            unsigned int i=3;
            td0p = get_double_data(data[i],"0.0"); i++;
            td0pp = get_double_data(data[i],"0.0"); i++;
            tq0p = get_double_data(data[i],"0.0"); i++;
            tq0pp = get_double_data(data[i],"0.0"); i++;
            H = get_double_data(data[i],"0.0"); i++;
            D = get_double_data(data[i],"0.0"); i++;
            xd = get_double_data(data[i],"0.0"); i++;
            xq = get_double_data(data[i],"0.0"); i++;
            xdp = get_double_data(data[i],"0.0"); i++;
            xqp = get_double_data(data[i],"0.0"); i++;
            xpp = get_double_data(data[i],"0.0"); i++;
            xl = get_double_data(data[i],"0.0"); i++;
            s1 = get_double_data(data[i],"0.0"); i++;
            s1p2 = get_double_data(data[i],"0.0");

            set_Xd(xd);
            set_Xq(xq);
            set_Xdp(xdp);
            set_Xqp(xqp);
            set_Xdpp(xpp);
            set_Xqpp(xpp);
            set_Xl(xl);
            set_Td0p_in_s(td0p);
            set_Td0pp_in_s(td0pp);
            set_Tq0p_in_s(tq0p);
            set_Tq0pp_in_s(tq0pp);
            set_H_in_s(H);
            set_D(D);
            set_saturation_at_1(s1);
            set_saturation_at_1p2(s1p2);

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool GENROU::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool GENROU::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void GENROU::initialize()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();

    setup_block_toolkit_and_parameters();

    update_source_impedance();

    initialize_rotor_angle();

    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    double speed = 0.0;
    rotor_speed_block->set_output(speed);
    rotor_speed_block->initialize();

    double rotor_angle = get_rotor_angle_in_rad();

    INTEGRAL_BLOCK* transient_block_d_axis = get_d_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_d_axis = get_d_axis_subtransient_block();
    INTEGRAL_BLOCK* transient_block_q_axis = get_q_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_q_axis = get_q_axis_subtransient_block();


    GENERATOR* generator = get_generator_pointer();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    complex<double> Zsource(get_Rs(), get_Xpp());

    double P = generator->get_p_generation_in_MW();
    double Q = generator->get_q_generation_in_MVar();
    complex<double> S(P*one_over_mbase,Q*one_over_mbase);

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    complex<double> Exy = Vxy+Ixy*Zsource;

    complex<double> Idq = xy2dq_with_angle_in_rad(Ixy, rotor_angle);
    complex<double> Edq = xy2dq_with_angle_in_rad(Exy, rotor_angle);
    complex<double> Flux_dq = Edq*complex<double>(0.0, -1.0);
    double Flux = steps_fast_complex_abs(Flux_dq);
    double delta_XadIfd = get_saturation_with_flux(Flux)*Flux;

    // d axis
    double xd = get_Xd();
    double xdp = get_Xdp();
    double xpp = get_Xpp();
    double xl = get_Xl();
    // sp*(Xd"-Xl)/(Xd'-Xl)+spp*(Xd'-Xd")/(Xd'-Xl)=Fd
    // sp-spp-Id*(Xd'-Xl)=0.0
    // or
    // sp+spp*(Xd'-Xd")/(Xd"-Xl)=Fd*(Xd'-Xl)/(Xd"-Xl)
    // -sp+spp=-Id*(Xd'-Xl)
    // or
    //spp*(Xd'-Xd")/(Xd"-Xl)+spp=Fd*(Xd'-Xl)/(Xd"-Xl)-Id*(Xd'-Xl)
    // or
    //spp*(Xd'-Xl)/(Xd"-Xl)=Fd*(Xd'-Xl)/(Xd"-Xl)-Id*(Xd'-Xl)
    // or
    //spp/(Xd"-Xl)=Fd/(Xd"-Xl)-Id
    // or
    //spp=Fd-Id*(Xd"-Xl)

    //spp = Flux_dq.real()-Idq.real()*(xpp-xl);
    //sp  = spp+Idq.real()*(xdp-xl);

    // A*sp + B*spp = fluxd
    // sp - spp = id*(xdp-xl)
    // sp = spp + id*(xdp-xl)
    // (A+B)*spp + A*id*(xdp-xl) = fluxd
    double A = (xpp-xl)/(xdp-xl), B = (xdp-xpp)/(xdp-xl);
    double sdp, sdpp; // state of transient, and subtransient
    sdpp = (Flux_dq.real()-A*Idq.real()*(xdp-xl))/(A+B);
    sdp = sdpp + Idq.real()*(xdp-xl);

    transient_block_d_axis->set_output(sdp);
    transient_block_d_axis->initialize();

    subtransient_block_d_axis->set_output(sdpp);
    subtransient_block_d_axis->initialize();

    // excitation voltage
    double efd0 = sdp+Idq.real()*(xd-xdp)+Flux_dq.real()*delta_XadIfd/Flux;
    set_initial_excitation_voltage_in_pu(efd0);
    if(efd0>5.0)
    {
        osstream<<"Warning. Initial EFD is "<<efd0<<" for "<<get_model_name()<<" model of "<<get_compound_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
    }

    // q axis
    double xqp = get_Xqp();
    /*double xqp = get_Xqp();
    // sp*(Xq"-Xl)/(Xq'-Xl)+spp*(Xq'-Xq")/(Xq'-Xl)=-Fq
    // sp-spp+Iq*(Xq'-Xl)=0.0
    // or
    // sp+spp*(Xq'-Xq")/(Xq"-Xl)=-Fq*(Xq'-Xl)/(Xq"-Xl)
    // -sp+spp=Iq*(Xq'-Xl)
    // or
    // spp*(Xq'-Xq")/(Xq"-Xl)+spp=-Fq*(Xq'-Xl)/(Xq"-Xl)+Iq*(Xq'-Xl)
    // or
    // spp*(Xq'-Xl)/(Xq"-Xl)=-Fq*(Xq'-Xl)/(Xq"-Xl)+Iq*(Xq'-Xl)
    // or
    // spp=-Fq+Iq*(Xq"-Xl)
    spp = -Flux_dq.imag()+Idq.imag()*(xpp-xl);
    sp  = spp-Idq.imag()*(xqp-xl);
    transient_block_q_axis->set_output(sp);
    subtransient_block_q_axis->set_output(spp);

    sp = Idq.imag()*(xq-xqp)+Flux_dq.imag()*delta_XadIfd*(xq-xl)/(xd-xl)/Flux;
    spp = sp+Idq.imag()*(xqp-xl);*/

    A = (xpp-xl)/(xqp-xl); B = (xqp-xpp)/(xqp-xl);
    // sp*A+spp*B = -fluxq
    //sp - spp + iq*(xqp-xl) = 0
    // sp = spp - iq*(xqp-xl)
    // (A+B)*spp -A*iq*(xqp-xl) = -fluxq
    double sqp, sqpp; // state of transient, and subtransient
    sqpp = (-Flux_dq.imag()+A*Idq.imag()*(xqp-xl))/(A+B);
    sqp = sqpp - Idq.imag()*(xqp-xl);

    transient_block_q_axis->set_output(sqp);
    transient_block_q_axis->initialize();

    subtransient_block_q_axis->set_output(sqpp);
    subtransient_block_q_axis->initialize();

    //cout<<"init Efd = "<<efd0<<", Flux_dq = "<<Flux_dq<<", Edq="<<Edq<<", Ed0'="<<sdp<<", Ed0''="<<sdpp<<", Eq0'="<<sqp<<", Eq0''="<<sqpp<<endl;
    //cout<<"init Flux="<<abs(Flux)<<", E''="<<steps_fast_complex_abs(Edq)<<", "<<steps_fast_complex_abs(Exy)<<endl;

    // mechanical power
    double I = steps_fast_complex_abs(Ixy);
    double pmech0 = S.real()+I*I*get_Rs();
    set_initial_mechanical_power_in_pu_based_on_mbase(pmech0);

    set_flag_model_initialized_as_true();
}

void GENROU::initialize_rotor_angle()
{
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    GENERATOR* generator = get_generator_pointer();
    double one_over_mbase = get_one_over_mbase_in_one_over_MVA();

    complex<double> Zsource(get_Rs(), get_Xpp());

    double P = generator->get_p_generation_in_MW();
    double Q = generator->get_q_generation_in_MVar();
    complex<double> S(P*one_over_mbase,Q*one_over_mbase);

    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = conj(S/Vxy);
    complex<double> Exy = Vxy+Ixy*Zsource;
    double Ex = Exy.real(), Ey = Exy.imag();
    double Ix = Ixy.real(), Iy = Ixy.imag();

    double rs = get_Rs();
    double xd = get_Xd();
    double xq = get_Xq();
    double xqp = get_Xqp();
    double xqpp = get_Xqpp();
    double xl = get_Xl();

    double Flux = steps_fast_complex_abs(Exy);
    double delta_XadIfd = get_saturation_with_flux(Flux)*Flux;

    double A = (xq-xqp)*(xqpp-xl)/(xqp-xl)+(xq-xl)*(xqp-xqpp)/(xqp-xl);
    double B = (xq-xl)/(xd-xl)*delta_XadIfd/Flux;

    double C = -Ex*(1.0+B)+A*Iy, D = -A*Ix-(1.0+B)*Ey;

    complex<double> Zq(rs, xq);
    complex<double> EQ = Vxy+Ixy*Zq;

    double rotor_angle_EQ = steps_fast_complex_arg(EQ/Vxy) + steps_fast_complex_arg(Vxy);
    double rotor_angle = 0.0;
    if(C != 0.0)
        rotor_angle = steps_atan(D/C);
    else
        rotor_angle = HALF_PI;

    while(true)
    {
        if(fabs(rotor_angle-rotor_angle_EQ)>HALF_PI)
        {
            if(rotor_angle>rotor_angle_EQ)
                rotor_angle -= PI;
            else
                rotor_angle += PI;
        }
        else
            break;
    }

    rotor_angle_block->set_output(rotor_angle);
    rotor_angle_block->initialize();
}

void GENROU::run(DYNAMIC_MODE mode)
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();
    INTEGRAL_BLOCK* transient_block_d_axis = get_d_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_d_axis = get_d_axis_subtransient_block();
    INTEGRAL_BLOCK* transient_block_q_axis = get_q_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_q_axis = get_q_axis_subtransient_block();

    double fbase = get_bus_base_frequency_in_Hz();

    double xd = get_Xd();
    double xdp = get_Xdp();
    double xpp = get_Xpp();
    double xq = get_Xq();
    double xqp = get_Xqp();
    double xl = get_Xl();

    double one_over_xdp_minus_xl = 1.0/(xdp-xl);
    double one_over_xqp_minus_xl = 1.0/(xqp-xl);

    double fluxd = (transient_block_d_axis->get_output()*(xpp-xl) + subtransient_block_d_axis->get_output()*(xdp-xpp))*one_over_xdp_minus_xl;
    double fluxq = (transient_block_q_axis->get_output()*(xpp-xl) + subtransient_block_q_axis->get_output()*(xqp-xpp))*one_over_xqp_minus_xl;
    fluxq = -fluxq;
    complex<double> Flux_dq(fluxd, fluxq);
    double Flux = steps_fast_complex_abs(Flux_dq);
    double saturation = get_saturation_with_flux(Flux);

    complex<double> Idq = get_terminal_complex_current_in_pu_in_dq_axis_based_on_mbase();

    double input;
    // d-axis
    input = transient_block_d_axis->get_output()-subtransient_block_d_axis->get_output()-Idq.real()*(xdp-xl);
    input = get_excitation_voltage_in_pu()-transient_block_d_axis->get_output()-(xd-xdp)*(Idq.real()+input*(xdp-xpp)*one_over_xdp_minus_xl*one_over_xdp_minus_xl)-
            Flux_dq.real()*saturation;
    transient_block_d_axis->set_input(input);
    transient_block_d_axis->run(mode);

    input = transient_block_d_axis->get_output()-subtransient_block_d_axis->get_output()-Idq.real()*(xdp-xl);
    subtransient_block_d_axis->set_input(input);
    subtransient_block_d_axis->run(mode);

    // q-axis
    input = transient_block_q_axis->get_output()-subtransient_block_q_axis->get_output()+Idq.imag()*(xqp-xl);
    input = -transient_block_q_axis->get_output()+(xq-xqp)*(Idq.imag()-input*(xqp-xpp)*one_over_xqp_minus_xl*one_over_xqp_minus_xl)+
            Flux_dq.imag()*(xq-xl)/(xd-xl)*saturation;
    transient_block_q_axis->set_input(input);
    transient_block_q_axis->run(mode);

    input = transient_block_q_axis->get_output()-subtransient_block_q_axis->get_output()+Idq.imag()*(xqp-xl);
    subtransient_block_q_axis->set_input(input);
    subtransient_block_q_axis->run(mode);

    double damping = get_D();

    double pmech = get_mechanical_power_in_pu_based_on_mbase();
    double speed = get_rotor_speed_deviation_in_pu();
    double one_over_omega = 1.0/get_rotor_speed_in_pu();
    double tmech = pmech*one_over_omega;

    double telec = get_air_gap_torque_in_pu_based_on_mbase();

    double delta_torque = tmech-telec-damping*speed*one_over_omega;

    rotor_speed_block->set_input(delta_torque);
    rotor_speed_block->run(mode);

    speed = get_rotor_speed_deviation_in_pu();

    speed = DOUBLE_PI*fbase*speed;
    rotor_angle_block->set_input(speed);
    rotor_angle_block->run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}


complex<double> GENROU::get_source_Norton_equivalent_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Z(get_Rs(), get_Xpp());
    double mbase = get_mbase_in_MVA();
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return (Exy/Z)*(mbase*one_over_sbase);
}

complex<double> GENROU::get_terminal_complex_current_in_pu_in_dq_axis_based_on_mbase()
{
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    double angle = get_rotor_angle_in_rad();

    return xy2dq_with_angle_in_rad(Ixy, angle);
}


complex<double> GENROU::get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();

    complex<double> Zsource(get_Rs(), get_Xpp());

    complex<double> Ixy = (Exy-Vxy)/Zsource;

    //cout<<"Exy = "<<Exy<<", Vxy = "<<Vxy<<", Z = "<<Zsource<<", Ixy = "<<Ixy<<endl;

    return Ixy;
}

complex<double> GENROU::get_terminal_complex_current_in_pu_in_xy_axis_based_on_sbase()
{
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return Ixy*(mbase*one_over_sbase);
}

double GENROU::get_terminal_current_in_pu_based_on_mbase()
{
    return steps_fast_complex_abs(get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase());
}

double GENROU::get_terminal_current_in_pu_based_on_sbase()
{
    double I = get_terminal_current_in_pu_based_on_mbase();
    double mbase = get_mbase_in_MVA();

    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    return I*(mbase*one_over_sbase);
}


void GENROU::check()
{
    ostringstream osstream;
    STEPS& toolkit = get_toolkit();
    double xd = get_Xd();
    double xq = get_Xq();
    double xdp = get_Xdp();
    double xqp = get_Xqp();
    double xpp = get_Xpp();
    double xl = get_Xl();
    double td0p = get_Td0p_in_s();
    double td0pp = get_Td0pp_in_s();
    double tq0p = get_Tq0p_in_s();
    double tq0pp = get_Tq0pp_in_s();
    osstream<<"Error is detected at "<<get_model_name()<<" model of "<<get_compound_device_name()<<".\n";
    bool error_found = false;
    if(xdp==xpp)
    {
        osstream<<"Xd'=X\" was detected: Xd'="<<xdp<<", X\"="<<xpp<<"\n";
        error_found = true;
    }
    if(xqp==xpp)
    {
        osstream<<"Xq'=X\" was detected: Xq'="<<xqp<<", X\"="<<xpp<<"\n";
        error_found = true;
    }
    if(xdp==xl)
    {
        osstream<<"Xd'=Xl was detected: Xd'="<<xdp<<", Xl="<<xl<<"\n";
        error_found = true;
    }
    if(xqp==xl)
    {
        osstream<<"Xq'=Xl was detected: Xq'="<<xqp<<", Xl="<<xl<<"\n";
        error_found = true;
    }
    if(xdp>=xd)
    {
        osstream<<"Xd'>=Xd was detected: Xd'="<<xdp<<", Xd="<<xd<<"\n";
        error_found = true;
    }
    if(xqp>=xq)
    {
        osstream<<"Xq'>=Xq was detected: Xq'="<<xqp<<", Xq="<<xq<<"\n";
        error_found = true;
    }
    if(xpp>=xdp)
    {
        osstream<<"X\">=Xdp was detected: X\"="<<xpp<<", Xd'="<<xdp<<"\n";
        error_found = true;
    }
    if(xpp>=xqp)
    {
        osstream<<"X\">=Xqp was detected: X\"="<<xpp<<", Xq'="<<xqp<<"\n";
        error_found = true;
    }
    if(xl>=xpp)
    {
        osstream<<"Xl>=X\" was detected: Xl="<<xl<<", X\"="<<xpp<<"\n";
        error_found = true;
    }
    if(td0p<=td0pp)
    {
        osstream<<"Td0'<=Td0\" was detected: Td0'="<<td0p<<", Td0\"="<<td0pp<<"\n";
        error_found = true;
    }
    if(tq0p<=tq0pp)
    {
        osstream<<"Tq0'<=Tq0\" was detected: Tq0'="<<tq0p<<", Tq0\"="<<tq0pp<<"\n";
        error_found = true;
    }
    /*
    if(xd<xdp*2)
    {
        osstream<<"Xd<2Xd\' was detected: Xd="<<xd<<", Xd\'="<<xdp<<"\n";
        error_found = true;
    }
    if(xdp<xpp*2)
    {
        osstream<<"Xd\'<2Xd\" was detected: Xd\'="<<xdp<<", X\"="<<xpp<<"\n";
        error_found = true;
    }
    if(xq<xqp*2)
    {
        osstream<<"Xq<2Xq\' was detected: Xq="<<xq<<", Xq\'="<<xqp<<"\n";
        error_found = true;
    }
    if(xqp<xpp*2)
    {
        osstream<<"Xq\'<2Xd\" was detected: Xq\'="<<xqp<<", X\"="<<xpp<<"\n";
        error_found = true;
    }
    if(xpp<xl*1.5)
    {
        osstream<<"X\"<1.5Xl was detected: X\"="<<xpp<<", Xl="<<xl<<"\n";
        error_found = true;
    }
    */
    if(error_found)
        toolkit.show_information_with_leading_time_stamp(osstream);
}

void GENROU::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void GENROU::save()
{
    ;
}

string GENROU::get_standard_psse_string(bool export_internal_bus_number) const
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
            <<setw(8)<<setprecision(6)<<get_Td0p_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Td0pp_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Tq0p_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_Tq0pp_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_H_in_s()<<", "
            <<setw(8)<<setprecision(6)<<get_D()<<", "
            <<setw(8)<<setprecision(6)<<get_Xd()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Xq()<<", "
            <<setw(8)<<setprecision(6)<<get_Xdp()<<", "
            <<setw(8)<<setprecision(6)<<get_Xqp()<<", "
            <<setw(8)<<setprecision(6)<<get_Xpp()<<", "
            <<setw(8)<<setprecision(6)<<get_Xl()<<", "
            <<setw(8)<<setprecision(6)<<get_saturation_at_1()<<", "
            <<setw(8)<<setprecision(6)<<get_saturation_at_1p2()<<" /";

    return osstream.str();
}

void GENROU::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("TD0'", i); i++;
    add_model_data_name_and_index_pair("TD0\"", i); i++;
    add_model_data_name_and_index_pair("TQ0'", i); i++;
    add_model_data_name_and_index_pair("TQ0\"", i); i++;
    add_model_data_name_and_index_pair("H", i); i++;
    add_model_data_name_and_index_pair("D", i); i++;
    add_model_data_name_and_index_pair("XD", i); i++;
    add_model_data_name_and_index_pair("XQ", i); i++;
    add_model_data_name_and_index_pair("XD'", i); i++;
    add_model_data_name_and_index_pair("XQ'", i); i++;
    add_model_data_name_and_index_pair("X\"", i); i++;
    add_model_data_name_and_index_pair("XL", i); i++;
    add_model_data_name_and_index_pair("S1", i); i++;
    add_model_data_name_and_index_pair("S1.2", i);
}

double GENROU::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        if(par_name == "TD0'") return get_Td0p_in_s();
        if(par_name == "TD0\"") return get_Td0pp_in_s();
        if(par_name == "TQ0'") return get_Tq0p_in_s();
        if(par_name == "TQ0\"") return get_Tq0pp_in_s();
        if(par_name =="H") return get_H_in_s();
        if(par_name =="D") return get_D();
        if(par_name == "XD") return get_Xd();
        if(par_name == "XQ") return get_Xq();
        if(par_name == "XD'") return get_Xdp();
        if(par_name == "XQ'") return get_Xqp();
        if(par_name == "X\"") return get_Xpp();
        if(par_name == "XL") return get_Xl();
        if(par_name == "S1") return get_saturation_at_1();
        if(par_name == "S1.2")  return get_saturation_at_1p2();
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void GENROU::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(is_model_data_exist(par_name))
    {
        if(par_name == "TD0'") return set_Td0p_in_s(value);
        if(par_name == "TD0\"") return set_Td0pp_in_s(value);
        if(par_name == "TQ0'") return set_Tq0p_in_s(value);
        if(par_name == "TQ0\"") return set_Tq0pp_in_s(value);
        if(par_name == "H") return set_H_in_s(value);
        if(par_name == "D") return set_D(value);
        if(par_name == "XD") return set_Xd(value);
        if(par_name == "XQ") return set_Xq(value);
        if(par_name == "XD'") return set_Xdp(value);
        if(par_name == "XQ'") return set_Xqp(value);
        if(par_name == "X\"") return set_Xpp(value);
        if(par_name == "XL") return set_Xl(value);
        if(par_name == "S1") return set_saturation_at_1(value);
        if(par_name == "S1.2") return set_saturation_at_1p2(value);
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double GENROU::get_minimum_nonzero_time_constant_in_s()
{
    double mint = INFINITE_THRESHOLD;
    if(get_H_in_s()!=0.0 and mint>get_H_in_s())
        mint = get_H_in_s();
    if(get_Td0p_in_s()!=0.0 and mint>get_Td0p_in_s())
        mint = get_Td0p_in_s();
    if(get_Tq0p_in_s()!=0.0 and mint>get_Tq0p_in_s())
        mint = get_Tq0p_in_s();
    if(get_Td0pp_in_s()!=0.0 and mint>get_Td0pp_in_s())
        mint = get_Td0pp_in_s();
    if(get_Tq0pp_in_s()!=0.0 and mint>get_Tq0pp_in_s())
        mint = get_Tq0pp_in_s();
    return mint;
}

void GENROU::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_internal_variable_name_and_index_pair("STATE@ROTOR ANGLE BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@ROTOR SPEED BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@D-AXIS TRANSIENT BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@Q-AXIS TRANSIENT BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@D-AXIS SUBTRANSIENT BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("STATE@Q-AXIS SUBTRANSIENT BLOCK", i); i++;
    add_model_internal_variable_name_and_index_pair("SATURATION", i); i++;
}

double GENROU::get_model_internal_variable_with_name(string var_name)
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();
    INTEGRAL_BLOCK* transient_block_d_axis = get_d_axis_transient_block();
    INTEGRAL_BLOCK* transient_block_q_axis = get_q_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_d_axis = get_d_axis_subtransient_block();
    INTEGRAL_BLOCK* subtransient_block_q_axis = get_q_axis_subtransient_block();

    var_name = string2upper(var_name);
    if(var_name == "STATE@ROTOR ANGLE BLOCK") return rotor_angle_block->get_state();
    if(var_name == "STATE@ROTOR SPEED BLOCK") return rotor_speed_block->get_state();
    if(var_name == "STATE@D-AXIS TRANSIENT BLOCK") return transient_block_d_axis->get_state();
    if(var_name == "STATE@Q-AXIS TRANSIENT BLOCK") return transient_block_q_axis->get_state();
    if(var_name == "STATE@D-AXIS SUBTRANSIENT BLOCK") return subtransient_block_d_axis->get_state();
    if(var_name == "STATE@Q-AXIS SUBTRANSIENT BLOCK") return subtransient_block_q_axis->get_state();
    if(var_name == "SATURATION")
    {
        INTEGRAL_BLOCK* transient_block_d_axis = get_d_axis_transient_block();
        INTEGRAL_BLOCK* subtransient_block_d_axis = get_d_axis_subtransient_block();
        INTEGRAL_BLOCK* transient_block_q_axis = get_q_axis_transient_block();
        INTEGRAL_BLOCK* subtransient_block_q_axis = get_q_axis_subtransient_block();

        double xdp = get_Xdp();
        double xpp = get_Xpp();
        double xqp = get_Xqp();
        double xl = get_Xl();

        double fluxd = transient_block_d_axis->get_output()*(xpp-xl)/(xdp-xl) + subtransient_block_d_axis->get_output()*(xdp-xpp)/(xdp-xl);
        double fluxq = transient_block_q_axis->get_output()*(xpp-xl)/(xqp-xl) + subtransient_block_q_axis->get_output()*(xqp-xpp)/(xqp-xl);
        fluxq = -fluxq;
        complex<double> Flux_dq(fluxd, fluxq);
        double Flux = steps_fast_complex_abs(Flux_dq);

        return get_saturation_with_flux(Flux);
    }

    return 0.0;
}

double GENROU::get_air_gap_power_in_pu_based_on_mbase()
{
    complex<double> Exy = get_internal_voltage_in_pu_in_xy_axis();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Exy*conj(Ixy);

    return S.real();
}

double GENROU::get_air_gap_power_in_MW()
{
    return get_air_gap_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double GENROU::get_air_gap_torque_in_pu_based_on_mbase()
{
    // since omega is ignored when converting from flux to voltage
    // the air gap torch is equal to air gap power
    // double omega = get_rotor_speed_in_pu();
    return get_air_gap_power_in_pu_based_on_mbase();
}

double GENROU::get_accelerating_power_in_pu_based_on_mbase()
{
    return get_mechanical_power_in_pu_based_on_mbase()-get_air_gap_power_in_pu_based_on_mbase();
}

double GENROU::get_accelerating_power_in_MW()
{
    return get_accelerating_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

complex<double> GENROU::get_terminal_complex_power_in_pu_based_on_mbase()
{
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Ixy = get_terminal_complex_current_in_pu_in_xy_axis_based_on_mbase();

    complex<double> S = Vxy*conj(Ixy);
    return S;
}

complex<double> GENROU::get_terminal_complex_power_in_MVA()
{
    return get_terminal_complex_power_in_pu_based_on_mbase()*get_mbase_in_MVA();
}

double GENROU::get_terminal_active_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().real();
}

double GENROU::get_terminal_active_power_in_MW()
{
    return get_terminal_complex_power_in_MVA().real();
}

double GENROU::get_terminal_reactive_power_in_pu_based_on_mbase()
{
    return get_terminal_complex_power_in_pu_based_on_mbase().imag();
}

double GENROU::get_terminal_reactive_power_in_MVar()
{
    return get_terminal_complex_power_in_MVA().imag();
}

double GENROU::get_field_current_in_pu_based_on_mbase()
{
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();
    INTEGRAL_BLOCK* transient_block_d_axis = get_d_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_d_axis = get_d_axis_subtransient_block();
    INTEGRAL_BLOCK* transient_block_q_axis = get_q_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_q_axis = get_q_axis_subtransient_block();

    double angle = rotor_angle_block->get_output();

    double xd = get_Xd();
    double xdp = get_Xdp();
    double xpp = get_Xpp();
    double xqp = get_Xqp();
    double xl = get_Xl();

    double fluxd = transient_block_d_axis->get_output()*(xpp-xl)/(xdp-xl) + subtransient_block_d_axis->get_output()*(xdp-xpp)/(xdp-xl);
    double fluxq = transient_block_q_axis->get_output()*(xpp-xl)/(xqp-xl) + subtransient_block_q_axis->get_output()*(xqp-xpp)/(xqp-xl);
    fluxq = -fluxq;
    complex<double> Flux_dq(fluxd, fluxq);
    double Flux = steps_fast_complex_abs(Flux_dq);

    complex<double> Edq = get_internal_voltage_in_pu_in_dq_axis();
    complex<double> Vxy = get_terminal_complex_voltage_in_pu();
    complex<double> Vdq = xy2dq_with_angle_in_rad(Vxy, angle);
    complex<double> Zsource(get_Rs(), get_Xpp());
    complex<double> Idq = (Edq-Vdq)/Zsource;

    double delta_XadIfd = get_saturation_with_flux(Flux)*Flux;

    double xadifd;
    // d-axis
    // input to subtransient block
    xadifd = transient_block_d_axis->get_output()-subtransient_block_d_axis->get_output()-Idq.real()*(xdp-xl);

    xadifd = transient_block_d_axis->get_output()+(xd-xdp)*(Idq.real()+xadifd*(xdp-xpp)/((xdp-xl)*(xdp-xl)))+
            Flux_dq.real()*delta_XadIfd/Flux;

    return xadifd/(xd-xl);
}

double GENROU::get_rotor_angle_in_rad()
{
    INTEGRAL_BLOCK* rotor_angle_block = get_rotor_angle_block();

    return rotor_angle_block->get_output();
}

double GENROU::get_rotor_angle_in_deg()
{
    return rad2deg(get_rotor_angle_in_rad());
}

double GENROU::get_rotor_speed_deviation_in_pu()
{
    INTEGRAL_BLOCK* rotor_speed_block = get_rotor_speed_block();

    return rotor_speed_block->get_output();
}

double GENROU::get_rotor_speed_in_pu()
{
    return 1.0+get_rotor_speed_deviation_in_pu();
}

complex<double> GENROU::get_internal_voltage_in_pu_in_dq_axis()
{
    INTEGRAL_BLOCK* transient_block_d_axis = get_d_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_d_axis = get_d_axis_subtransient_block();
    INTEGRAL_BLOCK* transient_block_q_axis = get_q_axis_transient_block();
    INTEGRAL_BLOCK* subtransient_block_q_axis = get_q_axis_subtransient_block();

    double flux_d = transient_block_d_axis->get_output()*(get_Xpp()-get_Xl())+subtransient_block_d_axis->get_output()*(get_Xdp()-get_Xpp());
    flux_d /= (get_Xdp()-get_Xl());

    double flux_q = transient_block_q_axis->get_output()*(get_Xpp()-get_Xl())+subtransient_block_q_axis->get_output()*(get_Xqp()-get_Xpp());
    flux_q /= (get_Xqp()-get_Xl());

    return complex<double>(flux_q, flux_d);

    /*
    flux_q = -flux_q;
    complex<double> Flux_dq(flux_d, flux_q);
    complex<double> Edq = Flux_dq/complex<double>(0.0, -1.0);  // omega is ignored when converting from flux to voltage
    //cout<<"Flux_dq = "<<Flux_dq<<", Edq = "<<Edq<<", Eq0'="<<transient_block_q_axis->get_output()<<", Eq0''="<<subtransient_block_q_axis->get_output()<<endl;

    return Edq;*/
}


complex<double> GENROU::get_internal_voltage_in_pu_in_xy_axis()
{
    complex<double> Edq = get_internal_voltage_in_pu_in_dq_axis();

    double angle = get_rotor_angle_in_rad();
    complex<double> Exy = dq2xy_with_angle_in_rad(Edq, angle);

    //cout<<"Edq = "<<Edq<<", angle = "<<angle<<"Exy = "<<Exy<<endl;

    return Exy;
}

string GENROU::get_dynamic_data_in_psse_format() const
{
    return "";
}

string GENROU::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string GENROU::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void GENROU::linearize()
{
    STEPS_SPARSE_MATRIX* gen_matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GEN", gen_matrix);
    // do linearization
    STEPS_SPARSE_MATRIX* gen_avr_matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GEN-AVR", gen_avr_matrix);
    // do linearization
    STEPS_SPARSE_MATRIX* gen_gov_matrix = new STEPS_SPARSE_MATRIX;
    set_linearized_matrix("GEN-GOV", gen_gov_matrix);
    // do linearization
}
