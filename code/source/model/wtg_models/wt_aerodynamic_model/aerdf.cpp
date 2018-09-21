#include "header/model/wtg_models/wt_aerodynamic_model/aerdf.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

AERDF::AERDF()
{
    clear();
    prepare_model_variable_table();
}

AERDF::AERDF(const AERDF& model):WT_AERODYNAMIC_MODEL()
{
    copy_from_const_model(model);
}

AERDF::~AERDF()
{
    ;
}

AERDF& AERDF::operator=(const AERDF& model)
{
    if(this==(&model))
        return *this;
    copy_from_const_model(model);
    return *this;
}

void AERDF::copy_from_const_model(const AERDF& model)
{
    clear();
    WT_AERODYNAMIC_MODEL::copy_from_const_model(model);
    set_Cp_file(model.get_Cp_file());
}

void AERDF::set_Cp_file(string file)
{
    cp_file_name = file;
    load_data_from_Cp_file();
    if(pitch_angles.size()==0)
    {
        ostringstream osstream;
        osstream<<"Error. Fail to load wind turbine Cp data from file '"<<file<<"'. Check model "<<get_model_name()<<" of "<<get_device_name();
        show_information_with_leading_time_stamp(osstream);
        cp_file_name = "";
        return;
    }
}

string AERDF::get_Cp_file() const
{
    return cp_file_name;
}

void AERDF::load_data_from_Cp_file()
{
    ostringstream oosstream;
    if(cp_file_name.size()<1)
    {
        oosstream<<"Initialization error. No file is provided for loading wind turbine Cp data. Check model "<<get_model_name()<<" of "<<get_device_name();
        show_information_with_leading_time_stamp(oosstream);
        return;
    }
    ifstream fid(cp_file_name);
    if(not fid.is_open())
    {
        oosstream<<"Initialization error. Fail to open wind turbine Cp data file '"<<cp_file_name<<"'. Check model "<<get_model_name()<<" of "<<get_device_name();
        show_information_with_leading_time_stamp(oosstream);
        return;
    }
    else
        fid.close();

    load_pitch_angles();
    load_tip_speed_ratios();
    load_Cp_matrix();
}

void AERDF::load_pitch_angles()
{
    pitch_angles.clear();

    ifstream fid(cp_file_name);

    string data;
    vector<string> datavec;
    getline(fid, data); // skip the head line
    getline(fid, data); // get the first valid line of pitch angles

    data = trim_string(data);
    data = string2csv(data);
    datavec = split_string(data,",");

    size_t n = datavec.size();
    if(n<2)
        return;

    for(size_t i=1; i<n; ++i)
    {
        double pitch = get_double_data(datavec.front(),"0.0");
        datavec.erase(datavec.begin());
        pitch_angles.push_back(pitch);
    }

    fid.close();
}

void AERDF::load_tip_speed_ratios()
{
    tip_speed_ratios.clear();

    ifstream fid(cp_file_name);
    ostringstream osstream;

    string data;
    vector<string> datavec;
    getline(fid, data); // skip the head line
    getline(fid, data); // get the first valid line of pitch angles

    while(true)
    {
        getline(fid, data);
        if(data.size()<2)
            break;
        data = trim_string(data);
        data = string2csv(data);
        datavec = split_string(data,",");

        size_t n = datavec.size();
        if(n==0)
        {
            osstream<<"Warning. No more tip speed ratio lines in wind turbine Cp data file '"<<cp_file_name<<"' of "<<get_model_name()<<" of "<<get_device_name()<<".";
            show_information_with_leading_time_stamp(osstream);
            break;
        }
        double tsr = get_double_data(datavec.front(),"0.0");
        tip_speed_ratios.push_back(tsr);
    }
    fid.close();
}

void AERDF::load_Cp_matrix()
{
    Cp_Matrix.clear();

    ifstream fid(cp_file_name);
    ostringstream osstream;

    string data;
    vector<string> datavec;
    getline(fid, data); // skip the head line
    getline(fid, data); // get the first valid line of pitch angles

    size_t N_pitch = pitch_angles.size();
    vector<double> cp_line;
    while(true)
    {
        getline(fid, data);
        if(data.size()<2)
            break;
        data = trim_string(data);
        data = string2csv(data);
        datavec = split_string(data,",");

        size_t n = datavec.size();
        if(n<N_pitch+1)
        {
            osstream<<"Warning. Different length of tip speed ratio line is detected in wind turbine Cp data file '"<<cp_file_name<<"' of "<<get_model_name()<<" of "<<get_device_name()<<":"<<endl
                    <<data<<endl
                    <<"No more data will be loaded from Cp data file '"<<cp_file_name<<"'.";
            show_information_with_leading_time_stamp(osstream);
            break;
        }

        cp_line.clear();
        for(size_t i=1; i<N_pitch+1; ++i)
        {
            double cp = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());
            cp_line.push_back(cp);
        }
        Cp_Matrix.push_back(cp_line);
    }

    fid.close();
}


double AERDF::get_Cp(double lambda, double pitch_deg) const
{
    return 0.0;
}

bool AERDF::is_Cp_point_exist(double pitch, double lambda)
{
    size_t N_pitch = pitch_angles.size();
    bool pitch_found = false;
    for(size_t i=0; i<N_pitch; ++i)
    {
        if(fabs(pitch_angles[i]-pitch)<FLOAT_EPSILON)
        {
            pitch_found = true;
            break;
        }
        if(pitch_angles[i]>pitch)
            break;
    }
    if(pitch_found==false)
        return false;

    size_t N_lambda = tip_speed_ratios.size();
    bool lambda_found = false;
    for(size_t i=0; i<N_lambda; ++i)
    {
        if(fabs(tip_speed_ratios[i]-lambda)<FLOAT_EPSILON)
        {
            lambda_found = true;
            break;
        }
        if(tip_speed_ratios[i]>lambda)
            break;
    }
    if(lambda_found==false)
        return false;

    return true;
}

double AERDF::get_derivative_of_Cp_over_lambda(double lambda, double pitch_deg) const
{
    double Cp0 = get_Cp(lambda, pitch_deg);
    double Cp1 = get_Cp(lambda+1e-4, pitch_deg);

    //cout<<lambda<<", "<<pitch_deg<<", "<<Cp0<<", "<<Cp1<<endl;

    return (Cp1-Cp0)*1e4;
}


string AERDF::get_model_name() const
{
    return "AERDF";
}

double AERDF::get_model_data_with_index(size_t index) const
{
    switch(index)
    {
        case 1:
            return get_turbine_speed_mode();
        case 2:
            return get_number_of_pole_pairs();
        case 3:
            return get_nominal_wind_speed_in_mps();
        case 4:
            return get_gear_efficiency();
        case 5:
            return get_nominal_air_density_in_kgpm3();
        case 6:
            return get_min_steady_state_turbine_speed_in_pu();
        case 7:
            return get_max_steady_state_turbine_speed_in_pu();
        case 8:
            return get_air_density_in_kgpm3();
        default:
        {
            show_set_get_model_data_with_index_error(get_device_name(), get_model_name(), __FUNCTION__, index);
            return 0.0;
        }
    }
}

double AERDF::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    size_t index = 1;
    if(par_name=="SPEED MODE")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="POLE PAIR")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="NOMINAL WIND SPEED IN M/S")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="GEAR EFFICIENCY")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="NOMINAL AIR DENSITY IN KG/M3")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="MIN TURBINE SPEED IN PU")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="MAX TURBINE SPEED IN PU")
        return get_model_data_with_index(index);

    index++;
    if(par_name=="AIR DENSITY IN KG/M3")
        return get_model_data_with_index(index);

    show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void AERDF::set_model_data_with_index(size_t index, double value)
{
    switch(index)
    {
        case 1:
        {
            int ivalue = int(value);
            WTG_TURBINE_SPEED_MODE mode = WT_MPPT_MODE;
            switch(ivalue)
            {
                case 1:
                    mode = WT_OVERSPEED_MODE;
                    break;
                case -1:
                    mode = WT_UNDERSPEED_MODE;
                    break;
                case 0:
                default:
                    mode = WT_MPPT_MODE;
                    break;
            }
            return set_turbine_speed_mode(mode);
        }
        case 2:
            return set_number_of_pole_pairs(size_t(value));
        case 3:
            return set_nominal_wind_speed_in_mps(value);
        case 4:
            return set_gear_efficiency(value);
        case 5:
            return set_nominal_air_density_in_kgpm3(value);
        case 6:
            return set_min_steady_state_turbine_speed_in_pu(value);
        case 7:
            return set_max_steady_state_turbine_speed_in_pu(value);
        case 8:
            return set_air_density_in_kgpm3(value);
        default:
        {
            show_set_get_model_data_with_index_error(get_device_name(), get_model_name(), __FUNCTION__, index);
            return;
        }
    }
}

void AERDF::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    size_t index = 1;
    if(par_name=="SPEED MODE")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="POLE PAIR")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="NOMINAL WIND SPEED IN M/S")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="GEAR EFFICIENCY")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="NOMINAL AIR DENSITY IN KG/M3")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="MIN TURBINE SPEED IN PU")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="MAX TURBINE SPEED IN PU")
        return set_model_data_with_index(index, value);

    index++;
    if(par_name=="AIR DENSITY IN KG/M3")
        return set_model_data_with_index(index, value);

    show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

bool AERDF::setup_model_with_steps_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return true;
}

bool AERDF::setup_model_with_psse_string(string data)
{
    bool is_successful = false;
    vector<string> dyrdata = split_string(data,",");
    if(dyrdata.size()<11)
        return is_successful;

    string model_name = get_string_data(dyrdata[1],"");
    if(model_name!=get_model_name())
        return is_successful;

    int speed_mode_flag=0;
    size_t n;
    double vwind0, gear_eta, rou0_air, min_speed, max_speed;
    double rou_air;
    string file;

    size_t i=3;
    speed_mode_flag = get_integer_data(dyrdata[i],"0"); i++;
    n = size_t(get_integer_data(dyrdata[i],"1")); i++;
    vwind0 = get_double_data(dyrdata[i],"0.0"); i++;
    gear_eta = get_double_data(dyrdata[i],"0.0"); i++;
    rou0_air = get_double_data(dyrdata[i],"0.0"); i++;
    min_speed = get_double_data(dyrdata[i],"0.6"); i++;
    max_speed = get_double_data(dyrdata[i],"1.2"); i++;
    rou_air = get_double_data(dyrdata[i],"0.0"); i++;
    file = get_string_data(dyrdata[i],"");

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
    set_Cp_file(file);

    is_successful = true;

    return is_successful;
}

bool AERDF::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    show_information_with_leading_time_stamp(osstream);
    return true;
}


void AERDF::check()
{
    ostringstream oosstream;
}

void AERDF::clear()
{
    cp_file_name = "";
    pitch_angles.clear();
    tip_speed_ratios.clear();
    Cp_Matrix.clear();
}

void AERDF::report()
{
    ;
}

void AERDF::save()
{
    ;
}

string AERDF::get_standard_model_string() const
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
      <<setw(6)<<setprecision(4)<<get_air_density_in_kgpm3()<<", '"
      <<setw(6)<<setprecision(4)<<get_Cp_file()<<"' /";

    return osstream.str();
}

void AERDF::prepare_model_variable_table()
{
    size_t i=0;
    add_model_variable_name_and_index_pair("", i); i++;
}

double AERDF::get_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    show_information_with_leading_time_stamp(osstream);
    return 0.0;
}

string AERDF::get_dynamic_data_in_psse_format() const
{
    return "";
}

string AERDF::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string AERDF::get_dynamic_data_in_steps_format() const
{
    return "";
}
