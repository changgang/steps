#include "load_dynamic_data.h"
#include "utilities.h"
#include "tesla_namespace.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace POWERSYSTEM;
using namespace DYNAMICS;

int load_dynamic_data();
int load_dynamic_data_PSSE(const string file, int ver, int subver)
{
    // for PSS/E, only bus number based raw file is supported
    // date: May 2, 2016

    // check file existence
    if(not is_file_exist(file)) return 1; // the raw file doesn't exist

    vector <string> fileInRAM;
    load_dynamic_data_to_RAM(file,fileInRAM); // load file into the RAM

    //for(size_t i=0; i<fileInRAM.size(); i++) cout<<fileInRAM[i]<<endl;

    string str;
    vector<string> data;
    string strval;
    string modelname;
    MODELTYPE modeltype;
    map<string,MODELTABLE>::iterator it;
    while(fileInRAM.size()>0)
    {

        data.clear();
        strval.clear();
        // read one line of string
        str = fileInRAM[0];
        fileInRAM.erase(fileInRAM.begin()); // erase the read line from the fileInRAM
        format_string_with_comma(str); // format it
        if(str.size()==0) break; // no more bus

        split_string(str,",",data); // split string

        modelname = data[1];
        it = ModelTable.find(modelname);
        if(it != ModelTable.end()) // model found
        {
            modeltype = it->second.get_model_type();
        }
        else
        {
            modeltype=MODEL_TYPE_VOID;
        }
        switch(modeltype)
        {
            case MODEL_TYPE_GENERATOR:
            {
                load_dynamic_data_PSSE_Generator_related(data);
                break;
            }
            case MODEL_TYPE_COMPENSATOR:
            case MODEL_TYPE_STABILIZER:
            case MODEL_TYPE_EXCITER:
            case MODEL_TYPE_UNDER_EXCITATION_LIMITER:
            case MODEL_TYPE_OVER_EXCITATION_LIMITER:
            case MODEL_TYPE_TURBINE_GOVERNOR:
            case MODEL_TYPE_CROSS_COMPOUND_TURBINE_GOVERNOR:
            case MODEL_TYPE_TURBINE_LOAD_CONTROLLER:
            case MODEL_TYPE_WIND_GENERATOR:
            case MODEL_TYPE_WIND_ELECTRICAL_CONTROLLER:
            case MODEL_TYPE_WIND_TURBINE:
            case MODEL_TYPE_WIND_PITCH_CONTROLLER:
            case MODEL_TYPE_WIND_AERODYNAMIC:
            case MODEL_TYPE_WIND_GUST:
            case MODEL_TYPE_WIND_AUXILIARY_CONTROLLER:
            case MODEL_TYPE_MACHINE_RELAY:
            case MODEL_TYPE_LOAD_CHARACTERISTICS:
            case MODEL_TYPE_LOAD_RELAY:
            {
                load_dynamic_data_PSSE_Load_related(data);
                break;
            }
            case MODEL_TYPE_LINE_RELAY:
            {
                load_dynamic_data_PSSE_Line_related(data);
                break;
            }
            case MODEL_TYPE_HVDC:
            {
                load_dynamic_data_PSSE_HVDC_related(data);
                break;
            }
            case MODEL_TYPE_VOID:
            default:
            {
                break;
            }
        }
    }
    for(size_t i=0;i<CON.size(); i++)
        cout<<"CON["<<i<<"]: "<<CON[i]<<endl;
    return 0;
}
int load_dynamic_data_BPA(const string file, int ver, int subver);


int load_dynamic_data_PSSE_Generator_related(vector<string> & data)
{
    // function to load generator dynamic data
    // Sep. 13, 2016

    string modelname = data[1];
    map<string,MODELTABLE>::iterator it = ModelTable.find(modelname);
    if(it==ModelTable.end()) return 1;

    int nICON  = it->second.get_model_N_ICON();
    int nCON   = it->second.get_model_N_CON();
    int nSTATE = it->second.get_model_N_STATE();
    int nVAR   = it->second.get_model_N_VAR();

    if(int(data.size()) != (3+nICON+nCON)) return 2;

    int n = 0;
    vector<string> icon;
    vector<double> con;

    int ibus  = get_integer_data(data[n],"0");  n+=2;
    string id = get_string_data(data[n],""); n++;

    int genindex = get_generator_index(ibus, id);
    if(genindex<0) return 3;

    int error = Generator[genindex].set_dynamic_model(modelname, ICON.size(), CON.size(), STATE.size(), VAR.size());
    if(error!=0) return error;

    int m;
    m = 0;
    while(m<nICON)
    {
        icon.push_back(get_string_data(data[n],"")); n++; m++;
    }
    m = 0;
    while(m<nCON)
    {
        con.push_back(get_double_data(data[n],"0.0")); n++; m++;
    }
    for(m=0; m<nICON; m++)
    {
        ICON.push_back(icon[m]);
        ICON_DESC.push_back("");
    }
    for(m=0; m< nCON; m++)
    {
        CON.push_back( con[m]);
        CON_DESC.push_back("");
    }
    for(m=0; m<nSTATE; m++)
    {
        STATE.push_back(0.0);
        STORE.push_back(0.0);
        newSTATE.push_back(0.0);
        STATE_DESC.push_back("");
    }
    for(m=0; m< nVAR; m++)
    {
        VAR.push_back(0.0);
        VAR_DESC.push_back("");
    }
    return 0;
}

int load_dynamic_data_PSSE_Load_related(vector<string> & data)
{
    return 0;
}
int load_dynamic_data_PSSE_Line_related(vector<string> & data)
{
    return 0;
}
int load_dynamic_data_PSSE_HVDC_related(vector<string> & data)
{
    return 0;
}
