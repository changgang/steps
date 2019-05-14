#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_show_device_data(const char* device_type, size_t toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="BUS")
    {
        vector<BUS*> buses = psdb.get_all_buses();
        size_t n = buses.size();

        for(size_t i=0; i!=n; ++i)
        {
            BUS* busptr = buses[i];
            busptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="GENERATOR")
    {
        vector<GENERATOR*> gens = psdb.get_all_generators();
        size_t n = gens.size();

        for(size_t i=0; i!=n; ++i)
        {
            GENERATOR* genptr = gens[i];
            genptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="WT GENERATOR")
    {
        vector<WT_GENERATOR*> sources = psdb.get_all_wt_generators();
        size_t n = sources.size();

        for(size_t i=0; i!=n; ++i)
        {
            WT_GENERATOR* sourceptr = sources[i];
            sourceptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="LOAD")
    {
        vector<LOAD*> loads = psdb.get_all_loads();
        size_t n = loads.size();

        for(size_t i=0; i!=n; ++i)
        {
            LOAD* loadptr = loads[i];
            loadptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="FIXED SHUNT")
    {
        vector<FIXED_SHUNT*> shunts = psdb.get_all_fixed_shunts();
        size_t n = shunts.size();

        for(size_t i=0; i!=n; ++i)
        {
            FIXED_SHUNT* shuntptr = shunts[i];
            shuntptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="LINE")
    {
        vector<LINE*> lines = psdb.get_all_lines();
        size_t n = lines.size();

        for(size_t i=0; i!=n; ++i)
        {
            LINE* lineptr = lines[i];
            lineptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="TRANSFORMER")
    {
        vector<TRANSFORMER*> transes = psdb.get_all_transformers();
        size_t n = transes.size();

        for(size_t i=0; i!=n; ++i)
        {
            TRANSFORMER* transptr = transes[i];
            transptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="HVDC")
    {
        vector<HVDC*> hvdcs = psdb.get_all_hvdcs();
        size_t n = hvdcs.size();

        for(size_t i=0; i!=n; ++i)
        {
            HVDC* hvdcptr = hvdcs[i];
            hvdcptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        vector<EQUIVALENT_DEVICE*> eds = psdb.get_all_equivalent_devices();
        size_t n = eds.size();

        for(size_t i=0; i!=n; ++i)
        {
            EQUIVALENT_DEVICE* edptr = eds[i];
            edptr->report();
        }
    }
    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="AREA")
    {
        vector<AREA*> areas = psdb.get_all_areas();
        size_t n = areas.size();

        for(size_t i=0; i!=n; ++i)
        {
            AREA* areaptr = areas[i];
            areaptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="ZONE")
    {
        vector<ZONE*> zones = psdb.get_all_zones();
        size_t n = zones.size();

        for(size_t i=0; i!=n; ++i)
        {
            ZONE* zoneptr = zones[i];
            zoneptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="OWNER")
    {
        vector<OWNER*> owners = psdb.get_all_owners();
        size_t n = owners.size();

        for(size_t i=0; i!=n; ++i)
        {
            OWNER* ownerptr = owners[i];
            ownerptr->report();
        }
    }
}
