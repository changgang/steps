#include "header/dynamic_model_database.h"
#include "header/STEPS.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include "header/model/all_supported_models.h"
#include <istream>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

DYNAMIC_MODEL_DATABASE::DYNAMIC_MODEL_DATABASE(STEPS& toolkit)
{
    this->toolkit = (&toolkit);

    warehouse_capacity = 0;
    occupied_warehouse_capacity = 0;
    is_full = false;
    model_warehouse = NULL;
    model_starting_position_table.clear();
}


DYNAMIC_MODEL_DATABASE::~DYNAMIC_MODEL_DATABASE()
{
    clear();
    toolkit = NULL;
}

STEPS& DYNAMIC_MODEL_DATABASE::get_toolkit() const
{
    return *toolkit;
}

void DYNAMIC_MODEL_DATABASE::check()
{
    ;
}

void DYNAMIC_MODEL_DATABASE::clear()
{
    size_t n = model_starting_position_table.size();
    for(size_t i=0; i<n; ++i)
    {
        MODEL* model = (MODEL*) (model_warehouse+model_starting_position_table[i]);
        model->destroy_manually_allocated_storage();
    }
    is_full = false;
    occupied_warehouse_capacity = 0;
    model_starting_position_table.clear();
    if(model_warehouse!=NULL)
    {
        free(model_warehouse);
        model_warehouse = NULL;
    }
}

void DYNAMIC_MODEL_DATABASE::add_model(MODEL* model)
{
    ostringstream osstream;
    if(model_warehouse==NULL)
    {
        warehouse_capacity = toolkit->get_dynamic_model_database_size_in_bytes();
        model_warehouse = (char*) malloc(warehouse_capacity);
        if(model_warehouse==NULL)
        {
            osstream<<"Error. Cannot allocate dynamic model database. Check codes.";
            toolkit->show_information_with_leading_time_stamp(osstream);
        }
    }

    if(is_full)
        return;

    unsigned int model_size = get_model_size(model);
    if(occupied_warehouse_capacity+model_size>warehouse_capacity)
    {
        is_full = true;
        osstream<<"******************************\n"
                <<"*****************************\n"
                <<"*****************************\n"
                <<"Error. Dynamic model database is full. No more model will be added. Increase dynamic model database size with API.\n"
                <<"The first model failing to append is: \n"
                <<model->get_dynamic_data_in_psse_format()<<"\n"
                <<"*****************************\n"
                <<"*****************************\n"
                <<"*****************************";
        cout<<osstream.str()<<endl;
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
    else
    {
        POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();
        DEVICE_ID did_new = model->get_device_id();
        string model_type = model->get_model_type();
        NONBUS_DEVICE* device = psdb.get_nonbus_device(did_new);
        MODEL* old_model = device->get_model_of_type(model_type);
        if(old_model==NULL)
        {
            common_set_model(model, model_size);
        }
        else
        {
            if(did_new.get_device_type()==STEPS_LOAD)
            {
                bool flag = load_related_model_is_to_update(old_model, model);
                if(flag==true)
                {
                    unsigned int old_model_size = get_model_size(old_model);
                    if(old_model_size>=model_size)
                        memcpy((void*)old_model, model, model_size);
                    else
                    {
                        common_set_model(model, model_size);
                        shrink_model_starting_position_table_at_position(old_model);
                    }
                }
            }
            else
            {
                if(old_model->get_model_name()==model->get_model_name())
                    memcpy((void*)old_model, model, model_size);
                else
                {
                    unsigned int old_model_size = get_model_size(old_model);
                    if(old_model_size>=model_size)
                        memcpy((void*)old_model, model, model_size);
                    else
                    {
                        common_set_model(model, model_size);
                        shrink_model_starting_position_table_at_position(old_model);
                    }
                }
            }
        }
    }
}

unsigned int DYNAMIC_MODEL_DATABASE::get_model_size(MODEL* model) const
{
    string model_name = model->get_model_name();
    if(model_name=="GENCLS") return sizeof(GENCLS);
    if(model_name=="GENROU") return sizeof(GENROU);
    if(model_name=="GENSAL") return sizeof(GENSAL);

    if(model_name=="COMP") return sizeof(COMP);
    if(model_name=="IEEEVC") return sizeof(IEEEVC);
    if(model_name=="PSASPVC") return sizeof(PSASPVC);

    if(model_name=="SEXS") return sizeof(SEXS);
    if(model_name=="IEEET1") return sizeof(IEEET1);
    if(model_name=="PSASPE1") return sizeof(PSASPE1);
    if(model_name=="PSASPE2") return sizeof(PSASPE2);
    if(model_name=="CSEET1") return sizeof(CSEET1);
    if(model_name=="CSEET2") return sizeof(CSEET2);
    if(model_name=="PSASPE13") return sizeof(PSASPE13);
    if(model_name=="PSASPE14") return sizeof(PSASPE14);

    if(model_name=="IEE2ST") return sizeof(IEE2ST);
    if(model_name=="PSASPS1") return sizeof(PSASPS1);
    if(model_name=="PSASPS2") return sizeof(PSASPS2);
    if(model_name=="PSASPS3") return sizeof(PSASPS3);
    if(model_name=="PSASPS4") return sizeof(PSASPS4);
    if(model_name=="PSASPS5") return sizeof(PSASPS5);
    if(model_name=="PSASPS6") return sizeof(PSASPS6);
    if(model_name=="PSASPS8") return sizeof(PSASPS8);

    if(model_name=="IEEEG1") return sizeof(IEEEG1);
    if(model_name=="IEEEG1SB") return sizeof(IEEEG1SB);
    if(model_name=="IEEEG2") return sizeof(IEEEG2);
    if(model_name=="IEEEG3") return sizeof(IEEEG3);
    if(model_name=="IEESGO") return sizeof(IEESGO);
    if(model_name=="TGOV1") return sizeof(TGOV1);
    if(model_name=="GAST2A") return sizeof(GAST2A);
    if(model_name=="URCSCT") return sizeof(URCSCT);

    if(model_name=="LCFB1") return sizeof(LCFB1);



    if(model_name=="ESTR0") return sizeof(ESTR0);



    if(model_name=="IEEL") return sizeof(IEEL);
    if(model_name=="CIM6") return sizeof(CIM6);

    if(model_name=="UVLS") return sizeof(UVLS);

    if(model_name=="UFLS") return sizeof(UFLS);
    if(model_name=="PUFLS") return sizeof(PUFLS);


    if(model_name=="CDC4T") return sizeof(CDC4T);
    if(model_name=="CDC6T") return sizeof(CDC6T);

    if(model_name=="VSC_HVDC1") return sizeof(VSC_HVDC1);

    if(model_name=="WT3G0") return sizeof(WT3G0);
    if(model_name=="WT3G1") return sizeof(WT3G1);
    if(model_name=="WT3G2") return sizeof(WT3G2);

    if(model_name=="AERD0") return sizeof(AERD0);

    if(model_name=="WT3T0") return sizeof(WT3T0);
    if(model_name=="WT3T1") return sizeof(WT3T1);

    if(model_name=="WT3E0") return sizeof(WT3E0);
    if(model_name=="WT3E1") return sizeof(WT3E1);

    if(model_name=="WT3P0") return sizeof(WT3P0);
    if(model_name=="WT3P1") return sizeof(WT3P1);

    if(model_name=="FILEWIND") return sizeof(FILEWIND);

    if(model_name=="WTRLY0") return sizeof(WTRLY0);


    if(model_name=="PVCV0") return sizeof(PVCV0);
    if(model_name=="PVGU1") return sizeof(PVGU1);


    if(model_name=="ARXL") return sizeof(ARXL);

    ostringstream osstream;
    osstream<<"Model '"<<model_name<<"' is not supported in DYNAMIC_MODEL_DATABASE::"<<__FUNCTION__<<". Fatal error may occur.\n"
            <<"Update DYNAMIC_MODEL_DATABASE::"<<__FUNCTION__<<"() is required to include the new model.\n"
            <<"Consult developer if required.";
    toolkit->show_information_with_leading_time_stamp(osstream);

    return 0;
}

void DYNAMIC_MODEL_DATABASE::common_set_model(MODEL* model, unsigned int model_size)
{
    memcpy((void*)(model_warehouse+occupied_warehouse_capacity), model, model_size);
    model_starting_position_table.push_back(occupied_warehouse_capacity);

    MODEL* model_pointer = (MODEL*) (model_warehouse+occupied_warehouse_capacity);

    model_pointer->allocate_model_variables();
    model_pointer->prepare_model_data_table();
    model_pointer->prepare_model_internal_variable_table();

    NONBUS_DEVICE* device = model_pointer->get_device_pointer();
    device->set_model(model_pointer);

    occupied_warehouse_capacity += model_size;
}

void DYNAMIC_MODEL_DATABASE::shrink_model_starting_position_table_at_position(void *pos)
{
    unsigned int n = model_starting_position_table.size();
    unsigned int model_table_index = INDEX_NOT_EXIST;
    for(unsigned int i=0; i<n; ++i)
    {
        void * this_model_postion = model_warehouse+model_starting_position_table[i];
        if(this_model_postion!=pos)
            continue;
        else
        {
            model_table_index = i;
            break;
        }
    }
    if(model_table_index!=INDEX_NOT_EXIST)
    {
        MODEL* model_to_delete = (MODEL*) (model_warehouse+model_starting_position_table[model_table_index]);
        model_to_delete->destroy_manually_allocated_storage();

        for(unsigned int i=model_table_index+1; i<n; ++i)
            model_starting_position_table[i-1] = model_starting_position_table[i];
        model_starting_position_table.pop_back();
    }
}

bool DYNAMIC_MODEL_DATABASE::load_related_model_is_to_update(MODEL* old_model, MODEL* new_model)
{
    string model_type = new_model->get_model_type();
    SUBSYSTEM_TYPE stype_old = BUS_SUBSYSTEM_TYPE;
    SUBSYSTEM_TYPE stype_new = BUS_SUBSYSTEM_TYPE;
    if(model_type=="LOAD CHARACTERISTICS")
    {
        LOAD_MODEL* old_load_model = (LOAD_MODEL*) old_model;
        LOAD_MODEL* new_load_model = (LOAD_MODEL*) new_model;

        stype_old = old_load_model->get_subsystem_type();
        stype_new = new_load_model->get_subsystem_type();
    }
    else
    {
        if(model_type=="LOAD VOLTAGE RELAY")
        {
            LOAD_VOLTAGE_RELAY_MODEL* old_load_v_relay_model = (LOAD_VOLTAGE_RELAY_MODEL*) old_model;
            LOAD_VOLTAGE_RELAY_MODEL* new_load_v_relay_model = (LOAD_VOLTAGE_RELAY_MODEL*) new_model;

            stype_old = old_load_v_relay_model->get_subsystem_type();
            stype_new = new_load_v_relay_model->get_subsystem_type();
        }
        else
        {
            LOAD_FREQUENCY_RELAY_MODEL* old_load_f_relay_model = (LOAD_FREQUENCY_RELAY_MODEL*) old_model;
            LOAD_FREQUENCY_RELAY_MODEL* new_load_f_relay_model = (LOAD_FREQUENCY_RELAY_MODEL*) new_model;

            stype_old = old_load_f_relay_model->get_subsystem_type();
            stype_new = new_load_f_relay_model->get_subsystem_type();
        }
    }
    if(stype_new<=stype_old)
        return true;
    else
        return false;
}

void DYNAMIC_MODEL_DATABASE::remove_the_last_model()
{
    unsigned int n = model_starting_position_table.size();
    switch(n)
    {
        case 0:
            return;
        case 1:
            occupied_warehouse_capacity = 0;
            model_starting_position_table.pop_back();
            return;
        default:
            occupied_warehouse_capacity = model_starting_position_table[n-1];
            model_starting_position_table.pop_back();
            return;
    }
}


struct DEVICE_MODEL_MIN_T
{
    DEVICE_ID did;
    string model_name;
    double min_t_in_s;
};

void DYNAMIC_MODEL_DATABASE::check_device_model_minimum_time_constants()
{
    vector<DEVICE_MODEL_MIN_T> mint_list;
    unsigned int max_record_count = 100;
    mint_list.reserve(max_record_count);

    unsigned int n = model_starting_position_table.size();
    for(unsigned int i=0; i<n; ++i)
    {
        MODEL* model = (MODEL*) (model_warehouse+model_starting_position_table[i]);
        DEVICE_ID did = model->get_device_id();
        string model_name = model->get_model_name();
        double mint = model->get_minimum_nonzero_time_constant_in_s();

        if(mint>0.0 and mint<1.0)
        {
            DEVICE_MODEL_MIN_T newrecord; // build new record
            newrecord.did = did;
            newrecord.model_name = model_name;
            newrecord.min_t_in_s = mint;
            if(mint_list.size()==max_record_count)// if list is full, try to replace the last one
            {
                double t_max = mint_list[max_record_count-1].min_t_in_s;
                if(mint>=t_max)
                    continue;
                else
                    mint_list[max_record_count-1] = newrecord;
            }
            else //if list is not full, add new record
                mint_list.push_back(newrecord);
            // then sort records
            unsigned int m = mint_list.size();
            for(unsigned int j=0; j<m-1; ++j)
            {
                for(unsigned int k=1; k<m; ++k)
                {
                    double tprevious = mint_list[k-1].min_t_in_s;
                    double tnext = mint_list[k].min_t_in_s;
                    if(tnext<tprevious)
                    {
                        DEVICE_MODEL_MIN_T previous_record = mint_list[k-1];
                        mint_list[k-1] = mint_list[k];
                        mint_list[k] = previous_record;
                    }
                }
            }
        }
    }
    n = mint_list.size();

    ostringstream osstream;
    osstream<<"The first "<<(n<max_record_count?n:max_record_count)<<" least dynamic time constants and its device and model name are listed as follows:\n"
            <<"No.    T(s)        Device              Model name\n";
    for(unsigned int i=0; i<n; ++i)
    {
        if(mint_list[i].min_t_in_s != INFINITE_THRESHOLD)
            osstream<<setw(4)<<i
                    <<setw(10)<<setprecision(4)<<mint_list[i].min_t_in_s
                    <<setw(30)<<mint_list[i].did.get_compound_device_name()
                    <<setw(10)<<mint_list[i].model_name<<"\n";
        else
            osstream<<setw(4)<<i
                    <<setw(10)<<"-infinite-"
                    <<setw(30)<<mint_list[i].did.get_compound_device_name()
                    <<setw(10)<<mint_list[i].model_name<<"\n";
    }
    toolkit->show_information_with_leading_time_stamp(osstream);
}

unsigned int DYNAMIC_MODEL_DATABASE::get_memory_usage_in_bytes()
{
    ostringstream osstream;
    if(model_starting_position_table.size()!=0)
        osstream<<"Dynamic model database warehouse used "<<model_starting_position_table[model_starting_position_table.size()-1]<<"B.";
    else
        osstream<<"Dynamic model database warehouse used 0 B.";
    toolkit->show_information_with_leading_time_stamp(osstream);
    return warehouse_capacity*sizeof(char)+model_starting_position_table.capacity()*sizeof(unsigned int);
}
