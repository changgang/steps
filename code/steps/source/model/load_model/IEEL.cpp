#include "header/model/load_model/IEEL.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <cstdio>
IEEL::IEEL(STEPS& toolkit) : LOAD_MODEL(toolkit),
                             voltage_sensor(toolkit),
                             angle_sensor(toolkit)
{
    clear();
}

IEEL::~IEEL()
{
}

void IEEL::clear()
{
    set_voltage_source_flag(false);

    set_model_float_parameter_count(14);
}

void IEEL::copy_from_const_model(const IEEL& model)
{

    STEPS& toolkit = model.get_toolkit();
    set_toolkit(toolkit);
    voltage_sensor.set_toolkit(toolkit);
    angle_sensor.set_toolkit(toolkit);

    clear();

    this->set_subsystem_type(model.get_subsystem_type());

    this->set_P_alpha_1(model.get_P_alpha_1());
    this->set_P_alpha_2(model.get_P_alpha_2());
    this->set_P_alpha_3(model.get_P_alpha_3());
    this->set_P_n_power_1(model.get_P_n_power_1());
    this->set_P_n_power_2(model.get_P_n_power_2());
    this->set_P_n_power_3(model.get_P_n_power_3());
    this->set_P_Kf(model.get_P_Kf());

    this->set_Q_alpha_1(model.get_Q_alpha_1());
    this->set_Q_alpha_2(model.get_Q_alpha_2());
    this->set_Q_alpha_3(model.get_Q_alpha_3());
    this->set_Q_n_power_1(model.get_Q_n_power_1());
    this->set_Q_n_power_2(model.get_Q_n_power_2());
    this->set_Q_n_power_3(model.get_Q_n_power_3());
    this->set_Q_Kf(model.get_Q_Kf());

}

IEEL::IEEL(const IEEL& model) : LOAD_MODEL(model.get_toolkit()),
                                voltage_sensor(model.get_toolkit()),
                                angle_sensor(model.get_toolkit())
{
    copy_from_const_model(model);
}

IEEL& IEEL::operator=(const IEEL& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string IEEL::get_model_name() const
{
    return "IEEL";
}

void IEEL::set_P_alpha_1(double alpha)
{
    P_alpha_1 = alpha;
}

void IEEL::set_P_alpha_2(double alpha)
{
    P_alpha_2 = alpha;
}

void IEEL::set_P_alpha_3(double alpha)
{
    P_alpha_3 = alpha;
}

void IEEL::set_P_n_power_1(double n)
{
    P_n_power_1 = n;
}

void IEEL::set_P_n_power_2(double n)
{
    P_n_power_2 = n;
}

void IEEL::set_P_n_power_3(double n)
{
    P_n_power_3 = n;
}

void IEEL::set_P_Kf(double kf)
{
    P_Kf = kf;
}

void IEEL::set_Q_alpha_1(double alpha)
{
    Q_alpha_1 = alpha;
}

void IEEL::set_Q_alpha_2(double alpha)
{
    Q_alpha_2 = alpha;
}

void IEEL::set_Q_alpha_3(double alpha)
{
    Q_alpha_3 = alpha;
}

void IEEL::set_Q_n_power_1(double n)
{
    Q_n_power_1 = n;
}

void IEEL::set_Q_n_power_2(double n)
{
    Q_n_power_2 = n;
}

void IEEL::set_Q_n_power_3(double n)
{
    Q_n_power_3 = n;
}

void IEEL::set_Q_Kf(double kf)
{
    Q_Kf = kf;
}


double IEEL::get_P_alpha_1() const
{
    return P_alpha_1;
}

double IEEL::get_P_alpha_2() const
{
    return P_alpha_2;
}

double IEEL::get_P_alpha_3() const
{
    return P_alpha_3;
}

double IEEL::get_P_n_power_1() const
{
    return P_n_power_1;
}

double IEEL::get_P_n_power_2() const
{
    return P_n_power_2;
}

double IEEL::get_P_n_power_3() const
{
    return P_n_power_3;
}

double IEEL::get_P_Kf() const
{
    return P_Kf;
}


double IEEL::get_Q_alpha_1() const
{
    return Q_alpha_1;
}

double IEEL::get_Q_alpha_2() const
{
    return Q_alpha_2;
}

double IEEL::get_Q_alpha_3() const
{
    return Q_alpha_3;
}

double IEEL::get_Q_n_power_1() const
{
    return Q_n_power_1;
}

double IEEL::get_Q_n_power_2() const
{
    return Q_n_power_2;
}

double IEEL::get_Q_n_power_3() const
{
    return Q_n_power_3;
}

double IEEL::get_Q_Kf() const
{
    return Q_Kf;
}


bool IEEL::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=17)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name=="IEELAL" or model_name=="IEELBL" or model_name=="IEELAR" or model_name=="IEELZN" or model_name=="IEELOW")
        {
            double ap1, ap2, ap3, aq1, aq2, aq3, kfp, kfq, np1, np2, np3, nq1, nq2, nq3;

            unsigned int i=3;
            ap1 = get_double_data(data[i],"0.0"); i++;
            ap2 = get_double_data(data[i],"0.0"); i++;
            ap3 = get_double_data(data[i],"0.0"); i++;
            aq1 = get_double_data(data[i],"0.0"); i++;
            aq2 = get_double_data(data[i],"0.0"); i++;
            aq3 = get_double_data(data[i],"0.0"); i++;
            kfp = get_double_data(data[i],"0.0"); i++;
            kfq = get_double_data(data[i],"0.0"); i++;
            np1 = get_double_data(data[i],"0.0"); i++;
            np2 = get_double_data(data[i],"0.0"); i++;
            np3 = get_double_data(data[i],"0.0"); i++;
            nq1 = get_double_data(data[i],"0.0"); i++;
            nq2 = get_double_data(data[i],"0.0"); i++;
            nq3 = get_double_data(data[i],"0.0"); i++;

            set_P_alpha_1(ap1);
            set_P_alpha_2(ap2);
            set_P_alpha_3(ap3);
            set_Q_alpha_1(aq1);
            set_Q_alpha_2(aq2);
            set_Q_alpha_3(aq3);
            set_P_n_power_1(np1);
            set_P_n_power_2(np2);
            set_P_n_power_3(np3);
            set_Q_n_power_1(nq1);
            set_Q_n_power_2(nq2);
            set_Q_n_power_3(nq3);
            set_P_Kf(kfp);
            set_Q_Kf(kfq);

            if(model_name=="IEELAL")
                set_subsystem_type(ALL_SYSTEM_TYPE);
            else
            {
                if(model_name=="IEELAR")
                    set_subsystem_type(AREA_SUBSYSTEM_TYPE);
                else
                {
                    if(model_name=="IEELZN")
                        set_subsystem_type(ZONE_SUBSYSTEM_TYPE);
                    else
                    {
                        //IEELBL
                        set_subsystem_type(BUS_SUBSYSTEM_TYPE);
                    }
                }
            }
            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool IEEL::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool IEEL::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

complex<double> IEEL::get_dynamic_source_admittance_in_pu_based_on_SBASE()
{
    return 0.0;
}

complex<double> IEEL::get_additional_admittance_in_pu_based_on_SBASE()
{
    return 0.0;
}

void IEEL::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit();
    double h = toolkit.get_dynamic_simulation_time_step_in_s();
    voltage_sensor.set_T_in_s(h*2);

    angle_sensor.set_bus_pointer(get_bus_pointer());
    if(h<1e-3)
        angle_sensor.set_Kp(500);
    else if(h<5e-3)
        angle_sensor.set_Kp(250);
    else if(h<10e-3)
        angle_sensor.set_Kp(100);
    else
        angle_sensor.set_Kp(50);
    angle_sensor.set_Ki(0.5);
}

void IEEL::initialize()
{
    setup_block_toolkit_and_parameters();

    synchronize_bus_voltage_and_frequency();
    swap_parameters();

    LOAD* load = get_load_pointer();

    complex<double> S = load->get_actual_total_load_in_MVA();
    double V = get_bus_positive_sequence_voltage_in_pu();

    voltage_sensor.set_output(V);
    voltage_sensor.initialize();

    angle_sensor.initialize();

    double P = S.real();
    double alpha1 = get_P_alpha_1();
    double alpha2 = get_P_alpha_2();
    double alpha3 = get_P_alpha_3();
    double n1 = get_P_n_power_1();
    double n2 = get_P_n_power_2();
    double n3 = get_P_n_power_3();

    /*P0 = P/(alpha1*load->get_load_scale_with_voltage(n1, V, LOAD_ELLIPTICAL_CV)+
            alpha2*load->get_load_scale_with_voltage(n2, V, LOAD_ELLIPTICAL_CV)+
            alpha3*load->get_load_scale_with_voltage(n3, V, LOAD_ELLIPTICAL_CV));*/
    P0 = P/(alpha1*load->get_load_scale_with_voltage(n1, V, LOAD_LINEAR_CV)+
            alpha2*load->get_load_scale_with_voltage(n2, V, LOAD_LINEAR_CV)+
            alpha3*load->get_load_scale_with_voltage(n3, V, LOAD_LINEAR_CV));

    double Q = S.imag();
    alpha1 = get_Q_alpha_1();
    alpha2 = get_Q_alpha_2();
    alpha3 = get_Q_alpha_3();
    n1 = get_Q_n_power_1();
    n2 = get_Q_n_power_2();
    n3 = get_Q_n_power_3();

    /*Q0 = Q/(alpha1*load->get_load_scale_with_voltage(n1, V, LOAD_ELLIPTICAL_CV)+
            alpha2*load->get_load_scale_with_voltage(n2, V, LOAD_ELLIPTICAL_CV)+
            alpha3*load->get_load_scale_with_voltage(n3, V, LOAD_ELLIPTICAL_CV));*/
    Q0 = Q/(alpha1*load->get_load_scale_with_voltage(n1, V, LOAD_LINEAR_CV)+
            alpha2*load->get_load_scale_with_voltage(n2, V, LOAD_LINEAR_CV)+
            alpha3*load->get_load_scale_with_voltage(n3, V, LOAD_LINEAR_CV));

    if(fabs(P_n_power_1-0)<FLOAT_EPSILON and fabs(P_n_power_2-1)<FLOAT_EPSILON and fabs(P_n_power_3-2)<FLOAT_EPSILON and
       fabs(Q_n_power_1-0)<FLOAT_EPSILON and fabs(Q_n_power_2-1)<FLOAT_EPSILON and fabs(Q_n_power_3-2)<FLOAT_EPSILON)
        is_PIZ = true;
    else
        is_PIZ = false;

    set_flag_model_initialized_as_true();
}

void IEEL::swap_parameters()
{
    if(P_n_power_1>P_n_power_2)
    {
        swap(P_n_power_1, P_n_power_2);
        swap(P_alpha_1, P_alpha_2);
    }
    if(P_n_power_1>P_n_power_3)
    {
        swap(P_n_power_1, P_n_power_3);
        swap(P_alpha_1, P_alpha_3);
    }
    if(P_n_power_2>P_n_power_3)
    {
        swap(P_n_power_2, P_n_power_3);
        swap(P_alpha_2, P_alpha_3);
    }
    if(Q_n_power_1>Q_n_power_2)
    {
        swap(Q_n_power_1, Q_n_power_2);
        swap(Q_alpha_1, Q_alpha_2);
    }
    if(Q_n_power_1>Q_n_power_3)
    {
        swap(Q_n_power_1, Q_n_power_3);
        swap(Q_alpha_1, Q_alpha_3);
    }
    if(Q_n_power_2>Q_n_power_3)
    {
        swap(Q_n_power_2, Q_n_power_3);
        swap(Q_alpha_2, Q_alpha_3);
    }
}

void IEEL::initialize_to_start()
{
    if(not is_model_initialized())
        initialize();
}

void IEEL::check_limits_in_initialization()
{
    return;
}

void IEEL::run(DYNAMIC_MODE mode)
{
    synchronize_bus_voltage_and_frequency();

    bool voltage_sensor_enabled = is_bus_voltage_sensor_enabled();
    bool voltage_sensor_initialized = is_bus_voltage_sensor_initialized();
    if(voltage_sensor_enabled and (not voltage_sensor_initialized))
    {
        double V = get_bus_positive_sequence_voltage_in_pu();
        voltage_sensor.set_output(V);
        voltage_sensor.initialize();
        set_bus_voltage_sensor_initialized_flag(true);
    }

    bool angle_sensor_enabled = is_bus_angle_sensor_enabled();
    bool angle_sensor_initialized = is_bus_angle_sensor_initialized();
    if(angle_sensor_enabled and (not angle_sensor_initialized))
    {
        angle_sensor.initialize();
        set_bus_angle_sensor_initialized_flag(true);
    }

    if(voltage_sensor_enabled)
    {
        double v = get_bus_positive_sequence_voltage_in_pu();
        voltage_sensor.set_input(v);
        voltage_sensor.run(mode);
    }

    if(angle_sensor_enabled)
        angle_sensor.run(mode);

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

complex<double> IEEL::get_load_power_in_MVA()
{
    if(is_PIZ)
        return get_load_power_in_MVA_as_PIZ_load();
    else
        return get_load_power_in_MVA_as_non_PIZ_load();
}

complex<double> IEEL::get_load_power_in_MVA_as_PIZ_load()
{
    double Vbus = get_bus_positive_sequence_voltage_in_pu();
    double Vsensor = get_bus_positive_sequence_voltage_in_pu_from_sensor();
    double V = is_bus_voltage_sensor_enabled() ? Vsensor : Vbus;
    double f = get_bus_frequency_deviation_in_pu();

    double voltage_scale_P = LOAD::get_constant_power_load_scale_with_voltage(V, LOAD_LINEAR_CV);
    double voltage_scale_I = LOAD::get_constant_current_load_scale_with_voltage(Vbus, LOAD_LINEAR_CV);
    double voltage_scale_Z = Vbus*Vbus;

    if(not is_bus_voltage_sensor_enabled())
    {
        double P_scale = P_alpha_1*voltage_scale_P + P_alpha_2*voltage_scale_I + P_alpha_3*voltage_scale_Z;
        double Q_scale = Q_alpha_1*voltage_scale_P + Q_alpha_2*voltage_scale_I + Q_alpha_3*voltage_scale_Z;

        double P = P0*P_scale*(1.0+P_Kf*f);
        double Q = Q0*Q_scale*(1.0+Q_Kf*f);

        return complex<double>(P,Q);
    }
    else
    {
        double angle_bus = get_bus_positive_sequence_angle_in_rad();
        double angle_sensor = get_bus_positive_sequence_angle_in_rad_from_sensor();
        double angle_shift = angle_bus - angle_sensor;
        complex<double> rotation(steps_cos(angle_shift), steps_sin(angle_shift));

        complex<double> Sp_target(P0*P_alpha_1*voltage_scale_P, Q0*Q_alpha_1*voltage_scale_P);
        complex<double> Sp_true = Sp_target*rotation*(Vbus/Vsensor);

        complex<double> Si_target(P0*P_alpha_2*voltage_scale_I, Q0*Q_alpha_2*voltage_scale_I);
        complex<double> Si_true = Si_target*rotation;

        complex<double> Sz_true(P0*P_alpha_3*voltage_scale_Z, Q0*Q_alpha_3*voltage_scale_Z);

        return Sp_true + Si_true + Sz_true;
    }
}

complex<double> IEEL::get_load_power_in_MVA_as_non_PIZ_load()
{
    double Vbus = get_bus_positive_sequence_voltage_in_pu();
    double Vsensor = get_bus_positive_sequence_voltage_in_pu_from_sensor();
    double V = is_bus_voltage_sensor_enabled() ? Vsensor : Vbus;
    double f = get_bus_frequency_deviation_in_pu();

    complex<double> S = 0;
    double voltage_scale1_P = LOAD::get_load_scale_with_voltage(P_n_power_1, V, LOAD_LINEAR_CV);
    double voltage_scale2_P = LOAD::get_load_scale_with_voltage(P_n_power_2, V, LOAD_LINEAR_CV);
    double voltage_scale3_P = LOAD::get_load_scale_with_voltage(P_n_power_3, V, LOAD_LINEAR_CV);

    if(not is_bus_voltage_sensor_enabled())
    {
        double scale = P_alpha_1*voltage_scale1_P + P_alpha_2*voltage_scale2_P + P_alpha_3*voltage_scale3_P;

        double P = P0*scale*(1.0+P_Kf*f);

        if(fabs(P_n_power_1-Q_n_power_1)<FLOAT_EPSILON and fabs(P_n_power_2-Q_n_power_2)<FLOAT_EPSILON and fabs(P_n_power_3-Q_n_power_3)<FLOAT_EPSILON)
        {
            scale = Q_alpha_1*voltage_scale1_P + Q_alpha_2*voltage_scale2_P + Q_alpha_3*voltage_scale3_P;
        }
        else
        {
            double voltage_scale1_Q = LOAD::get_load_scale_with_voltage(Q_n_power_1, V, LOAD_LINEAR_CV);
            double voltage_scale2_Q = LOAD::get_load_scale_with_voltage(Q_n_power_2, V, LOAD_LINEAR_CV);
            double voltage_scale3_Q = LOAD::get_load_scale_with_voltage(Q_n_power_3, V, LOAD_LINEAR_CV);

            scale = Q_alpha_1*voltage_scale1_Q + Q_alpha_2*voltage_scale2_Q + Q_alpha_3*voltage_scale3_Q;
        }
        double Q = Q0*scale*(1.0+Q_Kf*f);

        S = complex<double>(P,Q);
    }
    else
    {
        if(fabs(P_n_power_1-Q_n_power_1)<FLOAT_EPSILON and fabs(P_n_power_2-Q_n_power_2)<FLOAT_EPSILON and fabs(P_n_power_3-Q_n_power_3)<FLOAT_EPSILON)
        {
            double angle_bus = get_bus_positive_sequence_angle_in_rad();
            double angle_sensor = get_bus_positive_sequence_angle_in_rad_from_sensor();
            double angle_shift = angle_bus - angle_sensor;
            complex<double> rotation(steps_cos(angle_shift), steps_sin(angle_shift));

            complex<double> S_target = 0;
            complex<double> S_true = 0;

            S_target.real(P0*P_alpha_1*voltage_scale1_P);
            S_target.imag(Q0*Q_alpha_1*voltage_scale1_P);
            S_true = S_target*rotation*(Vbus/Vsensor);
            S += S_true;

            S_target.real(P0*P_alpha_2*voltage_scale2_P);
            S_target.imag(Q0*Q_alpha_2*voltage_scale2_P);
            S_true = S_target*rotation*(Vbus/Vsensor);
            S += S_true;

            S_target.real(P0*P_alpha_3*voltage_scale3_P);
            S_target.imag(Q0*Q_alpha_3*voltage_scale3_P);
            S_true = S_target*rotation*(Vbus/Vsensor);
            S += S_true;

            S.real(S.real()*(1.0+P_Kf*f));
            S.imag(S.imag()*(1.0+Q_Kf*f));
        }
        else
        {
            double voltage_scale1_Q = LOAD::get_load_scale_with_voltage(Q_n_power_1, V, LOAD_LINEAR_CV);
            double voltage_scale2_Q = LOAD::get_load_scale_with_voltage(Q_n_power_2, V, LOAD_LINEAR_CV);
            double voltage_scale3_Q = LOAD::get_load_scale_with_voltage(Q_n_power_3, V, LOAD_LINEAR_CV);

            double angle_bus = get_bus_positive_sequence_angle_in_rad();
            double angle_sensor = get_bus_positive_sequence_angle_in_rad_from_sensor();
            double angle_shift = angle_bus - angle_sensor;
            complex<double> rotation(steps_cos(angle_shift), steps_sin(angle_shift));

            complex<double> S_target = 0;
            complex<double> S_true = 0;

            S_target.real(P0*P_alpha_1*voltage_scale1_P);
            S_target.imag(Q0*Q_alpha_1*voltage_scale1_Q);
            S_true = S_target*rotation*(Vbus/Vsensor);
            S += S_true;

            S_target.real(P0*P_alpha_2*voltage_scale2_P);
            S_target.imag(Q0*Q_alpha_2*voltage_scale2_Q);
            S_true = S_target*rotation*(Vbus/Vsensor);
            S += S_true;

            S_target.real(P0*P_alpha_3*voltage_scale3_P);
            S_target.imag(Q0*Q_alpha_3*voltage_scale3_Q);
            S_true = S_target*rotation*(Vbus/Vsensor);
            S += S_true;

            S.real(S.real()*(1.0+P_Kf*f));
            S.imag(S.imag()*(1.0+Q_Kf*f));
        }
    }
    return S;
}

complex<double> IEEL::get_load_current_in_pu_based_on_SBASE()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();

    complex<double> S = get_load_power_in_MVA();
    complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
    return conj(S*one_over_sbase/V);
}

complex<double> IEEL::get_norton_current_in_pu_based_on_SBASE()
{
    return 0.0;
}

complex<double> IEEL::get_derivative_of_load_current_wrt_voltage_on_SBASE()
{
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    double V = get_bus_positive_sequence_voltage_in_pu();
    complex<double> complex_v = get_bus_positive_sequence_complex_voltage_in_pu();
    double f = get_bus_frequency_deviation_in_pu();
    complex<double> j = complex<double>(0, 1.0);

    if(not is_bus_voltage_sensor_enabled())//out put PIZ load
    {
        if(is_PIZ)
        {
            complex<double> d_voltage_scale_P_over_d_complex_v = LOAD::get_derivative_of_constant_power_load_scale_with_voltage(V, complex_v, LOAD_LINEAR_CV);
            complex<double> d_voltage_scale_I_over_d_complex_v = LOAD::get_derivative_of_constant_current_load_scale_with_voltage(V, complex_v, LOAD_LINEAR_CV);
            complex<double> d_voltage_scale_Z_over_d_complex_v = conj(complex_v);

            complex<double> dP_scale_over_d_complex_v = P_alpha_1*d_voltage_scale_P_over_d_complex_v + P_alpha_2*d_voltage_scale_I_over_d_complex_v + P_alpha_3*d_voltage_scale_Z_over_d_complex_v;
            complex<double> dQ_scale_over_d_complex_v = Q_alpha_1*d_voltage_scale_P_over_d_complex_v + Q_alpha_2*d_voltage_scale_I_over_d_complex_v + Q_alpha_3*d_voltage_scale_Z_over_d_complex_v;

            complex<double> dP_over_d_complex_v = P0*dP_scale_over_d_complex_v*(1.0+P_Kf*f);
            complex<double> dQ_over_d_complex_v = Q0*dQ_scale_over_d_complex_v*(1.0+Q_Kf*f);

            return one_over_sbase*(dP_over_d_complex_v-j*dQ_over_d_complex_v)/conj(complex_v);
        }
        else
        {
            complex<double> d_voltage_scale1_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(P_n_power_1, V, complex_v, LOAD_LINEAR_CV);
            complex<double> d_voltage_scale2_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(P_n_power_2, V, complex_v, LOAD_LINEAR_CV);
            complex<double> d_voltage_scale3_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(P_n_power_3, V, complex_v, LOAD_LINEAR_CV);

             complex<double> d_scale_over_d_complex_v = P_alpha_1*d_voltage_scale1_over_d_complex_v + P_alpha_2*d_voltage_scale2_over_d_complex_v + P_alpha_3*d_voltage_scale3_over_d_complex_v;

            complex<double> dP_over_d_complex_v = P0*d_scale_over_d_complex_v*(1.0+P_Kf*f);

            if(fabs(P_n_power_1-Q_n_power_1)<FLOAT_EPSILON and fabs(P_n_power_2-Q_n_power_2)<FLOAT_EPSILON and fabs(P_n_power_3-Q_n_power_3)<FLOAT_EPSILON)
            {
                d_scale_over_d_complex_v = Q_alpha_1*d_voltage_scale1_over_d_complex_v + Q_alpha_2*d_voltage_scale2_over_d_complex_v + Q_alpha_3*d_voltage_scale3_over_d_complex_v;
            }
            else
            {
                d_voltage_scale1_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(Q_n_power_1, V, complex_v, LOAD_LINEAR_CV);
                d_voltage_scale2_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(Q_n_power_2, V, complex_v, LOAD_LINEAR_CV);
                d_voltage_scale3_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(Q_n_power_3, V, complex_v, LOAD_LINEAR_CV);

                d_scale_over_d_complex_v = Q_alpha_1*d_voltage_scale1_over_d_complex_v + Q_alpha_2*d_voltage_scale2_over_d_complex_v + Q_alpha_3*d_voltage_scale3_over_d_complex_v;
            }
            complex<double> dQ_over_d_complex_v = Q0*d_scale_over_d_complex_v*(1.0+Q_Kf*f);

            return one_over_sbase*(dP_over_d_complex_v-j*dQ_over_d_complex_v)/conj(complex_v);
        }
    }
    else// only output constant Z load  when use PLL
    {
        if(is_PIZ)
        {
            complex<double> d_voltage_scale_Z_over_d_complex_v = conj(complex_v);

            complex<double> dP_scale_over_d_complex_v = P_alpha_3*d_voltage_scale_Z_over_d_complex_v;
            complex<double> dQ_scale_over_d_complex_v = Q_alpha_3*d_voltage_scale_Z_over_d_complex_v;

            complex<double> dP_over_d_complex_v = P0*dP_scale_over_d_complex_v*(1.0+P_Kf*f);
            complex<double> dQ_over_d_complex_v = Q0*dQ_scale_over_d_complex_v*(1.0+Q_Kf*f);

            return one_over_sbase*(dP_over_d_complex_v-j*dQ_over_d_complex_v)/conj(complex_v);
        }
        else
        {
            complex<double> d_voltage_scale3_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(P_n_power_3, V, complex_v, LOAD_LINEAR_CV);
             complex<double> d_scale_over_d_complex_v = P_alpha_3*d_voltage_scale3_over_d_complex_v;
            complex<double> dP_over_d_complex_v = P0*d_scale_over_d_complex_v*(1.0+P_Kf*f);

            if(fabs(P_n_power_1-Q_n_power_1)<FLOAT_EPSILON and fabs(P_n_power_2-Q_n_power_2)<FLOAT_EPSILON and fabs(P_n_power_3-Q_n_power_3)<FLOAT_EPSILON)
            {
                d_scale_over_d_complex_v = Q_alpha_3*d_voltage_scale3_over_d_complex_v;
            }
            else
            {
                d_voltage_scale3_over_d_complex_v = LOAD::get_derivative_of_load_scale_with_voltage(Q_n_power_3, V, complex_v, LOAD_LINEAR_CV);
                d_scale_over_d_complex_v = Q_alpha_3*d_voltage_scale3_over_d_complex_v;
            }
            complex<double> dQ_over_d_complex_v = Q0*d_scale_over_d_complex_v*(1.0+Q_Kf*f);

            return one_over_sbase*(dP_over_d_complex_v-j*dQ_over_d_complex_v)/conj(complex_v);
        }
    }
}

void IEEL::check()
{
    ;
}
void IEEL::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void IEEL::save()
{
    ;
}
string IEEL::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    LOAD* load = get_load_pointer();
    unsigned int bus = load->get_load_bus();
    string identifier = "'"+load->get_identifier()+"'";

    string model_name = "'IEELBL'";
    /*
    unsigned int index = bus;
    switch(get_subsystem_type())
    {
        case AREA_SUBSYSTEM_TYPE:
        {
            index = load->get_area_number();
            identifier = "*";
            break;
        }
        case ZONE_SUBSYSTEM_TYPE:
        {
            index = load->get_zone_number();
            identifier = "*";
            break;
        }
        case OWNER_SUBSYSTEM_TYPE:
        {
            OWNERSHIP os = load->get_ownership();
            vector<unsigned int> owners = os.get_all_owners();
            if(owners.size()==0)
                index = 0;
            else
                index = owners[0];
            identifier = "*";
            break;
        }
        case ALL_SYSTEM_TYPE:
        {
            index = 0;
            identifier = "*";
            break;
        }
        default:
            break;
    }*/

    STEPS& toolkit = get_toolkit();
    NETWORK_MATRIX& network = toolkit.get_network_matrix();
    if(export_internal_bus_number==true)
        bus = network.get_internal_bus_number_of_physical_bus(bus)+1;

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<setprecision(6)<<get_P_alpha_1()<<", "
            <<setw(8)<<setprecision(6)<<get_P_alpha_2()<<", "
            <<setw(8)<<setprecision(6)<<get_P_alpha_3()<<", "
            <<setw(8)<<setprecision(6)<<get_Q_alpha_1()<<", "
            <<setw(8)<<setprecision(6)<<get_Q_alpha_2()<<", "
            <<setw(8)<<setprecision(6)<<get_Q_alpha_3()<<", "
            <<setw(8)<<setprecision(6)<<get_P_Kf()<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(6)<<get_Q_Kf()<<", "
            <<setw(8)<<setprecision(6)<<get_P_n_power_1()<<", "
            <<setw(8)<<setprecision(6)<<get_P_n_power_2()<<", "
            <<setw(8)<<setprecision(6)<<get_P_n_power_3()<<", "
            <<setw(8)<<setprecision(6)<<get_Q_n_power_1()<<", "
            <<setw(8)<<setprecision(6)<<get_Q_n_power_2()<<", "
            <<setw(8)<<setprecision(6)<<get_Q_n_power_3()<<" /";

    return osstream.str();
}

void IEEL::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("PA1", i); i++;
    add_model_data_name_and_index_pair("PA2", i); i++;
    add_model_data_name_and_index_pair("PA3", i); i++;
    add_model_data_name_and_index_pair("QA1", i); i++;
    add_model_data_name_and_index_pair("QA2", i); i++;
    add_model_data_name_and_index_pair("QA3", i); i++;
    add_model_data_name_and_index_pair("PKF", i); i++;
    add_model_data_name_and_index_pair("QKF", i); i++;
    add_model_data_name_and_index_pair("PN1", i); i++;
    add_model_data_name_and_index_pair("PN2", i); i++;
    add_model_data_name_and_index_pair("PN3", i); i++;
    add_model_data_name_and_index_pair("QN1", i); i++;
    add_model_data_name_and_index_pair("QN2", i); i++;
    add_model_data_name_and_index_pair("QN3", i); i++;
}

double IEEL::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="PA1")
        return get_P_alpha_1();
    if(par_name=="PA2")
        return get_P_alpha_2();
    if(par_name=="PA3")
        return get_P_alpha_3();
    if(par_name=="QA1")
        return get_Q_alpha_1();
    if(par_name=="QA2")
        return get_Q_alpha_2();
    if(par_name=="QA3")
        return get_Q_alpha_3();
    if(par_name=="PKF")
        return get_P_Kf();
    if(par_name=="QKF")
        return get_Q_Kf();
    if(par_name=="PN1")
        return get_P_n_power_1();
    if(par_name=="PN2")
        return get_P_n_power_2();
    if(par_name=="PN3")
        return get_P_n_power_3();
    if(par_name=="QN1")
        return get_Q_n_power_1();
    if(par_name=="QN2")
        return get_Q_n_power_2();
    if(par_name=="QN3")
        return get_Q_n_power_3();

    return 0.0;
}

void IEEL::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(par_name=="PA1")
        return set_P_alpha_1(value);
    if(par_name=="PA2")
        return set_P_alpha_2(value);
    if(par_name=="PA3")
        return set_P_alpha_3(value);
    if(par_name=="QA1")
        return set_Q_alpha_1(value);
    if(par_name=="QA2")
        return set_Q_alpha_2(value);
    if(par_name=="QA3")
        return set_Q_alpha_3(value);
    if(par_name=="PKF")
        return set_P_Kf(value);
    if(par_name=="QKF")
        return set_Q_Kf(value);
    if(par_name=="PN1")
        return set_P_n_power_1(value);
    if(par_name=="PN2")
        return set_P_n_power_2(value);
    if(par_name=="PN3")
        return set_P_n_power_3(value);
    if(par_name=="QN1")
        return set_Q_n_power_1(value);
    if(par_name=="QN2")
        return set_Q_n_power_2(value);
    if(par_name=="QN3")
        return set_Q_n_power_3(value);

    return;
}

double IEEL::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void IEEL::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;

    add_model_internal_variable_name_and_index_pair("TOTAL ACTIVE POWER LOAD IN MW", i); i++;
    add_model_internal_variable_name_and_index_pair("TOTAL REACTIVE POWER LOAD IN MVAR", i); i++;
    add_model_internal_variable_name_and_index_pair("INITIAL ACTIVE POWER LOAD IN MW", i); i++;
    add_model_internal_variable_name_and_index_pair("INITIAL REACTIVE POWER LOAD IN MVAR", i); i++;
    add_model_internal_variable_name_and_index_pair("BUS ANGLE FROM SENSOR IN RAD", i); i++;
    add_model_internal_variable_name_and_index_pair("BUS ANGLE FROM SENSOR IN DEG", i); i++;
    add_model_internal_variable_name_and_index_pair("BUS VOLTAGE FROM SENSOR IN PU", i); i++;
}

double IEEL::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "TOTAL ACTIVE POWER LOAD IN MW")
        return get_load_power_in_MVA().real();
    if(var_name == "TOTAL REACTIVE POWER LOAD IN MVAR")
        return get_load_power_in_MVA().imag();
    if(var_name == "INITIAL ACTIVE POWER LOAD IN MW")
        return get_initial_load_power_in_MVA().real();
    if(var_name == "INITIAL REACTIVE POWER LOAD IN MVAR")
        return get_initial_load_power_in_MVA().imag();
    if(var_name == "BUS ANGLE FROM SENSOR IN RAD")
        return angle_sensor.get_pll_angle_in_rad();
    if(var_name == "BUS ANGLE FROM SENSOR IN DEG")
        return angle_sensor.get_pll_angle_in_deg();
    if(var_name == "BUS VOLTAGE FROM SENSOR IN PU")
        return voltage_sensor.get_output();

    return 0.0;
}

string IEEL::get_dynamic_data_in_psse_format() const
{
    return "";
}

string IEEL::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string IEEL::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}

void IEEL::build_linearized_matrix_ABCD()
{
    initialize_ABCD_matrix_for_linearization();
    STEPS_SPARSE_MATRIX A;
    STEPS_SPARSE_MATRIX B;
    STEPS_SPARSE_MATRIX C;
    STEPS_SPARSE_MATRIX D;

    /*complex<double> V = get_bus_positive_sequence_complex_voltage_in_pu();
    double Ux = V.real();
    double Uy = V.imag();
    double U = sqrt(Ux*Ux+Uy*Uy);
    STEPS& toolkit = get_toolkit();
    double one_over_sbase = toolkit.get_one_over_system_base_power_in_one_over_MVA();
    complex<double> PQ0 = get_initial_load_power_in_MVA()*one_over_sbase;
    double P0 = PQ0.real();
    double Q0 = PQ0.imag();
    double ap1 = get_P_alpha_1();
    double ap2 = get_P_alpha_2();
    double ap3 = get_P_alpha_3();
    double aq1 = get_Q_alpha_1();
    double aq2 = get_Q_alpha_2();
    double aq3 = get_Q_alpha_3();
    double np1 = get_P_n_power_1();
    double np2 = get_P_n_power_2();
    double np3 = get_P_n_power_3();
    double nq1 = get_Q_n_power_1();
    double nq2 = get_Q_n_power_2();
    double nq3 = get_Q_n_power_3();*/

    /* codes are wrong. need further check
    D.add_entry(0,0, (P0*Ux*Ux*(ap2+2*ap3)+Q0*Ux*Uy*(aq2+2*aq3))/(U*U*U*U)-P0/(U*U));
    D.add_entry(0,1, (Q0*Uy*Uy*(aq2+2*aq3)+P0*Ux*Uy*(ap2+2*ap3))/(U*U*U*U)-Q0/(U*U));
    D.add_entry(1,0, (Q0*Ux*Ux*(aq2+2*aq3)-P0*Ux*Uy*(ap2+2*ap3))/(U*U*U*U)-Q0/(U*U));
    D.add_entry(1,1, (P0*Uy*Uy*(ap2+2*ap3)-Q0*Ux*Uy*(aq2+2*aq3))/(U*U*U*U)-P0/(U*U));
    */
    A.compress_and_merge_duplicate_entries();
    B.compress_and_merge_duplicate_entries();
    C.compress_and_merge_duplicate_entries();
    D.compress_and_merge_duplicate_entries();

    vector<STEPS_SPARSE_MATRIX*> matrix;
    matrix.push_back(&A);
    matrix.push_back(&B);
    matrix.push_back(&C);
    matrix.push_back(&D);
    build_linearized_matrix_ABCD_with_basic_ABCD(matrix);
}

double IEEL::get_bus_positive_sequence_voltage_in_pu_from_sensor()
{
    return voltage_sensor.get_output();
}

double IEEL::get_bus_positive_sequence_angle_in_rad_from_sensor()
{
    return angle_sensor.get_pll_angle_in_rad();
}
