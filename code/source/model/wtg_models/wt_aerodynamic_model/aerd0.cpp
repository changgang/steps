#include "header/model/wtg_models/wt_aerodynamic_model/aerd0.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

AERD0::AERD0()
{
    set_C1(0.22);
    set_C2(116.0);
    set_C3(0.4);
    set_C4(5.0);
    set_C5(12.5);
    set_C6(0.0);

    set_C7(0.08);
    set_C8(0.035);
}

AERD0::AERD0(const AERD0& model):WT_AERODYNAMIC_MODEL()
{
    copy_from_const_model(model);
}

AERD0::~AERD0()
{
    ;
}

AERD0& AERD0::operator=(const AERD0& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void AERD0::copy_from_const_model(const AERD0& model)
{
    WT_AERODYNAMIC_MODEL::copy_from_const_model(model);

    set_C1(model.get_C1());
    set_C2(model.get_C2());
    set_C3(model.get_C3());
    set_C4(model.get_C4());
    set_C5(model.get_C5());
    set_C6(model.get_C6());
    set_C7(model.get_C7());
    set_C8(model.get_C8());
}

void AERD0::set_C1(double c)
{
    Cp_Coefficients[0] = c;
}

void AERD0::set_C2(double c)
{
    Cp_Coefficients[1] = c;
}

void AERD0::set_C3(double c)
{
    Cp_Coefficients[2] = c;
}

void AERD0::set_C4(double c)
{
    Cp_Coefficients[3] = c;
}

void AERD0::set_C5(double c)
{
    Cp_Coefficients[4] = c;
}

void AERD0::set_C6(double c)
{
    Cp_Coefficients[5] = c;
}

void AERD0::set_C7(double c)
{
    Cp_Coefficients[6] = c;
}

void AERD0::set_C8(double c)
{
    Cp_Coefficients[7] = c;
}

double AERD0::get_C1() const
{
    return Cp_Coefficients[0];
}

double AERD0::get_C2() const
{
    return Cp_Coefficients[1];
}

double AERD0::get_C3() const
{
    return Cp_Coefficients[2];
}

double AERD0::get_C4() const
{
    return Cp_Coefficients[3];
}

double AERD0::get_C5() const
{
    return Cp_Coefficients[4];
}

double AERD0::get_C6() const
{
    return Cp_Coefficients[5];
}

double AERD0::get_C7() const
{
    return Cp_Coefficients[6];
}

double AERD0::get_C8() const
{
    return Cp_Coefficients[7];
}

double AERD0::get_Cp(double lambda, double pitch_deg) const
{
    //double pitch_angle = deg2rad(pitch_deg);

    double C1 = get_C1();
    double C2 = get_C2();
    double C3 = get_C3();
    double C4 = get_C4();
    double C5 = get_C5();
    double C6 = get_C6();
    double C7 = get_C7();
    double C8  = get_C8();

    double pitch_angle = pitch_deg;
    double L = 1.0/(lambda+C7*pitch_angle)-C8/(pitch_angle*pitch_angle*pitch_angle+1.0);
    L = 1.0/L;


    double cp = C1*(C2/L-C3*pitch_angle-C4)*exp(-C5/L)+C6*lambda;
    if(cp<0.0)
        cp = 0.0;
    return cp;
}

double AERD0::get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const
{
    double Cp0 = get_Cp(lambda, pitch_deg);
    double Cp1 = get_Cp(lambda+1e-4, pitch_deg);

    return (Cp1-Cp0)*1e4;
}


string AERD0::get_model_name() const
{
    return "AERD0";
}

double AERD0::get_double_data_with_index(size_t index) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input index is provided: "<<index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double AERD0::get_double_data_with_name(string par_name) const
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input par_name is provided: "<<par_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

void AERD0::set_double_data_with_index(size_t index, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (index, value) is provided: ("<<index<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

void AERD0::set_double_data_with_name(string par_name, double value)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input (par_name, value) is provided: ("<<par_name<<", "<<value<<").";
    show_information_with_leading_time_stamp(osstream);
    return;
}

bool AERD0::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return true;
}

bool AERD0::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<17)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    int speed_mode_flag=0;
    size_t n;
    double vwind0, gear_eta, rou0_air, min_speed, max_speed;
    double rou_air;
    double c1, c2, c3, c4, c5, c6;

    size_t i=3;
    speed_mode_flag = get_integer_data(dyrdata[i],"0"); i++;
    n = size_t(get_integer_data(dyrdata[i],"1")); i++;
    vwind0 = get_double_data(dyrdata[i],"0.0"); i++;
    gear_eta = get_double_data(dyrdata[i],"0.0"); i++;
    rou0_air = get_double_data(dyrdata[i],"0.0"); i++;
    min_speed = get_double_data(dyrdata[i],"0.6"); i++;
    max_speed = get_double_data(dyrdata[i],"1.2"); i++;
    rou_air = get_double_data(dyrdata[i],"0.0"); i++;
    c1 = get_double_data(dyrdata[i],"0.0"); i++;
    c2 = get_double_data(dyrdata[i],"0.0"); i++;
    c3 = get_double_data(dyrdata[i],"0.0"); i++;
    c4 = get_double_data(dyrdata[i],"0.0"); i++;
    c5 = get_double_data(dyrdata[i],"0.0"); i++;
    c6 = get_double_data(dyrdata[i],"0.0");

    switch(speed_mode_flag)
    {
        case -1:
        {
            set_turbine_speed_mode(WT_UNDERSPEED_MODE);
            break;
        }
        case 1:
        {
            set_turbine_speed_mode(WT_OVERSPEED_MODE);
            break;
        }
        default:
        {
            set_turbine_speed_mode(WT_MPPT_MODE);
            break;
        }
    }
    set_number_of_pole_pairs(n);
    set_nominal_wind_speed_in_mps(vwind0);
    set_gear_efficiency(gear_eta);
    set_nominal_air_density_in_kgpm3(rou0_air);
    set_min_steady_state_turbine_speed_in_pu(min_speed);
    set_max_steady_state_turbine_speed_in_pu(max_speed);
    set_air_density_in_kgpm3(rou_air);
    set_C1(c1);
    set_C2(c2);
    set_C3(c3);
    set_C4(c4);
    set_C5(c5);
    set_C6(c6);

    is_successful = true;

    return is_successful;
}

bool AERD0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return true;
}


void AERD0::check()
{
    ostringstream oosstream;
}

void AERD0::clear()
{
    ;
}

void AERD0::report()
{
    ;
}

void AERD0::save()
{
    ;
}

string AERD0::get_standard_model_string() const
{
    ostringstream osstream;
    WT_GENERATOR* gen = get_wt_generator_pointer();
    size_t bus = gen->get_generator_bus();
    string identifier= gen->get_identifier();

    int speed_mode_flag = 0;
    switch(get_turbine_speed_mode())
    {
        case WT_UNDERSPEED_MODE:
        {
            speed_mode_flag = -1;
            break;
        }
        case WT_OVERSPEED_MODE:
        {
            speed_mode_flag = 1;
            break;
        }
        default:
        {
            speed_mode_flag = 0;
            break;
        }
    }
    size_t n = get_number_of_pole_pairs();

    osstream<<setw(8)<<bus<<", "
      <<"'"<<get_model_name()<<"', "
      <<"'"<<identifier<<"', "
      <<setw(4)<<speed_mode_flag<<", "
      <<setw(4)<<n<<", "
      <<setw(6)<<setprecision(4)<<get_nominal_wind_speed_in_mps()<<", "
      <<setw(6)<<setprecision(4)<<get_gear_efficiency()<<", "
      <<setw(6)<<setprecision(4)<<get_nominal_air_density_in_kgpm3()<<", "
      <<setw(6)<<setprecision(4)<<get_min_steady_state_turbine_speed_in_pu()<<", "
      <<setw(6)<<setprecision(4)<<get_max_steady_state_turbine_speed_in_pu()<<", "
      <<setw(6)<<setprecision(4)<<get_air_density_in_kgpm3()<<", "
      <<setw(6)<<setprecision(4)<<get_C1()<<", "
      <<setw(6)<<setprecision(4)<<get_C2()<<", "
      <<setw(6)<<setprecision(4)<<get_C3()<<", "
      <<setw(6)<<setprecision(4)<<get_C4()<<", "
      <<setw(6)<<setprecision(4)<<get_C5()<<", "
      <<setw(6)<<setprecision(4)<<get_C6()<<" /";

    return osstream.str();
}

size_t AERD0::get_variable_index_from_variable_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    show_information_with_leading_time_stamp(osstream);
    return 0;
}

string AERD0::get_variable_name_from_variable_index(size_t var_index)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_index is provided: "<<var_index;
    show_information_with_leading_time_stamp(osstream);
    return "";
}

double AERD0::get_variable_with_index(size_t var_index)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var_index is provided: "<<var_index;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

double AERD0::get_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string AERD0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string AERD0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string AERD0::get_dynamic_data_in_steps_format() const
{
    return "";
}
