#include "header/model/sg_models/turbine_governor_model/GAST2A.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <istream>
#include <iostream>
using namespace std;
GAST2A::GAST2A()
{
    clear();
}

GAST2A::~GAST2A()
{
    clear();
}
void GAST2A::clear()
{
    prepare_model_data_table();
    prepare_model_internal_variable_table();

    set_gas_W(0.0);
    set_gas_X_in_s(0.0);
    set_gas_Y_in_s(0.1);
    set_gas_Z(1);
    set_gas_ETD_in_s(5.0);
    set_gas_TCD_in_s(0.5);
    set_gas_Prate_in_MW(100.0);
    set_gas_T_in_s(3.0);
    set_gas_max_in_pu(1.0);
    set_gas_min_in_pu(0.0);
    set_gas_ECR_in_s(5.0);
    set_gas_K3(1.0);
    set_gas_a(1.0);
    set_gas_b_in_s(0.1);
    set_gas_c(1.0);
    set_gas_Tf_in_s(0.1);
    set_gas_Kf(1.0);
    set_gas_K5(1.0);
    set_gas_K4(1.0);
    set_gas_T3_in_s(1.0);
    set_gas_T4_in_s(1.0);
    set_gas_Tt_in_s(1.0);
    set_gas_T5_in_s(1.0);
    set_gas_af1(1.0);
    set_gas_bf1(1.0);
    set_gas_af2(1.0);
    set_gas_bf2(1.0);
    set_gas_cf2(1.0);
    set_gas_TR_in_deg(200.0);
    set_gas_K6(0.1);
    set_gas_TC_in_deg(250.0);
}
void GAST2A::copy_from_const_model(const GAST2A& model)
{
    clear();

    set_gas_W(model.get_gas_W());
    set_gas_X_in_s(model.get_gas_X_in_s());
    set_gas_Y_in_s(model.get_gas_Y_in_s());
    set_gas_Z(model.get_gas_Z());
    set_gas_ETD_in_s(model.get_gas_ETD_in_s());
    set_gas_TCD_in_s(model.get_gas_TCD_in_s());
    set_gas_Prate_in_MW(model.get_gas_Prate_in_MW());
    set_gas_T_in_s(model.get_gas_T_in_s());
    set_gas_max_in_pu(model.get_gas_max_in_pu());
    set_gas_min_in_pu(model.get_gas_min_in_pu());
    set_gas_ECR_in_s(model.get_gas_ECR_in_s());
    set_gas_K3(model.get_gas_K3());
    set_gas_a(model.get_gas_a());
    set_gas_b_in_s(model.get_gas_b_in_s());
    set_gas_c(model.get_gas_c());
    set_gas_Tf_in_s(model.get_gas_Tf_in_s());
    set_gas_Kf(model.get_gas_Kf());
    set_gas_K5(model.get_gas_K5());
    set_gas_K4(model.get_gas_K4());
    set_gas_T3_in_s(model.get_gas_T3_in_s());
    set_gas_T4_in_s(model.get_gas_T4_in_s());
    set_gas_Tt_in_s(model.get_gas_Tt_in_s());
    set_gas_T5_in_s(model.get_gas_T5_in_s());
    set_gas_af1(model.get_gas_af1());
    set_gas_bf1(model.get_gas_bf1());
    set_gas_af2(model.get_gas_af2());
    set_gas_bf2(model.get_gas_bf2());
    set_gas_cf2(model.get_gas_cf2());
    set_gas_TR_in_deg(model.get_gas_TR_in_deg());
    set_gas_K6(model.get_gas_K6());
    set_gas_TC_in_deg(model.get_gas_TC_in_deg());
}

GAST2A::GAST2A(const GAST2A&model) : TURBINE_GOVERNOR_MODEL()
{
    copy_from_const_model(model);
}

GAST2A& GAST2A::operator=(const GAST2A& model)
{
    if(this==&model)
        return *this;

    copy_from_const_model(model);

    return (*this);
}

string GAST2A::get_model_name() const
{
    return "GAST2A";
}


void GAST2A::set_gas_W(double W)
{
    gas_governor_W = W;
}

void GAST2A::set_gas_X_in_s(double T)
{
    gas_governor_X = T;
}

void GAST2A::set_gas_Y_in_s(double T)
{
    gas_governor_Y = T;
}

void GAST2A::set_gas_Z(size_t Z)
{
    gas_governor_Z = Z;
}

void GAST2A::set_gas_ETD_in_s(double T)
{
    gas_ETD = T;
}

void GAST2A::set_gas_TCD_in_s(double T)
{
    gas_turbine_dynamic.set_T_in_s(T);
}

void GAST2A::set_gas_Prate_in_MW(double P)
{
    gas_turbine_rate_MW = P;
}

void GAST2A::set_gas_T_in_s(double T)
{
    gas_T = T;
}

void GAST2A::set_gas_max_in_pu(double U)
{
    gas_governor_MAX = U;
}

void GAST2A::set_gas_min_in_pu(double U)
{
    gas_governor_MIN = U;
}

void GAST2A::set_gas_ECR_in_s(double T)
{
    gas_ECR = T;
}

void GAST2A::set_gas_K3(double K)
{
    gas_K3 = K;
}

void GAST2A::set_gas_a(double a)
{
    gas_a = a;
}

void GAST2A::set_gas_b_in_s(double T)
{
    gas_b = T;
}

void GAST2A::set_gas_c(double K)
{
    gas_c = K;
}

void GAST2A::set_gas_Tf_in_s(double T)
{
    gas_fuel_system.set_T_in_s(T);
}

void GAST2A::set_gas_Kf(double K)
{
    gas_Kf = K;
}

void GAST2A::set_gas_K5(double K)
{
    gas_radiation_shield.set_K(K);
}

void GAST2A::set_gas_K4(double K)
{
    gas_K4 = K;
}

void GAST2A::set_gas_T3_in_s(double T)
{
    gas_radiation_shield.set_T_in_s(T);
}

void GAST2A::set_gas_T4_in_s(double T)
{
    gas_thermocouple.set_T_in_s(T);
}

void GAST2A::set_gas_Tt_in_s(double T)
{
    gas_Tt = T;
}

void GAST2A::set_gas_T5_in_s(double T)
{
    gas_T5 = T;
}

void GAST2A::set_gas_af1(double K)
{
    gas_af1 = K;
}

void GAST2A::set_gas_bf1(double K)
{
    gas_bf1 = K;
}

void GAST2A::set_gas_af2(double K)
{
    gas_af2 = K;
}

void GAST2A::set_gas_bf2(double K)
{
    gas_bf2 = K;
}

void GAST2A::set_gas_cf2(double K)
{
    gas_cf2 = K;
}

void GAST2A::set_gas_TR_in_deg(double T)
{
    gas_TR = T;
}

void GAST2A::set_gas_K6(double K)
{
    gas_K6 = K;
}

void GAST2A::set_gas_TC_in_deg(double T)
{
    gas_TC = T;
}

double GAST2A::get_gas_W() const
{
    return gas_governor_W;
}

double GAST2A::get_gas_X_in_s() const
{
    return gas_governor_X;
}

double GAST2A::get_gas_Y_in_s() const
{
    return gas_governor_Y;
}

size_t GAST2A::get_gas_Z() const
{
    return gas_governor_Z;
}

double GAST2A::get_gas_ETD_in_s() const
{
    return gas_ETD;
}

double GAST2A::get_gas_TCD_in_s() const
{
    return gas_turbine_dynamic.get_T_in_s();
}

double GAST2A::get_gas_Prate_in_MW() const
{
    return gas_turbine_rate_MW;
}

double GAST2A::get_gas_T_in_s() const
{
    return gas_T;
}

double GAST2A::get_gas_max_in_pu() const
{
    return gas_governor_MAX;
}

double GAST2A::get_gas_min_in_pu() const
{
    return gas_governor_MIN;
}

double GAST2A::get_gas_ECR_in_s() const
{
    return gas_ECR;
}

double GAST2A::get_gas_K3() const
{
    return gas_K3;
}

double GAST2A::get_gas_a() const
{
    return gas_a;
}

double GAST2A::get_gas_b_in_s() const
{
    return gas_b;
}

double GAST2A::get_gas_c() const
{
    return gas_c;
}

double GAST2A::get_gas_Tf_in_s() const
{
    return gas_fuel_system.get_T_in_s();
}

double GAST2A::get_gas_Kf() const
{
    return gas_Kf;
}

double GAST2A::get_gas_K5() const
{
    return gas_radiation_shield.get_K();
}

double GAST2A::get_gas_K4() const
{
    return gas_K4;
}

double GAST2A::get_gas_T3_in_s() const
{
    return gas_fuel_system.get_T_in_s();
}

double GAST2A::get_gas_T4_in_s() const
{
    return gas_thermocouple.get_T_in_s();
}

double GAST2A::get_gas_Tt_in_s() const
{
    return gas_Tt;
}

double GAST2A::get_gas_T5_in_s() const
{
    return gas_T5;
}

double GAST2A::get_gas_af1() const
{
    return gas_af1;
}

double GAST2A::get_gas_bf1() const
{
    return gas_bf1;
}

double GAST2A::get_gas_af2() const
{
    return gas_af2;
}

double GAST2A::get_gas_bf2() const
{
    return gas_bf2;
}

double GAST2A::get_gas_cf2() const
{
    return gas_cf2;
}

double GAST2A::get_gas_TR_in_deg() const
{
    return gas_TR;
}

double GAST2A::get_gas_K6() const
{
    return gas_K6;
}

double GAST2A::get_gas_TC_in_deg() const
{
    return gas_TC;
}


bool GAST2A::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=34)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            size_t i=3;

            set_gas_W(get_double_data(data[i],"0.0")); i++;
            set_gas_X_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Y_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Z(get_integer_data(data[i],"0.0")); i++;
            set_gas_ETD_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_TCD_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Prate_in_MW(get_double_data(data[i],"0.0")); i++;
            set_gas_T_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_max_in_pu(get_double_data(data[i],"0.0")); i++;
            set_gas_min_in_pu(get_double_data(data[i],"0.0")); i++;
            set_gas_ECR_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_K3(get_double_data(data[i],"0.0")); i++;
            set_gas_a(get_double_data(data[i],"0.0")); i++;
            set_gas_b_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_c(get_double_data(data[i],"0.0")); i++;
            set_gas_Tf_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Kf(get_double_data(data[i],"0.0")); i++;
            set_gas_K5(get_double_data(data[i],"0.0")); i++;
            set_gas_K4(get_double_data(data[i],"0.0")); i++;
            set_gas_T3_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_T4_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_Tt_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_T5_in_s(get_double_data(data[i],"0.0")); i++;
            set_gas_af1(get_double_data(data[i],"0.0")); i++;
            set_gas_bf1(get_double_data(data[i],"0.0")); i++;
            set_gas_af2(get_double_data(data[i],"0.0")); i++;
            set_gas_bf2(get_double_data(data[i],"0.0")); i++;
            set_gas_cf2(get_double_data(data[i],"0.0")); i++;
            set_gas_TR_in_deg(get_double_data(data[i],"0.0")); i++;
            set_gas_K6(get_double_data(data[i],"0.0")); i++;
            set_gas_TC_in_deg(get_double_data(data[i],"0.0"));

            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool GAST2A::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool GAST2A::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void GAST2A::setup_block_toolkit_and_parameters()
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double delt = toolkit.get_dynamic_simulation_time_step_in_s();

    gas_governor_droop.set_toolkit(toolkit);
    gas_governor_droop.set_K(get_gas_W());
    gas_governor_droop.set_T1_in_s(get_gas_X_in_s());
    gas_governor_droop.set_T2_in_s(get_gas_Y_in_s());
    gas_governor_droop.set_limiter_type(NON_WINDUP_LIMITER);
    gas_governor_droop.set_upper_limit(get_gas_max_in_pu());
    gas_governor_droop.set_lower_limit(get_gas_min_in_pu());


    gas_governor_iso.set_toolkit(toolkit);
    gas_governor_iso.set_Kp(get_gas_W()*get_gas_X_in_s()/get_gas_Y_in_s());
    gas_governor_iso.set_Ki(get_gas_W()/get_gas_Y_in_s());
    gas_governor_iso.set_limiter_type(NON_WINDUP_LIMITER);
    gas_governor_iso.set_upper_limit(get_gas_max_in_pu());
    gas_governor_iso.set_lower_limit(get_gas_min_in_pu());

    gas_fuel_control.set_toolkit(toolkit);
    gas_fuel_control.set_buffer_size(round(get_gas_T_in_s()/delt));

    gas_valve_positioner.set_toolkit(toolkit);
    gas_valve_positioner.set_limiter_type(NO_LIMITER);
    gas_valve_positioner.set_K(get_gas_a()/get_gas_c());
    gas_valve_positioner.set_K(get_gas_b_in_s()/get_gas_c());

    gas_fuel_system.set_toolkit(toolkit);
    gas_fuel_system.set_limiter_type(NO_LIMITER);
    gas_fuel_system.set_K(1.0);
    gas_fuel_system.set_T_in_s(get_gas_Tf_in_s());

    gas_combustor.set_toolkit(toolkit);
    gas_combustor.set_buffer_size(round(get_gas_ECR_in_s()/delt));

    gas_turbine_exhaust.set_toolkit(toolkit);
    gas_turbine_exhaust.set_buffer_size(round(get_gas_ETD_in_s()/delt));

    gas_radiation_shield.set_toolkit(toolkit);
    gas_radiation_shield.set_limiter_type(NO_LIMITER);
    gas_radiation_shield.set_K(get_gas_K5());
    gas_radiation_shield.set_T_in_s(get_gas_T3_in_s());

    gas_thermocouple.set_toolkit(toolkit);
    gas_thermocouple.set_limiter_type(NO_LIMITER);
    gas_thermocouple.set_T_in_s(get_gas_T4_in_s());

    gas_temperature_control.set_toolkit(toolkit);
    gas_temperature_control.set_limiter_type(NON_WINDUP_LIMITER);
    gas_temperature_control.set_upper_limit(get_gas_max_in_pu());
    gas_temperature_control.set_lower_limit(-INFINITE_THRESHOLD);
    gas_temperature_control.set_Kp(get_gas_T5_in_s()/get_gas_Tt_in_s());
    gas_temperature_control.set_Ki(1.0/get_gas_Tt_in_s());

    gas_turbine_dynamic.set_toolkit(toolkit);
    gas_turbine_dynamic.set_limiter_type(NO_LIMITER);
    gas_turbine_dynamic.set_T_in_s(get_gas_TCD_in_s());
}

void GAST2A::initialize()
{
    ostringstream osstream;
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

                STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
                double time = toolkit.get_dynamic_simulation_time_in_s();

                double pmech0 = get_initial_mechanical_power_in_pu_based_on_mbase_from_sync_generator_model();
                double mbase = get_mbase_in_MVA();
                double prate = get_gas_Prate_in_MW();

                double f2 = pmech0*mbase/prate;
                double wf2 = (f2-get_gas_af2())/get_gas_bf2();

                gas_turbine_dynamic.set_output(wf2);
                gas_turbine_dynamic.initialize();

                gas_combustor.initialize_buffer(time, wf2);

                gas_turbine_exhaust.initialize_buffer(time, wf2);

                double wf1 = wf2;

                double f1 = get_gas_TR_in_deg() - get_gas_af1()*(1.0-wf1);

                gas_radiation_shield.set_output(f1*get_gas_K5());
                gas_radiation_shield.initialize();

                gas_thermocouple.set_output((get_gas_K4()+get_gas_K5())*f1);
                gas_thermocouple.initialize();

                gas_temperature_control.set_output(get_gas_max_in_pu());
                gas_temperature_control.initialize();

                gas_fuel_system.set_output(wf2);
                gas_fuel_system.initialize();

                gas_valve_positioner.set_output(wf2);
                gas_valve_positioner.initialize();

                double gfc = gas_valve_positioner.get_input()+get_gas_Kf()*wf2-get_gas_K6();
                gas_fuel_control.initialize_buffer(time, gfc);

                double output = gfc/get_gas_K3();
                if(output>get_gas_max_in_pu())
                {
                    osstream<<"Initialization error. Governor of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds upper limit."
                      <<"Governor is "<<output<<", and max is "<<get_gas_max_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }
                if(output<get_gas_min_in_pu())
                {
                    osstream<<"Initialization error. Governor of '"<<get_model_name()<<"' model of "<<get_device_name()<<" exceeds lower limit."
                      <<"Governor is "<<output<<", and min is "<<get_gas_min_in_pu()<<".";
                    toolkit.show_information_with_leading_time_stamp(osstream);
                }

                double input = 0.0;
                if(get_gas_Z()!=0.0)
                {
                    gas_governor_droop.set_output(output);
                    gas_governor_droop.initialize();
                    input = gas_governor_droop.get_input();
                }
                else
                {
                    gas_governor_iso.set_output(output);
                    gas_governor_iso.initialize();
                    input = gas_governor_iso.get_input();
                }
                set_initial_mechanical_power_reference_in_pu_based_on_mbase(input);

                set_flag_model_initialized_as_true();
            }
        }
    }
}

void GAST2A::run(DYNAMIC_MODE mode)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double time = toolkit.get_dynamic_simulation_time_in_s();

    double Pref = get_mechanical_power_reference_in_pu_based_on_mbase();

    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double input = Pref-speed;
    double output = 0.0;
    if(get_gas_Z()!=0.0)
    {
        gas_governor_droop.set_input(input);
        gas_governor_droop.run(mode);
        output = gas_governor_droop.get_output();
    }
    else
    {
        gas_governor_iso.set_input(input);
        gas_governor_iso.run(mode);
        output = gas_governor_iso.get_output();
    }

    double output2 = gas_temperature_control.get_output();
    if(get_gas_TC_in_deg()-gas_thermocouple.get_output()<0.0)
        output2 = output;

    output = (output<=output2)?output:output2;

    input = output*(1.0+speed)*get_gas_K3();

    gas_fuel_control.append_data(time, input);

    output = gas_fuel_control.get_buffer_value_at_head();

    input = output + get_gas_K6() - get_gas_Kf()*gas_fuel_system.get_output();

    gas_valve_positioner.set_input(input);
    gas_valve_positioner.run(mode);

    gas_fuel_system.set_input(gas_valve_positioner.get_output());
    gas_fuel_system.run(mode);

    gas_combustor.append_data(time, gas_fuel_system.get_output());

    double wf = gas_combustor.get_buffer_value_at_head();

    gas_turbine_exhaust.append_data(time, wf);

    double wf1 = gas_turbine_exhaust.get_buffer_time_at_head();
    double f1 = get_gas_TR_in_deg()-get_gas_af1()*(1.0-wf1)-get_gas_bf1()*speed;

    gas_radiation_shield.set_input(f1);
    gas_radiation_shield.run(mode);

    input = get_gas_K4()*f1+gas_radiation_shield.get_output();

    gas_thermocouple.set_input(input);
    gas_thermocouple.run(mode);

    gas_temperature_control.set_input(get_gas_TC_in_deg()-gas_thermocouple.get_output());
    gas_temperature_control.run(mode);

    gas_turbine_dynamic.set_input(wf);
    gas_turbine_dynamic.run(mode);

    if(mode==UPDATE_MODE)
        set_flag_model_updated_as_true();
}

double GAST2A::get_mechanical_power_in_pu_based_on_mbase() const
{
    double speed = get_rotor_speed_deviation_in_pu_from_sync_generator_model();

    double wf2 = gas_turbine_dynamic.get_output();
    double f2 = get_gas_af2()+get_gas_bf2()*wf2-get_gas_cf2()*speed;

    double pmech = f2*(1.0+speed);

    return pmech*get_gas_Prate_in_MW()/get_mbase_in_MVA();
}

double GAST2A::get_mechanical_power_upper_limit_in_pu_based_on_mbase() const
{
    return get_gas_max_in_pu();
}

double GAST2A::get_mechanical_power_lower_limit_in_pu_based_on_mbase() const
{
    return get_gas_min_in_pu();
}

void GAST2A::check()
{
    ;
}

void GAST2A::report()
{
    ostringstream osstream;
    osstream<<get_standard_model_string();
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
void GAST2A::save()
{
    ;
}
string GAST2A::get_standard_model_string() const
{
    ostringstream osstream;

    DEVICE_ID did = get_device_id();
    size_t bus = did.get_device_terminal().get_buses()[0];
    string identifier = did.get_device_identifier();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', 0, '', "
      <<setw(8)<<setprecision(6)<<get_gas_W()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_X_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Y_in_s()<<", "
      <<setw(8)<<setprecision(0)<<get_gas_Z()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_ETD_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_TCD_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Prate_in_MW()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_max_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_min_in_pu()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_ECR_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K3()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_a()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_b_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_c()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Tf_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Kf()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K5()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K4()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T3_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T4_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_Tt_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_T5_in_s()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_af1()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_bf1()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_af2()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_bf2()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_cf2()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_TR_in_deg()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_K6()<<", "
      <<setw(8)<<setprecision(6)<<get_gas_TC_in_deg()<<"  /";
    return osstream.str();
}

void GAST2A::prepare_model_data_table()
{
    clear_model_data_table();
    size_t i=0;
    add_model_data_name_and_index_pair("PMAX", i); i++;
    add_model_data_name_and_index_pair("PMIN", i); i++;
}

double GAST2A::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return get_gas_max_in_pu();
    if(par_name=="PMIN")
        return get_gas_min_in_pu();

    return 0.0;
}

void GAST2A::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="PMAX")
        return set_gas_max_in_pu(value);
    if(par_name=="PMIN")
        return set_gas_min_in_pu(value);
}

void GAST2A::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    size_t i=0;
    add_model_inernal_variable_name_and_index_pair("STATE@DROOP", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@SERVO MOTOR", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 1", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 2", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 3", i); i++;
    add_model_inernal_variable_name_and_index_pair("STATE@DELAYER 4", i); i++;
}

double GAST2A::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    if(var_name == "STATE@SPEED GOVERNOR")
    {
        if(get_gas_Z()!=0.0)
            return gas_governor_droop.get_state();
        else
            return gas_governor_iso.get_state();
    }

    if(var_name == "STATE@VALVE POSITIONER")
        return gas_valve_positioner.get_state();

    if(var_name == "STATE@FUEL SYSTEM")
        return gas_fuel_system.get_state();

    if(var_name == "STATE@TURBINE DYNAMICS")
        return gas_turbine_dynamic.get_state();

    if(var_name == "STATE@RADIATION SHIELD")
        return gas_radiation_shield.get_state();

    if(var_name == "STATE@THERMOCOUPLE")
        return gas_thermocouple.get_state();

    if(var_name == "STATE@TEMPERATURE CONTROL")
        return gas_temperature_control.get_state();

    return 0.0;
}


string GAST2A::get_dynamic_data_in_psse_format() const
{
    return "";
}

string GAST2A::get_dynamic_data_in_bpa_format() const
{
    return get_dynamic_data_in_psse_format();
}

string GAST2A::get_dynamic_data_in_steps_format() const
{
    return get_dynamic_data_in_psse_format();
}
