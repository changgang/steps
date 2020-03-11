#include "header/model/wtg_models/wt_aerodynamic_model/aerdf.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <istream>
#include <iostream>

using namespace std;

AERDF::AERDF(STEPS& toolkit) : WT_AERODYNAMIC_MODEL(toolkit)
{
    cp_file_name = new string;
    pitch_angles = new vector<double>;
    tip_speed_ratios = new vector<double>;
    Cp_Matrix = new vector<vector<double> >;
    clear();
}

AERDF::AERDF(const AERDF& model):WT_AERODYNAMIC_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

AERDF::~AERDF()
{
    delete cp_file_name;
    delete pitch_angles;
    delete tip_speed_ratios;
    delete Cp_Matrix;
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
    set_toolkit(model.get_toolkit());

    clear();
    WT_AERODYNAMIC_MODEL::copy_from_const_model(model);
    set_Cp_file(model.get_Cp_file());
}

void AERDF::set_Cp_file(string file)
{
    STEPS& toolkit = get_toolkit();
    (*cp_file_name) = file;
    load_data_from_Cp_file();
    if(pitch_angles->size()==0)
    {
        ostringstream osstream;
        osstream<<"Error. Fail to load wind turbine Cp data from file '"<<file<<"'. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        (*cp_file_name) = "";
        return;
    }
}

string AERDF::get_Cp_file() const
{
    return (*cp_file_name);
}

void AERDF::load_data_from_Cp_file()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    if(cp_file_name->size()<1)
    {
        osstream<<"Initialization error. No file is provided for loading wind turbine Cp data. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }
    ifstream fid((*cp_file_name));
    if(fid.is_open())
        fid.close();
    else
    {
        osstream<<"Initialization error. Fail to open wind turbine Cp data file '"<<(*cp_file_name)<<"'. Check model "<<get_model_name()<<" of "<<get_device_name();
        toolkit.show_information_with_leading_time_stamp(osstream);
        return;
    }

    load_pitch_angles();
    load_tip_speed_ratios();
    load_Cp_matrix();
}

void AERDF::load_pitch_angles()
{
    pitch_angles->clear();

    ifstream fid((*cp_file_name));

    string data;
    vector<string> datavec;
    getline(fid, data); // skip the head line
    getline(fid, data); // get the first valid line of pitch angles

    data = trim_string(data);
    data = string2csv(data);
    datavec = split_string(data,",");

    unsigned int n = datavec.size();
    if(n<2)
        return;

    for(unsigned int i=1; i<n; ++i)
    {
        double pitch = get_double_data(datavec.front(),"0.0");
        datavec.erase(datavec.begin());
        pitch_angles->push_back(pitch);
    }

    fid.close();
}

void AERDF::load_tip_speed_ratios()
{
    STEPS& toolkit = get_toolkit();
    tip_speed_ratios->clear();

    ifstream fid((*cp_file_name));
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

        unsigned int n = datavec.size();
        if(n==0)
        {
            osstream<<"Warning. No more tip speed ratio lines in wind turbine Cp data file '"<<(*cp_file_name)<<"' of "<<get_model_name()<<" of "<<get_device_name()<<".";
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }
        double tsr = get_double_data(datavec.front(),"0.0");
        tip_speed_ratios->push_back(tsr);
    }
    fid.close();
}

void AERDF::load_Cp_matrix()
{
    STEPS& toolkit = get_toolkit();
    Cp_Matrix->clear();

    ifstream fid((*cp_file_name));
    ostringstream osstream;

    string data;
    vector<string> datavec;
    getline(fid, data); // skip the head line
    getline(fid, data); // get the first valid line of pitch angles

    unsigned int N_pitch = pitch_angles->size();
    vector<double> cp_line;
    while(true)
    {
        getline(fid, data);
        if(data.size()<2)
            break;
        data = trim_string(data);
        data = string2csv(data);
        datavec = split_string(data,",");

        unsigned int n = datavec.size();
        if(n<N_pitch+1)
        {
            osstream<<"Warning. Different length of tip speed ratio line is detected in wind turbine Cp data file '"<<(*cp_file_name)<<"' of "<<get_model_name()<<" of "<<get_device_name()<<":"<<endl
                    <<data<<endl
                    <<"No more data will be loaded from Cp data file '"<<(*cp_file_name)<<"'.";
            toolkit.show_information_with_leading_time_stamp(osstream);
            break;
        }

        cp_line.clear();
        for(unsigned int i=1; i<N_pitch+1; ++i)
        {
            double cp = get_double_data(datavec.front(),"0.0");
            datavec.erase(datavec.begin());
            cp_line.push_back(cp);
        }
        Cp_Matrix->push_back(cp_line);
    }

    fid.close();
}


double AERDF::get_Cp(double lambda, double pitch_deg) const
{
    return 0.0;
}

bool AERDF::is_Cp_point_exist(double pitch, double lambda)
{
    unsigned int N_pitch = pitch_angles->size();
    bool pitch_found = false;
    for(unsigned int i=0; i<N_pitch; ++i)
    {
        if(fabs((*pitch_angles)[i]-pitch)<DOUBLE_EPSILON)
        {
            pitch_found = true;
            break;
        }
        if((*pitch_angles)[i]>pitch)
            break;
    }
    if(pitch_found==false)
        return false;

    unsigned int N_lambda = tip_speed_ratios->size();
    bool lambda_found = false;
    for(unsigned int i=0; i<N_lambda; ++i)
    {
        if(fabs((*tip_speed_ratios)[i]-lambda)<DOUBLE_EPSILON)
        {
            lambda_found = true;
            break;
        }
        if((*tip_speed_ratios)[i]>lambda)
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

bool AERDF::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=11)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            int speed_mode_flag=0;
            unsigned int n;
            double vwind0, gear_eta, rou0_air, min_speed, max_speed;
            double rou_air;
            string file;

            unsigned int i=3;
            speed_mode_flag = get_integer_data(data[i],"0"); i++;
            n = (unsigned int)(get_integer_data(data[i],"1")); i++;
            vwind0 = get_double_data(data[i],"0.0"); i++;
            gear_eta = get_double_data(data[i],"0.0"); i++;
            rou0_air = get_double_data(data[i],"0.0"); i++;
            min_speed = get_double_data(data[i],"0.6"); i++;
            max_speed = get_double_data(data[i],"1.2"); i++;
            rou_air = get_double_data(data[i],"0.0"); i++;
            file = get_string_data(data[i],"");

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
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool AERDF::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool AERDF::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return true;
}


void AERDF::check()
{
    ostringstream osstream;
}

void AERDF::clear()
{
    (*cp_file_name) = "";
    pitch_angles->clear();
    tip_speed_ratios->clear();
    Cp_Matrix->clear();
}

void AERDF::report()
{
    ;
}

void AERDF::save()
{
    ;
}

string AERDF::get_standard_psse_string() const
{
    ostringstream osstream;
    WT_GENERATOR* gen = get_wt_generator_pointer();
    unsigned int bus = gen->get_generator_bus();
    string identifier = "'"+gen->get_identifier()+"'";

    string model_name = "'"+get_model_name()+"'";

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
    unsigned int n = get_number_of_pole_pairs();

    osstream<<setw(8)<<bus<<", "
            <<setw(10)<<model_name<<", "
            <<setw(6)<<identifier<<", "
            <<setw(8)<<speed_mode_flag<<", "
            <<setw(8)<<n<<", "
            <<setw(6)<<setprecision(4)<<get_nominal_wind_speed_in_mps()<<", "
            <<setw(6)<<setprecision(4)<<get_gear_efficiency()<<", "
            <<setw(6)<<setprecision(4)<<get_nominal_air_density_in_kgpm3()<<", "
            <<setw(6)<<setprecision(4)<<get_min_steady_state_turbine_speed_in_pu()<<", "
            <<setw(6)<<setprecision(4)<<get_max_steady_state_turbine_speed_in_pu()<<", \n"
            <<setw(10)<<""
            <<setw(6)<<setprecision(4)<<get_air_density_in_kgpm3()<<", '"
            <<setw(6)<<setprecision(4)<<get_Cp_file()<<"' /";

    return osstream.str();
}

void AERDF::prepare_model_data_table()
{
    clear_model_data_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("A", i); i++;
}

double AERDF::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);

    if(par_name=="SPEED MODE")
    {
        WTG_TURBINE_SPEED_MODE mode = get_turbine_speed_mode();
        switch(mode)
        {
            case WT_OVERSPEED_MODE:
                return 1;
            case WT_UNDERSPEED_MODE:
                return -1;
            case WT_MPPT_MODE:
            default:
                return 0;
        }
    }

    if(par_name=="POLE PAIR")
        return get_number_of_pole_pairs();

    if(par_name=="NOMINAL WIND SPEED IN M/S")
        return get_nominal_wind_speed_in_mps();

    if(par_name=="GEAR EFFICIENCY")
        return get_gear_efficiency();

    if(par_name=="NOMINAL AIR DENSITY IN KG/M3")
        return get_nominal_air_density_in_kgpm3();

    if(par_name=="MIN TURBINE SPEED IN PU")
        return get_min_steady_state_turbine_speed_in_pu();

    if(par_name=="MAX TURBINE SPEED IN PU")
        return get_max_steady_state_turbine_speed_in_pu();

    if(par_name=="AIR DENSITY IN KG/M3")
        return get_air_density_in_kgpm3();

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void AERDF::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);
    if(par_name=="SPEED MODE")
    {
        int ivalue = int(value);
        WTG_TURBINE_SPEED_MODE mode;
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
        set_turbine_speed_mode(mode);
        return;
    }

    if(par_name=="POLE PAIR")
        return set_number_of_pole_pairs(value);

    if(par_name=="NOMINAL WIND SPEED IN M/S")
        return set_nominal_wind_speed_in_mps(value);

    if(par_name=="GEAR EFFICIENCY")
        return set_gear_efficiency(value);

    if(par_name=="NOMINAL AIR DENSITY IN KG/M3")
        return set_nominal_air_density_in_kgpm3(value);

    if(par_name=="MIN TURBINE SPEED IN PU")
        return set_min_steady_state_turbine_speed_in_pu(value);

    if(par_name=="MAX TURBINE SPEED IN PU")
        return set_max_steady_state_turbine_speed_in_pu(value);

    if(par_name=="AIR DENSITY IN KG/M3")
        return set_air_density_in_kgpm3(value);

    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_device_name(), get_model_name(), __FUNCTION__, par_name);
    return;
}

double AERDF::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;
}

void AERDF::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    unsigned int i=0;
    add_model_data_name_and_index_pair("SPEED MODE", i); i++;
    add_model_data_name_and_index_pair("POLE PAIR", i); i++;
    add_model_data_name_and_index_pair("NOMINAL WIND SPEED IN M/S", i); i++;
    add_model_data_name_and_index_pair("GEAR EFFICIENCY", i); i++;
    add_model_data_name_and_index_pair("NOMINAL AIR DENSITY IN KG/M3", i); i++;
    add_model_data_name_and_index_pair("MIN TURBINE SPEED IN PU", i); i++;
    add_model_data_name_and_index_pair("MAX TURBINE SPEED IN PU", i); i++;
    add_model_data_name_and_index_pair("AIR DENSITY IN KG/M3", i); i++;
}

double AERDF::get_model_internal_variable_with_name(string var_name)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() has not been implemented. Input var name is provided: "<<var_name;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
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
