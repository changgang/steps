#include "header/basic/base.h"
#include <cstdlib>
BASE::BASE()
{
    set_power_system_database(NULL);
}

BASE::~BASE()
{
    ;
}


void BASE::set_power_system_database(POWER_SYSTEM_DATABASE* psdb)
{
    this->db = psdb;
}

POWER_SYSTEM_DATABASE* BASE::get_power_system_database() const
{
    return db;
}
