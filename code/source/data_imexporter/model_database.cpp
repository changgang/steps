#include "header/model_database.h"
#include "header/basic/utility.h"
#include "header/model/all_models.h"

MODEL_DATABASE::MODEL_DATABASE()
{
    clear_database();
}

MODEL_DATABASE::~MODEL_DATABASE()
{
    clear_database();
}

void MODEL_DATABASE::clear_database()
{
    size_t n = get_model_table_count();
    for(size_t i=0; i<n; i++)
    {
        if(models[i]!=NULL)
        {
            delete models[i];
            models[i]=NULL;
        }
    }
    models.clear();
}

size_t MODEL_DATABASE::get_model_table_count() const
{
    return models.size();
}

size_t MODEL_DATABASE::get_valid_model_count() const
{
    size_t n= get_model_table_count();
    size_t n_NULL = 0;
    for(size_t i=0; i<n; i++)
        if(get_model_of_index(i)==NULL)
            n_NULL++;

    return n-n_NULL;
}

void MODEL_DATABASE::append_model(MODEL* model)
{
    POWER_SYSTEM_DATABASE* db = model->get_power_system_database();
    DEVICE* ptr = model->get_device_pointer();

    if(db==NULL or ptr==NULL)
        return;

    size_t model_index = get_model_table_count();
    models.push_back(model);

    string model_type = model->get_model_type();
    DEVICE_ID did = ptr->get_device_id();
    string device_type = did.get_device_type();

    if(device_type=="GENERATOR")
        set_generator_related_model_index((GENERATOR*) ptr, model_type, model_index);
    if(device_type=="WIND TURBINE GENERATOR")
        set_wind_turbine_generator_related_model_index((WIND_TURBINE_GENERATOR*) ptr, model_type, model_index);
    if(device_type=="LOAD")
        set_load_related_model_index((LOAD*) ptr, model_type, model_index);
    if(device_type=="HVDC")
        set_hvdc_related_model_index((HVDC*) ptr, model_type, model_index);

}
void MODEL_DATABASE::append_model2(const MODEL& model)
{
    if(not model.is_valid())
        return;

    DEVICE_ID did = model.get_device_id();
    string device_type = did.get_device_type();

    if(device_type=="GENERATOR")
        append_generator_related_model(model);
    if(device_type=="WIND TURBINE GENERATOR")
        append_wind_turbine_generator_related_model(model);
    if(device_type=="LOAD")
        append_load_related_model(model);
    if(device_type=="HVDC")
        append_hvdc_related_model(model);
}


void MODEL_DATABASE::append_generator_related_model(const MODEL& model)
{
    if(model.get_device_id().get_device_type()!="GENERATOR")
        return;

    string model_type = model.get_model_type();

    if(model_type=="SYNC GENERATOR")
    {
        SYNC_GENERATOR_MODEL* smodel = (SYNC_GENERATOR_MODEL*) (&model);
        append_sync_generator_model(*smodel);
        return;
    }
    if(model_type=="COMPENSATOR")
    {
        COMPENSATOR_MODEL* smodel = (COMPENSATOR_MODEL*) (&model);
        append_compensator_model(*smodel);
        return;
    }
    if(model_type=="EXCITER")
    {
        EXCITER_MODEL* smodel = (EXCITER_MODEL*) (&model);
        append_exciter_model(*smodel);
        return;
    }
    if(model_type=="STABILIZER")
    {
        STABILIZER_MODEL* smodel = (STABILIZER_MODEL*) (&model);
        append_stabilizer_model(*smodel);
        return;
    }
    if(model_type=="TURBINE GOVERNOR")
    {
        TURBINE_GOVERNOR_MODEL* smodel = (TURBINE_GOVERNOR_MODEL*) (&model);
        append_turbine_governor_model(*smodel);
        return;
    }

    char info[256];
    sprintf(info, "Warning. Model type '%s' is not supported when append model of %s.",
                   model_type.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_sync_generator_model(const SYNC_GENERATOR_MODEL& model)
{
    if(model.get_device_id().get_device_type()!="GENERATOR")
        return;

    if(model.get_model_type()!="SYNC GENERATOR")
        return;

    GENERATOR* generator = (GENERATOR*) model.get_device_pointer();

    SYNC_GENERATOR_MODEL* old_model = generator->get_sync_generator_model();
    if(old_model!=NULL)
    {
        delete old_model;
        generator->set_sync_generator_model(NULL);
    }

    string model_name = model.get_model_name();
    if(model_name=="GENCLS")
    {
        GENCLS* smodel = (GENCLS*) (&model);
        GENCLS *new_model = new GENCLS(*smodel);
        generator->set_sync_generator_model(new_model);
        return;
    }
    if(model_name=="GENROU")
    {
        GENROU* smodel = (GENROU*) (&model);
        GENROU *new_model = new GENROU(*smodel);
        generator->set_sync_generator_model(new_model);
        return;
    }
    if(model_name=="GENSAL")
    {
        GENSAL* smodel = (GENSAL*) (&model);
        GENSAL *new_model = new GENSAL(*smodel);
        generator->set_sync_generator_model(new_model);
        return;
    }

    char info[256];
    sprintf(info, "Warning. Model '%s' is not supported when append sync generator model of %s.",
                   model_name.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_compensator_model(const COMPENSATOR_MODEL& model)
{
    if(model.get_device_id().get_device_type()!="GENERATOR")
        return;

    if(model.get_model_type()!="COMPENSATOR")
        return;

    GENERATOR* generator = (GENERATOR*) model.get_device_pointer();

    COMPENSATOR_MODEL* old_model = generator->get_compensator_model();
    if(old_model!=NULL)
    {
        delete old_model;
        generator->set_compensator_model(NULL);
    }

    string model_name = model.get_model_name();
    if(model_name=="COMP")
    {
        COMP* smodel = (COMP*) (&model);
        COMP *new_model = new COMP(*smodel);
        generator->set_compensator_model(new_model);
        return;
    }
    char info[256];
    sprintf(info, "Warning. Model '%s' is not supported when append compensator model of %s.",
                   model_name.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_exciter_model(const EXCITER_MODEL& model)
{
    if(model.get_device_id().get_device_type()!="GENERATOR")
        return;

    if(model.get_model_type()!="EXCITER")
        return;

    GENERATOR* generator = (GENERATOR*) model.get_device_pointer();

    EXCITER_MODEL* old_model = generator->get_exciter_model();
    if(old_model!=NULL)
    {
        delete old_model;
        generator->set_exciter_model(NULL);
    }

    string model_name = model.get_model_name();
    if(model_name=="SEXS")
    {
        SEXS* smodel = (SEXS*) (&model);
        SEXS *new_model = new SEXS(*smodel);
        generator->set_exciter_model(new_model);
        return;
    }
    char info[256];
    sprintf(info, "Warning. Model '%s' is not supported when append exciter model of %s.",
                   model_name.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_stabilizer_model(const STABILIZER_MODEL& model)
{
    if(model.get_device_id().get_device_type()!="GENERATOR")
        return;

    if(model.get_model_type()!="STABILIZER")
        return;

    GENERATOR* generator = (GENERATOR*) model.get_device_pointer();

    STABILIZER_MODEL* old_model = generator->get_stabilizer_model();
    if(old_model!=NULL)
    {
        delete old_model;
        generator->set_stabilizer_model(NULL);
    }

    string model_name = model.get_model_name();
    if(model_name=="IEE2ST")
    {
        IEE2ST* smodel = (IEE2ST*) (&model);
        IEE2ST *new_model = new IEE2ST(*smodel);
        generator->set_stabilizer_model(new_model);
        return;
    }
    char info[256];
    sprintf(info, "Warning. Model '%s' is not supported when append stabilizer model of %s.",
                   model_name.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_turbine_governor_model(const TURBINE_GOVERNOR_MODEL& model)
{
    if(model.get_device_id().get_device_type()!="GENERATOR")
        return;

    if(model.get_model_type()!="TURBINE GOVERNOR")
        return;

    GENERATOR* generator = (GENERATOR*) model.get_device_pointer();

    TURBINE_GOVERNOR_MODEL* old_model = generator->get_turbine_governor_model();
    if(old_model!=NULL)
    {
        delete old_model;
        generator->set_turbine_governor_model(NULL);
    }

    string model_name = model.get_model_name();
    if(model_name=="TGOV1")
    {
        TGOV1* smodel = (TGOV1*) (&model);
        TGOV1 *new_model = new TGOV1(*smodel);
        generator->set_turbine_governor_model(new_model);
        return;
    }
    char info[256];
    sprintf(info, "Warning. Model '%s' is not supported when append turbine governor model of %s.",
                   model_name.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}



void MODEL_DATABASE::append_load_related_model(const MODEL& model)
{
    if(model.get_device_id().get_device_type()!="LOAD")
        return;

    string model_type = model.get_model_type();

    if(model_type=="LOAD CHARACTERISTICS")
    {
        LOAD_MODEL* smodel = (LOAD_MODEL*) (&model);
        append_load_model(*smodel);
        return;
    }
    if(model_type=="LAOD VOLTAGE RELAY")
    {
        LOAD_VOLTAGE_RELAY_MODEL* smodel = (LOAD_VOLTAGE_RELAY_MODEL*) (&model);
        append_load_voltage_relay_model(*smodel);
        return;
    }
    if(model_type=="LOAD FREQUENCY RELAY")
    {
        LOAD_FREQUENCY_RELAY_MODEL* smodel = (LOAD_FREQUENCY_RELAY_MODEL*) (&model);
        append_load_frequency_relay_model(*smodel);
        return;
    }
    char info[256];
    sprintf(info, "Warning. Model type '%s' is not supported when append model of %s.",
                   model_type.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_load_model(const LOAD_MODEL& model)
{
    if(model.get_device_id().get_device_type()!="LOAD")
        return;

    if(model.get_model_type()!="LOAD CHARACTERISTICS")
        return;

    LOAD* load = (LOAD*) model.get_device_pointer();

    LOAD_MODEL* old_model = load->get_load_model();
    if(old_model!=NULL)
    {
        delete old_model;
        load->set_load_model(NULL);
    }

    string model_name = model.get_model_name();
    if(model_name=="IEEL")
    {
        IEEL* smodel = (IEEL*) (&model);
        IEEL *new_model = new IEEL(*smodel);
        load->set_load_model(new_model);
        return;
    }

    char info[256];
    sprintf(info, "Warning. Model '%s' is not supported when append sync generator model of %s.",
                   model_name.c_str(), model.get_device_id().get_device_name().c_str());
    show_information_with_leading_time_stamp(info);
}

void MODEL_DATABASE::append_load_voltage_relay_model(const LOAD_VOLTAGE_RELAY_MODEL& model)
{
    ;
}
void MODEL_DATABASE::append_load_frequency_relay_model(const LOAD_FREQUENCY_RELAY_MODEL& model)
{
    ;
}



void MODEL_DATABASE::append_wind_turbine_generator_related_model(const MODEL& model)
{
    ;
}
void MODEL_DATABASE::append_hvdc_related_model(const MODEL& model)
{
    ;
}

void MODEL_DATABASE::set_generator_related_model_index(GENERATOR* device, string model_type, size_t model_index)
{
    char info[256];

    if(model_index==INDEX_NOT_EXIST)
    {
        sprintf(info, "Warning. Generator related model index is INDEX_NOT_EXIST. No model index will be set.");
        show_information_with_leading_time_stamp(info);
        return;
    }

    if(model_type=="SYNC GENERATOR")
    {
        device->set_sync_generator_model_index(model_index);
        return;
    }

    if(model_type=="COMPENSATOR")
    {
        device->set_compensator_model_index(model_index);
        return;
    }

    if(model_type=="EXCITER")
    {
        device->set_exciter_model_index(model_index);
        return;
    }

    if(model_type=="STABILIZER")
    {
        device->set_stabilizer_model_index(model_index);
        return;
    }

    if(model_type=="TURBINE GOVERNOR")
    {
        device->set_turbine_governor_model_index(model_index);
        return;
    }

    sprintf(info, "Warning. Model type '%s' is not supported for generator related model. No model index will be set",
            model_type.c_str());
    show_information_with_leading_time_stamp(info);
    return;

}
void MODEL_DATABASE::set_wind_turbine_generator_related_model_index(WIND_TURBINE_GENERATOR* device, string model_type, size_t model_index)
{
    ;
}

void MODEL_DATABASE::set_load_related_model_index(LOAD* device, string model_type, size_t model_index)
{
    char info[256];

    if(model_index==INDEX_NOT_EXIST)
    {
        sprintf(info, "Warning. Load related model index is INDEX_NOT_EXIST. No model index will be set.");
        show_information_with_leading_time_stamp(info);
        return;
    }

    if(model_type=="LOAD CHARACTERISTICS")
    {
        device->set_load_model_index(model_index);
        return;
    }

    if(model_type=="LOAD VOLTAGE RELAY")
    {
        device->set_load_voltage_relay_model_index(model_index);
        return;
    }

    if(model_type=="LOAD FREQUENCY RELAY")
    {
        device->set_load_frequency_relay_model_index(model_index);
        return;
    }

    sprintf(info, "Warning. Model type '%s' is not supported for load related model. No model index will be set",
            model_type.c_str());
    show_information_with_leading_time_stamp(info);
    return;
}

void MODEL_DATABASE::set_hvdc_related_model_index(HVDC* device, string model_type, size_t model_index)
{
    ;
}

MODEL* MODEL_DATABASE::get_model_of_index(size_t index) const
{
    if(index==INDEX_NOT_EXIST or index>=get_model_table_count())
        return NULL;
    else
        return models[index];
}

void MODEL_DATABASE::delete_model_of_index(size_t index)
{
    MODEL* model = get_model_of_index(index);
    if(model==NULL)
        return;
    else
    {
        delete model;
        models[index] = NULL;
    }
}

void MODEL_DATABASE::set_all_models_initialize_flag_as_false()
{
    size_t n = get_model_table_count();
    MODEL* model;
    for(size_t i=0; i<n; i++)
    {
        model = models[i];
        if(model==NULL)
            continue;
        else
        {
            model->set_flag_model_initialized_as_false();
        }
    }
}

void MODEL_DATABASE::set_all_models_initialize_flag_as_true()
{
    size_t n = get_model_table_count();
    MODEL* model;
    for(size_t i=0; i<n; i++)
    {
        model = models[i];
        if(model==NULL)
            continue;
        else
        {
            model->set_flag_model_initialized_as_true();
        }
    }
}

void MODEL_DATABASE::set_all_models_update_flag_as_false()
{
    size_t n = get_model_table_count();
    MODEL* model;
    for(size_t i=0; i<n; i++)
    {
        model = models[i];
        if(model==NULL)
            continue;
        else
        {
            model->set_flag_model_updated_as_false();
        }
    }
}

void MODEL_DATABASE::set_all_models_update_flag_as_true()
{
    size_t n = get_model_table_count();
    MODEL* model;
    for(size_t i=0; i<n; i++)
    {
        model = models[i];
        if(model==NULL)
            continue;
        else
        {
            model->set_flag_model_updated_as_true();
        }
    }
}

void MODEL_DATABASE::initialize_all_models()
{
    size_t n = get_model_table_count();
    MODEL* model;
    for(size_t i=0; i<n; i++)
    {
        model = models[i];
        if(model!=NULL)
        {
            model->initialize();
        }
    }
}


void MODEL_DATABASE::run_all_models(DYNAMIC_MODE mode)
{
    size_t n = get_model_table_count();
    MODEL* model;
    for(size_t i=0; i<n; i++)
    {
        model = models[i];
        if(model!=NULL)
        {
            model->run(mode);
        }
    }
}


