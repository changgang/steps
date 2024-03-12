#include "header/apis/steps_api.h"
#include "header/basic/utility.h"
#include "header/steps_namespace.h"
#include "header/data_imexporter/psse_imexporter.h"
#include "header/data_imexporter/bpa_imexporter.h"

void api_show_device_data(const char* device_type, unsigned int toolkit_index)
{
    STEPS& toolkit = get_toolkit(toolkit_index);
    POWER_SYSTEM_DATABASE& psdb = toolkit.get_power_system_database();
    string DEVICE_TYPE = string2upper(device_type);

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="BUS")
    {
        vector<BUS*> buses = psdb.get_all_buses();
        unsigned int n = buses.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            BUS* busptr = buses[i];
            busptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="GENERATOR")
    {
        vector<GENERATOR*> gens = psdb.get_all_generators();
        unsigned int n = gens.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            GENERATOR* genptr = gens[i];
            genptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="WT GENERATOR")
    {
        vector<WT_GENERATOR*> sources = psdb.get_all_wt_generators();
        unsigned int n = sources.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            WT_GENERATOR* sourceptr = sources[i];
            sourceptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="LOAD")
    {
        vector<LOAD*> loads = psdb.get_all_loads();
        unsigned int n = loads.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            LOAD* loadptr = loads[i];
            loadptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="FIXED SHUNT")
    {
        vector<FIXED_SHUNT*> shunts = psdb.get_all_fixed_shunts();
        unsigned int n = shunts.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            FIXED_SHUNT* shuntptr = shunts[i];
            shuntptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="AC LINE")
    {
        vector<AC_LINE*> lines = psdb.get_all_ac_lines();
        unsigned int n = lines.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            AC_LINE* lineptr = lines[i];
            lineptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="TRANSFORMER")
    {
        vector<TRANSFORMER*> transes = psdb.get_all_transformers();
        unsigned int n = transes.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            TRANSFORMER* transptr = transes[i];
            transptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="2T LCC HVDC")
    {
        vector<LCC_HVDC2T*> hvdcs = psdb.get_all_2t_lcc_hvdcs();
        unsigned int n = hvdcs.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            LCC_HVDC2T* hvdcptr = hvdcs[i];
            hvdcptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="VSC HVDC")
    {
        vector<VSC_HVDC*> vschvdcs = psdb.get_all_vsc_hvdcs();
        unsigned int n = vschvdcs.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            VSC_HVDC* vschvdcptr = vschvdcs[i];
            vschvdcptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="EQUIVALENT DEVICE")
    {
        vector<EQUIVALENT_DEVICE*> eds = psdb.get_all_equivalent_devices();
        unsigned int n = eds.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            EQUIVALENT_DEVICE* edptr = eds[i];
            edptr->report();
        }
    }
    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="AREA")
    {
        vector<AREA*> areas = psdb.get_all_areas();
        unsigned int n = areas.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            AREA* areaptr = areas[i];
            areaptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="ZONE")
    {
        vector<ZONE*> zones = psdb.get_all_zones();
        unsigned int n = zones.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            ZONE* zoneptr = zones[i];
            zoneptr->report();
        }
    }

    if(DEVICE_TYPE == "ALL" or DEVICE_TYPE=="OWNER")
    {
        vector<OWNER*> owners = psdb.get_all_owners();
        unsigned int n = owners.size();

        for(unsigned int i=0; i!=n; ++i)
        {
            OWNER* ownerptr = owners[i];
            ownerptr->report();
        }
    }
}
