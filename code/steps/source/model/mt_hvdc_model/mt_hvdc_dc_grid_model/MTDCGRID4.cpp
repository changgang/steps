#include "header/model/mt_hvdc_model/mt_hvdc_dc_grid_model/MTDCGRID4.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <cstdio>
#include <istream>
#include <iostream>

using namespace std;

MTDCGRID4::MTDCGRID4(STEPS& toolkit) : MT_DC_GRID_MODEL(toolkit)
 {
     clear();
 }

MTDCGRID4::~MTDCGRID4()
 {
     ;
 }

MTDCGRID4::MTDCGRID4(const MTDCGRID4& model) : MT_DC_GRID_MODEL(model.get_toolkit())
 {
    copy_from_const_model(model);
 }

MTDCGRID4& MTDCGRID4::operator=(const MTDCGRID4& model)
 {
    if(this == &model)
        return *this;

    copy_from_const_model(model);
    return *this;
 }

void MTDCGRID4::copy_from_const_model(const MTDCGRID4& model)
{
    STEPS& toolkit = get_toolkit();
    set_toolkit(toolkit);

    clear();
    set_dc_network_simulation_time_step(model.get_dc_network_simulation_time_step());
    set_is_dc_network_dynamics_consider(model.get_is_dc_network_dynamics_consider());
}

string MTDCGRID4::get_model_name() const
{
    return "MTDCGRID4";
}

void MTDCGRID4::set_dc_network_simulation_time_step(double t_dc)
{
    this->dc_network_simulation_time_step = t_dc;
}

void MTDCGRID4::set_is_dc_network_dynamics_consider(bool is_dc_dynamic_consider)
{
    this->is_dc_network_dynamic_considered = is_dc_dynamic_consider;
}

DC_LINE* MTDCGRID4::get_dc_line_with_virtual_bus_number(unsigned int bus_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        DC_BUS* bus = dc_lines[i]->get_dc_line_virtual_bus();
        if(bus->get_dc_bus_number()==bus_number)
        {
            return dc_lines[i];
        }
    }
    osstream<<"NO DC LINE WITH VIRTUAL BUS NUMBER: "<<bus_number;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

DC_BUS* MTDCGRID4::get_virtual_bus_with_virtual_bus_number(unsigned int bus_number)
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        DC_BUS* bus = dc_lines[i]->get_dc_line_virtual_bus();
        if(bus->get_dc_bus_number()==bus_number)
        {
            return bus;
        }
    }
    osstream<<"NO DC VIRTUAL WITH VIRTUAL BUS NUMBER: "<<bus_number;
    toolkit.show_information_with_leading_time_stamp(osstream);
    return NULL;
}

unsigned int MTDCGRID4::get_number_of_dc_buses_with_dynamic_UR_type()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();
    unsigned int n_ur = 0;
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        if(dc_buses[i]->get_dc_bus_dynamic_type()==DYNAMIC_DC_UR_BUS)
        {
            n_ur++;
        }
    }
    return n_ur;
}

bool MTDCGRID4::is_internal_bus_in_U_type(unsigned int bus)
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();

    unsigned int bus_number = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(bus);
    DC_BUS* dcbus = mtdc->get_dc_bus_with_bus_number(bus_number);
    if((dcbus->get_dc_bus_dynamic_type())==DYNAMIC_DC_U_BUS)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool MTDCGRID4::check_is_dc_line_fault_exit()
{
    is_dc_line_fault_exit = false;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        double fault_R = dc_lines[i]->get_dc_line_fault_R_in_ohm();
        if(fault_R == INFINITE_THRESHOLD)
        {
            ;
        }
        else
        {
            is_dc_line_fault_exit = true;
            return true;
        }
    }
    return is_dc_line_fault_exit;
}


void MTDCGRID4::initialize_dc_buses()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = buses.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        double req = mtdc->get_dc_bus_req_in_dynamic_simulation(buses[i]->get_dc_bus_number());
        if(req == 0)
        {
            buses[i]->set_dc_bus_dynamic_type(DYNAMIC_DC_U_BUS);
        }
        else
        {
            buses[i]->set_dc_bus_dynamic_type(DYNAMIC_DC_UR_BUS);
        }
    }
}

void MTDCGRID4::initialize_dc_lines()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    double h_dc = get_dc_network_simulation_time_step();
    //unsigned int n_dc_bus = mtdc->get_dc_bus_count();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        dc_lines[i]->set_dc_network_simulation_time_step(h_dc);

        unsigned int i_bus = dc_lines[i]->get_sending_side_bus_number();
        unsigned int j_bus = dc_lines[i]->get_receiving_side_bus_number();
        DC_BUS* ibus = mtdc->get_dc_bus_with_bus_number(i_bus);
        DC_BUS* jbus = mtdc->get_dc_bus_with_bus_number(j_bus);
        double ivoltage = ibus->get_dc_bus_voltage_in_kV();
        double jvoltage = jbus->get_dc_bus_voltage_in_kV();

        double R = dc_lines[i]->get_line_R_in_ohm();
        double idc = (ivoltage-jvoltage)/R;
        dc_lines[i]->set_i_RL_in_kA_on_sending_side(idc);
    }
}

void MTDCGRID4::initialize_dynamic_physical_internal_dc_bus_pair()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();
    if(n_dcbus!=0)
    {
        dynamic_inphno_dc_buses.clear();
        unsigned int internal_bus_number = 0;
        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DYNAMIC_DC_BUS_TYPE type = dc_buses[i]->get_dc_bus_dynamic_type();
            if(type==DYNAMIC_DC_UR_BUS)
            {
                unsigned int bus_number = dc_buses[i]->get_dc_bus_number();
                dynamic_inphno_dc_buses.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }

        for(unsigned int i=0;i!=n_dcbus;i++)
        {
            DYNAMIC_DC_BUS_TYPE type = dc_buses[i]->get_dc_bus_dynamic_type();
            if(type==DYNAMIC_DC_U_BUS)
            {
                unsigned int bus_number = dc_buses[i]->get_dc_bus_number();
                dynamic_inphno_dc_buses.set_physical_internal_bus_number_pair(bus_number, internal_bus_number);
                ++internal_bus_number;
            }
        }
    }
}


void MTDCGRID4::build_dc_network_matrix_with_dynamic()
{
    if(dynamic_inphno_dc_buses.get_table_size()<1)
        initialize_dynamic_physical_internal_dc_bus_pair();

    dynamic_dc_network_matrix.clear();
    build_dc_network_initial_zero_dynamic_matrix();
    add_dc_line_to_dc_dynamic_network_matrix();
    add_converter_to_dc_dynamic_network_matrix();
    dynamic_dc_network_matrix.compress_and_merge_duplicate_entries();
    //show_dc_network_matrix();
}

void MTDCGRID4::build_dc_network_initial_zero_dynamic_matrix()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    unsigned int n_dc_bus = mtdc->get_dc_bus_count();
    unsigned int n_dyr_matrix = n_dc_bus;
    for(unsigned int i=0;i!=n_dyr_matrix;i++)
    {
        dynamic_dc_network_matrix.add_entry(i,i,0);
    }
}

void MTDCGRID4::add_dc_line_to_dc_dynamic_network_matrix()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        double R = dc_lines[i]->get_line_R_in_ohm();

        unsigned int i_physical_bus = dc_lines[i]->get_sending_side_bus_number();
        unsigned int j_physical_bus = dc_lines[i]->get_receiving_side_bus_number();

        unsigned int i_bus = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(i_physical_bus);
        unsigned int j_bus = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(j_physical_bus);

        dynamic_dc_network_matrix.add_entry(i_bus,i_bus,1/R);
        dynamic_dc_network_matrix.add_entry(i_bus,j_bus,-1/R);
        dynamic_dc_network_matrix.add_entry(j_bus,i_bus,-1/R);
        dynamic_dc_network_matrix.add_entry(j_bus,j_bus,1/R);

    }
}

void MTDCGRID4::add_converter_to_dc_dynamic_network_matrix()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        if(dc_buses[i]->get_dc_bus_dynamic_type()==DYNAMIC_DC_UR_BUS)
        {
            unsigned int bus_number = dc_buses[i]->get_dc_bus_number();
            double req = mtdc->get_dc_bus_req_in_dynamic_simulation(bus_number);
            unsigned int internal_bus = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(bus_number);
            dynamic_dc_network_matrix.add_entry(internal_bus,internal_bus,1/req);
        }
    }
}


void MTDCGRID4::build_dc_network_matrix_without_dynamic()
{
     if(dynamic_inphno_dc_buses.get_table_size()<1)
        initialize_dynamic_physical_internal_dc_bus_pair();

    dynamic_dc_network_matrix.clear();
    build_dc_network_initial_zero_dynamic_matrix();
    add_dc_line_to_dc_dynamic_network_matrix();
    add_converter_to_dc_dynamic_network_matrix();
    dynamic_dc_network_matrix.compress_and_merge_duplicate_entries();
    //show_dc_network_matrix();

}

void MTDCGRID4::split_dynamic_dc_network_matrix_to_4_sub_matrix()
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    unsigned int nc = 0;
    unsigned int nu = 0;
    unsigned int n_dcbus = mtdc->get_dc_bus_count();
    unsigned int n_bus_ur = get_number_of_dc_buses_with_dynamic_UR_type();
    unsigned int n_bus_u = n_dcbus-n_bus_ur;
    nc = n_bus_ur;
    nu = n_bus_u;

    //build zero sub matrix
    dynamic_submatrix_YCC.clear();
    dynamic_submatrix_YCU.clear();
    dynamic_submatrix_YUC.clear();
    dynamic_submatrix_YUU.clear();

    unsigned int n = dynamic_dc_network_matrix.get_matrix_entry_count();
    for(unsigned int k=0;k!=n;k++)
    {
        unsigned int i = dynamic_dc_network_matrix.get_row_number_of_entry_index(k);
        unsigned int j = dynamic_dc_network_matrix.get_column_number_of_entry_index(k);

        bool ibus_is_dynamic_ubus = is_internal_bus_in_U_type(i) ? true : false;
        bool jbus_is_dynamic_ubus = is_internal_bus_in_U_type(j) ? true : false;


        if((not ibus_is_dynamic_ubus) and (not jbus_is_dynamic_ubus))
        {
            dynamic_submatrix_YCC.add_entry(i,j,dynamic_dc_network_matrix.get_entry_value(k));
        }
        else if((not ibus_is_dynamic_ubus) and (jbus_is_dynamic_ubus))
        {
            dynamic_submatrix_YCU.add_entry(i,j-nc,dynamic_dc_network_matrix.get_entry_value(k));
        }
        else if((ibus_is_dynamic_ubus) and (not jbus_is_dynamic_ubus))
        {
            dynamic_submatrix_YUC.add_entry(i-nc,j,dynamic_dc_network_matrix.get_entry_value(k));
        }
        else // U bus
        {
            dynamic_submatrix_YUU.add_entry(i-nc,j-nc,dynamic_dc_network_matrix.get_entry_value(k));
        }
    }

    dynamic_submatrix_YCC.compress_and_merge_duplicate_entries();
    dynamic_submatrix_YCU.compress_and_merge_duplicate_entries();
    dynamic_submatrix_YUC.compress_and_merge_duplicate_entries();
    dynamic_submatrix_YUU.compress_and_merge_duplicate_entries();
    /*
    show_sub_matrix_YCC();
    show_sub_matrix_YCU();
    show_sub_matrix_YUC();
    show_sub_matrix_YUU();
    */
}

void MTDCGRID4::initialize_dc_bus_injection_current()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();
    unsigned int n = 0;
    n = n_dcbus;

    injection_current.resize(n);
    for(unsigned int i=0;i!=n;i++)
    {
        injection_current[i] = 0;
    }

    for(unsigned int i=0;i!=n;i++)
    {
        unsigned int bus_number = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);
        double ueq = mtdc->get_dc_bus_ueq_in_dynamic_simulation(bus_number);
        double req  = mtdc->get_dc_bus_req_in_dynamic_simulation(bus_number);
        if(req!=0)
        {
            injection_current[i] = ueq/req;
        }
        else
        {
            DC_BUS* dcbus = mtdc->get_dc_bus_with_bus_number(bus_number);
            double idc = dcbus->get_dc_bus_current_in_kA();
            injection_current[i] = idc;
        }

    }

}

void MTDCGRID4::add_dc_line_to_injection_current()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        unsigned int i_physical_bus = dc_lines[i]->get_sending_side_bus_number();
        unsigned int j_physical_bus = dc_lines[i]->get_receiving_side_bus_number();
        DC_BUS* ibus = mtdc->get_dc_bus_with_bus_number(i_physical_bus);
        DC_BUS* jbus = mtdc->get_dc_bus_with_bus_number(j_physical_bus);

        double Ui = dc_lines[i]->get_dc_line_history_Ui_in_kV();
        double Uj = dc_lines[i]->get_dc_line_history_Uj_in_kV();

        //double Ui = ibus->get_dc_bus_voltage_in_kV();
        //double Uj = jbus->get_dc_bus_voltage_in_kV();
        //double Uv = vbus->get_dc_bus_voltage_in_kV();

        double IL0 = dc_lines[i]->get_i_RL_in_kA_on_sending_side();

        double Ieq_L = dc_lines[i]->get_line_dynamic_injection_current_of_RL_in_kA_in_implicit_trapezoid_method(1,Ui,Uj,IL0);

        double i_internal = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(i_physical_bus);
        double j_internal = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(j_physical_bus);


        injection_current[i_internal] = injection_current[i_internal]-Ieq_L;
        injection_current[j_internal] = injection_current[j_internal]+Ieq_L;

    }
}

void MTDCGRID4::initialize_dc_bus_voltage_vector()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();

    unsigned int n = 0;
    n = n_dcbus;

    bus_voltage.resize(n);
    for(unsigned int i=0;i!=n;i++)
    {
        bus_voltage[i] = 0;
    }
    //ostringstream osstream;
    //osstream<<"NUMBER OF BUS IS :"<<n;
    //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    for(unsigned int i=0;i!=n;i++)
    {
        if(is_internal_bus_in_U_type(i))
        {
            double bus_number = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);
            double ueq = mtdc->get_dc_bus_ueq_in_dynamic_simulation(bus_number);
            bus_voltage[i] = ueq;
            //osstream<<"voltage of U bus :"<<i<<"  is :  "<<ueq<< " and "<<bus_voltage[i];
            //show_information_with_leading_time_stamp_with_default_toolkit(osstream);

        }
        else
        {
            unsigned int bus_number = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);
            DC_BUS* dcbus = mtdc->get_dc_bus_with_bus_number(bus_number);
            double voltage = dcbus->get_dc_bus_voltage_in_kV();
            bus_voltage[i] = voltage;
            /*
            ostringstream osstream;
            osstream<<"voltage of U bus :"<<i<<"  is :  "<<voltage<< " and "<<bus_voltage[i];
            show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            */
        }
    }
}


void MTDCGRID4::update_dc_network_matrix()
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();
    bool update_flag = false;
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        DYNAMIC_DC_BUS_TYPE type = dc_buses[i]->get_dc_bus_dynamic_type();
        if(type==DYNAMIC_DC_UR_BUS)
        {
            double req = mtdc->get_dc_bus_req_in_dynamic_simulation(dc_buses[i]->get_dc_bus_number());
            if(req==0)
            {
                dc_buses[i]->set_dc_bus_dynamic_type(DYNAMIC_DC_U_BUS);
                update_flag = true;
                osstream<<"DC BUS DYNAMIC will be update with DC BUS:"<<dc_buses[i]->get_dc_bus_number()<<"  FROM UR BUS TO U BUS";
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            }
        }
        if(type == DYNAMIC_DC_U_BUS)
        {
            double req = mtdc->get_dc_bus_req_in_dynamic_simulation(dc_buses[i]->get_dc_bus_number());
            if(req!=0)
            {
                dc_buses[i]->set_dc_bus_dynamic_type(DYNAMIC_DC_UR_BUS);
                update_flag = true;
                osstream<<"DC BUS DYNAMIC will be update with DC BUS:"<<dc_buses[i]->get_dc_bus_number()<<"  FROM U BUS TO UR BUS";
                show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            }
        }
    }
    if(update_flag)
    {
        osstream<<"AFTER UPDATE THE DC DYNAMIC MATRIX";
        initialize_dynamic_physical_internal_dc_bus_pair();
        build_dc_network_matrix_without_dynamic();
        show_dc_network_matrix();
        split_dynamic_dc_network_matrix_to_4_sub_matrix();

        show_sub_matrix_YCC();
        show_sub_matrix_YCU();
        show_sub_matrix_YUC();
        show_sub_matrix_YUU();

    }

}


void MTDCGRID4::update_dc_bus_injection_current()
{
    //ostringstream osstream;
    //STEPS& toolkit = get_toolkit();
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    unsigned int n = injection_current.size();;
    for(unsigned int i=0;i!=n;i++)
    {
        injection_current[i] = 0.0;

        if(is_internal_bus_in_U_type(i))
        {
            ;
        }
        else
        {

            unsigned int bus_number = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);
            double ueq = mtdc->get_dc_bus_ueq_in_dynamic_simulation(bus_number);
            double req = mtdc->get_dc_bus_req_in_dynamic_simulation(bus_number);
            injection_current[i] = ueq/req;
        }
    }
}

void MTDCGRID4::update_dc_bus_voltage()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    unsigned int n = bus_voltage.size();;
    for(unsigned int i=0;i!=n;i++)
    {
        if(is_internal_bus_in_U_type(i))
        {
            unsigned int bus_number = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);
            double ueq = mtdc->get_dc_bus_ueq_in_dynamic_simulation(bus_number);
            bus_voltage[i] = ueq;
        }
    }
}

void MTDCGRID4::add_dc_fault_to_dc_network()
{
    return;
}

void MTDCGRID4::solve_dc_network_without_line_dynamics()
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    unsigned int nc = 0;
    unsigned int nu = 0;
    unsigned int n_dcbus = mtdc->get_dc_bus_count();
    unsigned int n_bus_ur = get_number_of_dc_buses_with_dynamic_UR_type();
    unsigned int n_bus_u = n_dcbus-n_bus_ur;
    nc = n_bus_ur;
    nu = n_bus_u;


    vector<double> IC;
    for(unsigned int i=0;i!=nc;i++)
        IC.push_back(0.0);

    vector<double> UC;
    for(unsigned int i=0;i!=nc;i++)
        UC.push_back(0.0);

    vector<double> IU;
    for(unsigned int i=0;i!=nu;i++)
        IU.push_back(0.0);

    vector<double> UU;
    for(unsigned int i=0;i!=nu;i++)
        UU.push_back(0.0);


    for(unsigned int i=0;i!=nc;i++)
    {
        IC[i] = injection_current[i];
        UC[i] = 0;
        //UC[i] = bus_voltage[i];
    }

    for(unsigned int i=0;i!=nu;i++)
    {
        //IU[i] = injection_current[i+nc];
        IU[i] = 0;
        UU[i] = bus_voltage[i+nc];
    }

    //voltage calculation
    if(nu==0)
    {
        UC = IC/dynamic_submatrix_YCC;
    }
    if(nc==0)
    {
        IU = dynamic_submatrix_YUU*UU;
    }

    if((nu!=0) and (nc!=0))
    {
        vector<double> ICU = dynamic_submatrix_YCU*UU;
        vector<double> ICC;
        for(unsigned int i=0;i!=nc;i++)
                ICC.push_back(0.0);
        for(unsigned int i=0;i!=nc;i++)
        {
                ICC[i] = IC[i]-ICU[i];
        }
        UC = ICC/dynamic_submatrix_YCC;

        vector<double> IUC = dynamic_submatrix_YUC*UC;
        vector<double> IUU = dynamic_submatrix_YUU*UU;
        for(unsigned int i=0;i!=nu;i++)
        {
            IU[i] = IUC[i]+IUU[i];
        }
    }

    //update bus voltage and current
    for(unsigned int i=0;i!=nu;i++)
    {
        injection_current[i+nc] = IU[i];

    }
    for(unsigned int i=0;i!=nc;i++)
    {
        bus_voltage[i] = UC[i];
    }

}

void MTDCGRID4::solve_dc_network_with_line_dynamics()
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    unsigned int nc = 0;
    unsigned int nu = 0;
    unsigned int n_dcbus = mtdc->get_dc_bus_count();
    unsigned int n_bus_ur = get_number_of_dc_buses_with_dynamic_UR_type();
    unsigned int n_bus_u = n_dcbus-n_bus_ur;
    nu = n_bus_u;
    nc = n_bus_ur;

    vector<double> IC;
    for(unsigned int i=0;i!=nc;i++)
        IC.push_back(0.0);

    vector<double> UC;
    for(unsigned int i=0;i!=nc;i++)
        UC.push_back(0.0);

    vector<double> IU;
    for(unsigned int i=0;i!=nu;i++)
        IU.push_back(0.0);

    vector<double> UU;
    for(unsigned int i=0;i!=nu;i++)
        UU.push_back(0.0);



    for(unsigned int i=0;i!=nc;i++)
    {
        IC[i] = injection_current[i];
        UC[i] = 0;
        //UC[i] = bus_voltage[i];
    }

    for(unsigned int i=0;i!=nu;i++)
    {
        //IU[i] = injection_current[i+nc];
        IU[i] = 0;
        UU[i] = bus_voltage[i+nc];

    }

    //voltage calculation
    if(nu==0)
    {
        UC = IC/dynamic_submatrix_YCC;
    }
    if(nc==0)
    {
        IU = dynamic_submatrix_YUU*UU;
    }

    if((nu!=0) and (nc!=0))
    {
        vector<double> ICU = dynamic_submatrix_YCU*UU;
        vector<double> ICC;
        for(unsigned int i=0;i!=nc;i++)
                ICC.push_back(0.0);
        for(unsigned int i=0;i!=nc;i++)
        {
                ICC[i] = IC[i]-ICU[i];
        }
        UC = ICC/dynamic_submatrix_YCC;

        vector<double> IUC = dynamic_submatrix_YUC*UC;
        vector<double> IUU = dynamic_submatrix_YUU*UU;
        for(unsigned int i=0;i!=nu;i++)
        {
            IU[i] = IUC[i]+IUU[i];
        }
    }


    //update bus voltage and current
    for(unsigned int i=0;i!=nu;i++)
    {
        injection_current[i+nc] = IU[i];

    }
    for(unsigned int i=0;i!=nc;i++)
    {
        bus_voltage[i] = UC[i];
    }

}


void MTDCGRID4::update_dc_bus_volatge_and_current()
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> dc_buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = dc_buses.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        unsigned int bus_number = dc_buses[i]->get_dc_bus_number();
        unsigned int internal_number = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(bus_number);
        double udc = bus_voltage[internal_number];
        dc_buses[i]->set_dc_bus_voltage_in_kV(udc);

        //update current:
        if(not is_internal_bus_in_U_type(internal_number))
        {
            double ueq = mtdc->get_dc_bus_ueq_in_dynamic_simulation(bus_number);
            double req = mtdc->get_dc_bus_req_in_dynamic_simulation(bus_number);
            double idc = (ueq-udc)/req;
            dc_buses[i]->set_dc_bus_current_in_kA(idc);

            //osstream<<"CURRENT OF UR DC BUS :"<<bus_number<<"IS UPDATE TO: "<<idc;
            //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        }
        else //U BUS
        {
            //osstream<<"BEGIN TO UPDATE IDC OF U BUS :"<<bus_number;
            //show_information_with_leading_time_stamp_with_default_toolkit(osstream);
            dc_buses[i]->set_dc_bus_current_in_kA(injection_current[internal_number]);
        }
    }

}

void MTDCGRID4::update_dc_line_voltage_and_current()
{
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_LINE*> dc_lines = mtdc->get_all_dc_lines();
    unsigned int n_dcline = dc_lines.size();
    for(unsigned int i=0;i!=n_dcline;i++)
    {
        unsigned int i_physical_bus = dc_lines[i]->get_sending_side_bus_number();
        unsigned int j_physical_bus = dc_lines[i]->get_receiving_side_bus_number();


        unsigned int i_internal_bus = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(i_physical_bus);
        unsigned int j_internal_bus = dynamic_inphno_dc_buses.get_internal_bus_number_of_physical_bus_number(j_physical_bus);

        double Ui = bus_voltage[i_internal_bus];
        double Uj = bus_voltage[j_internal_bus];
        double R = dc_lines[i]->get_line_R_in_ohm();

        double idc = (Ui-Uj)/R;
        dc_lines[i]->set_i_RL_in_kA_on_sending_side(idc);

    }
}

void MTDCGRID4::run_a_step()
{

    //STEPS& toolkit = get_toolkit();
    check_is_dc_line_fault_exit();
    update_dc_network_matrix();
    update_dc_bus_injection_current();
    update_dc_bus_voltage();
    /*
    ostringstream osstream;
    osstream<<" ********** BEFORE SOLVE DC NETWORK  ";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    show_bus_injection_current();
    show_dc_bus_voltage();
    */


    solve_dc_network_without_line_dynamics();

    update_dc_bus_volatge_and_current();
    update_dc_line_voltage_and_current();
    /*
    osstream<<" ********** AFTER SOLVE DC NETWORK  ";
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    show_bus_injection_current();
    show_dc_bus_voltage();
    */
}


double MTDCGRID4::get_dc_network_simulation_time_step() const
{
    return dc_network_simulation_time_step;
}

bool MTDCGRID4::get_is_dc_network_dynamics_consider() const
{
    return is_dc_network_dynamic_considered;
}

bool MTDCGRID4::setup_model_with_steps_string_vector(vector<string>& data)
{
    bool is_successful = false;
    if(data.size()>=4)
    {
        string model_name = get_string_data(data[0],"");
        if(model_name==get_model_name())
        {
            unsigned int data_index=2;
            double dc_simulation_time_step = get_double_data(data[data_index],"0.001"); data_index++;
            set_dc_network_simulation_time_step(dc_simulation_time_step);

            int is_dc_line_dynamic_considered = get_double_data(data[data_index],"0"); data_index++;
            set_is_dc_network_dynamics_consider(false);

            is_successful = true;
        }
        else
            return is_successful;
    }
    return is_successful;
}

bool MTDCGRID4::setup_model_with_psse_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}
bool MTDCGRID4::setup_model_with_bpa_string(string data)
{
    ostringstream osstream;
    osstream<<get_model_name()<<"::"<<__FUNCTION__<<"() is not fully supported to set up model with following data:"<<endl
            <<data;
    STEPS& toolkit = get_toolkit();
    toolkit.show_information_with_leading_time_stamp(osstream);
    return false;
}

void MTDCGRID4::setup_block_toolkit_and_parameters()
{
    ;
}

void MTDCGRID4::initialize()
{
    if(not is_model_initialized())
    {
        ostringstream osstream;
        setup_block_toolkit_and_parameters();

        initialize_dc_buses();
        initialize_dc_lines();

        initialize_dynamic_physical_internal_dc_bus_pair();
        build_dc_network_matrix_without_dynamic();
        split_dynamic_dc_network_matrix_to_4_sub_matrix();

        initialize_dc_bus_injection_current();
        initialize_dc_bus_voltage_vector();

        /*
        osstream<<" INITIALIZE RESULT OF DC NETWORK " <<endl;
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
        show_dc_network_matrix();
        show_sub_matrix_YCC();
        show_sub_matrix_YCU();
        show_sub_matrix_YUC();
        show_sub_matrix_YUU();
        show_dc_bus_voltage();
        show_bus_injection_current();
        */


        set_flag_model_initialized_as_true();
    }
}

void MTDCGRID4::run(DYNAMIC_MODE mode)
{
    run_a_step();

    if(mode==DYNAMIC_UPDATE_MODE)
        set_flag_model_updated_as_true();
}

void MTDCGRID4::check()
{
    ;
}

void MTDCGRID4::clear()
{
    dc_network_simulation_time_step = 0.001;
    is_dc_network_dynamic_considered = false;
    is_dc_line_fault_exit = false;
    dynamic_inphno_dc_buses.clear();
    bus_voltage.clear();
    injection_current.clear();
}

void MTDCGRID4::report()
{
    STEPS& toolkit = get_toolkit();
    ostringstream osstream;
    osstream<<get_standard_psse_string(false);
    toolkit.show_information_with_leading_time_stamp(osstream);;
}

void MTDCGRID4::save()
{
    ;
}

string MTDCGRID4::get_standard_psse_string(bool export_internal_bus_number) const
{
    ostringstream osstream;
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    string mtdc_name = "'"+mtdc->get_name()+"'";
    string model_name = "'"+get_model_name()+"'";
    string is_dc_dynamic = "";
    if(get_is_dc_network_dynamics_consider()==true)
    {
        is_dc_dynamic = "DC DYNAMICS IS CONSIDER";
    }
    else
        is_dc_dynamic = "DC DYNAMICS NOT CONSIDER";

    osstream<<setw(16)<<mtdc_name<<", "
            <<setw(10)<<model_name<<", \n"
            <<setw(10)<<""
            <<setw(8)<<setprecision(4)<<get_dc_network_simulation_time_step()<<", "
            <<setw(10)<<setprecision(4)<<is_dc_dynamic<<", \n";
    return osstream.str();
}

void MTDCGRID4::prepare_model_data_table()
{
    ;
}

double MTDCGRID4::get_model_data_with_name(string par_name) const
{
    return 0.0;
}
void MTDCGRID4::set_model_data_with_name(string par_name, double value)
{
    return;
}
double MTDCGRID4::get_minimum_nonzero_time_constant_in_s()
{
    return INFINITE_THRESHOLD;;
}

void MTDCGRID4::prepare_model_internal_variable_table()
{
    ;
}

double MTDCGRID4::get_model_internal_variable_with_name(string var_name)
{
    return 0.0;;
}


string MTDCGRID4::get_dynamic_data_in_psse_format() const
{
    return "";
}

string MTDCGRID4::get_dynamic_data_in_bpa_format() const
{
    return "";
}
string MTDCGRID4::get_dynamic_data_in_steps_format() const
{
    return "";
}


void MTDCGRID4::show_dc_network_matrix()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = dynamic_dc_network_matrix.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = dynamic_dc_network_matrix.get_starting_index_of_column(j);
        int k_end = dynamic_dc_network_matrix.get_starting_index_of_column(j+1);
        unsigned int jbus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = dynamic_dc_network_matrix.get_row_number_of_entry_index(k);
            unsigned int ibus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,dynamic_dc_network_matrix.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}
void MTDCGRID4::show_sub_matrix_YCC()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="YCC matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = dynamic_submatrix_YCC.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = dynamic_submatrix_YCC.get_starting_index_of_column(j);
        int k_end = dynamic_submatrix_YCC.get_starting_index_of_column(j+1);
        unsigned int jbus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = dynamic_submatrix_YCC.get_row_number_of_entry_index(k);
            unsigned int ibus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,dynamic_submatrix_YCC.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}
void MTDCGRID4::show_sub_matrix_YCU()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="YCU matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = dynamic_submatrix_YCU.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = dynamic_submatrix_YCU.get_starting_index_of_column(j);
        int k_end = dynamic_submatrix_YCU.get_starting_index_of_column(j+1);
        unsigned int jbus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = dynamic_submatrix_YCU.get_row_number_of_entry_index(k);
            unsigned int ibus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,dynamic_submatrix_YCU.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}
void MTDCGRID4::show_sub_matrix_YUC()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="YUC matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = dynamic_submatrix_YUC.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = dynamic_submatrix_YUC.get_starting_index_of_column(j);
        int k_end = dynamic_submatrix_YUC.get_starting_index_of_column(j+1);
        unsigned int jbus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = dynamic_submatrix_YUC.get_row_number_of_entry_index(k);
            unsigned int ibus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,dynamic_submatrix_YUC.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}

void MTDCGRID4::show_sub_matrix_YUU()
{
    string buffer;
    char cbuffer[1000];

    int i=0, j=0, k=0;

    buffer="YUU matrix entries:\n";
    show_information_with_leading_time_stamp_with_default_toolkit(buffer);

	snprintf(cbuffer, 1000, "row   rowbus     column    colbus     value\n");
    show_information_with_leading_time_stamp_with_default_toolkit(cbuffer);

    int n = dynamic_submatrix_YUU.get_matrix_size();
    for(j=0;j!=n;++j)
    {
        int k_start = dynamic_submatrix_YUU.get_starting_index_of_column(j);
        int k_end = dynamic_submatrix_YUU.get_starting_index_of_column(j+1);
        unsigned int jbus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(j);
        for(k=k_start;k<k_end;++k)
        {
            i = dynamic_submatrix_YUU.get_row_number_of_entry_index(k);
            unsigned int ibus = dynamic_inphno_dc_buses.get_physical_bus_number_of_internal_bus_number(i);

            snprintf(cbuffer,1000, "%-6d, %-6d, %-6d, %-6d, % 10.6f\n",i,ibus,j,jbus,dynamic_submatrix_YUU.get_real_entry_value(k));

            buffer = cbuffer;
            show_information_with_leading_time_stamp_with_default_toolkit(buffer);
        }
    }
}


void MTDCGRID4::show_dc_bus_voltage()
{
    ostringstream osstream;
    osstream<<" dc bus voltage " ;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = buses.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        osstream<<" bus voltage of dc bus" <<buses[i]->get_dc_bus_number()<<" is:"<<buses[i]->get_dc_bus_voltage_in_kV();
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
}

void MTDCGRID4::show_dc_bus_current()
{
    ostringstream osstream;
    osstream<<" dc bus current " ;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    MT_HVDC* mtdc = get_mt_hvdc_pointer();
    vector<DC_BUS*> buses = mtdc->get_all_dc_buses();
    unsigned int n_dcbus = buses.size();
    for(unsigned int i=0;i!=n_dcbus;i++)
    {
        osstream<<" bus current of dc bus" <<buses[i]->get_dc_bus_number()<<" is:"<<buses[i]->get_dc_bus_current_in_kA();
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    }
}

void MTDCGRID4::show_bus_injection_current()
{
    ostringstream osstream;
    osstream<<" dc bus injection current " ;
    show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    int n = injection_current.capacity();
    for(unsigned int i=0;i!=n;i++)
    {
        osstream<<" bus injection current of internal bus: " <<i<<" is:"<<injection_current[i];
        show_information_with_leading_time_stamp_with_default_toolkit(osstream);
    };
}
