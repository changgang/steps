#include "load_network_data.h"
#include "utilities.h"
#include "tesla_namespace.h"
#include "device_index_map.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace POWERSYSTEM;

int load_network_data_PSSE(const string file, int ver, int subver)
{
    // for PSS/E, only bus number based raw file is supported
    // date: May 2, 2016

    // check file existence
    if(not is_file_exist(file)) return 1; // the raw file doesn't exist

    vector <string> fileInRAM;
    load_network_data_to_RAM(file,fileInRAM); // load file into the RAM

    int error=0;
    error = load_network_data_PSSE_Case(fileInRAM); if(error) return 1;
    error = load_network_data_PSSE_Bus(fileInRAM); if(error) return 1;
    set_bus_index_map();
    error = load_network_data_PSSE_Load(fileInRAM); if(error) return 1;
    set_load_index_map();
    error = load_network_data_PSSE_FixedShunt(fileInRAM); if(error) return 1;
    set_fixed_shunt_index_map();
    error = load_network_data_PSSE_Generator(fileInRAM); if(error) return 1;
    set_generator_index_map();
    error = load_network_data_PSSE_Line(fileInRAM); if(error) return 1;
    set_line_index_map();
    error = load_network_data_PSSE_Transformer(fileInRAM); if(error) return 1;
    set_transformer_index_map();
    error = load_network_data_PSSE_Area(fileInRAM); if(error) return 1;
    error = load_network_data_PSSE_HVDC(fileInRAM); if(error) return 1;
    /*load_network_data_PSSE_VSCHVDC(fileInRAM);
    load_network_data_PSSE_TransZCorrectionTab(fileInRAM);
    load_network_data_PSSE_Zone(fileInRAM);
    */

    cout<<"Number of devices imported:"<<endl;
    cout<<"  Bus: "<<Bus.size()<<endl;
    cout<<"  Load: "<<Load.size()<<endl;
    cout<<"  Generator: "<<Generator.size()<<endl;
    cout<<"  Line: "<<Line.size()<<endl;
    cout<<"  Transformer: "<<Transformer.size()<<endl;
    cout<<"  HVDC: "<<Hvdc.size()<<endl;
    cout<<"  Fixed shunt: "<<FixedShunt.size()<<endl;
    cout<<"  Area: "<<Area.size()<<endl;
    return 0;

}

int load_network_data_PSSE_Case(vector <string> & fileInRAM)
{
    // load case information
    // date: May 3, 2016

    string dataformat="int,double,int,int,int,double";  // data format

    int ver = 31;

    cout<<"now read case data"<<endl;

    // get one line of data, and remove it from the fileInRAM variable.
    string str = fileInRAM[0];
    fileInRAM.erase(fileInRAM.begin());
    format_string_with_comma(str); // format the string

    int error = check_data_format(str, dataformat); // check data format
    if(error !=0) return 1; // if invalid data is provided

    vector<string> data;
    split_string(str,",",data); // split the string
    size_t ncount = data.size();

    string strval;
    size_t  n=0;
    if(ncount>n)
    {
        int appendCode = get_integer_data(data[n],"0");
        //if(appendCode==0) reset_simulator();
    }
    n++;
    if(ncount>n) set_system_base_power_in_MVA(get_double_data(data[n],"100.0"));
    n++;
    if(ncount>n) set_data_version(get_integer_data(data[n],"31"));
    n++;
    if(ncount>n) TransformerRatingUnit = get_integer_data(data[n],"0");
    n++;
    if(ncount>n) LineRatingUnit = get_integer_data(data[n],"0");
    n++;
    if(ncount>n) FBASE = get_double_data(data[n],"50.0");

    F_INIT = FBASE;

    // read case info string
    str = fileInRAM[0];
    fileInRAM.erase(fileInRAM.begin());
    CaseInfo=str;

    str = fileInRAM[0];
    fileInRAM.erase(fileInRAM.begin());
    CaseInfo+="\n";
    CaseInfo+=str;

    return 0;

}
int load_network_data_PSSE_Bus(vector <string> & fileInRAM)
{
    cout<<"now read bus data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    string dataformat="int,string,double,int,int,int,double,double,double,double,double,double"; // format
    string busname;
    while(true)
    {
        BUS bus;

        data.clear();
        strval.clear();
        // read one line of string
        str = fileInRAM[0];
        fileInRAM.erase(fileInRAM.begin()); // erase the read line from the fileInRAM
        format_string_with_comma(str); // format it
        if(str=="0") break; // no more bus

        int error = check_data_format(str, dataformat); // check format
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data); // split string
        ncount = data.size();

        n=0;
        if(ncount>n) bus.set_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n)
        {
            busname = get_string_data(data[n],"");
            trim_leading_spaces(busname);
            trim_tailing_spaces(busname);
            bus.set_bus_name(busname);
        }
        n++;
        if(ncount>n) bus.set_base_voltage_kV(get_double_data(data[n],"100.0"));
        n++;
        if(ncount>n)
        {
            int bt = get_integer_data(data[n],"1");
            if(bt==1) bus.set_bus_type(BUSTYPE_PQ);
            if(abs(bt)==2) bus.set_bus_type(BUSTYPE_PV);
            if(bt==3) bus.set_bus_type(BUSTYPE_SLACK);
            if(bt==4) bus.set_bus_type(BUSTYPE_OUT);
        }
        n++;
        if(ncount>n) bus.set_area_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) bus.set_zone_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) bus.set_owner_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) bus.set_voltage_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) bus.set_angle_deg(get_double_data(data[n],"0.0"));

        append_bus(bus);
    }
    cout<<Bus.size()<<" buses are imported"<<endl;
    return;
}
int load_network_data_PSSE_Load(vector <string> & fileInRAM)
{
    cout<<"now read load data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    string dataformat="int,string,int,int,int,double,double,double,double,double,double,int"; // format

    while(true)
    {
        LOAD load;

        data.clear();
        strval.clear();

        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it
        if(str=="0") break;// no more load

        int error = check_data_format(str, dataformat);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data);
        ncount = data.size();

        n=0;
        if(ncount>n) load.set_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) load.set_id(get_string_data(data[n],""));
        n++;
        if(ncount>n) load.set_status(get_integer_data(data[n],"1"));
        n++;
        if(ncount>n) load.set_area_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) load.set_zone_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) load.set_nominal_constant_power_P_MW(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) load.set_nominal_constant_power_Q_MVar(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) load.set_nominal_constant_current_P_MW(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) load.set_nominal_constant_current_Q_MVar(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) load.set_nominal_constant_impedance_P_MW(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) load.set_nominal_constant_impedance_Q_MVar(-get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) load.set_owner_number(get_integer_data(data[n],"0"));

        Load.push_back(load);
    }
    set_load_index_map();
    //list
    cout<<Load.size()<<" loads are imported"<<endl;
    return 0;
}
int load_network_data_PSSE_FixedShunt(vector <string> & fileInRAM)
{
    cout<<"now read fixed shunt data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    string dataformat="int,string,int,double,double"; // format

    while(true)
    {
        FIXEDSHUNT shunt;

        data.clear();
        strval.clear();

        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it
        if(str=="0") break;// no more shunt

        int error = check_data_format(str, dataformat);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data);
        ncount = data.size();

        n=0;
        if(ncount>n) shunt.set_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) shunt.set_id(get_string_data(data[n],""));
        n++;
        if(ncount>n) shunt.set_status(get_integer_data(data[n],"1"));
        n++;
        if(ncount>n) shunt.set_nominal_shunt_P_MW(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) shunt.set_nominal_shunt_Q_MVar(get_double_data(data[n],"0.0"));

        FixedShunt.push_back(shunt);
    }
    //list
    cout<<FixedShunt.size()<<" fixed shunts are imported"<<endl;
    return 0;

}
int load_network_data_PSSE_Generator(vector <string> & fileInRAM)
{
    cout<<"now read generator data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    //                 bus  id    pgen  qgen     qmax  qmin   vreg   regbus
    string dataformat="int,string,double,double,double,double,double,int,";
    //                 mbase   zr    zx     tr     tx     tap    status rmp pmax   pmin
    dataformat      +="double,double,double,double,double,double,int,double,double,double,";
    //                 own  frac  own frac   own frac   own frac   wmod wpf
    dataformat      +="int,double,int,double,int,double,int,double,int,double"; // format

    while(true)
    {
        GENERATOR generator;

        data.clear();
        strval.clear();

        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it
        if(str=="0") break;// no more generator

        int error = check_data_format(str, dataformat);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data);
        ncount = data.size();

        n=0;
        if(ncount>n) generator.set_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) generator.set_id(get_string_data(data[n],""));
        n++;
        if(ncount>n) generator.set_P_generation_MW(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) generator.set_Q_generation_MVar(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) generator.set_Q_generation_max_MVar(get_double_data(data[n],"999.0"));
        n++;
        if(ncount>n) generator.set_Q_generation_min_MVar(get_double_data(data[n],"-999.0"));
        n++;
        if(ncount>n) generator.set_regulate_voltage_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) generator.set_regulated_bus(get_integer_data(data[n],"0"));
        n++;
        char mbase[100]; sprintf(mbase,"%lf",MVABASE);
        if(ncount>n) generator.set_MBASE_MVA(get_double_data(data[n],mbase));
        n++;
        if(ncount>n) generator.set_generator_resistance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) generator.set_generator_reactance_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) generator.set_unit_transformer_resistance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) generator.set_unit_transformer_reactance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) generator.set_unit_transformer_turn_ratio_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) generator.set_status(get_integer_data(data[n],"1"));
        n++;
        if(ncount>n) generator.set_remote_control_percentage(get_double_data(data[n],"100.0"));
        n++;
        if(ncount>n) generator.set_P_generation_max_MW(get_double_data(data[n],"999.0"));
        n++;
        if(ncount>n) generator.set_P_generation_min_MW(get_double_data(data[n],"-999.0"));
        n++;
        int owner; double frac;
        for(int i=0; i<4; i++)
        {
            if(ncount>n) owner = get_integer_data(data[n],"0");
            n++;
            if(ncount>n)
            {
                frac = get_double_data(data[n],"0.0");
                generator.set_owner(owner,frac);
                owner = 0;
                frac = 0.0;
            }
            n++;
        }
        if(ncount>n) generator.set_wind_var_control_mode(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) generator.set_wind_power_factor(get_double_data(data[n],"1.0"));

        Generator.push_back(generator);
    }
    //list
    cout<<Generator.size()<<" generators are imported"<<endl;
    return 0;
}
int load_network_data_PSSE_Line(vector <string> & fileInRAM)
{
    cout<<"now read line data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    //                 bus  bus id    R      X     B       ratea   rateb ratec
    string dataformat="int,int,string,double,double,double,double,double,double,";
    //                 GI     BI     GJ     BJ     sta  met len
    dataformat      +="double,double,double,double,int,int,double,";
    //                 own  frac  own frac   own frac   own frac
    dataformat      +="int,double,int,double,int,double,int,double"; // format

    while(true)
    {
        LINE line;

        data.clear();
        strval.clear();

        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it
        if(str=="0") break;// no more line

        int error = check_data_format(str, dataformat);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data);
        ncount = data.size();

        n=0;
        if(ncount>n) line.set_I_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) line.set_J_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) line.set_id(get_string_data(data[n],""));
        n++;
        if(ncount>n) line.set_resistance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_reactance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_susceptance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_rateA_MVA(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_rateB_MVA(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_rateC_MVA(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_I_shunt_conductance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_I_shunt_susceptance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_J_shunt_conductance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_J_shunt_susceptance_pu(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) line.set_status(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) line.set_meter_end(get_integer_data(data[n],"1"));
        n++;
        if(ncount>n) line.set_length_km(get_double_data(data[n],"0.0"));
        n++;
        int owner; double frac;
        for(int i=0; i<4; i++)
        {
            if(ncount>n) owner = get_integer_data(data[n],"0");
            n++;
            if(ncount>n)
            {
                frac = get_double_data(data[n],"0.0");
                line.set_owner(owner,frac);
                owner = 0;
                frac = 0.0;
            }
            n++;
        }

        Line.push_back(line);
    }
    //list
    cout<<Line.size()<<" lines are imported:"<<endl;
    return 0;
}

int load_network_data_PSSE_Transformer(vector <string> & fileInRAM)
{
    cout<<"now read transformer data"<<endl;

    string str;
    vector<string> data_trans, data_z, data_winding1, data_winding2, data_winding3;
    string strval;
    size_t ncount;
    size_t  n;
    //                       bus bus bus id     CW  CZ  CM  MAG1   MAG2   metr name  status
    string dataformat_trans="int,int,int,string,int,int,int,double,double,int,string,int,";
    //                       own  frac  own frac   own frac   own frac   vector
    dataformat_trans      +="int,double,int,double,int,double,int,double,string"; // format
    //                   R1-2   X1-2   S1-2   R2-3   X2-3   S2-3   R3-1   X3-1   S3-1   Vstar  Anglestar
    string dataformat_z="double,double,double,double,double,double,double,double,double,double,double";
    //                         WindV  NormV    Angle RateA  RateB  RateC  COD CONT RMAX  RMIN   VMAX   VMIN
    string dataformat_winding="double,double,double,double,double,double,int,int,double,double,double,double,";
    //                         NTap Table CR  CX     connection
    dataformat_winding      +="int,int,double,double,double"; // format

    while(true)
    {
        TRANSFORMER trans;

        // read transformer info 1
        data_trans.clear();
        data_z.clear();
        data_winding1.clear();
        data_winding2.clear();
        data_winding3.clear();

        strval.clear();

        // try to read transformer info
        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it
        if(str=="0") break;// no more line

        int error = check_data_format(str, dataformat_trans);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data_trans);

        // try to read transformer Z info
        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it

        error = check_data_format(str, dataformat_z);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data_z);

        // try to read transformer winding 1 info
        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it

        error = check_data_format(str, dataformat_winding);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data_winding1);

        // try to read transformer winding 2 info
        str = fileInRAM[0]; // read one line of data from the fileInRAM
        fileInRAM.erase(fileInRAM.begin()); // erase the read line
        format_string_with_comma(str); // format it

        error = check_data_format(str, dataformat_winding);
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data_winding2);

        if(not (data_trans[2] == "" || data_trans[2]=="0")) // kBus is provided
        {
            // try to read transformer winding 3 info
            str = fileInRAM[0]; // read one line of data from the fileInRAM
            fileInRAM.erase(fileInRAM.begin()); // erase the read line
            format_string_with_comma(str); // format it

            error = check_data_format(str, dataformat_winding);
            if(error !=0) return 1; // if invalid data is provided

            split_string(str,",",data_winding3);
        }

        // now we have all the data for the transformer stored in data_ s

        // first get the transformer information
        ncount = data_trans.size();

        n=0;
        if(ncount>n) trans.set_I_bus_number(get_integer_data(data_trans[n],"0"));
        n++;
        if(ncount>n) trans.set_J_bus_number(get_integer_data(data_trans[n],"0"));
        n++;
        if(ncount>n) trans.set_K_bus_number(get_integer_data(data_trans[n],"0"));
        n++;
        if(ncount>n) trans.set_id(get_string_data(data_trans[n],"1"));
        n++;
        if(ncount>n) trans.set_transformer_winding_code(get_integer_data(data_trans[n],"0"));
        n++;
        if(ncount>n) trans.set_transformer_impedance_code(get_integer_data(data_trans[n],"0"));
        n++;
        if(ncount>n) trans.set_transformer_magnetizing_code(get_integer_data(data_trans[n],"1"));
        n++;
        if(ncount>n) trans.set_Gm_pu(get_double_data(data_trans[n],"0.0"));
        n++;
        if(ncount>n) trans.set_Bm_pu(get_double_data(data_trans[n],"0.0"));
        n++;
        if(ncount>n) trans.set_non_meter_end(get_integer_data(data_trans[n],"2"));
        n++;
        if(ncount>n) trans.set_transformer_name(get_string_data(data_trans[n],""));
        n++;
        if(ncount>n) trans.set_status(get_integer_data(data_trans[n],"1"));
        n++;
        int owner; double frac;
        for(int i=0; i<4; i++)
        {
            if(ncount>n) owner = get_integer_data(data_trans[n],"0");
            n++;
            if(ncount>n)
            {
                frac = get_double_data(data_trans[n],"0.0");
                trans.set_owner(owner,frac);
                owner = 0;
                frac = 0.0;
            }
            n++;
        }
        if(ncount>n) trans.set_vector_group(get_string_data(data_trans[n],"0"));

        // now go reading transformer impedance and capacity data
        ncount = data_z.size();
        n=0;
        if(ncount>n) trans.set_Rij_pu(get_double_data(data_z[n],"0.0"));
        n++;
        if(ncount>n) trans.set_Xij_pu(get_double_data(data_z[n],"0.0"));
        n++;
        if(ncount>n) trans.set_base_Sij_MVA(get_double_data(data_z[n],"0.0"));
        n++;
        if(trans.get_K_bus_number()!=0)
        {
            if(ncount>n) trans.set_Rjk_pu(get_double_data(data_z[n],"0.0"));
            n++;
            if(ncount>n) trans.set_Xjk_pu(get_double_data(data_z[n],"0.0"));
            n++;
            if(ncount>n) trans.set_base_Sjk_MVA(get_double_data(data_z[n],"0.0"));
            n++;
            if(ncount>n) trans.set_Rki_pu(get_double_data(data_z[n],"0.0"));
            n++;
            if(ncount>n) trans.set_Xki_pu(get_double_data(data_z[n],"0.0"));
            n++;
            if(ncount>n) trans.set_base_Ski_MVA(get_double_data(data_z[n],"0.0"));
            n++;
            if(ncount>n) trans.set_star_bus_voltage_pu(get_double_data(data_z[n],"1.0"));
            n++;
            if(ncount>n) trans.set_star_bus_angle_deg(get_double_data(data_z[n],"0.0"));
        }

        // now go reading winding i data
        ncount = data_winding1.size();
        n=0;
        if(ncount>n) trans.set_winding_I_turn_ratio_pu(get_double_data(data_winding1[n],"1.0"));
        n++;
        if(ncount>n) trans.set_winding_I_nominal_voltage_kV(get_double_data(data_winding1[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_I_angle_shift_deg(get_double_data(data_winding1[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_I_rateA_MVA(get_double_data(data_winding1[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_I_rateB_MVA(get_double_data(data_winding1[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_I_rateC_MVA(get_double_data(data_winding1[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_I_control_mode(get_integer_data(data_winding1[n],"0"));
        n++;
        if(ncount>n) trans.set_winding_I_controlled_bus(get_integer_data(data_winding1[n],"0"));
        n++;
        switch(abs(trans.get_winding_I_control_mode()))
        {
            case 0:// fixed tap and fixed angle shift
            case 1:// voltage control
            case 2:// Q control
            {
                if(ncount>n) trans.set_winding_I_turn_ratio_max_pu(get_double_data(data_winding1[n],"1.1"));
                n++;
                if(ncount>n) trans.set_winding_I_turn_ratio_min_pu(get_double_data(data_winding1[n],"0.9"));
                n++;
                break;
            }
            case 3:// P control
            case 5:// asymmetric P control
            {
                if(ncount>n) trans.set_winding_I_angle_shift_max_deg(get_double_data(data_winding1[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_I_angle_shift_min_deg(get_double_data(data_winding1[n],"0.0"));
                n++;
                break;
            }
            case 4:// DC control
            {
                // bypass
                n++; n++;
                break;
            }
        }
        // now read controlled limit
        switch(abs(trans.get_winding_I_control_mode()))
        {
            case 1:// voltage control
            {
                if(ncount>n) trans.set_winding_I_controlled_voltage_max_pu(get_double_data(data_winding1[n],"1.1"));
                n++;
                if(ncount>n) trans.set_winding_I_controlled_voltage_min_pu(get_double_data(data_winding1[n],"0.9"));
                n++;
                break;
            }
            case 2:// Q control
            {
                if(ncount>n) trans.set_winding_I_controlled_reactive_power_max_MVar(get_double_data(data_winding1[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_I_controlled_reactive_power_min_MVar(get_double_data(data_winding1[n],"0.0"));
                n++;
                break;
            }
            case 3:// P control
            case 5:// asymmetric P control
            {
                if(ncount>n) trans.set_winding_I_controlled_active_power_max_MW(get_double_data(data_winding1[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_I_controlled_active_power_min_MW(get_double_data(data_winding1[n],"0.0"));
                n++;
                break;
            }
            case 0:// fixed tap and fixed angle shift
            case 4:// DC control
            {
                // bypass two data
                n++; n++;
                break;
            }
        }
        if(ncount>n) trans.set_winding_I_tap_number(get_integer_data(data_winding1[n],"33"));
        n++;
        if(ncount>n) trans.set_winding_I_Z_correction_table(get_integer_data(data_winding1[n],"0"));
        n++;
        if(ncount>n) trans.set_winding_I_compensation_R_pu(get_double_data(data_winding1[n],"0,0"));
        n++;
        if(ncount>n) trans.set_winding_I_compensation_X_pu(get_double_data(data_winding1[n],"0,0"));
        n++;
        if(ncount>n) trans.set_winding_I_connection_angle_deg(get_double_data(data_winding1[n],"0,0"));


        // now go reading winding j data
        ncount = data_winding2.size();
        n=0;
        if(ncount>n) trans.set_winding_J_turn_ratio_pu(get_double_data(data_winding2[n],"1.0"));
        n++;
        if(ncount>n) trans.set_winding_J_nominal_voltage_kV(get_double_data(data_winding2[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_J_angle_shift_deg(get_double_data(data_winding2[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_J_rateA_MVA(get_double_data(data_winding2[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_J_rateB_MVA(get_double_data(data_winding2[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_J_rateC_MVA(get_double_data(data_winding2[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_J_control_mode(get_integer_data(data_winding2[n],"0"));
        n++;
        if(ncount>n) trans.set_winding_J_controlled_bus(get_integer_data(data_winding2[n],"0"));
        n++;
        switch(abs(trans.get_winding_J_control_mode()))
        {
            case 0:// fixed tap and fixed angle shift
            case 1:// voltage control
            case 2:// Q control
            {
                if(ncount>n) trans.set_winding_J_turn_ratio_max_pu(get_double_data(data_winding2[n],"1.1"));
                n++;
                if(ncount>n) trans.set_winding_J_turn_ratio_min_pu(get_double_data(data_winding2[n],"0.9"));
                n++;
                break;
            }
            case 3:// P control
            case 5:// asymmetric P control
            {
                if(ncount>n) trans.set_winding_J_angle_shift_max_deg(get_double_data(data_winding2[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_J_angle_shift_min_deg(get_double_data(data_winding2[n],"0.0"));
                n++;
                break;
            }
            case 4:// DC control
            {
                // bypass
                n++; n++;
                break;
            }
        }
        // now read controlled limit
        switch(abs(trans.get_winding_J_control_mode()))
        {
            case 1:// voltage control
            {
                if(ncount>n) trans.set_winding_J_controlled_voltage_max_pu(get_double_data(data_winding2[n],"1.1"));
                n++;
                if(ncount>n) trans.set_winding_J_controlled_voltage_min_pu(get_double_data(data_winding2[n],"0.9"));
                n++;
                break;
            }
            case 2:// Q control
            {
                if(ncount>n) trans.set_winding_J_controlled_reactive_power_max_MVar(get_double_data(data_winding2[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_J_controlled_reactive_power_min_MVar(get_double_data(data_winding2[n],"0.0"));
                n++;
                break;
            }
            case 3:// P control
            case 5:// asymmetric P control
            {
                if(ncount>n) trans.set_winding_J_controlled_active_power_max_MW(get_double_data(data_winding2[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_J_controlled_active_power_min_MW(get_double_data(data_winding2[n],"0.0"));
                n++;
                break;
            }
            case 0:// fixed tap and fixed angle shift
            case 4:// DC control
            {
                // bypass two data
                n++; n++;
                break;
            }
        }
        if(ncount>n) trans.set_winding_J_tap_number(get_integer_data(data_winding2[n],"33"));
        n++;
        if(ncount>n) trans.set_winding_J_Z_correction_table(get_integer_data(data_winding2[n],"0"));
        n++;
        if(ncount>n) trans.set_winding_J_compensation_R_pu(get_double_data(data_winding2[n],"0,0"));
        n++;
        if(ncount>n) trans.set_winding_J_compensation_X_pu(get_double_data(data_winding2[n],"0,0"));
        n++;
        if(ncount>n) trans.set_winding_J_connection_angle_deg(get_double_data(data_winding2[n],"0,0"));

        // now go reading winding k data
        ncount = data_winding3.size();
        n=0;
        if(ncount>n) trans.set_winding_K_turn_ratio_pu(get_double_data(data_winding3[n],"1.0"));
        n++;
        if(ncount>n) trans.set_winding_K_nominal_voltage_kV(get_double_data(data_winding3[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_K_angle_shift_deg(get_double_data(data_winding3[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_K_rateA_MVA(get_double_data(data_winding3[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_K_rateB_MVA(get_double_data(data_winding3[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_K_rateC_MVA(get_double_data(data_winding3[n],"0.0"));
        n++;
        if(ncount>n) trans.set_winding_K_control_mode(get_integer_data(data_winding3[n],"0"));
        n++;
        if(ncount>n) trans.set_winding_K_controlled_bus(get_integer_data(data_winding3[n],"0"));
        n++;
        switch(abs(trans.get_winding_K_control_mode()))
        {
            case 0:// fixed tap and fixed angle shift
            case 1:// voltage control
            case 2:// Q control
            {
                if(ncount>n) trans.set_winding_K_turn_ratio_max_pu(get_double_data(data_winding3[n],"1.1"));
                n++;
                if(ncount>n) trans.set_winding_K_turn_ratio_min_pu(get_double_data(data_winding3[n],"0.9"));
                n++;
                break;
            }
            case 3:// P control
            case 5:// asymmetric P control
            {
                if(ncount>n) trans.set_winding_K_angle_shift_max_deg(get_double_data(data_winding3[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_K_angle_shift_min_deg(get_double_data(data_winding3[n],"0.0"));
                n++;
                break;
            }
            case 4:// DC control
            {
                // bypass
                n++; n++;
                break;
            }
        }
        // now read controlled limit
        switch(abs(trans.get_winding_K_control_mode()))
        {
            case 1:// voltage control
            {
                if(ncount>n) trans.set_winding_K_controlled_voltage_max_pu(get_double_data(data_winding3[n],"1.1"));
                n++;
                if(ncount>n) trans.set_winding_K_controlled_voltage_min_pu(get_double_data(data_winding3[n],"0.9"));
                n++;
                break;
            }
            case 2:// Q control
            {
                if(ncount>n) trans.set_winding_K_controlled_reactive_power_max_MVar(get_double_data(data_winding3[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_K_controlled_reactive_power_min_MVar(get_double_data(data_winding3[n],"0.0"));
                n++;
                break;
            }
            case 3:// P control
            case 5:// asymmetric P control
            {
                if(ncount>n) trans.set_winding_K_controlled_active_power_max_MW(get_double_data(data_winding3[n],"0.0"));
                n++;
                if(ncount>n) trans.set_winding_K_controlled_active_power_min_MW(get_double_data(data_winding3[n],"0.0"));
                n++;
                break;
            }
            case 0:// fixed tap and fixed angle shift
            case 4:// DC control
            {
                // bypass two data
                n++; n++;
                break;
            }
        }
        if(ncount>n) trans.set_winding_K_tap_number(get_integer_data(data_winding3[n],"33"));
        n++;
        if(ncount>n) trans.set_winding_K_Z_correction_table(get_integer_data(data_winding3[n],"0"));
        n++;
        if(ncount>n) trans.set_winding_K_compensation_R_pu(get_double_data(data_winding3[n],"0,0"));
        n++;
        if(ncount>n) trans.set_winding_K_compensation_X_pu(get_double_data(data_winding3[n],"0,0"));
        n++;
        if(ncount>n) trans.set_winding_K_connection_angle_deg(get_double_data(data_winding3[n],"0,0"));

        Transformer.push_back(trans);
    }
    //list
    cout<<Transformer.size()<<" transformers are imported"<<endl;
    return 0;
}

int load_network_data_PSSE_Area(vector <string> & fileInRAM)
{
    cout<<"now read area data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    string dataformat="int,int,double,double,string"; // format

    while(true)
    {
        AREA area;

        data.clear();
        strval.clear();
        // read one line of string
        str = fileInRAM[0];
        fileInRAM.erase(fileInRAM.begin()); // erase the read line from the fileInRAM
        format_string_with_comma(str); // format it
        if(str=="0") break; // no more area

        int error = check_data_format(str, dataformat); // check format
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data); // split string
        ncount = data.size();

        n=0;
        if(ncount>n) area.set_area_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) area.set_area_swing_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) area.set_desired_leaving_power_MW(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) area.set_tolerance_power_MW(get_double_data(data[n],"10.0"));
        n++;
        if(ncount>n) area.set_area_name(get_string_data(data[n],""));

        Area.push_back(area);
    }
    // list
    cout<<Area.size()<<" areas are imported"<<endl;
    return 0;
}
int load_network_data_PSSE_HVDC(vector <string> & fileInRAM)
{
    cout<<"now read hvdc data"<<endl;

    string str;
    vector<string> data;
    string strval;
    size_t ncount;
    size_t  n;
    string dataformat_hvdc="string,int,double,double,double,double,double,double,string,double,int,double"; // format
    string dataformat_converter="int,int,double,double,double,double,double,double,double,double,double,double,int,int,int,string,double"; // format

    while(true)
    {
        HVDC hvdc;

        data.clear();
        strval.clear();
        // read one line of string
        str = fileInRAM[0];
        fileInRAM.erase(fileInRAM.begin()); // erase the read line from the fileInRAM
        format_string_with_comma(str); // format it
        if(str=="0") break; // no more hvdc

        int error = check_data_format(str, dataformat_hvdc); // check format
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data); // split string
        ncount = data.size();
        // store hvdc data
        n=0;
        if(ncount>n) hvdc.set_HVDC_name(get_string_data(data[n],""));
        n++;
        if(ncount>n) hvdc.set_control_mode(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_HVDC_resistance_ohm(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n)
        {
            if(hvdc.get_control_mode()==0) hvdc.set_HVDC_power_demand_MW(get_double_data(data[n],"0.0"));
            if(hvdc.get_control_mode()==1) hvdc.set_HVDC_power_demand_MW(get_double_data(data[n],"0.0"));
            if(hvdc.get_control_mode()==2) hvdc.set_HVDC_current_demand_A(get_double_data(data[n],"0.0"));
        }
        n++;
        if(ncount>n) hvdc.set_HVDC_scheduled_compounded_DC_voltage_kV(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_HVDC_mode_switch_DC_voltage_kV(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_HVDC_compounded_resistance_ohm(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n)
        {
            if(hvdc.get_control_mode()==0) hvdc.set_HVDC_power_demand_margin_pu(get_double_data(data[n],"0.0"));
            if(hvdc.get_control_mode()==1) hvdc.set_HVDC_power_demand_margin_pu(get_double_data(data[n],"0.0"));
            if(hvdc.get_control_mode()==2) hvdc.set_HVDC_current_demand_margin_pu(get_double_data(data[n],"0.0"));
        }
        n++;
        if(ncount>n) hvdc.set_meter_end(get_string_data(data[n],"I"));
        n++;
        if(ncount>n) hvdc.set_HVDC_min_compounded_DC_voltage_kV(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_HVDC_max_iteration_for_CC_HVDC(get_integer_data(data[n],"20"));
        n++;
        if(ncount>n) hvdc.set_HVDC_acceleration_factor_for_CC_HVDC(get_double_data(data[n],"1.0"));

        // read rectifier
        str = fileInRAM[0];
        fileInRAM.erase(fileInRAM.begin()); // erase the read line from the fileInRAM
        format_string_with_comma(str); // format it

        error = check_data_format(str, dataformat_converter); // check format
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data); // split string
        ncount = data.size();
        // store rectifier data
        n=0;
        if(ncount>n) hvdc.set_rectifier_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_bridge_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_alpha_max_deg(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_alpha_min_deg(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_commutating_resistance_ohm(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_commutating_reactance_ohm(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_AC_base_voltage_kV(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_transformer_turn_ratio_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_transformer_tap_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_transformer_tap_max_pu(get_double_data(data[n],"1.5"));
        n++;
        if(ncount>n) hvdc.set_rectifier_transformer_tap_min_pu(get_double_data(data[n],"0.51"));
        n++;
        if(ncount>n) hvdc.set_rectifier_transformer_tap_step_pu(get_double_data(data[n],"0.00625"));
        n++;
        if(ncount>n) hvdc.set_rectifier_firing_angle_measurement_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_control_transformer_I_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_control_transformer_J_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_rectifier_control_transformer_id(get_string_data(data[n],""));
        n++;
        if(ncount>n) hvdc.set_rectifier_capacitor_reactance_ohm(get_double_data(data[n],"0.0"));


        // read inverter
        str = fileInRAM[0];
        fileInRAM.erase(fileInRAM.begin()); // erase the read line from the fileInRAM
        format_string_with_comma(str); // format it

        error = check_data_format(str, dataformat_converter); // check format
        if(error !=0) return 1; // if invalid data is provided

        split_string(str,",",data); // split string
        ncount = data.size();
        // store rectifier data
        n=0;
        if(ncount>n) hvdc.set_inverter_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_inverter_bridge_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_inverter_gamma_max_deg(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_gamma_min_deg(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_commutating_resistance_ohm(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_commutating_reactance_ohm(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_AC_base_voltage_kV(get_double_data(data[n],"0.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_transformer_turn_ratio_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_transformer_tap_pu(get_double_data(data[n],"1.0"));
        n++;
        if(ncount>n) hvdc.set_inverter_transformer_tap_max_pu(get_double_data(data[n],"1.5"));
        n++;
        if(ncount>n) hvdc.set_inverter_transformer_tap_min_pu(get_double_data(data[n],"0.51"));
        n++;
        if(ncount>n) hvdc.set_inverter_transformer_tap_step_pu(get_double_data(data[n],"0.00625"));
        n++;
        if(ncount>n) hvdc.set_inverter_firing_angle_measurement_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_inverter_control_transformer_I_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_inverter_control_transformer_J_bus_number(get_integer_data(data[n],"0"));
        n++;
        if(ncount>n) hvdc.set_inverter_control_transformer_id(get_string_data(data[n],""));
        n++;
        if(ncount>n) hvdc.set_inverter_capacitor_reactance_ohm(get_double_data(data[n],"0.0"));

        Hvdc.push_back(hvdc);
    }
    // list
    cout<<Hvdc.size()<<" HVDC links are imported"<<endl;
    return 0;
}
int load_network_data_PSSE_VSCHVDC(vector <string> & fileInRAM);
int load_network_data_PSSE_TransZCorrectionTab(vector <string> & fileInRAM);
int load_network_data_PSSE_Zone(vector <string> & fileInRAM);
