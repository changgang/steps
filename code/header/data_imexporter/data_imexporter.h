#ifndef DATA_IMEXPORTER_H
#define DATA_IMEXPORTER_H

#include "header/power_system_database.h"
#include <string>
#include <fstream>

using namespace std;
class DATA_IMEXPORTER
{
    public:
        DATA_IMEXPORTER();
        virtual ~DATA_IMEXPORTER();

        void set_power_system_database(POWER_SYSTEM_DATABASE* db);
        POWER_SYSTEM_DATABASE* get_power_system_database() const;

        bool is_power_system_database_set() const;

        virtual void load_powerflow_data(string pf_source) = 0;
        virtual void load_dynamic_data(string dy_source) = 0;
        virtual void load_sequence_data(string sq_source) = 0;

        virtual void export_powerflow_data(string file) = 0;
        virtual void export_dynamic_data(string file) = 0;
        virtual void export_sequence_data(string file) = 0;
    private:
        POWER_SYSTEM_DATABASE* db;
};

#endif // DATA_IMEXPORTER_H
