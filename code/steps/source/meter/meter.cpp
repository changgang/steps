#include "header/meter/meter.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include <map>
#include <vector>
#include <istream>
#include <iostream>
#include <cstring>

using namespace std;

METER::METER(STEPS& toolkit)
{
    this->toolkit = (&toolkit);
    clear();
}

void METER::copy_from_const_meter(const METER& meter)
{
    clear();
    this->toolkit = &(meter.get_toolkit());
    set_device_id(meter.get_device_id());
    set_meter_type(meter.get_meter_type());
    string meter_type = get_meter_type();
    if(meter_type.find("INTERNAL VARIABLE") != string::npos)
        set_internal_variable_name(meter.get_internal_variable_name());
    if(get_device_type()==STEPS_VSC_HVDC)
    {
        if(get_meter_type().rfind("CONVERTER", 0) == 0)
            set_metered_side_bus(meter.get_metered_side_bus());
        else
        {
            set_metered_side_bus(meter.get_metered_side_bus());
            if(get_meter_type().rfind("DC LINE", 0) == 0)
                set_sub_dc_device_id(meter.get_sub_dc_device_id());
        }
    }
    else
    {
        if(get_device_type()==STEPS_HYBRID_DC)
            set_sub_device_name(meter.get_sub_device_name());
        else
            set_metered_side_bus(meter.get_metered_side_bus());
    }
}

METER::METER(const METER& meter)
{
    copy_from_const_meter(meter);
}

STEPS& METER::get_toolkit() const
{
    return *toolkit;
}

METER& METER::operator=(const METER& meter)
{
    if(this==&meter)
        return *this;

    copy_from_const_meter(meter);

    return *this;
}


METER::~METER()
{
    ;
}

void METER::set_device_id(const DEVICE_ID& did)
{
    if(not did.is_valid())
    {
        ostringstream osstream;
        osstream<<"Warning. Device id is invalid for setting up meter.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
    set_device_pointer(did);
}

void METER::set_meter_type(string meter_type)
{
    ostringstream osstream;
    if(device_pointer!=NULL)
    {
        meter_type = string2upper(meter_type);

        if(is_valid_meter_type(meter_type))
        {
            change_meter_type(meter_type);
        }
        else
        {
            osstream<<"Warning. Invalid meter type '"<<meter_type<<"' is not supported for setting up meter type of "<<device_pointer->get_compound_device_name()<<".";
            toolkit->show_information_with_leading_time_stamp(osstream);
            change_meter_type("");
        }
    }
    else
    {
        osstream<<"Warning. Device pointer is NULL for setting up meter type. Set up proper device id first.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

void METER::set_internal_variable_name(string name)
{
    ostringstream osstream;
    if(device_pointer!=NULL)
    {
        string meter_type = get_meter_type();
        if(meter_type.find("INTERNAL VARIABLE") != string::npos)
        {
            if(is_internal_variable_name_valid(name))
            {
                change_meter_internal_variable_name(name);
            }
            else
            {
                osstream<<"Warning. Internal variable name '"<<name<<"' is not valid. No internal variable name will be set.";
                toolkit->show_information_with_leading_time_stamp(osstream);
                change_meter_internal_variable_name("");
            }
        }
        else
        {
            osstream<<"Warning. Meter type '"<<meter_type<<"' is not valid Internal Variable Meter. No internal variable name will be set.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            change_meter_internal_variable_name("");
        }
    }
    else
    {
        osstream<<"Warning. Device pointer is NULL for setting up meter internal variable name. Set up proper device id first.";
        toolkit->show_information_with_leading_time_stamp(osstream);
        return;
    }
}

bool METER::is_internal_variable_name_valid(string& name, unsigned int index) const
{
    if(get_device_pointer()!=NULL)
    {
        name = string2upper(name);
        string meter_type = get_meter_type();
        MODEL* model=NULL;
        STEPS_DEVICE_TYPE device_type = get_device_type();
        if(device_type==STEPS_GENERATOR)
        {
            GENERATOR* ptr = (GENERATOR*)get_device_pointer();
            if(meter_type=="SYNC GENERATOR MODEL INTERNAL VARIABLE")
                model = ptr->get_sync_generator_model();
            if(meter_type=="COMPENSATOR MODEL INTERNAL VARIABLE")
                model = ptr->get_compensator_model();
            if(meter_type=="EXCITER MODEL INTERNAL VARIABLE")
                model = ptr->get_exciter_model();
            if(meter_type=="STABILIZER MODEL INTERNAL VARIABLE")
                model = ptr->get_stabilizer_model();
            if(meter_type=="TURBINE GOVERNOR MODEL INTERNAL VARIABLE")
                model = ptr->get_turbine_governor_model();
            if(meter_type=="TURBINE LOAD CONTROLLER MODEL INTERNAL VARIABLE")
                model = ptr->get_turbine_load_controller_model();
        }
        if(device_type==STEPS_WT_GENERATOR)
        {
            WT_GENERATOR* ptr = (WT_GENERATOR*)get_device_pointer();
            if(meter_type=="WT GENERATOR MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_generator_model();
            if(meter_type=="WT AERODYNAMIC MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_aerodynamic_model();
            if(meter_type=="WT TURBINE MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_turbine_model();
            if(meter_type=="WT ELECTRICAL MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_electrical_model();
            if(meter_type=="WT PITCH MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_pitch_model();
            if(meter_type=="WT VRT MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_vrt_model();
            if(meter_type=="WT RELAY MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_relay_model();
            if(meter_type=="WT DBR MODEL INTERNAL VARIABLE")
                model = ptr->get_wt_dbr_model();
            if(meter_type=="WIND SPEED MODEL INTERNAL VARIABLE")
                model = ptr->get_wind_speed_model();
        }
        if(device_type==STEPS_PV_UNIT)
        {
            PV_UNIT* ptr = (PV_UNIT*)get_device_pointer();
            if(meter_type=="PV CONVERTER MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_converter_model();
            if(meter_type=="PV PANEL MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_panel_model();
            if(meter_type=="PV ELECTRICAL MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_electrical_model();
            if(meter_type=="PV IRRADIANCE MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_irradiance_model();
            if(meter_type=="PV TEMPERATURE MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_temperature_model();
            if(meter_type=="PV VRT MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_vrt_model();
            if(meter_type=="PV RELAY MODEL INTERNAL VARIABLE")
                model = ptr->get_pv_relay_model();
        }
        if(device_type==STEPS_ENERGY_STORAGE)
        {
            ENERGY_STORAGE* ptr = (ENERGY_STORAGE*)get_device_pointer();
            if(meter_type=="ES CONVERTER MODEL INTERNAL VARIABLE")
                model = ptr->get_es_converter_model();
            if(meter_type=="ES ELECTRICAL MODEL INTERNAL VARIABLE")
                model = ptr->get_es_electrical_model();
            if(meter_type=="ES SOC MODEL INTERNAL VARIABLE")
                model = ptr->get_es_soc_model();
            if(meter_type=="ES VRT MODEL INTERNAL VARIABLE")
                model = ptr->get_es_vrt_model();
            if(meter_type=="ES RELAY MODEL INTERNAL VARIABLE")
                model = ptr->get_es_relay_model();
            if(meter_type=="ES PRIMARY MODEL INTERNAL VARIABLE")
                model = ptr->get_es_primary_model();
            if(meter_type=="ES ROTOR MODEL INTERNAL VARIABLE")
                model = ptr->get_es_rotor_model();
            if(meter_type=="ES AUXILIARY MODEL INTERNAL VARIABLE")
                model = ptr->get_es_auxiliary_model();
        }
        if(device_type==STEPS_SV_SOURCE)
        {
            SV_SOURCE* ptr = (SV_SOURCE*)get_device_pointer();
            if(meter_type=="SVS CONTROL MODEL INTERNAL VARIABLE")
                model = ptr->get_svs_control_model();
        }
        if(device_type==STEPS_LOAD)
        {
            LOAD* ptr = (LOAD*)get_device_pointer();
            if(meter_type=="LOAD MODEL INTERNAL VARIABLE")
                model = ptr->get_load_model();
            if(meter_type=="FREQUENCY RELAY MODEL INTERNAL VARIABLE")
                model = ptr->get_load_frequency_relay_model();
            if(meter_type=="VOLTAGE RELAY MODEL INTERNAL VARIABLE")
                model = ptr->get_load_voltage_relay_model();
        }
        if(device_type==STEPS_LCC_HVDC2T)
        {
            LCC_HVDC2T* ptr = (LCC_HVDC2T*)get_device_pointer();
            if(meter_type=="2T LCC HVDC MODEL INTERNAL VARIABLE")
                model = ptr->get_lcc_hvdc2t_model();
        }
        if(device_type==STEPS_VSC_HVDC)
        {
            VSC_HVDC* ptr = (VSC_HVDC*)get_device_pointer();
            if(meter_type=="VSC HVDC NETWORK MODEL INTERNAL VARIABLE")
                model = ptr->get_vsc_hvdc_network_model();
            if(meter_type=="VSC HVDC CONVERTER MODEL INTERNAL VARIABLE")
                model = ptr->get_vsc_hvdc_converter_model(index);
        }
        if(device_type==STEPS_HYBRID_DC)
        {
            MT_HVDC* ptr = (MT_HVDC*)get_device_pointer();
            if(meter_type=="MT HVDC NETWORK MODEL INTERNAL VARIABLE")
                model = ptr->get_mtdc_network_model();
            if(meter_type=="MT HVDC LCC MODEL INTERNAL VARIABLE")
                model = ptr->get_lcc_model_with_index(index);
            if(meter_type=="MT HVDC VSC MODEL INTERNAL VARIABLE")
                model = ptr->get_vsc_model_with_index(index);
        }
        if(model!=NULL)
            return model->is_model_internal_variable_exist(name);
        else
        {
            ostringstream osstream;
            osstream<<"Warning. No dynamic model is found when check if internal variable name is valid.";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return false;
        }
    }
    else
        return false;
}

void METER::set_metered_side_bus(unsigned int meter_side)
{
    STEPS_DEVICE_TYPE dtype = get_device_type();
    string meter_type = get_meter_type();
    if(dtype == STEPS_BUS or dtype == STEPS_GENERATOR or dtype == STEPS_WT_GENERATOR
       or dtype == STEPS_PV_UNIT or dtype == STEPS_ENERGY_STORAGE or dtype == STEPS_SV_SOURCE
       or dtype == STEPS_LOAD or dtype == STEPS_FIXED_SHUNT)
        metered_side_bus = 0;
    else
    {
        if(dtype == STEPS_AC_LINE or dtype == STEPS_TRANSFORMER or dtype == STEPS_LCC_HVDC2T)
        {
            NONBUS_DEVICE* device = (NONBUS_DEVICE*) get_device_pointer();
            if(device->is_connected_to_bus(meter_side))
                metered_side_bus = meter_side;
            else
                metered_side_bus = 0;
        }
        else if(dtype == STEPS_VSC_HVDC)
        {
            VSC_HVDC* vsc_hvdc = (VSC_HVDC*) get_device_pointer();
            if(meter_type.starts_with("CONVERTER"))
            {
                if(vsc_hvdc->is_connected_to_bus(meter_side) or vsc_hvdc->is_connected_to_dc_bus(meter_side))
                    metered_side_bus = meter_side;
                else
                    metered_side_bus = 0;
            }
            else
            {
                if(meter_type.starts_with("DC BUS"))
                {
                    if(vsc_hvdc->is_connected_to_dc_bus(meter_side))
                        metered_side_bus = meter_side;
                    else
                        metered_side_bus = 0;
                }
                else
                {
                    if(meter_type.starts_with("DC LINE"))
                    {
                        if(vsc_hvdc->is_connected_to_dc_bus(meter_side))
                            metered_side_bus = meter_side;
                        else
                            metered_side_bus = 0;
                    }
                    else
                        metered_side_bus = 0;
                }
            }
        }
        else if(dtype == STEPS_HYBRID_DC)
        {
            MT_HVDC* mt_hvdc = (MT_HVDC*) get_device_pointer();
            if(meter_type.starts_with("LCC CONVERTER") or meter_type.starts_with("VSC CONVERTER"))
            {
                if(mt_hvdc->is_connected_to_bus(meter_side) or mt_hvdc->is_connected_to_dcbus(meter_side))
                    metered_side_bus = meter_side;
                else
                    metered_side_bus = 0;
            }
            else
            {
                if(meter_type.starts_with("DC BUS"))
                {
                    if(mt_hvdc->is_connected_to_dcbus(meter_side))
                        metered_side_bus = meter_side;
                    else
                        metered_side_bus = 0;
                }
                else
                {
                    if(meter_type.starts_with("DC LINE"))
                    {
                        if(mt_hvdc->is_connected_to_dcbus(meter_side))
                            metered_side_bus = meter_side;
                        else
                            metered_side_bus = 0;
                    }
                    else
                        metered_side_bus = 0;
                }
            }
        }
    }
}

void METER::set_sub_device_name(string name)
{
    if(get_device_type()==STEPS_HYBRID_DC)
    {
        string meter_type = get_meter_type();
        if(meter_type.starts_with("LCC CONVERTER") or meter_type.starts_with("VSC CONVERTER")
           or meter_type.starts_with("DC BUS") or meter_type.starts_with("DC LINE"))
            this->sub_device_name = name;
    }
}

void METER::set_sub_dc_device_id(DC_DEVICE_ID did)
{
    if(did.get_device_type()==STEPS_DC_LINE)
        sub_dc_did = did;
}

void METER::change_device_id(DEVICE_ID did)
{
    set_device_id(did);
}

void METER::change_meter_type(const string& meter_type)
{
    this->meter_type_string_index = get_index_of_string(meter_type);
    if(meter_type!="")
        update_meter_function();
}

void METER::change_meter_internal_variable_name(const string& name)
{
    this->internal_variable_name_index = get_index_of_string(name);
}

bool METER::is_valid_meter_type(string& meter_type) const
{
    if(device_pointer!=NULL)
    {
        STEPS_DEVICE_TYPE device_type = get_device_type();
        meter_type = string2upper(meter_type);

        return is_valid_meter_type_of_device(meter_type, device_type);
    }
    else
        return false;
}

bool METER::is_valid_meter_type_of_device(const string& meter_type, STEPS_DEVICE_TYPE device_type) const
{
    switch(device_type)
    {
        case STEPS_BUS:
            return bus_meter_map.find(meter_type) != bus_meter_map.end();
        case STEPS_GENERATOR:
            return sg_meter_map.find(meter_type) != sg_meter_map.end();
        case STEPS_WT_GENERATOR:
            return wtg_meter_map.find(meter_type) != wtg_meter_map.end();
        case STEPS_PV_UNIT:
            return pvu_meter_map.find(meter_type) != pvu_meter_map.end();
        case STEPS_ENERGY_STORAGE:
            return es_meter_map.find(meter_type) != es_meter_map.end();
        case STEPS_SV_SOURCE:
            return sv_source_meter_map.find(meter_type) != sv_source_meter_map.end();
        case STEPS_LOAD:
            return load_meter_map.find(meter_type) != load_meter_map.end();
        case STEPS_AC_LINE:
            return ac_line_meter_map.find(meter_type) != ac_line_meter_map.end();
        case STEPS_TRANSFORMER:
            return transformer_meter_map.find(meter_type) != transformer_meter_map.end();
        case STEPS_LCC_HVDC2T:
            return lcc_hvdc2t_meter_map.find(meter_type) != lcc_hvdc2t_meter_map.end();
        case STEPS_VSC_HVDC:
            return vsc_hvdc_meter_map.find(meter_type) != vsc_hvdc_meter_map.end();
        case STEPS_HYBRID_DC:
            return mt_hvdc_meter_map.find(meter_type) != mt_hvdc_meter_map.end();
        case STEPS_EQUIVALENT_DEVICE:
            return equivalent_device_meter_map.find(meter_type) != equivalent_device_meter_map.end();
        default:
        {
            ostringstream osstream;
            osstream<<"Warning. Device type "<<device_type<<" is invalid for setting meter type of "<<meter_type<<".";
            toolkit->show_information_with_leading_time_stamp(osstream);
            return false;
        }
    }
}

DEVICE_ID METER::get_device_id() const
{
    DEVICE_ID did;
    if(device_pointer!=NULL)
        return device_pointer->get_device_id();
    else
        return did;
}

string METER::get_meter_type() const
{
    if(meter_type_string_index!=INDEX_NOT_EXIST)
        return get_string_of_index(meter_type_string_index);
    else
        return "";
}

unsigned int METER::get_metered_side_bus() const
{
    return metered_side_bus;
}

DC_DEVICE_ID METER::get_sub_dc_device_id() const
{
    return sub_dc_did;
}

STEPS_DEVICE_TYPE METER::get_device_type() const
{
    return device_type;
}

string METER::get_sub_device_name() const
{
    return sub_device_name;
}

string METER::get_internal_variable_name() const
{
    if(internal_variable_name_index!=INDEX_NOT_EXIST)
        return get_string_of_index(internal_variable_name_index);
    else
        return "";
}

DEVICE* METER::get_device_pointer() const
{
    return device_pointer;
}

string METER::get_meter_name() const
{
    string meter_type = get_meter_type();
    if(meter_type!="")
    {
        string name;

        if(device_type==STEPS_LCC_HVDC2T)
        {
            LCC_HVDC2T* hvdc = (LCC_HVDC2T*) get_device_pointer();
            name = converter_side2string(hvdc->get_converter_side_at_bus(get_metered_side_bus())) + " ";
        }

        name += meter_type;
        if(meter_type.find("INTERNAL VARIABLE")!=string::npos)
            name += " " + get_internal_variable_name();

        STEPS_DEVICE_TYPE device_type = get_device_type();
        if(device_type==STEPS_VSC_HVDC and meter_type.rfind("DC LINE",0)==0)
            name += " @ "+get_sub_dc_device_id().get_compound_device_name();

        name += " @ "+get_device_id().get_compound_device_name();

        if(device_type==STEPS_AC_LINE or
           device_type==STEPS_TRANSFORMER or
           device_type==STEPS_LCC_HVDC2T or
           device_type==STEPS_VSC_HVDC)
            name += " @ SIDE "+num2str(get_metered_side_bus());
        if(device_type==STEPS_HYBRID_DC)
            name += " @ DC DEVICE "+ get_sub_device_name();

        return trim_string(name);
    }
    else
        return "";
}

bool METER::is_valid() const
{
    if(device_pointer!=NULL and get_meter_type()!="")
    {
        STEPS_DEVICE_TYPE device_type = get_device_type();
        if(device_type==STEPS_AC_LINE or
           device_type==STEPS_TRANSFORMER or
           device_type==STEPS_VSC_HVDC)
        {
            if(get_metered_side_bus()!=0)
                return true;
            else
                return false;
        }
        if(device_type==STEPS_HYBRID_DC)
        {
            if(get_sub_device_name()!="")
                return true;
            else
                return false;
        }
        return true;
    }
    else
        return false;
}

void METER::clear()
{
    device_pointer = NULL;
    sub_device_name = "";
    device_type = STEPS_INVALID_DEVICE;
    meter_type_string_index = INDEX_NOT_EXIST;
    metered_side_bus = 0;
    internal_variable_name_index = INDEX_NOT_EXIST;
    sub_dc_did.clear();

    meter_function = nullptr;
}


bool METER::operator==(const METER& meter)
{
    if(this->get_device_id() == meter.get_device_id() and
       this->get_meter_type() == meter.get_meter_type())
    {
        if(this->get_meter_type().find("INTERNAL VARIABLE") == string::npos)
            return true;
        else
        {
            if(this->get_internal_variable_name() == meter.get_internal_variable_name())
                return true;
            else
                return false;
        }
    }
    else
        return false;
}

bool METER::operator!=(const METER& meter)
{
    return not ((*this)==meter);
}

void METER::set_device_pointer(DEVICE_ID device_id)
{
    if(not device_id.is_valid())
    {
        device_pointer = NULL;
        return;
    }
    POWER_SYSTEM_DATABASE& psdb = toolkit->get_power_system_database();

    device_type = device_id.get_device_type();
    DEVICE* deviceptr = NULL;
    switch(device_type)
    {
        case STEPS_BUS:
        {
            TERMINAL terminal = device_id.get_device_terminal();
            vector<unsigned int> tbuses = terminal.get_buses();
            deviceptr = (DEVICE*) psdb.get_bus(tbuses[0]);
            break;
        }
        case STEPS_GENERATOR:
            deviceptr = (DEVICE*) psdb.get_generator(device_id);
            break;
        case STEPS_WT_GENERATOR:
            deviceptr = (DEVICE*) psdb.get_wt_generator(device_id);
            break;
        case STEPS_PV_UNIT:
            deviceptr = (DEVICE*) psdb.get_pv_unit(device_id);
            break;
        case STEPS_ENERGY_STORAGE:
            deviceptr = (DEVICE*) psdb.get_energy_storage(device_id);
            break;
        case STEPS_SV_SOURCE:
            deviceptr = (DEVICE*) psdb.get_sv_source(device_id);
            break;
        case STEPS_LOAD:
            deviceptr = (DEVICE*) psdb.get_load(device_id);
            break;
        case STEPS_AC_LINE:
            deviceptr = (DEVICE*) psdb.get_ac_line(device_id);
            break;
        case STEPS_TRANSFORMER:
            deviceptr = (DEVICE*) psdb.get_transformer(device_id);
            break;
        case STEPS_LCC_HVDC2T:
            deviceptr = (DEVICE*) psdb.get_lcc_hvdc2t(device_id);
            break;
        case STEPS_VSC_HVDC:
            deviceptr = (DEVICE*) psdb.get_vsc_hvdc(device_id);
            break;
        case STEPS_HYBRID_DC:
            deviceptr = (DEVICE*) psdb.get_mt_hvdc(device_id);
            break;
        case STEPS_EQUIVALENT_DEVICE:
            deviceptr = (DEVICE*) psdb.get_equivalent_device(device_id);
            break;
        default:
            break;
    }
    device_pointer = deviceptr;
    if(deviceptr==NULL)
    {
        ostringstream osstream;
        osstream<<"Device cannot be found in database for setting meter. Device is "<<device_id.get_compound_device_name();
        toolkit->show_information_with_leading_time_stamp(osstream);
    }
}

double METER::get_meter_value() const
{
    METER_PARAMETER para;
    para.device_pointer = get_device_pointer();
    para.sub_device_name = get_sub_device_name();
    para.metered_side_bus = get_metered_side_bus();
    para.internal_var_name = get_internal_variable_name();
    para.sub_dc_did = get_sub_dc_device_id();
    return meter_function(para);
}

void METER::update_meter_function()
{
    switch(get_device_type())
    {
        case STEPS_BUS:
            update_meter_function_as_a_bus();
            break;
        case STEPS_GENERATOR:
            update_meter_function_as_a_generator();
            break;
        case STEPS_WT_GENERATOR:
            update_meter_function_as_a_wt_generator();
            break;
        case STEPS_PV_UNIT:
            update_meter_function_as_a_pv_unit();
            break;
        case STEPS_ENERGY_STORAGE:
            update_meter_function_as_an_energy_storage();
            break;
        case STEPS_SV_SOURCE:
            update_meter_function_as_a_sv_source();
            break;
        case STEPS_LOAD:
            update_meter_function_as_a_load();
            break;
        case STEPS_AC_LINE:
            update_meter_function_as_an_ac_line();
            break;
        case STEPS_TRANSFORMER:
            update_meter_function_as_a_transformer();
            break;
        case STEPS_LCC_HVDC2T:
            update_meter_function_as_a_lcc_hvdc2t();
            break;
        case STEPS_VSC_HVDC:
            update_meter_function_as_a_vsc_hvdc();
            break;
        case STEPS_HYBRID_DC:
            update_meter_function_as_a_mt_hvdc();
            break;
        case STEPS_EQUIVALENT_DEVICE:
            update_meter_function_as_an_equivalent_device();
            break;
        default:
            break;
    }
}

void METER::update_meter_function_as_a_bus()
{
    update_meter_function_with_meter_function_map(bus_meter_map);
}

void METER::update_meter_function_as_a_generator()
{
    update_meter_function_with_meter_function_map(sg_meter_map);
}

void METER::update_meter_function_as_a_wt_generator()
{
    update_meter_function_with_meter_function_map(wtg_meter_map);
}

void METER::update_meter_function_as_a_pv_unit()
{
    update_meter_function_with_meter_function_map(pvu_meter_map);
}

void METER::update_meter_function_as_an_energy_storage()
{
    update_meter_function_with_meter_function_map(es_meter_map);
}

void METER::update_meter_function_as_a_sv_source()
{
    update_meter_function_with_meter_function_map(sv_source_meter_map);
}

void METER::update_meter_function_as_a_load()
{
    update_meter_function_with_meter_function_map(load_meter_map);
}

void METER::update_meter_function_as_an_ac_line()
{
    update_meter_function_with_meter_function_map(ac_line_meter_map);
}

void METER::update_meter_function_as_a_transformer()
{
    update_meter_function_with_meter_function_map(transformer_meter_map);
}

void METER::update_meter_function_as_a_lcc_hvdc2t()
{
    update_meter_function_with_meter_function_map(lcc_hvdc2t_meter_map);
}

void METER::update_meter_function_as_a_vsc_hvdc()
{
    update_meter_function_with_meter_function_map(vsc_hvdc_meter_map);
}

void METER::update_meter_function_as_a_mt_hvdc()
{
    update_meter_function_with_meter_function_map(mt_hvdc_meter_map);
}

void METER::update_meter_function_as_an_equivalent_device()
{
    update_meter_function_with_meter_function_map(equivalent_device_meter_map);
}

void METER::update_meter_function_with_meter_function_map(unordered_map<string, function<double(METER_PARAMETER)>>& meter_map)
{
    unordered_map<string, function<double(METER_PARAMETER)>>::const_iterator it = meter_map.find(get_meter_type());
    if (it != meter_map.end())
        meter_function = it->second;
}
