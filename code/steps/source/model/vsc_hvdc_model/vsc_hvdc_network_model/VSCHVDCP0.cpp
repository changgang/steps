#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/VSCHVDCP0.h"
#include "header/basic/utility.h"
#include "header/STEPS.h"
#include <iostream>
using namespace std;

VSCHVDCP0::VSCHVDCP0(STEPS& toolkit):VSC_HVDC_NETWORK_MODEL(toolkit)
{
}

VSCHVDCP0::VSCHVDCP0(const VSCHVDCP0& model):VSC_HVDC_NETWORK_MODEL(model.get_toolkit())
{
    copy_from_const_model(model);
}

VSCHVDCP0::~VSCHVDCP0()
{
    ;
}

VSCHVDCP0& VSCHVDCP0::operator=(const VSCHVDCP0& model)
{
    if(this == &model)
        return *this;

    copy_from_const_model(model);
    return *this;
}

void VSCHVDCP0::copy_from_const_model(const VSCHVDCP0& model)
{
    ;
}

string VSCHVDCP0::get_model_name() const
{
    return "VSCHVDCP0";
}

bool VSCHVDCP0::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=2)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            is_successful = true;

            return is_successful;
        }
        else
            return is_successful;
    }
    else
        return is_successful;
}

bool VSCHVDCP0::setup_model_with_psse_string(string data)
{
    vector<string> record = psse_dyr_string2steps_string_vector(data);
    return setup_model_with_steps_string_vector(record);
}

bool VSCHVDCP0::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void VSCHVDCP0::setup_block_toolkit_and_parameters()
{

}

void VSCHVDCP0::initialize()
{
    VSC_HVDC* vsc_hvdc  = get_vsc_hvdc_pointer();
    inphno_all_buses = vsc_hvdc->get_network_internal_physical_number_inphno();
    build_dynamic_dc_network_matrix();
}

void VSCHVDCP0::run(DYNAMIC_MODE mode)
{
    if(is_model_active())
    {
        solve_dynamic_network_with_quasi_steady_state_model();
        if(mode==UPDATE_MODE)
            set_flag_model_updated_as_true();
    }
}

void VSCHVDCP0::check()
{

}

void VSCHVDCP0::clear()
{

}

void VSCHVDCP0::report()
{
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
}

void VSCHVDCP0::save()
{

}

string VSCHVDCP0::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    VSC_HVDC* vsc = get_vsc_hvdc_pointer();
    string vsc_name = "'"+vsc->get_name()+"'";

    string model_name = "'"+get_model_name()+"'";


    osstream<<setw(8)<<vsc_name<<", "
            <<setw(10)<<model_name<<" /";
    return osstream.str();
}

void VSCHVDCP0::prepare_model_data_table()
{
    clear_model_data_table();
    //unsigned int i=0;
    //add_model_data_name_and_index_pair("TEST", i); i++;
}

double VSCHVDCP0::get_model_data_with_name(string par_name) const
{
    par_name = string2upper(par_name);
    if(is_model_data_exist(par_name))
    {
        //if(par_name == "TEST") return 0.0;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
    return 0.0;
}

void VSCHVDCP0::set_model_data_with_name(string par_name, double value)
{
    par_name = string2upper(par_name);

    if(is_model_data_exist(par_name))
    {
        //if(par_name == "TEST") return;
    }
    STEPS& toolkit = get_toolkit();
    toolkit.show_set_get_model_data_with_name_error(get_compound_device_name(), get_model_name(), __FUNCTION__, par_name);
}

double VSCHVDCP0::get_minimum_nonzero_time_constant_in_s()
{
    return 0.0;
}

void VSCHVDCP0::prepare_model_internal_variable_table()
{
    clear_model_internal_variable_table();
    //unsigned int i=0;
    //add_model_internal_variable_name_and_index_pair("TEST STATE", i); i++;
}

double VSCHVDCP0::get_model_internal_variable_with_name(string var_name)
{
    var_name = string2upper(var_name);
    //if(var_name == "TEST STATE") return 0.0;
    return 0.0;
}

string VSCHVDCP0::get_dynamic_data_in_psse_format() const
{
    return "";
}

string VSCHVDCP0::get_dynamic_data_in_bpa_format() const
{
    return "";
}

string VSCHVDCP0::get_dynamic_data_in_steps_format() const
{
    return "";
}

void VSCHVDCP0::build_dynamic_dc_network_matrix()
{
    dc_network_matrix.clear();
    dc_network_submatrix_CC.clear();
    dc_network_submatrix_CN.clear();
    dc_network_submatrix_NC.clear();
    dc_network_submatrix_NN.clear();

    if(inphno_converter_buses.get_table_size()<2)
        initialize_converter_and_nonconverter_physical_internal_bus_pair();

    build_initial_zero_matrix();
    add_dc_lines_with_fault_to_dc_network();
    dc_network_matrix.compress_and_merge_duplicate_entries();

    split_dynamic_dc_network_matrix_to_4_sub_matrix();
}


void VSCHVDCP0::initialize_converter_and_nonconverter_physical_internal_bus_pair()
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();

    unsigned int n_dcbus = vsc_hvdc->get_dc_bus_count();

    if(n_dcbus!=0)
    {
        inphno_converter_buses.clear();
        inphno_nonconverter_buses.clear();

        unsigned int count_converter = INDEX_NOT_EXIST, count_nonconverter = INDEX_NOT_EXIST;
        for(unsigned int i=0; i!=n_dcbus; ++i)
        {
            unsigned int bus_number = inphno_all_buses.get_physical_bus_number_of_internal_bus_number(i);
            unsigned int converter_index = vsc_hvdc->get_converter_index_with_dc_bus(bus_number);
            if(converter_index != INDEX_NOT_EXIST)
            {
                if(count_converter==INDEX_NOT_EXIST)
                    count_converter = 0;
                else
                    ++count_converter;
                inphno_converter_buses.set_physical_internal_bus_number_pair(bus_number, count_converter);
            }
            else
            {
                if(count_nonconverter==INDEX_NOT_EXIST)
                    count_nonconverter = 0;
                else
                    ++count_nonconverter;
                inphno_nonconverter_buses.set_physical_internal_bus_number_pair(bus_number, count_nonconverter);
            }
        }
    }
}

void VSCHVDCP0::build_initial_zero_matrix()
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int n = vsc_hvdc->get_dc_bus_count();
    for(unsigned int i=0; i!=n; ++i)
        dc_network_matrix.add_entry(i, i, 0);
}

void VSCHVDCP0::add_dc_lines_with_fault_to_dc_network()
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    unsigned int n = vsc_hvdc->get_dc_line_count();
    for(unsigned int i=0; i!=n; ++i)
    {
        if(vsc_hvdc->get_dc_line_status(i)==true)
        {
            unsigned int ibus = vsc_hvdc->get_dc_line_sending_side_bus(i);
            unsigned int jbus = vsc_hvdc->get_dc_line_receiving_side_bus(i);
            double g = 1.0/vsc_hvdc->get_dc_line_resistance_in_ohm(i);
            unsigned int i_inbus = inphno_all_buses.get_internal_bus_number_of_physical_bus_number(ibus);
            unsigned int j_inbus = inphno_all_buses.get_internal_bus_number_of_physical_bus_number(jbus);

            if(vsc_hvdc->get_dc_line_fault_r_in_ohm(i)==INFINITE_THRESHOLD)
            {
                dc_network_matrix.add_entry(i_inbus, i_inbus, g);
                dc_network_matrix.add_entry(j_inbus, j_inbus, g);
                dc_network_matrix.add_entry(i_inbus, j_inbus, -g);
                dc_network_matrix.add_entry(j_inbus, i_inbus, -g);
            }
            else
            {
                double line_r = vsc_hvdc->get_dc_line_resistance_in_ohm(i);
                double fault_r = vsc_hvdc->get_dc_line_fault_r_in_ohm(i);
                double location = vsc_hvdc->get_dc_line_fault_location(i);

                double r_iside = line_r*location;
                double r_jside = line_r*(1-location);

                double numerator = r_iside*r_jside+r_iside*fault_r+r_jside*fault_r;

                double Rij = numerator/fault_r;
                double Rif = numerator/r_jside;
                double Rjf = numerator/r_iside;

                double gij = 1.0/Rij;
                double gif = 1.0/Rif;
                double gjf = 1.0/Rjf;

                dc_network_matrix.add_entry(i_inbus, i_inbus, gij+gif);
                dc_network_matrix.add_entry(j_inbus, j_inbus, gif+gjf);
                dc_network_matrix.add_entry(i_inbus, j_inbus, -gij);
                dc_network_matrix.add_entry(j_inbus, i_inbus, -gij);
            }
        }
    }
}

void VSCHVDCP0::split_dynamic_dc_network_matrix_to_4_sub_matrix()
{
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();
    int n = dc_network_matrix.get_matrix_entry_count();
    for(int k=0; k!=n; ++k)
    {
        int i = dc_network_matrix.get_row_number_of_entry_index(k);
        int j = dc_network_matrix.get_column_number_of_entry_index(k);

        unsigned int ibus = inphno_all_buses.get_physical_bus_number_of_internal_bus_number(i);
        unsigned int jbus = inphno_all_buses.get_physical_bus_number_of_internal_bus_number(j);

        bool ibus_is_converter_bus = vsc_hvdc->get_converter_index_with_dc_bus(ibus)!=INDEX_NOT_EXIST ? true : false;
        bool jbus_is_converter_bus = vsc_hvdc->get_converter_index_with_dc_bus(jbus)!=INDEX_NOT_EXIST ? true : false;

        if(ibus_is_converter_bus and jbus_is_converter_bus)
        {
            int row = inphno_converter_buses.get_internal_bus_number_of_physical_bus_number(ibus);
            int col = inphno_converter_buses.get_internal_bus_number_of_physical_bus_number(jbus);
            dc_network_submatrix_CC.add_entry(row, col, dc_network_matrix.get_entry_value(k));
        }
        else
        {
            if(ibus_is_converter_bus and (not jbus_is_converter_bus))
            {
                int row = inphno_converter_buses.get_internal_bus_number_of_physical_bus_number(ibus);
                int col = inphno_nonconverter_buses.get_internal_bus_number_of_physical_bus_number(jbus);
                dc_network_submatrix_CN.add_entry(row, col, dc_network_matrix.get_entry_value(k));
            }
            else
            {
                if((not ibus_is_converter_bus) and jbus_is_converter_bus)
                {
                    int row = inphno_nonconverter_buses.get_internal_bus_number_of_physical_bus_number(ibus);
                    int col = inphno_converter_buses.get_internal_bus_number_of_physical_bus_number(jbus);
                    dc_network_submatrix_NC.add_entry(row, col, dc_network_matrix.get_entry_value(k));
                }
                else
                {
                    int row = inphno_nonconverter_buses.get_internal_bus_number_of_physical_bus_number(ibus);
                    int col = inphno_nonconverter_buses.get_internal_bus_number_of_physical_bus_number(jbus);
                    dc_network_submatrix_NN.add_entry(row, col, dc_network_matrix.get_entry_value(k));
                }
            }
        }
    }
    dc_network_submatrix_CC.compress_and_merge_duplicate_entries();
    dc_network_submatrix_CN.compress_and_merge_duplicate_entries();
    dc_network_submatrix_NC.compress_and_merge_duplicate_entries();
    dc_network_submatrix_NN.compress_and_merge_duplicate_entries();
}


void VSCHVDCP0::solve_dynamic_network_with_quasi_steady_state_model()
{
    /*
    IC =  YCC*UC + YCN*UN
    PN/UN = YNC*UC + YNN*UN

    solve the second one to get UN, then solve the first one to get IC
    */
    VSC_HVDC* vsc_hvdc = get_vsc_hvdc_pointer();

    unsigned int n = vsc_hvdc->get_converter_count();
    unsigned int m = vsc_hvdc->get_dc_bus_count();
    unsigned int n_nonconverter_bus = m - n;

    vector<double> UC;
    for(unsigned int i=0; i!=n; ++i)
        UC.push_back(0.0);

    for(unsigned int i=0; i!=n; ++i)
    {
        unsigned int dcbus_index = vsc_hvdc->get_dc_bus_index_with_converter_index(i);
        unsigned int dcbus = vsc_hvdc->get_dc_bus_number(dcbus_index);
        int index = inphno_converter_buses.get_internal_bus_number_of_physical_bus_number(dcbus);
        UC[index] = vsc_hvdc->get_dc_bus_Vdc_in_kV(dcbus_index);
    }

    if(n_nonconverter_bus!=0)
    {
        vector<double> IN_C = dc_network_submatrix_NC*UC;

        vector<double> delta_IN;
        for(unsigned int i=0; i!=n_nonconverter_bus; ++i)
            delta_IN.push_back(0.0);
        vector<double> UN;
        iteration_count = 0;
        while(true)
        {
            for(unsigned int i=0; i!=n_nonconverter_bus; ++i)
            {
                unsigned int dcbus = inphno_nonconverter_buses.get_physical_bus_number_of_internal_bus_number(i);
                unsigned int index = vsc_hvdc->dc_bus_no2index(dcbus);
                double Pload = vsc_hvdc->get_dc_bus_load_power_in_MW(index);
                double Pgen = vsc_hvdc->get_dc_bus_generation_power_in_MW(index);
                double Udc = vsc_hvdc->get_dc_bus_Vdc_in_kV(index);
                delta_IN[i] = (Pgen-Pload)/Udc;
                delta_IN[i] -= IN_C[i];
            }
            UN = delta_IN/dc_network_submatrix_NN;
            for(unsigned int i=0; i!=n_nonconverter_bus; ++i)
            {
                unsigned int dcbus = inphno_nonconverter_buses.get_physical_bus_number_of_internal_bus_number(i);
                unsigned int index = vsc_hvdc->dc_bus_no2index(dcbus);
                vsc_hvdc->set_dc_bus_Vdc_in_kV(index, UN[i]);
            }
            for(unsigned int i=0; i!=n_nonconverter_bus; ++i)
            {
                unsigned int dcbus = inphno_nonconverter_buses.get_physical_bus_number_of_internal_bus_number(i);
                unsigned int index = vsc_hvdc->dc_bus_no2index(dcbus);
                double Pload = vsc_hvdc->get_dc_bus_load_power_in_MW(index);
                double Pgen = vsc_hvdc->get_dc_bus_generation_power_in_MW(index);
                delta_IN[i] = (Pgen-Pload)/UN[i];
                delta_IN[i] -= IN_C[i];
            }
            vector<double> IN_N = dc_network_submatrix_NN*UN;
            double max_P_error = 0.0;
            for(unsigned int i=0; i!=n_nonconverter_bus; ++i)
            {
                delta_IN[i] -= IN_N[i];
                double P_error = delta_IN[i] * UN[i];
                if(fabs(P_error)>max_P_error)
                    max_P_error = fabs(P_error);
            }
            if(max_P_error<vsc_hvdc->get_allowed_max_active_power_imbalance_in_MW())
                break;
            ++iteration_count;
            if(iteration_count>vsc_hvdc->get_max_iteration())
            {
                ostringstream osstream;
                osstream<<"VSC_HVDC::"<<__FUNCTION__<<"() failed to converge.";
                STEPS& toolkit = get_toolkit();
                toolkit.show_information_with_leading_time_stamp(osstream);
                break;
            }
        }
        vector<double> IC, IC_N;
        IC = dc_network_submatrix_CC*UC;
        IC_N = dc_network_submatrix_CN*UN;

        for(unsigned int i=0; i!=n; ++i)
        {
            int dcbus = inphno_converter_buses.get_physical_bus_number_of_internal_bus_number(i);
            unsigned int index = vsc_hvdc->get_converter_index_with_dc_bus(dcbus);
            IC[i] += IC_N[i];
            double Pdc = IC[i]*UC[i];
            vsc_hvdc->set_converter_Pdc_from_Ceq_to_DC_network_in_MW(index, Pdc);
        }
    }
    else
    {
        vector<double> IC = dc_network_submatrix_CC*UC;

        for(unsigned int i=0; i!=n; ++i)
        {
            int dcbus = inphno_converter_buses.get_physical_bus_number_of_internal_bus_number(i);
            unsigned int index = vsc_hvdc->get_converter_index_with_dc_bus(dcbus);
            double Pdc = IC[i]*UC[i];
            vsc_hvdc->set_converter_Pdc_from_Ceq_to_DC_network_in_MW(index, Pdc);
        }

    }
}
