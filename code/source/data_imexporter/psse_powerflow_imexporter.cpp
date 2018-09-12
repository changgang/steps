#include "header/data_imexporter/psse_imexporter.h"
#include "header/basic/utility.h"
#include "header/device/bus.h"
#include "header/device/load.h"
#include "header/device/fixed_shunt.h"
#include "header/device/generator.h"
#include "header/device/wt_generator.h"
#include "header/device/line.h"
#include "header/device/transformer.h"
#include "header/steps_namespace.h"
#include <cstdio>
#include <istream>
#include <iostream>
using namespace std;

PSSE_IMEXPORTER::PSSE_IMEXPORTER()
{
    raw_data_in_ram.clear();
    dyr_data_in_ram.clear();
}

PSSE_IMEXPORTER::~PSSE_IMEXPORTER()
{
    raw_data_in_ram.clear();
    dyr_data_in_ram.clear();
}

void PSSE_IMEXPORTER::load_powerflow_data(string file)
{
    if(not is_power_system_database_set())
        return;

    ostringstream osstream;
    osstream<<"Loading powerflow data from PSS/E file: "<<file;
    show_information_with_leading_time_stamp(osstream);

    load_powerflow_data_into_ram(file);
    if(raw_data_in_ram.size()==0)
    {
        osstream<<"No data in the given PSS/E file: "<<file<<endl
          <<"Please check if the file exist or not.";
        show_information_with_leading_time_stamp(osstream);

        return;
    }
    load_case_data();
    load_bus_data();
    load_load_data();
    load_fixed_shunt_data();
    load_source_data();
    load_line_data();
    load_transformer_data();
    load_area_data();
    load_hvdc_data();
    load_vsc_hvdc_data();
    load_transformer_impedance_correction_table_data();
    load_multi_terminal_hvdc_data();
    load_multi_section_line_data();
    load_zone_data();
    load_interarea_transfer_data();
    load_owner_data();
    load_facts_data();
    load_switched_shunt_data();

    osstream<<"Done loading powerflow data.";
    show_information_with_leading_time_stamp(osstream);
}

void PSSE_IMEXPORTER::load_sequence_data(string sq_source)
{
    if(not is_power_system_database_set())
        return;

    sq_source = string2upper(sq_source);
}


void PSSE_IMEXPORTER::load_powerflow_data_into_ram(string file)
{
    ostringstream osstream;

    raw_data_in_ram.clear();

    FILE* fid = fopen(file.c_str(),"rt");
    if(fid == NULL)
    {
        osstream<<"PSS/E raw file '"<<file<<"' is not accessible. Loading PSS/E raw data is failed.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    char buffer[10240];
    string sbuffer;

    vector<string> data_of_one_type;
    data_of_one_type.clear();

    if(fgets(buffer, 1024, fid)==NULL)
        return;
    sbuffer = trim_psse_comment(buffer);
    data_of_one_type.push_back(trim_string(sbuffer));

    raw_data_in_ram.push_back(data_of_one_type);

    data_of_one_type.clear();

    for(size_t i=0; i!=2; ++i)
    {
        if(fgets(buffer, 1024, fid)==NULL)
            return;
        sbuffer = trim_psse_comment(buffer);
        data_of_one_type.push_back(trim_string(sbuffer));
    }
    raw_data_in_ram.push_back(data_of_one_type);

    data_of_one_type.clear();

    while(true)
    {
        if(fgets(buffer, 1024, fid)==NULL)
        {
            if(data_of_one_type.size()!=0)
            {
                raw_data_in_ram.push_back(data_of_one_type);
                data_of_one_type.clear();
            }
            break;
        }
        sbuffer = trim_psse_comment(buffer);
        sbuffer = trim_string(sbuffer);
        if(sbuffer.size()!=0)
        {
            if(sbuffer != "0")
            {
                data_of_one_type.push_back(sbuffer);
            }
            else
            {
                raw_data_in_ram.push_back(data_of_one_type);
                data_of_one_type.clear();
            }
        }
        else
            break;
    }
    fclose(fid);
}

string PSSE_IMEXPORTER::trim_psse_comment(string str)
{
    if(str.size()==0)
        return str;

    size_t found = str.find_first_of('/');
    if(found!=string::npos) str.erase(found, str.size());
    return str;
}

void PSSE_IMEXPORTER::load_case_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<2)
        return;
    vector<string> data = raw_data_in_ram[0];
    string case_str = data[0];

    data = split_string(case_str,",");
    if(data.size()>0)
    {
        int appendCode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        if(appendCode==0) psdb->clear_database(); // if not in appending mode, clear the database
    }
    if(data.size()>0)
    {
        psdb->set_system_base_power_in_MVA(get_double_data(data.front(),"100.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        set_data_version(get_integer_data(data.front(),"31"));
        data.erase(data.begin());
    }
    data.erase(data.begin()); // transformer rating
    data.erase(data.begin()); // line rating
    if(data.size()>0)
    {
        psdb->set_system_base_frequency_in_Hz(get_double_data(data.front(),"50.0"));
        data.erase(data.begin());
    }
    data = raw_data_in_ram[1];

    psdb->set_case_title_1(data[0]);
    psdb->set_case_title_2(data[1]);
    //
}


void PSSE_IMEXPORTER::set_data_version(size_t version)
{
    data_version = version;
}

size_t PSSE_IMEXPORTER::get_data_version() const
{
    if(not is_power_system_database_set())
        return 0;

    return data_version;
}

void PSSE_IMEXPORTER::load_bus_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<3)
        return;
    vector<string> DATA = raw_data_in_ram[2];
    string str;
    vector<string> data;

    size_t ndata = DATA.size();
    for(size_t i=0; i!=ndata; ++i)
    {
        str = DATA[i];
        BUS bus(psdb);

        data.clear();
        data = split_string(str,",");

        if(data.size()>0)
        {
            bus.set_bus_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            string busname = get_string_data(data.front(),"");
            bus.set_bus_name(trim_string(busname));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_base_voltage_in_kV(get_double_data(data.front(),"100.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int bt = get_integer_data(data.front(),"1");
            if(bt==1) bus.set_bus_type(PQ_TYPE);
            if(abs(bt)==2) bus.set_bus_type(PV_TYPE);
            if(bt==3) bus.set_bus_type(SLACK_TYPE);
            if(bt==4) bus.set_bus_type(OUT_OF_SERVICE);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_area_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_zone_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_owner_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_voltage_in_pu(get_double_data(data.front(),"1.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_angle_in_deg(get_double_data(data.front(),"0.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_normal_voltage_upper_limit_in_pu(get_double_data(data.front(),"1.1"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_normal_voltage_lower_limit_in_pu(get_double_data(data.front(),"0.9"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_emergency_voltage_upper_limit_in_pu(get_double_data(data.front(),"1.1"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_emergency_voltage_lower_limit_in_pu(get_double_data(data.front(),"0.9"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            bus.set_base_frequency_in_Hz(get_double_data(data.front(),"0.0"));
            data.erase(data.begin());
        }

        psdb->append_bus(bus);
    }
}
void PSSE_IMEXPORTER::load_load_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<4)
        return;
    vector<string> DATA = raw_data_in_ram[3];
    string str;
    vector<string> data;

    size_t ndata = DATA.size();
    double p = 0.0, q = 0.0;
    for(size_t i=0; i!=ndata; ++i)
    {
        str = DATA[i];
        LOAD load(psdb);

        data = split_string(str,",");
        if(data.size()>0)
        {
            load.set_load_bus(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            load.set_identifier(get_string_data(data.front(),""));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int status = get_integer_data(data.front(),"1");
            if(status==1) load.set_status(true);
            else          load.set_status(false);
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            load.set_area_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            load.set_zone_number(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        load.set_nominal_constant_power_load_in_MVA(complex<double>(p,q));
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        load.set_nominal_constant_current_load_in_MVA(complex<double>(p,q));
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        load.set_nominal_constant_impedance_load_in_MVA(complex<double>(p,-q));
        if(data.size()>0)
        {
            load.set_owner_number(get_integer_data(data.front(),"1"));
            data.erase(data.begin());
        }

        psdb->append_load(load);
    }
}
void PSSE_IMEXPORTER::load_fixed_shunt_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<5)
        return;
    vector<string> DATA = raw_data_in_ram[4];
    string str, strval;
    vector<string> data;

    size_t ndata = DATA.size();
    size_t n=0;
    double p = 0.0, q = 0.0;
    for(size_t i=0; i!=ndata; ++i)
    {
        str = DATA[i];
        FIXED_SHUNT shunt(psdb);

        data = split_string(str,",");
        if(data.size()>0)
        {
            shunt.set_shunt_bus(get_integer_data(data[n],"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            shunt.set_identifier(get_string_data(data[n],""));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            int status = get_integer_data(data[n],"1");
            if(status==1) shunt.set_status(true);
            else          shunt.set_status(false);
            data.erase(data.begin());
        }
        p = 0.0; q = 0.0;
        if(data.size()>0)
        {
            p=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            q=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        shunt.set_nominal_impedance_shunt_in_MVA(complex<double>(p,-q));

        psdb->append_fixed_shunt(shunt);
    }
}
void PSSE_IMEXPORTER::load_source_data()
{
    if(raw_data_in_ram.size()<6)
        return;
    vector<string> DATA = raw_data_in_ram[5];
    string str, strval;
    vector<string> data;

    size_t ndata = DATA.size();

    size_t SOURCE_TYPE_INDEX = 28;
    for(size_t i=0; i!=ndata; ++i)
    {
        str = DATA[i];
        data = split_string(str,",");

        size_t n = data.size();

        SOURCE_TYPE source_type = SYNC_GENERATOR_SOURCE;
        if(n>SOURCE_TYPE_INDEX)
        {
            int type = get_integer_data(data[SOURCE_TYPE_INDEX],"0");
            switch(type)
            {
                case 0:
                    source_type = SYNC_GENERATOR_SOURCE;
                    break;
                case 1:
                    source_type = WT_GENERATOR_SOURCE;
                    break;
                case 2:
                    source_type = PV_UNIT_SOURCE;
                    break;
                case 3:
                    source_type = ENERGY_STORAGE_SOURCE;
                    break;
                default:
                    source_type = SYNC_GENERATOR_SOURCE;
                    break;
            }
        }

        switch(source_type)
        {
            case SYNC_GENERATOR_SOURCE:
            {
                load_generator_data(data);
                break;
            }
            case WT_GENERATOR_SOURCE:
            {
                load_wt_generator_data(data);
                break;
            }
            case PV_UNIT_SOURCE:
            {
                load_pv_unit_data(data);
                break;
            }
            case ENERGY_STORAGE_SOURCE:
            {
                load_energy_storage_data(data);
                break;
            }
            default:
            {
                char buffer[MAX_TEMP_CHAR_BUFFER_SIZE];
                snprintf(buffer, MAX_TEMP_CHAR_BUFFER_SIZE, "Invalid source type is detected in PSS/E raw file of line:\n%s",str.c_str());
                show_information_with_leading_time_stamp(buffer);
                break;
            }
        }
    }
    /*
    POWER_SYSTEM_DATABASE* db = get_power_system_database();
    vector<SOURCE*> sources = db->get_all_sources();
    for(size_t i=0; i<sources.size(); ++i)
        sources[i]->report();
    */
}

void PSSE_IMEXPORTER::load_generator_data(vector<string>& data)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    GENERATOR generator(psdb);

    load_source_common_data(data, &generator);

    psdb->append_generator(generator);
}

void PSSE_IMEXPORTER::load_wt_generator_data(vector<string>& data)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    WT_GENERATOR wt_generator(psdb);

    load_source_common_data(data, &wt_generator);
    load_source_var_control_data(data, &wt_generator);

    psdb->append_wt_generator(wt_generator);
}


void PSSE_IMEXPORTER::load_pv_unit_data(vector<string>& data)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    PV_UNIT pv_unit(psdb);

    load_source_common_data(data, &pv_unit);
    load_source_var_control_data(data, &pv_unit);

    psdb->append_pv_unit(pv_unit);
}


void PSSE_IMEXPORTER::load_energy_storage_data(vector<string>& data)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    ENERGY_STORAGE estorage(psdb);

    load_source_common_data(data, &estorage);
    load_source_var_control_data(data, &estorage);

    psdb->append_energy_storage(estorage);
}

void PSSE_IMEXPORTER::load_source_common_data(vector<string>& data, SOURCE* source)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double rs = 0.0, xs = 0.0;
    //double rt = 0.0, xt = 0.0, tt = 1.0;
    int status;
    if(data.size()>0)
    {
        source->set_source_bus(get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_identifier(get_string_data(data.front(),""));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_p_generation_in_MW(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_q_generation_in_MVar(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_q_max_in_MVar(get_double_data(data.front(),"999.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_q_min_in_MVar(get_double_data(data.front(),"-999.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_voltage_to_regulate_in_pu(get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_bus_to_regulate(get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        char mbase[100];
        snprintf(mbase, 100, "%lf",psdb->get_system_base_power_in_MVA());
        source->set_mbase_in_MVA(get_double_data(data.front(),mbase));
        data.erase(data.begin());
    }
    rs = 0.0; xs = 0.0;
    if(data.size()>0)
    {
        rs = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        xs = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    source->set_source_impedance_in_pu(complex<double>(rs, xs));
    //rt = 0.0; xt = 0.0;
    if(data.size()>0)
    {
        //rt = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        //xt = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        //tt = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    status = 0;
    if(data.size()>0)
    {
        status = get_integer_data(data.front(),"1");
        if(status == 1) source->set_status(true);
        else            source->set_status(false);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        get_double_data(data.front(),"100.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_p_max_in_MW(get_double_data(data.front(),"999.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        source->set_p_min_in_MW(get_double_data(data.front(),"-999.0"));
        data.erase(data.begin());
    }
    OWNERSHIP os;
    int owner=0; double frac=0.0;
    for(int i=0; i!=4; ++i)
    {
        owner = 0;
        frac = 0.0;
        if(data.size()>0)
        {
            owner = get_integer_data(data.front(),"0");
            data.erase(data.begin());
        }
        else
            break;

        if(data.size()>0)
        {
            frac = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        else
            break;

        os.append_owner_and_its_fraction(owner, frac);
    }
    os.normalize();
    source->set_ownership(os);
}

void PSSE_IMEXPORTER::load_source_var_control_data(vector<string>& data, SOURCE* source)
{
    double pgen = source->get_p_generation_in_MW();

    int var_control_mode = 0;
    double pf = 1.0;
    if(data.size()>0)
    {
        var_control_mode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        if(data.size()>0)
        {
            pf = get_double_data(data.front(),"1.0");
            data.erase(data.begin());
        }
        if(pf==0.0)
            pf = 1.0;
        if(pf<0.0)
            pf = -pf;
        if(pf>1.0)
            pf = 1.0;;

        switch(var_control_mode)
        {
            case 0:
            case 1: // qmax and qmin to hold voltage
            {
                break;
            }
            case 2: // constant power factor to hold voltage
            {
                double qmax = 0.0;
                if(pf==1.0)
                    qmax = 0.0;
                else
                    qmax = pgen/pf*sqrt(1.0-pf*pf);
                source->set_q_max_in_MVar(qmax);
                source->set_q_min_in_MVar(-qmax);
                break;
            }
            case 3: // constant power
            default:
            {
                double qgen = source->get_q_generation_in_MVar();
                source->set_q_max_in_MVar(qgen);
                source->set_q_min_in_MVar(qgen);
                break;
            }
        }
    }

}

void PSSE_IMEXPORTER::load_line_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<7)
        return;
    vector<string> DATA = raw_data_in_ram[6];
    string str, strval;
    vector<string> data;

    size_t ndata = DATA.size();
    size_t n=0;
    double r = 0.0, x = 0.0, g = 0.0, b = 0.0;
    int status;
    RATING rating;
    int meterend;
    for(size_t i=0; i!=ndata; ++i)
    {
        str = DATA[i];
        LINE line(psdb);

        data = split_string(str,",");

        if(data.size()>0)
        {
            line.set_sending_side_bus(get_integer_data(data.front(),"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            line.set_receiving_side_bus(get_integer_data(data[n],"0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            line.set_identifier(get_string_data(data[n],""));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            r=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            x=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_line_positive_sequence_z_in_pu(complex<double>(r,x));
        if(data.size()>0)
        {
            b=get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_line_positive_sequence_y_in_pu(complex<double>(0.0,b));
        rating.clear();
        if(data.size()>0)
        {
            rating.set_rating_A_MVA(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            rating.set_rating_B_MVA(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            rating.set_rating_C_MVA(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        line.set_rating(rating);
        g=0.0; b=0.0;
        if(data.size()>0)
        {
            g = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            b = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_shunt_positive_sequence_y_at_sending_side_in_pu(complex<double>(g,b));
        g=0.0; b=0.0;
        if(data.size()>0)
        {
            g = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            b = get_double_data(data[n],"0.0");
            data.erase(data.begin());
        }
        line.set_shunt_positive_sequence_y_at_receiving_side_in_pu(complex<double>(g,b));
        status  = 0;
        if(data.size()>0)
        {
            status = get_integer_data(data[n],"0");
            data.erase(data.begin());
        }
        if(status == 1)
        {
            line.set_sending_side_breaker_status(true);
            line.set_receiving_side_breaker_status(true);
        }
        else
        {
            line.set_sending_side_breaker_status(false);
            line.set_receiving_side_breaker_status(false);
        }
        meterend = 1;
        if(data.size()>0)
        {
            meterend = get_integer_data(data[n],"1");
            data.erase(data.begin());
        }
        if(meterend<=1) line.set_meter_end_bus(line.get_sending_side_bus());
        else            line.set_meter_end_bus(line.get_receiving_side_bus());
        if(data.size()>0)
        {
            line.set_length(get_double_data(data[n],"0.0"));
            data.erase(data.begin());
        }
        OWNERSHIP os;
        int owner=0; double frac=0.0;
        for(size_t j=0; j!=4; ++j)
        {
            owner = 0;
            frac = 0.0;
            if(data.size()>0)
            {
                owner = get_integer_data(data.front(),"0");
                data.erase(data.begin());
            }
            else
                break;

            if(data.size()>0)
            {
                frac = get_double_data(data.front(),"0.0");
                data.erase(data.begin());
            }
            else
                break;

            os.append_owner_and_its_fraction(owner, frac);
        }
        os.normalize();
        line.set_ownership(os);

        psdb->append_line(line);
    }
}
void PSSE_IMEXPORTER::load_transformer_data()
{
    if(raw_data_in_ram.size()<8)
        return;
    vector<string> DATA = raw_data_in_ram[7];

    vector<string> data;

    size_t ndata = DATA.size();

    vector<string> trans_data;
    for(size_t i=0; i!=ndata; ++i)
    {
        trans_data.clear();

        data = split_string(DATA[i],",");
        trans_data.push_back(DATA[i]);
        ++i;
        if(i>=ndata)
            break;
        trans_data.push_back(DATA[i]);
        ++i;
        if(i>=ndata)
            break;
        trans_data.push_back(DATA[i]);
        ++i;
        if(i>=ndata)
            break;
        trans_data.push_back(DATA[i]);

        if(data[2]=="" or data[2]=="0")
            trans_data.push_back("");
        else
        {
            ++i;
            if(i>=ndata)
                break;
            trans_data.push_back(DATA[i]);
        }
        add_transformer_with_data(trans_data);
    }
}

void PSSE_IMEXPORTER::add_transformer_with_data(vector<string> trans_data)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    TRANSFORMER trans(psdb);

    string str_trans = trans_data[0];
    string str_z = trans_data[1];
    string str_winding_p = trans_data[2];
    string str_winding_s = trans_data[3];
    string str_winding_t = trans_data[4];

    vector<string> data_trans = split_string(str_trans,",");
    vector<string> data_z = split_string(str_z,",");
    vector<string> data_winding_p = split_string(str_winding_p,",");
    vector<string> data_winding_s = split_string(str_winding_s,",");
    vector<string> data_winding_t = split_string(str_winding_t,",");

    //string str, strval;
    //vector<string> data;

    TRANSFORMER_WINDING_TAP_CODE winding_code = TAP_WINDING_VOLTAGE_IN_KV;
    TRANSFORMER_IMPEDANCE_CODE impedance_code = IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE;
    TRANSFORMER_ADMITTANCE_CODE magnetizing_code = ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE;

    switch(get_integer_data(data_trans[4],"1"))
    {
        case 1:
            winding_code = TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE;
            break;
        case 2:
            winding_code = TAP_WINDING_VOLTAGE_IN_KV;
            break;
        case 3:
            winding_code = TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE;
            break;
        default: // default is 1
            winding_code = TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE;
            break;
    };

    switch(get_integer_data(data_trans[5],"1"))
    {
        case 1:
            impedance_code = IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
        case 2:
            impedance_code = IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
        case 3:
            impedance_code = IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
        default: // default is 1
            impedance_code = IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE;
            break;
    }
    switch(get_integer_data(data_trans[6],"1"))
    {
        case 1:
            magnetizing_code = ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE;
            break;
        case 2:
            magnetizing_code = ADMITTANCE_LOSS_IN_WATT_AND_CURRENT_IN_PU_ON_PRIMARY_SECONDARY_WINDINGS_POWER_AND_PRIMARY_WINDING_NOMINAL_VOLTAGE;
            break;
        default: // default is 1
            magnetizing_code = ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE;
            break;
    }

    // first get the transformer information
    add_transformer_basic_data(trans, data_trans);
    add_transformer_winding_data(trans, PRIMARY_SIDE, data_winding_p, winding_code);
    add_transformer_winding_data(trans, SECONDARY_SIDE, data_winding_s, winding_code);
    add_transformer_winding_data(trans, TERTIARY_SIDE, data_winding_t, winding_code);
    add_transformer_impedance_admittance_data(trans, data_z, impedance_code, magnetizing_code);

    psdb->append_transformer(trans);
}


void PSSE_IMEXPORTER::add_transformer_basic_data(TRANSFORMER& trans, vector<string> data)
{
    double gm = 0.0, bm = 0.0;
    if(data.size()>0)
    {
        trans.set_winding_bus(PRIMARY_SIDE, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        trans.set_winding_bus(SECONDARY_SIDE, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        trans.set_winding_bus(TERTIARY_SIDE, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        trans.set_identifier(get_string_data(data.front(),""));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        // winding_code is already set in its father function
        //winding_code = get_integer_data(data.front(),"0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        // impedance_code is already set in its father function
        //impedance_code = get_integer_data(data.front(),"0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        // magnetizing_code code is already set in its father function
        //magnetizing_code = get_integer_data(data.front(),"1");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        gm = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        bm = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double>(gm,bm));
    if(data.size()>0)
    {
        int nonmeterend = get_integer_data(data.front(),"2");
        data.erase(data.begin());
        switch(nonmeterend)
        {
            case 1:
            {
                trans.set_non_metered_end_bus(trans.get_winding_bus(PRIMARY_SIDE));
                break;
            }
            case 2:
            {
                trans.set_non_metered_end_bus(trans.get_winding_bus(SECONDARY_SIDE));
                break;
            }
            case 3:
            {
                trans.set_non_metered_end_bus(trans.get_winding_bus(TERTIARY_SIDE));
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        trans.set_transformer_name(get_string_data(data.front(),""));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        int status = get_integer_data(data.front(),"1");
        data.erase(data.begin());

        switch(status)
        {
            case 1:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                break;
            }
            case 2:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                trans.set_winding_breaker_status(SECONDARY_SIDE, false);
                trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                break;
            }
            case 3:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, true);
                trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                trans.set_winding_breaker_status(TERTIARY_SIDE, false);
                break;
            }
            case 4:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, false);
                trans.set_winding_breaker_status(SECONDARY_SIDE, true);
                trans.set_winding_breaker_status(TERTIARY_SIDE, true);
                break;
            }
            case 0:
            {
                trans.set_winding_breaker_status(PRIMARY_SIDE, false);
                trans.set_winding_breaker_status(SECONDARY_SIDE, false);
                trans.set_winding_breaker_status(TERTIARY_SIDE, false);
                break;
            }
            default:
                break;
        }
    }

    OWNERSHIP os;
    int owner=0; double frac=0.0;
    for(size_t j=0; j!=4; ++j)
    {
        owner = 0;
        frac = 0.0;
        if(data.size()>0)
        {
            owner = get_integer_data(data.front(),"0");
            data.erase(data.begin());
        }
        else
            break;

        if(data.size()>0)
        {
            frac = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        else
            break;

        os.append_owner_and_its_fraction(owner, frac);
    }
    os.normalize();
    trans.set_ownership(os);
}
void PSSE_IMEXPORTER::add_transformer_winding_data(TRANSFORMER&trans, TRANSFORMER_WINDING_SIDE winding, vector<string> data, TRANSFORMER_WINDING_TAP_CODE winding_code)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(data.size()<2)
        return;
    trans.set_winding_nominal_voltage_in_kV(winding, get_double_data(data[1],"0.0"));
    data.erase(data.begin()+1);
    if(data.size()>0)
    {
        double t = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(winding_code)
        {
            case TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE:
            {
                t *= psdb->get_bus_base_voltage_in_kV(trans.get_winding_bus(winding));
                t /= trans.get_winding_nominal_voltage_in_kV(winding);
                break;
            }
            case TAP_WINDING_VOLTAGE_IN_KV:
            {
                t /= trans.get_winding_nominal_voltage_in_kV(winding);
                break;
            }
            default: // TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE is default of steps
                break;
        }
        trans.set_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding, t);
    }
    if(data.size()>0)
    {
        trans.set_winding_angle_shift_in_deg(winding, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    RATING rate;
    if(data.size()>0)
    {
        rate.set_rating_A_MVA(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        rate.set_rating_B_MVA(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        rate.set_rating_C_MVA(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    trans.set_winding_rating_in_MVA(winding, rate);
    if(data.size()>0)
    {
        int mode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        switch(mode)
        {
            case 0:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_NO_CONTROL);
                break;
            }
            case 1:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_VOLTAGE_CONTROL);
                break;
            }
            case 2:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_REACTIVE_POWER_CONTROL);
                break;
            }
            case 3:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_ACTIVE_POWER_CONTROL);
                break;
            }
            case 4:
            {
                trans.set_winding_control_mode(winding, TRANSFORMER_TAP_HVDC_LINE_CONTROL);
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        trans.set_winding_controlled_bus(winding, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double tamax = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_NO_CONTROL:
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            {
                switch(winding_code)
                {
                    case TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE:
                    {
                        tamax *= psdb->get_bus_base_voltage_in_kV(trans.get_winding_bus(winding));
                        tamax /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    case TAP_WINDING_VOLTAGE_IN_KV:
                    {
                        tamax /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    default: // TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE is default of steps
                        break;
                }
                trans.set_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding, tamax);
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_winding_max_angle_shift_in_deg(winding, tamax);
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        double tamin = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_NO_CONTROL:
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            {
                switch(winding_code)
                {
                    case TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_BUS_BASE_VOLTAGE:
                    {
                        tamin *= psdb->get_bus_base_voltage_in_kV(trans.get_winding_bus(winding));
                        tamin /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    case TAP_WINDING_VOLTAGE_IN_KV:
                    {
                        tamin /= trans.get_winding_nominal_voltage_in_kV(winding);
                        break;
                    }
                    default: //TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE is default of steps
                        break;
                }
                trans.set_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding, tamin);
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_winding_min_angle_shift_in_deg(winding, tamin);
                break;
            }
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        double vpqmax = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            {
                trans.set_winding_controlled_max_voltage_in_pu(winding, vpqmax);
                break;
            }
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_max_reactive_power_into_winding_in_MVar(winding, vpqmax);
                break;
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_max_active_power_into_winding_in_MW(winding, vpqmax);
                break;
            }
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            case TRANSFORMER_TAP_NO_CONTROL:
            default:
                break;
        }
    }
    if(data.size()>0)
    {
        double vpqmin = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        switch(trans.get_winding_control_mode(winding))
        {
            case TRANSFORMER_TAP_VOLTAGE_CONTROL:
            {
                trans.set_winding_controlled_min_voltage_in_pu(winding, vpqmin);
                break;
            }
            case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_min_reactive_power_into_winding_in_MVar(winding, vpqmin);
                break;
            }
            case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
            {
                trans.set_controlled_min_active_power_into_winding_in_MW(winding, vpqmin);
                break;
            }
            case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
            case TRANSFORMER_TAP_NO_CONTROL:
            default:
                break;
        }
    }

    if(data.size()>0)
    {
        trans.set_winding_number_of_taps(winding, get_integer_data(data.front(),"33"));
        data.erase(data.begin());
    }
}

void PSSE_IMEXPORTER::add_transformer_impedance_admittance_data(TRANSFORMER& trans, vector<string> data, TRANSFORMER_IMPEDANCE_CODE impedance_code, TRANSFORMER_ADMITTANCE_CODE magnetizing_code)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    double g = trans.get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().real();
    double b = trans.get_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu().imag();

    double mvabase = psdb->get_system_base_power_in_MVA();
    double vbase, sbase;

    double r = 0.0, x = 0.0, s = 0.0;

    if(data.size()>0)
    {
        r = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        x = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        s = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    switch(impedance_code)
    {
        case IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE:
        {
            r = r*s/mvabase;
            x = x*s/mvabase;
            break;
        }
        case IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE:
        {
            r = r/s;
            x = sqrt(x*x-r*r);
            break;
        }
        default: // IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE is default of steps
            break;
    }
    trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE, complex<double>(r,x));
    trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE, s);

    if(trans.is_three_winding_transformer())
    {
        r = 0.0; x = 0.0; s = 0.0;
        if(data.size()>0)
        {
            r = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            x = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            s = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        switch(impedance_code)
        {
            case IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r*s/mvabase;
                x = x*s/mvabase;
                break;
            }
            case IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r/s;
                x = sqrt(x*x-r*r);
                break;
            }
            default:// IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE is default of steps
                break;
        }
        trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE, complex<double>(r,x));
        trans.set_winding_nominal_capacity_in_MVA(SECONDARY_SIDE,  TERTIARY_SIDE, s);

        r = 0.0; x = 0.0; s = 0.0;
        if(data.size()>0)
        {
            r = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            x = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        if(data.size()>0)
        {
            s = get_double_data(data.front(),"0.0");
            data.erase(data.begin());
        }
        switch(impedance_code)
        {
            case IMPEDANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r*s/mvabase;
                x = x*s/mvabase;
                break;
            }
            case IMPEDANCE_LOSS_IN_WATT_AND_Z_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE:
            {
                r = r/s;
                x = sqrt(x*x-r*r);
                break;
            }
            default: // IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE is default of steps
                break;
        }
        trans.set_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE, complex<double>(r,x));
        trans.set_winding_nominal_capacity_in_MVA(PRIMARY_SIDE,  TERTIARY_SIDE, s);
    }

    switch(magnetizing_code)
    {
        case ADMITTANCE_LOSS_IN_WATT_AND_CURRENT_IN_PU_ON_PRIMARY_SECONDARY_WINDINGS_POWER_AND_PRIMARY_WINDING_NOMINAL_VOLTAGE:
            g = g*1e-6;
            vbase = trans.get_winding_nominal_voltage_in_kV(PRIMARY_SIDE);
            sbase = trans.get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE);
            g = g/(vbase*vbase);
            b = b*sbase/(vbase*vbase);
            sbase = psdb->get_system_base_power_in_MVA();
            vbase = psdb->get_bus_base_voltage_in_kV(trans.get_winding_bus(PRIMARY_SIDE));
            g = g/(sbase/(vbase*vbase));
            b = b/(sbase/(vbase*vbase));
            b = sqrt(b*b-g*g);
            b = -b;
            trans.set_magnetizing_admittance_based_on_primary_winding_bus_base_voltage_and_system_base_power_in_pu(complex<double>(g,b));
            break;
        default: //ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE is default of steps
            break;
    }
}

void PSSE_IMEXPORTER::load_area_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<9)
        return;
    vector<string> DATA = raw_data_in_ram[8];

    vector<string> data;

    size_t ndata = DATA.size();

    for(size_t i=0; i!=ndata; ++i)
    {
        data.clear();

        data = split_string(DATA[i],",");

        AREA area(psdb);

        area.set_area_number(get_integer_data(data[0],"0"));
        area.set_area_swing_bus(get_integer_data(data[1],"0"));
        area.set_expected_power_leaving_area_in_MW(get_double_data(data[2],"0.0"));
        area.set_area_power_mismatch_tolerance_in_MW(get_double_data(data[3],"10.0"));
        area.set_area_name(get_string_data(data[4],""));

        psdb->append_area(area);
    }
}

void PSSE_IMEXPORTER::load_hvdc_data()
{
    if(raw_data_in_ram.size()<10)
        return;
    vector<string> DATA = raw_data_in_ram[9];

    vector<string> data;

    size_t ndata = DATA.size();
    RATING rating;

    vector<string> hvdc_data;
    for(size_t i=0; i!=ndata; ++i)
    {
        hvdc_data.clear();

        data = split_string(DATA[i],",");
        hvdc_data.push_back(DATA[i]);
        i++;
        if(i>=ndata)
            break;
        hvdc_data.push_back(DATA[i]);
        i++;
        if(i>=ndata)
            break;
        hvdc_data.push_back(DATA[i]);

        add_hvdc_with_data(hvdc_data);
    }
}

void PSSE_IMEXPORTER::add_hvdc_with_data(vector<string> hvdc_data)
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    HVDC hvdc(psdb);

    string str_hvdc = hvdc_data[0];
    string str_rec = hvdc_data[1];
    string str_inv = hvdc_data[2];

    vector<string> data_hvdc = split_string(str_hvdc,",");
    vector<string> data_rec = split_string(str_rec,",");
    vector<string> data_inv = split_string(str_inv,",");

    add_hvdc_basic_data(hvdc, data_hvdc);
    add_hvdc_converter_data(hvdc, RECTIFIER, data_rec);
    add_hvdc_converter_data(hvdc, INVERTER, data_inv);

    while(psdb->is_hvdc_exist(hvdc.get_device_id()))
        hvdc.set_identifier(hvdc.get_identifier()+"#");

    psdb->append_hvdc(hvdc);
}
void PSSE_IMEXPORTER::add_hvdc_basic_data(HVDC& hvdc, vector<string> data)
{
    if(data.size()>0)
    {
        string name = get_string_data(data.front(),"");
        hvdc.set_name(name);
        hvdc.set_identifier(name);
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        int mode = get_integer_data(data.front(),"0");
        data.erase(data.begin());
        if(mode==0)
            hvdc.set_status(false);
        else
        {
            hvdc.set_status(true);
            if(mode==1)
                hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_POWER);
            else
                hvdc.set_converter_operation_mode(RECTIFIER, RECTIFIER_CONSTANT_CURRENT);
        }
    }
    if(data.size()>0)
    {
        hvdc.set_line_resistance_in_ohm(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double value = get_double_data(data.front(), "0.0");
        data.erase(data.begin());
        switch(hvdc.get_converter_operation_mode(RECTIFIER))
        {
            case RECTIFIER_CONSTANT_CURRENT:
                hvdc.set_nominal_dc_current_per_pole_in_kA(value*1e-3);
                break;
            default:// constant power + blocked
                hvdc.set_nominal_dc_power_per_pole_in_MW(fabs(value));
                if(value>0.0)
                    hvdc.set_side_to_hold_power(RECTIFIER);
                else
                    hvdc.set_side_to_hold_power(INVERTER);
                break;
        }
    }
    if(data.size()>0)
    {
        hvdc.set_nominal_dc_voltage_per_pole_in_kV(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV(get_double_data(data.front(),"0.15"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_compensating_resistance_to_hold_dc_voltage_in_ohm(get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_current_power_margin(get_double_data(data.front(),"0.15"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        string meterend = get_string_data(data.front(),"I");
        if(meterend=="R")
            hvdc.set_meter_end(RECTIFIER);
        else
            hvdc.set_meter_end(INVERTER);
        data.erase(data.begin());
    }
}

void PSSE_IMEXPORTER::add_hvdc_converter_data(HVDC& hvdc, HVDC_CONVERTER_SIDE converter, vector<string> data)
{
    if(data.size()>0)
    {
        hvdc.set_converter_bus(converter, get_integer_data(data.front(),"0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_number_of_bridge(converter, get_integer_data(data.front(),"1"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_max_alpha_or_gamma_in_deg(converter, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_min_alpha_or_gamma_in_deg(converter, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    double r=0.0, x=0.0;
    if(data.size()>0)
    {
        r = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        x = get_double_data(data.front(),"0.0");
        data.erase(data.begin());
    }
    hvdc.set_converter_transformer_impedance_in_ohm(converter, complex<double>(r,x));
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_grid_side_base_voltage_in_kV(converter, get_double_data(data.front(),"0.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double tap = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        double v = hvdc.get_converter_transformer_grid_side_base_voltage_in_kV(converter)*tap;
        hvdc.set_converter_transformer_converter_side_base_voltage_in_kV(converter, v);
    }
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_tap_in_pu(converter, get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_max_tap_in_pu(converter, get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        hvdc.set_converter_transformer_min_tap_in_pu(converter, get_double_data(data.front(),"1.0"));
        data.erase(data.begin());
    }
    if(data.size()>0)
    {
        double step = get_double_data(data.front(),"1.0");
        data.erase(data.begin());
        double tap = hvdc.get_converter_transformer_max_tap_in_pu(converter)-hvdc.get_converter_transformer_min_tap_in_pu(converter);
        int n = int(fabs(round(tap/step)))+1;
        hvdc.set_converter_transformer_number_of_taps(converter, n);
    }
    if(data.size()>0)
    {
        data.erase(data.begin());
    }
}

void PSSE_IMEXPORTER::load_vsc_hvdc_data()
{
    ;
}

void PSSE_IMEXPORTER::load_transformer_impedance_correction_table_data()
{
    ;
}

void PSSE_IMEXPORTER::load_multi_terminal_hvdc_data()
{
    ;
}

void PSSE_IMEXPORTER::load_multi_section_line_data()
{
    ;
}

void PSSE_IMEXPORTER::load_zone_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<15)
        return;
    vector<string> DATA = raw_data_in_ram[14];

    vector<string> data;

    size_t ndata = DATA.size();

    for(size_t i=0; i!=ndata; ++i)
    {
        data.clear();

        data = split_string(DATA[i],",");

        ZONE zone(psdb);

        zone.set_zone_number(get_integer_data(data[0],"0"));
        zone.set_zone_name(get_string_data(data[1],""));

        psdb->append_zone(zone);
    }
}

void PSSE_IMEXPORTER::load_interarea_transfer_data()
{
    ;
}

void PSSE_IMEXPORTER::load_owner_data()
{
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();

    if(raw_data_in_ram.size()<17)
        return;
    vector<string> DATA = raw_data_in_ram[16];

    vector<string> data;

    size_t ndata = DATA.size();

    for(size_t i=0; i!=ndata; ++i)
    {
        data.clear();

        data = split_string(DATA[i],",");

        OWNER owner(psdb);

        owner.set_owner_number(get_integer_data(data[0],"0"));
        owner.set_owner_name(get_string_data(data[1],""));

        psdb->append_owner(owner);
    }
}

void PSSE_IMEXPORTER::load_facts_data()
{
    ;
}

void PSSE_IMEXPORTER::load_switched_shunt_data()
{
    ;
}


void PSSE_IMEXPORTER::export_powerflow_data(string file)
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No powerflow data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    ofstream ofs(file);
    if(!ofs)
    {
        osstream<<"Warning. PSS/E raw file "<<file<<" cannot be opened for exporting powerflow data.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }

    ofs<<export_case_data();
    ofs<<export_bus_data();
    ofs<<"0 / END OF BUS DATA, BEGIN LOAD DATA"<<endl;
    ofs<<export_load_data();
    ofs<<"0 / END OF LOAD DATA, BEGIN FIXED SHUNT DATA"<<endl;
    ofs<<export_fixed_shunt_data();
    ofs<<"0 / END OF FIXED SHUNT DATA, BEGIN GENERATOR DATA"<<endl;
    ofs<<export_source_data();
    ofs<<"0 / END OF GENERATOR DATA, BEGIN TRANSMISSION LINE DATA"<<endl;
    ofs<<export_line_data();
    ofs<<"0 / END OF TRANSMISSION LINE DATA, BEGIN TRANSFORMER DATA"<<endl;
    ofs<<export_transformer_data();
    ofs<<"0 / END OF TRANSFORMER DATA, BEGIN AREA DATA"<<endl;
    ofs<<export_area_data();
    ofs<<"0 / END OF AREA DATA, BEGIN TWO-TERMINAL HVDC DATA"<<endl;
    ofs<<export_hvdc_data();
    ofs<<"'0' / END OF TWO-TERMINAL HVDC DATA, BEGIN VSC HVDC LINE DATA"<<endl;
    ofs<<export_vsc_hvdc_data();
    ofs<<"'0' / END OF VSC HVDC LINE DATA, BEGIN IMPEDANCE CORRECTION DATA"<<endl;
    ofs<<export_transformer_impedance_correction_table_data();
    ofs<<"0 / END OF IMPEDANCE CORRECTION DATA, BEGIN MULTI-TERMINAL HVDC DATA"<<endl;
    ofs<<export_multi_terminal_hvdc_data();
    ofs<<"0 / END OF MULTI-TERMINAL HVDC DATA, BEGIN MULTI-SECTION LINE DATA"<<endl;
    ofs<<export_multi_section_line_data();
    ofs<<"0 / END OF MULTI-SECTION LINE DATA, BEGIN ZONE DATA"<<endl;
    ofs<<export_zone_data();
    ofs<<"0 / END OF ZONE DATA, BEGIN INTER-AREA TRANSFER DATA"<<endl;
    ofs<<export_interarea_transfer_data();
    ofs<<"0 / END OF INTER-AREA TRANSFER DATA, BEGIN OWNER DATA"<<endl;
    ofs<<export_owner_data();
    ofs<<"0 / END OF OWNER DATA, BEGIN FACTS DEVICE DATA"<<endl;
    ofs<<export_facts_data();
    ofs<<"0 / END OF FACTS DEVICE DATA, BEGIN SWITCHED SHUNT DATA"<<endl;
    ofs<<export_switched_shunt_data();
    ofs<<"0 / END OF SWITCHED SHUNT DATA, BEGIN GNE DATA"<<endl;
    ofs<<"0 / END OF GNE DATA, BEGIN INDUCTION MACHINE DATA"<<endl;
    ofs<<"0 / END OF INDUCTION MACHINE DATA"<<endl;
    ofs<<"Q";

    ofs.close();
}



string PSSE_IMEXPORTER::export_case_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No case data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    char buffer[1000];
    snprintf(buffer, 1000, "0, %f, 33, 0, 0, %f", psdb->get_system_base_power_in_MVA(), psdb->get_system_base_frequency_in_Hz());
    osstream<<buffer<<endl;
    snprintf(buffer, 1000, "%s", (psdb->get_case_title_1()).c_str());
    osstream<<buffer<<endl;
    snprintf(buffer, 1000, "%s", (psdb->get_case_title_2()).c_str());
    osstream<<buffer<<endl;
    return osstream.str();
}

string PSSE_IMEXPORTER::export_bus_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No bus data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<BUS*> buses = psdb->get_all_buses();
    size_t n = buses.size();
    char buffer[1000];
    for(size_t i=0; i!=n; ++i)
    {
        BUS* bus = buses[i];

        BUS_TYPE bus_type = bus->get_bus_type();
        int type = 4;
        if(bus_type == PQ_TYPE) type = 1;
        if(bus_type == PV_TYPE) type = 2;
        if(bus_type == PV_TO_PQ_TYPE_1 or bus_type == PV_TO_PQ_TYPE_2 or
           bus_type == PV_TO_PQ_TYPE_3 or bus_type == PV_TO_PQ_TYPE_4) type = -2;
        if(bus_type == SLACK_TYPE) type = 3;
        if(bus_type == OUT_OF_SERVICE) type = 4;

        /*osstream<<right
              <<setw(8)<<setprecision(0)<<bus->get_bus_number()<<", "
              <<"\""
              <<left
              <<setw(16)<<bus->get_bus_name()
              <<"\""<<", "
              <<right
              <<setw(8)<<setprecision(2)<<fixed<<bus->get_base_voltage_in_kV()<<", "
              <<setw(2)<<setprecision(0)<<type<<", "
              <<setw(4)<<setprecision(0)<<bus->get_area_number()<<", "
              <<setw(4)<<setprecision(0)<<bus->get_zone_number()<<", "
              <<setw(4)<<setprecision(0)<<bus->get_owner_number()<<", "
              <<setw(10)<<setprecision(6)<<fixed<<bus->get_voltage_in_pu()<<", "
              <<setw(10)<<setprecision(6)<<fixed<<bus->get_angle_in_deg()<<", "
              <<setw(6)<<setprecision(4)<<fixed<<bus->get_normal_voltage_upper_limit_in_pu()<<", "
              <<setw(6)<<setprecision(4)<<fixed<<bus->get_normal_voltage_lower_limit_in_pu()<<", "
              <<setw(6)<<setprecision(4)<<fixed<<bus->get_emergency_voltage_upper_limit_in_pu()<<", "
              <<setw(6)<<setprecision(4)<<fixed<<bus->get_emergency_voltage_lower_limit_in_pu()<<endl;*/
        snprintf(buffer, 1000, "%8lu, \"%-16s\", %8.2f, %2d, %4lu, %4lu, %4lu, %10.6f, %10.6f, %6.4f, %6.4f, %6.4f, %6.4f, %4.1f",
                 bus->get_bus_number(), (bus->get_bus_name()).c_str(), bus->get_base_voltage_in_kV(), type,
                 bus->get_area_number(), bus->get_zone_number(), bus->get_owner_number(),
                 bus->get_voltage_in_pu(), bus->get_angle_in_deg(),
                 bus->get_normal_voltage_upper_limit_in_pu(), bus->get_normal_voltage_lower_limit_in_pu(),
                 bus->get_emergency_voltage_upper_limit_in_pu(), bus->get_emergency_voltage_lower_limit_in_pu(),
                 bus->get_base_frequency_in_Hz());
        osstream<<buffer<<endl;

    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_load_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No load data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<LOAD*> loads = psdb->get_all_loads();
    size_t n = loads.size();
    char buffer[1000];
    for(size_t i=0; i!=n; ++i)
    {
        LOAD* load = loads[i];

        /*osstream<<right
          <<setw(8)
          <<load->get_load_bus()<<", "
          <<"\""<<left
          <<load->get_identifier()<<"\""<<", "
          <<right
          <<load->get_status()<<", "
          <<setw(4)<<load->get_area_number()<<", "
          <<setw(4)<<load->get_zone_number()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_power_load_in_MVA().real()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_power_load_in_MVA().imag()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_current_load_in_MVA().real()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_current_load_in_MVA().imag()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<load->get_nominal_constant_impedance_load_in_MVA().real()<<", "
          <<setw(12)<<setprecision(6)<<fixed<<-load->get_nominal_constant_impedance_load_in_MVA().imag()<<", "
          <<setw(4)<<load->get_owner_number()<<", "
          <<setw(2)<<fixed<<1<<", "
          <<setw(2)<<load->get_flag_interruptable()<<endl;*/

        snprintf(buffer, 1000, "%lu, \"%s\", %d, %4lu, %4lu, %12.6f, %12.6f, %12.6f, %12.6f, %12.6f, %12.6f, %4lu, 1, %2d",
                load->get_load_bus(), (load->get_identifier()).c_str(), (load->get_status()==true)?1:0,
                load->get_area_number(), load->get_zone_number(),
                load->get_nominal_constant_power_load_in_MVA().real(), load->get_nominal_constant_power_load_in_MVA().imag(),
                load->get_nominal_constant_current_load_in_MVA().real(), load->get_nominal_constant_current_load_in_MVA().imag(),
                load->get_nominal_constant_impedance_load_in_MVA().real(), load->get_nominal_constant_impedance_load_in_MVA().imag(),
                load->get_owner_number(), (load->get_flag_interruptable()==true)?1:0);
        osstream<<buffer<<endl;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_fixed_shunt_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No fixed shunt data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<FIXED_SHUNT*> fshunts = psdb->get_all_fixed_shunts();
    size_t n = fshunts.size();
    for(size_t i=0; i!=n; ++i)
    {
        FIXED_SHUNT* shunt = fshunts[i];

        osstream<<right
                <<setw(8)<<shunt->get_shunt_bus()<<", "
                <<"\""
                <<left
                <<setw(2)<<shunt->get_identifier()
                <<"\""<<", "
                <<right
                <<shunt->get_status()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<shunt->get_nominal_impedance_shunt_in_MVA().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<-shunt->get_nominal_impedance_shunt_in_MVA().imag()<<endl;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_source_data() const
{
    ostringstream osstream;
    osstream<<export_generator_data();
    osstream<<export_wt_generator_data();
    osstream<<export_pv_unit_data();
    osstream<<export_energy_storage_data();

    return osstream.str();
}

string PSSE_IMEXPORTER::export_generator_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No generator data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<GENERATOR*> generators = psdb->get_all_generators();
    size_t n = generators.size();
    for(size_t i=0; i!=n; ++i)
    {
        GENERATOR* generator = generators[i];

        osstream<<export_source_common_data(generator);
        osstream<<"0, 0.0, 0"<<endl;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_wt_generator_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No WT generator data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<WT_GENERATOR*> wt_generators = psdb->get_all_wt_generators();
    size_t n = wt_generators.size();
    for(size_t i=0; i!=n; ++i)
    {
        WT_GENERATOR* wt_generator = wt_generators[i];

        osstream<<export_source_common_data(wt_generator);
        osstream<<export_source_var_control_data(wt_generator);
        osstream<<", 1"<<endl;
    }
    return osstream.str();
}


string PSSE_IMEXPORTER::export_pv_unit_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No PV unit data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<PV_UNIT*> pv_units = psdb->get_all_pv_units();
    size_t n = pv_units.size();
    for(size_t i=0; i!=n; ++i)
    {
        PV_UNIT* pv_unit = pv_units[i];

        osstream<<export_source_common_data(pv_unit);
        osstream<<export_source_var_control_data(pv_unit);

        osstream<<", 2"<<endl;
    }
    return osstream.str();
}


string PSSE_IMEXPORTER::export_energy_storage_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No energy storage data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<ENERGY_STORAGE*> estorages = psdb->get_all_energy_storages();
    size_t n = estorages.size();
    for(size_t i=0; i!=n; ++i)
    {
        ENERGY_STORAGE* estorage = estorages[i];

        osstream<<export_source_common_data(estorage);
        osstream<<export_source_var_control_data(estorage);

        osstream<<", 3"<<endl;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_source_common_data(SOURCE* source) const
{
    ostringstream osstream;

    osstream<<right
           <<setw(8)<<source->get_source_bus()<<", "
           <<"\""<<left
           <<setw(2)<<source->get_identifier()<<"\""<<", "
           <<right
           <<setw(12)<<setprecision(6)<<fixed<<source->get_p_generation_in_MW()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_q_generation_in_MVar()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_q_max_in_MVar()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_q_min_in_MVar()<<", "
           <<setw(8)<<setprecision(6)<<fixed<<source->get_voltage_to_regulate_in_pu()<<", ";
    if(source->get_bus_to_regulate()==source->get_source_bus())
        osstream<<setw(8)<<0<<", ";
    else
        osstream<<setw(8)<<source->get_bus_to_regulate()<<", ";
    osstream<<setw(10)<<setprecision(4)<<fixed<<source->get_mbase_in_MVA()<<", "
           <<setw(9)<<setprecision(6)<<fixed<<source->get_source_impedance_in_pu().real()<<", "
           <<setw(9)<<setprecision(6)<<fixed<<source->get_source_impedance_in_pu().imag()<<", "
           <<setprecision(2)<<0.0<<", "<<0.0<<", "<<1.0<<", "
           <<source->get_status()<<","
           <<setprecision(2)<<100.0<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_p_max_in_MW()<<", "
           <<setw(12)<<setprecision(6)<<fixed<<source->get_p_min_in_MW()<<", ";
    if(source->get_owner_count()==0)
        osstream<<"1, 1.0, 0, 0.0, 0, 0.0, 0, 0.0, ";
    else
    {
        osstream<< setprecision(0) << source->get_owner_of_index(0) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(0) << ", "
                << setprecision(0) << source->get_owner_of_index(1) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(1) << ", "
                << setprecision(0) << source->get_owner_of_index(2) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(2) << ", "
                << setprecision(0) << source->get_owner_of_index(3) << ", " << setprecision(6) << source->get_fraction_of_owner_of_index(3) << ", ";
    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_source_var_control_data(SOURCE* source) const
{
    ostringstream osstream;
    double p = source->get_p_generation_in_MW();

    double qmax = source->get_q_max_in_MVar();
    double qmin = source->get_q_min_in_MVar();
    if(fabs(qmax+qmin)>FLOAT_EPSILON)
    {
        if(fabs(qmax-qmin)>FLOAT_EPSILON)
            osstream<<"0, 0.0";
        else
            osstream<<"3, 0.0";
    }
    else
    {
        double pf = p/sqrt(p*p+qmax*qmax);
        osstream<<"2, "<<setprecision(6)<<pf;
    }
    return osstream.str();
}

string PSSE_IMEXPORTER::export_line_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No line data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<LINE*> lines = psdb->get_all_lines();
    size_t n = lines.size();
    for(size_t i=0; i!=n; ++i)
    {
        LINE* line = lines[i];

        size_t meterend = 1;
        if(line->get_meter_end_bus()==line->get_receiving_side_bus())
            meterend = 2;

        osstream<<right
                <<setw(8)<<line->get_sending_side_bus()<<", "
                <<setw(8)<<line->get_receiving_side_bus()<<", "
                <<"\""<<left
                <<setw(2)<<line->get_identifier()<<"\""<<", "
                <<right
                <<setw(12)<<setprecision(6)<<fixed<<line->get_line_positive_sequence_z_in_pu().real()<<","
                <<setw(12)<<setprecision(6)<<fixed<<line->get_line_positive_sequence_z_in_pu().imag()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_line_positive_sequence_y_in_pu().imag()<<", "
                <<setw(10)<<setprecision(3)<<fixed<<line->get_rating().get_rating_A_MVA()<<", "
                <<setw(10)<<setprecision(3)<<fixed<<line->get_rating().get_rating_B_MVA()<<", "
                <<setw(10)<<setprecision(3)<<fixed<<line->get_rating().get_rating_C_MVA()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_sending_side_in_pu().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_sending_side_in_pu().imag()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_receiving_side_in_pu().real()<<", "
                <<setw(12)<<setprecision(6)<<fixed<<line->get_shunt_positive_sequence_y_at_receiving_side_in_pu().imag()<<", "
                <<(line->get_sending_side_breaker_status() and line->get_receiving_side_breaker_status())<<", "
                <<meterend<<", "
                <<setw(6)<<setprecision(2)<<fixed<<line->get_length()<<", "
                <<setw(4)<<line->get_owner_of_index(0)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(0)<<", "
                <<setw(4)<<line->get_owner_of_index(1)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(1)<<", "
                <<setw(4)<<line->get_owner_of_index(2)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(2)<<", "
                <<setw(4)<<line->get_owner_of_index(3)<<", "<<setw(6)<<setprecision(3)<<line->get_fraction_of_owner_of_index(3)<<endl;

    }
    return osstream.str();
}
string PSSE_IMEXPORTER::export_transformer_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No transformer data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<TRANSFORMER*> transformers = psdb->get_all_transformers();
    size_t n = transformers.size();
    for(size_t i=0; i!=n; ++i)
    {
        TRANSFORMER* trans = transformers[i];
        size_t nonmeterend = 2;
        if(trans->is_two_winding_transformer())
        {
            if(trans->get_non_metered_end_bus() == trans->get_winding_bus(PRIMARY_SIDE))
                nonmeterend = 1;
            else
                nonmeterend = 2;
        }
        else
        {
            if(trans->get_non_metered_end_bus() == trans->get_winding_bus(PRIMARY_SIDE))
                nonmeterend = 1;
            else
            {
                if(trans->get_non_metered_end_bus() == trans->get_winding_bus(SECONDARY_SIDE))
                    nonmeterend = 2;
                else
                {
                    if(trans->get_non_metered_end_bus() == trans->get_winding_bus(TERTIARY_SIDE))
                        nonmeterend = 3;
                    else
                        nonmeterend = 2;
                }
            }
        }

        size_t status = 1;
        if(trans->is_two_winding_transformer())
        {
            if(trans->get_winding_breaker_status(PRIMARY_SIDE)==false or trans->get_winding_breaker_status(SECONDARY_SIDE)==false)
                status = 0;
        }
        else
        {
            if(trans->get_winding_breaker_status(PRIMARY_SIDE)==false and trans->get_winding_breaker_status(SECONDARY_SIDE)==false
               and trans->get_winding_breaker_status(TERTIARY_SIDE)==false)
                status = 0;
            else
            {
                if(trans->get_winding_breaker_status(SECONDARY_SIDE)==false)
                    status = 2;
                if(trans->get_winding_breaker_status(TERTIARY_SIDE)==false)
                    status = 2;
                if(trans->get_winding_breaker_status(PRIMARY_SIDE)==false)
                    status = 2;
            }
        }
        osstream<<right
              <<setw(8)<<trans->get_winding_bus(PRIMARY_SIDE)<<", "
              <<setw(8)<<trans->get_winding_bus(SECONDARY_SIDE)<<", "
              <<setw(8)<<trans->get_winding_bus(TERTIARY_SIDE)<<", "
              <<"\""<<left
              <<setw(2)<<trans->get_identifier()<<"\", "
              <<right
              <<TAP_OFF_NOMINAL_TURN_RATIO_BASED_ON_WINDING_NOMNAL_VOLTAGE<<", "
              <<IMPEDANCE_IN_PU_ON_WINDINGS_POWER_AND_WINDING_NOMINAL_VOLTAGE<<", "
              <<ADMITTANCE_IN_PU_ON_SYSTEM_BASE_POWER_AND_PRIMARY_WINDING_BUS_BASE_VOLTAGE<<", "
              <<setprecision(6)<<fixed<<trans->get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu().real()<<", "
              <<setprecision(6)<<fixed<<trans->get_magnetizing_admittance_based_on_winding_norminal_voltage_and_system_base_power_in_pu().imag()<<", "
              <<nonmeterend<<", "
              <<"\""<<left
              <<setw(16)<<trans->get_transformer_name()<<"\", "
              <<right
              <<status<<", "
              <<trans->get_owner_of_index(0)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(0)<<", "
              <<trans->get_owner_of_index(1)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(1)<<", "
              <<trans->get_owner_of_index(2)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(2)<<", "
              <<trans->get_owner_of_index(3)<<", "<<setprecision(6)<<fixed<<trans->get_fraction_of_owner_of_index(3)<<", "
              <<"\"\""<<endl;
        if(trans->is_two_winding_transformer())
        {
            osstream<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).real()<<", "
              <<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).imag()<<", "
              <<trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)<<endl;

            TRANSFORMER_WINDING_SIDE winding = PRIMARY_SIDE;

            size_t control_mode;
            switch(trans->get_winding_control_mode(winding))
            {
                case TRANSFORMER_TAP_NO_CONTROL:
                    control_mode = 0;
                    break;
                case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                    control_mode = 1;
                    break;
                case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                    control_mode = 2;
                    break;
                case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                    control_mode = 3;
                    break;
                case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                    control_mode = 4;
                    break;
                case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                    control_mode = 5;
                    break;
                default:
                    control_mode = 0;
                    break;
            }

            osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
              <<setw(7)<<setprecision(2)<<fixed<<trans->get_winding_nominal_voltage_in_kV(winding)<<", "
              <<setw(6)<<setprecision(2)<<fixed<<trans->get_winding_angle_shift_in_deg(winding)<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_A_MVA()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_B_MVA()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_C_MVA()<<", "
              <<control_mode<<", ";
            switch(trans->get_winding_control_mode(winding))
            {
                case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                    osstream<<trans->get_winding_controlled_bus(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                    break;
                case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                    osstream<<trans->get_winding_controlled_bus(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_reactive_power_into_winding_in_MVar(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_reactive_power_into_winding_in_MVar(winding)<<", ";
                    break;
                case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                    osstream<<trans->get_winding_controlled_bus(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_angle_shift_in_deg(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_angle_shift_in_deg(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_active_power_into_winding_in_MW(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_active_power_into_winding_in_MW(winding)<<", ";
                    break;
                case TRANSFORMER_TAP_NO_CONTROL:
                case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                default:
                    osstream<<0<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                      <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                    break;
            }

            osstream<<setw(3)<<setprecision(0)<<trans->get_winding_number_of_taps(winding)<<", "
              <<setw(3)<<setprecision(0)<<0<<", "
              <<setw(3)<<setprecision(1)<<0.0<<", "
              <<setw(3)<<setprecision(1)<<0.0<<", "
              <<setw(3)<<setprecision(1)<<0.0<<endl;

            winding = SECONDARY_SIDE;
            osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
              <<setw(7)<<setprecision(2)<<fixed<<trans->get_winding_nominal_voltage_in_kV(winding)<<endl;
        }
        else
        {
            osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).real()<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, SECONDARY_SIDE).imag()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, SECONDARY_SIDE)<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).real()<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(SECONDARY_SIDE, TERTIARY_SIDE).imag()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_nominal_capacity_in_MVA(SECONDARY_SIDE, TERTIARY_SIDE)<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE).real()<<", "
              <<setw(8)<<setprecision(6)<<fixed<<trans->get_leakage_impedance_between_windings_based_on_winding_nominals_in_pu(PRIMARY_SIDE, TERTIARY_SIDE).imag()<<", "
              <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_nominal_capacity_in_MVA(PRIMARY_SIDE, TERTIARY_SIDE)<<", "
              <<"1.0, 0.0"<<endl;

            for(size_t j=1; j!=4; ++j)
            {
                TRANSFORMER_WINDING_SIDE winding=PRIMARY_SIDE;
                if(j==0) winding = PRIMARY_SIDE;
                if(j==1) winding = SECONDARY_SIDE;
                if(j==2) winding = TERTIARY_SIDE;

                size_t control_mode;
                switch(trans->get_winding_control_mode(winding))
                {
                    case TRANSFORMER_TAP_NO_CONTROL:
                        control_mode = 0;
                        break;
                    case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                        control_mode = 1;
                        break;
                    case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                        control_mode = 2;
                        break;
                    case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                        control_mode = 3;
                        break;
                    case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                        control_mode = 4;
                        break;
                    case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                        control_mode = 5;
                        break;
                    default:
                        control_mode = 0;
                        break;
                }

                osstream<<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                  <<setw(7)<<setprecision(2)<<fixed<<trans->get_winding_nominal_voltage_in_kV(winding)<<", "
                  <<setw(6)<<setprecision(2)<<fixed<<trans->get_winding_angle_shift_in_deg(winding)<<", "
                  <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_A_MVA()<<", "
                  <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_B_MVA()<<", "
                  <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_rating_in_MVA(winding).get_rating_C_MVA()<<", "
                  <<control_mode<<", ";
                switch(trans->get_winding_control_mode(winding))
                {
                    case TRANSFORMER_TAP_VOLTAGE_CONTROL:
                        osstream<<trans->get_winding_controlled_bus(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                        break;
                    case TRANSFORMER_TAP_REACTIVE_POWER_CONTROL:
                        osstream<<trans->get_winding_controlled_bus(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_reactive_power_into_winding_in_MVar(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_reactive_power_into_winding_in_MVar(winding)<<", ";
                        break;
                    case TRANSFORMER_TAP_ACTIVE_POWER_CONTROL:
                    case TRANSFORMER_TAP_ASYMMETRIC_ACTIVE_POWER_CONTROL:
                        osstream<<trans->get_winding_controlled_bus(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_angle_shift_in_deg(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_angle_shift_in_deg(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_max_active_power_into_winding_in_MW(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_controlled_min_active_power_into_winding_in_MW(winding)<<", ";
                        break;
                    case TRANSFORMER_TAP_NO_CONTROL:
                    case TRANSFORMER_TAP_HVDC_LINE_CONTROL:
                    default:
                        osstream<<0<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_max_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(6)<<fixed<<trans->get_winding_min_turn_ratio_based_on_winding_nominal_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_max_voltage_in_pu(winding)<<", "
                          <<setw(8)<<setprecision(2)<<fixed<<trans->get_winding_controlled_min_voltage_in_pu(winding)<<", ";
                        break;
                }

                osstream<<setw(3)<<setprecision(0)<<trans->get_winding_number_of_taps(winding)<<", "
                  <<setw(3)<<setprecision(0)<<0<<", "
                  <<setw(3)<<setprecision(1)<<0.0<<", "
                  <<setw(3)<<setprecision(1)<<0.0<<", "
                  <<setw(3)<<setprecision(1)<<0.0<<endl;
            }
        }
    }
    return osstream.str();
}
string PSSE_IMEXPORTER::export_area_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No area data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<AREA*> areas = psdb->get_all_areas();
    size_t n = areas.size();
    for(size_t i=0; i!=n; ++i)
    {
        AREA* area = areas[i];

        osstream<<setw(8)<<area->get_area_number()<<", "
          <<area->get_area_swing_bus()<<", "
          <<setprecision(3)<<area->get_expected_power_leaving_area_in_MW()<<", "
          <<setprecision(3)<<area->get_area_power_mismatch_tolerance_in_MW()<<", "
          <<"\""<<area->get_area_name()<<"\""<<endl;
    }

    return osstream.str();
}
string PSSE_IMEXPORTER::export_hvdc_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No area data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<HVDC*> hvdcs = psdb->get_all_hvdcs();
    size_t n = hvdcs.size();
    for(size_t i=0; i!=n; ++i)
    {
        HVDC* hvdc = hvdcs[i];

        osstream<<"\""<<left
               <<setw(16)<<hvdc->get_name()<<"\""<<", ";
        osstream<<right;

        bool status = hvdc->get_status();
        HVDC_OPERATION_MODE mode = hvdc->get_converter_operation_mode(RECTIFIER);
        if(status == false)
            osstream<<0<<", ";
        else
        {
            if(mode==RECTIFIER_CONSTANT_CURRENT)
                osstream<<2<<", ";
            else
                osstream<<1<<", ";
        }
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_line_resistance_in_ohm()<<", ";
        if(mode==RECTIFIER_CONSTANT_CURRENT)
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_current_per_pole_in_kA()*100.0<<", ";
        else
        {
            HVDC_CONVERTER_SIDE side = hvdc->get_side_to_hold_dc_power();
            if(side==RECTIFIER)
                osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_power_per_pole_in_MW()<<", ";
            else
                osstream<<setw(6)<<setprecision(2)<<fixed<<-hvdc->get_nominal_dc_power_per_pole_in_MW()<<", ";
        }

        osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_nominal_dc_voltage_per_pole_in_kV()<<", ";
        osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_threshold_dc_voltage_for_constant_power_and_constant_current_mode_in_kV()<<", ";
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_compensating_resistance_to_hold_dc_voltage_in_ohm()<<", ";
        osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_current_power_margin()<<", ";

        osstream<<"\""<<(hvdc->get_meter_end()==RECTIFIER?"R":"I")<<"\", ";
        osstream<<"0.0, 20, 1.0"<<endl;

        for(size_t j=0; j!=2; ++j)
        {
            HVDC_CONVERTER_SIDE converter=RECTIFIER;
            if(j==0) converter = RECTIFIER;
            if(j==1) converter = INVERTER;

            osstream<<setw(8)<<hvdc->get_converter_bus(converter)<<", ";
            osstream<<hvdc->get_converter_number_of_bridge(converter)<<", ";
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_converter_max_alpha_or_gamma_in_deg(converter)<<", ";
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_converter_min_alpha_or_gamma_in_deg(converter)<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_impedance_in_ohm(converter).real()<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_impedance_in_ohm(converter).imag()<<", ";
            osstream<<setw(6)<<setprecision(2)<<fixed<<hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(converter)<<", ";
            double turn_ratio = hvdc->get_converter_transformer_converter_side_base_voltage_in_kV(converter)/
                       hvdc->get_converter_transformer_grid_side_base_voltage_in_kV(converter);
            osstream<<setw(6)<<setprecision(4)<<fixed<<turn_ratio<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_tap_in_pu(converter)<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_max_tap_in_pu(converter)<<", ";
            osstream<<setw(6)<<setprecision(4)<<fixed<<hvdc->get_converter_transformer_min_tap_in_pu(converter)<<", ";
            double tap_step = (hvdc->get_converter_transformer_max_tap_in_pu(converter)-
                               hvdc->get_converter_transformer_min_tap_in_pu(converter))/
                               hvdc->get_converter_transformer_number_of_taps(converter);
            osstream<<setw(6)<<setprecision(5)<<fixed<<tap_step<<", ";
            osstream<<"0, 0, 0, \"1\", 0.0"<<endl;
        }
    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_vsc_hvdc_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_transformer_impedance_correction_table_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_multi_terminal_hvdc_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_multi_section_line_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_zone_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No zone data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<ZONE*> zones = psdb->get_all_zones();
    size_t n = zones.size();
    for(size_t i=0; i!=n; ++i)
    {
        ZONE* zone = zones[i];

        osstream<<setw(8)<<zone->get_zone_number()<<", "
          <<"\""<<zone->get_zone_name()<<"\""<<endl;
    }

    return osstream.str();
}

string PSSE_IMEXPORTER::export_interarea_transfer_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_owner_data() const
{
    ostringstream osstream;
    POWER_SYSTEM_DATABASE* psdb = get_power_system_database();
    if(psdb==NULL)
    {
        osstream<<"Warning. PSSE imexporter is not connected to any power system database. No area data will be exported.";
        show_information_with_leading_time_stamp(osstream);
        return "";
    }

    vector<OWNER*> owners = psdb->get_all_owners();
    size_t n = owners.size();
    for(size_t i=0; i!=n; ++i)
    {
        OWNER* owner = owners[i];

        osstream<<setw(8)<<owner->get_owner_number()<<", "
          <<"\""<<owner->get_owner_name()<<"\""<<endl;
    }

    return osstream.str();
}
string PSSE_IMEXPORTER::export_facts_data() const
{
    return "";
}

string PSSE_IMEXPORTER::export_switched_shunt_data() const
{
    return "";
}


void PSSE_IMEXPORTER::export_sequence_data(string file)
{
    ofstream fid(file);
    fid.close();
    return;
}
