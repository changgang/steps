#include "header/data_imexporter/data_imexporter.h"
#include <cstdio>
#include "header/basic/utility.h"
DATA_IMEXPORTER::DATA_IMEXPORTER()
{
    db = NULL;
}
DATA_IMEXPORTER::~DATA_IMEXPORTER()
{
    ;
}

void DATA_IMEXPORTER::set_power_system_database(POWER_SYSTEM_DATABASE* psdb)
{
    this->db = psdb;
}

POWER_SYSTEM_DATABASE* DATA_IMEXPORTER::get_power_system_database() const
{
    return db;
}

bool DATA_IMEXPORTER::is_power_system_database_set() const
{
    if(get_power_system_database()!=NULL)
        return true;
    else
    {
        ostringstream sstream;
        sstream<<"Error. Data imexporter is not connected to any target power system database."<<endl
          <<"No operation on the imexporter will work.";
        show_information_with_leading_time_stamp(sstream);
        return false;
    }
}

